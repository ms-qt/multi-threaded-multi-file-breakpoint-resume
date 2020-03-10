//
// Created by maohuawei on 2020/3/9.
//

#include "DownloadClient.h"
#include <QtCore/qfileinfo.h>

DownloadClient::DownloadClient() {

  qNetworkAccessManager = new QNetworkAccessManager;
}

void DownloadClient::start(QString file_path, QString url, int start_position,
                           int end_position, int _position) {

  status = DOWNLOAD_ING;
  this->file_path = file_path;
  this->url = url;
  this->start_position = start_position;
  this->end_position = end_position;

  file.setFileName(file_path);
  file.open(QIODevice::ReadWrite | QIODevice::Append);
  // 文件指针移动的位置
  qDebug() << "文件指针移动的位置 : " << _position;
  file.seek(_position);

  qDebug() << "开始下载 ";
  QNetworkRequest request(url);

  QString bytes = "bytes=" + QString::number(start_position + _position) + "-" +
                  QString::number(end_position);

  qDebug() << "bytes " << bytes;

  request.setRawHeader("Range", bytes.toLocal8Bit());
  qNetworkReply = qNetworkAccessManager->get(request);

  connect(qNetworkReply, SIGNAL(downloadProgress(qint64, qint64)), this,
          SLOT(downloadProgress(qint64, qint64)));
  connect(qNetworkReply, SIGNAL(finished()), this, SLOT(finish()));
  connect(qNetworkReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(qNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(error(QNetworkReply::NetworkError)));
}

void DownloadClient::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {

  qDebug() << file_path << "  bytesReceived : " << bytesReceived
           << "bytesTotal : " << bytesTotal;
}

void DownloadClient::finish() {

  qDebug() << "下载完成";

  // 下载写入长度对齐才能说明下载成功
  if (count_position == end_position - start_position + 1) {
    status = DOWNLOAD_FINISH;
    file.close();
    // 更新数据库信息
    QString finish_sql =
        "update _download_task_info set _finish = " + QString::number(true) +
        " where _download_save_path = '" + file_path + "'";

    if (qSqlQuery.exec(finish_sql)) {
      qDebug() << file_path << "  :  更新数据库成功 " << qSqlQuery.lastError();
      emit DownloadProxy::getInstance()->downloadFinish(url,file_path);
    } else {
      qDebug() << file_path << "  :  更新数据库失败 " << qSqlQuery.lastError();
    }
  } else {
    start(file_path, url, start_position, end_position, count_position);
  }
}

void DownloadClient::readyRead() {

  qint64 position = file.write(qNetworkReply->readAll());
  count_position += position;
  QString sql = "update _download_task_info set _position = " +
                QString::number(count_position) +
                " where _download_save_path ='" + file_path + "';";
  qSqlQuery.exec(sql);
  file.flush();
}

void DownloadClient::error(QNetworkReply::NetworkError) {

  qDebug() << "下载错误 : " << qNetworkReply->errorString();
}