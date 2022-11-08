#include "bthelpers.h"

#include <cstdlib>
#include <cstring>
#include <ctype.h>

void *bt_malloc(size_t size)
{
    return malloc(size);
}

void baswap(bdaddr_t *dst, const bdaddr_t *src)
{
    register unsigned char *d = (unsigned char *) dst;
    register const unsigned char *s = (const unsigned char *) src;
    register int i;

    for (i = 0; i < 6; i++)
        d[i] = s[5-i];
}
   
char *batostr(const bdaddr_t *ba)
{
    char *str = (char*)bt_malloc(18);
    if (!str)
        return NULL;

    sprintf(str, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
        ba->b[0], ba->b[1], ba->b[2],
        ba->b[3], ba->b[4], ba->b[5]);

    return str;
}

int ba2str(const bdaddr_t *ba, char *str)
{
    return sprintf(str, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
        ba->b[5], ba->b[4], ba->b[3], ba->b[2], ba->b[1], ba->b[0]);
}

int bachk(const char *str)
{
    if (!str)
        return -1;

    if (strlen(str) != 17)
        return -1;

    while (*str) {
        if (!isxdigit(*str++))
            return -1;

        if (!isxdigit(*str++))
            return -1;

        if (*str == 0)
            break;

        if (*str++ != ':')
            return -1;
    }

    return 0;
}
  
int str2ba(const char *str, bdaddr_t *ba)
{
    int i;

    if (bachk(str) < 0) {
        memset(ba, 0, sizeof(*ba));
        return -1;
    }

    for (i = 5; i >= 0; i--, str += 3)
        ba->b[i] = strtol(str, NULL, 16);

    return 0;
}
  
bdaddr_t *strtoba(const char *str)
{
    bdaddr_t b;
    bdaddr_t *ba = (bdaddr_t *)bt_malloc(sizeof(*ba));

    if (ba) {
        str2ba(str, &b);
        baswap(ba, &b);
    }

    return ba;
}
