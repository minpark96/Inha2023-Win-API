// WinAPI_Proj.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI_Proj.h"
#include <CommCtrl.h>

// >> :
#pragma comment(lib, "msimg32.lib")

HBITMAP hBackImage;
BITMAP bitBack;

HBITMAP hFrontImage;
BITMAP bitFront;

HBITMAP hTransparentImage;
BITMAP bitTransparent;

HBITMAP hAniImage;
BITMAP bitAni;
const int SPRITE_SIZE_X = 57;
const int SPRITE_SIZE_Y = 52;

int RUN_FRAME_MAX = 0;
int RUN_FRAME_MIN = 0;
int curframe = RUN_FRAME_MIN;

int SPRITE_FRAME_COUNT_X = 0;
int SPRITE_FRAME_COUNT_Y = 0;

RECT rectView;

// >> : FUNCS..
void CreateBitmap();
void DrawBitmap(HWND hWnd, HDC hdc);
void DeleteBitmap();
void UpdateFrame(HWND hWnd);
void DrawRectText(HDC hdc);

// >> :double buffering
HBITMAP hDoubleBufferImage;
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);

VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

HWND hModalessDlg;
BOOL CALLBACK Dialog_Test1_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void MakeColumn(HWND hDlg);
void InsertData(HWND hDlg);

// << :

// >> : GDI+

ULONG_PTR g_GdiToken;

void Gdi_Init();
void Gdi_Draw(HDC hdc);
void Gdi_End();

// << :

// >> : split windows
HWND ChildWnd[3];
LRESULT CALLBACK ChildWndProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
// << :

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

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

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    Gdi_Init();

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        Gdi_End();
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;

    // 기본 메시지 루프입니다:
    /*while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    Gdi_End();

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJ));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIPROJ);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    // : split window 1
    wcex.lpfnWndProc = ChildWndProc1;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _T("Child Window Class 1");
    RegisterClassExW(&wcex);
    // : split window 2
    wcex.lpfnWndProc = ChildWndProc2;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _T("Child Window Class 2");
    RegisterClassExW(&wcex);
    // : split window 3
    wcex.lpfnWndProc = ChildWndProc3;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _T("Child Window Class 3");
    RegisterClassExW(&wcex);

    return NULL;
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define timer_ID_1 11
#define timer_ID_2 123 // << : for ani

//void OutFromFile(TCHAR filename[], HWND hwnd)
//{
//    FILE* fPtr;
//    HDC hdc;
//    int line;
//    TCHAR buffer[500];
//    line = 0;
//    hdc = GetDC(hwnd);
//#ifndef _UNICODE
//    _tfopen_s(&fPtr, filename, _T("r, ccs = UNICODE"));
//#else
//    _tfopen_s(&fPtr, filename, _T("r"));
//#endif
//    while (_fgetts(buffer, 100, fPtr) != NULL)
//    {
//        if (buffer[_tcslen(buffer) - 1] == _T('\n'))
//            buffer[_tcslen(buffer) - 1] = NULL;
//        TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
//        line++;
//    }
//    fclose(fPtr);
//    ReleaseDC(hwnd, hdc);
//}

//void OutToFile(TCHAR filename[], std::list<TCHAR*> li)
//{
//    FILE* fPtr;
//    TCHAR buffer[500];
//#ifndef _UNICODE
//    _tfopen_s(&fPtr, filename, _T("r, ccs = UNICODE"));
//#else
//    _tfopen_s(&fPtr, filename, _T("r"));
//#endif
//    if (!li.empty())
//    {
//        for (auto it = li.begin(); it != li.end(); it++)
//        {
//            _fputts(*it, fPtr);
//        }
//
//    }
//
//    fclose(fPtr);
//}

//double LengthPts(POINT pt1, POINT pt2)
//{
//    return (sqrt((float)(pt2.x - pt1.x) * (pt2.x - pt1.x) + (float)(pt2.y - pt1.y) * (pt2.y - pt1.y)));
//}
//
//BOOL InCirCle(POINT pt1, POINT pt2, double r)
//{
//    if (LengthPts(pt1, pt2) < r) return TRUE;
//    else return FALSE;
//}

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
    using namespace std;

    PAINTSTRUCT ps;
    HDC hdc;

    static POINT ptMousePos;
    //static RECT rectView;
    static bool bFlag = false;
    enum type {NONE, CIRCLE, RECTANGLE, STAR};
    static int selectedMenu = NONE;

    static HMENU hMenu, hSubMenu;

    switch (message)
    {
    case WM_SIZE :
        GetClientRect(hWnd, &rectView);
        break;
    case WM_CREATE:
        srand(time(NULL));
        GetClientRect(hWnd, &rectView);
        //SetTimer(hWnd, timer_ID_1, 10, NULL);
        //SetTimer(hWnd, timer_ID_2, 10, AniProc);
        CreateBitmap();

        // >> : split window
        {
            ChildWnd[0] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class 1"),
                NULL, WS_CHILD | WS_VISIBLE, 0, 0, rectView.right / 2 - 1, rectView.bottom / 2 - 1,
                hWnd, NULL, hInst, NULL);
            ChildWnd[1] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class 2"),
                NULL, WS_CHILD | WS_VISIBLE, 0, rectView.bottom / 2 + 1, rectView.right, rectView.bottom / 2 - 1,
                hWnd, NULL, hInst, NULL);
            ChildWnd[2] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class 3"),
                NULL, WS_CHILD | WS_VISIBLE, rectView.right / 2 + 1, 0, rectView.right, rectView.bottom / 2 - 1,
                hWnd, NULL, hInst, NULL);
        }

        break;
    /*case WM_TIMER:
        if (wParam == timer_ID_2)
        {
            UpdateFrame(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;*/
    //case WM_PAINT:
    //{
    //    hdc = BeginPaint(hWnd, &ps);
    //    // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

    //    DrawBitmapDoubleBuffering(hWnd, hdc);
    //    //DrawBitmap(hWnd, hdc);
    //    DrawRectText(hdc);

    //        EndPaint(hWnd, &ps);
    //    }
    //    break;


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
            case ID_DRAW_RECT:
                selectedMenu = RECTANGLE;
                /*DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dialog_Test1_Proc);*/

                hModalessDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1),
                    hWnd, Dialog_Test1_Proc);
                ShowWindow(hModalessDlg, SW_SHOW);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, timer_ID_2);
        DeleteBitmap();
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

void CreateBitmap()
{
    // >> : 수지
    {
        hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/해린2.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        if (hBackImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러"), _T("에러"), MB_OK);
            return;
        }
        GetObject(hBackImage, sizeof(BITMAP), &bitBack);
    }

    // >> : background.bmp
    {
        {
            hFrontImage = (HBITMAP)LoadImage(NULL, TEXT("images/Background.bmp"),
                IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

            if (hFrontImage == NULL)
            {
                DWORD dwError = GetLastError();
                MessageBox(NULL, _T("이미지 로드 에러"), _T("에러"), MB_OK);
                return;
            }
            GetObject(hFrontImage, sizeof(BITMAP), &bitFront);
        }
    }

    // >> : 시공
    {
        hTransparentImage = (HBITMAP)LoadImage(NULL, TEXT("images/sigong.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        if (hTransparentImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러"), _T("에러"), MB_OK);
            return;
        }
        GetObject(hTransparentImage, sizeof(BITMAP), &bitTransparent);
    }

    // >> : 애니
    {
        hAniImage = (HBITMAP)LoadImage(NULL, TEXT("images/zero_run.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        if (hAniImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러"), _T("에러"), MB_OK);
            return;
        }
        GetObject(hAniImage, sizeof(BITMAP), &bitAni);

        // : 
        RUN_FRAME_MAX = bitAni.bmWidth / SPRITE_SIZE_X - 1;
        RUN_FRAME_MIN = 2;
        curframe = RUN_FRAME_MIN;

        SPRITE_FRAME_COUNT_X = bitAni.bmWidth / SPRITE_SIZE_X;
        SPRITE_FRAME_COUNT_Y = bitAni.bmHeight / SPRITE_SIZE_Y;
    }
}

void DrawBitmap(HWND hWnd, HDC hdc)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;

    { // >> : 수지
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }

    { // >> : 시공
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hTransparentImage);
        bx = bitTransparent.bmWidth;
        by = bitTransparent.bmHeight;

        TransparentBlt(hdc, 450, 130, bx * 2, by * 2, hMemDC, 0, 0, bx, by, RGB(255,0,255));
        TransparentBlt(hdc, 218, 250, bx / 2, by / 2 , hMemDC, 0, 0, bx, by, RGB(255, 0, 255));
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }

    { // >> : 애니
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hAniImage);
        bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y;

        int xStart = curframe * bx;
        int yStart = 0;
        static int xPos = 0;
        TransparentBlt(hdc, 150 + xPos, 200, bx, by, hMemDC, xStart, yStart, bx, by, RGB(255, 0, 255));
        StretchBlt(hdc, 150 + xPos, 300, -bx, by, hMemDC, xStart, yStart, bx, by, SRCCOPY);
        xPos++;
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }
}

void DeleteBitmap()
{
    DeleteObject(hBackImage);
    DeleteObject(hTransparentImage);
    DeleteObject(hAniImage);
}

void UpdateFrame(HWND hWnd)
{
    curframe++;
    if (curframe > RUN_FRAME_MAX)
        curframe = RUN_FRAME_MIN;
    InvalidateRect(hWnd, NULL, false);
}

static int yPos = 0;

void DrawRectText(HDC hdc)
{
    TCHAR strTest[] = _T("이미지 출력");
    TextOut(hdc, 10, yPos, strTest, _tcslen(strTest));
}

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;

    HDC hMemDC2;
    HBITMAP hOldBitmap2;

    hMemDC = CreateCompatibleDC(hdc);
    if (hDoubleBufferImage == NULL)
    {
        hDoubleBufferImage = CreateCompatibleBitmap(hdc,
            rectView.right, rectView.bottom);
    }
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);

    { // >> : 수지
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage);
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        BitBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, SRCCOPY);
        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }


    { // >> : 시공
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hTransparentImage);
        bx = bitTransparent.bmWidth;
        by = bitTransparent.bmHeight;

        TransparentBlt(hMemDC, 450, 130, bx * 2, by * 2, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));
        TransparentBlt(hMemDC, 218, 250, bx / 2, by / 2, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));
        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }

    { // >> : 애니
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hAniImage);
        bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y;

        int xStart = curframe * bx;
        int yStart = 0;
        static int xPos = 0;
        TransparentBlt(hMemDC, 150 + xPos, 200, bx, by, hMemDC2, xStart, yStart, bx, by, RGB(255, 0, 255));
        StretchBlt(hMemDC, 150 + xPos, 300, -bx, by, hMemDC2, xStart, yStart, bx, by, SRCCOPY);
        xPos++;
        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }

    // << : hMemDC 에 그려주기
    { // >> : front
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hFrontImage);
        bx = bitFront.bmWidth;
        by = bitFront.bmHeight;

        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC2, hBrush);

        Ellipse(hMemDC2, 350, 100, 850, 550);

        SelectObject(hMemDC2, oldBrush);
        DeleteObject(hBrush);

        TransparentBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));
        SelectObject(hMemDC2, hOldBitmap2);

        DeleteDC(hMemDC2);
    }
    // >> : hdc에 그려주기

    Gdi_Draw(hMemDC);

    TransparentBlt(hdc, 0, 0, rectView.right, rectView.bottom,
        hMemDC, 0, 0, rectView.right, rectView.bottom,
        RGB(255, 0, 255));
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}

VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    UpdateFrame(hWnd);

    yPos += 5;
    if (yPos > rectView.bottom) yPos = 0;
}

BOOL CALLBACK Dialog_Test1_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static int Check[3], Radio;
    TCHAR hobby[][30] = { _T("독서"), _T("음악감상"), _T("게임") };
    TCHAR sex[][30] = { _T("여성"), _T("남성") };
    TCHAR output[200];

    static HWND hCombo;
    static int selection;
    TCHAR name[20];
    static HWND hList;
    static int selectionList;

    switch (iMsg)
    {
    case WM_INITDIALOG:
    {
        HWND hBtn = GetDlgItem(hDlg, IDC_PAUSE);
        EnableWindow(hBtn, FALSE);

        CheckRadioButton(hDlg, IDC_RADIO_FEMALE, IDC_RADIO_MALE, IDC_RADIO_FEMALE);

        hCombo = GetDlgItem(hDlg, IDC_COMBO_LIST);
        hList = GetDlgItem(hDlg, IDC_LIST_NAME);
        MakeColumn(hDlg); // << :
    }
        return 1;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_INSERT_MEMBER:
            InsertData(hDlg);
            return 0;
        case IDC_BUTTON_INSERT:
            GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
            if (_tcscmp(name, _T("")))
            {
                SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)name);
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)name);
            }
            SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
            return 0;
            break;
        case IDC_BUTTON_DELETE:
            SendMessage(hCombo, CB_DELETESTRING, selection, 0);
            break;
        case IDC_LIST_NAME:
            if (HIWORD(wParam) == LBN_SELCHANGE)
                selectionList = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
            break;
        case IDC_BUTTON_DELETE2:
            SendMessage(hList, LB_DELETESTRING, selectionList, 0);
            break;
        case IDC_COMBO_LIST:
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                selection = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            }
            break;
        case IDC_CHECK_READING:
            Check[0] = 1 - Check[0];
            break;
        case IDC_CHECK_MUSIC:
            Check[1] = 1 - Check[1];
            break;
        case IDC_CHECK_GAME:
            Check[2] = 1 - Check[2];
            break;
        case IDC_RADIO_FEMALE:
            Radio = 0;
            break;
        case IDC_RADIO_MALE:
            Radio = 1;
            break;
        case IDC_BUTTON_OUTPUT:
            _stprintf_s(output, _T("선택한 취미는 %s %s %s 입니다.\r\n")
                _T("선택한 성별은 %s입니다."), 
                Check[0] ? hobby[0] : _T(""), 
                Check[1] ? hobby[1] : _T(""),
                Check[2] ? hobby[2] : _T(""), 
                sex[Radio]);
            SetDlgItemText(hDlg, IDC_EDIT_OUTPUT, output);
            break;
        case IDC_BUTTON_COPY:
        {
            TCHAR str[100];
            GetDlgItemText(hDlg, IDC_EDIT_INPUT, str, 100);
            SetDlgItemText(hDlg, IDC_EDIT_COPY, str);
        }
        break;
        case IDC_BUTTON_CLEAR:
        {
            SetDlgItemText(hDlg, IDC_EDIT_INPUT, _T(""));
            SetDlgItemText(hDlg, IDC_EDIT_COPY, _T(""));
        }
        break;
        case IDC_START:
        {
            HDC hdc = GetDC(hDlg);
            SetDlgItemText(hDlg, IDC_TEXT, _T("Start"));
            ReleaseDC(hDlg, hdc);

            HWND hBtn = GetDlgItem(hDlg, IDC_START);
            EnableWindow(hBtn, FALSE);

            hBtn = GetDlgItem(hDlg, IDC_PAUSE);
            EnableWindow(hBtn, TRUE);
        }
            break;
        case IDC_PAUSE:
        {
            HDC hdc = GetDC(hDlg);
            SetDlgItemText(hDlg, IDC_TEXT, _T("Pause"));
            ReleaseDC(hDlg, hdc);

            HWND hBtn = GetDlgItem(hDlg, IDC_START);
            EnableWindow(hBtn, TRUE);

            hBtn = GetDlgItem(hDlg, IDC_PAUSE);
            EnableWindow(hBtn, FALSE);
        }
            break;
        case IDC_BUTTON_PRINT:
        {
            HDC hdc = GetDC(hDlg);
            TextOut(hdc, 0, 0, _T("Print"), 5);
            SetDlgItemText(hDlg, IDC_TEXT, _T("Print"));
            ReleaseDC(hDlg, hdc);
        }
        break;
        case IDOK:
            //EndDialog(hDlg, 0);
            DestroyWindow(hDlg);
            break;
        case ID_EXIT:
            //EndDialog(hDlg, 0);
            DestroyWindow(hDlg);
            break;
        }
        break;
    }

    return 0;
}

void Gdi_Init()
{
    GdiplusStartupInput gpsi;
    GdiplusStartup(&g_GdiToken, &gpsi, NULL);
}

void Gdi_Draw(HDC hdc)
{
    Graphics graphics(hdc);

    // : text
    SolidBrush brush(Color(255, 255, 0, 0));
    FontFamily fontFamily(L"Times New Roman");
    Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF pointF(10.0f, 20.0f);
    graphics.DrawString(L"Hello GDI+!!", -1, &font, pointF, &brush);

    // : line
    Pen pen(Color(128, 255, 0, 255));
    graphics.DrawLine(&pen, 0, 0, 200, 100);

    // : image
    Image img((WCHAR*)L"images/sigong.png");
    int w = img.GetWidth();
    int h = img.GetHeight();
    graphics.DrawImage(&img, 300, 100, w, h);

    // : ani
    Image img2((WCHAR*)L"images/zero_run.png");
    w = img2.GetWidth() / SPRITE_FRAME_COUNT_X;
    h = img2.GetHeight() / SPRITE_FRAME_COUNT_Y;
    int xStart = curframe * w;
    int yStart = 0;

    ImageAttributes imgAttr0;
    imgAttr0.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
    graphics.DrawImage(&img2, Rect(400, 100, w, h), xStart, yStart, w, h, UnitPixel, &imgAttr0);



    // >> : rotation
    Image* pImg = nullptr;
    pImg = Image::FromFile((WCHAR*)L"images/sigong.png");
    int xPos = 400;
    int yPos = 200;
    if (pImg)
    {
        w = pImg->GetWidth();
        h = pImg->GetHeight();

        Gdiplus::Matrix mat;
        static int rot = 0;
        mat.RotateAt((rot % 360),
            Gdiplus::PointF(xPos + (float)(w / 2), yPos+(float)(h / 2))
            );
        graphics.SetTransform(&mat);
        graphics.DrawImage(pImg, xPos, yPos, w, h);
        rot += 10;

        mat.Reset();
        graphics.SetTransform(&mat);
    }

    ImageAttributes imgAttr;
    imgAttr.SetColorKey(Color(240, 0, 240), Color(255, 10, 255));
    xPos = 500;
    graphics.DrawImage(pImg, Rect(xPos, yPos, w, h),
        0, 0, w, h, UnitPixel, & imgAttr);

    if (pImg)
    {
        REAL transparency = 0.5f;
        ColorMatrix colorMatrix =
        {
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, transparency, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f
        };
        imgAttr.SetColorMatrix(&colorMatrix);
        xPos = 600;
        graphics.DrawImage(pImg, Rect(xPos, yPos, w, h),
            0, 0, w, h, UnitPixel, &imgAttr);

        ColorMatrix grayMatrix =
        {
            0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
            0.6f, 0.6f, 0.6f, 0.0f, 0.0f,
            0.1f, 0.1f, 0.1f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f
        };
        imgAttr.SetColorMatrix(&grayMatrix);
        xPos = 700;
        graphics.DrawImage(pImg, Rect(xPos, yPos, w, h),
            0, 0, w, h, UnitPixel, &imgAttr);

        xPos = 800;
        pImg->RotateFlip(RotateNoneFlipX);
        graphics.DrawImage(pImg, Rect(xPos, yPos, w, h),
            0, 0, w, h, UnitPixel, &imgAttr);

        delete pImg;
    }

    // >> : alpha rect
    brush.SetColor(Color(128, 255, 0, 0));
    graphics.FillRectangle(&brush, 100, 100, 200, 300);
}

void Gdi_End()
{
    GdiplusShutdown(g_GdiToken);
}

#define IDC_CHILD1_BTN 2000

LRESULT CALLBACK ChildWndProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hBtn;
    static bool bToggle = false;

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, timer_ID_2, 0, AniProc);

        hBtn = CreateWindow(_T("button"), _T("OK"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            200, 10, 100, 30, hWnd, (HMENU)IDC_CHILD1_BTN,
            hInst, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_CHILD1_BTN:
            bToggle = !bToggle;
            break;
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        DrawBitmapDoubleBuffering(hWnd, hdc);
        if (bToggle)
            TextOut(hdc, 200, 50, _T("Button Clicked"), 14);
        //DrawBitmap(hWnd, hdc);
        DrawRectText(hdc);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);

}

LRESULT CALLBACK ChildWndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT ptMouse;
    switch (message)
    {
    case WM_CREATE:
        //SetTimer(hWnd, timer_ID_2, 10, AniProc);
        break;
    case WM_COMMAND:
        break;
    case WM_MOUSEMOVE:
        GetCursorPos(&ptMouse);
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TCHAR str[128];
        wsprintf(str, TEXT("WORLD POSITION : (%04d, %04d)"), ptMouse.x, ptMouse.y);
        TextOut(hdc, 10, 30, str, lstrlen(str));

        ScreenToClient(hWnd, &ptMouse);
        wsprintf(str, TEXT("LOCAL POSITION : (%04d, %04d)"), ptMouse.x, ptMouse.y);
        TextOut(hdc, 10, 50, str, lstrlen(str));

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT ptMouse;
    switch (message)
    {
    case WM_CREATE:
        //SetTimer(hWnd, timer_ID_2, 10, AniProc);
        break;
    case WM_COMMAND:
        break;
    case WM_MOUSEMOVE:
        GetCursorPos(&ptMouse);
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TCHAR str[128];
        wsprintf(str, TEXT("WORLD POSITION : (%04d, %04d)"), ptMouse.x, ptMouse.y);
        TextOut(hdc, 10, 30, str, lstrlen(str));

        //ScreenToClient(hWnd, &ptMouse);
        wsprintf(str, TEXT("LOCAL POSITION : (%04d, %04d)"), ptMouse.x, ptMouse.y);
        TextOut(hdc, 10, 50, str, lstrlen(str));

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void MakeColumn(HWND hDlg)
{
    LPCTSTR name[2] = { _T("이름"), _T("전화번호") };
    LVCOLUMN lvCol = { 0, };
    HWND hList;
    hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
    lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvCol.fmt = LVCFMT_LEFT;

    for (int i = 0; i < 2; i++)
    {
        lvCol.cx = 90;
        lvCol.iSubItem = i;
        lvCol.pszText = (LPWSTR)name[i];
        SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)i, (LPARAM)&lvCol);
    }
}

void InsertData(HWND hDlg)
{
    LVITEM item;
    HWND hList;
    LPCTSTR name[20] = { _T("김철수"), _T("김영희") };
    LPCTSTR phone[20] = { _T("010-2573-8574"), _T("011-9872-5867") };
    hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);

    for (int i = 0; i < 2; i++)
    {
        item.mask = LVIF_TEXT;
        item.iItem = i;
        item.iSubItem = 0;
        item.pszText = (LPWSTR)name[i];
        ListView_InsertItem(hList, &item);
        ListView_SetItemText(hList, i, 1, (LPWSTR)phone[i]);
    }
}