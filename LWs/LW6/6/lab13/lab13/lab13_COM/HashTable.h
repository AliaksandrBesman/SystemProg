#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>


//#ifdef MATHLIBRARY_EXPORTS
//#define MATHLIBRARY_API __declspec(dllexport)
//#else
//#define MATHLIBRARY_API __declspec(dllimport)
//#endif


#define MATHLIBRARY_API __declspec(dllexport)

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


	MATHLIBRARY_API struct HTHANDLE    // блок управления HT
	{
		HTHANDLE();
		MATHLIBRARY_API HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, wchar_t FileName[512], LPCWSTR HTUsersGroup);
		int     Capacity = 20;               // емкость хранилища в количестве элементов 
		int     SecSnapshotInterval = 120;    // переодичность сохранения в сек. 
		int     MaxKeyLength = 64;           // максимальная длина ключа
		int     MaxPayloadLength = 64;       // максимальная длина данных
		wchar_t    FileName[512] = L"map.txt";          // имя файла 
		HANDLE  File = 0;                   // File HANDLE != 0, если файл открыт
		HANDLE  FileMapping = 0;            // Mapping File HANDLE != 0, если mapping создан  
		LPVOID  Addr = 0;                   // Addr != NULL, если mapview выполнен. Адрас з якога пачынаюцца элементы  
		char    LastErrorMessage[512] = "\0";  // сообщение об последней ошибке или 0x00  
		time_t  lastsnaptime = 0;           // дата последнего snap'a (time())  

		wchar_t mutexName[512];
		wchar_t fileMapName[512];
		wchar_t timerName[512];
		wchar_t HTUsersGroup[512];
		DWORD creatorPid;
		HANDLE hTimer;
		HANDLE hTimerThread;
	};

	extern "C" MATHLIBRARY_API struct Element   // элемент 
	{
	public:
		//Element();
		Element(void* key, int keylength);                                             // for Get
		Element(void* key, int keylength, void* payload, int  payloadlength);    // for Insert
		//MATHLIBRARY_API Element(Element* oldelement, void* newpayload, int  newpayloadlength);         // for update
		void* key;                 // значение ключа 
		int             keylength;           // рахмер ключа
		void* payload;             // данные 
		int             payloadlength;       // размер данных
	};

	Element* CreateElement(void* key, int keylength);
	Element* CreateElement(void* key, int keylength, void* payload, int  payloadlength);
	HTHANDLE * Create   //  создать HT             
	(
		int	  Capacity,					   // емкость хранилища
		int   SecSnapshotInterval,		   // переодичность сохранения в сек.
		int   MaxKeyLength,                // максимальный размер ключа
		int   MaxPayloadLength,            // максимальный размер данных
		char  FileName[512],          // имя файла 
		LPCWSTR HTUsersGroup
	); 	// != NULL успешное завершение  

	MATHLIBRARY_API  HTHANDLE * Open     //  открыть HT             
	(
		char    FileName[512]         // имя файла 
	); 	// != NULL успешное завершение  

	MATHLIBRARY_API  HTHANDLE * Open     //  открыть HT             
	(
		char    FileName[512],
		LPCWSTR userName,
		LPCWSTR password
	);

	extern "C" MATHLIBRARY_API  BOOL Snap         // выполнить Snapshot
	(
		HTHANDLE * hthandle           // управление HT (File, FileMapping)
	);


	extern "C" MATHLIBRARY_API BOOL Close        // Snap и закрыть HT  и  очистить HTHANDLE
	(
		HTHANDLE * hthandle           // управление HT (File, FileMapping)
	);	//  == TRUE успешное завершение   


	extern "C" MATHLIBRARY_API BOOL Insert      // добавить элемент в хранилище
	(
		HTHANDLE * hthandle,            // управление HT
		Element * element              // элемент
	);	//  == TRUE успешное завершение 


	extern "C" MATHLIBRARY_API BOOL Delete      // удалить элемент в хранилище
	(
		HTHANDLE * hthandle,            // управление HT (ключ)
		Element * element              // элемент 
	);	//  == TRUE успешное завершение 

	extern "C" MATHLIBRARY_API Element * Get     //  читать элемент в хранилище
	(
		HTHANDLE * hthandle,            // управление HT
		Element * element              // элемент 
	); 	//  != NULL успешное завершение 

	extern "C" MATHLIBRARY_API int PrintAll     // print all elements aand return their count
	(
		HTHANDLE * ht           // управление HT
	); 	//  != NULL успешное завершение 


	extern "C" MATHLIBRARY_API BOOL Update     //  именить элемент в хранилище
	(
		HTHANDLE * hthandle,            // управление HT
		Element * oldelement,          // старый элемент (ключ, размер ключа)
		void* newpayload,          // новые данные  
		int             newpayloadlength     // размер новых данных
	); 	//  != NULL успешное завершение 

	extern "C" MATHLIBRARY_API char* GetLastHtError  // получить сообщение о последней ошибке
	(
		HTHANDLE * ht                         // управление HT
	);

	extern "C" MATHLIBRARY_API void print                               // распечатать элемент 
	(
		Element * element              // элемент 
	);
	extern "C" MATHLIBRARY_API int sum(int a, int b);
};
