#include "pch.h"
#include <Windows.h>
#include <iostream>

using namespace std;
namespace AUTH {

    bool checkGroupExisting(LPCWSTR groupName);
    bool checkUserGroups(LPCWSTR userName, LPCWSTR groupName);
    bool checkCurrentUserGroup(LPCWSTR groupName);
    bool checkUsersCred(LPCWSTR name, LPCWSTR pass);
}