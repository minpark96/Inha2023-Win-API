#include "cobject.h"

CObject::CObject(POINT p, int type)
{
    cx = p.x;
    cy = p.y;
    double speed = rand() / MAX * 9 + 3;
    vx = (rand() / HALF - 1) * speed;
    vy = (rand() / HALF - 1) * speed;
    volume = 0;
    this->type = type;
}

CObject::~CObject()
{
}

void CObject::Update()
{
    cx += vx;
    cy += vy;
}

double CObject::distance(const CObject& other) const
{
    return sqrt((cx - other.cx) * (cx - other.cx) + (cy - other.cy) * (cy - other.cy));
}

void CObject::SetPositionBoundary(double na, double nb)
{
        vx *= na;
        vy *= nb;
}

int CObject::GetType() const
{
    return type;
}

double CObject::GetVolume() const
{
    return volume;
}

double CObject::GetVX() const
{
    return vx;
}

double CObject::GetVY() const
{
    return vy;
}

CCircle::CCircle(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 90 + 10;
    volume = PI * radius * radius;
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

void CCircle::SetPositionCollsion(CObject& other)
{
    float vX1, vY1, vX2, vY2, m1, m2;
    double vol = other.GetVolume();
    double va = other.GetVX();
    double vb = other.GetVY();

    // 충돌 전의 운동량 벡터 계산
    double p1 = volume * sqrt(vx * vx + vy * vy);
    double p2 = vol * sqrt(va * va + vb * vb);
    double sumP = p1 + p2;
    double theta1 = atan2(vy, vx);
    double theta2 = atan2(vb, va);
    double phi = atan2(vb - vy, va - vx);
    vx = ((p1 * cos(theta1 - phi) * (volume - vol) + 2 * vol * p2 * cos(theta2 - phi)) / sumP) * cos(phi) + p1 * sin(theta1 - phi) * cos(phi + PI / 2);
    vy = ((p1 * cos(theta1 - phi) * (m1 - m2) + 2 * m2 * p2 * cos(theta2 - phi)) / sumP) * sin(phi) + p1 * sin(theta1 - phi) * sin(phi + PI / 2);
    other.vx = ((p2 * cos(theta2 - phi) * (m2 - m1) + 2 * m1 * p1 * cos(theta1 - phi)) / sumP) * cos(phi) + p2 * sin(theta2 - phi) * cos(phi + PI / 2);
    float new_vy2 = ((p2 * cos(theta2 - phi) * (m2 - m1) + 2 * m1 * p1 * cos(theta1 - phi)) / sumP) * sin(phi) + p2 * sin(theta2 - phi) * sin(phi + PI / 2);
}

CStar::CStar(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 90 + 10;
    angle = DegreeToRadian(360.0 / 5);
    volume = radius * tan(angle / 2) * radius * 5;
}

void CStar::Draw(HDC hdc)
{

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

void CStar::SetPositionCollsion(const CObject& other) const
{
}

CRectangle::CRectangle(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 90 + 10;
    volume = radius * radius;
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

void CRectangle::SetPositionCollsion(const CObject& other) const
{
}


