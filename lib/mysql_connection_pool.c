#include "mysql_connection_pool.h"
#include "mysql_config.h"
#include <stdio.h>

void init_connection_pool(ConnectionPool *pool) {
    for (int i = 0; i < CONNECTION_POOL_SIZE; i++) {
        pool->connections[i] = connect_to_mysql();
        pool->is_used[i] = false;
    }
}

MYSQL *get_connection(ConnectionPool *pool) {
    for (int i = 0; i < CONNECTION_POOL_SIZE; i++) {
        if (!pool->is_used[i]) {
            pool->is_used[i] = true;
            return pool->connections[i];
        }
    }
    return NULL;
}

void release_connection(ConnectionPool *pool, MYSQL *conn) {
    for (int i = 0; i < CONNECTION_POOL_SIZE; i++) {
        if (pool->connections[i] == conn) {
            pool->is_used[i] = false;
            break;
        }
    }
}

void destroy_connection_pool(ConnectionPool *pool) {
    for (int i = 0; i < CONNECTION_POOL_SIZE; i++) {
        if (pool->connections[i]) {
            disconnect_from_mysql(pool->connections[i]);
        }
    }
}    