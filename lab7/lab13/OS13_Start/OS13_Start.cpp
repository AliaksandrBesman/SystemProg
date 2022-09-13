#include <iostream>
#include "../SimpleClient/HashTable.h"
#include "../SimpleClient/HashTableAPI.h"
#include <string>

using namespace std;

string key = "my_key";
string value = "my_value";
HT::HTHANDLE* ht = NULL;


void StartHT(LPCSTR FileName) {
	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("/") + 1;
	string filePath = path.substr(0, pos) + FileName;
	char* filePathCost = (char*)filePath.c_str();


	HT::HTHANDLE* ht = HTAPI::Open(filePathCost);

	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost, ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
		wprintf(L"user group:%s\n", ht->HTUsersGroup);
		wprintf(L"\nFilemap name: %s\n", ht->fileMapName);
	}
	else {
		printf("\n some error fail@");
	}
	printf("\n enter some KEY to snap and closse...\n");

	system("pause");
	if (HTAPI::Snap(ht)) {
		printf("\nsuccessful snap)");
	}
	else {
		printf("\nfeil snap(");
	}
	HTAPI::Close(ht);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RU");

	char* fileName = new char[]{"D:/Учеба/6 сем/Системное программирование/Lab7/lab13/map4.txt"};
	if (argc == 2) {
		fileName = argv[1];
	}
	else {
		printf("\nProgram need 1 parameter! argc: %d.\nDefault parameters are used", argc);
		//argv[1] = (char*)"map4.txt";
	}
	if (HTAPI::OpenApi()) {
		StartHT(fileName);


		if (HTAPI::CloseApi() == false)
			printf("error with close api\n");
	}
	else {
		printf("error with open api\n");
	}
	return 0;
}