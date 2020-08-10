#pragma once

#include <vkapi.h>
#define max_tok_count 50

typedef void (*function)(message *msg, int argc, char *argv[]);
typedef map_t(function) map_function_t;
map_function_t commands;

void tokenize(char *str, char *tokens[max_tok_count], int *token_count);
void commands_init();
void handler(message *msg);
message *get_longpoll_events(cJSON *longpoll);
