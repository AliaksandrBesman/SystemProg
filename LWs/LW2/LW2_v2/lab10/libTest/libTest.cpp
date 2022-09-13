#include <iostream>
#pragma comment(lib,"C:\\Users\\alber\\OneDrive\\3course\\6semestr\\OS\\labs\\lab10\\Debug\\lib.lib")
#include "../lib/HT.h"

void printStr(char* str);
int main()
{
	int capacity = 10;
	int capacity2 = 4;
	int snapshotIntervalSec = 2;
	int maxKeyLength = 10;
	int maxKeyLength2 = 4;
	int maxPayloadLength = 10;
	const wchar_t* fileName = L"C:\\Users\\alber\\OneDrive\\3course\\6semestr\\OS\\labs\\lab10\\storage1.ht";
	const wchar_t* fileName2 = L"C:\\Users\\alber\\OneDrive\\3course\\6semestr\\OS\\labs\\lab10\\storage2.ht";
	try {
		HT::HTHANDLE* HT = HT::Create(capacity, snapshotIntervalSec, maxKeyLength, maxPayloadLength, fileName);
		HT::HTHANDLE* HT2 = HT::Create(capacity2, snapshotIntervalSec, maxKeyLength2, maxPayloadLength, fileName2);

		//HT::HTHANDLE* HT = HT::Open(fileName);
		if (HT == NULL)
			throw "Invalid handle";
		if (HT2 == NULL)
			throw "Invalid handle";

		HT::Element* el1 = new HT::Element("str", 4, "albert", 7);
		HT::Element* el2 = new HT::Element("key123", 7, "value1234", 10);
		HT::Element* el3 = new HT::Element("str", 4, "value2", 7);
		HT::Element* el4 = new HT::Element("keyq", 5, "value123456", 12);
#pragma region INSERT1

		if (!HT::Insert(HT, el1)) {
			printStr(HT::GetLastError(HT));
		}
		if (!HT::Insert(HT, el2)) {
			printStr(HT::GetLastError(HT));
		}
		if (!HT::Insert(HT, el3)) {
			printStr(HT::GetLastError(HT));
		}
		if (!HT::Insert(HT, el4)) {
			printStr(HT::GetLastError(HT));
		}
#pragma endregion
#pragma region INSERT2

		if (!HT::Insert(HT2, el1)) {
			printStr(HT::GetLastError(HT));
		}
		if (!HT::Insert(HT2, el2)) {
			printStr(HT::GetLastError(HT));
		}
		if (!HT::Insert(HT2, el3)) {
			printStr(HT::GetLastError(HT));
		}
		if (!HT::Insert(HT2, el4)) {
			printStr(HT::GetLastError(HT));
		}
#pragma endregion
		HT::Element* el;
		std::cout << "-----INSERT1-----" << std::endl;
#pragma region GET1
		if ((el = HT::Get(HT, el1)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT, el2)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT, el3)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT, el4)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
#pragma endregion
		std::cout << "-----INSERT2-----" << std::endl;
#pragma region GET2
		if ((el = HT::Get(HT2, el1)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT2, el2)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT2, el3)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT2, el4)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
#pragma endregion

		HT::Close(HT);
		HT::Close(HT2);
		HT::HTHANDLE* HT3 = HT::Open(fileName);
		HT::HTHANDLE* HT4 = HT::Open(fileName2);

		std::cout << "-----GET3-----" << std::endl;
#pragma region GET3
		if ((el = HT::Get(HT3, el1)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT3, el2)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT3, el3)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
#pragma endregion
		std::cout << "-----GET4-----" << std::endl;

#pragma region GET4
		if ((el = HT::Get(HT4, el1)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT4, el2)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT4, el3)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
		if ((el = HT::Get(HT4, el4)) == NULL)
			printStr(HT::GetLastError(HT));
		else
			HT::print(el);
#pragma endregion
		HT::Close(HT);
		HT::Close(HT2);
		delete el1;
		delete el2;
		delete el3;
		delete el4;
		delete el;
	}
	catch (const char* err) {
		std::cout << err << std::endl;
	}
	
	system("pause");
	return 0;
}
void printStr(char* str) {
	std::cout << "ERROR:\t";
	int i = 0;
	while (str[i]) {
		std::cout << str[i];
		i++;
	}
	std::cout << std::endl;
}