// WindowsProject1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "WinAPI_Proj.h"
#include <cmath>
#include <ctime>
#include <vector>
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

void DrawCircle(HDC hdc, POINT center, int r);
void DrawStar(HDC hdc, POINT center, int r);
void DrawRectangle(HDC hdc, POINT center, int d);

#define PI 3.141592
#define degreeToRadian(degree) ((degree) * PI / 180)

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
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(204, 204, 255));
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

	HWND hWnd = CreateWindowW(szWindowClass, _T("김한결의 첫 번째 윈도우"), WS_OVERLAPPEDWINDOW,
		200, 200, 1024, 768, nullptr, nullptr, hInstance, nullptr);

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


#define timer_ID_1 11
#define timer_ID_2 22
const int circleRadius = 50;

double LengthPts(POINT pt1, POINT pt2)
{
	return (sqrt(
		(float)(pt2.x - pt1.x) * (pt2.x - pt1.x) +
		(float)(pt2.y - pt1.y) * (pt2.y - pt1.y)));
}

BOOL InCircle(POINT pt1, POINT pt2)
{
	if (LengthPts(pt1, pt2) < circleRadius) return TRUE;
	else return FALSE;
}

#include "test2.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static POINT ptCurPos;
	static POINT ptMousePos;
	static RECT rectView;

	static bool bFlag;
	static std::vector<CObject*>objects;
	static CObject* obj;


	switch (message)
	{
	case WM_CREATE:
		bFlag = false;
		GetClientRect(hWnd, &rectView);
		SetTimer(hWnd, 1, 10, NULL); // 1000 = 1초 단위
		break;

	case WM_TIMER:
	{
		for (auto e : objects)
		{
			e->Update(&rectView);
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;

	case WM_KEYDOWN: //키보드 눌렀을 때
	{
		int breakpoint = 999;
	}
	break;

	case WM_KEYUP:
	{
		int breakpoint = 999;
	}
	break;

	case WM_CHAR:
	{
		int breakpoint = 999;

		//InvalidateRgn(hWnd, NULL, TRUE); //영역 지정해줘야함, false = 일부분만 지워줌
	}
	break;
	case WM_LBUTTONDOWN:
	{
		ptMousePos.x = LOWORD(lParam);
		ptMousePos.y = HIWORD(lParam);
		obj = new CCircle(ptMousePos, 50);
		objects.push_back(obj);
		InvalidateRgn(hWnd, NULL, TRUE);
	}
	break;

	case WM_LBUTTONUP:
	{
		bFlag = TRUE;
		InvalidateRgn(hWnd, NULL, TRUE);
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

		for (auto e : objects)
		{
			e->Draw(hdc);
		}

		/*srand(time(NULL));
		int a = rand() % 3;

		if (bFlag == TRUE)
		{
			switch (a)
			{
			case 0:
				DrawRectangle(hdc, ptMousePos, 20);
				break;
			case 1:
				DrawCircle(hdc, ptMousePos, 20);
				break;
			case 2:
				DrawStar(hdc, ptMousePos, 20);
				break;
			}
		}*/

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_COMMAND: //메뉴를 선택했을 때 발생하는 이벤트
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
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
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
	int a = Width / Count;
	int b = Height / Count;

	for (int i = 0; i <= Width; i += a)
	{
		MoveToEx(hdc, center.x - Width / 2 + i, center.y - Height / 2, NULL);
		LineTo(hdc, center.x - Width / 2 + i, center.y + Height / 2);
	}
	for (int j = 0; j <= Height; j += b)
	{
		MoveToEx(hdc, center.x - Width / 2, center.y - Height / 2 + j, NULL);
		LineTo(hdc, center.x + Width / 2, center.y - Height / 2 + j);
	}
}

void DrawRectangle(HDC hdc, POINT center, int s)
{
	Rectangle(hdc, center.x - s, center.y - s, center.x + s, center.y + s);
}

void DrawCircle(HDC hdc, POINT center, int r)
{
	//if (bFlag)
		//SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
	Ellipse(hdc, center.x - r, center.y - r, center.x + r, center.y + r);
}

void DrawStar(HDC hdc, POINT center, int r)
{
	POINT point[10];
	double theta = degreeToRadian(360 / 5);

	for (int i = 0; i < 5; i++)
	{
		point[2 * i].x = r * cos(theta * i) + center.x;
		point[2 * i].y = r * sin(theta * i) + center.y;
	}

	for (int i = 0; i < 5; i++)
	{
		point[2 * i + 1].x = r / 2 * cos(theta / 2 + theta * i) + center.x;
		point[2 * i + 1].y = r / 2 * sin(theta / 2 + theta * i) + center.y;

	}

	Polygon(hdc, point, 10);
}

/*
Q2. 1. 클라이언트 영역에 마우스를 클릭, 그 위치에 원 생성
	   1.1 랜덤으로 사각형 , 원, 별이 생성되도록 한다.

	2. 임의의 방향으로 이동 / 단 클라이언트 외곽선에서 반사

	3. 클래스로 구현
		사각형 / 별도 같은 식으로 작동하도록 한다.

	4. 다른 오브젝트와의 관계 설정
	  모드 설정 1번 반사, 2번 합체, 3번 분열로 키 설정
	  4.1 반사 - 다른 오브젝트와 부딪히면 튕기기
	  4.2 합체 - 상성 관계에 따라 다른 오브젝트와 만나면 합체해서 커지게 하기
	  4.3 분열 - 상성 관계에 따라 다른 오브젝트와 만나면 분열해서 작아지게 하기
		  별 > 원 > 사각형 > 별

	5. 일정 크기 이상, 또는 이하는 사라지도록 한다.

	6.  각 오브젝트를 회전 시킨다.

		CObject
		{
		protected:
			좌표
			이동 스피드
			이동 방향
			타입 ( 원, 별, 사각형 )
		public:
			virtual void Update() = 0;
			virtual void Draw() = 0; (자기 자신을 그려주는 것)
			virtual BOOL collision() = 0;
			// : 그외 함수
		};

		CCircle : public CObject
		{
		private:
			반지름
		public:
			setradius
			void Update() override;
			void Draw() override;
			BOOL collision() override;
		}
*/