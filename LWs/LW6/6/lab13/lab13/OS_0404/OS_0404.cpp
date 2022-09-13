#include <iostream>
#include "../SimpleClient/HashTable.h"
#include "../SimpleClient/HashTableAPI.h"
//#include "HashTableAPI.h"
//#include "HashTable.h"
#include <string>

using namespace std;

string key = "my_key";
string value = "my_value";
HT::HTHANDLE* ht = NULL;




void Do(LPCSTR FileName) {
	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("\\") + 1;
	string filePath = path.substr(0, pos) + FileName;
	char* filePathCost = (char*)filePath.c_str();
	DWORD er;
	int arr[51];
	for (int i = 0; i < 51; i++) 
	{
		arr[i] = 0;
	}
	HT::HTHANDLE* ht = HTAPI::Open(filePathCost);
	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost, ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
	}
	else {
		printf("\n some error fail@: %d", GetLastError());
		return;
	}
	srand(time(NULL));
	HT::Element* element;
	int random;
	
	int value = 0;
	while (true) {
		random = rand() % 50;
		element = new HT::Element((void*)&random, 4, &value, 4);
		element = HTAPI::Get(ht, element);
		if (element != NULL) {
			arr[random] += 1;
			printf("\nSuccessful getting element with key: %d. Value: %d", *(int*)element->key, arr[random]-1);
			element->payload=(void*)arr[random];
			if (HTAPI::Update(ht, element, &value, 4) || arr[random]<4) {
				printf("\nSuccessful updating element with key: %d. NewValue: %d", random, arr[random]);
			}
			else {
				printf("\nFail with updating key: %d. ", random);
			}
		}
		else {
			printf("\nFail with getting key: %d. ", random);
		}
		Sleep(1000);
	}


	HTAPI::Close(ht);
}

int main(int argc, char* argv[])
{
	cout << "Input name of file" << endl;
	string filename;
	cin >> filename;
	if (HTAPI::OpenApi()) {

		if (argc == 2) {
		}
		else {
			printf("\nProgram need 1 parameter! argc: %d.\nDefault parameters are used", argc);
			argv[1] = (char*)filename.c_str();
		}
		Do(argv[1]);


		if (HTAPI::CloseApi() == false)
			printf("error with close api\n");
	}
	else {
		printf("error with open api\n");
	}
	return 0;
}