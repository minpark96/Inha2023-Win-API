// WinAPI_Proj.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI_Proj.h"

#define MAX_LOADSTRING 100
#define PI 3.141592653589793
#define DegreeToRadian(degree) ((degree) * PI / 180)

void DrawGrid(HDC hdc, POINT center, int Width, int Height, int Count);
void DrawCircle(HDC hdc, POINT center, int radius);
void DrawSunFlower(HDC hdc, POINT c0, int r0, int x);
void DrawStar(HDC hdc, POINT c0, int r0);

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

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

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

    /*static TCHAR str[100] = L"초기값";
    static list<TCHAR*> li;
    static int count, yPos;
    SIZE size;*/

    switch (message)
    {
    //case WM_CREATE:
    //    /*count = 0;
    //    yPos = 200;
    //    CreateCaret(hWnd, NULL, 5, 15);
    //    ShowCaret(hWnd);*/
    //    break;
    //case WM_KEYDOWN:
    //    {
    //        int breakpoint = 999;
    //    }
    //    break;
    //case WM_KEYUP:
    //    {
    //        int breakpoint = 999;
    //    }
    //    break;
    //case WM_CHAR:
    //    {
    //        int breakpoint = 999;
    //        if (wParam == VK_BACK && count > 0)
    //        {
    //            count--;
    //        }
    //        else if (wParam == VK_RETURN)
    //        {
    //            TCHAR* temp = new TCHAR[_tcslen(str) + 1];
    //            wcscpy_s(temp, _tcslen(str) + 1, str);

    //           
    //            if (li.size() == 10)
    //            {
    //                delete[] li.front();
    //                li.pop_front();
    //            }

    //            li.push_back(temp);
    //            count = 0;
    //        }
    //        else
    //        {
    //            str[count++] = wParam;
    //        }
    //        str[count] = NULL;
    //        InvalidateRect(hWnd, NULL, TRUE);
    //    }
    //    break;

    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            HPEN hPen, oldPen;
            hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 255));
            oldPen = (HPEN)SelectObject(hdc, hPen);
            HBRUSH hBrush, oldBrush;
            hBrush = CreateSolidBrush(RGB(0, 255, 255));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            POINT c0 = { 300, 300 };
            int r0 = 100;
            DrawStar(hdc, c0, r0);

            SelectObject(hdc, oldPen);
            DeleteObject(hPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(oldBrush);

            EndPaint(hWnd, &ps);
        }
        break;
        //DrawGrid(hdc, center, 400, 300, 6);

/*GetTextExtentPoint(hdc, str, _tcslen(str), &size);
TextOut(hdc, 100, yPos, str, _tcslen(str));
list<TCHAR*>::iterator iter = li.begin();
if (!li.empty())
{
    for (int i = 0; i < 10; i++, iter++)
    {
        if (iter == li.end())
            break;
        TextOut(hdc, 100, yPos - 200 + i * 20, *iter, _tcslen(*iter));
    }
}
SetCaretPos(100 + size.cx, 0 + yPos);*/
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
    case WM_DESTROY:
        //HideCaret(hWnd);
        //DestroyCaret();

        ///* list 개체 동적배열 해제 및 리스트 삭제 */
        //
        //for (auto de : li)
        //    delete[] de;
        //li.clear();

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

void DrawGrid(HDC hdc, POINT center, int Width, int Height, int Count)
{
    double gapX, gapY;

    if (Count != 0)
    {
        gapX = Width / (double)Count;
        gapY = Height / (double)Count;
    }
    else
    {
        Count++;
        gapX = Width;
        gapY = Height;
    }

    for (int i = 0; i < Count + 1; i++)
    {
        MoveToEx(hdc, center.x - Width / 2 + gapX * i, center.y + Height / 2, NULL);
        LineTo(hdc, center.x - Width / 2 + gapX * i, center.y - Height / 2);
        MoveToEx(hdc, center.x - Width / 2, center.y - Height / 2 + gapY * i, NULL);
        LineTo(hdc, center.x + Width / 2, center.y - Height / 2 + gapY * i);
    }
}

void DrawCircle(HDC hdc, POINT c, int r)
{
    Ellipse(hdc, c.x - r, c.y - r, c.x + r, c.y + r);
}

void DrawSunFlower(HDC hdc, POINT c0, int r0, int x)
{
    POINT c1;
    double angle = DegreeToRadian(360.0 / x);
    double r1 = (r0 * sin(angle / 2)) / (1 - sin(angle / 2));

    for (int i = 0; i < x; i++)
    {
        c1.x = c0.x + cos(i * angle) * (r0 + r1);
        c1.y = c0.y + sin(i * angle) * (r0 + r1);
        DrawCircle(hdc, c1, r1);
    }
}

void DrawStar(HDC hdc, POINT c0, int r0)
{
    double angle = DegreeToRadian(360.0 / 5);
    double x = r0 / (2 * cos(angle / 2));
    POINT p;
    POINT point[10];

    for (int i = 0; i < 5; i++)
    {
        p.x = c0.x + cos(i * angle) * x;
        p.y = c0.y + sin(i * angle) * x;
        point[2 * i] = p;
        p.x = c0.x + cos(i * angle + angle / 2) * r0;
        p.y = c0.y + sin(i * angle + angle / 2) * r0;
        point[2 * i + 1] = p;
    }

    Polygon(hdc, point, 10);
}