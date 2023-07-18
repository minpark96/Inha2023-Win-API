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
    radius = rand() % 20 + 10;
    volume = PI * radius * radius;
}

CCircle::CCircle(POINT p, int type, double radius, double vx, double vy) : CObject(p, type)
{
    this->radius = radius;
    volume = PI * radius * radius;
    this->vx = vx;
    this->vy = vy;
}

void CCircle::Update(RECT* rectView)
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

bool CCircle::Collision(const CObject& other) const
{
    switch (other.GetType())
    {
    case 1: // 원과 원 충돌
    {
        const CCircle* circle = dynamic_cast<const CCircle*>(&other);
        if (circle)
        {
            return distance(*circle) <= radius + circle->radius;
        }
    }
    break;
    case 2: // 원과 사각형 충돌
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            return distance(*rect) <= radius + rect->GetRadius();
        }
    }
    break;    
    case 3: // 원과 별 충돌
    {
        const CStar* star = dynamic_cast<const CStar*>(&other);
        if (star)
        {
            return distance(*star) <= radius + star->GetRadius();
        }
    }
    break;
    default:
        break;
    }

    return false;
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
    ///* #1 단순 방향만 변경 (충돌 지점 고려하지 않아 정확하지 않음) */

    //// 원B의 속도 저장
    //double va = other.GetVX();
    //double vb = other.GetVY();
    //
    //// 원A의 속도 임시 저장
    //double tmp_vx = vx;
    //double tmp_vy = vy;
    //
    //// 원A의 속도를 원B의 속도로
    //vx = va;
    //vy = vb;
    //
    //// 원B의 속도를 원A의 속도로
    //other.SetVX(tmp_vx);
    //other.SetVY(tmp_vy);


    ////*============================================================================================
    //

    ///* #2 충돌 지점 고려 하여 방향만 변경 (속도 변화 없음) */

    //// 원B의 속도 저장
    //double vx_B = other.GetVX();
    //double vy_B = other.GetVY();


    //// 두 원의 중심 사이 벡터 (충돌 방향 벡터)
    //double nx = other.GetCX() - cx;
    //double ny = other.GetCY() - cy;


    //// 두 원의 중심의 거리
    //double d = distance(other);
    //
    ////*============================================================================================
    ////* 거리 계산 공식: 두 점 사이의 거리를 계산하기 위해 유클리드 거리 계산 공식을 사용합니다.
    ////* distance = sqrt((x2 - x1)^2 + (y2 - y1)^2)
    ////*============================================================================================


    //// 중심 사이 벡터의 단위 벡터
    //double unx = nx / d;
    //double uny = ny / d;

    ////*============================================================================================
    ////* 단위 벡터 계산 공식: 두 점 사이의 단위 벡터는 거리로 나눈 값입니다.
    ////* ux = (x2 - x1) / distance
    ////* uy = (y2 - y1) / distance
    ////*============================================================================================


    //// 상대 속도 벡터
    //double rel_vx = vx - vx_B;
    //double rel_vy = vy - vy_B;

    ////*============================================================================================
    ////* 상대 속도를 사용하는 이유는 충돌 시 발생하는 속도 및 방향 변화를 구하기 위해서입니다.
    ////* 상대 속도는 한 객체의 속도와 다른 객체의 속도 사이의 차이를 나타냅니다.
    ////* 이를 사용하면 두 객체 간의 상호 작용이 어떻게 충돌 시 발생하는 속도 변화에 영향을 주는지 계산할 수 있습니다. 
    ////* 상대 속도를 사용하면 충돌에서 발생한 각 객체의 속도 변화를 더 정확하게 예측할 수 있습니다. 
    ////* 이를 통해 객체가 서로 상호 작용하는 방식에 대한 명확한 이해와 충돌에서 일어나는 속도 및 방향 변화를 얻을 수 있습니다.
    ////* 또한, 상대 속도를 사용하면 충돌 기준으로 얼마나 다른 각 객체의 속도를 수정해야 하는지 얻을 수 있습니다.
    ////* 이렇게 하면 충돌하는 두 객체 사이의 원래 거리를 유지하면서 움직임을 재현할 수 있습니다.
    ////*============================================================================================
    //

    //// 상대 속도 벡터와 중심 사이 단위 벡터 내적 (충돌 방향의 정사영 계산)
    //double dot = rel_vx * unx + rel_vy * uny;
    //
    ////*============================================================================================
    ////* 벡터의 내적 공식: 벡터 a와 벡터 b의 내적은 각 성분을 곱한 값의 합입니다.
    ////* dot_product = a.x * b.x + a.y * b.y
    ////*
    ////* 충돌 방향(단위 벡터)과 상대 속도 벡터의 내적(dot product)을 계산하는 이유는 
    ////* 충돌 시 발생하는 속도 변화를 구하기 위해서입니다.
    ////* 내적을 사용하면 두 벡터 사이의 각도에 따른 영향을 고려할 수 있기 때문에, 
    ////* 충돌 시 발생하는 방향 변화를 더 정확하게 계산할 수 있습니다.
    ////* 내적의 값은 두 벡터의 길이와 두 벡터 사이의 각도(cosine)를 곱한 값입니다.
    ////* 즉, 내적은 두 벡터 사이의 각도 정보를 가지고 있습니다.
    ////* 충돌 시에는 속도 벡터의 변화가 두 객체 사이의 각도에 따라서 달라집니다. 
    ////* 상대 속도 벡터와 충돌 방향의 단위 벡터의 내적을 통해 충돌 방향과 관련된 속도 변화를 얻을 수 있습니다. 
    ////* 
    ////* 이 계산 결과는 두 벡터가 얼마나 유사한 방향을 가지고 있는지를 나타내는 값입니다.
    ////* 두 벡터가 같은 방향일 경우, 내적의 값은 두 벡터의 길이의 곱만큼 크게 됩니다.
    ////* 반면, 두 벡터가 수직일 경우 내적의 값은 0이 됩니다.
    ////* 이 경우는 공식을 적용해보면 코사인 90도 값이기 때문입니다.
    ////* 따라서 이 코드에서 dot_product = (rel_vx * unx + rel_vy * uny)의 계산 결과가 0에 가까울수록
    ////* 두 객체의 속도 벡터가 충돌 방향과 수직에 가까워지므로 충돌 방향에 대한 영향이 적어집니다.
    ////* 반면, 내적의 값이 크면 큰 방향 수정이 필요합니다.
    ////* 내적의 값이 양수일 경우, 두 벡터가 같은 방향을 가지고 있고, 따라서 충돌 방향에 따라 속도 변화가 크게 일어납니다.
    ////* 내적의 값이 음수일 경우, 두 벡터가 수직에 가깝거나 반대 방향을 가지고 있으므로 
    ////* 충돌 방향에 따라 속도 변화가 작게 일어납니다.
    ////*============================================================================================
    //

    //// 내적을 사용하여 속도를 업데이트
    //double new_vx = vx - 2 * dot * unx;
    //double new_vy = vy - 2 * dot * uny;
    //double new_vx_B = vx_B + 2 * dot * unx;
    //double new_vy_B = vy_B + 2 * dot * uny;
    //
    ////*============================================================================================
    ////* 충돌에 따른 속도 업데이트 공식: 충돌 방향에 따른 속도 변화와 원래 속도를 고려하여 업데이트됩니다.
    ////* 객체 A: 
    ////* new_A.x = A.x - 2 * dot * unx
    ////* new_A.y = A.y - 2 * dot * uny
    ////* 객체 B: 
    ////* new_B.x = B.x + 2 * dot * unx
    ////* new_B.y = B.y + 2 * dot * uny
    ////*
    ////* dot: 충돌 방향에 대한 상대 속도 벡터의 정사영을 계산한 후에 얻어진 값입니다.
    ////*
    ////* 정사영(투영): 벡터의 정사영은 한 벡터가 다른 벡터에 떨어뜨릴 때 그림자와 같아서 
    ////* 한 벡터의 특정 요소를 포함하는 직교 방향에 대한 정보를 제공합니다.
    ////*
    ////* 2와 unx, uny를 곱해주는 이유: 충돌하는 두 물체는 작용-반작용 법칙에 따라 서로 동일한 힘이 상호 작용합니다.
    ////* 이를 포함하려면 속도 변화에 2를 곱해줍니다. 
    ////* 정사영에 2를 곱해 매칭된 힘을 구한 다음 이를 충돌 단위 벡터(unx, uny)와 곱하게 되면 
    ////* 충돌 방향에 따른 속도 변화를 얻을 수 있습니다.
    ////* 따라서, 2를 곱한 후 충돌 단위 벡터와 곱하는 이유는 작용-반작용 법칙과 
    ////* 모멘텀 보존 법칙(운동량 보존 법칙)을 반영하려는 것입니다.
    ////*============================================================================================
    //
    //
    //// 원래 속력과 업데이트된 속력
    //double org_speed = sqrt(vx * vx + vy * vy);
    //double new_speed = sqrt(new_vx * new_vx + new_vy * new_vy);
    //double org_speed_other = sqrt(vx_B * vx_B + vy_B * vy_B);
    //double new_speed_other = sqrt(new_vx_B * new_vx_B + new_vy_B * new_vy_B);
    //

    //// 업데이트된 속도를 원래 속력으로 정규화
    //new_vx = new_vx * org_speed / new_speed;
    //new_vy = new_vy * org_speed / new_speed;
    //new_vx_B = new_vx_B * org_speed_other / new_speed_other;
    //new_vy_B = new_vy_B * org_speed_other / new_speed_other;
    //
    ////*============================================================================================
    ////* 원래 속력과 업데이트된 속력을 비교하여 업데이트된 속력을 원래 속력만큼 정규화합니다.
    ////* 이렇게 하면 방향은 바뀌지만 속력이 유지됩니다.
    ////* org_speed와 new_speed는 달랐지만 계산 후 업데이트된 new_vx, new_vy의 속력은 org_speed와 같아집니다.
    ////* 즉, 방향 변화만 반영됩니다.
    ////*============================================================================================
    //

    //// 업데이트된 속도를 객체에 설정
    //vx = new_vx;
    //vy = new_vy;
    //other.SetVX(new_vx_B);
    //other.SetVY(new_vy_B);
    //

    ////*============================================================================================


    /* #3 운동량 보존 법칙을 고려한 속도, 방향 변화 */

    double vol_B = other.GetVolume();
    double vx_B = other.GetVX();
    double vy_B = other.GetVY();
    double nx = other.GetCX() - cx;
    double ny = other.GetCY() - cy;

    // 두 원의 중심의 거리
    double d = distance(other);

    // 충돌 방향 벡터(두 원의 중심 사이 벡터)의 단위 벡터
    double unx = nx / d;
    double uny = ny / d;

    // 상대 속도 벡터
    double rel_vx = vx - vx_B;
    double rel_vy = vy - vy_B;

    // 상대 속도 벡터와 중심 사이 단위 벡터 내적 (충돌 방향의 정사영 계산)
    double dot = rel_vx * unx + rel_vy * uny;

    // 부피를 고려한 충격량을 계산 (물체의 밀도가 동일하여 부피가 질량과 같다고 가정)
    double impulse_A = 2 * vol_B * dot / (volume + vol_B);
    double impulse_B = 2 * volume * dot / (volume + vol_B);

    //*============================================================================================
    //* p = m * v (p는 운동량, m은 질량, v는 속도)
    //* 질량(m) = 밀도(d) * 부피(v) (m은 질량, d는 밀도, v는 부피)
    //* 충돌할 때 물체의 운동량의 변화량을 충격량(impulse)라고 정의합니다.
    //* 운동량 보존 법칙에 의해, 두 물체의 충격량의 합은 0이 될 것입니다.
    //* x축 운동량 보존 : m1 * v1 + m2 * v2 = m1 * v'1 * cosθ1 + m2 * v'2 * cosθ2
    //* y축 운동량 보존 : 0 = m1 * v'1 sinθ1 + m2 * v'2 * sinθ2
    //* 에너지 보존 : 1/2 * m1 * v1^2 + 1/2 * m2 * v2^2 = 1/2 * m1 * v'1^2 + 1/2 * m2 * v'2^2
    //* https://cosmos520.tistory.com/60
    //* 
    //* 원시 모멘텀 및 에너지 보존 법칙에 따라 충격량 구하는 공식: 
    //* impulse_A = 2 * m_B * dot / (m_A + m_B) -> B가 A에게 주는 충격량
    //* impulse_B = 2 * m_A * dot / (m_A + m_B) -> A가 B에게 주는 충격량
    //* 작용-반작용이므로 2를 곱해줍니다.
    //*============================================================================================

    // 충격량에 따라 속도 벡터를 업데이트
    double new_vx = vx - unx * impulse_A;
    double new_vy = vy - uny * impulse_A;

    // 다른 객체도 충격량에 따라 속도 벡터를 업데이트
    double new_vx_B = vx_B + unx * impulse_B;
    double new_vy_B = vy_B + uny * impulse_B;

    // 업데이트된 속도를 객체에 설정
    vx = new_vx;
    vy = new_vy;
    other.SetVX(new_vx_B);
    other.SetVY(new_vy_B);
}

int CCircle::Combination(CObject& other)
{
    CCircle* circle = dynamic_cast<CCircle*>(&other);

    if (circle->volume + volume > 31416)
        return 0;

    if (circle->radius > radius)
    { // 다른 원에 흡수
        circle->SetVolume(circle->volume + volume);
        double r = sqrt(circle->volume / PI);
        circle->SetRadius(r);
        return 1;
    }
    else
    {
        SetVolume(circle->volume + volume);
        double r = sqrt(volume / PI);
        SetRadius(r);
        return 2;
    }
}

int CCircle::Decomposition(CObject& other)
{
    switch (other.GetType())
    {
    case 2: // 원과 사각형 충돌 -> 사각형 분열
    {
        CRectangle* rect = dynamic_cast<CRectangle*>(&other);
        if (rect)
        {
            if (rect->GetVolume() < 100)
                return 4;
            else
            {
                double vol = rect->GetVolume() / 4;
                rect->SetVolume(vol);
                double r = sqrt(vol);
                rect->SetRadius(r);
                return 6;
            }
        }
    }
    break;
    case 3: // 원과 별 충돌 -> 원 분열
    {
        if (volume < 314)
            return 0;
        else
        {
            double vol = volume / 4;
            SetVolume(vol);
            double r = sqrt(vol / PI);
            SetRadius(r);
            return 1;
        }
    }
    break;
    }
}

void CCircle::SetVolume(double vol)
{
    volume = vol;
}

/*
===============================================================================
                                        별
===============================================================================
*/

CStar::CStar(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 20 + 10;
    angle = DegreeToRadian(360.0 / 5);
    volume = radius * tan(angle / 2) * radius * 5;
    rotation = rand() % 72 + 1;
    angleVelocity = (rand() / HALF - 1) / 10;
}

CStar::CStar(POINT p, int type, double radius, double vx, double vy) : CObject(p, type)
{
    this->radius = radius;
    angle = DegreeToRadian(360.0 / 5);
    volume = radius * tan(angle / 2) * radius * 5;
    rotation = rand() % 72 + 1;
    angleVelocity = (rand() / HALF - 1) / 10;
    this->vx = vx;
    this->vy = vy;
}

void CStar::Update(RECT* rectView)
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
    rotation += angleVelocity;
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

bool CStar::Collision(const CObject& other) const
{
    switch (other.GetType())
    {
    case 1: // 별과 원 충돌
    {
        const CCircle* circle = dynamic_cast<const CCircle*>(&other);
        if (circle)
        {
            return distance(*circle) <= radius + circle->GetRadius();
        }
    }
    break;
    case 2: // 별과 사각형 충돌
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            return distance(*rect) <= radius + rect->GetRadius();
        }
    }
    break; 
    case 3: // 별과 별 충돌
    {
        const CStar* star = dynamic_cast<const CStar*>(&other);
        if (star)
        {
            return distance(*star) <= radius + star->radius;
        }
    }
    break;
    default:
        break;
    }

    return false;
}

double CStar::GetRadius() const
{
    return radius;
}

void CStar::SetRadius(double r)
{
    radius = r;
}

void CStar::SetPosition(CObject& other)
{
    double vol_B = other.GetVolume();
    double vx_B = other.GetVX();
    double vy_B = other.GetVY();
    double nol_x = other.GetCX() - cx;
    double nol_y = other.GetCY() - cy;

    // 두 원의 중심의 거리
    double d = distance(other);

    // 충돌 방향 벡터(두 원의 중심 사이 벡터)의 단위 벡터
    double u_nol_x = nol_x / d;
    double u_nol_y = nol_y / d;

    // 상대 속도 벡터
    double rel_vx = vx - vx_B;
    double rel_vy = vy - vy_B;

    // 상대 속도 벡터와 중심 사이 단위 벡터 내적 (충돌 방향의 정사영 계산)
    double dot = rel_vx * u_nol_x + rel_vy * u_nol_y;

    // 부피를 고려한 충격량을 계산 (물체의 밀도가 동일하여 부피가 질량과 같다고 가정)
    double impulse_A = 2 * vol_B * dot / (volume + vol_B);
    double impulse_B = 2 * volume * dot / (volume + vol_B);

    //* 원시 모멘텀 및 에너지 보존 법칙에 따라 충격량 구하는 공식: 
    //* impulse_A = 2 * m_B * dot / (m_A + m_B) -> B가 A에게 주는 충격량
    //* impulse_B = 2 * m_A * dot / (m_A + m_B) -> A가 B에게 주는 충격량
    //* 작용-반작용이므로 2를 곱해줍니다.

    // 충격량에 따라 속도 벡터를 업데이트
    double new_vx = vx - u_nol_x * impulse_A;
    double new_vy = vy - u_nol_y * impulse_A;

    // 다른 객체도 충격량에 따라 속도 벡터를 업데이트
    double new_vx_B = vx_B + u_nol_x * impulse_B;
    double new_vy_B = vy_B + u_nol_y * impulse_B;

    // 업데이트된 속도를 객체에 설정
    vx = new_vx;
    vy = new_vy;
    other.SetVX(new_vx_B);
    other.SetVY(new_vy_B);
}

int CStar::Combination(CObject& other)
{
    CStar* star = dynamic_cast<CStar*>(&other);

    if (star->volume + volume > 36327)
        return 0;

    if (star->radius > radius)
    { // 다른 원에 흡수
        star->SetVolume(star->volume + volume);
        double r = sqrt(star->volume / (5 * tan(angle / 2)));
        star->SetRadius(r);
        return 1;
    }
    else
    {
        SetVolume(star->volume + volume);
        double r = sqrt(volume / (5 * tan(angle / 2)));
        SetRadius(r);
        return 2;
    }
}

int CStar::Decomposition(CObject& other)
{
    switch (other.GetType())
    {
    case 2: // 별과 사각형 충돌 -> 별 분열
    {
        if (volume < 363)
            return 0;
        else
        {
            double vol = volume / 4;
            SetVolume(vol);
            double r = sqrt(vol / (5 * tan(angle / 2)));
            SetRadius(r);
            return 3;
        }
    }
    break;
    case 1: // 별과 원 충돌 -> 원 분열
    {
        CCircle* circle = dynamic_cast<CCircle*>(&other);
        if (circle->GetVolume() < 314)
            return 4;
        else
        {
            double vol = circle->GetVolume() / 4;
            circle->SetVolume(vol);
            double r = sqrt(vol / PI);
            circle->SetRadius(r);
            return 5;
        }
    }
    break;
    }
}

void CStar::SetVolume(double vol)
{
    volume = vol;
}

/*
===============================================================================
                                        사각형
===============================================================================
*/

CRectangle::CRectangle(POINT p, int type) : CObject(p, type)
{
    radius = rand() % 20 + 10;
    angle = DegreeToRadian(360.0 / 4);
    volume = radius * radius;
    rotation = rand() % 90 + 1;
    angleVelocity = (rand() / HALF - 1) / 10;
}

CRectangle::CRectangle(POINT p, int type, double radius, double vx, double vy) : CObject(p, type)
{
    this->radius = radius;
    angle = DegreeToRadian(360.0 / 4);
    volume = radius * radius;
    rotation = rand() % 90 + 1;
    angleVelocity = (rand() / HALF - 1) / 10;
    this->vx = vx;
    this->vy = vy;
}

void CRectangle::Update(RECT* rectView)
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
    rotation += angleVelocity;
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
}

bool CRectangle::Collision(const CObject& other) const
{
    switch (other.GetType())
    {
    case 1: // 별과 원 충돌
    {
        const CCircle* circle = dynamic_cast<const CCircle*>(&other);
        if (circle)
        {
            return distance(*circle) <= radius + circle->GetRadius();
        }
    }
    break;
    case 2: // 별과 사각형 충돌
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            return distance(*rect) <= radius + rect->radius;
        }
    }
    break;
    case 3: // 별과 별 충돌
    {
        const CStar* star = dynamic_cast<const CStar*>(&other);
        if (star)
        {
            return distance(*star) <= radius + star->GetRadius();
        }
    }
    break;
    default:
        break;
    }

    return false;
}

double CRectangle::GetRadius() const
{
    return radius;
}

void CRectangle::SetRadius(double r)
{
    radius = r;
}

void CRectangle::SetPosition(CObject& other)
{
    double vol_B = other.GetVolume();
    double vx_B = other.GetVX();
    double vy_B = other.GetVY();
    double nol_x = other.GetCX() - cx;
    double nol_y = other.GetCY() - cy;

    // 두 원의 중심의 거리
    double d = distance(other);

    // 충돌 방향 벡터(두 원의 중심 사이 벡터)의 단위 벡터
    double u_nol_x = nol_x / d;
    double u_nol_y = nol_y / d;

    // 상대 속도 벡터
    double rel_vx = vx - vx_B;
    double rel_vy = vy - vy_B;

    // 상대 속도 벡터와 중심 사이 단위 벡터 내적 (충돌 방향의 정사영 계산)
    double dot = rel_vx * u_nol_x + rel_vy * u_nol_y;

    // 부피를 고려한 충격량을 계산 (물체의 밀도가 동일하여 부피가 질량과 같다고 가정)
    double impulse_A = 2 * vol_B * dot / (volume + vol_B);
    double impulse_B = 2 * volume * dot / (volume + vol_B);

    //* 원시 모멘텀 및 에너지 보존 법칙에 따라 충격량 구하는 공식: 
    //* impulse_A = 2 * m_B * dot / (m_A + m_B) -> B가 A에게 주는 충격량
    //* impulse_B = 2 * m_A * dot / (m_A + m_B) -> A가 B에게 주는 충격량
    //* 작용-반작용이므로 2를 곱해줍니다.

    // 충격량에 따라 속도 벡터를 업데이트
    double new_vx = vx - u_nol_x * impulse_A;
    double new_vy = vy - u_nol_y * impulse_A;

    // 다른 객체도 충격량에 따라 속도 벡터를 업데이트
    double new_vx_B = vx_B + u_nol_x * impulse_B;
    double new_vy_B = vy_B + u_nol_y * impulse_B;

    // 업데이트된 속도를 객체에 설정
    vx = new_vx;
    vy = new_vy;
    other.SetVX(new_vx_B);
    other.SetVY(new_vy_B);
}

int CRectangle::Combination(CObject& other)
{
    CRectangle* rect = dynamic_cast<CRectangle*>(&other);

    if (rect->volume + volume > 10000)
        return 0;

    if (rect->radius > radius)
    { // 다른 원에 흡수
        rect->SetVolume(rect->volume + volume);
        double r = sqrt(rect->volume);
        rect->SetRadius(r);
        return 1;
    }
    else
    {
        SetVolume(rect->volume + volume);
        double r = sqrt(volume);
        SetRadius(r);
        return 2;
    }
}

int CRectangle::Decomposition(CObject& other)
{
    switch (other.GetType())
    {
    case 1: // 사각형과 원 충돌 -> 사각형 분열
    {
        if (volume < 100)
            return 0;
        else
        {
            double vol = volume / 4;
            SetVolume(vol);
            double r = sqrt(vol);
            SetRadius(r);
            return 2;
        }
    }
    break;
    case 3: // 사각형과 별 충돌 -> 별 분열
    {
        CStar* star = dynamic_cast<CStar*>(&other);
        if (star->GetVolume() < 363)
            return 4;
        else
        {
            double vol = star->GetVolume() / 4;
            star->SetVolume(vol);
            double r = sqrt(vol / (5 * tan(angle / 2)));
            star->SetRadius(r);
            return 7;
        }
    }
    break;
    }
}

void CRectangle::SetVolume(double vol)
{
    volume = vol;
}
