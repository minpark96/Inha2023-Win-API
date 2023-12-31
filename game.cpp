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
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJ));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIPROJ);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
    enum mode { REFLECTION, COMBINATION, DECOMPOSITION };
    static int seletedMode = REFLECTION;
    enum type { NONE, CIRCLE, RECTANGLE, STAR };

    switch (message)
    {
    case WM_CREATE:
        srand(time(NULL));
        GetClientRect(hWnd, &rectView);
        SetTimer(hWnd, timer_ID_1, 20, NULL);
        break;
    case WM_TIMER:
        if (wParam == timer_ID_1)
        {
            switch (seletedMode)
            {
            case REFLECTION:
            {
                for (auto it1 = objs.begin(); it1 != objs.end(); ++it1)
                {
                    auto it2 = it1;
                    for (++it2; it2 != objs.end(); ++it2)
                    {
                        if ((*it1)->Collision(**it2))
                        {
                            (*it1)->SetPosition(**it2);
                            do
                            {
                                (*it1)->Update(&rectView);
                                (*it2)->Update(&rectView);
                            } while ((*it1)->Collision(**it2));
                        }
                    }

                    (*it1)->Update(&rectView);
                }
            }
            break;
            case COMBINATION:
            {
                auto it1 = objs.begin();

                while (it1 != objs.end())
                {
                    bool it1Erased = false;
                    auto it2 = it1;
                    ++it2;
                    while (it2 != objs.end())
                    {
                        if ((*it1)->Collision(**it2))
                        {
                            if ((*it1)->GetType() == (*it2)->GetType())
                            {
                                if ((*it1)->Combination(**it2) == 0)
                                {
                                    objs.erase(it2);
                                    it1 = objs.erase(it1);
                                    it1Erased = true;
                                    break;
                                }
                                else if ((*it1)->Combination(**it2) == 1)
                                {
                                    it1 = objs.erase(it1);
                                    it1Erased = true;
                                    break;
                                }
                                else if ((*it1)->Combination(**it2) == 2)
                                {
                                    it2 = objs.erase(it2);
                                    continue;
                                }
                            }
                            else
                            {
                                (*it1)->SetPosition(**it2);
                                do
                                {
                                    (*it1)->Update(&rectView);
                                    (*it2)->Update(&rectView);
                                } while ((*it1)->Collision(**it2));
                            }
                        }

                        ++it2;
                    }

                    (*it1)->Update(&rectView);

                    if (!it1Erased)
                    {
                        ++it1;
                    }
                }
            }
            break;
            case DECOMPOSITION:
            {
                /*
                0 : it1이 사라짐 -> it2는 직진
                1 : 원 it1이 분열 -> it2는 반사
                2 : 사각형 it1이 분열 -> it2는 반사
                3 : 별 it1이 분열 -> it2는 반사
                4 : it2이 사라짐 -> it1는 직진
                5 : 원 it2이 분열 -> it1는 반사
                6 : 사각형 it2이 분열 -> it1는 반사
                7 : 별 it2이 분열 -> it1는 반사
                */

                auto it1 = objs.begin();

                while (it1 != objs.end())
                {
                    bool it1Erased = false;
                    auto it2 = it1;
                    ++it2;
                    while (it2 != objs.end())
                    {
                        if ((*it1)->Collision(**it2))
                        {
                            (*it1)->SetPosition(**it2);
                            do
                            {
                                (*it1)->Update(&rectView);
                                (*it2)->Update(&rectView);
                            } while ((*it1)->Collision(**it2));

                            if ((*it1)->GetType() != (*it2)->GetType())
                            {
                                if ((*it1)->Decomposition(**it2) == 0)
                                {
                                    it1 = objs.erase(it1);
                                    it1Erased = true;
                                    break;
                                }
                                else if ((*it1)->Decomposition(**it2) == 1)
                                {
                                    // it1 3개 생성
                                    CCircle* circle = dynamic_cast<CCircle*>(*it1);
                                    double r = circle->GetRadius();
                                    double cx = circle->GetCX();
                                    double cy = circle->GetCY();
                                    double vx = circle->GetVX();
                                    double vy = circle->GetVY();
                                    POINT p = { cx + r, cy };
                                    objs.push_back(new CCircle(p, CIRCLE, r, -vx, vy));
                                    p.y = cy + r;
                                    objs.push_back(new CCircle(p, CIRCLE, r, vx, -vy));
                                    p.x = cx;
                                    objs.push_back(new CCircle(p, CIRCLE, r, -vx, -vy));
                                }
                                else if ((*it1)->Decomposition(**it2) == 2)
                                {
                                    // it1 3개 생성
                                    CRectangle* rect = dynamic_cast<CRectangle*>(*it1);
                                    double r = rect->GetRadius();
                                    double cx = rect->GetCX();
                                    double cy = rect->GetCY();
                                    double vx = rect->GetVX();
                                    double vy = rect->GetVY();
                                    POINT p = { cx + r, cy };
                                    objs.push_back(new CRectangle(p, RECTANGLE, r, -vx, vy));
                                    p.y = cy + r;
                                    objs.push_back(new CRectangle(p, RECTANGLE, r, vx, -vy));
                                    p.x = cx;
                                    objs.push_back(new CRectangle(p, RECTANGLE, r, -vx, -vy));
                                    break;
                                }
                                else if ((*it1)->Decomposition(**it2) == 3)
                                {
                                    // it1 3개 생성
                                    CStar* star = dynamic_cast<CStar*>(*it1);
                                    double r = star->GetRadius();
                                    double cx = star->GetCX();
                                    double cy = star->GetCY();
                                    double vx = star->GetVX();
                                    double vy = star->GetVY();
                                    POINT p = { cx + r, cy };
                                    objs.push_back(new CStar(p, STAR, r, -vx, vy));
                                    p.y = cy + r;
                                    objs.push_back(new CStar(p, STAR, r, vx, -vy));
                                    p.x = cx;
                                    objs.push_back(new CStar(p, STAR, r, -vx, -vy));
                                    break;
                                }
                                else if ((*it1)->Decomposition(**it2) == 4)
                                {
                                    it2 = objs.erase(it2);
                                    continue;
                                }
                                else if ((*it1)->Decomposition(**it2) == 5)
                                {
                                    // it2 3개 생성
                                    CCircle* circle = dynamic_cast<CCircle*>(*it2);
                                    double r = circle->GetRadius();
                                    double cx = circle->GetCX();
                                    double cy = circle->GetCY();
                                    double vx = circle->GetVX();
                                    double vy = circle->GetVY();
                                    POINT p = { cx + r, cy };
                                    objs.push_back(new CCircle(p, CIRCLE, r, -vx, vy));
                                    p.y = cy + r;
                                    objs.push_back(new CCircle(p, CIRCLE, r, vx, -vy));
                                    p.x = cx;
                                    objs.push_back(new CCircle(p, CIRCLE, r, -vx, -vy));
                                    break;
                                }
                                else if ((*it1)->Decomposition(**it2) == 6)
                                {
                                    // it2 3개 생성
                                    CRectangle* rect = dynamic_cast<CRectangle*>(*it2);
                                    double r = rect->GetRadius();
                                    double cx = rect->GetCX();
                                    double cy = rect->GetCY();
                                    double vx = rect->GetVX();
                                    double vy = rect->GetVY();
                                    POINT p = { cx + r, cy };
                                    objs.push_back(new CRectangle(p, RECTANGLE, r, -vx, vy));
                                    p.y = cy + r;
                                    objs.push_back(new CRectangle(p, RECTANGLE, r, vx, -vy));
                                    p.x = cx;
                                    objs.push_back(new CRectangle(p, RECTANGLE, r, -vx, -vy));
                                    break;
                                }
                                else if ((*it1)->Decomposition(**it2) == 7)
                                {
                                    // it2 3개 생성
                                    CStar* star = dynamic_cast<CStar*>(*it2);
                                    double r = star->GetRadius();
                                    double cx = star->GetCX();
                                    double cy = star->GetCY();
                                    double vx = star->GetVX();
                                    double vy = star->GetVY();
                                    POINT p = { cx + r, cy };
                                    objs.push_back(new CStar(p, STAR, r, -vx, vy));
                                    p.y = cy + r;
                                    objs.push_back(new CStar(p, STAR, r, vx, -vy));
                                    p.x = cx;
                                    objs.push_back(new CStar(p, STAR, r, -vx, -vy));
                                    break;
                                }
                            }
                        }

                        ++it2;
                    }

                    (*it1)->Update(&rectView);

                    if (!it1Erased)
                    {
                        ++it1;
                    }
                }
            }
            break;
            }

            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;
    case WM_LBUTTONDOWN:
    {
        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);

        int type = rand() % 3 + 1;
        switch (type)
        {
        case CIRCLE:
            objs.push_back(new CCircle(ptMousePos, CIRCLE));
            break;
        case RECTANGLE:
            objs.push_back(new CRectangle(ptMousePos, RECTANGLE));
            break;
        case STAR:
            objs.push_back(new CStar(ptMousePos, STAR));
            break;
        default:
            break;
        }

        InvalidateRgn(hWnd, NULL, TRUE);
    }
    break;
    case WM_KEYDOWN:
    {
        if (wParam == '1')
        {
            seletedMode = REFLECTION;
        }
        else if (wParam == '2')
        {
            seletedMode = COMBINATION;
        }
        else if (wParam == '3')
        {
            seletedMode = DECOMPOSITION;
        }
    }
    break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        for (CObject* obj : objs)
        {
            obj->Draw(hdc);
        }

        EndPaint(hWnd, &ps);
    }
    break;


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
        KillTimer(hWnd, timer_ID_1);
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

