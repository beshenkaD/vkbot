#include <stdbool.h>
#include <utils/stringutils.h>
#include <vkapi/vkapi.h>

static char *args_serialize(map_str_t args);

void vk_init() {
  vk          = calloc(1, sizeof(vk_handle));
  curl_handle = curl_init();
  update_longpoll();
}

void vk_destroy() {
  curl_easy_cleanup(curl_handle);
  free(vk);
}

cJSON *call_method(const char *method, map_str_t *args, bool as_user) {
  map_set(args, "v", VK_API_VERSION);
  if (!as_user)
    map_set(args, "access_token", token);
  else
    map_set(args, "access_token", user_token);
  map_set(args, "group_id", botid);

  sds s = args_serialize(*args);

  char *ret = curl_post(va("%s/%s", VK_METHOD_URL, method), s);

  cJSON *root = cJSON_ParseWithOpts(ret, NULL, false);

  sdsfree(s);
  free(ret);

  return root;
}

void update_longpoll() {
  map_str_t n;
  map_init(&n);

  cJSON *root = call_method("groups.getLongPollServer", &n, false);
  map_deinit(&n);

  cJSON *response = cJSON_GetObjectItem(root, "response");
  cJSON *server   = cJSON_GetObjectItem(response, "server");
  cJSON *key      = cJSON_GetObjectItem(response, "key");
  cJSON *ts       = cJSON_GetObjectItem(response, "ts");

  strncpy(vk->server, server->valuestring, 31);
  strncpy(vk->key, key->valuestring, 63);
  strncpy(vk->ts, ts->valuestring, 31);

  cJSON_Delete(root);
}

cJSON *get_longpoll_json() {
  map_str_t args;
  map_init(&args);

  map_set(&args, "act", "a_check");
  map_set(&args, "key", vk->key);
  map_set(&args, "wait", TIMEOUT);
  map_set(&args, "ts", vk->ts);

  char *s = args_serialize(args);
  char *r = curl_post(vk->server, s);

  map_deinit(&args);
  sdsfree(s);

  cJSON *root = cJSON_ParseWithOpts(r, NULL, false);

  free(r);
  return root;
}

static char *args_serialize(map_str_t args) {
  sds s = sdsempty();

  const char *key;
  map_iter_t iter = map_iter(&m);

  while ((key = map_next(&args, &iter))) {
    s = sdscatfmt(s, "%s=%s&", key, *map_get(&args, key));
  }
  return s;
}
