#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include "HashTable.h"
#include <string>

using namespace std;

string key = "my_key";
string value = "my_value";
HT::HTHANDLE* ht = NULL;

void InitHt(LPCSTR FileName) {
	HT::HTHANDLE* ht = HT::Create(40, 20, 64, 64, FileName);

	cout << "\ninserting...\n";
	string tempKey = "my_key";
	string tempValue = "my_value";
	char* error = NULL;
	for (int i = 0; i < 10; i++) {
		tempKey = key + to_string(i);
		tempValue = value + to_string(i);
		if (!HT::Insert(ht, new HT::Element(tempKey.c_str(), tempKey.length(), tempValue.c_str(), tempValue.length()))) {
			cout << "\nerror with inserting on " << i;
			error = HT::GetLastError(ht);
			if (error != NULL)
				cout << "\n" << error;
		}
	}
	cout << "\ninserted items:\n";
	HT::PrintAll(ht);

	HANDLE htFile = ht->FileMapping;

	HT::Close(ht);

	//CloseHandle(htFile);
	//DWORD er = GetLastError();
	delete [] error;
}


void TestPrintAll(LPCSTR FileName) {
	//HT::HTHANDLE* ht = HT::Open(FileName);

	cout << "\nPrint all:\n";
	HT::PrintAll(ht);
	//HT::Close(ht);
}

bool TestGet(LPCSTR FileName) {
	bool result = true;
	//HT::HTHANDLE* ht = HT::Open(FileName);


	HT::Element *elem = new HT::Element(key.c_str(), key.length(), value.c_str(), value.length());
	HT::Element* getElem = HT::Get(ht, elem);
	if (getElem == NULL || strcmp((char*)(elem->payload), (char*)(getElem->payload)) != 0) {
		DWORD er = GetLastError();
		result = false;
		printf("\nget error code: %d, msg: %s", er, HT::GetLastError(ht));
	}
	else {
		printf("\n Gotten element:%s, %s\n", getElem->key, getElem->payload);
	}


	//HT::Close(ht);
	return result;
}
bool TestInsert(LPCSTR FileName) {
	bool result = true;
	//HT::HTHANDLE* ht = HT::Open(FileName);


	HT::Element* elem = new HT::Element(key.c_str(), key.length(), value.c_str(), value.length());
	if (!HT::Insert(ht, elem)) {
		result = false;
		DWORD er = GetLastError();
		printf("\ninsert error. code: %d, msg: %s", er, HT::GetLastError(ht));
	}
	else {
		printf("\n Inserted element:%s, %s", elem->key, elem->payload);
	}


	//HT::Close(ht);
	return result;
}

bool TestUpdate(LPCSTR FileName) {
	bool result = true;
	//HT::HTHANDLE* ht = HT::Open(FileName);

	value = "my_valueUpdt";


	HT::Element* elem = new HT::Element(key.c_str(), key.length(), value.c_str(), value.length());
	if (!HT::Update(ht, elem, value.c_str(), value.length())) {
		result = false;
		DWORD er = GetLastError();
		printf("\nupdate error. code: %d, msg: %s", er, HT::GetLastError(ht));
	}
	else {
		printf("\n Updated element:%s, %s", elem->key, elem->payload);
	}


	//HT::Close(ht);
	return result;
}


bool TestDelete(LPCSTR FileName) {
	bool result = true;
	//HT::HTHANDLE* ht = HT::Open(FileName);


	HT::Element* elem = new HT::Element(key.c_str(), key.length(), value.c_str(), value.length());
	if (!HT::Delete(ht, elem)) {
		DWORD er = GetLastError();
		result = false;
		printf("\ndelete error code: %d, msg: %s", er, HT::GetLastError(ht));
	}
	else {
		printf("\n deleted element:%s, %s\n", elem->key, elem->payload);
	}


	//HT::Close(ht);
	return result;
}


bool TestSeveralOpenings(LPCSTR FileName) {
	bool result = true;
	DWORD er;
	ht = HT::Open(FileName);
	HT::HTHANDLE* ht2 = HT::Open(FileName);




	if (ht == NULL || ht2 == NULL)
		result = false;
	printf("\nht all:");
	HT::PrintAll(ht);
	printf("\nht2 all:");
	HT::PrintAll(ht2);




	HT::Element* elem = new HT::Element(key.c_str(), key.length(), value.c_str(), value.length());
	if (!HT::Insert(ht, elem)) {
		result = false;
		DWORD er = GetLastError();
		printf("\ninsert int ht error . code: %d, msg: %s", er, HT::GetLastError(ht));
	}
	else {
		printf("\n Inserted element int ht:%s, %s", elem->key, elem->payload);
	}

	printf("\nht all:");
	HT::PrintAll(ht);
	printf("\nht2 all:");
	HT::PrintAll(ht2);

	printf("\nht closing....");
	HT::Close(ht);

	key = key + "ht2";
	value = value + "ht2";
	elem = new HT::Element(key.c_str(), key.length(), value.c_str(), value.length());
	if (!HT::Insert(ht2, elem)) {
		result = false;
		DWORD er = GetLastError();
		printf("\ninsert in ht2 error. code: %d, msg: %s", er, HT::GetLastError(ht2));
	}
	else {
		printf("\n Inserted element in ht2 :%s, %s", elem->key, elem->payload);
	}

	printf("\nht2 all:");
	HT::PrintAll(ht2);

	HT::Close(ht2);


	printf("\nopening ht again...");
	ht = HT::Open(FileName);
	printf("\nht all:");
	HT::PrintAll(ht);
	HT::Close(ht);

	return result;
}


void TestParallel(LPCSTR FileName) {
	ht = HT::Open(FileName);

	cout << "\ninserting...\n";
	string tempKey = "my_key";
	string tempValue = "my_value";
	char* error = NULL;
	int i = 0; 
	DWORD er;
	HT::Element* elem = new HT::Element(tempKey.c_str(), tempKey.length(), tempValue.c_str(), tempValue.length());
	while(true) {
		if (!HT::Insert(ht, elem)) {
			er = GetLastError();
			printf("\n%dinsert error code: %d, msg: %s", i, er, HT::GetLastError(ht));
		}
		else {
			printf("\ninserted %d", i);
		}
		Sleep(min(i + 50, 500));
		if (!HT::Delete(ht, elem)) {
			er = GetLastError();
			printf("\n%ddelete error code: %d, msg: %s",i, er, HT::GetLastError(ht));
		}else {
			printf("\n-----deleted %d", i);
		}
		//Sleep(min(i + 50, 500));
		i++;
	}
	HT::Close(ht);
	delete[] error;
}


bool TestSnapShot(LPCSTR FileName) {
	bool result = true;
	//HT::HTHANDLE* ht = HT::Open(FileName);

	string tempKey = "my_key";
	string tempValue = "my_value";
	char* error = NULL;
	int i = 33;
	while (true) {
		Sleep(3000);

		tempKey = key + to_string(i);
		tempValue = value + to_string(i);
		if (!HT::Insert(ht, new HT::Element(tempKey.c_str(), tempKey.length(), tempValue.c_str(), tempValue.length()))) {
			cout << "\nerror with inserting on " << i;
			error = HT::GetLastError(ht);
			if (error != NULL)
				cout << "\n" << error;
		}
		i++;
		if (!HT::Snap(ht)) {
			DWORD er = GetLastError();
			result = false;
			printf("\nsnap error code: %d, msg: %s", er, HT::GetLastError(ht));
		}
		else {
			printf("\nsnap success");

		}
	}

	//HT::Close(ht);
	return result;
}

void DoTestCases(LPCSTR FileName) {

	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("\\") + 1;
	string filePath = path.substr(0, pos) + FileName;
	DWORD er;
	char* filePathCost = (char*)filePath.c_str();

	InitHt(filePathCost);
	char choise;
	bool cycle = true;
	while (cycle) {
		printf("\n0: Exit. 1: Crud test. 2: Several table handles. 3: Parallel test. 4: SnapShot test");
		cin >> choise;
		switch (choise) {
		case '0':cycle = false; break;
		case '1':
			ht = HT::Open(filePathCost);
			TestGet(filePathCost);
			TestInsert(filePathCost);
			TestGet(filePathCost);
			TestUpdate(filePathCost);
			TestGet(filePathCost);
			TestDelete(filePathCost);
			TestGet(filePathCost);
			HT::Close(ht);
			break;
		case '2':
			TestSeveralOpenings(filePathCost);
			break;
		case '3':
			TestParallel(filePathCost);
			break;
		case '4':
			ht = HT::Open(filePathCost);
			TestSnapShot(filePathCost);
			HT::Close(ht);
			break;
		}
	}

	//TestSnapShot(filePathCost);
	//TestParallel(filePathCost);
	//TestSeveralOpenings(filePathCost);

	//TestInsert(filePathCost);
	//TestInsert(filePathCost);
	//TestGet(filePathCost);
	//TestUpdate(filePathCost);
	//TestGet(filePathCost);
	//TestDelete(filePathCost);
	//TestGet(filePathCost);
	//TestPrintAll(filePathCost);

}
int main()
{
    DoTestCases("map3.txt");
    system("pause"); 
    return 0;
}