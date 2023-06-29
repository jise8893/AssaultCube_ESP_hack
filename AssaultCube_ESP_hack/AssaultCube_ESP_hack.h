#pragma once

#include "resource.h"
HANDLE targetHdl = NULL;
class PlayerData
{
	uintptr_t playerBaseAddr;


    int hpOffset = 0xEC;
    int bulletNumOffset = 0x140;
    int xPosOffset = 0x28;
    int yPosOffset = 0x2C;
    int zPosOffset = 0x30;
    
    int nameOffset = 0x205;

    char name[20];
	int hp=0;
    int bullet = 0;
	float x=0;
	float y=0;
	float z=0;
public :
    int GetHp() {
        return hp;
    }
    int GetBullet() {
        return bullet;
    }
    float GetXpos() {
        return x;
    }
    float GetYpos() {
        return y;
    }
    float GetZpos() {
        return z;
    }
	BOOL SetPlayerDataAddr(HANDLE pHnd,uintptr_t base,int offset)
	{
        SIZE_T readbytes;
        return ReadProcessMemory(pHnd, (LPCVOID)(base + offset), &playerBaseAddr, sizeof(playerBaseAddr), &readbytes);

	}
    void GetPlayerData(HANDLE pHnd)
    {
        SIZE_T readbytes;
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + hpOffset), &hp, sizeof(hp), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + bulletNumOffset), &bullet, sizeof(bullet), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + xPosOffset ), &x, sizeof(x), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + yPosOffset), &y, sizeof(y), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + zPosOffset), &z, sizeof(z), &readbytes);
        
        
    }
};
HMODULE GetBaseAddressByName(PROCESSENTRY32* sub)
{



    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, sub->th32ProcessID);

    if (NULL != hProcess)
    {
        HMODULE hMod;
        //HINSTANCE 핸들은 보통 실행되고 있는 Win32 프로그램의 메모리 상에 올라가 있는 시작 주소값을 갖고 있음
        DWORD cbNeeded;

        if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_32BIT))
            //EnumProcessModulesEx : 지정된 필터 조건을 충족시키는 지정된 프로세스의 각 모듈에 대한 핸들을 검색
        {
            GetModuleBaseName(hProcess, hMod, sub->szExeFile, sizeof(sub->szExeFile) / sizeof(TCHAR));
            //GetModuleBaseName : 지정된 모듈의 기본 이름을 검색
            //GetModuleBaseName 함수는 주로 디버거 및 다른 프로세스에서 모듈 정보를 추출해야하는 유사한 응용 프로그램에서 사용하도록 설계됨

            //ReadProcessMemory(hProcess, hMod + 0x108E, buf2, 6, &readSize);
            return hMod;

        }
    }

    CloseHandle(hProcess);
}
HMODULE GetProcessBaseAddressByName(LPCWSTR string) {
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    uintptr_t ret;
    SIZE_T readbytes;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        return 0;
    }

    do
    {
        if (!wcscmp(pe32.szExeFile, string))
        {
            MessageBox(NULL, L"Success Find AC_CLIENT.EXE", NULL, NULL);
            targetHdl = OpenProcess(PROCESS_ALL_ACCESS, 0, pe32.th32ProcessID);
            if (targetHdl != NULL)
            {
                HMODULE hMod= GetBaseAddressByName(&pe32); 
                

                return hMod;
            }
               

            if (targetHdl == NULL)
                return 0;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    return 0;
}
