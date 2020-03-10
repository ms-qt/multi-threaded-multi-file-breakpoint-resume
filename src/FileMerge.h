//
// Created by maohuawei on 2020/3/9.
//

#ifndef MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_FILEMERGE_H
#define MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_FILEMERGE_H

#include <QDebug>
#include <QDir>
#include <QFile>
class FileMerge {

public:
  FileMerge() {

    int position = 0;

    QString dirpath = QDir::currentPath() + "/downloadtemp";

    QFile mergeFile(dirpath + "\\merge.apk");

    if (mergeFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
    }
    //设置要遍历的目录
    QDir dir(dirpath);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.par*";
    //将过滤后的文件名称存入到files列表中
    QStringList files =
        dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);

    for (QString it : files) {
      qDebug() << dirpath + "/" + it;
      QFile t_file(dirpath + "/" + it);
      if (t_file.open(QIODevice::ReadWrite | QIODevice::Append)) {
        t_file.seek(0);
        //mergeFile.seek(position);
        position = mergeFile.write(t_file.readAll());
        qDebug() << "刷新";
        mergeFile.flush();
      } else {
        qDebug() << "打开失败";
      }
    }

    mergeFile.close();
  }
};

#endif // MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_FILEMERGE_H
