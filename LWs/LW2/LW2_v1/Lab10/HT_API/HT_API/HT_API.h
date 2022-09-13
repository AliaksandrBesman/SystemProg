#pragma once
#include <Windows.h>

namespace HT    // HT API
{
	// API HT - программный интерфейс для доступа к НТ-хранилищу 
	//          НТ-хранилище предназначено для хранения данных в ОП в формате ключ/значение
	//          Персистестеность (сохранность) данных обеспечивается с помощью snapshot-менханизма 
	//          Create - создать  и открыть HT-хранилище для использования   
	//          Open   - открыть HT-хранилище для использования
	//          Insert - создать элемент данных
	//          Delete - удалить элемент данных    
	//          Get    - читать  элемент данных
	//          Update - изменить элемент данных
	//          Snap   - выпонить snapshot
	//          Close  - выполнить Snap и закрыть HT-хранилище для использования
	//          GetLastError - получить сообщение о последней ошибке

#pragma region My additions

	//used to be part of HTHANDLE
	//size must be multiple of 4 to keep propper alignment
	struct HTMeta
	{
		HTMeta();
		HTMeta
		(
			int Capacity,
			int SecSnapshotInterval,
			int MaxKeyLength,
			int MaxPayloadLength
		);
		int     Capacity;               // емкость хранилища в количестве элементов 
		int     SecSnapshotInterval;    // переодичность сохранения в сек. 
		int     MaxKeyLength;           // максимальная длина ключа
		int     MaxPayloadLength;       // максимальная длина данных
	};

#pragma endregion My additions

	struct HTHANDLE    // блок управления HT
	{
		HTHANDLE();
		HTHANDLE
		(
			int Capacity, 
			int SecSnapshotInterval, 
			int MaxKeyLength, 
			int MaxPayloadLength, 
			const TCHAR FileName[512]
		);
		HTHANDLE
		(
			HTMeta* pHTMeta,
			const TCHAR FileName[512]
		);

		CRITICAL_SECTION	lock;
		HANDLE				hSnapThread;
		bool				alive = true;

		LPVOID  nthKeyAddr(int n) const;
		LPVOID	nthPayloadAddr(int n) const;

		HTMeta  Meta;							//
		int		ContentOffset = sizeof(HTMeta);	// distance from Addr to start of actual content
		int		MaxContentSize;					// summary size of all keys and payloads

		TCHAR   FileName[512];					// имя файла 
		HANDLE  File;							// File HANDLE != 0, если файл открыт
		HANDLE  FileMapping;					// Mapping File HANDLE != 0, если mapping создан  
		LPVOID  Addr;							// Addr != NULL, если mapview выполнен (start of file view)
		LPVOID  ContentAddr;					// ptr to actual content in a file view (Addr + ContentOffset)

		time_t  LastSnapTime;					// дата последнего snap'a (time())
		time_t  LastChangeTime;					//

		TCHAR   LastErrorMessage[512];			// сообщение об последней ошибке или 0x00  
	};

	struct Element   // элемент 
	{
		Element();
		Element(LPCVOID key, int keylength);                                             // for Get
		Element(LPCVOID key, int keylength, LPCVOID payload, int  payloadlength);    // for Insert
		Element(Element* oldelement, LPCVOID newpayload, int  newpayloadlength);         // for update
		LPCVOID		    key;                 // значение ключа 
		int             keylength;           // рахмер ключа
		LPCVOID		    payload;             // данные 
		int             payloadlength;       // размер данных
	};
	
	HTHANDLE*  Create   //  создать HT             
	(
		int			Capacity,						// емкость хранилища
		int			SecSnapshotInterval,			// переодичность сохранения в сек.
		int			MaxKeyLength,					// максимальный размер ключа
		int			MaxPayloadLength,				// максимальный размер данных
		const TCHAR	FileName[512]					// имя файла 
	); 	// != NULL успешное завершение  

	HTHANDLE*   Open     //  открыть HT             
	(
		const TCHAR    FileName[512]		// имя файла 
	); 	// != NULL успешное завершение  

	BOOL Snap         // выполнить Snapshot
	(
		const HTHANDLE*    hthandle           // управление HT (File, FileMapping)
	);
	

	BOOL Close        // Snap и закрыть HT  и  очистить HTHANDLE
	(
		HTHANDLE*    hthandle           // управление HT (File, FileMapping)
	);	//  == TRUE успешное завершение   

		
	BOOL Insert      // добавить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		const Element*  element              // элемент
	);	//  == TRUE успешное завершение 


	BOOL Delete      // удалить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT (ключ)
		const Element*  element              // элемент 
	);	//  == TRUE успешное завершение 

	Element* Get     //  читать элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		const Element*  element              // элемент 
	); 	//  != NULL успешное завершение 

	
	BOOL Update     //  именить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		const Element*  oldelement,          // старый элемент (ключ, размер ключа)
		const void*     newpayload,          // новые данные  
		int             newpayloadlength     // размер новых данных
	); 	//  != NULL успешное завершение 

	TCHAR* HTGetLastError  // получить сообщение о последней ошибке
	(
		HTHANDLE* ht                         // управление HT
	);
	
	void print                               // распечатать элемент 
	(
		const Element*  element              // элемент 
	);

	
};
