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

  managerModel->addDownloadTask("web", "https://dlied4.myapp.com/myapp/1104466820/cos.release-40109/2017_com.tencent.tmgp.sgame_h652813_1.52.1.5_3rlzl9.apk");


 // FileMerge *fileMerge = new FileMerge();



  return app.exec();
}
