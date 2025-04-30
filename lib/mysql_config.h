#ifndef MYSQL_CONFIG_H
#define MYSQL_CONFIG_H

#include <mysql/mysql.h>

MYSQL* connect_to_mysql();
void disconnect_from_mysql(MYSQL* conn);

#endif    