#ifndef MYSQL_CONNECTION_POOL_H
#define MYSQL_CONNECTION_POOL_H

#include <mysql/mysql.h>
#include <stdbool.h>

#define CONNECTION_POOL_SIZE 5

typedef struct {
    MYSQL *connections[CONNECTION_POOL_SIZE];
    bool is_used[CONNECTION_POOL_SIZE];
} ConnectionPool;

void init_connection_pool(ConnectionPool *pool);
MYSQL *get_connection(ConnectionPool *pool);
void release_connection(ConnectionPool *pool, MYSQL *conn);
void destroy_connection_pool(ConnectionPool *pool);

#endif    