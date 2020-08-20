#include <curl.h>

void *curl_init() { return curl_easy_init(); }

static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *userp) {
  size_t realsize          = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);

  if (ptr == NULL) {
    fprintf(stderr, "not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

// make post request Function returns allocated cstring
char *curl_post(char *url, char *post) {
  CURLcode res;
  curl_easy_reset(curl_handle);

  struct MemoryStruct chunk;

  chunk.memory = malloc(1);
  chunk.size   = 0;

  if (curl_handle) {
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, post);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
      exit(EXIT_FAILURE);
    }

    return chunk.memory;
  }
  return NULL;
}
