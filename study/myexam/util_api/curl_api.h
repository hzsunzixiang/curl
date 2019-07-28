/* CURL ½Ó¿Ú·â×° */

#pragma once

#include <string>
#include <vector>
#include <curl/curl.h>

using namespace std;

#define _curl_type_get	0
#define _curl_type_post	1

namespace curl_api {

    struct curl_request_st {
        string cookies;
        vector<string> heads;
        string proxy;
        string cert_file;
        string cert_key_file;
        string cert_passwd;
        string cert_type;
        string cert_ca;
        string data;
    };

    struct curl_respond_st {
        string heads;
        string data;
    };

    void curl_set_timeout_ms(long conn_timeout, long timeout);
    int curl_writer(char *data, size_t size, size_t nmemb, string *writer_data);
    int curl_run(const string &url, const curl_request_st &req, curl_respond_st &res, string &err, int type=_curl_type_get);
    int curl_get(const string &url, const curl_request_st &req, curl_respond_st &res, string &err);
    int curl_post(const string &url, const curl_request_st &req, curl_respond_st &res, string &err);
}
