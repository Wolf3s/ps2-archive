/*
 * sysclib.h - Standard C Library subset.
 *
 * Copyright (c) 2003 Marcus R. Brown <mrbrown@0xd6.org>
 *
 * See the file LICENSE included with this distribution for licensing terms.
 */

#ifndef IOP_SYSCLIB_H
#define IOP_SYSCLIB_H

#include "types.h"
#include "irx.h"
#include "setjmp.h"

#define sysclib_IMPORTS_start DECLARE_IMPORT_TABLE(sysclib, 1, 1)
#define sysclib_IMPORTS_end END_IMPORT_TABLE

/* These functions are already defined in setjmp.h */
#define I_setjmp DECLARE_IMPORT(4, setjmp)
#define I_longjmp DECLARE_IMPORT(5, longjmp)

char toupper(char c);
#define I_toupper DECLARE_IMPORT(6, toupper)
char tolower(char c);
#define I_tolower DECLARE_IMPORT(7, tolower)
unsigned char look_ctype_table(char character);
#define I_look_ctype_table DECLARE_IMPORT(8, look_ctype_table)
void *memchr(const void *s, int c, size_t n);
#define I_memchr DECLARE_IMPORT(10, memchr)
int memcmp(const void *p, const void *q, size_t size);
#define I_memcmp DECLARE_IMPORT(11, memcmp)
void * memcpy(void *dest, const void *src, size_t size);
#define I_memcpy DECLARE_IMPORT(12, memcpy)
void *memmove(void *dest, const void *src, size_t size);
#define I_memmove DECLARE_IMPORT(13, memmove)
void * memset(void *ptr, int c, size_t size);
#define I_memset DECLARE_IMPORT(14, memset)

int sprintf(char *str, const char *format, ...);
#define I_sprintf DECLARE_IMPORT(19, sprintf)

char *strcat(char *dest, const char *src);
#define I_strcat DECLARE_IMPORT(20, strcat)
char *strchr(const char *s, int c);
#define I_strchr DECLARE_IMPORT(21, strchr)
int strcmp(const char *p, const char *q);
#define I_strcmp DECLARE_IMPORT(22, strcmp)
char *strcpy(char *dest, const char *src);
#define I_strcpy DECLARE_IMPORT(23, strcpy)
size_t strcspn(const char *s, const char *reject);
#define I_strcspn DECLARE_IMPORT(24, strcspn)
char *index(const char *s, int c);
#define I_index DECLARE_IMPORT(25, index)
char *rindex(const char *s, int c);
#define I_rindex DECLARE_IMPORT(26, rindex)
size_t strlen(const char *s);
#define I_strlen DECLARE_IMPORT(27, strlen)
int strncat(char *dest, const char *src, size_t size);
#define I_strncat DECLARE_IMPORT(28, strncat)
int strncmp(const char *p, const char *q, size_t size);
#define I_strncmp DECLARE_IMPORT(29, strncmp)
int strncpy(char *dest, const char *src, size_t size);
#define I_strncpy DECLARE_IMPORT(30, strncpy)
char *strpbrk(const char *s, const char *accept);
#define I_strpbrk DECLARE_IMPORT(31, strpbrk)
char *strrchr(const char *s, int c);
#define I_strrchr DECLARE_IMPORT(32, strrchr)
size_t strspn(const char *s, const char *accept);
#define I_strspn DECLARE_IMPORT(33, strspn)
char *strstr(const char *haystack, const char *needle);
#define I_strstr DECLARE_IMPORT(34, strstr)
char *strtok(char *s, const char *delim);
#define I_strtok DECLARE_IMPORT(35, strtok)
long strtol(const char *s, char **endptr, int base);
#define I_strtol DECLARE_IMPORT(36, strtol)
unsigned long strtoul(const char *s, char **endptr, int base);
#define I_strtoul DECLARE_IMPORT(38, strtoul)

#endif /* IOP_SYSCLIB_H */
