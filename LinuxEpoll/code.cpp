
#include "code.h"

int code_convert(char *fromCharset, char *toCharset, 
		char *inbuf, size_t inlen,
		char *outbuf, size_t outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;
	cd = iconv_open(toCharset, fromCharset);
	if(cd == 0){
		return -1;
	}
	memset(outbuf, 0, outlen);
	if(iconv(cd, pin, &inlen, pout, &outlen) == -1){
		return -1;
	}
	iconv_close(cd);
	return 0;
}

int utog(char *inbuf, size_t inlen, char *outbuf, size_t outlen){
	return code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
}

int gtou(char *inbuf, size_t inlen, char *outbuf, size_t outlen){
	return code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen);
}
