/* CURL ½Ó¿Ú·â×° */

#include "curl_api.h"

#define ErrorLog printf
#define InfoLog printf
#define DebugLog printf
namespace curl_api
{
    long g_conn_timeout_ms = (5*1000);
    long g_timeout_ms = (60*1000);

    void curl_set_timeout_ms(long conn_timeout, long timeout)
    {
        g_conn_timeout_ms = conn_timeout;
        g_timeout_ms = timeout;
    }

    int curl_writer(char *data, size_t size, size_t nmemb, string *writer_data)
    {
        if (writer_data == NULL)
            return 0;
        //InfoLog("size[%d], data[%s]=====",size*nmemb, data);

        writer_data->append(data, size*nmemb);

        //InfoLog("p_writer_data[%d] write_data=[%s]==== write_data", writer_data, (*writer_data).c_str());

        return size * nmemb;
    }

#define _setopt(name, value) \
    do { \
        ret = curl_easy_setopt(curl, name, value); \
        if (ret != CURLE_OK) { \
            string _tmp = #name; \
            err = "curl_easy_setopt[" + _tmp + "] error:"; \
            err += curl_easy_strerror(ret); \
            curl_easy_cleanup(curl); \
            curl_global_cleanup(); \
            return -1; \
        } \
    } while(0)

    int curl_run(const string &url, const curl_request_st &req, curl_respond_st &res, string &err, int type)
    {
        CURLcode ret = curl_global_init(CURL_GLOBAL_ALL);
        if (ret != CURLE_OK) {
            err = "curl_global_init error:";
            err += curl_easy_strerror(ret);
            return -1;
        }

        CURL *curl = curl_easy_init();
        if (curl == NULL) {
            err = "curl_easy_init error";
            err += curl_easy_strerror(ret);
            curl_global_cleanup();
            return -1;
        }

        _setopt(CURLOPT_NOSIGNAL, 1);
        _setopt(CURLOPT_CONNECTTIMEOUT_MS, g_conn_timeout_ms);
        _setopt(CURLOPT_TIMEOUT_MS, g_timeout_ms);
        _setopt(CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        _setopt(CURLOPT_URL, url.c_str());
        _setopt(CURLOPT_WRITEFUNCTION, curl_writer);
        _setopt(CURLOPT_HEADERDATA, &(res.heads));
        _setopt(CURLOPT_WRITEDATA, &(res.data));
        //_setopt(CURLOPT_SSL_VERIFYHOST, 2);
        _setopt(CURLOPT_SSL_VERIFYHOST, 0);

        if (type == _curl_type_post) {
            _setopt(CURLOPT_POST, 1);
            _setopt(CURLOPT_POSTFIELDS, req.data.c_str());
            _setopt(CURLOPT_POSTFIELDSIZE, req.data.size());
        }

        if (req.cookies.size() > 0) {
            _setopt(CURLOPT_COOKIE, req.cookies.c_str());
        }

        if (req.cert_file.size() > 0) {
            InfoLog("set CURLOPT_SSLCERT=[%s]", req.cert_file.c_str());
            _setopt(CURLOPT_SSLCERT, req.cert_file.c_str());
            InfoLog("set CURLOPT_SSLCERTTYPE=[%s]", req.cert_type.c_str());
            _setopt(CURLOPT_SSLCERTTYPE, req.cert_type.c_str());
            if (!req.cert_key_file.empty())
            {
                InfoLog("set CURLOPT_SSLKEY=[%s]", req.cert_key_file.c_str());
                _setopt(CURLOPT_SSLKEY, req.cert_key_file.c_str());
                InfoLog("set CURLOPT_SSLKEYTYPE=[%s]", req.cert_type.c_str());
                _setopt(CURLOPT_SSLKEYTYPE, req.cert_type.c_str());
            }
            if (!req.cert_passwd.empty())
            {
                InfoLog("set CURLOPT_SSLCERTPASSWD=[%s]", req.cert_passwd.c_str());
                _setopt(CURLOPT_SSLCERTPASSWD, req.cert_passwd.c_str());
            }
        }
        if (req.cert_ca.size() > 0) {
            InfoLog("set CURLOPT_CAINFO=[%s]", req.cert_ca.c_str());
            _setopt(CURLOPT_SSL_VERIFYPEER, 1);
            _setopt(CURLOPT_CAINFO, req.cert_ca.c_str());
        } else {
            _setopt(CURLOPT_SSL_VERIFYPEER, 0);
        }

        if (req.proxy.size() > 0) {
            _setopt(CURLOPT_PROXY,req.proxy.c_str());
        }

        struct curl_slist *slist = NULL;
        if (req.heads.size() > 0) {
            for (size_t i = 0; i < req.heads.size(); i++) {
                slist = curl_slist_append(slist, req.heads[i].c_str());
            }
            _setopt(CURLOPT_HTTPHEADER, slist);
        }

        ret = curl_easy_perform(curl);
        if (ret != CURLE_OK) {
            err = "curl_easy_perform error:";
            err += curl_easy_strerror(ret);
            if (slist) {
                curl_slist_free_all(slist);
            }
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return -1;
        }

        if (slist) {
            curl_slist_free_all(slist);
        }
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        //InfoLog("res.data[%s]", res.data.c_str());
        return 0;

    }

    int curl_get(const string &url, const curl_request_st &req, curl_respond_st &res, string &err)
    {
        return curl_run(url, req, res, err);
    }

    int curl_post(const string &url, const curl_request_st &req, curl_respond_st &res, string &err)
    {
        return curl_run(url, req, res, err, _curl_type_post);
    }
}
