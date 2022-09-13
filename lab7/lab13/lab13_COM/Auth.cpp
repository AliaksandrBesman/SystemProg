#include "pch.h"
#include "Auth.h"
#include <Windows.h>
#include <iostream>
#include <iostream>
#include <LMaccess.h>
#include <Lm.h>
#include "SEQLOG.h"
#pragma comment (lib,"netapi32.lib")

using namespace std;

bool  AUTH::checkGroupExisting(LPCWSTR groupName) {
    GROUP_INFO_0* groupsInfo;
    DWORD readed = 0, total = 0;
    NetLocalGroupEnum(
        NULL,
        0,
        (LPBYTE*)&groupsInfo,
        MAX_PREFERRED_LENGTH,
        &readed,
        &total,
        NULL
    );
    bool exosts = false;
    for (int i = 0; i < readed; i++) {
        int res = lstrcmpW(groupName, groupsInfo[i].grpi0_name);
        if (res == 0) {
            exosts = true;
            break;
        }
    }
    NetApiBufferFree((LPVOID)groupsInfo);
    //if(exosts)
    //    wprintf(L"\ngroup %s exists", groupName);
    return exosts;
}
bool  AUTH::checkUserGroups(LPCWSTR userName, LPCWSTR groupName) {
    //setlocale(LC_ALL, "rus");
    GROUP_USERS_INFO_0* groupUsersInfo;
    DWORD uc = 0, tc = 0;
    NET_API_STATUS ns = NetUserGetLocalGroups(
        NULL,
        userName,
        0,
        LG_INCLUDE_INDIRECT,
        (LPBYTE*)&groupUsersInfo,
        MAX_PREFERRED_LENGTH,
        &uc,
        &tc
    );
    bool exosts = false;
    if (ns == NERR_Success) {
        for (int i = 0; i < uc; i++) {
            int res = lstrcmpW(groupName, groupUsersInfo[i].grui0_name);
            if (res == 0) {
                exosts = true;
                break;
            }
        }
        NetApiBufferFree((LPVOID)groupUsersInfo);
    }
    return exosts;
}
bool  AUTH::checkCurrentUserGroup(LPCWSTR groupName) {
    WCHAR currentUserName[512];
    DWORD lenUserName = 512;
    bool resGetUser = GetUserName(currentUserName, &lenUserName);

    wofstream myfile;
    myfile.open("d:\\sem6\\os\\os13COM_HT\\lab13\\Release\\authLog.txt", ios::app);
    if ((int)currentUserName[0]<1000){
        myfile << L"\nlenUserName:" << lenUserName;
        myfile << L"\nresGetUser:" << resGetUser;
        for (int i = 0; i < lenUserName; i++) {
            myfile << L"\ncurrentUserName[" << i << "]:" << (int)currentUserName[i];
            myfile << L"\ncurrentUserName[" << i << "]:" << (WCHAR)currentUserName[i];

        }
        wstring wstr = wstring(currentUserName);
        myfile << L"\ncurrentUserName wstr:" << wstr;
        myfile << L"\ncurrentUserName:" << currentUserName;
        myfile << L"\ngroupName:" << groupName;
    }
    else {
        myfile << L"\ncurrentUserName null";
        return true;
    }
    myfile.close();

    return checkUserGroups(currentUserName, groupName);
}
bool  AUTH::checkUsersCred(LPCWSTR name, LPCWSTR pass) {
    bool res;
    HANDLE hToken = 0;
    res = LogonUserW(
        name,
        L".",
        pass,
        LOGON32_LOGON_INTERACTIVE,
        LOGON32_PROVIDER_DEFAULT,
        &hToken
    );
    CloseHandle(hToken);
    return res;
}

