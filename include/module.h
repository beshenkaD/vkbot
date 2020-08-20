#pragma once

#define max_cmd_count 16
#include <handler.h>
#include <vkapi/vkapi.h>

#include "../config/config.h"

extern int module_count;
extern int act_commands_count;

// permissions
typedef enum {
  error  = -2,
  banned = 0,
  user   = 1,
  helper = 2,
  admin  = 3
} permission_t;

typedef struct {
  char *name;
  function_cmd cmd_func;

  char *prefix;
  char *description;
  permission_t permission;

  char *triggers[max_cmd_count];
} command;

typedef struct {
  function_act act_func;
  char *action_type;
} action_command;

typedef struct {
  char *name;
  char *description;

  command cmds[max_cmd_count];
  action_command a_cmds[max_cmd_count];
} module;

map_function_cmd_t commands;
action_command act_commands[16];
module modules[max_modules_count];


void modules_init();
void load_module(module *m);
void generate_help();
