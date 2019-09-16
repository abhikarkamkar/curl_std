/*++

Module Name:
	Source.cpp.

Abstract:

Revision History:
	Date:	October 8 2018.
	Author:	Abhishek Karkamkar.
	Desc:	Created.

--*/


/////////////////////////////////////////////////////////////////////
//	H E A D E R S
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Source.h"


/////////////////////////////////////////////////////////////////////
//	G L O B A L S.
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
//	F U N C T I O N D E F I N I T I O N
/////////////////////////////////////////////////////////////////////

size_t
callbackfunc(
	void *ptr,
	size_t size,
	size_t nmemb,
	ResponseData *userdata
	)
{
	size_t newlen = userdata->stDataLen + (size * nmemb);

	userdata->pchRes = (char *)realloc(userdata->pchRes, newlen + 1);

	memcpy((userdata -> pchRes + userdata ->stDataLen), ptr, size*nmemb);

	*(userdata->pchRes + newlen) = '\0';
	userdata->stDataLen = newlen;

	return (size * nmemb);
}


void
GetRandomStr(
		char *pszString,
		const int ilen
		)
{

	// current time is given as seed
	// so that every time random string is generated
	srand(time(0));

	 const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int iLoopCount = 0; iLoopCount < (ilen-1); ++iLoopCount)
	{
		pszString[iLoopCount] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	pszString[ilen-1] = '\0';
}

int
Process()
{
	char szData[50];
	CURL *pCurlHandle;
	CURLcode CurlResCode;
	ResponseData ServerResponse;
	struct curl_slist *pCurlHeaders;

	ServerResponse.stDataLen = 0;
	ServerResponse.pchRes = NULL;

	pCurlHandle = curl_easy_init();
	curl_easy_setopt(pCurlHandle, CURLOPT_CUSTOMREQUEST, REQUEST_TYPE);
	curl_easy_setopt(pCurlHandle, CURLOPT_URL, REQUEST_URL);

	pCurlHeaders = NULL;
	pCurlHeaders = curl_slist_append(pCurlHeaders, HEADER_CONTENT_TYPE);
	curl_easy_setopt(pCurlHandle, CURLOPT_HTTPHEADER, pCurlHeaders);

	curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, callbackfunc);
	curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &ServerResponse);

	curl_easy_setopt(pCurlHandle, CURLOPT_TCP_KEEPALIVE, 1L);

	for(;;)
	{
		//Sleep(6000);
		int i;
		std::cout<<"\nEnter 1 to send data\n";
		std::cin>>i;

		if(i != 1) break;

		GetRandomStr(szData, 20);

		curl_easy_setopt(
							pCurlHandle,
							CURLOPT_POSTFIELDS,
							szData
							);

		CurlResCode = curl_easy_perform(pCurlHandle);
		if(CURLE_OK != CurlResCode)
		{
			printf("\ncurl_easy_perform() Failed error :%d [%s]\n", CurlResCode, curl_easy_strerror(CurlResCode));
			return 1;
		}

		std::cout<<"\nSent data :- "<<szData<<std::endl;

	}
	printf("\nResponse => %s\n", ServerResponse.pchRes);

	curl_easy_cleanup(pCurlHandle);

	return 0;
}

int main()
{

	int iRet = Process();
	if (iRet != 0)
	{
		printf("\nFailed to Process");
	}

	printf("\nSuccess");

	return 0;
}
