//
// Created by 毛华伟 on 2020/3/9.
//
#pragma execution_character_set("utf-8")

#ifndef MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWNLOADFILEMANAGERMODEL_H
#define MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWNLOADFILEMANAGERMODEL_H

#include <QDebug>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlResult>
#include <QString>

#include "DowmloadFileTaskModel.h"


class DownloadFileManagerModel : public QSqlQueryModel {

  Q_OBJECT
public:
  DownloadFileManagerModel();

public slots:

  // 添加下载任务
  void addDownloadTask(QString type, QString url);

  // 检查任务是否存在
  bool checkTaskExists(QString url);

  // 检查任务是否下载完成
  bool checkTaskFinish(QString url);

  // 下载完成
  void finish();

  // 下载错误
  void error(QNetworkReply::NetworkError);

  // 下载进度
  void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

  //
  void readyRead();

  // 更具url 获取文件名称
  QString getNameByUrl(QString url);


  void mergeFile(QString url);



private:
  // 表名称
  QString __TABLE_NAME__ = "_download_info";
  // 创建表的sql
  QString __CREATE_TABLE__ =
      "create table _download_info\n"
      "(\n"
      "    _id                 integer primary key autoincrement,\n"
      "    _url                varchar(8192),\n"
      "    _file_name          varchar(255),\n"
      "    _download_save_dir varchar(255),\n"
      "    _full_save_path     varchar(255),\n"
      "    _file_length        integer,\n"
      "    _thread_count       integer,\n"
      "    _status             integer,\n"
      "    _thread_id_1        varchar(255),\n"
      "    _thread_id_2        varchar(255),\n"
      "    _thread_id_3        varchar(255),\n"
      "    _thread_id_4        varchar(255),\n"
      "    _thread_id_5        varchar(255),\n"
      "    _thread_id_6        varchar(255),\n"
      "    _thread_id_7        varchar(255),\n"
      "    _thread_id_8        varchar(255),\n"
      "    _thread_id_9        varchar(255),\n"
      "    _thread_id_10       varchar(255),\n"
      "    _thread_id_11       varchar(255),\n"
      "    _thread_id_12       varchar(255),\n"
      "    _thread_id_13       varchar(255),\n"
      "    _thread_id_14       varchar(255),\n"
      "    _thread_id_15       varchar(255),\n"
      "    _thread_id_16       varchar(255)\n"
      ");";

  const QString PLUGIN_QML = "qml";
  const QString PLUGIN_WEB = "web";

  // 最大线程数量
  const int MAX_THREAD_COUNT = 16;

  QSqlQuery sqlQuery;

  //
  QNetworkAccessManager *manager;
  //
  QNetworkReply *reply;
  // 下载的url
  QString url;

  // 下载完成后 完整的 文件存储路径 （合并若干 子文件）
  QString fullFilePath;

  // 文件下载过程中存放的地方
  QString downloadTempDir;

  DowmloadFileTaskModel *dowmloadFileTaskModel;


};

#endif // MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWNLOADFILEMANAGERMODEL_H
