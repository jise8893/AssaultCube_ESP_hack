﻿// AssaultCube_ESP_hack.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "AssaultCube_ESP_hack.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];   // 기본 창 클래스 이름입니다.
HWND gHwnd; // ESP 핵을 위한 그리기 핸들을 전역변수로 선언
HANDLE pHandle;//AssaultCubePhandle


//게임핵 내에서 사용할 전역변수
int pid = 0;
PlayerData user;


void DrawUserData()
{
    ShowWindow(gHwnd, SW_SHOW);
    InvalidateRect(gHwnd, NULL, TRUE);
}

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    HWND tHdl = FindWindow(0, L"AssaultCube");
    DWORD pid = 0;
    GetWindowThreadProcessId(tHdl, &pid);
    pHandle = OpenProcess(PROCESS_VM_READ, 0, pid);
    uintptr_t hMod = (uintptr_t)GetProcessBaseAddressByName(L"ac_client.exe");


    if (user.SetPlayerDataAddr(pHandle, hMod, 0x0017E0A8) == FALSE)
        return 0;


    user.GetPlayerData(pHandle);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASSAULTCUBEESPHACK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
   
    SIZE_T size=0;
        
        

       


    


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASSAULTCUBEESPHACK));

    MSG msg;
    while (1) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            DrawUserData();
        }
        Sleep(16);
    }
    // 기본 메시지 루프입니다:
    

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASSAULTCUBEESPHACK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASSAULTCUBEESPHACK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

  

   HWND tHdl = NULL;
   RECT tRect;
   int tWidth, tHeight;

 
   

   tHdl = FindWindow(0, L"AssaultCube");
   if (!GetWindowRect(tHdl, &tRect))
   {
       MessageBox(NULL, L"Failed Find AssaultCube", NULL, NULL);
       return false;
   }
   tWidth = tRect.right - tRect.left;
   tHeight = tRect.bottom - tRect.top;
   HWND hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT,szWindowClass, L"kkuraop-AssaultCubeHack",
       (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU)| WS_POPUP, tRect.left, tRect.top, tWidth, tHeight, NULL, NULL, hInstance, NULL);
 
   SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);  
   SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);  

   if (!hWnd)
   {
      return FALSE;
   }
   
   gHwnd = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   


   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
        
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            RECT rect;
            rect.left = 10;
            rect.top = 10;
            user.GetPlayerData(pHandle); 
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 0, 0));
            DrawText(hdc, L"kkuraop AssaultCube hack", -1, &rect, DT_SINGLELINE | DT_NOCLIP);

            rect.top = 30;
            std::wstring text = L"User Hp: " + std::to_wstring(user.GetHp());
            DrawText(hdc, text.c_str(), text.length(), &rect, DT_SINGLELINE | DT_NOCLIP);
            rect.top = 50;
            std::wstring textb = L"User Bullet: " + std::to_wstring(user.GetBullet());
            DrawText(hdc, textb.c_str(), textb.length(), &rect, DT_SINGLELINE | DT_NOCLIP);

            EndPaint(hWnd, &ps);

        
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        PostQuitMessage(0); 
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
