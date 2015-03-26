#include "ATLAS_Stdafx.h"
#include "ATLAS_RenderContext.h"
#include "ATLAS_Texture.h"
#include <memory>
#include <Windows.h>

Point &Point::operator+=(const Point &rhs)
{
	x += rhs.x;
	y += rhs.y;
	c += rhs.c;

	return *this;
}
Point &Point::operator-=(const Point &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	c -= rhs.c;

	return *this;
}
Point &Point::operator*=(const real32 &rhs)
{
	x *= rhs;
	y *= rhs;
	c *= rhs;

	return *this;
}
Point operator+(Point lhs, const Point &rhs)
{
	return lhs += rhs;
}
Point operator-(Point lhs, const Point &rhs)
{
	return lhs -= rhs;
}
Point operator*(Point lhs, const real32 &rhs)
{
	return lhs *= rhs;
}

RenderContext::RenderContext(void *buffer, uint32 width, uint32 height, uint32 bpp)
{
	m_FrameBuffer = buffer;
	m_Width = width;
	m_Height = height;

	SetBlendMode(Color::BLEND_NORMAL);
	SetClearColor(Color::BLACK);
	SetPointSize(1);
}
RenderContext::~RenderContext()
{
}

void RenderContext::DrawCurve(Point p0, Point p1, Point p2)
{
	for (real32 t = 0.0f; t < 1.0f; t += 0.01f) {
		real32 one_minus_t = 1.0f - t;
		real32 t_plus = t + 0.01f;
		real32 one_minus_t_plus = 1.0f - t_plus;

		Point b1 = (p0 * (one_minus_t * one_minus_t)) +
			(p1 * (2 * one_minus_t * t)) +
			(p2 * (t * t));

		Point b2 = (p0 * (one_minus_t_plus * one_minus_t_plus)) +
			(p1 * (2 * one_minus_t_plus * t_plus)) +
			(p2 * (t_plus * t_plus));

		DrawLine(b1, b2);
	}
}
void RenderContext::DrawCurve(Point p0, Point p1, Point p2, Point p3)
{
	for (real32 t = 0.0f; t < 1.0f; t += 0.01f) {
		real32 one_minus_t = 1.0f - t;
		real32 t_plus = t + 0.01f;
		real32 one_minus_t_plus = 1.0f - t_plus;

		Point b1 = (p0 * (one_minus_t * one_minus_t * one_minus_t)) +
			(p1 * (3 * (one_minus_t * one_minus_t) * t)) +
			(p2 * (3 * one_minus_t * (t * t))) + 
			(p3 * (t * t * t));
		Point b2 = (p0 * (one_minus_t_plus * one_minus_t_plus * one_minus_t_plus)) +
			(p1 * (3 * (one_minus_t_plus * one_minus_t_plus) * t_plus)) +
			(p2 * (3 * one_minus_t_plus * (t_plus * t_plus))) +
			(p3 * (t_plus * t_plus * t_plus));

		DrawLine(b1, b2);
	}
}
void RenderContext::DrawLine(Point p0, Point p1)
{
	real32 xdiff = (p1.x - p0.x);
	real32 ydiff = (p1.y - p0.y);

	if (xdiff == 0.0f && ydiff == 0.0f) {
		DrawPixel((uint32)p0.x, (uint32)p0.y, p0.c);
		return;
	}

	if (fabs(xdiff) > fabs(ydiff)) {
		real32 xmin, xmax;

		if (p0.x < p1.x) {
			xmin = p0.x;
			xmax = p1.x;
		}
		else {
			xmin = p1.x;
			xmax = p0.x;
		}

		real32 slope = ydiff / xdiff;
		for (real32 x = xmin; x <= xmax; x += 1.0f) {
			real32 y = p0.y + ((x - p0.x) * slope);
			Color color = p0.c + ((p1.c - p0.c) * ((x - p0.x) / xdiff));
			DrawPixel((uint32)x, (uint32)y, color);
		}
	}
	else {
		real32 ymin, ymax;

		if (p0.y < p1.y) {
			ymin = p0.y;
			ymax = p1.y;
		}
		else {
			ymin = p1.y;
			ymax = p0.y;
		}

		real32 slope = xdiff / ydiff;
		for (real32 y = ymin; y <= ymax; y += 1.0f) {
			real32 x = p0.x + ((y - p0.y) * slope);
			Color color = p0.c + ((p1.c - p0.c) * ((y - p0.y) / ydiff));
			DrawPixel((uint32)x, (uint32)y, color);
		}
	}
}
void RenderContext::DrawPoint(Point p)
{
	for (int32 i = -m_PointSize; i < m_PointSize; ++i) {
		for (int32 j = -m_PointSize; j < m_PointSize; ++j) {
			DrawPixel((uint32)p.x + j, (uint32)p.y + i, p.c);
		}
	}
}
void RenderContext::DrawPixel(uint32 x, uint32 y, const Color &color) {
	if (x >= m_Width || y >= m_Height) {
		//__debugbreak();
		return;
	}

	uint32 *pixel = (uint32 *)(m_FrameBuffer) + x + (y * m_Width);
	*pixel = color.toColor32();
}
void RenderContext::Resize(uint32 width, uint32 height)
{
	m_Width = width;
	m_Height = height;
}
void RenderContext::Clear()
{
	uint32 *pixel = (uint32 *)m_FrameBuffer;

	for (uint32 i = 0; i < m_Height * m_Width; ++i) {
			pixel[i] = m_ClearColor;
	}
}

void RenderContext::SetBlendMode(Color::BlendMode blend_mode)
{
	m_BlendMode = blend_mode;
}
void RenderContext::SetClearColor(const Color &color)
{
	m_ClearColor = color.toColor32();
}
void RenderContext::SetPointSize(int32 size)
{
	m_PointSize = size;
}

Color::BlendMode RenderContext::GetBlendMode()
{
	return (m_BlendMode);
}
Color RenderContext::GetClearColor()
{
	real32 a = ((m_ClearColor >> 24) & 0xFF) / 255.0f;
	real32 r = ((m_ClearColor >> 16) & 0xFF) / 255.0f;
	real32 g = ((m_ClearColor >> 8) & 0xFF) / 255.0f;
	real32 b = ((m_ClearColor >> 0) & 0xFF) / 255.0f;
	return Color (r, g, b, a);
}
int32 RenderContext::GetPointSize()
{
	return m_PointSize;
}