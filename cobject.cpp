#include "cobject.h"

CObject::CObject(POINT p, int type)
{
    cx = p.x;
    cy = p.y;
    double speed = rand() / MAX * 3 + 3;
    vx = (rand() / HALF - 1) * 10;
    vy = (rand() / HALF - 1) * 10;
    volume = 0;
    this->type = type;
}

CObject::~CObject()
{
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

double CObject::GetCX() const
{
    return cx;
}

double CObject::GetCY() const
{
    return cy;
}

double CObject::GetVX() const
{
    return vx;
}

double CObject::GetVY() const
{
    return vy;
}

double CObject::GetVolume() const
{
    return volume;
}

void CObject::SetCX(double v)
{
    cx = v;
}

void CObject::SetCY(double v)
{
    cy = v;
}

void CObject::SetVX(double v)
{
    vx = v;
}

void CObject::SetVY(double v)
{
    vy = v;
}
/*
===============================================================================
                                        원
===============================================================================
*/
CCircle::CCircle(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 50 + 10;
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
    case 2: // 원과 사각형 충돌
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            collision = distance(*rect) <= radius + rect->GetRadius();
        }
    }
    break;    
    case 3: // 원과 별 충돌
    {
        const CStar* star = dynamic_cast<const CStar*>(&other);
        if (star)
        {
            collision = distance(*star) <= radius + star->GetRadius();
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

void CCircle::SetRadius(double r)
{
    radius = r;
}

void CCircle::SetPosition(CObject& other)
{
    // 원B의 속도 저장
    double va = other.GetVX();
    double vb = other.GetVY();
    // 원A의 속도 임시 저장
    double tmp_vx = vx;
    double tmp_vy = vy;
    // 원A의 속도를 원B의 속도로
    vx = va;
    vy = vb;
    // 원B의 속도를 원A의 속도로
    other.SetVX(tmp_vx);
    other.SetVY(tmp_vy);

    double vnx = other.GetCX() - cx;
    double vny = other.GetCY() - cy;

    // Calculate the distance between the centers of two objects
    //double distance = sqrt(vnx * vnx + vny * vny);

    // Calculate the unit vector (ux, uy) between the centers
    //double ux = vnx / distance;
    //double uy = vny / distance;

    // Calculate the relative velocities
    //double rel_vx = vx - va;
    //double rel_vy = vy - vb;

    // Calculate the dot product of the unit vector in the direction of the collision and the relative velocities
    //double dot = rel_vx * ux + rel_vy * uy;

    // Update the velocities considering the direction using the dot product, but maintain the original speed
    //double nvx = vx - 2 * dot * ux;
    //double nvy = vy - 2 * dot * uy;
    //double navx = va + 2 * dot * ux;
    //double navy = vb + 2 * dot * uy;

    // Set the updated velocities to the CCircle object and the CObject


    //double vol = other.GetVolume();
    //double va = other.GetVX();
    //double vb = other.GetVY();
    //double vnx = other.GetCX() - cx;
    //double vny = other.GetCY() - cy;

    //// Calculate the distance between the centers of the two objects.
    //double distance = this->distance(other);

    //// Calculate the unit vector (ux, uy) between the centers.
    //double ux = vnx / distance;
    //double uy = vny / distance;

    //// Calculate the relative velocities.
    //double rel_vx = vx - va;
    //double rel_vy = vy - vb;

    //// Calculate the dot product of the unit vector in the direction of the collision and the relative velocities.
    //double dot = rel_vx * ux + rel_vy * uy;

    //// Calculate the collision impulse.
    //double impulse = 2 * vol * dot / (volume + vol);

    //// Update the velocities of the CCircle object by considering the collision impulse.
    //double nvx = vx - ux * impulse;
    //double nvy = vy - uy * impulse;

    //// Update the velocities of the CObject by considering the opposite collision impulse.
    //double navx = va + (ux * impulse * volume) / vol;
    //double navy = vb + (uy * impulse * volume) / vol;

    //vx = nvx;
    //vy = nvy;
    //other.SetVX(navx);
    //other.SetVY(navy);
}

void CCircle::Update()
{
    cx += vx;
    cy += vy;
}

void CCircle::TestUpdate(RECT* rectView)
{
    // 오른쪽
    if (cx + radius >= rectView->right)
    {
        vx *= -1;
        cx = rectView->right - radius;
    }
    // 왼쪽
    if (cx - radius <= rectView->left)
    {
        vx *= -1;
        cx = rectView->left + radius;
    }
    // 아래
    if (cy - radius <= rectView->top)
    {
        vy *= -1;
        cy = rectView->top + radius;
    }
    // 위
    if (cy + radius >= rectView->bottom)
    {
        vy *= -1;
        cy = rectView->bottom - radius;
    }

    cx += vx;
    cy += vy;
}

/*
===============================================================================
                                        별
===============================================================================
*/

CStar::CStar(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 90 + 10;
    angle = DegreeToRadian(360.0 / 5);
    volume = radius * tan(angle / 2) * radius * 5;
    rotation = rand() % 72 + 1;
    angleVelocity = (rand() / HALF - 1) / 10;
}

void CStar::Draw(HDC hdc)
{
    double x = radius / (2 * cos(angle / 2));
    POINT p;
    POINT point[10];

    for (int i = 0; i < 5; i++)
    {
        p.x = round(cx + cos(rotation + i * angle) * x);
        p.y = round(cy + sin(rotation + i * angle) * x);
        point[2 * i] = p;
        p.x = round(cx + cos(rotation + i * angle + angle / 2) * radius);
        p.y = round(cy + sin(rotation + i * angle + angle / 2) * radius);
        point[2 * i + 1] = p;
    }

    Polygon(hdc, point, 10);
}

bool CStar::CollisionBoundary(int flag, const LONG& rect1, const LONG& rect2) const
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

bool CStar::Collision(const CObject& other) const
{
    bool collision = false;

    switch (other.GetType())
    {
    case 1: // 별과 원 충돌
    {
        const CCircle* circle = dynamic_cast<const CCircle*>(&other);
        if (circle)
        {
            collision = distance(*circle) <= radius + circle->GetRadius();
        }
    }
    break;
    case 2: // 별과 사각형 충돌
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            collision = distance(*rect) <= radius + rect->GetRadius();
        }
    }
    break; 
    case 3: // 별과 별 충돌
    {
        const CStar* star = dynamic_cast<const CStar*>(&other);
        if (star)
        {
            collision = distance(*star) <= radius + star->radius;
        }
    }
    break;
    default:
        break;
    }

    return collision;
}

double CStar::GetRadius() const
{
    return radius;
}

void CStar::Update()
{
    cx += vx;
    cy += vy;
    rotation += angleVelocity;
}

void CStar::TestUpdate(RECT* rectView)
{
}

void CStar::SetPosition(CObject& other)
{
    double va = other.GetVX();
    double vb = other.GetVY();
    double vnx = other.GetCX() - cx;
    double vny = other.GetCY() - cy;

    // Calculate the distance between the centers of two objects
    double distance = sqrt(vnx * vnx + vny * vny);

    // Calculate the unit vector (ux, uy) between the centers
    double ux = vnx / distance;
    double uy = vny / distance;

    // Calculate the relative velocities
    double rel_vx = vx - va;
    double rel_vy = vy - vb;

    // Calculate the dot product of the unit vector in the direction of the collision and the relative velocities
    double dot = rel_vx * ux + rel_vy * uy;

    // Update the velocities considering the direction using the dot product, but maintain the original speed
    double nvx = vx - 2 * dot * ux;
    double nvy = vy - 2 * dot * uy;
    double navx = va + 2 * dot * ux;
    double navy = vb + 2 * dot * uy;

    // Set the updated velocities to the CCircle object and the CObject
    vx = nvx;
    vy = nvy;
    other.SetVX(navx);
    other.SetVY(navy);

    //double vol = other.GetVolume();
    //double va = other.GetVX();
    //double vb = other.GetVY();
    //double vnx = other.GetCX() - cx;
    //double vny = other.GetCY() - cy;

    //// Calculate the distance between the centers of the two objects.
    //double distance = this->distance(other);

    //// Calculate the unit vector (ux, uy) between the centers.
    //double ux = vnx / distance;
    //double uy = vny / distance;

    //// Calculate the relative velocities.
    //double rel_vx = vx - va;
    //double rel_vy = vy - vb;

    //// Calculate the dot product of the unit vector in the direction of the collision and the relative velocities.
    //double dot = rel_vx * ux + rel_vy * uy;

    //// Calculate the collision impulse.
    //double impulse = 2 * vol * dot / (volume + vol);

    //// Update the velocities of the CCircle object by considering the collision impulse.
    //double nvx = vx - ux * impulse;
    //double nvy = vy - uy * impulse;

    //// Update the velocities of the CObject by considering the opposite collision impulse.
    //double navx = va + (ux * impulse * volume) / vol;
    //double navy = vb + (uy * impulse * volume) / vol;

    //vx = nvx;
    //vy = nvy;
    //other.SetVX(navx);
    //other.SetVY(navy);
}

/*
===============================================================================
                                        사각형
===============================================================================
*/

CRectangle::CRectangle(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 90 + 10;
    angle = DegreeToRadian(360.0 / 4);
    volume = radius * radius;
    rotation = rand() % 90 + 1;
    angleVelocity = (rand() / HALF - 1) / 10;
}

void CRectangle::Draw(HDC hdc)
{
    double x = sqrt(radius * radius);
    POINT p;
    POINT point[4];

    for (int i = 0; i < 4; i++)
    {
        p.x = round(cx + cos(rotation + i * angle) * x);
        p.y = round(cy + sin(rotation + i * angle) * x);
        point[i] = p;
    }

    Polygon(hdc, point, 4);
    //Rectangle(hdc, round(cx - radius / 2), round(cy - radius / 2), round(cx + radius / 2), round(cy + radius / 2));
}

bool CRectangle::CollisionBoundary(int flag, const LONG& rect1, const LONG& rect2) const
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

bool CRectangle::Collision(const CObject& other) const
{
    bool collision = false;

    switch (other.GetType())
    {
    case 1: // 별과 원 충돌
    {
        const CCircle* circle = dynamic_cast<const CCircle*>(&other);
        if (circle)
        {
            collision = distance(*circle) <= radius + circle->GetRadius();
        }
    }
    break;
    case 2: // 별과 사각형 충돌
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            collision = distance(*rect) <= radius + rect->radius;
        }
    }
    break;
    case 3: // 별과 별 충돌
    {
        const CStar* star = dynamic_cast<const CStar*>(&other);
        if (star)
        {
            collision = distance(*star) <= radius + star->GetRadius();
        }
    }
    break;
    default:
        break;
    }

    return collision;
}

double CRectangle::GetRadius() const
{
    return radius;
}

void CRectangle::Update()
{
    cx += vx;
    cy += vy;
    rotation += angleVelocity;
}

void CRectangle::TestUpdate(RECT* rectView)
{
}

void CRectangle::SetPosition(CObject& other)
{
    double va = other.GetVX();
    double vb = other.GetVY();
    double vnx = other.GetCX() - cx;
    double vny = other.GetCY() - cy;

    // Calculate the distance between the centers of two objects
    double distance = sqrt(vnx * vnx + vny * vny);

    // Calculate the unit vector (ux, uy) between the centers
    double ux = vnx / distance;
    double uy = vny / distance;

    // Calculate the relative velocities
    double rel_vx = vx - va;
    double rel_vy = vy - vb;

    // Calculate the dot product of the unit vector in the direction of the collision and the relative velocities
    double dot = rel_vx * ux + rel_vy * uy;

    // Update the velocities considering the direction using the dot product, but maintain the original speed
    double nvx = vx - 2 * dot * ux;
    double nvy = vy - 2 * dot * uy;
    double navx = va + 2 * dot * ux;
    double navy = vb + 2 * dot * uy;

    // Set the updated velocities to the CCircle object and the CObject
    vx = nvx;
    vy = nvy;
    other.SetVX(navx);
    other.SetVY(navy);

    //double vol = other.GetVolume();
    //double va = other.GetVX();
    //double vb = other.GetVY();
    //double vnx = other.GetCX() - cx;
    //double vny = other.GetCY() - cy;

    //// Calculate the distance between the centers of the two objects.
    //double distance = this->distance(other);

    //// Calculate the unit vector (ux, uy) between the centers.
    //double ux = vnx / distance;
    //double uy = vny / distance;

    //// Calculate the relative velocities.
    //double rel_vx = vx - va;
    //double rel_vy = vy - vb;

    //// Calculate the dot product of the unit vector in the direction of the collision and the relative velocities.
    //double dot = rel_vx * ux + rel_vy * uy;

    //// Calculate the collision impulse.
    //double impulse = 2 * vol * dot / (volume + vol);

    //// Update the velocities of the CCircle object by considering the collision impulse.
    //double nvx = vx - ux * impulse;
    //double nvy = vy - uy * impulse;

    //// Update the velocities of the CObject by considering the opposite collision impulse.
    //double navx = va + (ux * impulse * volume) / vol;
    //double navy = vb + (uy * impulse * volume) / vol;

    //vx = nvx;
    //vy = nvy;
    //other.SetVX(navx);
    //other.SetVY(navy);
}
