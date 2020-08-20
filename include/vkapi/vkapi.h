#pragma once

#include "curl.h"
#include "map.h"
#include <cjson/cJSON.h>
#include <sds.h>
#include <stdbool.h>

#include "../config/config.h"

#define VK_METHOD_URL  "https://api.vk.com/method/"
#define VK_API_VERSION "5.110"
#define TIMEOUT        "30"

#define max_msg_length 1024

typedef struct {
  char key[64];
  char server[32];
  char ts[32];
} vk_handle;

// Type of event from longpoll
typedef enum {
  act = 1,
  cmd = 2
} event_type;

typedef struct {
  char text[max_msg_length];
  uint32_t peer_id;
  int from_id;

  // Reply message
  char r_text[max_msg_length];
  uint32_t r_peer_id;
  int r_from_id;

  // Type
  event_type event_type;
  char type[32];
  int member_id;

  int count;
} message;

vk_handle *vk;

cJSON *call_method(const char *method, map_str_t *args, bool as_user);
cJSON *get_longpoll_json();
void update_longpoll();
void vk_init();
void vk_destroy();
