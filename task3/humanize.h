#ifndef HUMANIZE_H
#define HUMANIZE_H

/*
 * On success, return a non-negative number of how many words were modified
 * On failure, return -1
 */
int humanize_file(const char *file_name);

#endif
