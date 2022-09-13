#pragma once
#include <objbase.h>
#include <iostream>
#include "INTERFACE.h"
#include <string>

class OS13:public IHashTable
{
public:
	OS13();
	~OS13();


	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);


	virtual HRESULT STDMETHODCALLTYPE Create   //  создать HT             
	(
		//DWORD& address,
		HT::HTHANDLE** result,
		int	  Capacity,					   // емкость хранилища
		int   SecSnapshotInterval,		   // переодичность сохранения в сек.
		int   MaxKeyLength,                // максимальный размер ключа
		int   MaxPayloadLength,            // максимальный размер данных
		char  FileName[512],
		LPCWSTR HTUsersGroup
	); 	// != NULL успешное завершение  

	virtual HRESULT STDMETHODCALLTYPE Open     //  открыть HT             
	(
		char    FileName[512],         // имя файла 
		HT::HTHANDLE**  result
	); 	// != NULL успешное завершение  
	
	virtual HRESULT STDMETHODCALLTYPE Open     //  открыть HT             
	(
		char    FileName[512],
		LPCWSTR userName,
		LPCWSTR password,
		HT::HTHANDLE** result
	);
	virtual HRESULT STDMETHODCALLTYPE Snap         // выполнить Snapshot
	(
		HT::HTHANDLE**  hthandle           // управление HT (File, FileMapping)
	);


	virtual HRESULT STDMETHODCALLTYPE Close        // Snap и закрыть HT  и  очистить HT::HTHANDLE
	(
		HT::HTHANDLE** hthandle
		//DWORD address     // управление HT (File, FileMapping)
	);	//  == TRUE успешное завершение   


	virtual HRESULT STDMETHODCALLTYPE Insert      // добавить элемент в хранилище
	(
		HT::HTHANDLE**  hthandle,            // управление HT
		HT::Element* element              // элемент
	);	//  == TRUE успешное завершение 


	virtual HRESULT STDMETHODCALLTYPE Delete      // удалить элемент в хранилище
	(
		HT::HTHANDLE**  hthandle,            // управление HT (ключ)
		HT::Element* element              // элемент 
	);	//  == TRUE успешное завершение 

	virtual HRESULT STDMETHODCALLTYPE Get     //  читать элемент в хранилище
	(
		HT::Element& result,
		HT::HTHANDLE**  hthandle,            // управление HT
		HT::Element* element              // элемент 
	); 	//  != NULL успешное завершение 


	virtual HRESULT STDMETHODCALLTYPE Update     //  именить элемент в хранилище
	(
		HT::HTHANDLE**  hthandle,            // управление HT
		HT::Element* oldelement,          // старый элемент (ключ, размер ключа)
		void* newpayload,          // новые данные  
		int             newpayloadlength     // размер новых данных
	); 	//  != NULL успешное завершение 

	virtual HRESULT STDMETHODCALLTYPE GetLastHtError  // получить сообщение о последней ошибке
	(
		std::string& error,
		HT::HTHANDLE**  ht                         // управление HT
	);

private:
	volatile ULONG m_Ref;
};
