#ifndef SYNONYM_H
#define SYNONYM_H
#include <stddef.h>

/* -1 on error
 *  0 on ignored word (buf holds the original word)
 *  1 on altered word (buf holds a synonym)
 */
int get_synonym(const char *word, char *buf, const size_t buf_size);

#endif
