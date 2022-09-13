#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "Service.h"

#include "../SimpleClient/HashTable.h"
#include "../SimpleClient/HashTableAPI.h"
//#include "HashTableAPI.h"
//#include "HashTable.h"
#include <string>
#include <algorithm>
#include <thread>



using namespace std;

WCHAR ServiceName[] = SERVICENAME;
SERVICE_STATUS_HANDLE hServiceStatusHandle;
SERVICE_STATUS ServiceStatus;
char* fileName = (char*)"E:/map4.txt";

void trace(const char* msg, int r) {
	std::ofstream out;
	out.open(TRACEPATH, r);
	out << msg << "\n";
	out.close();
}
void trace(const wchar_t* msg, int r) {
	std::wofstream out;
	out.open(TRACEPATH, r);
	out << msg << "\n";
	out.close();
}

HT::HTHANDLE* ht = NULL;


void Delete() {
	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("/") + 1;
	string filePath = path.substr(0, pos) + fileName;
	char* filePathCost = (char*)filePath.c_str();
	DWORD er;

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
	random = rand() % 50;
	element = new HT::Element((void*)&random, 4, &value, 4);
	if (HTAPI::Delete(ht, element)) {
		printf("\nSuccessful deleting key: %d", random);
		trace("Successful deleting key");
	}
	else {
		trace("Fail with deleting key");
		printf("\nFail with deleting key: %d. Message: %s", random, HTAPI::GetLastHtError(ht));
	}
	Sleep(2000);



	HTAPI::Close(ht);
}

void Insert() {
	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("/") + 1;
	string filePath = path.substr(0, pos) + fileName;
	char* filePathCost = (char*)filePath.c_str();
	DWORD er;

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

	random = rand() % 50;
	element = new HT::Element((void*)&random, 4, (void*)&value, 4);
	if (HTAPI::Insert(ht, element)) {
		printf("\nSuccessful inserting key: %d", random);
		trace("Successful inserting key ");
	}
	else {
		trace("Fail with inserting key ");
		printf("\nFail with inserting key: %d. Message: %s", random, HTAPI::GetLastHtError(ht));
	}
	Sleep(1700);

	HTAPI::Close(ht);
}


void Update() {
	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("/") + 1;
	string filePath = path.substr(0, pos) + fileName;
	char* filePathCost = (char*)filePath.c_str();
	DWORD er;

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

	random = rand() % 50;
	element = new HT::Element((void*)&random, 4, &value, 4);
	element = HTAPI::Get(ht, element);
	if (element != NULL) {
		printf("\nSuccessful getting element with key: %d. Value: %d", *(int*)element->key, *(int*)element->payload);
		value = *(int*)element->payload + 1;
		if (HTAPI::Update(ht, element, &value, 4)) {
			trace("Successful updating element ");
			printf("\nSuccessful updating element with key: %d. OldValue: %d", *(int*)element->key, *(int*)element->payload);
		}
		else {
			trace("Fail with updating key ");

			printf("\nFail with updating key: %d. Message: %s", random, HTAPI::GetLastHtError(ht));
		}
	}
	else {
		printf("\nFail with getting key: %d. Message: %s", random, HTAPI::GetLastHtError(ht));
	}
	Sleep(2800);


	HTAPI::Close(ht);
}

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

void StartHT() {
	trace("start");

	//path of mapping file
	char cwd[MAX_PATH];
	GetModuleFileNameA(NULL, cwd, MAX_PATH);
	string path(cwd);
	int pos = path.find_last_of("/") + 1;
	string filePath = path.substr(0, pos) + fileName;
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

	if (HTAPI::Snap(ht)) {
		printf("\nsuccessful snap)");
	}
	else {
		printf("\nfeil snap(");
	}
	HTAPI::Close(ht);
}


VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv) {
	trace("\nServiceMain\n");
	char temp[121];
	wchar_t wtemp[121];

	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	if (!(hServiceStatusHandle = RegisterServiceCtrlHandler(ServiceName, ServiceHandler))) {
		sprintf_s(temp, "\nSetServiceStatusFailed, error code = %d\n", GetLastError());
		trace(temp);
	}
	else {

		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus)) {
			sprintf_s(temp, "\nSetSetviceStatus failed, error code =%d\n", GetLastError());
			trace(temp);
		}
		else {
			
			trace("start service", std::ofstream::out);
			try {

				int capacity = 2000, keyLen = 4, valLen = 4, snapTime = 20;

				if (HTAPI::OpenApi()) {
					CreateHT(fileName, capacity, keyLen, valLen, snapTime);


					if (HTAPI::CloseApi() == false)
						printf("error with close api\n");
				}
				else {
					printf("error with open api\n");
				}


				if (HTAPI::OpenApi()) {
					std::thread t1(StartHT);
					t1.join();

					if (HTAPI::CloseApi() == false)
						printf("error with close api\n");
				}
				else {
					printf("error with open api\n");
				}

				if (HTAPI::OpenApi()) {
					while (true)
					{
						Insert();
						Delete();
						Update();
					}


					if (HTAPI::CloseApi() == false)
						printf("error with close api\n");
				}
				else {
					printf("error with open api\n");
				}
			}
			catch (char* e) {
				trace(e);
			}

		}
	}

}


VOID WINAPI ServiceHandler(DWORD fdwControl) {
	char temp[121];
	switch (fdwControl) {
	case SERVICE_CONTROL_STOP:
		trace("stopede\n");
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		trace("shutdownene\n");

		break;
	case SERVICE_CONTROL_PAUSE:
		ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_INTERROGATE: break;
	default:
		sprintf_s(temp, "Unrecognized opcode%d\n", fdwControl);
		trace(temp);
	};
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus)) {

		sprintf_s(temp, "SetServiceStatus failed, error code = %d\n", GetLastError());
		trace(temp);
	}
}
