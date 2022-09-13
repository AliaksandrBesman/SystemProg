#pragma once
#include <Windows.h>

namespace HT    // HT API
{
	// API HT - ����������� ��������� ��� ������� � ��-��������� 
	//          ��-��������� ������������� ��� �������� ������ � �� � ������� ����/��������
	//          ���������������� (�����������) ������ �������������� � ������� snapshot-���������� 
	//          Create - �������  � ������� HT-��������� ��� �������������   
	//          Open   - ������� HT-��������� ��� �������������
	//          Insert - ������� ������� ������
	//          Delete - ������� ������� ������    
	//          Get    - ������  ������� ������
	//          Update - �������� ������� ������
	//          Snap   - �������� snapshot
	//          Close  - ��������� Snap � ������� HT-��������� ��� �������������
	//          GetLastError - �������� ��������� � ��������� ������

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
		int     Capacity;               // ������� ��������� � ���������� ��������� 
		int     SecSnapshotInterval;    // ������������� ���������� � ���. 
		int     MaxKeyLength;           // ������������ ����� �����
		int     MaxPayloadLength;       // ������������ ����� ������
	};

#pragma endregion My additions

	struct HTHANDLE    // ���� ���������� HT
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

		TCHAR   FileName[512];					// ��� ����� 
		HANDLE  File;							// File HANDLE != 0, ���� ���� ������
		HANDLE  FileMapping;					// Mapping File HANDLE != 0, ���� mapping ������  
		LPVOID  Addr;							// Addr != NULL, ���� mapview �������� (start of file view)
		LPVOID  ContentAddr;					// ptr to actual content in a file view (Addr + ContentOffset)

		time_t  LastSnapTime;					// ���� ���������� snap'a (time())
		time_t  LastChangeTime;					//

		TCHAR   LastErrorMessage[512];			// ��������� �� ��������� ������ ��� 0x00  
	};

	struct Element   // ������� 
	{
		Element();
		Element(LPCVOID key, int keylength);                                             // for Get
		Element(LPCVOID key, int keylength, LPCVOID payload, int  payloadlength);    // for Insert
		Element(Element* oldelement, LPCVOID newpayload, int  newpayloadlength);         // for update
		LPCVOID		    key;                 // �������� ����� 
		int             keylength;           // ������ �����
		LPCVOID		    payload;             // ������ 
		int             payloadlength;       // ������ ������
	};
	
	HTHANDLE*  Create   //  ������� HT             
	(
		int			Capacity,						// ������� ���������
		int			SecSnapshotInterval,			// ������������� ���������� � ���.
		int			MaxKeyLength,					// ������������ ������ �����
		int			MaxPayloadLength,				// ������������ ������ ������
		const TCHAR	FileName[512]					// ��� ����� 
	); 	// != NULL �������� ����������  

	HTHANDLE*   Open     //  ������� HT             
	(
		const TCHAR    FileName[512]		// ��� ����� 
	); 	// != NULL �������� ����������  

	BOOL Snap         // ��������� Snapshot
	(
		const HTHANDLE*    hthandle           // ���������� HT (File, FileMapping)
	);
	

	BOOL Close        // Snap � ������� HT  �  �������� HTHANDLE
	(
		HTHANDLE*    hthandle           // ���������� HT (File, FileMapping)
	);	//  == TRUE �������� ����������   

		
	BOOL Insert      // �������� ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT
		const Element*  element              // �������
	);	//  == TRUE �������� ���������� 


	BOOL Delete      // ������� ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT (����)
		const Element*  element              // ������� 
	);	//  == TRUE �������� ���������� 

	Element* Get     //  ������ ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT
		const Element*  element              // ������� 
	); 	//  != NULL �������� ���������� 

	
	BOOL Update     //  ������� ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT
		const Element*  oldelement,          // ������ ������� (����, ������ �����)
		const void*     newpayload,          // ����� ������  
		int             newpayloadlength     // ������ ����� ������
	); 	//  != NULL �������� ���������� 

	TCHAR* HTGetLastError  // �������� ��������� � ��������� ������
	(
		HTHANDLE* ht                         // ���������� HT
	);
	
	void print                               // ����������� ������� 
	(
		const Element*  element              // ������� 
	);

	
};
