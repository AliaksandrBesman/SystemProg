#pragma once
#include <Windows.h>



namespace HT    // HT API
{

	extern "C"  struct HTHANDLE    // блок управления HT
	{
		HTHANDLE();
		HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const wchar_t FileName[512]);
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
		DWORD creatorPid;
		HANDLE hTimer;
		HANDLE hTimerThread;
	};

	extern "C" struct Element   // элемент 
	{
	public:
		Element();
		Element(void* key, int keylength);                                             // for Get
		Element(void* key, int keylength, void* payload, int  payloadlength);    // for Insert
		//MATHLIBRARY_API Element(Element* oldelement, const void* newpayload, int  newpayloadlength);         // for update
		void* key;                 // значение ключа 
		int             keylength;           // рахмер ключа
		void* payload;             // данные 
		int             payloadlength;       // размер данных
	};

};
