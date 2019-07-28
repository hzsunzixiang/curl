#include "common.h"
#include "get_passwd.h"
#include "base64.h"
#include "curl_api.h"
#include "log.h"
#include "json.h"
#include "json_helper.h"

string SignHmac(CStr2Map &param, const string &domain, const string &secretKey)
{
    // 组合所有参数
    string signStr = getSignStr(param, false);

    signStr = "POST" + domain + "/?" + signStr;
    log_info("signStr:%s", signStr.c_str());

	string  result = gen_hmac_sha256_sign_raw(signStr, secretKey);
	log_info("hash256 result length: %ld", result.size()); 

    std::string resultBase64 = base64_encode((unsigned char const*)result.c_str(), result.size());
	log_info("result base64: %s", resultBase64.c_str()); 
    return resultBase64;
}
int SendHttps(const string &strUrlencode, const string &domain, string &strPasswd) 
{
	int itimeout = 30;
    // 单位ms
    curl_api::curl_set_timeout_ms(itimeout*1000, itimeout*1000);

    struct curl_api::curl_request_st req;
    struct curl_api::curl_respond_st res;
    string errmsg;
    
	// 请求头部
    req.heads.push_back(string("Content-Type: application/x-www-form-urlencoded"));
    if (domain.size() > 0)
	{
    	req.heads.push_back(string("Host: ") + domain);
	}
	else
	{
		log_error("host is empty return");
        return -1;
	}
	req.data =  strUrlencode;
    
	string url = "https://" + domain + "/";
    if (curl_api::curl_post(url, req, res, errmsg) != 0)
    {
    	log_error("post curl_post fail, errmsg: %s", errmsg.c_str());
        return -1;
    }

    return ParsePasswdFromJson(res.data, strPasswd);

}
int ParsePasswdFromJson(const string &strResult, string &strPasswd)
{
    //{"Response":{"Error":{"Code":"InvalidParameter","Message":"Parameter `Nonce` type error."},"RequestId":"ccb8bb9f-c6d0-48fb-9af7-2914531704e2"}}
    //{ "Response": { "Password": "a1234", "RequestId": "01c732c2-2086-4a5b-a0a8-d47766f09028", "UID": "demo@192.168.1.10:3306" } }
    // parse result to map
	Json::Value httpResponse = JsonDecode(strResult);
	if (!httpResponse.isMember("Response")				||
		httpResponse["Response"].isMember("Error"))
	{
        log_error("failed strResult:%s", strResult.c_str());
        if (httpResponse["Response"]["Error"].isMember("Code"))
        {
            log_error("failed strResult Error:%s", httpResponse["Response"]["Error"]["Code"].asString().c_str());
        }
        return -1;
    }
	if (!httpResponse["Response"].isMember("Password"))
    {
        log_error("Response has not Password field");
        return -1;
    }
	strPasswd = httpResponse["Response"]["Password"].asString();
    log_info("passwd:%c******", strPasswd.c_str()[0]);
    return 0;
}
string GetDBPassword(const string &secretId, const string &secretKey, const string &region, 
        const string &domain, const string &host, const string &port, const string &user)

{
    // 组合所有参数
	string strUID = user + "@" + host + ":" + port;
    log_info("strUID:%s", strUID.c_str());
	 
    CStr2Map param;
	param["Action"] = "DescribeDBPassword";
    srand(time(NULL));
    param["Nonce"] = toString(random() + time(NULL));
    // 可以模拟参数错误
    //param["Nonce"] = toString(random()) + toString( time(NULL));
	param["Version"] = "2019-03-14";
	param["UID"] = strUID; 
	param["Timestamp"] = toString(time(NULL));
    //可以模拟过期失败
	//param["Timestamp"] = "1563302370";
	param["Region"] = region;
	param["SignatureMethod"] = "HmacSHA256";
	//param["RequestClient"] = "SDK_PYTHON_3.0.59";
	param["RequestClient"] = "SDK_CPP0.1";
	param["SecretId"] = secretId;

	// 计算 hmac
	string resultBase64 = SignHmac(param, domain, secretKey);
	param["Signature"] = resultBase64;

	// 对参数做 urlencode
    string strUrlencode = getSignStr(param, true);
    log_info("strUrlencode:%s", strUrlencode.c_str());
    string strPasswd;
    int result = SendHttps(strUrlencode, domain, strPasswd);
    if (result != 0)
    {
        log_error("Query task failed");
        strPasswd = "";
    }


    return strPasswd;
}

