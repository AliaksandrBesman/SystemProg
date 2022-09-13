#include <iostream>
#include "HT.h"
void printStr(char* str);
int main()
{
	const wchar_t* fileName = L"C:\\Users\\alber\\OneDrive\\3course\\6semestr\\OS\\labs\\lab10\\lab10\\storage.ht";
	try {
		int capacity = 10;
		int snapshotIntervalSec = 2;
		int maxKeyLength = 10;
		int maxPayloadLength = 10;
		HT::HTHANDLE* HT = HT::Create(capacity, snapshotIntervalSec, maxKeyLength, maxPayloadLength, fileName);
		//HT::HTHANDLE* HT = HT::Open(fileName);
		if (HT == NULL)
			throw "Invalid handle";

		HT::Element* el1 = new HT::Element("str", 4, "albert", 7);
		HT::Element* el2 = new HT::Element("key123", 7, "value1234", 10);
		HT::Element* el3 = new HT::Element("str", 4, "value2", 7);
		HT::Element* el4 = new HT::Element("keyq", 5, "value123456", 12);
#pragma region INSERT

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
		HT::Element* el;
		std::cout << "-----INSERT-----" << std::endl;
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
#pragma endregion

#pragma region UPDATE
		if (!HT::Update(HT, el1, el3->payload, el3->payloadlength)) {
			printStr(HT::GetLastError(HT));
		}
		if (!HT::Update(HT, el4, el3->payload, el3->payloadlength)) {
			printStr(HT::GetLastError(HT));
		}
#pragma endregion
		if (!HT::Snap(HT))
			printStr(HT::GetLastError(HT));
		std::cout << "-----UPDATE-----" << std::endl;
#pragma region GET2
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
#pragma endregion
		Sleep(10000);
#pragma region DELETE
		if (!HT::Delete(HT, el2)) {
			printStr(HT::GetLastError(HT));
		}
		if (!HT::Delete(HT, el4)) {
			printStr(HT::GetLastError(HT));
		}
#pragma endregion
		std::cout << "-----DELETE-----" << std::endl;
#pragma region GET3
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
#pragma endregion
		HT::Close(HT);
		HT::HTHANDLE* HT2 = HT::Open(fileName);
		std::cout << "-----GET-----" << std::endl;
#pragma region GET4
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
#pragma endregion
		HT::Close(HT2);
		delete el1;
		delete el2;
		delete el3;
		delete el4;
		delete el;
	}
	catch (const char* mess) {
		std::cout << mess << std::endl;
	}
		
	system("pause");
	return 0;
}
void printStr(char* str) {
	std::cout << "ERROR:\t"<<str<<std::endl;
}