#pragma once

#include <string>
#include <map>
#include <time.h>
#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <string.h>
#include "log.h"

using namespace std;

typedef long long LONG;
typedef map<string, string> CStr2Map;

#define NOT_NULL_STR(p) (p == NULL ? "" : p)

string toString( int value );

string toString( long value );

string toString(LONG value);


/*
 * 在iContent取出位于iKey1 和iKey2 之间的子串
 */
string extractValueByKeys( char *iContent, const char *iKey1, const char *iKey2 );

/*
 * 根据请求原串生成sha1签名
 */
string gen_sha1_sign( const string &sign_src );

/*
 * 根据请求原串生成hmac-sha256签名
 */
string gen_hmac_sha256_sign( const string &sign_src, const string &key );
string gen_hmac_sha256_sign_raw( const string &sign_src, const string &key );

/*
 * char转化为16进制
 */
string char2hex( char c );

/*
 * 二进制转化为十六进制字符串
 */
void binary2hex( char *bin, int binlen, char *dest );

 
/*
 * 获取当前的日月
 */
string GetMonthDate();

string GetTodayDate();


/*
 * xml转换为MAP
 */
void parseXmlToMap( const string &content, CStr2Map &outMap );

/*
 * 删除子串及其后面的内容
 */
string delete_substring( string src, const string &sub_string );

string genSHA1Sign( const std::string &strSrc );
string genSHA256Sign( const std::string &strSrc );

/*
 * 得到不做任何urldecode变换的MAP
 */
void getRawMap( const string &strRawQryData, CStr2Map &rawMap );

/*
 * 得到末尾不拼接&的签名原串
 */
string urlEncodeForBody(CStr2Map &reqMap, bool urlEncode);

string Str2UTF8( const string &strSrc );



string sha256(const string& strSrc);

string map2Xmlstring(CStr2Map inputMap);

string replaceSpecialChar(const string &strSrc);

string replaceSpecialWchar(const string &strSrc, const string &locale = "zh_CN.UTF-8");

char* getMd5(const char *key, int len, char *szRes, int iResSize);

string getMd5(const string &srcText);

string appendUrl(const string& url, const string& str);

string getSignStr(CStr2Map &reqMap, bool urlEncode);


int GetDBInfo(string &db_name, string &ip, string &port, string &user, string &password);

