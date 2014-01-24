#ifndef UTF_8_H_INCLUDE

#define UTF_8_H_INCLUDE

#include <iconv.h>

#define HAVE_ICONV_H
int utf8(const char * fromcode, char * inputbuf, char ** outputbuf);

#endif
