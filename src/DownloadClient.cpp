//
// Created by maohuawei on 2020/3/9.
//

#include "DownloadClient.h"
#include <QtCore/qfileinfo.h>

DownloadClient::DownloadClient() {

  qNetworkAccessManager = new QNetworkAccessManager;
}

void DownloadClient::start(QString file_path, QString url, int start_position,
                           int end_position,int _position) {

  this->file_path = file_path;
  this->url = url;

  file.setFileName(file_path);
  file.open(QIODevice::ReadWrite | QIODevice::Append);

  file.seek(_position);



  qDebug() << "开始下载 ";

  QNetworkRequest request(url);

  QString bytes = "bytes=" + QString::number(start_position) + "-" +
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
           << " bytesTotal : " << bytesTotal;


}

void DownloadClient::finish() {

  qDebug() << "下载完成";
  file.close();
}

void DownloadClient::readyRead() {

  qint64  position = file.write(qNetworkReply->readAll());
  count_position+=position;

  QString sql = "update _download_task_info set _position = " +
                QString::number(count_position) +
                " where _download_save_path ='" + file_path + "';";
  qSqlQuery.exec(sql);
  file.flush();

}

void DownloadClient::error(QNetworkReply::NetworkError) {}