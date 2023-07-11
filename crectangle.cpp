#include "crectangle.h"

CRectangle::CRectangle()
{
    width = 100;
    height = 20;
    cx = 700;
    cy = 750;
    left = cx - width / 2;
    right = cx + width / 2;
    top = cy - height / 2;
    bottom = cy + height / 2;
    vx = 0;
    hp = 0;
    color = RGB( 255, 255, 255 );
}

CRectangle::CRectangle(double cx, double cy, int hp)
{
    width = 100;
    height = 50;
    this->cx = cx;
    this->cy = cy;
    left = this->cx - width / 2;
    right = this->cx + width / 2;
    top = this->cy - height / 2;
    bottom = this->cy + height / 2;
    this->hp = hp;
    vx = 0;
    SetColor(hp);
}

CRectangle::~CRectangle()
{
}

void CRectangle::Update(RECT* rectView)
{
    // ¿ÞÂÊ
    if (cx - width / 2 <= rectView->left)
    {
        vx *= -1;
        cx = rectView->left + width / 2;
    }
    // ¿À¸¥ÂÊ
    if (cx + width / 2 >= rectView->right)
    {
        vx *= -1;
        cx = rectView->right - width / 2;
    }

    cx += vx;
}

void CRectangle::Draw(HDC hdc)
{
    POINT p;
    POINT point[4];

    p.x = round(right);
    p.y = round(bottom);
    point[0] = p;
    p.x = round(left);
    p.y = round(bottom);
    point[1] = p;
    p.x = round(left);
    p.y = round(top);
    point[2] = p;
    p.x = round(right);
    p.y = round(top);
    point[3] = p;

    Polygon(hdc, point, 4);
}

bool CRectangle::Collision(const CCircle& cir)
{
    double clamped_x = max(left, min(cir.GetCX(), right));
    double clamped_y = max(top, min(cir.GetCY(), bottom));

    double d = cir.distance(clamped_x, clamped_y);

    if (d <= cir.GetRadius())
    {
        hp--;
        SetColor(hp);
        return true;
    }
        
    else
        return false;
}

double CRectangle::GetWidth() const
{
	return 0.0;
}

double CRectangle::GetHeight() const
{
	return 0.0;
}

COLORREF CRectangle::GetColor() const
{
    return color;
}

void CRectangle::SetColor(int hp)
{
    switch (hp)
    {
    case 1:
    {
        color = RGB(255, 0, 0);
    }
    break;
    case 2:
    {
        color = RGB(255, 192, 203);
    }
    break;
    case 3:
    {
        color = RGB(255, 165, 0);
    }
    break;
    case 4:
    {
        color = RGB(144, 238, 144);
    }
    break;
    case 5:
    {
        color = RGB(0, 0, 255);
    }
    break;
    case 6:
    {
        color = RGB(128, 128, 128);
    }
    break;
    default:
    {
        color = RGB(255, 255, 255);
    }
    break;
    }
}
