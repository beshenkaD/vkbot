#pragma once

#include <vkapi.h>

typedef void (*function)(message *msg);
typedef map_t(function) map_function_t;
map_function_t commands;

void commands_init();
void handler(message *msg);
message *get_longpoll_events(cJSON *longpoll);
