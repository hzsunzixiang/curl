#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

int main()
{
    printf("Version : \t%s\n", curl_version());
	return 0;
}


//Version :       libcurl/7.52.1 OpenSSL/1.0.2s zlib/1.2.8 libidn2/0.16 libpsl/0.17.0 (+libidn2/0.16) libssh2/1.7.0 nghttp2/1.18.1 librtmp/2.3

