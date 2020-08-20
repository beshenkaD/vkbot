#include <modules/fun/anek.h>
#include <time.h>

#define ANEK_SOURCE -149279263

command Anek = {"Андекдот", anek,
                "/",        "Случайный анекдот категории Б+",
                user,       {"anek", "Anek", "Анек", "анек"}};

void anek(message *msg, int argc, char *argv[], int p) {
  srand(time(NULL));
  int offset  = rand() % 7915;
  cJSON *root = wall_get(ANEK_SOURCE, "", offset, 10, "");

  cJSON *response = cJSON_GetObjectItem(root, "response");
  cJSON *items    = cJSON_GetObjectItem(response, "items");

  if (!items) {
    messages_send(cJSON_Print(root), msg->peer_id);
    cJSON_Delete(root);
    return;
  }

  int n = cJSON_GetArraySize(items);

  for (int i = 0; i < n; i++) {
    cJSON *item      = cJSON_GetArrayItem(items, i);
    cJSON *ads       = cJSON_GetObjectItem(item, "marked_as_ads");
    cJSON *post_type = cJSON_GetObjectItem(item, "post_type");

    if (ads->valueint != 0 || strcmp(post_type->valuestring, "post") != 0)
      continue;

    cJSON *text = cJSON_GetObjectItem(item, "text");

    if (strlen(text->valuestring) == 0)
      continue;
    if (strstr(text->valuestring, "https://") != NULL)
      continue;
    if (strstr(text->valuestring, "http://") != NULL)
      continue;
    if (strstr(text->valuestring, "vk.") != NULL)
      continue;
    if (strstr(text->valuestring, "|") != NULL)
      continue;

    messages_send(text->valuestring, msg->peer_id);
    break;
  }
  cJSON_Delete(root);
}
