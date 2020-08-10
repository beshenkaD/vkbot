#pragma once

#include <module.h>
#include <methods.h>
#include <sqlite3.h>
#include <modules/usersDb.h>

void kick(message *msg, int argc, char *argv[]);

extern command Kick;
extern command Set;
