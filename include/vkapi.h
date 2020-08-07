#pragma once

#include "curl.h"
#include "map.h"
#include <sds.h>
#include <cjson/cJSON.h>

#include "../config/config.h"

#define VK_METHOD_URL "https://api.vk.com/method/"
#define VK_API_VERSION "5.110"
#define TIMEOUT "30"

typedef struct {
  char key[64];
  char server[32];
  char ts[32];
} vk_handle;

typedef struct {
  char text[64];
  long peer_id;
  long from_id;

  char r_text[64]; // reply
  long r_peer_id;
  long r_from_id;

  int count;
} message;

vk_handle *vk;

cJSON *call_method(const char *method, map_str_t *args);
cJSON *get_longpoll_json();
void vk_init();
void vk_destroy();
