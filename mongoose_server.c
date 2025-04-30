#include "mongoose.h"
#include "mongoose_event.h"
#include "lib/mysql_connection_pool.h"

ConnectionPool pool;

int main(void) {
    struct mg_mgr mgr;
    struct mg_connection *c;

    // 初始化连接池
    init_connection_pool(&pool);

    mg_mgr_init(&mgr);

    c = mg_http_listen(&mgr, "http://0.0.0.0:8000", ev_handler, NULL);
    if (c == NULL) {
        fprintf(stderr, "Failed to listen on port 8000\n");
        return 1;
    }

    // 将连接池指针存储在连接的 fn_data 中
    c->fn_data = &pool;

    printf("Server listening on port 8000\n");

    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }

    // 销毁连接池
    destroy_connection_pool(&pool);
    mg_mgr_free(&mgr);
    return 0;
}