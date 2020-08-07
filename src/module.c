#include <module.h>
#include <utils/stringutils.h>

int module_count = 0;

void load(module *m) {
  printf("Loading module '%s': ", m->name);

  if (module_count == max_modules_count){
    printf("ERROR. max_modules_count reached\n");
    return;
  }

  modules[module_count] = *m;

  for (int i = 0; i < m->triggers_count; i++) {
    map_set(&commands, va("%s%s", m->prefix, m->triggers[i]), m->func);
  }

  printf("OK\n");
  module_count++;
}
