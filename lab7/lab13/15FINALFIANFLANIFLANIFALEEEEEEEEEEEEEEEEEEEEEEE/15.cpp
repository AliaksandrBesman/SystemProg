#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include "Service.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>


using namespace std;

int main()
{
    WCHAR ServiceName[] = SERVICENAME;
    SERVICE_TABLE_ENTRY ServiceStartTable[] = {
        {ServiceName, ServiceMain},
        {NULL,NULL}
    };


    if (!StartServiceCtrlDispatcher(ServiceStartTable)) {
        trace("startServiceCtrlDispacther", std::ofstream::out);
    }

    std::cout << "Hello World!\n";
    system("pause");
    return 0;
}
