//
// Created by maohuawei on 2020/3/9.
//
#pragma execution_character_set("utf-8")
#ifndef MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWNLOADCLIENT_H
#define MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWNLOADCLIENT_H

#include <QDataStream>
#include <QFile>
#include <QNetworkReply>
#include <QSqlError>
#include <QtNetwork/QNetworkAccessManager>
#include <QtSql/QSqlQuery>
#include "DownloadProxy.h"
#include "DOWNLOAD_CONSTANT.h"
class DownloadClient : public QObject {

  Q_OBJECT
public:
  DownloadClient();

  void start(QString file_path, QString url, int start_position,
             int end_position, int _position);

public slots:

  // 下载完成
  void finish();

  // 下载错误
  void error(QNetworkReply::NetworkError);

  // 下载进度
  void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

  //
  void readyRead();

private:
  QNetworkAccessManager *qNetworkAccessManager;
  QNetworkReply *qNetworkReply;
  QString file_path;
  QString url;
  QSqlQuery qSqlQuery;
  QFile file;

  qint64 count_position = 0;

  qint64 start_position = 0;

  qint64 end_position = 0;



  int status =DOWNLOAD_NOT_START;


};

#endif // MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWNLOADCLIENT_H
