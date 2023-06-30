#pragma once

#include "resource.h"
HANDLE targetHdl = NULL;
extern class PlayerData user;
extern class PlayerData Enemy1;
extern HWND gHwnd; // ESP 핵을 위한 그리기 핸들을 전역변수로 선언
extern HANDLE pHandle;//AssaultCubeHandle 
class PlayerData
{
    


    int hpOffset = 0xEC;
    int bulletNumOffset = 0x140;
    int xPosOffset = 0x28;
    int yPosOffset = 0x2C;
    int zPosOffset = 0x30;

    int nameOffset = 0x205;

    char name[20];
    int hp = 0;
    int bullet = 0;
    float x = 0;
    float y = 0;
    float z = 0;

    
public:
    float xDegree;
    float yDegree;
    int xDegreeOffset = 0x34;
    int yDegreeOffset = 0x38;

    uintptr_t playerBaseAddr; 

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
    BOOL SetPlayerDataAddr(HANDLE pHnd, uintptr_t base, int offset)
    {
        SIZE_T readbytes;
        return ReadProcessMemory(pHnd, (LPCVOID)(base + offset), &playerBaseAddr, sizeof(playerBaseAddr), &readbytes);

    }
    void GetPlayerData(HANDLE pHnd)
    {
        SIZE_T readbytes;
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + hpOffset), &hp, sizeof(hp), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + bulletNumOffset), &bullet, sizeof(bullet), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + xPosOffset), &x, sizeof(x), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + yPosOffset), &y, sizeof(y), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + zPosOffset), &z, sizeof(z), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + xDegreeOffset), &xDegree, sizeof(xDegree), &readbytes);
        ReadProcessMemory(pHnd, (LPCVOID)(playerBaseAddr + yDegreeOffset), &yDegree, sizeof(yDegree), &readbytes);



    }
};
float GetDistance()
{
    double xd = pow(Enemy1.GetXpos() - user.GetXpos(), 2);
    double yd = pow(Enemy1.GetYpos() - user.GetYpos(), 2);
    double zd = pow(Enemy1.GetZpos() - user.GetZpos(), 2);
    double distance = sqrt(pow(sqrt(xd + yd), 2) + pow(zd, 2));
    return distance;

}
float Get2Dangle() {
    double x = Enemy1.GetXpos() - user.GetXpos();
    double y = Enemy1.GetYpos() - user.GetYpos();
    double angle =atan(y / x) * 180/ 3.141592653589793;

    if (x < 0)
        angle = 270 + angle;    
    else
        angle = 90 + angle; 

    return angle;
}
float GetYangle() {
    double xd = pow(Enemy1.GetXpos() - user.GetXpos(), 2); 
    double yd = pow(Enemy1.GetYpos() - user.GetYpos(), 2); 

    double xyd = sqrt(xd + yd);
    double zd = Enemy1.GetZpos() - user.GetZpos();
    double valid = 1;

    if (yd < 0)
        valid = -1;
    return valid*atan(zd/xyd) * 180 / 3.141592653589793;

}
bool SetWallHack(HDC hdc)
{
    float distance;
    float x_angle;
    float y_angle;
    distance = GetDistance();
    x_angle = Get2Dangle();
    y_angle = GetYangle();
    HWND tHdl = NULL;
    RECT tRect = { 0, };
    int tWidth, tHeight;    
    int sightX = 90;
    int sightY = 80;
    float pixelOfDegreeX = 0; 
    float pixelOfDegreeY = 0;


    tHdl = FindWindow(0, L"AssaultCube"); 
  
  
    if (!GetWindowRect(tHdl, &tRect))
    {
        MessageBox(NULL, L"Failed Find AssaultCube", NULL, NULL);
        return false;
    }
    tWidth = tRect.right - tRect.left-16;
    tHeight = tRect.bottom - tRect.top-40;
    
    pixelOfDegreeX = tWidth / sightX ;
    pixelOfDegreeY = tWidth / sightY; 



    RECT drawBox;
    drawBox = { 0, };
    int sizex = 1000/distance;
    int sizey = 2000 / distance;
    int resizing = 80;
    if ((user.xDegree + (sightX / 2)) >= x_angle && (user.xDegree - (sightX / 2)) <= x_angle)
    {
        drawBox.left = (tWidth / 2) - sizex - ((user.xDegree - x_angle) * pixelOfDegreeX);
        drawBox.top = (tHeight / 2) - sizey +((user.yDegree -y_angle) * pixelOfDegreeY)  +resizing ;
        drawBox.right = (tWidth / 2) + sizex - ((user.xDegree - x_angle) * pixelOfDegreeX);
        drawBox.bottom = (tHeight / 2) + sizey + ((user.yDegree - y_angle) * pixelOfDegreeY) +resizing;
    }

    HBRUSH newBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, newBrush);
    FrameRect(hdc, &drawBox, newBrush);
    DeleteObject(newBrush);
    
    return true;
}
void SetAimHack(HDC hdc)
{
    float distance; 
    float x_angle; 
    float y_angle; 
    distance = GetDistance(); 
    x_angle = Get2Dangle(); 
    y_angle = GetYangle(); 
    RECT rect;
    rect.left = 10;
    rect.top = 10;

    rect.top = 110;
    std::wstring text = L"Enemy distance: " + std::to_wstring(distance);
    DrawText(hdc, text.c_str(), text.length(), &rect, DT_SINGLELINE | DT_NOCLIP);

    rect.top = 130;
    std::wstring textd = L"Enemy angle " + std::to_wstring(x_angle);
    DrawText(hdc, textd.c_str(), textd.length(), &rect, DT_SINGLELINE | DT_NOCLIP);

    rect.top = 150;
    std::wstring textb = L"Enemy Y angle " + std::to_wstring(y_angle);
    DrawText(hdc, textb.c_str(), textb.length(), &rect, DT_SINGLELINE | DT_NOCLIP);
    if(GetAsyncKeyState(0x02)&0x8000)
    {
        WriteProcessMemory(pHandle, (LPVOID)(user.playerBaseAddr + user.xDegreeOffset), &x_angle, sizeof(x_angle), NULL);  
        WriteProcessMemory(pHandle, (LPVOID)(user.playerBaseAddr + user.yDegreeOffset), &y_angle, sizeof(y_angle), NULL); 
    } 

}
void DrawUserData()
{
    ShowWindow(gHwnd, SW_SHOW); 
    
    Enemy1.GetPlayerData(pHandle); 
    user.GetPlayerData(pHandle); 
    InvalidateRect(gHwnd, NULL, TRUE); 

}


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
