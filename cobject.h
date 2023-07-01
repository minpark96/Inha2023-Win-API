#pragma once

#include "framework.h"

#define DegreeToRadian(degree) ((degree) * PI / 180)
#define PI 3.141592653589793
#define HALF 16383.5
#define MAX 32767.0

class CObject
{
protected:
	double cx, cy;
	double speed;
	double va, vb;
	int type;
public:
	CObject(POINT p, int type);
	~CObject();
	void Update();
	virtual void Draw(HDC hdc) = 0;
	virtual bool CollisionBoundary(int flag, const LONG& rect1, const LONG& rect2) const = 0;
	virtual bool Collision(const CObject& other) const = 0;
	double distance(const CObject& other) const;
	void SetPosition(double na, double nb);
	int GetType() const;
};

class CCircle : public CObject
{
private :
	double radius;
public:
	CCircle(POINT p, int type);
	void Draw(HDC hdc) override;
	bool CollisionBoundary(int flag, const LONG& rect1, const LONG& rect2) const override;
	bool Collision(const CObject& other) const override;
	double GetRadius() const;
};

class CStar : public CObject
{
private:
	double radius;
public:
	CStar(POINT p, int type);
	void Draw(HDC hdc) override;
	bool CollisionBoundary(int flag, const LONG& rect1, const LONG& rect2) const override;
	bool Collision(const CObject& other) const override;
	double GetRadius() const;
};

class CRectangle : public CObject
{
private:
	double radius;
public:
	CRectangle(POINT p, int type);
	void Draw(HDC hdc) override;
	bool CollisionBoundary(int flag, const LONG& rect1, const LONG& rect2) const override;
	bool Collision(const CObject& other) const override;
	double GetRadius() const;
};

