#include <methods.h>
#include <utils/stringutils.h>

void get_longpoll() {
  map_str_t n;
  map_init(&n);

  cJSON *root = call_method("groups.getLongPollServer", &n);
  map_deinit(&n);

  cJSON *response = cJSON_GetObjectItem(root, "response");
  cJSON *server = cJSON_GetObjectItem(response, "server");
  cJSON *key = cJSON_GetObjectItem(response, "key");
  cJSON *ts = cJSON_GetObjectItem(response, "ts");

  strncpy(vk->server, server->valuestring, 31);
  strncpy(vk->key, key->valuestring, 63);
  strncpy(vk->ts, ts->valuestring, 31);

  cJSON_Delete(root);
}

void messages_send(char *msg, long peer_id) {
  map_str_t args;
  map_init(&args);

  map_set(&args, "message", msg);
  map_set(&args, "random_id", "0");
  map_set(&args, "peer_id", va("%i", peer_id));

  cJSON *root = call_method("messages.send", &args);

  map_deinit(&args);

  cJSON_Delete(root);
}

void messages_remove_chat_user(long chat_id, long user_id) {
  map_str_t args;
  map_init(&args);

  map_set(&args, "chat_id", va("%ld", chat_id));
  map_set(&args, "member_id", va("%ld", user_id));

  cJSON *root = call_method("messages.removeChatUser", &args);
  map_deinit(&args);
  cJSON *resp = cJSON_GetObjectItem(root, "error");

  if (resp) {
    cJSON *code = cJSON_GetObjectItem(resp, "error_code");
    if (code->valueint == 15) messages_send("Access denied.", chat_id + 2000000000);
    if (code->valueint == 925) messages_send("Бот не является администратором.", chat_id + 2000000000);
  }

  cJSON_Delete(root);
}

cJSON *messages_get_conv_members(long peer_id, char *fields) {
  map_str_t args;
  map_init(&args);

  map_set(&args, "peer_id", va("%ld", peer_id));
  map_set(&args, "fields", fields);

  cJSON *root = call_method("messages.getConversationMembers", &args);
  cJSON *resp = cJSON_GetObjectItem(root, "error");

  if (resp) {
    cJSON *code = cJSON_GetObjectItem(resp, "error_code");
    if (code->valueint == 917) messages_send("Бот не имеет доступа к чату.", peer_id);
  }

  map_deinit(&args);
  return root;
}

cJSON *utils_resolve_screen_name(char *screen_name) {
  map_str_t args;
  map_init(&args);

  map_set(&args, "screen_name", screen_name);

  cJSON *root = call_method("utils.resolveScreenName", &args);
  map_deinit(&args);

  return root;
}
