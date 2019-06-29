


# 缺少选项 CURLOPT_SSL_VERIFYPEER
CURLOPT_SSL_VERIFYPEER.pcap
curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
// 如果缺少这一句
//curl_easy_perform() failed: Peer certificate cannot be authenticated with given CA certificates



# 缺少选项 CURLOPT_SSL_VERIFYHOST 
CURLOPT_SSL_VERIFYHOST.pcap

curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
// 如果缺少这一句
//curl_easy_perform() failed: SSL peer certificate or SSH remote key was not OK

