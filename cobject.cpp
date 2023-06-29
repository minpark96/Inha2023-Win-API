#include "cobject.h"

CObject::CObject(POINT p)
{
    cx = p.x;
    cy = p.y;
    speed = rand() / MAX * 9 + 3;
    va = rand() / HALF - 1;
    vb = rand() / HALF - 1;
}

CObject::~CObject()
{
}

void CObject::Update()
{
    cx += va * speed;
    cy += vb * speed;
}

CCircle::CCircle(POINT p) : CObject(p)
{
    radius = rand() % 90 + 10;
}

void CCircle::Draw(HDC hdc)
{
    Ellipse(hdc, round(cx - radius), round(cy - radius), round(cx + radius), round(cy + radius));
}

bool CCircle::Collision()
{
	return false;
}

CStar::CStar(POINT p) : CObject(p)
{
    radius = rand() % 90 + 10;
}

void CStar::Draw(HDC hdc)
{
    double angle = DegreeToRadian(360.0 / 5);
    double x = radius / (2 * cos(angle / 2));
    POINT p;
    POINT point[10];

    for (int i = 0; i < 5; i++)
    {
        p.x = round(cx + cos(i * angle) * x);
        p.y = round(cy + sin(i * angle) * x);
        point[2 * i] = p;
        p.x = round(cx + cos(i * angle + angle / 2) * radius);
        p.y = round(cy + sin(i * angle + angle / 2) * radius);
        point[2 * i + 1] = p;
    }

    Polygon(hdc, point, 10);
}

bool CStar::Collision()
{
	return false;
}

CRectangle::CRectangle(POINT p) : CObject(p)
{
    width = height = rand() % 90 + 10;
}

void CRectangle::Draw(HDC hdc)
{
    Rectangle(hdc, round(cx - width / 2), round(cy - height / 2), round(cx + width / 2), round(cy + height / 2));
}

bool CRectangle::Collision()
{
	return false;
}


