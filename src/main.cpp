#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStandardPaths>
#include <iostream>

#include <QCoreApplication>

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

  return app.exec();
}
