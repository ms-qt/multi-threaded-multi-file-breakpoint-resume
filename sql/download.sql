create table _download_info
(
    _id                 integer primary key autoincrement,
    _url                varchar(8192),
    _file_name          varchar(255),
    _download_save_path varchar(255),
    _full_save_path     varchar(255),
    _file_length        integer,
    _thread_count       integer,
    _status             integer,
    _thread_id_1        varchar(255),
    _thread_id_2        varchar(255),
    _thread_id_3        varchar(255),
    _thread_id_4        varchar(255),
    _thread_id_5        varchar(255),
    _thread_id_6        varchar(255),
    _thread_id_7        varchar(255),
    _thread_id_8        varchar(255),
    _thread_id_9        varchar(255),
    _thread_id_10       varchar(255),
    _thread_id_11       varchar(255),
    _thread_id_12       varchar(255),
    _thread_id_13       varchar(255),
    _thread_id_14       varchar(255),
    _thread_id_15       varchar(255),
    _thread_id_16       varchar(255)
);



insert into _download_info(_url, _file_name, _download_save_path, _full_save_path, _file_length, _thread_count, _status)
values (:_url, :_file_name, :_download_save_path, :_full_save_path, :_file_length, :_threac_count, :_status);


create table _download_task_info
(
    _id                 integer primary key autoincrement,
    _url                varchar(8192),
    _thread_id          varchar(255),
    _download_save_path varchar(255),
    _start_position     integer,
    _end_position       integer,
    _position           integer,
    _finish             boolean
);