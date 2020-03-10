#pragma execution_character_set("utf-8")
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStandardPaths>
#include <iostream>

#include <QCoreApplication>

#include "DownloadFileManagerModel.h"
#include "FileMerge.h"
// 初始化数据库
static void initDb() {

  // 初始化数据库
  QSqlDatabase database = QSqlDatabase::database();
  if (!database.isValid()) {
    // QSQLITE 代表 SQlite 数据库
    database = QSqlDatabase::addDatabase("QSQLITE");
    // 检查是否实例化
    if (!database.isValid()) {
      qFatal("实例化失败: %s", qPrintable(database.lastError().text()));
    }
  }

  const QDir writeDir =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  if (!writeDir.mkpath(".")) {
    qFatal("创建文件夹 %s", qPrintable(writeDir.absolutePath()));
  }

  const QString fileName = writeDir.absolutePath() + "/app-database.sqlite3";
  qDebug() << "数据库位置 :  " << fileName;
  database.setDatabaseName(fileName);
  if (!database.open()) {
    qFatal("数据库打开失败: %s", qPrintable(database.lastError().text()));
    QFile::remove(fileName);
  }
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  // 初始化数据库
  initDb();

   DownloadFileManagerModel *managerModel = new DownloadFileManagerModel();

   managerModel->addDownloadTask("web",
   "https://dlied4.myapp.com/myapp/1104466820/cos.release-40109/2017_com.tencent.tmgp.sgame_h652813_1.52.1.5_3rlzl9.apk");
//   managerModel->addDownloadTask("web",
//   "https://07e5e3085316c337a91fe2a02dae2d71.dlied1.cdntips.com/godlied4.myapp.com/myapp/1104466820/cos.release-40109/2017_com.tencent.tmgp.sgame_h652813_1.52.1.5_3rlzl9.apk?mkey=5e672841ddd86d13&f=5846&cip=221.216.75.230&proto=https");

  // FileMerge *fileMerge = new FileMerge();

//  QFile file(
//      "F:/src/qt/multi-threaded-multi-file-breakpoint-resume/cmake-build-debug/"
//      "plugins/webs/2017_com.tencent.tmgp.sgame_h652813_1.52.1.5_3rlzl9.apk");
//
//  QString dirPath = "F:/src/qt/multi-threaded-multi-file-breakpoint-resume/"
//                    "cmake-build-debug/plugins/qmls/a/a/a/a/b/b/b/b/b/b/b";
//
//



//  QDir dir(dirPath);
//
//  dir.refresh();
//
//  if (!dir.exists()) {
//
//    bool b = dir.mkpath(dirPath);
//
//    qDebug() << "res : " << b;
//  }

  return app.exec();
}
