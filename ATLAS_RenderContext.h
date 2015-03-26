#ifndef ATLAS_RENDERCONTEXT_H
#include "ATLAS_Color.h"

class Point
{
public:
	Point() : x(0), y(0), c(Color::WHITE) {}
	Point(uint32 x, uint32 y, Color c = Color::WHITE) : x(x), y(y), c(c) {}

	Point &operator+=(const Point &rhs);
	Point &operator-=(const Point &rhs);
	Point &operator*=(const real32 &rhs);

	friend Point operator+(Point lhs, const Point &rhs);
	friend Point operator-(Point lhs, const Point &rhs);
	friend Point operator*(Point lhs, const real32 &rhs);

	real32 x, y;
	Color c;
};

Point operator+(Point lhs, const Point &rhs);
Point operator-(Point lhs, const Point &rhs);
Point operator*(Point lhs, const real32 &rhs);

class RenderContext
{
public:
	RenderContext(void *buffer, uint32 width, uint32 height, uint32 bpp);
	~RenderContext();

	void DrawCurve(Point p0, Point p1, Point p2);
	void DrawCurve(Point p0, Point p1, Point p2, Point p3);
	void DrawLine(Point p0, Point p1);
	void DrawPoint(Point p);
	void DrawPixel(uint32 x, uint32 y, const Color &color);
	void Resize(uint32 width, uint32 height);
	void Clear();

	void SetBlendMode(Color::BlendMode blend_mode);
	void SetClearColor(const Color &color);
	void SetPointSize(int32 size);

	Color::BlendMode GetBlendMode();
	Color GetClearColor();
	int32 GetPointSize();

private:
	void				*m_FrameBuffer;
	uint32				m_Width;
	uint32				m_Height;

	Color::BlendMode	m_BlendMode;
	Color32				m_ClearColor;
	int32				m_PointSize;
};

#define ATLAS_RENDERCONTEXT_H
#endif