#pragma once

#include <vkapi.h>
#include <stdbool.h>
#include <sds.h>

void get_longpoll();
void messages_send(char *msg, long peer_id);
void messages_remove_chat_user(long chat_id, long user_id);
cJSON *messages_get_conv_members(long peer_id, char *fields);
cJSON *utils_resolve_screen_name(char *screen_name);
