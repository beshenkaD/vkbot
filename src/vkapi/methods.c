#include <utils/stringutils.h>
#include <vkapi/methods.h>

void messages_send(char *msg, uint32_t peer_id) {
  map_str_t args;
  map_init(&args);

  map_set(&args, "message", msg);
  map_set(&args, "random_id", "0");
  map_set(&args, "disable_mentions", "1");
  map_set(&args, "peer_id", va("%i", peer_id));

  cJSON *root = call_method("messages.send", &args, false);
  map_deinit(&args);

  cJSON_Delete(root);
}

void messages_remove_chat_user(uint32_t chat_id, int user_id) {
  map_str_t args;
  map_init(&args);

  if (chat_id > 2000000000)
    chat_id = chat_id - 2000000000;

  map_set(&args, "chat_id", va("%ld", chat_id));
  map_set(&args, "member_id", va("%ld", user_id));

  cJSON *root = call_method("messages.removeChatUser", &args, false);
  map_deinit(&args);
  cJSON *resp = cJSON_GetObjectItem(root, "error");

  if (resp) {
    cJSON *err_msg = cJSON_GetObjectItem(resp, "error_msg");
    messages_send(err_msg->valuestring, chat_id + 2000000000);
  }

  cJSON_Delete(root);
}

cJSON *messages_get_conv_members(uint32_t peer_id, char *fields) {
  map_str_t args;
  map_init(&args);

  map_set(&args, "peer_id", va("%ld", peer_id));
  map_set(&args, "fields", fields);

  cJSON *root = call_method("messages.getConversationMembers", &args, false);
  cJSON *resp = cJSON_GetObjectItem(root, "error");

  if (resp) {
    cJSON *err_msg = cJSON_GetObjectItem(resp, "error_msg");
    messages_send(err_msg->valuestring, peer_id);
  }

  map_deinit(&args);
  return root;
}

members_t *get_conv_members(uint32_t peer_id, char *fields, bool profiles_need,
                            bool members_need, bool groups_need) {
  if (!members_need && !profiles_need && !groups_need)
    return NULL;

  members_t *users = malloc(sizeof(members_t));

  cJSON *root = messages_get_conv_members(peer_id, "");
  cJSON *resp = cJSON_GetObjectItem(root, "response");

  if (members_need) {
    cJSON *items    = cJSON_GetObjectItem(resp, "items");
    int items_count = cJSON_GetArraySize(items);

    users->members       = calloc(items_count, sizeof(member));
    users->members_count = items_count;

    for (int i = 0; i < items_count; i++) {
      cJSON *item = cJSON_GetArrayItem(items, i);

      cJSON *member_id = cJSON_GetObjectItem(item, "member_id");
      cJSON *is_admin  = cJSON_GetObjectItem(item, "is_admin");
      cJSON *join_date = cJSON_GetObjectItem(item, "join_date");

      users->members[i].join_date = join_date->valueint;
      users->members[i].member_id = member_id->valueint;
      users->members[i].is_admin  = cJSON_IsTrue(is_admin);
    }
  } else
    users->members = NULL;

  if (profiles_need) {
    cJSON *profiles    = cJSON_GetObjectItem(resp, "profiles");
    int profiles_count = cJSON_GetArraySize(profiles);

    users->profiles       = calloc(profiles_count, sizeof(profile));
    users->profiles_count = profiles_count;

    for (int i = 0; i < profiles_count; i++) {
      cJSON *profile = cJSON_GetArrayItem(profiles, i);

      cJSON *id         = cJSON_GetObjectItem(profile, "id");
      cJSON *first_name = cJSON_GetObjectItem(profile, "first_name");
      cJSON *last_name  = cJSON_GetObjectItem(profile, "last_name");
      cJSON *sex        = cJSON_GetObjectItem(profile, "sex");

      users->profiles[i].id = id->valueint;

      if (sex)
        users->profiles[i].sex = sex->valueint;
      else
        users->profiles[i].sex = -2;

      strncpy(users->profiles[i].first_name, first_name->valuestring, 31);
      strncpy(users->profiles[i].last_name, last_name->valuestring, 31);
    }
  } else
    users->profiles = NULL;

  if (groups_need) {
    cJSON *groups    = cJSON_GetObjectItem(resp, "groups");
    int groups_count = cJSON_GetArraySize(groups);

    users->groups       = calloc(groups_count, sizeof(group));
    users->groups_count = groups_count;

    for (int i = 0; i < groups_count; i++) {
      cJSON *group = cJSON_GetArrayItem(groups, i);

      cJSON *id   = cJSON_GetObjectItem(group, "id");
      cJSON *name = cJSON_GetObjectItem(group, "name");

      users->groups[i].id = id->valueint;
      strncpy(users->groups[i].name, name->valuestring, 63);
    }
  } else
    users->groups = NULL;

  cJSON_Delete(root);
  return users;
}

cJSON *wall_get(int owner_id, char *domain, int offset, int count,
                char *filter) {
  map_str_t args;
  map_init(&args);

  map_set(&args, "owner_id", va("%i", owner_id));
  map_set(&args, "domain", domain);
  map_set(&args, "offset", va("%i", offset));
  map_set(&args, "count", va("%i", count));
  map_set(&args, "filter", filter);

  cJSON *root = call_method("wall.get", &args, true);
  map_deinit(&args);

  return root;
}
