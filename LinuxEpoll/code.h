

#ifndef _CODE_H
#define _CODE_H

#include <iconv.h>
#include <string.h>

int utog(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
int gtou(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

#endif
