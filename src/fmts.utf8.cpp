#include "fmts.utf8.h"

#include <string.h>
#include <stdlib.h>

#define UTF8_GOOD_RESULT 0
#define UTF8_BAD_RESULT 1
#define UTF8_NO_RESULT 2

/* Return allocated string containing UTF8 string converted from encoding fromcode */
int utf8( const char * fromcode, char * inputbuf, char ** outputbuf )
{
	iconv_t cd;
	char * outputptr;
	size_t outbytesleft;
	size_t inbytesleft;

	inbytesleft = strlen(inputbuf);

	cd = iconv_open("UTF-8", fromcode);

	if ( cd == ((iconv_t)(-1)) )
		return UTF8_NO_RESULT;

	outbytesleft = inbytesleft * 3 + 1; /* UTF8 string can be 3 times larger */
	/* then local string */
	*outputbuf = (char *)malloc(outbytesleft);

	if (!*outputbuf)
		return UTF8_NO_RESULT;

	memset(*outputbuf, 0, outbytesleft);
	outputptr = *outputbuf;

	if ( iconv(cd, &inputbuf, &inbytesleft, &outputptr, &outbytesleft) == -1 )
	{
		free(*outputbuf);
		iconv_close(cd);
		return UTF8_BAD_RESULT;
	}

	iconv_close(cd);
	return UTF8_GOOD_RESULT;
}
