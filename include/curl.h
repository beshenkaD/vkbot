#pragma once

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

CURL *curl_handle;

void *curl_init();
char *curl_post(char *url, char *post);
