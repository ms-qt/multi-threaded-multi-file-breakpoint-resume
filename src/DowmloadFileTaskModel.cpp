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

      DownloadClient *downloadClient = new DownloadClient;
      downloadClient->start(_download_save_path, _url, _start_position,
                            _end_position,_position);
    }
  }
}