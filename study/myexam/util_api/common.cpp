#include "common.h"

#include <openssl/hmac.h>
#include "urlcodec.h"
#include "get_passwd.h"
#include "ini_parser.h"
#include "constant.h"

string toString( int value )
{
    char szTmp[128];
    snprintf( szTmp, sizeof( szTmp ), "%d", value );
    return szTmp;
}

string toString( long value )
{
    char szTmp[128];
    snprintf( szTmp, sizeof( szTmp ), "%ld", value );
    return szTmp;
}

/**
 * 将整数转换为字符串
 */
string toString(LONG value)
{
    char szTmp[128];
    snprintf(szTmp, sizeof(szTmp), "%lld", value);
    return szTmp;
}

string extractValueByKeys( char *iContent, const char *iKey1, const char *iKey2 )
{
    char *p1 = NULL, *p2 = NULL;
    
    string result;
    
    p1 = strstr( iContent, iKey1 );
    
    if( p1 == NULL )
    {
        return "";
    }
    
    if( NULL == p1 + 1 )
    {
        return "";
    }
    
    
    p2 = strstr( p1 + 1, iKey2 );
    
    if( p2 == NULL )
    {
        return "";
    }
    
    result.assign( p1 + strlen( iKey1 ), p2 - ( p1 + strlen( iKey1 ) ) );
    return result;
}


#if OPENSSL_VERSION_NUMBER < 0x10100000L
/*
 * 根据请求原串生成hmac-sha256签名
 */
string gen_hmac_sha256_sign(const string &sign_src, const string &key)
{
    // 算出hmac-sha256
    char md[EVP_MAX_MD_SIZE];
    unsigned int len;
    const EVP_MD * engine = NULL;
    engine = EVP_sha256();

    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, key.c_str(), key.size(), engine, NULL);
    HMAC_Update(&ctx, (unsigned char*) sign_src.c_str(), sign_src.size());

    HMAC_Final(&ctx, (unsigned char*) md, &len);
    HMAC_CTX_cleanup(&ctx);

    // 转化为字符串
    char buff[2 * EVP_MAX_MD_SIZE + 1];
    memset(buff, 0, sizeof(buff));
    binary2hex(md, len, buff);

    return string(buff);
}
#else

/*
 * 根据请求原串生成hmac-sha256签名
   https://developer.akamai.com/blog/2017/02/14/upgrade-openssl-110
 */
string gen_hmac_sha256_sign(const string &sign_src, const string &key)
{
    // 算出hmac-sha256
    char md[EVP_MAX_MD_SIZE];
    unsigned int len;
    const EVP_MD * engine = NULL;
    engine = EVP_sha256();

	HMAC_CTX *ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, key.c_str(), key.size(), engine, NULL);
    HMAC_Update(ctx, (unsigned char*) sign_src.c_str(), sign_src.size());

    HMAC_Final(ctx, (unsigned char*) md, &len);
	HMAC_CTX_free(ctx);

    // 转化为字符串
    char buff[2 * EVP_MAX_MD_SIZE + 1];
    memset(buff, 0, sizeof(buff));
    binary2hex(md, len, buff);

    return string(buff);
}
#endif

#if OPENSSL_VERSION_NUMBER < 0x10100000L
/*
 * 根据请求原串生成hmac-sha256签名
 */
string gen_hmac_sha256_sign_raw(const string &sign_src, const string &key)
{
    // 算出hmac-sha256
    char md[EVP_MAX_MD_SIZE];
    unsigned int len;
    const EVP_MD * engine = NULL;
    engine = EVP_sha256();

    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, key.c_str(), key.size(), engine, NULL);
    HMAC_Update(&ctx, (unsigned char*) sign_src.c_str(), sign_src.size());

    HMAC_Final(&ctx, (unsigned char*) md, &len);
    HMAC_CTX_cleanup(&ctx);

    return string(md, len);
}
#else

/*
 * 根据请求原串生成hmac-sha256签名
   https://developer.akamai.com/blog/2017/02/14/upgrade-openssl-110
 */
string gen_hmac_sha256_sign_raw(const string &sign_src, const string &key)
{
    // 算出hmac-sha256
    char md[EVP_MAX_MD_SIZE];
    unsigned int len;
    const EVP_MD * engine = NULL;
    engine = EVP_sha256();

	HMAC_CTX *ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, key.c_str(), key.size(), engine, NULL);
    HMAC_Update(ctx, (unsigned char*) sign_src.c_str(), sign_src.size());

    HMAC_Final(ctx, (unsigned char*) md, &len);
	HMAC_CTX_free(ctx);

    return string(md, len);
}
#endif

/*
 * char转化为16进制
 */
string char2hex( char c )
{
    std::string result;
    char first, second;
    
    first = ( c & 0xF0 ) / 16;
    first += first > 9 ? 'A' - 10 : '0';
    second = c & 0x0F;
    second += second > 9 ? 'A' - 10 : '0';
    
    result.append( 1, first );
    result.append( 1, second );
    
    return result;
}

/*
 * 二进制转化为十六进制字符串
 */
void binary2hex( char *bin, int binlen, char *dest )
{
    string tmp;
    
    for( int i = 0; i < binlen; i++ )
    {
        tmp = char2hex( bin[i] );
        dest[i * 2] = tmp[0];
        dest[i * 2 + 1] = tmp[1];
    }
}

std::string genSHA1Sign( const std::string &strSrc )
{
    unsigned char hash[20];
    char outputBuffer[41] = {0};
    SHA_CTX sha1;
    SHA1_Init( &sha1 );
    SHA1_Update( &sha1, strSrc.c_str(), strSrc.length() );
    SHA1_Final( hash, &sha1 );
    int i = 0;
    
    for( i = 0; i < 20; i++ )
    {
        snprintf( outputBuffer + ( i * 2 ), 2, "%02x", hash[i] );
    }
    
    binary2hex( ( char * )hash, 20, outputBuffer );
    
    return outputBuffer;
}

std::string genSHA256Sign( const std::string &strSrc )
{
    unsigned char hash[32];
    char outputBuffer[65] = {0};
    SHA256_CTX sha256;
    SHA256_Init( &sha256 );
    SHA256_Update( &sha256, strSrc.c_str(), strSrc.length() );
    SHA256_Final( hash, &sha256 );
    int i = 0;
    
    for( i = 0; i < 32; i++ )
    {
        snprintf( outputBuffer + ( i * 2 ), 2, "%02x", hash[i] );
    }
    
    binary2hex( ( char * )hash, 32, outputBuffer );
    
    return outputBuffer;
}

void getRawMap(const string &strRawQryData, CStr2Map &rawMap)
{
    std::string::size_type iStart = 0;
    std::string::size_type iTmp;
    for(iTmp = strRawQryData.find("&");
        iTmp != std::string::npos;
        iTmp = strRawQryData.find("&", iStart))
    {
        string strSub = strRawQryData.substr(iStart, iTmp - iStart);
        std::string::size_type pos = strSub.find("=");
        if (pos != std::string::npos)
        {
            string key = strSub.substr(0, pos);
            string value = strSub.substr(pos + 1, strSub.size() - pos - 1);
            rawMap[key] = value;
        }
        iStart = iTmp + 1;
    }

    string strSub = strRawQryData.substr(iStart);
    std::string::size_type pos = strSub.find("=");
    if (pos != std::string::npos)
    {
        string key = strSub.substr(0, pos);
        string value = strSub.substr(pos + 1, strSub.size() - pos - 1);
        rawMap[key] = value;
    }
}


//  参数拼接， 做 urlEncode 
string urlEncodeForBody(CStr2Map &reqMap, bool urlEncode)
{
    string result = "";
    int count = 0;

    CStr2Map::iterator iter = reqMap.begin();
    for ( ; iter != reqMap.end(); ++iter)
    {
        count++;
        result += iter->first;
        result += "=";
        if(urlEncode){
            result += regUrlEncode(iter->second);
        }else{
            result += iter->second;
        }

        if (count < (int)reqMap.size())
        {
            result += "&";
        }
    }

    return result;
}
string sha256(const string& strSrc)
{
    if (strSrc.empty())
    {
        return "";
    }
    
    unsigned char szDest[32 + 1] = {0};
    SHA256((const unsigned char *)(strSrc.c_str()), strSrc.length(), szDest);
     // 转化为字符串
    char buff[64 + 1] = {0};
    binary2hex((char *)szDest, 32, buff );
    
    return string( buff );
}


/**
 * 入参不要出现中文字符
 */
string replaceSpecialChar(const string &strSrc)
{
    int iLen = strSrc.length();

    if (strSrc.empty() || 0 == iLen)
    {
        log_info("Input string is empty!");
        return "";
    }

    string strDst = strSrc;

    for (int i = 0; i < iLen; ++i)
    {
        char cTmp = strDst[i];
        bool bLegalCharFlag = (cTmp >= 'a' && cTmp <= 'z') 
                                || (cTmp >= 'A' && cTmp <= 'Z') 
                                || (cTmp >= '0' && cTmp <= '9') 
                                || (cTmp == '.');
        if (!bLegalCharFlag)
        {
            strDst[i] = ' ';
        }
    }

    strDst.erase(0, strDst.find_first_not_of(" "));
    strDst.erase(strDst.find_last_not_of(" ") + 1);

    log_info("strSrc string is [%s], strDst string is [%s]", strSrc.c_str(), strDst.c_str());
    return strDst;
}

/**
 * 支持所有字符
 */
string replaceSpecialWchar(const string &strSrc, const string &locale/* = "zh_CN.UTF-8"*/)
{
    int iLen = strSrc.length();

    if (strSrc.empty() || 0 == iLen)
    {
        log_info("Input string is empty!");
        return "";
    }

    if(iLen > 128)
    {
        log_error("src string is too long, iLen: %d", iLen);
        return strSrc;
    }

    char *pOldLocale = setlocale(LC_ALL, NULL);
    if (NULL == pOldLocale)
    {
        log_error("Get old locale info failed!");
    }
    setlocale(LC_ALL, locale.c_str());
    wchar_t wcsDst[128];
    wmemset(wcsDst, 0, sizeof(wcsDst) / sizeof(wchar_t));
    mbstowcs(wcsDst, strSrc.c_str(), iLen);

    for (int i = 0; i < (int)wcslen(wcsDst); ++i)
    {
        wchar_t wcTmp = wcsDst[i];
        bool bLegalCharFlag = (wcTmp >= L'a' && wcTmp <= L'z') 
                                || (wcTmp >= L'A' && wcTmp <= L'Z') 
                                || (wcTmp >= L'0' && wcTmp <= L'9') 
                                || (wcTmp == L'.');
        if (!bLegalCharFlag)
        {
            wcsDst[i] = L' ';
        }
    }

    char cDst[512] = {0};
    wcstombs(cDst, wcsDst, sizeof(wcsDst));
    string strDst = cDst;

    strDst.erase(0, strDst.find_first_not_of(" "));
    strDst.erase(strDst.find_last_not_of(" ") + 1);

    log_info("strSrc string is [%s], strDst string is [%s]", strSrc.c_str(), strDst.c_str());
    setlocale(LC_ALL, pOldLocale);
    return strDst;
}

/**
 * 获取MD5摘要
 */
char* getMd5(const char *key, int len, char *szRes, int iResSize)
{
    MD5_CTX md5c;
    unsigned char digest[16];	
    MD5_Init(&md5c);
    MD5_Update(&md5c, key, len);
    MD5_Final(digest, &md5c);
    
    for (int i = 0; i < 16; i++) 
    {
        snprintf(szRes + i * 2, iResSize - i * 2, "%02x", digest[i]);
    }

    szRes[32] = '\0';
    return szRes;
}

string getMd5(const string &srcText)
{
    char md5_buf[32 + 1] = {0};
    memset(md5_buf, 0, sizeof(md5_buf));
    
    getMd5(srcText.c_str(), srcText.length(), md5_buf, sizeof(md5_buf));

    return md5_buf;
}

string appendUrl(const string& url, const string& str)
{
    std::string s = url;

    if (url.find('?') == std::string::npos)
        s += '?';

    char chLast = *(s.rbegin());
    if (chLast != '?' && chLast != '&')
        s += '&';

    s += str;
    return s;
}


// 末尾不拼接 & 
string getSignStr(CStr2Map &reqMap, bool urlEncode)
{
    string result = "";
    int count = 0;

    CStr2Map::iterator iter = reqMap.begin();
    for ( ; iter != reqMap.end(); ++iter)
    {
        count++;
        result += iter->first;
        result += "=";
        if(urlEncode){
            result += regUrlEncode(iter->second);
        }else{
            result += iter->second;
        }

        if (count < (int)reqMap.size())
        {
            result += "&";
        }
    }

    return result;
}

int GetDBInfo(string &db_name, string &ip, string &port, string &user, string &password)
{
	Comm_Config conf(DES_CONF_FILE);
    int iReturn = 0;
	if ( (iReturn = conf.Init()) != 0)
	{
		log_error("%s Comm_Config::Init failed. errorcode=%d", __FUNCTION__, iReturn);
		return -1;
	}

    string secretId = "";
    string secretKey = "";
    string domain = "";
    string region = "";

	conf.ReadItem("db", "db", "", db_name);
	conf.ReadItem("db", "ip", "", ip);
	conf.ReadItem("db", "port", "", port);
	conf.ReadItem("db", "user", "", user);
	conf.ReadItem("db", "password", "", password);


    log_info("db_name:%s, ip:%s, port:%s, user:%s, secretId:%s, secretKey:%s, domain:%s, region:%s", 
            db_name.c_str(), ip.c_str(), port.c_str(), user.c_str(), 
            secretId.c_str(), secretKey.c_str(), domain.c_str(), region.c_str());

    // 调用API接口获取密码 
    if (password != "")
    {
        log_info("password is not empty, directly use it!");
    }
    else
    {
	    conf.ReadItem("get_passwd", "secretId", "", secretId);
	    conf.ReadItem("get_passwd", "secretKey", "", secretKey);
	    conf.ReadItem("get_passwd", "domain", "", domain);
	    conf.ReadItem("region", "region", "", region);

        if (secretId == "" || secretKey == "" || region == "" || domain =="" || ip == "" || port == "" || user == "")
        {
            log_error("secretId | secretKey | region | domain | ip | port | user is empty!");
            return -1;
        }
        password = GetDBPassword(secretId, secretKey, region, domain, ip, port, user);
        if (password == "")
        {
            log_error("get password failed");
            return -1;
        }
        else
        {
            log_info("success password: %c****\n", password.c_str()[0]);
        }
    }

    return 0;

}


