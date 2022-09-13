#include <iostream>
#include <Windows.h>
#include <iostream>
#include "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um/winsvc.h"

#define SERVICENAME L"Demidenko"
//#define SERVICENAME L"4JI6EPT"
#define SERVICEPATH L"E:\\University\\SP\\lab7\\lab13\\Release\\Lab15.exe"


char* errortxt(const char* msg, int code) {
	char* buf = new char[512];
	sprintf_s(buf, 512, "%s: error code = %d\n", msg, code);
	return buf;
}

int main()
{
	setlocale(LC_ALL, "ru");
	SC_HANDLE schService = NULL, schSCManager = NULL;
	try{
		schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (!schSCManager) throw errortxt("OpenSCManager", GetLastError());
		else {
			schService = CreateService(
				schSCManager,
				SERVICENAME,
				SERVICENAME,
				SERVICE_ALL_ACCESS,
				SERVICE_WIN32_OWN_PROCESS,
				SERVICE_AUTO_START,
				SERVICE_ERROR_NORMAL,
				SERVICEPATH,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL);
			if (!schService) throw errortxt("CreateService", GetLastError());
		}
	}
	catch (char* txt) { std::cout << txt << "\n"; }
	if (schSCManager) CloseServiceHandle(schSCManager);
	if (schService) CloseServiceHandle(schSCManager);
	system("pause");
}