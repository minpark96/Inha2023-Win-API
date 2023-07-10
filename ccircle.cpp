#include "ccircle.h"

CCircle::CCircle()
{
    cx = 700;
    cy = 700;
    radius = 10;
    vx = 0;
    vy = -5;
}

CCircle::~CCircle()
{
}

void CCircle::Update(RECT* rectView)
{        
    // 왼쪽
    if (cx - radius <= rectView->left)
    {
        vx *= -1;
        cx = rectView->left + radius;
    }
    // 오른쪽
    if (cx + radius >= rectView->right)
    {
        vx *= -1;
        cx = rectView->right - radius;
    }
    // 위
    if (cy - radius <= rectView->top)
    {
        vy *= -1;
        cy = rectView->top + radius;
    }
    // 아래
    if (cy + radius >= rectView->bottom)
    {
        vy *= -1;
        cy = rectView->bottom - radius;
    }

    cx += vx;
    cy += vy;
}

void CCircle::Draw(HDC hdc)
{
    Ellipse(hdc, round(cx - radius), round(cy - radius), round(cx + radius), round(cy + radius));
}

double CCircle::distance(const double x, const double y) const
{
    return sqrt((cx - x) * (cx - x) + (cy - y) * (cy - y));
}

double CCircle::GetCX() const
{
    return cx;
}

double CCircle::GetCY() const
{
    return cy;
}

double CCircle::GetRadius() const
{
	return radius;
}

void CCircle::SetPosition(CCircle& other)
{
}
