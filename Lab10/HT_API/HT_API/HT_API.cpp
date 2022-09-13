#include <iostream>
#include <stdio.h>
#include <TCHAR.H>
#include <time.h>
#include "HT_API.h"


#pragma region Snap / callback


BOOL HT::Snap(
	const HT::HTHANDLE* hthandle
)
{
	EnterCriticalSection((LPCRITICAL_SECTION) & (hthandle->lock));

	FlushViewOfFile(hthandle->Addr, hthandle->ContentOffset + hthandle->MaxContentSize);
	FlushFileBuffers(hthandle->File);

	LeaveCriticalSection((LPCRITICAL_SECTION) & (hthandle->lock));

	return TRUE;
}

DWORD WINAPI snapCallback(
	LPVOID hthandle
)
{
	int snapTime = ((HT::HTHANDLE*)hthandle)->Meta.SecSnapshotInterval * 1000;
	int slept = 0;

	for (;;)
	{
		int sleepTime = snapTime < 1000 ? snapTime : 500;
		Sleep(sleepTime);
		slept += sleepTime;

		if (snapTime <= slept)
		{
			HT::Snap(((HT::HTHANDLE*)hthandle));
			slept = 0;
		}

		if (!((HT::HTHANDLE*)hthandle)->alive)
			return TRUE;
	}
}

#pragma endregion Snap / callback


#pragma region HTHANDLE


#pragma region HTMeta constructors


HT::HTMeta::HTMeta() {

}


HT::HTMeta::HTMeta(
	int Capacity,
	int SecSnapshotInterval,
	int MaxKeyLength,
	int MaxPayloadLength
)
{
	this->Capacity = Capacity;
	this->SecSnapshotInterval = SecSnapshotInterval;
	this->MaxKeyLength = MaxKeyLength;
	this->MaxPayloadLength = MaxPayloadLength;
}


#pragma endregion HTMeta constructors


#pragma region HTHANDLE constructors


HT::HTHANDLE::HTHANDLE() {

}


HT::HTHANDLE::HTHANDLE(
	int Capacity, 
	int SecSnapshotInterval, 
	int MaxKeyLength, 
	int MaxPayloadLength, 
	const TCHAR FileName[512]
)
{
	HT::HTMeta* pMeta = new HT::HTMeta(
		Capacity, 
		SecSnapshotInterval,
		MaxKeyLength, 
		MaxPayloadLength
	);
	
	HT::HTHANDLE(pMeta, FileName);
}


HT::HTHANDLE::HTHANDLE(
	HT::HTMeta* pHTMeta,
	const TCHAR FileName[512]
)
{
	this->Meta = *pHTMeta;
	_tcscpy_s(this->FileName, FileName);	
	this->File = NULL;
	this->FileMapping = NULL;
	this->Addr = NULL;
	memset(this->LastErrorMessage, 0, sizeof(this->LastErrorMessage));

	this->LastSnapTime = this->LastChangeTime = time(NULL);
}


#pragma endregion HTHANDLE constructors


#pragma region HTHANDLE Create / Open / Close


HT::HTHANDLE* HT::Create(
	int			Capacity,
	int			SecSnapshotInterval,
	int			MaxKeyLength,
	int			MaxPayloadLength,
	const TCHAR	FileName[512]
)
{
	//create or replace file
	HANDLE hHTFile = CreateFile(
		FileName,
		GENERIC_READ | GENERIC_WRITE,
		0,	//sharing not allowed
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hHTFile == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("Failed to creat new HT-file"));
	}

	//initialize meta and write it to th beginning of file
	HT::HTMeta* pMeta = new HT::HTMeta(
		Capacity,
		SecSnapshotInterval,
		MaxKeyLength,
		MaxPayloadLength
	);
	DWORD numberOfMetaBytesWritten;
	WriteFile(
		hHTFile,
		pMeta,
		sizeof(HT::HTMeta),
		&numberOfMetaBytesWritten,
		NULL
	);

	//!!!!!!!!!!!!
	delete pMeta;
	CloseHandle(hHTFile);

	return HT::Open(FileName);
}


HT::HTHANDLE* HT::Open(
	const TCHAR FileName[512]
)
{
	//open
	HANDLE hHTFile = CreateFile(
		FileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, //0,	//sharing not allowed
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hHTFile == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("Failed to open HT-file"));
	}

	//read meta
	HT:HTMeta MetaBuf;
	DWORD numberOfBytesRead;
	ReadFile(
		hHTFile,
		&MetaBuf,
		sizeof(HT::HTMeta),
		&numberOfBytesRead,
		NULL
	);

	//!!!!!!!!!!!!!!!!!!!!!!
	//CloseHandle(hHTFile);

	HT::HTHANDLE* hHT = new HT::HTHANDLE(&MetaBuf, FileName);

	hHT->File = hHTFile;


	hHT->MaxContentSize = (hHT->Meta.Capacity * (hHT->Meta.MaxKeyLength + hHT->Meta.MaxPayloadLength));

	hHT->FileMapping = CreateFileMapping(
		hHTFile,
		NULL,
		PAGE_READWRITE,
		NULL,
		sizeof(HT::HTMeta) + hHT->MaxContentSize,
		NULL
	);

	hHT->Addr = MapViewOfFile(
		hHT->FileMapping,
		FILE_MAP_ALL_ACCESS,
		NULL,
		0, //starting point must be alligned with file page zize
		hHT->ContentOffset + hHT->MaxContentSize
	);
	if (!hHT->Addr) {
		printf("MapViewOfFile error N: (%d).\n", GetLastError());
	}

	
	hHT->ContentAddr = (LPVOID)((LPBYTE)hHT->Addr + hHT->ContentOffset);
	
	InitializeCriticalSection(&(hHT->lock));

	hHT->hSnapThread = CreateThread(NULL, NULL, snapCallback, (LPVOID)hHT, NULL, NULL);

	return hHT;
}


BOOL HT::Close(
	HT::HTHANDLE* hthandle
)
{
	hthandle->alive = false;
	WaitForSingleObject(hthandle->hSnapThread, INFINITE);

	UnmapViewOfFile(hthandle->Addr);
	CloseHandle(hthandle->File);

	return TRUE;
}


#pragma endregion HTHANDLE Create / Open / Close


#pragma region Address calc


LPVOID HT::HTHANDLE::nthKeyAddr(int n) const
{
	return (LPVOID)
	(
		(LPBYTE)(this->ContentAddr) + n*(this->Meta.MaxKeyLength + this->Meta.MaxPayloadLength)
	);
}


LPVOID HT::HTHANDLE::nthPayloadAddr(int n) const
{
	return (LPVOID)
	(
		(LPBYTE)(this->ContentAddr) + n*(this->Meta.MaxKeyLength + this->Meta.MaxPayloadLength)
			+ this->Meta.MaxKeyLength
	);
}


#pragma endregion Address calc


TCHAR* HT::HTGetLastError(
	HTHANDLE* ht
)
{
	return ht->LastErrorMessage;
}


#pragma endregion HTHANDLE


#pragma region Element


#pragma region constructors


HT::Element::Element() {

}

// GETting / DELETing constructor
HT::Element::Element(
	LPCVOID     key,
	int			keylength
)
{
	this->key = key;
	this->keylength = keylength;
	this->payload = NULL;
	this->payloadlength = NULL;
}

// INSERTing constructor
HT::Element::Element(
	LPCVOID     key,
	int			keylength,
	LPCVOID     payload,
	int			payloadlength
)
{
	this->key = key;
	this->keylength = keylength;
	this->payload = payload;
	this->payloadlength = payloadlength;
}

// UPDATing constructor
HT::Element::Element(
	Element* oldelement,
	LPCVOID newpayload,
	int  newpayloadlength
)
{
	this->key = oldelement->key;
	this->keylength = oldelement->keylength;
	this->payload = newpayload;
	this->payloadlength = newpayloadlength;
}


#pragma endregion constructors


void HT::print(
	const Element* element
)
{
	_tprintf(_T("%s%d, %s%d"), _T("Key: "), *(int*)element->key, _T("Value: "), *(int*)element->payload);
}


#pragma endregion Element


#pragma region Utils


bool isMemEmpty(LPCVOID mem, int len)
{
	LPVOID zeroBuf = malloc(len);
	memset(zeroBuf, 0, len);

	bool res = (0 == memcmp(zeroBuf, mem, len));

	free(zeroBuf);

	return res;
}


int HashBytes(LPCVOID bytes, int numOfBytes)
{
	LPBYTE pByte = (LPBYTE)bytes;
	int res = 0;

	for (int i = 0; i < numOfBytes; i++)
	{
		res ^= *(pByte + i) << ( 8*(i % sizeof(int)) );
	}

	return (int)abs( ((float)res / 1.61803) );
}


#pragma endregion Utils


#pragma region CRUD


BOOL HT::Insert(
	HTHANDLE* hthandle,
	const Element*	element
)
{
	int firstEmptyRecordInd = -1;
	int maxKeyLength = hthandle->Meta.MaxKeyLength;

	int hash = HashBytes(element->key, element->keylength) % hthandle->Meta.Capacity;

	for (int ii = hash; ii < hthandle->Meta.Capacity + hash; ii++)
	{
		int i = ii % hthandle->Meta.Capacity;

		LPCVOID nKey = hthandle->nthKeyAddr(i);

		if (isMemEmpty(nKey, maxKeyLength))
			firstEmptyRecordInd = firstEmptyRecordInd == -1 ? i : firstEmptyRecordInd;

		LPVOID maxLenKey = malloc(maxKeyLength);
		memset(maxLenKey, 0,      maxKeyLength);
		memcpy(maxLenKey, element->key, element->keylength);

		if (!memcmp(nKey, maxLenKey, maxKeyLength))
		{
			free(maxLenKey);

			_tcscpy_s(hthandle->LastErrorMessage, _T("Key exists"));

			return FALSE;
		}
		free(maxLenKey);
	}

	if (firstEmptyRecordInd == -1)
	{
		_tcscpy_s(hthandle->LastErrorMessage, _T("Full"));
		return FALSE;
	}

	LPVOID nKey =		hthandle->nthKeyAddr(firstEmptyRecordInd);
	LPVOID nPayload =	hthandle->nthPayloadAddr(firstEmptyRecordInd);

	//memset(nKey,     0, hthandle->Meta.MaxKeyLength);
	//memset(nPayload, 0, hthandle->Meta.MaxPayloadLength);

	memcpy(nKey,	 element->key,	   element->keylength);
	memcpy(nPayload, element->payload, element->payloadlength);

	return TRUE;
}


BOOL HT::Delete(
	HTHANDLE* hthandle,
	const Element*	element
)
{
	int maxKeyLength = hthandle->Meta.MaxKeyLength;

	int hash = HashBytes(element->key, element->keylength) % hthandle->Meta.Capacity;

	for (int ii = hash; ii < hthandle->Meta.Capacity + hash; ii++)
	{
		int i = ii % hthandle->Meta.Capacity;

		LPVOID maxLenKey = malloc(maxKeyLength);
		memset(maxLenKey, 0, maxKeyLength);
		memcpy(maxLenKey, element->key, element->keylength);

		LPVOID nKey = hthandle->nthKeyAddr(i);

		if (0 == memcmp(maxLenKey, nKey, maxKeyLength))
		{
			memset(nKey, 0, maxKeyLength + hthandle->Meta.MaxPayloadLength);

			free(maxLenKey);
			return TRUE;
		}
		free(maxLenKey);
	}

	_tcscpy_s(hthandle->LastErrorMessage, _T("No such key"));
	return FALSE;	//error no such key
}


HT::Element* HT::Get
(
	HTHANDLE* hthandle,
	const Element*	element
)
{
	int maxKeyLength = hthandle->Meta.MaxKeyLength;
	int maxPayloadLength = hthandle->Meta.MaxPayloadLength;

	int hash = HashBytes(element->key, element->keylength) % hthandle->Meta.Capacity;

	for (int ii = hash; ii < hthandle->Meta.Capacity + hash; ii++)
	{
		int i = ii % hthandle->Meta.Capacity;

		LPVOID maxLenKey = malloc(maxKeyLength);
		memset(maxLenKey, 0, maxKeyLength);
		memcpy(maxLenKey, element->key, element->keylength);

		LPVOID nKey = hthandle->nthKeyAddr(i);

		if (0 == memcmp(maxLenKey, nKey, maxKeyLength))
		{
			LPVOID newElKey = malloc(element->keylength);
			memcpy(newElKey, nKey, element->keylength);

			LPVOID newElPayload = malloc(maxPayloadLength);
			memcpy(newElPayload, hthandle->nthPayloadAddr(i), maxPayloadLength);

			HT::Element* el = new HT::Element(
				newElKey,
				element->keylength,
				newElPayload,
				hthandle->Meta.MaxPayloadLength
			);

			free(maxLenKey);
			return el;
		}
		free(maxLenKey);
	}

	_tcscpy_s(hthandle->LastErrorMessage, _T("No such key"));
	return NULL;
}


BOOL HT::Update
(
	HTHANDLE* hthandle,
	const Element*	oldelement,
	const void*		newpayload,
	int             newpayloadlength
)
{
	int maxKeyLength = hthandle->Meta.MaxKeyLength;
	int maxPayloadLength = hthandle->Meta.MaxPayloadLength;

	int hash = HashBytes(oldelement->key, oldelement->keylength) % hthandle->Meta.Capacity;

	for (int ii = hash; ii < hthandle->Meta.Capacity + hash; ii++)
	{
		int i = ii % hthandle->Meta.Capacity;

		LPVOID maxLenKey = malloc(maxKeyLength);
		memset(maxLenKey, 0, maxKeyLength);
		memcpy(maxLenKey, oldelement->key, oldelement->keylength);

		LPVOID nKey = hthandle->nthKeyAddr(i);

		if (0 == memcmp(nKey, maxLenKey, maxKeyLength))
		{
			LPVOID oldPayload = hthandle->nthPayloadAddr(i);
			memset(oldPayload, 0, maxPayloadLength);
			memcpy(oldPayload, newpayload, newpayloadlength);

			free(maxLenKey);

			return TRUE;
		}
		free(maxLenKey);
	}

	_tcscpy_s(hthandle->LastErrorMessage, _T("No such key"));
	return FALSE;
}


#pragma endregion CRUD


int main()
{
	setlocale(LC_ALL, "ru-RU");

	HT::HTHANDLE* h = HT::Create(5, 5, 32, 32, TEXT("D:\\учёба\\sem6\\СисПрог\\Lab\\Lab10\\HTTEST.txt"));

	//HT::HTHANDLE* h = HT::Open(TEXT("D:\\учёба\\sem6\\СисПрог\\Lab\\Lab10\\HTTEST.txt"));
	//HT::HTMeta m = h->Meta;

	const int k = 11;
	const int p = 22;

	HT::Element* el = new HT::Element(&k, 4, &p, 4);
	//HT::Element* got_el = HT::Get(h, el);

	//printf("%d %d", *(int*)got_el->key, *(int*)got_el->payload);

	//std::cout << HashBytes(&k, sizeof(k));

	HT::Insert(h, el);
	////Sleep(5000);
	////HT::Delete(h, el);
	//int k2 = 88;
	//HT::Element* got = HT::Get(h, new HT::Element(&k2, sizeof(k2)));
	//HT::print(got);

	//int k3 = 555;
	//int p3 = 99999;
	//HT::Close(h);
	//HT::Element* el2 = new HT::Element(&k3, sizeof(int));
	//HT::Update(h, el2, &p3, sizeof(int));


	///*printf("Capacity: %d; MaxKeyength: %d; MaxPayloadLength: %d; SecSnapshotInterval: %d;", 
	//	m->Capacity, m->MaxKeyLength, m->MaxPayloadLength, m->SecSnapshotInterval);*/

	////HT::HTHANDLE* h = HT::Open(_T("D:\\учёба\\sem6\\СисПрог\\Lab\\Lab10\\HTTEST.txt"));

	//const char* buf = "TESTING";
	//memset(   h->ContentAddr, 0, m->Capacity * (m->MaxKeyLength + m->MaxPayloadLength));
	//memcpy(h->nthKeyAddr(2), buf, strlen(buf) + 1);
	////strcpy_s( (char*)h->ContentAddr, strlen(buf) + 1, buf);
	//memset(h->nthPayloadAddr(3), 254, 32);
	////(void*)((LPBYTE)h->Addr + h->ContentOffset)

	/*int five = 555;
	float nmb = 123.456;

	printf("%d\n", HashBytes((LPVOID)"abcdefghijklmno", 16));
	printf("%d\n", HashBytes((LPVOID)&five, 4));
	printf("%d\n", HashBytes((LPVOID)"abcdefghijklm", 14));
	printf("%d\n", HashBytes((LPVOID)"abcdefghijklmnogg", 18));
	printf("%d\n", HashBytes((LPVOID)L"abcdefghijklmnogg", 18*2));
	printf("%d\n", HashBytes((LPVOID)&nmb, sizeof(nmb)));

	printf("%d\n", HashBytes((LPVOID)"abcdefghijklmno", 16) % 20);
	printf("%d\n", HashBytes((LPVOID)&five, 4) % 20);
	printf("%d\n", HashBytes((LPVOID)"abcdefghijklm", 14) % 20);
	printf("%d\n", HashBytes((LPVOID)"abcdefghijklmnogg", 18) % 20);
	printf("%d\n", HashBytes((LPVOID)L"abcdefghijklmnogg", 18 * 2) % 20);
	printf("%d\n", HashBytes((LPVOID)&nmb, sizeof(nmb)) % 20);*/


	/*LPVOID buf = malloc(32);
	printf("%d\n", sizeof(buf));*/


	/*char randomName[32];
	memset(randomName, 0, sizeof(randomName));
	memcpy( randomName, (const void*)((LPBYTE)h->Addr + h->ContentOffset), m->MaxKeyLength );

	printf(randomName);*/

	//_tprintf(_T("%s"), h->FileName); //<< ' ' << h->Meta->Capacity;
}