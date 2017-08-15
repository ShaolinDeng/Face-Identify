#include <iostream>
#include <curl/curl.h>
#include <json.h>
#include "access_token.h"
//token url
const std::string access_token_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials";
// �ص�������ȡ����access_token��ű���
// static std::string access_token_result;
/**
* curl����http������õĻص��������ص������жԷ��ص�json��ʽ��body�����˽������������������result��
* @param ���������libcurl���ĵ�
* @return ����ֵ�����libcurl���ĵ�
*/
static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	std::string s((char *)ptr, size * nmemb);
	// ��ʼ��ȡjson�е�access token��Ŀ
	Json::Reader reader;
	Json::Value root;
	// ʹ��boost�����json
	reader.parse(s, root);
	std::string* access_token_result = static_cast<std::string*>(stream);
	*access_token_result = root["access_token"].asString();
	return size * nmemb;
}

/**
* ���Ի�ȡaccess_token�ĺ�����ʹ��ʱ��Ҫ���ڰٶ��ƿ���̨������Ӧ���ܵ�Ӧ�ã���ö�Ӧ��API Key��Secret Key
* @param access_token ��ȡ�õ���access token�����ú���ʱ�贫��ò���
* @param AK Ӧ�õ�API key
* @param SK Ӧ�õ�Secret key
* @return ����0�����ȡaccess token�ɹ�����������ֵ�����ȡʧ��
*/
int get_access_token(std::string &access_token, const std::string &AK, const std::string &SK) {
	CURL *curl;
	CURLcode result_code;
	int error_code = 0;
	curl = curl_easy_init();
	if (curl) {
		std::string url = access_token_url + "&client_id=" + AK + "&client_secret=" + SK;
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		std::string access_token_result;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			return 1;
		}
		access_token = access_token_result;
		curl_easy_cleanup(curl);
		error_code = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		error_code = 1;
	}
	return error_code;
}

