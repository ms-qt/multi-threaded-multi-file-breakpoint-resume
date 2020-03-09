//
// Created by 毛华伟 on 2020/3/9.
//

#include "DownloadFileManagerModel.h"

DownloadFileManagerModel::DownloadFileManagerModel() {
  if (QSqlDatabase::database().tables().contains(__TABLE_NAME__)) {
    qDebug() << __TABLE_NAME__ << " 连接成功";
  } else {
    qDebug() << __TABLE_NAME__ << " 连接失败 ";
    qDebug() << " 创建消息表 ";
    QSqlQuery sqlQuery;
    if (sqlQuery.exec(__CREATE_TABLE__)) {
      qDebug() << __TABLE_NAME__ << "  创建成功";
    } else {
      qDebug() << __TABLE_NAME__ << "  创建失败" << sqlQuery.lastError().text();
    }
  }

  manager = new QNetworkAccessManager;

  // 初始化的意思
  dowmloadFileTaskModel = new DowmloadFileTaskModel();
}

void DownloadFileManagerModel::addDownloadTask(QString type, QString url) {

  // 1.  检查url 任务是否已经存在
  //    存在，检查是否下载完成，
  //    不存在 添加下载任务

  if (!checkTaskExists(url)) {

    this->url = url;

    downloadTempDir = QDir::currentPath() + "/downloadtemp";

    QDir tDir = QDir(downloadTempDir);
    if (!tDir.exists()) {
      tDir.mkdir(tDir.path());
    }

    if (type == PLUGIN_QML) {
      fullFilePath = QDir::currentPath() + "/plugins/qmls";
    }

    if (type == PLUGIN_WEB) {
      fullFilePath = QDir::currentPath() + "/plugins/webs";
    }

    QNetworkRequest request(url);

    // request.setRawHeader("Accept-Encoding","identity");
    // 加入下面参数，可以获取文件大小
    // gzip
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    reply = manager->head(request);
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this,
            SLOT(downloadProgress(qint64, qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(finish()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(error(QNetworkReply::NetworkError)));

  } else {
    qDebug() << "任务已经存在";

    // 如果任务已经完成 ，删除
    if (checkTaskFinish(url)) {

    } else {
      // 没有完成 继续下载
    }
  }
}

bool DownloadFileManagerModel::checkTaskExists(QString url) {

  QString sql = "select _url from  _download_info where _url='" + url + "'";
  qDebug() << "检查任务是否存在 sql : " << sql;

  bool exists = false;

  if (sqlQuery.exec(sql)) {
    qDebug() << "查询成功";

    QString t_url;

    while (sqlQuery.next()) {
      t_url = sqlQuery.value(0).toString();
      if (t_url != nullptr || !"" == t_url.trimmed()) {
        exists = true;
      }
    }
  } else {
    qDebug() << "查询失败" << sqlQuery.lastError();
  }

  return exists;
}

bool DownloadFileManagerModel::checkTaskFinish(QString url) { return false; }

void DownloadFileManagerModel::downloadProgress(qint64 bytesReceived,
                                                qint64 bytesTotal) {}

QString DownloadFileManagerModel::getNameByUrl(QString url) {
  return url.mid(url.lastIndexOf("/") + 1, url.size());
}

void DownloadFileManagerModel::finish() {

  // 文件大小，字符串
  QString fileSize;
  for (QByteArray it : reply->rawHeaderList()) {
    qDebug() << it + " : " + reply->rawHeader(it);
  }
  if (reply->hasRawHeader(QString("Content-Length").toUtf8())) {
    qDebug() << "含有 Content-Length";
    fileSize = reply->rawHeader(QString("Content-Length").toUtf8());
  } else {
    qDebug() << "没有 Content-Length";
  }
  qDebug() << "文件大小" << fileSize;

  // 文件大小
  int file_length = fileSize.toInt();
  // 文件名称
  QString file_name = getNameByUrl(url);

  qDebug() << "文件名称 : " << file_name;

  // 完整的路径
  fullFilePath = fullFilePath + "/" + file_name;

  qDebug() << "完整路径 : " << fullFilePath;

  // 下载线程数量
  int thread_count = 1;

  // 大于100M开启多线程下载
  if (file_length > 1024 * 1024 * 100) {
    thread_count = (int)file_length % MAX_THREAD_COUNT;
  }
  qDebug() << "线程数 : " << thread_count;
  // 求余数
  int remainder = file_length % thread_count;
  qDebug() << "余数 : " << remainder;
  // 最后一快大小
  int laster_block_size = file_length % MAX_THREAD_COUNT;
  // 去除最后一块，剩下的可以整除
  int ave_length = file_length - laster_block_size;
  qDebug() << "length1 : " << ave_length;
  // 平均每个线程下载的大小
  int t_size = ave_length / thread_count;
  qDebug() << "每一个线程下载大小 : " << t_size;
  qDebug() << "最后一块大小 : " << laster_block_size;

  // 下载的基本信息存入数据库
  QString download_info_base_sql =
      "insert into _download_info(_url, _file_name, _download_save_dir, "
      "_full_save_path, _file_length, _thread_count, _status)\n"
      "values (:_url, :_file_name, :_download_save_dir, :_full_save_path, "
      ":_file_length, :_thread_count, :_status)";

  sqlQuery.prepare(download_info_base_sql);

  sqlQuery.bindValue(":_url", url);
  sqlQuery.bindValue(":_file_name", file_name);
  sqlQuery.bindValue(":_download_save_dir", downloadTempDir);
  sqlQuery.bindValue(":_full_save_path", fullFilePath);
  sqlQuery.bindValue(":_file_length", file_length);
  sqlQuery.bindValue(":_thread_count", thread_count);
  sqlQuery.bindValue(":_status", DOWNLOAD_NOT_START);

  if (sqlQuery.exec()) {
    qDebug() << "下载的基本信息存入数据库插入成功";

  } else {
    qDebug() << "下载的基本信息存入数据库插入成功" << sqlQuery.lastError();
  }

  // 循环线程
  for (int i = 0; i < thread_count; i++) {

    int start_position = i * t_size + 1;
    int end_position = t_size * (i+1);

    QString thread_id = file_name + QString::number(i + 1);

    QString update_download_thread_info =
        "update _download_info set _thread_id_" + QString::number(i + 1) +
        " = '" + thread_id + "' where _url='" + url + "'";

    if (sqlQuery.exec(update_download_thread_info)) {
      qDebug() << "线程信息更新成功" << sqlQuery.lastError();
    } else {
      qDebug() << "线程信息更新失败" << sqlQuery.lastError();
    }

    QString download_task_sql =
        "insert into _download_task_info(\n"
        "                                _url, \n"
        "                                _thread_id,\n"
        "                                _download_save_path, \n"
        "                                _start_position, \n"
        "                                _end_position, \n"
        "                                _position,\n"
        "                                _finish)\n"
        "values (\n"
        "        :_url, \n"
        "        :_thread_id,\n"
        "        :_download_save_path, \n"
        "        :_start_position,\n"
        "        :_end_position,\n"
        "        :_position, \n"
        "        :_finish\n"
        "        );";

    qDebug() << "下载任务 sql  : " << download_task_sql;

    sqlQuery.prepare(download_task_sql);

    sqlQuery.bindValue(":_url", url);
    sqlQuery.bindValue(":_thread_id", thread_id);
    sqlQuery.bindValue(":_download_save_path", downloadTempDir + "/" +
                                                   file_name + ".part" +
                                                   QString::number(i + 1));

    sqlQuery.bindValue(":_start_position", start_position);

    sqlQuery.bindValue(":_end_position", end_position);
    sqlQuery.bindValue(":_position", 0);
    sqlQuery.bindValue(":_finish", false);

    if (sqlQuery.exec()) {
      qDebug() << "下载任务插入成功" << sqlQuery.lastError();
    } else {
      qDebug() << "下载任务插入失败" << sqlQuery.lastError();
    }
  }
  // 插入最后一条

  QString update_download_thread_info_laster =
      "update _download_info set _thread_id_" +
      QString::number(thread_count + 1) + " = '" + file_name +
      QString::number(thread_count + 1) + "' where _url='" + url + "'";

  if (sqlQuery.exec(update_download_thread_info_laster)) {
    qDebug() << "最后一条更新成功" << sqlQuery.lastError();

  } else {
    qDebug() << "最后一条更新失败" << sqlQuery.lastError();
  }

  QString download_task_laster_sql =
      "insert into _download_task_info(_url, _thread_id, "
      "_download_save_path, _start_position, _end_position, _position, "
      "_finish) values (:_url,:_thread_id, :_download_save_path, "
      ":_start_position, :_end_position, :_position, :_finish);";

  sqlQuery.prepare(download_task_laster_sql);

  sqlQuery.bindValue(":_url", url);
  sqlQuery.bindValue(":_thread_id",
                     file_name + QString::number(thread_count + 1));
  sqlQuery.bindValue(":_download_save_path",
                     downloadTempDir + "/" + file_name + ".part" +
                         QString::number(thread_count + 1));

  sqlQuery.bindValue(":_start_position", file_length - laster_block_size);

  sqlQuery.bindValue(":_end_position", file_length);
  sqlQuery.bindValue(":_position", 0);
  sqlQuery.bindValue(":_finish", false);
  if (sqlQuery.exec()) {
    qDebug() << "最后一块下载任务插入成功";
  } else {
    qDebug() << "最后一块下载任务插入失败";
  }

  dowmloadFileTaskModel->start(url);
}

void DownloadFileManagerModel::readyRead() {}

void DownloadFileManagerModel::error(QNetworkReply::NetworkError) {}