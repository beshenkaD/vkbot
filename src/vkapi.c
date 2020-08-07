#include <vkapi.h>
#include <methods.h>
#include <stdbool.h>
#include <utils/stringutils.h>

static char *args_serialize(map_str_t args);

void vk_init() {
  vk = calloc(1, sizeof(vk_handle));
  curl_handle = curl_init();
  get_longpoll();
}

void vk_destroy() {
  curl_easy_cleanup(curl_handle);
  free(vk);
}

cJSON *call_method(const char *method, map_str_t *args) {
  map_set(args, "v", VK_API_VERSION);
  map_set(args, "access_token", token);
  map_set(args, "group_id", id);

  sds s = args_serialize(*args);

  char *ret = curl_post(va("%s/%s", VK_METHOD_URL, method), s);

  cJSON *root = cJSON_ParseWithOpts(ret, NULL, false);

  sdsfree(s);
  free(ret);

  return root;
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
