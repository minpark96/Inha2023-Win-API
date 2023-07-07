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
	double vx, vy;
	double volume;
	int type;
public:
	CObject(POINT p, int type);
	~CObject();

	/* 가상 함수 */
	virtual void Update(RECT* rectView) = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual bool Collision(const CObject& other) const = 0;
	virtual int Combination(CObject& other) = 0;
	virtual int Decomposition(CObject& other) = 0;
	virtual void SetPosition(CObject& other) = 0;
	virtual void SetVolume(double vol) = 0;

	/* GET */
	int GetType() const;
	double GetCX() const;
	double GetCY() const;
	double GetVX() const;
	double GetVY() const;
	double GetVolume() const;

	/* SET */
	void SetCX(double v);
	void SetCY(double v);
	void SetVX(double v);
	void SetVY(double v);

	/* 기타 */
	double distance(const CObject& other) const;
};

class CCircle : public CObject
{
private:
	double radius;
public:
	CCircle(POINT p, int type);
	void Update(RECT* rectView);
	void Draw(HDC hdc) override;
	bool Collision(const CObject& other) const override;
	double GetRadius() const;
	void SetRadius(double r);
	void SetPosition(CObject& other) override;
	int Combination(CObject& other) override;
	int Decomposition(CObject& other) override;
	void SetVolume(double vol) override;
};

class CStar : public CObject
{
private:
	double radius;
	double angle;
	double rotation;
	double angleVelocity;
public:
	CStar(POINT p, int type);
	void Update(RECT* rectView);
	void Draw(HDC hdc) override;
	bool Collision(const CObject& other) const override;
	double GetRadius() const;
	void SetRadius(double r);
	void SetPosition(CObject& other) override;
	int Combination(CObject& other) override;
	int Decomposition(CObject& other) override;
	void SetVolume(double vol) override;
};

class CRectangle : public CObject
{
private:
	double radius;
	double angle;
	double rotation;
	double angleVelocity;
public:
	CRectangle(POINT p, int type);
	void Update(RECT* rectView);
	void Draw(HDC hdc) override;
	bool Collision(const CObject& other) const override;
	double GetRadius() const;
	void SetRadius(double r);
	void SetPosition(CObject& other) override;
	int Combination(CObject& other) override;
	int Decomposition(CObject& other) override;
	void SetVolume(double vol) override;
};
