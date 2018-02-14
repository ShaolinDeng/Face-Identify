#include <iostream>
#include <curl/curl.h>
#include "detect.h"
#include <string.h>
#include <json.h>
#include <fstream>
#include <sstream>


const static std::string face_detect_url = "https://aip.baidubce.com/rest/2.0/face/v1/detect";
static std::string face_detect_result;
/**
* curl����http������õĻص��������ص������жԷ��ص�json��ʽ��body�����˽������������������ȫ�ֵľ�̬��������
* @param ���������libcurl�ĵ�
* @return ����ֵ�����libcurl�ĵ�
*/
static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	// ��ȡ����body�����ptr�У��Ƚ���ת��Ϊstring��ʽ
	face_detect_result = std::string((char *)ptr, size * nmemb);
	return size * nmemb;
}
/**
* �����������ӿڣ�����json��ʽ�Ľ���������ʽ�������ٶȴ����ĵ�
* @param json_result ��string��ʽ���ص�json��ʽ�Ľ��
* @param image_base64 ��string��ʽ���ݵ�base64�����ͼ�����ݣ�ע��base64���ݲ�������ʽ��Ϣ����������data:image/jpeg;base64����
* @param access_token ��string��ʽ�����access token���ݣ�access token��ȡ��ʽ��access_token��ȡ����ĵ�������
* @return ���óɹ�����0���������󷵻�����������
*/
int face_detect(std::string &json_result, const std::string &image_base64, std::string &access_token) {
	std::string url = face_detect_url + "?access_token=" + access_token;
	CURL *curl = NULL;
	CURLcode result_code;
	int is_success;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_httppost *post = NULL;
		curl_httppost *last = NULL;
		curl_formadd(&post, &last, CURLFORM_COPYNAME, "image", CURLFORM_COPYCONTENTS, image_base64.data(),
			CURLFORM_END);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			is_success = 1;
			return is_success;
		}
		json_result = face_detect_result;
		curl_easy_cleanup(curl);
		is_success = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		is_success = 1;
	}
	return is_success;
}
std::string readFileIntoString(char * filename)
{
	std::ifstream ifile(filename);
	//���ļ����뵽ostringstream����buf��  
	std::ostringstream buf;
	char ch;
	while (buf&&ifile.get(ch))
		buf.put(ch);
	//������������buf�������ַ���  
	return buf.str();
}


/*int main()
{
	std::string access_token;
	const static std::string AK = "eb3M8gTn19iYbghBb3qceQDz";
	const static std::string SK = "y1XLzsvQyVb3TdCvtjY4jtxhWK3GCyw0";
	get_access_token(access_token,AK,SK);
	std::string result;
	std::string test;
	char * fn = "C:\fakepath\f32837fd8878959e95839ad65df26f07.jpg";
	test = readFileIntoString(fn);

	face_detect(result, test, access_token);
	std::cout << result << std::endl;

}*/