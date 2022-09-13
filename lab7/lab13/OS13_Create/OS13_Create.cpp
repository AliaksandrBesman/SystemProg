#include <iostream>
#include "../SimpleClient/HashTable.h"
#include "../SimpleClient/HashTableAPI.h"
#include <string>

using namespace std;

string key = "my_key";
string value = "my_value";
HT::HTHANDLE* ht = NULL;

void CreateHT(LPCSTR FileName, int capacity, int keyLen, int valLen, int snapTime) {
	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("/") + 1;
	string filePath = path.substr(0, pos) + FileName;
	char* filePathCost = (char*)filePath.c_str();


	HT::HTHANDLE* ht = HTAPI::Create(capacity, snapTime, keyLen, valLen, filePathCost, L"HT");

	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost, snapTime, capacity, keyLen, valLen);
		wprintf(L"user group:%s\n", ht->HTUsersGroup);
	}
	else {
		printf("\n some error fail@");
	}

	HTAPI::Close(ht);
}

int main(int argc, char* argv[])
{

	setlocale(LC_ALL, "RU");
	string t = "при";
	int capacity = 2000, keyLen = 4, valLen = 4, snapTime = 20;
	if (argc == 6) {
		int temp;

		temp = atoi(argv[2]);
		if (temp != 0)
			capacity = temp;

		temp = atoi(argv[3]);
		if (temp != 0)
			keyLen = temp;

		temp = atoi(argv[4]);
		if (temp != 0)
			valLen = temp;

		temp = atoi(argv[5]);
		if (temp != 0)
			snapTime = temp;

	}
	else {
		printf("\nProgram need 5 parameters! argc: %d.\nDefault parameters are used", argc);
		argv[1] = (char*)"D:/Учеба/6 сем/Системное программирование/Lab7/lab13/map4.txt";
	}


	if (HTAPI::OpenApi()) {
		CreateHT(argv[1], capacity, keyLen, valLen, snapTime);


		if (HTAPI::CloseApi() == false)
			printf("error with close api\n");
	}
	else {
		printf("error with open api\n");
	}
	system("pause");
	return 0;
}