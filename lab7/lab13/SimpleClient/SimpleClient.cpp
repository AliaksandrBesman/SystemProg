#include <objbase.h>
#include <iostream>
#include "HashTable.h"
#include "HashTableAPI.h"
#include <string>

using namespace std;

int key = 12;
string value = "my_value";
LPCSTR FileName = "comHt.txt";

int main()
{
	if (HTAPI::OpenApi()) {
		HT::HTHANDLE* ht;


		char cwd[MAX_PATH];
		GetModuleFileNameA(NULL, cwd, MAX_PATH);
		string path(cwd);
		int pos = path.find_last_of("\\") + 1;
		string filePath = path.substr(0, pos) + FileName;
		DWORD er;
		char* filePathCost = (char*)filePath.c_str();

		ht = HTAPI::Create(20,20,4,64, filePathCost, L"HT");

		HT::Element* el = new HT::Element((void*)&key, sizeof(key), (void*)value.c_str(), value.length());
		if (!HTAPI::Insert(ht, el)) {
			printf("error with inserting el\n");
			printf(HTAPI::GetLastHtError(ht));
		}
		HT::Element* el2;
		el2 = HTAPI::Get(ht, el);
		if (el==NULL){
			printf(HTAPI::GetLastHtError(ht));
		}
		else {
			printf("got value: %s\n", (char*)el2->payload);
		}


		if (HTAPI::Close(ht) == false)
			printf("error with closing ht\n");

		if (HTAPI::CloseApi() == false)
			printf("error with close api\n");
	}
	else {
		printf("error with open api\n");
	}
	system("pause");
	return 0;
}

