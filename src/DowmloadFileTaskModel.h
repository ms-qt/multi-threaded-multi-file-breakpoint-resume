//
// Created by 毛华伟 on 2020/3/9.
//

#ifndef MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWMLOADFILETASKMODEL_H
#define MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWMLOADFILETASKMODEL_H

#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class DowmloadFileTaskModel : public QSqlQueryModel {

  Q_OBJECT
public:
  DowmloadFileTaskModel();

public slots:

private:

  // 表名称
  QString __TABLE_NAME__ = "_download_task_info";


  // 创建表
  QString __CREATE_TABLE__ = "create table _download_task_info\n"
      "(\n"
      "    _id                 integer primary key autoincrement,\n"
      "    _url                varchar(8192),\n"
      "    _thread_id          varchar(255),\n"
      "    _download_save_path varchar(255),\n"
      "    _start_position     integer,\n"
      "    _end_position       integer,\n"
      "    _position           integer,\n"
      "    _finish             boolean\n"
      ");";
};

#endif // MULTI_THREADED_MULTI_FILE_BREAKPOINT_RESUME_DOWMLOADFILETASKMODEL_H
