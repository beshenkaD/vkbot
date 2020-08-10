#include <module.h>
#include <utils/stringutils.h>

int module_count = 0;

void load(command *m) {
  for (int i = 0; i < m->triggers_count; i++) {
    map_set(&commands, va("%s%s", m->prefix, m->triggers[i]), m->func);
  }
}

void load_module(module *m) {
  printf("Loading module '%s': ", m->name);
  modules[module_count] = *m;

  if (module_count == max_modules_count){
    printf("ERROR. max_modules_count reached\n");
    return;
  }

  for (int i = 0; i < m->cmds_count; i++) {
    load(&m->cmds[i]);
  }
  printf("OK\n");

  module_count++;
}
