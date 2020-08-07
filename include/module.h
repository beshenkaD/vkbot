#pragma once 

#include <vkapi.h>
#include <handler.h>

#include "../config/config.h"

#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

extern int module_count;

typedef struct {
  char *name;
  function func;
  char *prefix;
  char *description;
  int triggers_count;
  char *triggers[];
} module;

map_function_t commands;
module modules[max_modules_count];

void modules_init();
void load(module *m);
