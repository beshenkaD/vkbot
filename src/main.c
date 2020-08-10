#include <main.h>
#include <modules/std.h>
#include <modules/admin.h>

#include <unistd.h>

bool running = true;

void get_time() {
  time_t rawtime;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  printf("Bot started. at: %s\n", asctime(timeinfo));
}

int main(int argc, char **argv) {
  users_init();
  get_time();

  module std = {"Standart", "Стандартные функции бота.", 3, {Ping, Stat, Off}};
  module adm = {"Admin", "Функции для администрирования бесед", 2, {Kick, Set}};
  load_module(&std);
  load_module(&adm);

  vk_init();
  
  while (running) {
    cJSON *lp = get_longpoll_json();
    message *msg = get_longpoll_events(lp);

    handler(msg);

    cJSON_Delete(lp);
    free(msg);
  }

  users_deinit();
  vk_destroy();
  return 0;
}

