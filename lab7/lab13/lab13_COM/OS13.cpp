#include "pch.h"
#include "OS13.h"
#include "SEQLOG.h"
#include "HashTable.h"
#include <string>

extern ULONG g_Components;


OS13::OS13() :m_Ref(1) {
	SEQ;
	InterlockedIncrement((LONG*)&g_Components);
	LOG("OS12::Adder g_Components = ", g_Components);
};
OS13::~OS13() {
	SEQ;
	InterlockedDecrement((LONG*)&g_Components);
	LOG("OS12::~Adder g_Components = ", g_Components);
};


HRESULT STDMETHODCALLTYPE OS13::QueryInterface(REFIID riid, void** ppv)
{
	SEQ;
	HRESULT rc = S_OK;
	*ppv = NULL;
	if (riid == IID_IUnknown)	*ppv = (IHashTable*)this;
	else if (riid == IID_IHashTable)	*ppv = (IHashTable*)this;
	else rc = E_NOINTERFACE;

	if (rc == S_OK) this->AddRef();
	LOG("OS12::QueryInterface rc = ", rc);
	return rc;
};
ULONG STDMETHODCALLTYPE OS13::AddRef(void) {
	SEQ;
	InterlockedIncrement((LONG*)&(this->m_Ref));
	LOG("OS12::AddRef m_Ref = ", this->m_Ref);
	return this->m_Ref;
};
ULONG STDMETHODCALLTYPE OS13::Release(void) {
	SEQ;

	ULONG rc = this->m_Ref;
	if ((rc = InterlockedDecrement((LONG*)&(this->m_Ref))) == 0) delete this;
	LOG("OS12::Release rc = ", rc);
	return rc;
};



HRESULT STDMETHODCALLTYPE OS13::Create   //  создать HT             
(
	//DWORD &address,
	HT::HTHANDLE** result,
	int	  Capacity,					   // емкость хранилища
	int   SecSnapshotInterval,		   // переодичность сохранения в сек.
	int   MaxKeyLength,                // максимальный размер ключа
	int   MaxPayloadLength,            // максимальный размер данных
	char  FileName[512],          // имя файла 
	LPCWSTR HTUsersGroup
)
{
	//address = (DWORD)(void*)HT::Create(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);
	//result = (HT::HTHANDLE*)(void*)address;
	*result = HT::Create(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName, HTUsersGroup);
	//HT::HTHANDLE* temp = HT::Create(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);
	//if (temp==NULL)
	//	return E_FAIL;
	//result = *temp;
	SEQHT;
	LOGHT("Created ht result: ", &result);
	return S_OK;
	//return E_FAIL;
}

// != NULL успешное завершение  

 HRESULT STDMETHODCALLTYPE OS13::Open     //  открыть HT             
 (
	 char    FileName[512],         // имя файла 
	 HT::HTHANDLE**  result
 )
 {
	 SEQHT;
	 LOGHT("Open file with name: ", FileName);
	 HT::HTHANDLE* temp = HT::Open(FileName);
	 LOGHT("Opened hte: ", temp);
	 if (temp == NULL)
		 return E_FAIL;
	 *result = temp;
	 if (*result != NULL)
		 return S_OK;
	 else
		 return E_FAIL;
 }
 HRESULT STDMETHODCALLTYPE OS13::Open     //  открыть HT             
 (
	 char    FileName[512],         // имя файла 
	 LPCWSTR userName,
	 LPCWSTR password,
	 HT::HTHANDLE** result
 )
 {
	 SEQHT;
	 LOGHT("Open file with name: ", FileName);
	 HT::HTHANDLE* temp = HT::Open(FileName,userName,password);
	 if (temp == NULL)
		 return E_FAIL;
	 *result = temp;
	 if (*result != NULL)
		 return S_OK;
	 else
		 return E_FAIL;
 }
// != NULL успешное завершение  

 HRESULT STDMETHODCALLTYPE OS13::Snap         // выполнить Snapshot
 (
	 HT::HTHANDLE**  hthandle           // управление HT (File, FileMapping)
 ) {
	 if (HT::Snap(*hthandle))
		 return S_OK;
	 else
		 return E_FAIL;
 }


 HRESULT STDMETHODCALLTYPE OS13::Close        // Snap и закрыть HT  и  очистить HT::HTHANDLE
 (
	 HT::HTHANDLE** hthandle
	 //DWORD address         // управление HT (File, FileMapping)
 )
 {
	 //bool res = false;
	 //bool res = HT::Close((HT::HTHANDLE*)((void*)address));
	 bool res = HT::Close(*hthandle);
	 //LOGHT("result of closing ht", res);
	 //new ((void*)address)  HT::HTHANDLE(0, 0, 0, 0, L"asd");
	 //(HT::HTHANDLE*)((void*)address) = new HT::HTHANDLE(0, 0, 0, 0, L"asd");
	 //HT::HTHANDLE* ht2 = new HT::HTHANDLE(0, 0, 0, 0, L"asd");
	 //memcpy(hthandle, ht2, sizeof(ht2));
	 //hthandle = NULL;
	 //ht2 = HT::HTHANDLE(0, 0, 0, 0, L"asd");
	 //hthandle = HT::HTHANDLE();
	 if (res)
		 return S_OK;
	 else
		 return E_FAIL;
 }
//  == TRUE успешное завершение   


 HRESULT STDMETHODCALLTYPE OS13::Insert      // добавить элемент в хранилище
 (
	 HT::HTHANDLE**  hthandle,            // управление HT
	 HT::Element* element              // элемент
 )
 {
	 //SEQHT;
	 //LOGHT("Inserting element with key: ", *(int*)element->key);
	 //LOGHT("Inserting element with ht: ", &hthandle);
	 //Sleep(500);
	 if (HT::Insert(*hthandle, element))
		 return S_OK;
	 else
		 return E_FAIL;
 }
//  == TRUE успешное завершение 


 HRESULT STDMETHODCALLTYPE OS13::Delete      // удалить элемент в хранилище
 (
	 HT::HTHANDLE**  hthandle,            // управление HT (ключ)
	 HT::Element* element              // элемент 
 )
 {
	 if (HT::Delete(*hthandle, element))
		 return S_OK;
	 else
		 return E_FAIL;
 }
//  == TRUE успешное завершение 

 HRESULT STDMETHODCALLTYPE OS13::Get     //  читать элемент в хранилище
 (
	 HT::Element& result,
	 HT::HTHANDLE**  hthandle,            // управление HT
	 HT::Element* element              // элемент 
 )
 {
	 SEQHT;
	 //result = *HT::Get(&hthandle, element);
	 //if (HT::Get(&hthandle, element)==NULL)
		// return E_FAIL;
	 HT::Element* temp = HT::Get(*hthandle, element);
	 LOGHT("Get element with temp: ", temp);
	 if (temp == NULL)
		 return E_FAIL;
	 result = *temp;
	 
	 //result = *temp;
	 //LOGHT("Get element with temp: ", temp);
	 //LOGHT("Get element with result: ", &result);
	 ////delete temp;
	 //LOGHT("Get element with temp: ", temp);
	 //LOGHT("Get element with result: ", &result);
	 return S_OK;
 }
//  != NULL успешное завершение 


 HRESULT STDMETHODCALLTYPE OS13::Update     //  именить элемент в хранилище
(
	HT::HTHANDLE**  hthandle,            // управление HT
	HT::Element* oldelement,          // старый элемент (ключ, размер ключа)
	void* newpayload,          // новые данные  
	int             newpayloadlength     // размер новых данных
)
 {
	 if (HT::Update(*hthandle, oldelement, newpayload, newpayloadlength))
		 return S_OK;
	 else
		 return E_FAIL;
 }
 //  != NULL успешное завершение 

 HRESULT STDMETHODCALLTYPE OS13::GetLastHtError  // получить сообщение о последней ошибке
 (
	 std::string& error,
	 HT::HTHANDLE**  ht                         // управление HT
 ) {
	 error = std::string(HT::GetLastHtError(*ht));
	 LOGHT("Ht error:  ", error);
	 return S_OK;
 }