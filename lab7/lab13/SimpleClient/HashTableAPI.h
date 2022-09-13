#pragma once
#include <Windows.h>
#include "HashTable.h"

namespace HTAPI    // HT API
{
	bool OpenApi();
	bool CloseApi();

	HT::HTHANDLE * Create   //  создать HT             
	(
		int	  Capacity,					   // емкость хранилища
		int   SecSnapshotInterval,		   // переодичность сохранения в сек.
		int   MaxKeyLength,                // максимальный размер ключа
		int   MaxPayloadLength,            // максимальный размер данных
		char  FileName[512],          // имя файла 
		LPCWSTR HTUsersGroup
	);

	HT::HTHANDLE * Open     //  открыть HT             
	(
		char    FileName[512]         // имя файла 
	); 	// != NULL успешное завершение  

	HT::HTHANDLE* Open     //  открыть HT             
	(
		char    FileName[512],
		LPCWSTR userName,
		LPCWSTR password
	);

	BOOL Snap         // выполнить Snapshot
	(
		HT::HTHANDLE * hthandle           // управление HT (File, FileMapping)
	);


	BOOL Close        // Snap и закрыть HT  и  очистить HT::HTHANDLE
	(
		HT::HTHANDLE * hthandle           // управление HT (File, FileMapping)
	);	//  == TRUE успешное завершение   


	BOOL Insert      // добавить элемент в хранилище
	(
		HT::HTHANDLE * hthandle,            // управление HT
		HT::Element * element              // элемент
	);	//  == TRUE успешное завершение 


	BOOL Delete      // удалить элемент в хранилище
	(
		HT::HTHANDLE * hthandle,            // управление HT (ключ)
		HT::Element * element              // элемент 
	);	//  == TRUE успешное завершение 

	HT::Element * Get     //  читать элемент в хранилище
	(
		HT::HTHANDLE * hthandle,            // управление HT
		HT::Element * element              // элемент 
	); 	//  != NULL успешное завершение 


	BOOL Update     //  именить элемент в хранилище
	(
		HT::HTHANDLE * hthandle,            // управление HT
		HT::Element * oldelement,          // старый элемент (ключ, размер ключа)
		void* newpayload,          // новые данные  
		int             newpayloadlength     // размер новых данных
	); 	//  != NULL успешное завершение 

	char* GetLastHtError  // получить сообщение о последней ошибке
	(
		HT::HTHANDLE * ht                         // управление HT
	);

};
