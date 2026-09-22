#ifndef SYNONYM_H
#define SYNONYM_H
#include <stddef.h>

/* 0 on success
 * 1 on failure
 */
int get_synonym(const char *word, char *buf, const size_t buf_size);

#endif // !SYNONYM_H
