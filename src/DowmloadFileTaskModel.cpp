//
// Created by 毛华伟 on 2020/3/9.
//

#include "DowmloadFileTaskModel.h"

DowmloadFileTaskModel::DowmloadFileTaskModel() {
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

  connect(DownloadProxy::getInstance(),
          SIGNAL(downloadFinish(QString, QString)), this,
          SLOT(downloadFinish(QString, QString)));
}

void DowmloadFileTaskModel::start(QString url) {
  QString sql = "select * from _download_task_info where _url='" + url + "';";
  if (qSqlQuery.exec(sql)) {
    while (qSqlQuery.next()) {

      int _id = qSqlQuery.value(0).toInt();
      QString _url = qSqlQuery.value(1).toString();
      QString _thread_id = qSqlQuery.value(2).toString();
      QString _download_save_path = qSqlQuery.value(3).toString();
      int _start_position = qSqlQuery.value(4).toInt();
      int _end_position = qSqlQuery.value(5).toInt();
      int _position = qSqlQuery.value(6).toInt();
      bool _finish = qSqlQuery.value(7).toBool();

      if (!_finish) {
        DownloadClient *downloadClient = new DownloadClient;
        downloadClient->start(_download_save_path, _url, _start_position,
                              _end_position, _position);
      } else {
        qDebug() << "已经完成";
        downloadFinish(url, _download_save_path);
      }
    }
  }
}

void DowmloadFileTaskModel::downloadFinish(QString url, QString file_path) {

  QString sql = "select _finish where _url='" + url + "'";
  bool finish = true;
  if (qSqlQuery.exec(sql)) {
    while (qSqlQuery.next()) {
      bool f = qSqlQuery.value(0).toBool();
      if (!f) {
        finish = false;
      }
    }
  } else {
  }
  if (finish) {
    // 合并文件
    QString mergeFilePath = getFullSavePath(url);
    qDebug() << " mergeFilePath : " << mergeFilePath;
    QFile mergeFile(mergeFilePath);
    if (!mergeFile.open(QIODevice::ReadWrite | QIODevice::Append)) {
      qDebug() << "合并文件打开失败";
      return;
    }
    if (mergeFile.exists()) {
      qDebug() << "文件存在";
      mergeFile.seek(0);
    }
    qDebug() << "合并文件是否存在 : " << mergeFile.exists();
    QString sql =
        "select _download_save_path from _download_task_info where _url='" +
        url + "'";
    if (qSqlQuery.exec(sql)) {
      while (qSqlQuery.next()) {
        QString t_file_path = qSqlQuery.value(0).toString();
        qDebug() << "碎片文件 : " << t_file_path;
        QFile t_file(t_file_path);
        if (t_file.open(QIODevice::ReadWrite | QIODevice::Append)) {
          qDebug() << "碎片文件打开成功";
          t_file.seek(0);
          mergeFile.write(t_file.readAll());
          qDebug() << "刷新";
          mergeFile.flush();
        } else {
          return;
          qDebug() << "碎片文件打开失败";
        }
      }

      qDebug() << "文件合并完成";
      mergeFile.close();
      // 合并完成
      //
      QString update_download_info_status_sql =
          "update _download_info  set _status=" +
          QString::number(DOWNLOAD_FINISH);
      qSqlQuery.exec(update_download_info_status_sql);

    } else {
      qDebug() << "查询碎片文件失败" << qSqlQuery.lastError();
    }
  }
}

QString DowmloadFileTaskModel::getFullSavePath(QString url) {
  QString merge_save_path_sql =
      "select _full_save_path from _download_info where _url ='" + url + "'";
  QString _full_save_path;
  if (qSqlQuery.exec(merge_save_path_sql)) {
    while (qSqlQuery.next()) {
      _full_save_path = qSqlQuery.value(0).toString();
    }
  } else {
  }
  return _full_save_path;
}