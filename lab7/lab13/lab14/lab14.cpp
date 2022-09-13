#include <iostream>
#include "../SimpleClient/HashTable.h"
#include "../SimpleClient/HashTableAPI.h"
#include <string>

using namespace std;

int printMenu() {
	cout << "\nEnter option(0-exit, 1-create ht, 2-open with current, 3-open with other user):";
	int res;
	cin >> res;
	return res;
}

void createHt() {
	string FileName;
	cout << "\n Enter file name:";
	cin >> FileName;
	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("\\") + 1;
	string filePath = path.substr(0, pos) + FileName;
	char* filePathCost = (char*)filePath.c_str();

	int capacity = 2000, keyLen = 4, valLen = 4, snapTime = 20;

	HT::HTHANDLE* ht = HTAPI::Create(capacity, snapTime, keyLen, valLen, filePathCost, L"HT");

	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost, snapTime, capacity, keyLen, valLen);
		wprintf(L"\nuser group:%s\n", ht->HTUsersGroup);
	}
	else {
		printf("\n some error fail@");
	}

	HTAPI::Close(ht);
}
void openHt() {
	string FileName;
	cout << "\n Enter file name:";
	cin >> FileName;
	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("\\") + 1;
	string filePath = path.substr(0, pos) + FileName;
	char* filePathCost = (char*)filePath.c_str();

	//int capacity = 2000, keyLen = 4, valLen = 4, snapTime = 20;
	HT::HTHANDLE* ht = HTAPI::Open(filePathCost);
	//HT::HTHANDLE* ht = HTAPI::Create(capacity, snapTime, keyLen, valLen, filePathCost, L"HT");

	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost, ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
		wprintf(L"\nuser group:%s\n", ht->HTUsersGroup);
	}
	else {
		printf("\n some error fail@");
	}

	HTAPI::Close(ht);
}
void openHtUser() {
	string FileName;
	cout << "\n Enter file name:";
	cin >> FileName;
	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("\\") + 1;
	string filePath = path.substr(0, pos) + FileName;
	char* filePathCost = (char*)filePath.c_str();
	
	wstring user, pass;
	cout << "\nenter username and password:";
	wcin >> user;
	wcin >> pass;
	//int capacity = 2000, keyLen = 4, valLen = 4, snapTime = 20;
	HT::HTHANDLE* ht = HTAPI::Open(filePathCost,user.c_str(),pass.c_str());
	//HT::HTHANDLE* ht = HTAPI::Create(capacity, snapTime, keyLen, valLen, filePathCost, L"HT");

	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost, ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
		wprintf(L"\nuser group:%s\n", ht->HTUsersGroup);
	}
	else {
		printf("\n some error fail@");
	}

	HTAPI::Close(ht);
}

int main(int argc, char* argv[])
{

	if (HTAPI::OpenApi()) {

		int option = 0;
		do {
			option = printMenu();
			if (option == 1) {
				createHt();
			}
			else if (option == 2) {
				openHt();
			}
			else if (option == 3) {
				openHtUser();
			}
		} while (option != 0);

		if (HTAPI::CloseApi() == false)
			printf("error with close api\n");
	}
	system("pause");
	return 0;
}