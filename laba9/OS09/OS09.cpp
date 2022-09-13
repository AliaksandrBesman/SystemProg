#include <iostream>
#include <Windows.h>

using namespace std;


//bool printFileTxt(LPWSTR FileName) {
//	HANDLE fileHandle = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (fileHandle == INVALID_HANDLE_VALUE)
//		return false;
//
//	DWORD n = NULL;
//	char buf[2048];
//
//	ZeroMemory(buf, sizeof(buf));
//	bool readFile = ReadFile(fileHandle, &buf, sizeof(buf), &n, NULL);
//
//	cout << "File:\n" << buf << endl;
//	CloseHandle(fileHandle);
//}

bool printFileTxt(LPWSTR FileName) {
	HANDLE fileHandle = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE)
		return false;

	DWORD n = NULL;
	bool readFile;
	char buf[2048];
	do
	{
		ZeroMemory(buf, sizeof(buf));
		readFile = ReadFile(fileHandle, &buf, 4, &n, NULL);
		cout << "File:\n" << buf << endl;
	} while (n>0);
	

	
	CloseHandle(fileHandle);
}

//
bool printFileInfo(LPWSTR FileName) {
	HANDLE fileHandle = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle  == INVALID_HANDLE_VALUE)
		return false;


	_BY_HANDLE_FILE_INFORMATION fileInformation;
	if (!GetFileInformationByHandle(fileHandle , &fileInformation)) {
		CloseHandle(fileHandle );
		return false;
	}

	wstring wFilePath = wstring(FileName);
	string file = string(wFilePath.begin(), wFilePath.end());

	cout << "File name: " << file.c_str() << endl;

	cout << "File type: ";
	DWORD fileType = GetFileType(fileHandle );
	switch (fileType) {
	case FILE_TYPE_CHAR: cout << "FILE_TYPE_CHAR" << std::endl; break;
	case FILE_TYPE_DISK: cout << "FILE_TYPE_DISK" << std::endl; break;
	case FILE_TYPE_PIPE: cout << "FILE_TYPE_PIPE" << std::endl; break;
	case FILE_TYPE_REMOTE: cout << "FILE_TYPE_REMOTE" << std::endl; break;
	case FILE_TYPE_UNKNOWN: cout << "FILE_TYPE_UNKNOWN" << std::endl; break;
	default: cout << "FILE_TYPE_UNKNOWN " << endl << endl; break;
	}

	LARGE_INTEGER lFileSize;
	BOOL bGetSize = GetFileSizeEx(fileHandle, &lFileSize);
	if (!bGetSize)
		return false;
	cout << "File size: " << lFileSize.QuadPart << endl;

	FILETIME createFile = fileInformation.ftCreationTime;
	SYSTEMTIME createFileToSystemTime;
	FileTimeToSystemTime(&createFile, &createFileToSystemTime);
	cout << "Datetime of creation:  "
		<< createFileToSystemTime.wDay<< "/"
		<< createFileToSystemTime.wMonth<<"/"
		<< createFileToSystemTime.wYear
		<< "\t" << createFileToSystemTime.wHour + 3
		<< ":" << createFileToSystemTime.wMinute
		<< "\n" << endl;

	FILETIME lastUpdate = fileInformation.ftLastWriteTime;
	SYSTEMTIME lastUpdateToSysTime;
	FileTimeToSystemTime(&lastUpdate, &lastUpdateToSysTime);
	cout << "Datetime of updation:  "
		<< lastUpdateToSysTime.wDay << "/"
		<< lastUpdateToSysTime.wMonth << "/"
		<< lastUpdateToSysTime.wYear
		<< "\t" << lastUpdateToSysTime.wHour + 3
		<< ":" << lastUpdateToSysTime.wMinute
		<< "\n" << endl;


	CloseHandle(fileHandle );
	return true;

}


void main() {
	//SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "en_US.UTF-8");
	//LPCWSTR file = L"E:\\University\\SP\\laba9\\OS09_01.txt";
	LPCWSTR file = L"E:\\University\\SP\\laba9\\OS09_01_8Bytes.txt";
	cout << "-----------task 1---------" << endl;
	//if (!printFileInfo(LPWSTR(file)))
	//	cout << "Error";

	cout << "\n-----------task 2---------" << endl;
	if (!printFileTxt(LPWSTR(file)))
		cout << "Error";


	///////////////
	/*SYSTEMTIME st, lt;
	GetSystemTime(&st);
	GetLocalTime(&lt);

	cout << "\n---------------\nsystem time: " << st.wHour << ":" << st.wMinute;
	cout << "\nlocal time: " << lt.wHour << ":" << lt.wMinute << "\n---------------\n";*/
	/////////
	system("pause");
}



