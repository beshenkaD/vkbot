#include <handler.h>
#include <utils/stringutils.h>

#include <modules/std.h>

void handler(message *msg) {
  if (!msg) return;

  for (int i = 0; i < module_count; i++) {
    for (int j = 0; j < modules[i].cmds_count; j++) {
      if (strncmp(modules[i].cmds[j].prefix, msg->text, 1) != 0)
        return;
    }
  }

  char *tokens[max_tok_count];
  int tokens_count = 0;

  tokenize(msg->text, tokens, &tokens_count);

  for (int i = 0; i < msg->count; i++) {
    function *f = map_get(&commands, msg[i].text);

    if (f) {
      (*f)(&msg[i], tokens_count, tokens);
    }
  }

  return;
}

void tokenize(char *str, char *tokens[max_tok_count], int *token_count) {
  size_t n = 0;

  for (char *p = strtok(str, " "); p; p = strtok(NULL, " ")) {
    if (n >= max_tok_count)
      break;
    tokens[n++] = p;

    *token_count = n;
  }
}

message *get_longpoll_events(cJSON *longpoll) {
  cJSON *ts = cJSON_GetObjectItem(longpoll, "ts");

  if (!ts) {
    get_longpoll(); // key must be updated every hour
    return NULL;
  }

  strncpy(vk->ts, ts->valuestring, 31);

  cJSON *updates = cJSON_GetObjectItem(longpoll, "updates");


  int n = cJSON_GetArraySize(updates);
  if (n == 0) {
    return NULL;
  }

  message *events = calloc(n, sizeof(message));
  events->count = n;

  for (int i = 0; i < n; i++) {
    cJSON *item = cJSON_GetArrayItem(updates, i);
    cJSON *object = cJSON_GetObjectItem(item, "object");
    cJSON *message = cJSON_GetObjectItem(object, "message");
    cJSON *from_id = cJSON_GetObjectItem(message, "from_id");
    cJSON *peer_id = cJSON_GetObjectItem(message, "peer_id");
    cJSON *text = cJSON_GetObjectItem(message, "text");

    cJSON *reply_message = cJSON_GetObjectItem(message, "reply_message");
    if (reply_message) {
      cJSON *r_from_id = cJSON_GetObjectItem(reply_message, "from_id");
      cJSON *r_peer_id = cJSON_GetObjectItem(reply_message, "peer_id");
      cJSON *r_text = cJSON_GetObjectItem(reply_message, "text");

      events[i].r_from_id = r_from_id->valueint;
      events[i].r_peer_id = r_peer_id->valueint;
      strncpy(events[i].r_text, r_text->valuestring, 63);
    }
   
    if (from_id->valueint == peer_id->valueint)
      return NULL; // Private message

    events[i].from_id = from_id->valueint;
    events[i].peer_id = peer_id->valueint;
    strncpy(events[i].text, text->valuestring, 63);
  }
  return events;
}
