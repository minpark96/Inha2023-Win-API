// WinAPI_Proj.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI_Proj.h"

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

#define timer_ID_1 1

void OutFromFile(TCHAR filename[], HWND hwnd)
{
    FILE* fPtr;
    HDC hdc;
    int line;
    TCHAR buffer[500];
    line = 0;
    hdc = GetDC(hwnd);
#ifndef _UNICODE
    _tfopen_s(&fPtr, filename, _T("r, ccs = UNICODE"));
#else
    _tfopen_s(&fPtr, filename, _T("r"));
#endif
    while (_fgetts(buffer, 100, fPtr) != NULL)
    {
        if (buffer[_tcslen(buffer) - 1] == _T('\n'))
            buffer[_tcslen(buffer) - 1] = NULL;
        TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
        line++;
    }
    fclose(fPtr);
    ReleaseDC(hwnd, hdc);
}

void OutToFile(TCHAR filename[], std::list<TCHAR*> li)
{
    FILE* fPtr;
    TCHAR buffer[500];
#ifndef _UNICODE
    _tfopen_s(&fPtr, filename, _T("r, ccs = UNICODE"));
#else
    _tfopen_s(&fPtr, filename, _T("r"));
#endif
    if (!li.empty())
    {
        for (auto it = li.begin(); it != li.end(); it++)
        {
            _fputts(*it, fPtr);
        }

    }

    fclose(fPtr);
}

//double LengthPts(POINT pt1, POINT pt2)
//{
//    return (sqrt((float)(pt2.x - pt1.x) * (pt2.x - pt1.x) + (float)(pt2.y - pt1.y) * (pt2.y - pt1.y)));
//}
//
//BOOL InCirCle(POINT pt1, POINT pt2)
//{
//    if (LengthPts(pt1, pt2) < circleRadius) return TRUE;
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

    static list<CObject*> objs;
    static POINT ptMousePos;
    static RECT rectView;
    static bool bFlag = false;
    enum type {NONE, CIRCLE, RECTANGLE, STAR};
    static int selectedMenu = NONE;

    /* 채팅창 */
    static TCHAR str[100] = L"초기값";
    static list<TCHAR*> li;
    static int count, yPos;
    SIZE size;

    TCHAR filter[] = _T("Every file(*.*) \0*.*\0Text file\0*.txt;*.doc\0");
    TCHAR lpstrFile[100] = _T("");

    switch (message)
    {
    case WM_CREATE:
        srand(time(NULL));
        GetClientRect(hWnd, &rectView);
        SetTimer(hWnd, timer_ID_1, 10, NULL);
        count = 0;
        yPos = 200;
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;
    case WM_TIMER:
        if (wParam == timer_ID_1)
        {
            if (!objs.empty())
            {
                for (auto it1 = objs.begin(); it1 != objs.end(); ++it1)
                {
                    auto it2 = it1;
                    ++it2;
                    for (; it2 != objs.end(); ++it2)
                    {
                        if ((*it1)->Collision(**it2))
                        {
                            (*it1)->SetPosition(**it2);
                            do
                            {
                                (*it1)->Update();
                                (*it2)->Update();
                            } while ((*it1)->Collision(**it2));
                        }
                    }

                    if ((*it1)->CollisionBoundary(1, rectView.left, rectView.right))
                    {
                        (*it1)->SetPositionBoundary(-1, 1);
                        do
                        {
                            (*it1)->Update();
                        } while ((*it1)->CollisionBoundary(1, rectView.left, rectView.right));
                    }
                    
                    if ((*it1)->CollisionBoundary(0, rectView.top, rectView.bottom))
                    {
                        (*it1)->SetPositionBoundary(1, -1);
                        do
                        {
                            (*it1)->Update();
                        } while ((*it1)->CollisionBoundary(0, rectView.top, rectView.bottom));
                    }

                    (*it1)->Update();
                }
                InvalidateRgn(hWnd, NULL, TRUE);
            }
        }
        break;
    /*case WM_KEYDOWN:
        {
            bFlag = true;
            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;
    case WM_KEYUP:
        {
            bFlag = false;
            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;*/
    case WM_CHAR:
    {
        if (wParam == VK_BACK && count > 0)
        {
            count--;
        }
        else if (wParam == VK_RETURN)
        {
            TCHAR* temp = new TCHAR[_tcslen(str) + 1];
            wcscpy_s(temp, _tcslen(str) + 1, str);

            if (li.size() == 10)
            {
                delete[] li.front();
                li.pop_front();
            }

            li.push_back(temp);
            count = 0;
        }
        else
        {
            str[count++] = wParam;
        }
        str[count] = NULL;
        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);
        switch (selectedMenu)
        {
        case 1:
            objs.push_back(new CCircle(ptMousePos, 1));
            break;
        case 2:
            objs.push_back(new CRectangle(ptMousePos, 2));
            break;
        case 3:
            objs.push_back(new CStar(ptMousePos, 3));
            break;
        default:
            break;
        }
        InvalidateRgn(hWnd, NULL, TRUE);
    }
    break;
    /*case WM_LBUTTONUP:
    {
        bFlag = FALSE;
        InvalidateRgn(hWnd, NULL, TRUE);
    }
    break;

    case WM_MOUSEMOVE:
        if (bFlag)
        {
            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;*/
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        if (!objs.empty())
        {
            for (CObject* obj : objs)
            {
                obj->Draw(hdc);
            }
        }

        GetTextExtentPoint(hdc, str, _tcslen(str), &size);
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

        SetCaretPos(100 + size.cx, 0 + yPos);

            EndPaint(hWnd, &ps);
        }
        break;


    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_DRAW_CIRCLE:
                // : 원 그리기
            {
                int temp = selectedMenu;

                int ans = MessageBox(hWnd, _T("원 그릴래?"), _T("도형 선택"), MB_YESNOCANCEL);
                if (ans == IDYES)
                {
                    selectedMenu = CIRCLE;
                }
                else if (ans == IDNO)
                {
                    selectedMenu = NONE;
                }
                else
                {
                    selectedMenu = temp;
                }
            }
                break;
            case ID_DRAW_RECT:
                // : 사각형 그리기
                selectedMenu = RECTANGLE;
                break;
            case ID_DRAW_STAR:
                // : 별 그리기
                selectedMenu = STAR;
                break;
            case ID_FILEOPEN:
            {
                OPENFILENAME ofn;
                memset(&ofn, 0, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = filter;
                ofn.lpstrFile = lpstrFile;
                ofn.nMaxFile = 100;
                ofn.lpstrInitialDir = _T(".");
                if (GetOpenFileName(&ofn) != 0)
                {
                    TCHAR str[100];
                    _stprintf_s(str, _T("%s 파일을 열겠습니까?"), ofn.lpstrFile);
                    MessageBox(hWnd, str, _T("파일 선택"), MB_OK);
                    OutFromFile(ofn.lpstrFile, hWnd);
                }
            }
                break;
            case ID_FILESAVE:
            {
                OPENFILENAME sfn;
                memset(&sfn, 0, sizeof(OPENFILENAME));
                sfn.lStructSize = sizeof(OPENFILENAME);
                sfn.hwndOwner = hWnd;
                sfn.lpstrFilter = filter;
                sfn.lpstrFile = lpstrFile;
                sfn.nMaxFile = 256;
                sfn.lpstrInitialDir = _T(".");
                if (GetSaveFileName(&sfn) != 0)
                {
                    TCHAR str[100];
                    _stprintf_s(str, _T("%s 파일로 저장하겠습니까?"), sfn.lpstrFile);
                    MessageBox(hWnd, str, _T("저장하기 선택"), MB_OK);
                    OutToFile(sfn.lpstrFile, li);
                }
            }
            break;
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
        HideCaret(hWnd);
        DestroyCaret();

        /* list 개체 동적배열 해제 및 리스트 삭제*/
        for (auto de : li)
            delete[] de;
        li.clear();

        KillTimer(hWnd, timer_ID_1);
        PostQuitMessage(0);

        if (!objs.empty())
        {
            for (CObject* obj : objs)
            {
                delete obj;
            }
        }

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

