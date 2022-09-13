#include <iostream>
#include <Windows.h>

using namespace std;

bool printFileTxt(LPWSTR FileName) {
    HANDLE fileHandle = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
        return false;

    DWORD n = NULL;
    char buf[1024];

    ZeroMemory(buf, sizeof(buf));
    bool readFile = ReadFile(fileHandle, &buf, sizeof(buf), &n, NULL);

	cout << buf << endl;
    CloseHandle(fileHandle);
}

BOOL delRowFileTxt(LPWSTR FileName, DWORD row)
{
	HANDLE fileHandle = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE)
		return false;
	DWORD n = NULL;
	char buf[1024];
	ZeroMemory(buf, sizeof(buf));
	bool readFile = ReadFile(fileHandle, &buf, 1024, &n, NULL);
	int i = 0;
	int currentRowNum = 1;
	int filePointer = 0;
	char wbuf[1024];
	while (buf[i++] != '\0')
	{
		if (currentRowNum == row)
		{
			while (buf[i] != '\n' && buf[i] != '\0')
			{
				i++;
			}
			int y = 0;
			while (buf[i] != '\0')
			{
				i++;
				wbuf[y++] = buf[i];
			}
			SetFilePointer(fileHandle, filePointer, NULL, FILE_BEGIN);
			WriteFile(fileHandle, &wbuf, sizeof(char) * y - 1, &n, NULL);
			SetEndOfFile(fileHandle);
		}
		if (buf[i] == '\n')
			currentRowNum++;
		if (currentRowNum == row)
			filePointer = i + 1;
	}
	CloseHandle(fileHandle);
	if (currentRowNum < row)
		return false;

	return true;
}

void main()
{
	LPCWSTR file = L"E:\\University\\SP\\laba9\\OS09_01.txt";
	cout << "File before deleting:" << endl;
	printFileTxt((LPWSTR)file);
	cout << "\n\n";
	if (!delRowFileTxt((LPWSTR)file, 1))
		cout << "Error del" << endl;
	if (!delRowFileTxt((LPWSTR)file, 3))
		cout << "Error del" << endl;
	if (!delRowFileTxt((LPWSTR)file, 8))
		cout << "Error del" << endl;
	if (!delRowFileTxt((LPWSTR)file, 10))
		cout << "Error del" << endl;

	cout << "File after deleting:" << endl;
	printFileTxt((LPWSTR)file);

	system("pause");
}