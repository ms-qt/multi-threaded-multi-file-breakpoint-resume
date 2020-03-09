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