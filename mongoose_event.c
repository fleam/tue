#include "mongoose.h"
#include "response_handler.h"
#include "lib/mysql_connection_pool.h"

// 事件处理函数
void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        ConnectionPool *pool = (ConnectionPool *)c->fn_data;

        if (pool != NULL) {
            if (mg_match(hm->uri, mg_str("/api/time/get"), NULL)) {
                handle_time_get(c);
            } else if (mg_match(hm->uri, mg_str("/api/directory/structure"), NULL)) {
                handle_directory_structure(c);
            } else if (mg_match(hm->uri, mg_str("/api/cpi"), NULL)) {
                handle_cpi(c, hm, pool);
            } else {
                handle_unsupported_uri(c);
            }
        } else {
            // 处理连接池指针为空的情况
            fprintf(stderr, "Connection pool pointer is NULL\n");
        }
    }
}