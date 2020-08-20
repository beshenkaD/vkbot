#pragma once

#include <vkapi/vkapi.h>

#define max_tok_count 50

typedef void (*function_cmd)(message *msg, int argc, char *argv[], int permission);
typedef void (*function_act)(message *msg);

typedef map_t(function_cmd) map_function_cmd_t;
typedef map_t(function_act) map_function_act_t;

map_function_cmd_t commands;

void commands_init();
void handler(message *msg);
message *get_longpoll_events(cJSON *longpoll);
