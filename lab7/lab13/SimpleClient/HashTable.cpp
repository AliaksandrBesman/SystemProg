#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "HashTable.h"


namespace HT    // HT API
{
	HTHANDLE::HTHANDLE() {
	}
	HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const wchar_t FileName[512], 
		LPCWSTR HTUsersGroup) {
		this->Capacity = Capacity;
		this->SecSnapshotInterval = SecSnapshotInterval;
		this->MaxKeyLength = MaxKeyLength;
		this->MaxPayloadLength = MaxPayloadLength;
		wcscpy_s(this->FileName, _countof(this->FileName), FileName);
		wcscpy(this->HTUsersGroup, HTUsersGroup);
	}
	HT::Element::Element() {
	}
	//// for Get
	HT::Element::Element(void* key, int keylength) {
		this->keylength = keylength;
		this->key = (void*)key;
	}
	// for Insert
	HT::Element::Element(void* key, int keylength, void* payload, int  payloadlength) {
		this->keylength = keylength;
		this->key = (void*)key;

		this->payloadlength = payloadlength;
		this->payload = (void*)payload;
	}
};
