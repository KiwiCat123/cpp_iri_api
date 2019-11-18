#pragma once
#include <string>
#include <curl/curl.h>
using std::string;

typedef char IOTAaddress[82];

class IRI_API_get_send
{
public:
	//Init libcurl
	IRI_API_get_send();
	virtual ~IRI_API_get_send();

	//import new URL as std::string and char[]
	void setURL(const std::string& newURL);
	void setURL(const char* pNewURL);

	void GetNodeInfo();
	void GetNeighbors();
	void interruptAttachingToTangle();
	void wereAddressesSpentFrom(const IOTAaddress addAddress[], int count);

	template <class T> const char* run(T& answer) {
		CURLcode ret;

		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
		errbuf[0] = 0; //clear errors
		response_string.clear(); //clear old response
		ret = curl_easy_perform(curl);
		answer = response_string;
		return (const char*) &errbuf; //always return pointer on errbuf
	}
	void reset();
protected:
	string response_string;
	char* pURL = nullptr;
	char* command = nullptr;
	CURL* curl = nullptr; //libcurl Handle
	struct curl_slist* HeaderList = nullptr;
	char errbuf[CURL_ERROR_SIZE];

	//CURL Write function for response
	static size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data);
};

