#include "synonym.h"
#include <curl/curl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define MAX_URL_SIZE 256
#define MAX_SYN_SIZE 256
#define MAX_RESPONSE_SIZE 2048

typedef struct {
  char data[MAX_RESPONSE_SIZE];
  size_t len;
} ResponseBuffer;

static CURL *curl;
static CURLcode res;
static int curl_global_ready = 0;

size_t write_response(void *data, size_t size, size_t nmemb, void *userdata) {
  ResponseBuffer *buf = userdata;
  size_t total = size * nmemb;

  if (buf->len + total >= sizeof(buf->data)) {
    return 0;
  }

  memcpy(buf->data + buf->len, data, total);
  buf->len += total;
  buf->data[buf->len] = '\0';
  return total;
}

int extract_synonym(const char *response, char *synonym, size_t syn_size) {
  const char *key = strstr(response, "\"word\":\"");
  if (key == NULL) {
    return 1;
  }
  key += strlen("\"word\":\"");

  const char *end = strchr(key, '"');
  if (end == NULL) {
    return 1;
  }

  size_t len = (size_t)(end - key);
  if (len == 0 || len >= syn_size) {
    return 1;
  }

  memcpy(synonym, key, len);
  synonym[len] = '\0';
  return 0;
}

int should_ignore_word(const char *word) {
  // GENERATED list of most common words that don't need synonyms
  const char *skip_word_array[] = {
      "the",     "of",      "and",    "a",      "to",      "in",    "is",
      "you",     "that",    "it",     "he",     "was",     "for",   "on",
      "are",     "as",      "with",   "his",    "they",    "i",     "at",
      "be",      "this",    "have",   "from",   "or",      "had",   "by",
      "but",     "not",     "what",   "all",    "were",    "we",    "when",
      "your",    "can",     "there",  "an",     "each",    "which", "she",
      "do",      "how",     "their",  "if",     "will",    "up",    "other",
      "about",   "out",     "many",   "then",   "them",    "these", "so",
      "some",    "her",     "would",  "him",    "into",    "has",   "more",
      "no",      "could",   "my",     "than",   "been",    "who",   "am",
      "did",     "may",     "over",   "only",   "very",    "after", "our",
      "just",    "too",     "any",    "same",   "also",    "does",  "those",
      "me",      "us",      "must",   "should", "whether", "here",  "because",
      "before",  "through", "during", "under",  "between", "while", "until",
      "against", "among",   "might",  "shall",  "own",     NULL};
  for (int i = 0; skip_word_array[i] != NULL; i++) {
    if (strcmp(word, skip_word_array[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

int get_synonym(const char *word, char *buf, const size_t buf_size) {
  if (should_ignore_word(word)) {
    snprintf(buf, buf_size, "%s", word);
    return 0;
  }

  if (!curl_global_ready) {
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
      fprintf(stderr, "CURL: curl_global_init() failed. (%s)",
              curl_easy_strerror(res));
      return -1;
    }
    curl_global_ready = 1;
  }

  curl = curl_easy_init();
  if (curl == NULL) {
    fprintf(stderr,
            "CURL: curl_easy_init() failed. variable CURL *curl is NULL");
    return -1;
  }

  // compile the url
  char url[MAX_URL_SIZE];
  snprintf(url, sizeof(url), "https://api.datamuse.com/words?ml=%s&max=1",
           word);

  res = curl_easy_setopt(curl, CURLOPT_URL, url);
  if (res != CURLE_OK) {
    fprintf(stderr, "CURL: curl_easy_setopt() failed. (%s)",
            curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return -1;
  }

  ResponseBuffer response = {0};
  res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
  if (res != CURLE_OK) {
    fprintf(stderr, "CURL: curl_easy_setopt() failed. (%s)",
            curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return -1;
  }
  res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  if (res != CURLE_OK) {
    fprintf(stderr, "CURL: curl_easy_setopt() failed. (%s)",
            curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return -1;
  }
  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "CURL: curl_easy_perform() failed. (%s)",
            curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return -1;
  }

  curl_easy_cleanup(curl);

  char synonym[MAX_SYN_SIZE];
  if (extract_synonym(response.data, synonym, sizeof(synonym)) != 0) {
    fprintf(stderr, "extract_synonym() failed for response: %s\n",
            response.data);
    return -1;
  }

  snprintf(buf, buf_size, "%s", synonym);
  printf("%s => %s\n", word, synonym);
  return 1;
}
