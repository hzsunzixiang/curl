#pragma once

string GetDBPassword(const string &secretId, const string &secretKey, const string &region, 
        const string &domain, const string &host, const string &port, const string &user);
int SendHttps(const string &strUrlencode, const string &domain, string &strPasswd);
string SignHmac(CStr2Map &param, const string &domain, const string &secretKey);
int ParsePasswdFromJson(const string &strResult, string &strPasswd);


/*
| 错误码 | 错误描述 |
|----------|----------|
| AuthFailure.InvalidSecretId | 密钥非法（不是云 API 密钥类型）。 |
| AuthFailure.MFAFailure | MFA 错误。 |
| AuthFailure.SecretIdNotFound | 密钥不存在。 |
| AuthFailure.SignatureExpire | 签名过期。 |
| AuthFailure.SignatureFailure | 签名错误。 |
| AuthFailure.TokenFailure | token 错误。 |
| AuthFailure.UnauthorizedOperation | 请求未 CAM 授权。 |
| DryRunOperation | DryRun 操作，代表请求将会是成功的，只是多传了 DryRun 参数。 |
| FailedOperation | 操作失败。 |
| InternalError | 内部错误。 |
| InvalidAction | 接口不存在。 |
| InvalidParameter | 参数错误。 |
| InvalidParameterValue | 参数取值错误。 |
| LimitExceeded | 超过配额限制。 |
| MissingParameter | 缺少参数错误。 |
| NoSuchVersion | 接口版本不存在。 |
| RequestLimitExceeded | 请求的次数超过了频率限制。 |
| ResourceInUse | 资源被占用。 |
| ResourceInsufficient | 资源不足。 |
| ResourceNotFound | 资源不存在。 |
| ResourceUnavailable | 资源不可用。 |
| UnauthorizedOperation | 未授权操作。 |
| UnknownParameter | 未知参数错误。 |
| UnsupportedOperation | 操作不支持。 |
| UnsupportedProtocol | http(s)请求协议错误，只支持 GET 和 POST 请求。 |
| UnsupportedRegion | 接口不支持所传地域。 |
*/


// {"Response":{"Error":{"Code":"InvalidParameter","Message":"Parameter `Nonce` type error."},"RequestId":"5587f800-e5b1-4441-9c53-9237e22468cb"}}
// {"Response":{"Error":{"Code":"InvalidAction","Message":"The request `Action` is invalid or not found."},"RequestId":"8100d04b-71eb-477e-994d-eb002c5c68c8"}}
// {"Response":{"Error":{"Code":"AuthFailure.SignatureExpire","Message":"The signature is expired, please request again."},"RequestId":"12302dc2-1fbe-48d0-b007-2465664f9ee7"}}
//
