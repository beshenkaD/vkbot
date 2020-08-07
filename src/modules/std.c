#include <modules/std.h>
#include <main.h>
#include <sys/resource.h>
#include <utils/stringutils.h>
#define maxSize 512

void ping(message *msg) {
  messages_send("pong", msg->peer_id);  //
}
module Ping = {"Ping", ping, "/", "Проверка работоспособности бота.", 4, {"ping", "пинг", "Ping", "Пинг"}};

void stat(message *msg) {
  /* get module names */
  char result[maxSize] = {0x0};
  for (int i = 0; i < module_count; i++) {
    strncat(result, va("「%s」 ", modules[i].name), maxSize - strlen(result) - 1);
  }

  /* get usage */
  struct rusage r_usage;
  getrusage(RUSAGE_SELF, &r_usage);

  float mem = r_usage.ru_maxrss / 1000.0;

  messages_send(va("Бот запущен: %s\nЗагружен с модулями: %s\nПотребление памяти: %01.3f MB", asctime(timeinfo), result, mem), msg->peer_id);
}
module Stat = {"Statistic", stat, "/", "Статистика бота.", 4, {"stat", "стат", "Stat", "Стат"}};

void off(message *msg) {
  if (msg->from_id == hostid)
    running = false;
  else
    messages_send("Access denied. Only host can run this command", msg->peer_id);
}
module Off = {"Off", off, "/", "Отключает бота.", 1, {"off"}};
