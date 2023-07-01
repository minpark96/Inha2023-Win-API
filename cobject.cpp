#include "cobject.h"

CObject::CObject(POINT p, int type)
{
    cx = p.x;
    cy = p.y;
    speed = rand() / MAX * 9 + 3;
    va = rand() / HALF - 1;
    vb = rand() / HALF - 1;
    this->type = type;
}

CObject::~CObject()
{
}

void CObject::Update()
{
    cx += va * speed;
    cy += vb * speed;
}

double CObject::distance(const CObject& other) const
{
    return sqrt((cx - other.cx) * (cx - other.cx) + (cy - other.cy) * (cy - other.cy));
}

void CObject::SetPosition(double na, double nb)
{
        va *= na;

        vb *= nb;
}

int CObject::GetType() const
{
    return type;
}

CCircle::CCircle(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 90 + 10;
}

void CCircle::Draw(HDC hdc)
{
    Ellipse(hdc, round(cx - radius), round(cy - radius), round(cx + radius), round(cy + radius));
}

bool CCircle::CollisionBoundary(int flag, const LONG& rect1, const LONG& rect2) const
{
    if (flag) // 좌, 우
    {
        if (cx - radius <= rect1 ||
            cx + radius >= rect2)
        {
            return true;
        }
    }
    else // 상, 하
    {
        if (cy - radius <= rect1 ||
            cy + radius >= rect2)
        {
            return true;
        }
    }

    return false;
}

bool CCircle::Collision(const CObject& other) const
{
    bool collision = false;

    switch (other.GetType())
    {
    case 1: // 원과 원 충돌
    {
        const CCircle* circle = dynamic_cast<const CCircle*>(&other);
        if (circle)
        {
            collision = distance(*circle) <= radius + circle->radius;
        }
    }
    break;
    case 2: // 원과 별 충돌
    {
        const CStar* star = dynamic_cast<const CStar*>(&other);
        if (star)
        {
            collision = distance(*star) <= radius + star->GetRadius();
        }
    }
    break;
    case 3: // 원과 사각형 충돌
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            collision = distance(*rect) <= radius + rect->GetRadius();
        }
    }
    break;
    default:
        break;
    }

    return collision;
}

double CCircle::GetRadius() const
{
    return radius;
}

CStar::CStar(POINT p, int type) : CObject(p, type)
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

bool CStar::CollisionBoundary(int flag, const LONG& rect1, const LONG& rect2) const
{
    return false;
}

bool CStar::Collision(const CObject& other) const
{
    return false;
}

double CStar::GetRadius() const
{
    return radius;
}

CRectangle::CRectangle(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 90 + 10;
}

void CRectangle::Draw(HDC hdc)
{
    Rectangle(hdc, round(cx - radius / 2), round(cy - radius / 2), round(cx + radius / 2), round(cy + radius / 2));
}

bool CRectangle::CollisionBoundary(int flag, const LONG& rect1, const LONG& rect2) const
{
    return false;
}

bool CRectangle::Collision(const CObject& other) const
{
    return false;
}

double CRectangle::GetRadius() const
{
    return radius;
}


