#include "synonym.h"
#include <curl/curl.h>
#include <stdio.h>

static CURL *curl;

static inline int init_curl(void) {
  CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
  if (res != 0) {
    fprintf(stderr, "CURL: curl_global_init() failed.");
    return 1;
  }
  curl = curl_easy_init();
  if (curl == NULL) {
    fprintf(stderr, "CURL: curl_easy_init() failed.");
    return 1;
  }
  return 0;
}

// static int compile_url(const char *word, const int buf_elements, char *url) {
//   return 0;
// }

int get_synonym(const char *word, char *buf[], const int buf_elements,
                const int element_size) {
  int err = init_curl();
  if (err != 0) {
    fprintf(stderr, "GET_SYNONYM: failed to initalize curl.");
  }
  // char *url;
  // compile_url(word, buf_elements, url);
  if (curl) {
    // curl_easy_setopt(curl, CURLOPT_URL, url);
  }
  return 0;
}
