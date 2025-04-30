#include "cpi_handler.h"
#include "../lib/mysql_connection_pool.h"
#include <stdio.h>
#include <string.h>

// 辅助函数：发送错误响应
void send_error_response(struct mg_connection *c, const char *error_msg) {
    cJSON *root = cJSON_CreateObject();
    cJSON *error_obj = cJSON_CreateString(error_msg);
    cJSON_AddItemToObject(root, "error", error_obj);
    char *json_str = cJSON_Print(root);
    mg_http_reply(c, 500, "Content-Type: application/json\r\n", "%s", json_str);
    cJSON_free(json_str);
    cJSON_Delete(root);
}

// 处理 GET 请求，获取所有 CPI 数据
void handle_get_cpi(struct mg_connection *c, ConnectionPool *pool) {
    MYSQL *conn = get_connection(pool);
    if (conn == NULL) {
        send_error_response(c, "No available connections in the pool");
        return;
    }

    if (mysql_query(conn, "SELECT * FROM cpi") != 0) {
        send_error_response(c, mysql_error(conn));
    } else {
        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            send_error_response(c, mysql_error(conn));
        } else {
            int num_fields = mysql_num_fields(result);
            MYSQL_ROW row;

            cJSON *root = cJSON_CreateArray();
            while ((row = mysql_fetch_row(result))) {
                cJSON *item = cJSON_CreateObject();
                for (int i = 0; i < num_fields; i++) {
                    cJSON *field = cJSON_CreateString(row[i]? row[i] : "NULL");
                    cJSON_AddItemToObject(item, mysql_fetch_field_direct(result, i)->name, field);
                }
                cJSON_AddItemToArray(root, item);
            }

            char *json_str = cJSON_Print(root);
            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json_str);
            cJSON_free(json_str);
            cJSON_Delete(root);

            mysql_free_result(result);
        }
    }

    release_connection(pool, conn);
}

// 处理 POST 请求，添加 CPI 数据
void handle_post_cpi(struct mg_connection *c, struct mg_http_message *hm, ConnectionPool *pool) {
    MYSQL *conn = get_connection(pool);
    if (conn == NULL) {
        send_error_response(c, "No available connections in the pool");
        return;
    }

    // 解析 JSON 数据
    cJSON *root = cJSON_Parse((char *)hm->body.buf);
    if (root == NULL) {
        send_error_response(c, "Failed to parse JSON data");
        release_connection(pool, conn);
        return;
    }

    cJSON *cpi_json = cJSON_GetObjectItem(root, "cpi");
    cJSON *date_json = cJSON_GetObjectItem(root, "date");
    if (!cpi_json ||!date_json) {
        send_error_response(c, "Missing 'cpi' or 'date' in JSON data");
        cJSON_Delete(root);
        release_connection(pool, conn);
        return;
    }

    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO cpi (cpi, date) VALUES ('%s', '%s')", cpi_json->valuestring, date_json->valuestring);

    if (mysql_query(conn, query) != 0) {
        send_error_response(c, mysql_error(conn));
    } else {
        cJSON *response = cJSON_CreateObject();
        cJSON *message_obj = cJSON_CreateString("Insert successful");
        cJSON_AddItemToObject(response, "message", message_obj);
        char *json_str = cJSON_Print(response);
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json_str);
        cJSON_free(json_str);
        cJSON_Delete(response);
    }

    cJSON_Delete(root);
    release_connection(pool, conn);
}

// 处理 DELETE 请求，删除指定 ID 的 CPI 数据
void handle_delete_cpi(struct mg_connection *c, struct mg_http_message *hm, ConnectionPool *pool) {
    MYSQL *conn = get_connection(pool);
    if (conn == NULL) {
        send_error_response(c, "No available connections in the pool");
        return;
    }

    // 解析 URL 参数获取 ID
    char id_buf[32];
    struct mg_str query_str = hm->query;
    if (mg_http_get_var(&query_str, "id", id_buf, sizeof(id_buf)) == 0) {
        send_error_response(c, "Missing 'id' parameter in URL");
        release_connection(pool, conn);
        return;
    }

    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM cpi WHERE id = %s", id_buf);

    if (mysql_query(conn, query) != 0) {
        send_error_response(c, mysql_error(conn));
    } else {
        cJSON *response = cJSON_CreateObject();
        cJSON *message_obj = cJSON_CreateString("Delete successful");
        cJSON_AddItemToObject(response, "message", message_obj);
        char *json_str = cJSON_Print(response);
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json_str);
        cJSON_free(json_str);
        cJSON_Delete(response);
    }

    release_connection(pool, conn);
}

// 处理 PUT 请求，更新指定 ID 的 CPI 数据
void handle_put_cpi(struct mg_connection *c, struct mg_http_message *hm, ConnectionPool *pool) {
    MYSQL *conn = get_connection(pool);
    if (conn == NULL) {
        send_error_response(c, "No available connections in the pool");
        return;
    }

    // 解析 URL 参数获取 ID
    char id_buf[32];
    struct mg_str query_str = hm->query;
    if (mg_http_get_var(&query_str, "id", id_buf, sizeof(id_buf)) == 0) {
        send_error_response(c, "Missing 'id' parameter in URL");
        release_connection(pool, conn);
        return;
    }

    // 解析 JSON 数据
    cJSON *root = cJSON_Parse((char *)hm->body.buf);
    if (root == NULL) {
        send_error_response(c, "Failed to parse JSON data");
        release_connection(pool, conn);
        return;
    }

    cJSON *cpi_json = cJSON_GetObjectItem(root, "cpi");
    cJSON *date_json = cJSON_GetObjectItem(root, "date");
    if (!cpi_json &&!date_json) {
        send_error_response(c, "Missing 'cpi' or 'date' in JSON data");
        cJSON_Delete(root);
        release_connection(pool, conn);
        return;
    }

    char query[256];
    if (cpi_json && date_json) {
        snprintf(query, sizeof(query), "UPDATE cpi SET cpi = '%s', date = '%s' WHERE id = %s", cpi_json->valuestring, date_json->valuestring, id_buf);
    } else if (cpi_json) {
        snprintf(query, sizeof(query), "UPDATE cpi SET cpi = '%s' WHERE id = %s", cpi_json->valuestring, id_buf);
    } else {
        snprintf(query, sizeof(query), "UPDATE cpi SET date = '%s' WHERE id = %s", date_json->valuestring, id_buf);
    }

    if (mysql_query(conn, query) != 0) {
        send_error_response(c, mysql_error(conn));
    } else {
        cJSON *response = cJSON_CreateObject();
        cJSON *message_obj = cJSON_CreateString("Update successful");
        cJSON_AddItemToObject(response, "message", message_obj);
        char *json_str = cJSON_Print(response);
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json_str);
        cJSON_free(json_str);
        cJSON_Delete(response);
    }

    cJSON_Delete(root);
    release_connection(pool, conn);
}

// 主处理函数，根据 HTTP 方法分发请求
void handle_cpi(struct mg_connection *c, struct mg_http_message *hm, ConnectionPool *pool) {
    struct mg_str method = hm->method;
    struct mg_str get = mg_str("GET");
    struct mg_str post = mg_str("POST");
    struct mg_str del = mg_str("DELETE");
    struct mg_str put = mg_str("PUT");

    if (mg_strcmp(method, get) == 0) {
        handle_get_cpi(c, pool);
    } else if (mg_strcmp(method, post) == 0) {
        handle_post_cpi(c, hm, pool);
    } else if (mg_strcmp(method, del) == 0) {
        handle_delete_cpi(c, hm, pool);
    } else if (mg_strcmp(method, put) == 0) {
        handle_put_cpi(c, hm, pool);
    } else {
        send_error_response(c, "Unsupported HTTP method");
    }
}