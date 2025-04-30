#ifndef MONGOOSE_EVENT_H
#define MONGOOSE_EVENT_H

#include "mongoose.h"

void ev_handler(struct mg_connection *c, int ev, void *ev_data);

#endif
    