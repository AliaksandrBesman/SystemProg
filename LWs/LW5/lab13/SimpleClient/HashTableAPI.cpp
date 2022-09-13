#pragma once
#include <Windows.h>
#include <objbase.h>
#include "HashTableAPI.h"
#include "INTERFACE.h"
#include <iostream>

namespace HTAPI    // HT API
{
	IHashTable* pIHashTable = nullptr;
	IUnknown* pIUnknown = NULL;

	bool  HTAPI::OpenApi() {
		CoInitialize(NULL);                        // инициализация библиотеки OLE32
		HRESULT hr0 = CoCreateInstance(CLSID_CA, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pIUnknown);
		if (SUCCEEDED(hr0))
		{
			std::cout << "CoCreateInstance succeeded" << std::endl;
			if (SUCCEEDED(pIUnknown->QueryInterface(IID_IHashTable, (void**)&pIHashTable)))
			{
				return true;
			}
		}
		return false;
	}
	bool  HTAPI::CloseApi() {
		bool res = true;
		if (!SUCCEEDED(pIHashTable->Release()))
			res = false;
		if (!SUCCEEDED(pIUnknown->Release()))
			res = false;
		CoFreeUnusedLibraries();
		return true;
	}


	HT::HTHANDLE* HTAPI::Create   //  создать HT             
	(
		int	  Capacity,					   // емкость хранилища
		int   SecSnapshotInterval,		   // переодичность сохранения в сек.
		int   MaxKeyLength,                // максимальный размер ключа
		int   MaxPayloadLength,            // максимальный размер данных
		char  FileName[512]          // имя файла 
	) {
		HT::HTHANDLE* pht;
		if (SUCCEEDED(pIHashTable->Create(
			&pht, Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName))) {
			return pht;
		}
		else {
			return NULL;
		}
	}
	HT::HTHANDLE* HTAPI::Open     //  открыть HT             
	(
		char    FileName[512]         // имя файла 
	) {
		HT::HTHANDLE* pht;
		if (SUCCEEDED(pIHashTable->Open(FileName, & pht))) {
			return pht;
		}
		else {
			return NULL;
		}
	}

	BOOL Snap         // выполнить Snapshot
	(
		HT::HTHANDLE* hthandle           // управление HT (File, FileMapping)
	){
		if (SUCCEEDED(pIHashTable->Snap(&hthandle))) {
			return true;
		}
		else {
			return false;
		}
	}


	BOOL Close        // Snap и закрыть HT  и  очистить HT::HTHANDLE
	(
		HT::HTHANDLE* hthandle           // управление HT (File, FileMapping)
	) {

		if (SUCCEEDED(pIHashTable->Close(&hthandle))) {
			return true;
		}
		else {
			return false;
		}
	}


	BOOL Insert      // добавить элемент в хранилище
	(
		HT::HTHANDLE* hthandle,            // управление HT
		HT::Element* element              // элемент
	) {
		if (SUCCEEDED(pIHashTable->Insert(&hthandle, element))) {
			return true;
		}
		else {
			return false;
		}
	}


	BOOL Delete      // удалить элемент в хранилище
	(
		HT::HTHANDLE* hthandle,            // управление HT (ключ)
		HT::Element* element              // элемент 
	) {
		if (SUCCEEDED(pIHashTable->Delete(&hthandle, element))) {
			return true;
		}
		else {
			return false;
		}
	}

	HT::Element* Get     //  читать элемент в хранилище
	(
		HT::HTHANDLE* hthandle,            // управление HT
		HT::Element* element              // элемент 
	) {
		HT::Element result;
		if (SUCCEEDED(pIHashTable->Get(result ,&hthandle, element))) {
			return &result;
		}
		else {
			return NULL;
		}
	}



	BOOL Update     //  именить элемент в хранилище
	(
		HT::HTHANDLE* hthandle,            // управление HT
		HT::Element* oldelement,          // старый элемент (ключ, размер ключа)
		void* newpayload,          // новые данные  
		int             newpayloadlength     // размер новых данных
	) {
		if (SUCCEEDED(pIHashTable->Update(&hthandle, oldelement, newpayload, newpayloadlength))) {
			return true;
		}
		else {
			return false;
		}
	}

	char* GetLastHtError  // получить сообщение о последней ошибке
	(
		HT::HTHANDLE* ht                         // управление HT
	) {
		std::string err;
		if (SUCCEEDED(pIHashTable->GetLastHtError(err ,&ht))) {
			return (char*)(err.c_str());
		}
		else {
			return NULL;
		}
	}

};
