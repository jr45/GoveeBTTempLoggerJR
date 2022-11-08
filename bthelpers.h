#ifndef BTHELPERS_H
#define BTHELPERS_H

#include <cstddef>
#include <cstdio>

typedef unsigned char __u8;

typedef struct {
	__u8 b[6];
} __attribute__((packed)) bdaddr_t;

void *bt_malloc(size_t size);
void baswap(bdaddr_t *dst, const bdaddr_t *src);   
char *batostr(const bdaddr_t *ba);
int ba2str(const bdaddr_t *ba, char *str);
int bachk(const char *str);
int str2ba(const char *str, bdaddr_t *ba);
bdaddr_t *strtoba(const char *str);

#endif // BTHELPERS_H