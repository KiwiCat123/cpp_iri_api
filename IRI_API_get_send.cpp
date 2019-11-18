#include "IRI_API_get_send.h"
#include <cstdlib>
#include <cstring>

IRI_API_get_send::IRI_API_get_send()
{
	curl = curl_easy_init(); //Init curl

	//set SSL, HEADER
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	HeaderList = curl_slist_append(HeaderList, "Content-Type: application/json");
	HeaderList = curl_slist_append(HeaderList, "X-IOTA-API-Version: 1");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, HeaderList);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

	errbuf[0] = '\0';
}

IRI_API_get_send::~IRI_API_get_send()
{
	curl_easy_cleanup(curl); //cleanup
	delete pURL;
	delete command;
}

void IRI_API_get_send::GetNodeInfo()
{
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"command\": \"getNodeInfo\"}");
}

void IRI_API_get_send::GetNeighbors()
{
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"command\": \"getNeighbors\"}");
}

void IRI_API_get_send::interruptAttachingToTangle()
{
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"command\": \"interruptAttachingToTangle\"}");
}

void IRI_API_get_send::wereAddressesSpentFrom(const IOTAaddress addAddress[], int count)
{
	CURLcode ret;
	string addresses;

	delete[] command;

	//build json string with addresses
	addresses = "{\"command\": \"wereAddressesSpentFrom\",\"addresses\": [";
	for (int i = 0; i < count; i++) {
		addresses.append("\"");
		addresses.append(addAddress[i]);
		addresses.append("\"");
		if (i != count - 1) addresses.append(",");
	}
	addresses.append("]}");

	size_t Size = addresses.size() + 1;
	command = new char[Size];
	strcpy(command, addresses.c_str());
	ret = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, command);
}

void IRI_API_get_send::setURL(const std::string& newURL)
{
	size_t Size = newURL.size() + 1; //get string size
	pURL = (char*)realloc(pURL, Size); //change URL size
	if (pURL == nullptr) return;
	strcpy(pURL, newURL.c_str()); //set new URL
	CURLcode ret = curl_easy_setopt(curl, CURLOPT_URL, pURL);
}

void IRI_API_get_send::setURL(const char* pNewURL)
{
	size_t Size = strlen(pNewURL) + 1; //get size
	pURL = (char*)realloc(pURL, Size); //change URL size
	if (pURL == nullptr) return;
	strcpy(pURL, pNewURL); //set new URL
	CURLcode ret = curl_easy_setopt(curl, CURLOPT_URL, pURL);
}

void IRI_API_get_send::reset()
{
	curl_easy_reset(curl); //reset

	//set URL, Header, SSL again
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, HeaderList);
	curl_easy_setopt(curl, CURLOPT_URL, pURL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
}

size_t IRI_API_get_send::writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}