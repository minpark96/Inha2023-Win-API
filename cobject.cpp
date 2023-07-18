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
                                        ��
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
    // ������
    if (cx + radius >= rectView->right)
    {
        vx *= -1;
        cx = rectView->right - radius;
    }
    // ����
    if (cx - radius <= rectView->left)
    {
        vx *= -1;
        cx = rectView->left + radius;
    }
    // ��
    if (cy - radius <= rectView->top)
    {
        vy *= -1;
        cy = rectView->top + radius;
    }
    // �Ʒ�
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
    case 1: // ���� �� �浹
    {
        const CCircle* circle = dynamic_cast<const CCircle*>(&other);
        if (circle)
        {
            return distance(*circle) <= radius + circle->radius;
        }
    }
    break;
    case 2: // ���� �簢�� �浹
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            return distance(*rect) <= radius + rect->GetRadius();
        }
    }
    break;    
    case 3: // ���� �� �浹
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
    ///* #1 �ܼ� ���⸸ ���� (�浹 ���� ������� �ʾ� ��Ȯ���� ����) */

    //// ��B�� �ӵ� ����
    //double va = other.GetVX();
    //double vb = other.GetVY();
    //
    //// ��A�� �ӵ� �ӽ� ����
    //double tmp_vx = vx;
    //double tmp_vy = vy;
    //
    //// ��A�� �ӵ��� ��B�� �ӵ���
    //vx = va;
    //vy = vb;
    //
    //// ��B�� �ӵ��� ��A�� �ӵ���
    //other.SetVX(tmp_vx);
    //other.SetVY(tmp_vy);


    ////*============================================================================================
    //

    ///* #2 �浹 ���� ��� �Ͽ� ���⸸ ���� (�ӵ� ��ȭ ����) */

    //// ��B�� �ӵ� ����
    //double vx_B = other.GetVX();
    //double vy_B = other.GetVY();


    //// �� ���� �߽� ���� ���� (�浹 ���� ����)
    //double nx = other.GetCX() - cx;
    //double ny = other.GetCY() - cy;


    //// �� ���� �߽��� �Ÿ�
    //double d = distance(other);
    //
    ////*============================================================================================
    ////* �Ÿ� ��� ����: �� �� ������ �Ÿ��� ����ϱ� ���� ��Ŭ���� �Ÿ� ��� ������ ����մϴ�.
    ////* distance = sqrt((x2 - x1)^2 + (y2 - y1)^2)
    ////*============================================================================================


    //// �߽� ���� ������ ���� ����
    //double unx = nx / d;
    //double uny = ny / d;

    ////*============================================================================================
    ////* ���� ���� ��� ����: �� �� ������ ���� ���ʹ� �Ÿ��� ���� ���Դϴ�.
    ////* ux = (x2 - x1) / distance
    ////* uy = (y2 - y1) / distance
    ////*============================================================================================


    //// ��� �ӵ� ����
    //double rel_vx = vx - vx_B;
    //double rel_vy = vy - vy_B;

    ////*============================================================================================
    ////* ��� �ӵ��� ����ϴ� ������ �浹 �� �߻��ϴ� �ӵ� �� ���� ��ȭ�� ���ϱ� ���ؼ��Դϴ�.
    ////* ��� �ӵ��� �� ��ü�� �ӵ��� �ٸ� ��ü�� �ӵ� ������ ���̸� ��Ÿ���ϴ�.
    ////* �̸� ����ϸ� �� ��ü ���� ��ȣ �ۿ��� ��� �浹 �� �߻��ϴ� �ӵ� ��ȭ�� ������ �ִ��� ����� �� �ֽ��ϴ�. 
    ////* ��� �ӵ��� ����ϸ� �浹���� �߻��� �� ��ü�� �ӵ� ��ȭ�� �� ��Ȯ�ϰ� ������ �� �ֽ��ϴ�. 
    ////* �̸� ���� ��ü�� ���� ��ȣ �ۿ��ϴ� ��Ŀ� ���� ��Ȯ�� ���ؿ� �浹���� �Ͼ�� �ӵ� �� ���� ��ȭ�� ���� �� �ֽ��ϴ�.
    ////* ����, ��� �ӵ��� ����ϸ� �浹 �������� �󸶳� �ٸ� �� ��ü�� �ӵ��� �����ؾ� �ϴ��� ���� �� �ֽ��ϴ�.
    ////* �̷��� �ϸ� �浹�ϴ� �� ��ü ������ ���� �Ÿ��� �����ϸ鼭 �������� ������ �� �ֽ��ϴ�.
    ////*============================================================================================
    //

    //// ��� �ӵ� ���Ϳ� �߽� ���� ���� ���� ���� (�浹 ������ ���翵 ���)
    //double dot = rel_vx * unx + rel_vy * uny;
    //
    ////*============================================================================================
    ////* ������ ���� ����: ���� a�� ���� b�� ������ �� ������ ���� ���� ���Դϴ�.
    ////* dot_product = a.x * b.x + a.y * b.y
    ////*
    ////* �浹 ����(���� ����)�� ��� �ӵ� ������ ����(dot product)�� ����ϴ� ������ 
    ////* �浹 �� �߻��ϴ� �ӵ� ��ȭ�� ���ϱ� ���ؼ��Դϴ�.
    ////* ������ ����ϸ� �� ���� ������ ������ ���� ������ ����� �� �ֱ� ������, 
    ////* �浹 �� �߻��ϴ� ���� ��ȭ�� �� ��Ȯ�ϰ� ����� �� �ֽ��ϴ�.
    ////* ������ ���� �� ������ ���̿� �� ���� ������ ����(cosine)�� ���� ���Դϴ�.
    ////* ��, ������ �� ���� ������ ���� ������ ������ �ֽ��ϴ�.
    ////* �浹 �ÿ��� �ӵ� ������ ��ȭ�� �� ��ü ������ ������ ���� �޶����ϴ�. 
    ////* ��� �ӵ� ���Ϳ� �浹 ������ ���� ������ ������ ���� �浹 ����� ���õ� �ӵ� ��ȭ�� ���� �� �ֽ��ϴ�. 
    ////* 
    ////* �� ��� ����� �� ���Ͱ� �󸶳� ������ ������ ������ �ִ����� ��Ÿ���� ���Դϴ�.
    ////* �� ���Ͱ� ���� ������ ���, ������ ���� �� ������ ������ ����ŭ ũ�� �˴ϴ�.
    ////* �ݸ�, �� ���Ͱ� ������ ��� ������ ���� 0�� �˴ϴ�.
    ////* �� ���� ������ �����غ��� �ڻ��� 90�� ���̱� �����Դϴ�.
    ////* ���� �� �ڵ忡�� dot_product = (rel_vx * unx + rel_vy * uny)�� ��� ����� 0�� ��������
    ////* �� ��ü�� �ӵ� ���Ͱ� �浹 ����� ������ ��������Ƿ� �浹 ���⿡ ���� ������ �������ϴ�.
    ////* �ݸ�, ������ ���� ũ�� ū ���� ������ �ʿ��մϴ�.
    ////* ������ ���� ����� ���, �� ���Ͱ� ���� ������ ������ �ְ�, ���� �浹 ���⿡ ���� �ӵ� ��ȭ�� ũ�� �Ͼ�ϴ�.
    ////* ������ ���� ������ ���, �� ���Ͱ� ������ �����ų� �ݴ� ������ ������ �����Ƿ� 
    ////* �浹 ���⿡ ���� �ӵ� ��ȭ�� �۰� �Ͼ�ϴ�.
    ////*============================================================================================
    //

    //// ������ ����Ͽ� �ӵ��� ������Ʈ
    //double new_vx = vx - 2 * dot * unx;
    //double new_vy = vy - 2 * dot * uny;
    //double new_vx_B = vx_B + 2 * dot * unx;
    //double new_vy_B = vy_B + 2 * dot * uny;
    //
    ////*============================================================================================
    ////* �浹�� ���� �ӵ� ������Ʈ ����: �浹 ���⿡ ���� �ӵ� ��ȭ�� ���� �ӵ��� ����Ͽ� ������Ʈ�˴ϴ�.
    ////* ��ü A: 
    ////* new_A.x = A.x - 2 * dot * unx
    ////* new_A.y = A.y - 2 * dot * uny
    ////* ��ü B: 
    ////* new_B.x = B.x + 2 * dot * unx
    ////* new_B.y = B.y + 2 * dot * uny
    ////*
    ////* dot: �浹 ���⿡ ���� ��� �ӵ� ������ ���翵�� ����� �Ŀ� ����� ���Դϴ�.
    ////*
    ////* ���翵(����): ������ ���翵�� �� ���Ͱ� �ٸ� ���Ϳ� ����߸� �� �׸��ڿ� ���Ƽ� 
    ////* �� ������ Ư�� ��Ҹ� �����ϴ� ���� ���⿡ ���� ������ �����մϴ�.
    ////*
    ////* 2�� unx, uny�� �����ִ� ����: �浹�ϴ� �� ��ü�� �ۿ�-���ۿ� ��Ģ�� ���� ���� ������ ���� ��ȣ �ۿ��մϴ�.
    ////* �̸� �����Ϸ��� �ӵ� ��ȭ�� 2�� �����ݴϴ�. 
    ////* ���翵�� 2�� ���� ��Ī�� ���� ���� ���� �̸� �浹 ���� ����(unx, uny)�� ���ϰ� �Ǹ� 
    ////* �浹 ���⿡ ���� �ӵ� ��ȭ�� ���� �� �ֽ��ϴ�.
    ////* ����, 2�� ���� �� �浹 ���� ���Ϳ� ���ϴ� ������ �ۿ�-���ۿ� ��Ģ�� 
    ////* ����� ���� ��Ģ(��� ���� ��Ģ)�� �ݿ��Ϸ��� ���Դϴ�.
    ////*============================================================================================
    //
    //
    //// ���� �ӷ°� ������Ʈ�� �ӷ�
    //double org_speed = sqrt(vx * vx + vy * vy);
    //double new_speed = sqrt(new_vx * new_vx + new_vy * new_vy);
    //double org_speed_other = sqrt(vx_B * vx_B + vy_B * vy_B);
    //double new_speed_other = sqrt(new_vx_B * new_vx_B + new_vy_B * new_vy_B);
    //

    //// ������Ʈ�� �ӵ��� ���� �ӷ����� ����ȭ
    //new_vx = new_vx * org_speed / new_speed;
    //new_vy = new_vy * org_speed / new_speed;
    //new_vx_B = new_vx_B * org_speed_other / new_speed_other;
    //new_vy_B = new_vy_B * org_speed_other / new_speed_other;
    //
    ////*============================================================================================
    ////* ���� �ӷ°� ������Ʈ�� �ӷ��� ���Ͽ� ������Ʈ�� �ӷ��� ���� �ӷ¸�ŭ ����ȭ�մϴ�.
    ////* �̷��� �ϸ� ������ �ٲ����� �ӷ��� �����˴ϴ�.
    ////* org_speed�� new_speed�� �޶����� ��� �� ������Ʈ�� new_vx, new_vy�� �ӷ��� org_speed�� �������ϴ�.
    ////* ��, ���� ��ȭ�� �ݿ��˴ϴ�.
    ////*============================================================================================
    //

    //// ������Ʈ�� �ӵ��� ��ü�� ����
    //vx = new_vx;
    //vy = new_vy;
    //other.SetVX(new_vx_B);
    //other.SetVY(new_vy_B);
    //

    ////*============================================================================================


    /* #3 ��� ���� ��Ģ�� ����� �ӵ�, ���� ��ȭ */

    double vol_B = other.GetVolume();
    double vx_B = other.GetVX();
    double vy_B = other.GetVY();
    double nx = other.GetCX() - cx;
    double ny = other.GetCY() - cy;

    // �� ���� �߽��� �Ÿ�
    double d = distance(other);

    // �浹 ���� ����(�� ���� �߽� ���� ����)�� ���� ����
    double unx = nx / d;
    double uny = ny / d;

    // ��� �ӵ� ����
    double rel_vx = vx - vx_B;
    double rel_vy = vy - vy_B;

    // ��� �ӵ� ���Ϳ� �߽� ���� ���� ���� ���� (�浹 ������ ���翵 ���)
    double dot = rel_vx * unx + rel_vy * uny;

    // ���Ǹ� ����� ��ݷ��� ��� (��ü�� �е��� �����Ͽ� ���ǰ� ������ ���ٰ� ����)
    double impulse_A = 2 * vol_B * dot / (volume + vol_B);
    double impulse_B = 2 * volume * dot / (volume + vol_B);

    //*============================================================================================
    //* p = m * v (p�� ���, m�� ����, v�� �ӵ�)
    //* ����(m) = �е�(d) * ����(v) (m�� ����, d�� �е�, v�� ����)
    //* �浹�� �� ��ü�� ����� ��ȭ���� ��ݷ�(impulse)��� �����մϴ�.
    //* ��� ���� ��Ģ�� ����, �� ��ü�� ��ݷ��� ���� 0�� �� ���Դϴ�.
    //* x�� ��� ���� : m1 * v1 + m2 * v2 = m1 * v'1 * cos��1 + m2 * v'2 * cos��2
    //* y�� ��� ���� : 0 = m1 * v'1 sin��1 + m2 * v'2 * sin��2
    //* ������ ���� : 1/2 * m1 * v1^2 + 1/2 * m2 * v2^2 = 1/2 * m1 * v'1^2 + 1/2 * m2 * v'2^2
    //* https://cosmos520.tistory.com/60
    //* 
    //* ���� ����� �� ������ ���� ��Ģ�� ���� ��ݷ� ���ϴ� ����: 
    //* impulse_A = 2 * m_B * dot / (m_A + m_B) -> B�� A���� �ִ� ��ݷ�
    //* impulse_B = 2 * m_A * dot / (m_A + m_B) -> A�� B���� �ִ� ��ݷ�
    //* �ۿ�-���ۿ��̹Ƿ� 2�� �����ݴϴ�.
    //*============================================================================================

    // ��ݷ��� ���� �ӵ� ���͸� ������Ʈ
    double new_vx = vx - unx * impulse_A;
    double new_vy = vy - uny * impulse_A;

    // �ٸ� ��ü�� ��ݷ��� ���� �ӵ� ���͸� ������Ʈ
    double new_vx_B = vx_B + unx * impulse_B;
    double new_vy_B = vy_B + uny * impulse_B;

    // ������Ʈ�� �ӵ��� ��ü�� ����
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
    { // �ٸ� ���� ���
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
    case 2: // ���� �簢�� �浹 -> �簢�� �п�
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
    case 3: // ���� �� �浹 -> �� �п�
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
                                        ��
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
    // ������
    if (cx + radius >= rectView->right)
    {
        vx *= -1;
        cx = rectView->right - radius;
    }
    // ����
    if (cx - radius <= rectView->left)
    {
        vx *= -1;
        cx = rectView->left + radius;
    }
    // ��
    if (cy - radius <= rectView->top)
    {
        vy *= -1;
        cy = rectView->top + radius;
    }
    // �Ʒ�
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
    case 1: // ���� �� �浹
    {
        const CCircle* circle = dynamic_cast<const CCircle*>(&other);
        if (circle)
        {
            return distance(*circle) <= radius + circle->GetRadius();
        }
    }
    break;
    case 2: // ���� �簢�� �浹
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            return distance(*rect) <= radius + rect->GetRadius();
        }
    }
    break; 
    case 3: // ���� �� �浹
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

    // �� ���� �߽��� �Ÿ�
    double d = distance(other);

    // �浹 ���� ����(�� ���� �߽� ���� ����)�� ���� ����
    double u_nol_x = nol_x / d;
    double u_nol_y = nol_y / d;

    // ��� �ӵ� ����
    double rel_vx = vx - vx_B;
    double rel_vy = vy - vy_B;

    // ��� �ӵ� ���Ϳ� �߽� ���� ���� ���� ���� (�浹 ������ ���翵 ���)
    double dot = rel_vx * u_nol_x + rel_vy * u_nol_y;

    // ���Ǹ� ����� ��ݷ��� ��� (��ü�� �е��� �����Ͽ� ���ǰ� ������ ���ٰ� ����)
    double impulse_A = 2 * vol_B * dot / (volume + vol_B);
    double impulse_B = 2 * volume * dot / (volume + vol_B);

    //* ���� ����� �� ������ ���� ��Ģ�� ���� ��ݷ� ���ϴ� ����: 
    //* impulse_A = 2 * m_B * dot / (m_A + m_B) -> B�� A���� �ִ� ��ݷ�
    //* impulse_B = 2 * m_A * dot / (m_A + m_B) -> A�� B���� �ִ� ��ݷ�
    //* �ۿ�-���ۿ��̹Ƿ� 2�� �����ݴϴ�.

    // ��ݷ��� ���� �ӵ� ���͸� ������Ʈ
    double new_vx = vx - u_nol_x * impulse_A;
    double new_vy = vy - u_nol_y * impulse_A;

    // �ٸ� ��ü�� ��ݷ��� ���� �ӵ� ���͸� ������Ʈ
    double new_vx_B = vx_B + u_nol_x * impulse_B;
    double new_vy_B = vy_B + u_nol_y * impulse_B;

    // ������Ʈ�� �ӵ��� ��ü�� ����
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
    { // �ٸ� ���� ���
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
    case 2: // ���� �簢�� �浹 -> �� �п�
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
    case 1: // ���� �� �浹 -> �� �п�
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
                                        �簢��
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
    // ������
    if (cx + radius >= rectView->right)
    {
        vx *= -1;
        cx = rectView->right - radius;
    }
    // ����
    if (cx - radius <= rectView->left)
    {
        vx *= -1;
        cx = rectView->left + radius;
    }
    // ��
    if (cy - radius <= rectView->top)
    {
        vy *= -1;
        cy = rectView->top + radius;
    }
    // �Ʒ�
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
    case 1: // ���� �� �浹
    {
        const CCircle* circle = dynamic_cast<const CCircle*>(&other);
        if (circle)
        {
            return distance(*circle) <= radius + circle->GetRadius();
        }
    }
    break;
    case 2: // ���� �簢�� �浹
    {
        const CRectangle* rect = dynamic_cast<const CRectangle*>(&other);
        if (rect)
        {
            return distance(*rect) <= radius + rect->radius;
        }
    }
    break;
    case 3: // ���� �� �浹
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

    // �� ���� �߽��� �Ÿ�
    double d = distance(other);

    // �浹 ���� ����(�� ���� �߽� ���� ����)�� ���� ����
    double u_nol_x = nol_x / d;
    double u_nol_y = nol_y / d;

    // ��� �ӵ� ����
    double rel_vx = vx - vx_B;
    double rel_vy = vy - vy_B;

    // ��� �ӵ� ���Ϳ� �߽� ���� ���� ���� ���� (�浹 ������ ���翵 ���)
    double dot = rel_vx * u_nol_x + rel_vy * u_nol_y;

    // ���Ǹ� ����� ��ݷ��� ��� (��ü�� �е��� �����Ͽ� ���ǰ� ������ ���ٰ� ����)
    double impulse_A = 2 * vol_B * dot / (volume + vol_B);
    double impulse_B = 2 * volume * dot / (volume + vol_B);

    //* ���� ����� �� ������ ���� ��Ģ�� ���� ��ݷ� ���ϴ� ����: 
    //* impulse_A = 2 * m_B * dot / (m_A + m_B) -> B�� A���� �ִ� ��ݷ�
    //* impulse_B = 2 * m_A * dot / (m_A + m_B) -> A�� B���� �ִ� ��ݷ�
    //* �ۿ�-���ۿ��̹Ƿ� 2�� �����ݴϴ�.

    // ��ݷ��� ���� �ӵ� ���͸� ������Ʈ
    double new_vx = vx - u_nol_x * impulse_A;
    double new_vy = vy - u_nol_y * impulse_A;

    // �ٸ� ��ü�� ��ݷ��� ���� �ӵ� ���͸� ������Ʈ
    double new_vx_B = vx_B + u_nol_x * impulse_B;
    double new_vy_B = vy_B + u_nol_y * impulse_B;

    // ������Ʈ�� �ӵ��� ��ü�� ����
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
    { // �ٸ� ���� ���
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
    case 1: // �簢���� �� �浹 -> �簢�� �п�
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
    case 3: // �簢���� �� �浹 -> �� �п�
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
