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
* curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在全局的静态变量当中
* @param 参数定义见libcurl文档
* @return 返回值定义见libcurl文档
*/
static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	// 获取到的body存放在ptr中，先将其转换为string格式
	face_detect_result = std::string((char *)ptr, size * nmemb);
	return size * nmemb;
}
/**
* 调用人脸检测接口，返回json格式的结果，具体格式解析见百度大脑文档
* @param json_result 以string格式返回的json格式的结果
* @param image_base64 以string格式传递的base64编码的图像数据（注：base64数据不包含格式信息（即不包含data:image/jpeg;base64））
* @param access_token 以string格式传入的access token数据，access token获取方式见access_token获取相关文档及代码
* @return 调用成功返回0，发生错误返回其他错误码
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
	//将文件读入到ostringstream对象buf中  
	std::ostringstream buf;
	char ch;
	while (buf&&ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串  
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