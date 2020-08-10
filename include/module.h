#pragma once

#define max_cmd_count 16
#include <vkapi.h>
#include <handler.h>

#include "../config/config.h"

extern int module_count;

// permissions
#define BANNED 0
#define USER 1
#define HELPER 2
#define ADMIN 3

typedef enum {
  error = -2,
  banned,
  user,
  helper
} permission_t;

typedef struct {
  char *name;
  function func;
  char *prefix;
  char *description;
  int triggers_count;
  int permission;

  char *triggers[max_cmd_count];
} command;

typedef struct {
  char *name;
  char *description;
  int cmds_count;

  command cmds[max_cmd_count];
} module;

map_function_t commands;
module modules[max_modules_count];

void modules_init();
void load(command *m);
void load_module(module *m);
