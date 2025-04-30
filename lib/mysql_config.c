#include "mysql_config.h"
#include <stdio.h>
#include <stdlib.h>

MYSQL* connect_to_mysql() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return NULL;
    }

    if (mysql_real_connect(conn, "172.29.176.1", "root", "root", "tue", 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    return conn;
}

void disconnect_from_mysql(MYSQL* conn) {
    if (conn != NULL) {
        mysql_close(conn);
    }
}    