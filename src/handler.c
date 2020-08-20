#include <handler.h>
#include <usersDb.h>

static void tokenize(char *str, char *tokens[max_tok_count], int *token_count);
static permission_t get_perm(message *msg);

void handler(message *msg) {
  if (!msg)
    return;

  // Prefix checker here

  for (int i = 0; i < msg->count; i++) {
    if (msg[i].event_type == cmd) {
      char *tokens[max_tok_count] = {NULL, NULL};
      int tokens_count            = 0;

      tokenize(msg[i].text, tokens, &tokens_count);

      permission_t perm = get_perm(&msg[i]);
      function_cmd *f   = map_get(&commands, msg[i].text);

      if (f)
        (*f)(&msg[i], tokens_count, tokens, perm);
    }

    if (msg[i].event_type == act) {
      for (int j = 0; j < act_commands_count; j++) {
        if (strcmp(msg[i].type, act_commands[j].action_type) == 0)
          (*act_commands[j].act_func)(&msg[i]);
      }
    }
  }
  return;
}

message *get_longpoll_events(cJSON *longpoll) {
  cJSON *ts = cJSON_GetObjectItem(longpoll, "ts");

  if (!ts) {
    update_longpoll(); // key must be updated every hour
    return NULL;
  }

  strncpy(vk->ts, ts->valuestring, 31);

  cJSON *updates = cJSON_GetObjectItem(longpoll, "updates");

  int n = cJSON_GetArraySize(updates);
  if (n == 0) {
    return NULL;
  }

  message *events = calloc(n, sizeof(message));
  events->count   = n;

  for (int i = 0; i < n; i++) {
    cJSON *item    = cJSON_GetArrayItem(updates, i);
    cJSON *object  = cJSON_GetObjectItem(item, "object");
    cJSON *message = cJSON_GetObjectItem(object, "message");
    cJSON *from_id = cJSON_GetObjectItem(message, "from_id");
    cJSON *peer_id = cJSON_GetObjectItem(message, "peer_id");
    cJSON *text    = cJSON_GetObjectItem(message, "text");

    cJSON *reply_message = cJSON_GetObjectItem(message, "reply_message");
    if (reply_message) {
      cJSON *r_from_id = cJSON_GetObjectItem(reply_message, "from_id");
      cJSON *r_peer_id = cJSON_GetObjectItem(reply_message, "peer_id");
      cJSON *r_text    = cJSON_GetObjectItem(reply_message, "text");

      events[i].r_from_id = r_from_id->valueint;
      events[i].r_peer_id = r_peer_id->valueint;
      strncpy(events[i].r_text, r_text->valuestring, max_msg_length);
    }
    cJSON *action = cJSON_GetObjectItem(message, "action");
    if (action) {
      events[i].event_type = act;
      cJSON *type          = cJSON_GetObjectItem(action, "type");
      cJSON *member_id     = cJSON_GetObjectItem(action, "member_id");

      strncpy(events[i].type, type->valuestring, 31);
      events[i].member_id = member_id->valueint;
    } else
      events[i].event_type = cmd;

    if (from_id->valueint == peer_id->valueint)
      return NULL; // Private message

    events[i].from_id = from_id->valueint;
    events[i].peer_id = peer_id->valueint;
    strncpy(events[i].text, text->valuestring, max_msg_length);
  }
  return events;
}

static void tokenize(char *str, char *tokens[max_tok_count], int *token_count) {
  size_t n = 0;

  for (char *p = strtok(str, " "); p; p = strtok(NULL, " ")) {
    if (n >= max_tok_count)
      break;
    tokens[n++] = p;

    *token_count = n;
  }
}

static permission_t get_perm(message *msg) {
  permission_t p;
  p = get_permission_by_id(msg->from_id, msg->peer_id);

  return p;
}
