#include "test2.h"
#include <cmath>
#include <ctime>
#define PI 3.141592
#define degreeToRadian(degree) ((degree) * PI / 180)


CCircle::CCircle()
{
    center.x = 0;
    center.y = 0;
    radius = 0;
    xdirection = 0;
    ydirection = 0;
}

CCircle::CCircle(POINT center, double radius)
{
    srand(time(NULL));
    this->center = center;
    this->radius = radius;
    this->xdirection = cos(degreeToRadian(rand() % 360));
    this->ydirection = sin(degreeToRadian(rand() % 360));
    this->speed = 6;
}

CCircle::~CCircle()
{

}

void CCircle::Update(RECT* rectView)
{
    // 오른쪽
    if (center.x + round(radius) >= rectView->right)
    {
        xdirection *= -1;
        center.x = rectView->right - round(radius);
    }
    // 왼쪽
    if (center.x - round(radius) <= rectView->left)
    {
        xdirection *= -1;
        center.x = rectView->left + round(radius);
    }
    // 아래
    if (center.y - round(radius) <= rectView->top)
    {
        ydirection *= -1;
        center.y = rectView->top + round(radius);
    }
    // 위
    if (center.y + round(radius) >= rectView->bottom)
    {
        ydirection *= -1;
        center.y = rectView->bottom - round(radius);
    }

    center.x += round(speed * xdirection);
    center.y += round(speed * ydirection);
}

void CCircle::Draw(HDC hdc)
{
    Ellipse(hdc, center.x - round(radius), center.y - round(radius), center.x + round(radius), center.y + round(radius));
}

BOOL CCircle::Collision()
{
    return 0;
}
