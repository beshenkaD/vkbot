#include <main.h>
#include <modules/std.h>
#include <modules/admin.h>

bool running = true;

void get_time() {
  time_t rawtime;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  printf("Bot started. at: %s\n", asctime(timeinfo));
}

int main(int argc, char **argv) {
  get_time();
  vk_init();

  load(&Ping);
  load(&Stat);
  load(&Off);
  load(&Kick);

  while (running) {
    cJSON *lp = get_longpoll_json();
    message *msg = get_longpoll_events(lp);

    handler(msg);

    cJSON_Delete(lp);
    free(msg);
  }

  vk_destroy();
  return 0;
}

