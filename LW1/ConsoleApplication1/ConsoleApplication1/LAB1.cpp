#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>
#include <Windows.h>
#include <fstream>
#include <locale.h>
#include <sstream>

BOOL printFileInfo(LPWSTR FileName);
BOOL printFileTxt(LPWSTR FileName);
BOOL delRowFileTxt(LPWSTR FileName, DWORD row);
BOOL insRowFileTxt(LPWSTR FileName, LPWSTR str,DWORD row);
BOOL printWatchPowFileTxt(LPWSTR FileName, DWORD mlsec);

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	LPWSTR ptr;
	{
		char text[] = "D:/Projects/OC/ConsoleApplication1/1.txt"; 
		wchar_t wtext[sizeof(text) / sizeof(text[0])];
		mbstowcs(wtext, text, strlen(text) + 1);//Plus null
		ptr = wtext;
	}

	printFileInfo(ptr);
	cout << endl;

	printFileTxt(ptr);
	cout << endl;

	//delRowFileTxt(ptr, 1);
	//delRowFileTxt(ptr, 3);
	//delRowFileTxt(ptr, 8);
	//delRowFileTxt(ptr, 10);
	//printFileTxt(ptr);

	//LPWSTR name1;
	//{
	//	char text1[] = "TTT";
	//	wchar_t wtext1[sizeof(text1) / sizeof(text1[0])];
	//	mbstowcs(wtext1, text1, strlen(text1) + 1);//Plus null
	//	name1 = wtext1;
	//}

	//LPWSTR name2;
	//{
	//	char text2[] = "YYY";
	//	wchar_t wtext2[sizeof(text2) / sizeof(text2[0])];
	//	mbstowcs(wtext2, text2, strlen(text2) + 1);//Plus null
	//	name2 = wtext2;
	//}

	//insRowFileTxt(ptr, name1, 3);
	//insRowFileTxt(ptr, name2, -1);

	//printWatchPowFileTxt(ptr, 20000);

	system("pause");
}

BY_HANDLE_FILE_INFORMATION fileinfo;

BOOL printFileInfo(LPWSTR FileName) {
	try
	{
		ZeroMemory(&fileinfo, sizeof(BY_HANDLE_FILE_INFORMATION));

		HANDLE hf = CreateFile(
			FileName,
			NULL,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hf != INVALID_HANDLE_VALUE)
		{
			if (GetFileInformationByHandle(hf, &fileinfo))
			{
				FILETIME fc;
				FILETIME fa;
				FILETIME fw;

				GetFileTime(hf, &fc, &fa, &fw);
				SYSTEMTIME cm;
				SYSTEMTIME wm;

				FileTimeToSystemTime(&fc, &cm);
				FileTimeToSystemTime(&fw, &wm);

				string fileType;
				wstring ws(FileName);
				string myVarS = string(ws.begin(), ws.end());

				switch (GetFileType(hf))
				{
				case FILE_TYPE_CHAR:
				{
					fileType = "The specified file is a character file, usually an LPT device or console.";
					break;
				}
				case FILE_TYPE_DISK:
				{
					fileType = "The specified file is a disk file.";
					break;
				}
				case FILE_TYPE_PIPE:
				{
					fileType = "The specified file is a socket, named pipe, or anonymous pipe.";
					break;
				}
				case FILE_TYPE_REMOTE:
				{
					fileType = "Not used.";
					break;
				}
				case FILE_TYPE_UNKNOWN:
				{
					fileType = "Either the type of the specified file is unknown, or an error has occurred.";
					break;
				}
				default:
					fileType = "Uknown file type";
					break;
				}

				cout << "File name: " << myVarS.substr(myVarS.find_last_of('/')+1) << endl
					<< "File type: " << fileType << endl
					<< "File type: " << myVarS.substr(myVarS.find_last_of('.') + 1) << endl
					<< "File size: " << fileinfo.nFileSizeLow << endl
					<< "Date and time created file: " << cm.wHour + 3 << ":" << cm.wMinute
					<< " " << cm.wDay << "/" << cm.wMonth << "/" << cm.wYear << endl
					<< "Date and time changed file: " << wm.wHour + 3 << ":" << wm.wMinute
					<< " " << wm.wDay << "/" << wm.wMonth << "/" << wm.wYear << endl;
			}
			CloseHandle(hf);
		}
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
 }

BOOL printFileTxt(LPWSTR FileName) {
	try
	{
		HANDLE hf = CreateFile(
			FileName,
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hf != INVALID_HANDLE_VALUE)
		{
			DWORD n = NULL;
			char read[1024];
			ZeroMemory(read, sizeof(read));

			BOOL successfulRead = ReadFile(
				hf,
				&read,
				sizeof(read),
				&n,
				NULL
			);

			if (successfulRead) {
				cout << read << endl;
			}
		}
		CloseHandle(hf);
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

BOOL delRowFileTxt(LPWSTR FileName, DWORD row) {
	try
	{
		HANDLE hf = CreateFile(
			FileName,
			GENERIC_READ|GENERIC_WRITE,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hf != INVALID_HANDLE_VALUE)
		{
			DWORD n = NULL;
			char read[1024];
			ZeroMemory(read, sizeof(read));

			BOOL successfulRead = ReadFile(
				hf,
				&read,
				sizeof(read),
				&n,
				NULL
			);

			if (successfulRead)
			{
				string result;
				int countN = 0;
				bool find = false;

				for (size_t i = 0; i < sizeof(read) / sizeof(char); i++)
				{
					if (read[i] == '\0' && read[i + 1] == '\0')
					{
						break;
					}

					if (countN == row)
					{
						if (read[i] == '\n')
						{
							find = false;
							countN++;
							continue;
						}
						else
						{
							find = true;
						}
					}

					if (read[i] == '\n' && !find)
					{
						result += read[i];
						countN++;
					}

					if (read[i] != '\n' && !find)
					{
						result += read[i];
					}
				}

				CloseHandle(hf);

				hf = CreateFile(
					FileName,
					GENERIC_WRITE,
					NULL,
					NULL,
					TRUNCATE_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL
				);

				BOOL successfulWrite=WriteFile(
					hf,
					result.c_str(),
					result.length(),
					&n,
					NULL
				);
			}
		}
		CloseHandle(hf);
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

BOOL insRowFileTxt(LPWSTR FileName, LPWSTR str, DWORD row) {
	try
	{
		HANDLE hf = CreateFile(
			FileName,
			GENERIC_READ | GENERIC_WRITE,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hf != INVALID_HANDLE_VALUE)
		{
			DWORD n = NULL;
			char read[1024];
			ZeroMemory(read, sizeof(read));

			BOOL successfulRead = ReadFile(
				hf,
				&read,
				sizeof(read),
				&n,
				NULL
			);

			if (successfulRead)
			{
				int countLines = 0;
				for (size_t i = 0; i < sizeof(read) / sizeof(char); i++)
				{
					if (read[i] == '\n')
					{
						countLines++;
					}

					if (read[i] == '\0' && read[i + 1] == '\0')
					{
						break;
					}
				}

				int newRow = ((int)row + countLines) % countLines;
				newRow += (int)row < 0 ? 2 : 0;

				string result;
				int countN = 0;
				bool find = false;

				for (size_t i = 0; i < sizeof(read) / sizeof(char); i++)
				{
					if (countN == newRow)
					{
						wstring ws(str);
						string myVarS = string(ws.begin(), ws.end());
						result += myVarS + "\n";
						countN++;
						find = true;
					}

					if (read[i] == '\0' && read[i + 1] == '\0')
					{
						break;
					}

					if (read[i] == '\n')
					{
						result += read[i];
						countN++;
					}

					if (read[i] != '\n')
					{
						result += read[i];
					}
				}

				if (!find)
				{
					wstring ws(str);
					string myVarS = string(ws.begin(), ws.end());
					result += "\n" + myVarS;
				}

				cout << result;

				CloseHandle(hf);

				hf = CreateFile(
					FileName,
					GENERIC_WRITE,
					NULL,
					NULL,
					TRUNCATE_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL
				);

				BOOL successfulWrite = WriteFile(
					hf,
					result.c_str(),
					result.length(),
					&n,
					NULL
				);

				cout << endl;
			}
		}
		CloseHandle(hf);
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

BOOL printWatchPowFileTxt(LPWSTR FileName, DWORD mlsec) {
	try
	{
		char read[1024];
		DWORD n;

		HANDLE hf = CreateFile(
			FileName,
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		BOOL successfulRead = ReadFile(
			hf,
			&read,
			sizeof(read),
			&n,
			NULL
		);

		CloseHandle(hf);

		int startCountLines = 0;
		int endCountLines = 0;

		for (size_t i = 0; i < sizeof(read) / sizeof(char); i++)
		{
			if (read[i] == '\n')
			{
				startCountLines++;
			}

			if (read[i] == '\0' && read[i + 1] == '\0')
			{
				break;
			}
		}
		cout << startCountLines<<endl;

		wstring ws(FileName);
		ws = ws.substr(0,ws.find_last_of('/'));

		if (successfulRead)
		{
			HANDLE dir = FindFirstChangeNotification(
				ws.c_str(),
				TRUE,
				FILE_NOTIFY_CHANGE_LAST_WRITE
			);

			HANDLE _mutex = ::CreateEvent(NULL, FALSE, TRUE, NULL);

			if (!_mutex)
				throw std::runtime_error("CreateEvent failed");

			if (::WaitForSingleObject(dir, mlsec) != WAIT_OBJECT_0)
				throw std::runtime_error("WaitForSingleObject failed");

			ZeroMemory(read, sizeof(read));
			Sleep(100);

			HANDLE hf = CreateFile(
				FileName,
				GENERIC_READ,
				NULL,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);

			BOOL successfulRead = ReadFile(
				hf,
				&read,
				sizeof(read),
				&n,
				NULL
			);

			if (successfulRead)
			{
				for (size_t i = 0; i < sizeof(read) / sizeof(char); i++)
				{
					if (read[i] == '\n')
					{
						endCountLines++;
					}

					if (read[i] == '\0' && read[i + 1] == '\0')
					{
						break;
					}
				}
				CloseHandle(hf);

				if (startCountLines != endCountLines)
					cout << "Count lines in file was changed" << endl;
			}
		}

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}