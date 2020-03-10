//
// Created by dev on 2020/3/10.
//

#ifndef MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWNLOADPROXY_H
#define MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWNLOADPROXY_H

#include <QtCore/QObject>
class DownloadProxy : public QObject {

  Q_OBJECT
public:
  static DownloadProxy *getInstance();
signals:
  // 下载完成
  void downloadFinish(QString url, QString file_path);
  // 下载完成，合并文件
  void mergeFile(QString url);

private:
  DownloadProxy();
  static DownloadProxy *instance;
};

#endif // MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWNLOADPROXY_H
