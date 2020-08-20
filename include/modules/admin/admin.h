#pragma once

#include <module.h>
#include <usersDb.h>
#include <vkapi/methods.h>

#define NO_ARGS "Недостаточно аргументов. Используйте помощь"
#define NO_PERM "У вас недостаточно прав для выполнения этой команды"
#define BAD_ARG "Неверный аргумент: %s"

extern command Kick;
extern command Info;
extern command Set;
extern command Ban;
extern command Heads;

extern action_command Ban_act;

void update_admins_db(message *msg);
