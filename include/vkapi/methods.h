#pragma once

#include <vkapi/users.h>
#include <vkapi/vkapi.h>

void messages_send(char *msg, uint32_t peer_id);
void messages_remove_chat_user(uint32_t chat_id, int user_id);
cJSON *messages_get_conv_members(uint32_t peer_id, char *fields);
members_t *get_conv_members(uint32_t peer_id, char *fields, bool profiles_need,
                            bool items_need, bool groups_need);
cJSON *wall_get(int owner_id, char *domain, int offset, int count,
                char *filter);
