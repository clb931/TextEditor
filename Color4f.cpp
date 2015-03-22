#include "Stdafx.h"
#include "Color4f.h"

Color4f::Color4f(real32 r, real32 g, real32 b, real32 a)
{
	R = min(1.0f, r);
	G = min(1.0f, g);
	B = min(1.0f, b);
	A = min(1.0f, a);
}
Color4f::Color4f(uint32 c)
{
	R = ((c >> 16) & 0xFF) / 255.0f;
	G = ((c >> 8) & 0xFF) / 255.0f;
	B = ((c >> 0) & 0xFF) / 255.0f;
	A = ((c >> 24) & 0xFF) / 255.0f;
}
Color32 Color4f::toColor32() const
{
	return Color32(
		((uint32)(A * 255.0f) << 24) |
		((uint32)(R * 255.0f) << 16) |
		((uint32)(G * 255.0f) << 8) |
		((uint32)(B * 255.0f) << 0));
}
Color4f Color4f::operator+=(const Color4f &top)
{
	R += top.R;
	G += top.G;
	B += top.B;
	A += top.A;
	return *this;
}
Color4f Color4f::operator-(const Color4f &top) const
{
	return Color4f(R - top.R, G - top.G, B - top.B, A - top.A);
}
Color4f Color4f::operator*(const Color4f &top) const
{
	return Color4f(R * top.R, G * top.G, B * top.B, A * top.A);
}
Color4f Color4f::operator/(const Color4f &top) const
{
	if (top.R == 0.0f && top.G == 0.0f && top.B == 0.0f)
		return Color4f();

	Color4f c(R / top.R, G / top.G, B / top.B, A / top.A);
	return c;
}
Color4f Color4f::operator*(real32 f) const
{
	return Color4f(R * f, G * f, B * f, A * f);
}
Color4f Color4f::operator/(real32 f) const
{
	return Color4f(R / f, G / f, B / f, A / f);
}
Color4f Color4f::operator~()
{
	return Color4f(1.0f - R, 1.0f - G, 1.0f - B, 1.0f - A);
}

Color4f operator+(Color4f bottom, const Color4f &top)
{
	return bottom += top;
}

char *GetBlendModeName(BlendMode blend_mode)
{
	char *result;

	switch (blend_mode) {
	default:
	case BLEND_NORMAL:
		result = "BLEND_NORMAL";
		break;
	case BLEND_ADD:
		result = "BLEND_ADD";
		break;
	case BLEND_SUBTRACT:
		result = "BLEND_SUBTRACT";
		break;
	case BLEND_MULTIPLY:
		result = "BLEND_MULTIPLY";
		break;
	case BLEND_DIVIDE:
		result = "BLEND_DIVIDE";
		break;
	case BLEND_SCREEN:
		result = "BLEND_SCREEN";
		break;
	case BLEND_OVERLAY:
		result = "BLEND_OVERLAY";
		break;
	case BLEND_COLOR_DOGE:
		result = "BLEND_COLOR_DOGE";
		break;
	case BLEND_COLOR_BURN:
		result = "BLEND_COLOR_BURN";
		break;
	case BLEND_DIFFERENCE:
		result = "BLEND_DIFFERENCE";
		break;
	case BLEND_DARKEN:
		result = "BLEND_DARKEN";
		break;
	case BLEND_LIGHTEN:
		result = "BLEND_LIGHTEN";
		break;
	}

	return result;
}
Color4f Blend(Color4f bottom, Color4f top, BlendMode blend_mode)
{
	Color4f result;

	switch (blend_mode) {
	default:
	case BLEND_NORMAL:
		result = BlendNormal(bottom, top);
		break;
	case BLEND_ADD:
		result = BlendAdd(bottom, top);
		break;
	case BLEND_SUBTRACT:
		result = BlendSubtract(bottom, top);
		break;
	case BLEND_MULTIPLY:
		result = BlendMultiply(bottom, top);
		break;
	case BLEND_DIVIDE:
		result = BlendDivide(bottom, top);
		break;
	case BLEND_SCREEN:
		result = BlendScreen(bottom, top);
		break;
	case BLEND_OVERLAY:
		result = BlendOverlay(bottom, top);
		break;
	case BLEND_COLOR_DOGE:
		result = BlendDodge(bottom, top);
		break;
	case BLEND_COLOR_BURN:
		result = BlendBurn(bottom, top);
		break;
	case BLEND_DIFFERENCE:
		result = BlendDifference(bottom, top);
		break;
	case BLEND_DARKEN:
		result = BlendDarken(bottom, top);
		break;
	case BLEND_LIGHTEN:
		result = BlendLighten(bottom, top);
		break;
	}

	return result;
}
Color4f BlendNormal(Color4f bottom, Color4f top)
{
	return top;
}
Color4f BlendAdd(Color4f bottom, Color4f top)
{
	return bottom + top;
}
Color4f BlendSubtract(Color4f bottom, Color4f top)
{
	return bottom - top;
}
Color4f BlendMultiply(Color4f bottom, Color4f top)
{
	return bottom * top;
}
Color4f BlendDivide(Color4f bottom, Color4f top)
{
	return bottom / top;
}
Color4f BlendScreen(Color4f bottom, Color4f top)
{
	return ~(~bottom * ~top);
}
Color4f BlendOverlay(Color4f bottom, Color4f top)
{
	Color4f c1 = (bottom * top) * 2.0f;
	Color4f c2 = ~((~bottom * ~top) * 2.0f);
	return Color4f(bottom.R < 0.5f ? c1.R : c2.R,
		bottom.G < 0.5f ? c1.G : c2.G,
		bottom.B < 0.5f ? c1.B : c2.B,
		bottom.A < 0.5f ? c1.A : c2.A);
}
Color4f BlendDodge(Color4f bottom, Color4f top)
{
	return bottom / ~top;
}
Color4f BlendBurn(Color4f bottom, Color4f top)
{
	return ~((~bottom) / top);
}
Color4f BlendDifference(Color4f bottom, Color4f top)
{
	Color4f c;
	c.R = bottom.R < top.R ? top.R - bottom.R : bottom.R - top.R;
	c.G = bottom.G < top.G ? top.G - bottom.G : bottom.G - top.G;
	c.B = bottom.B < top.B ? top.B - bottom.B : bottom.B - top.B;
	c.A = bottom.A < top.A ? top.A - bottom.A : bottom.A - top.A;
	return c;
}
Color4f BlendDarken(Color4f bottom, Color4f top)
{
	return Color4f(min(bottom.R, top.R), min(bottom.G, top.G), min(bottom.B, top.B), min(bottom.A, top.A));
}
Color4f BlendLighten(Color4f bottom, Color4f top)
{
	return Color4f(max(bottom.R, top.R), max(bottom.G, top.G), max(bottom.B, top.B), max(bottom.A, top.A));
}

	
struct hsv
{
	real32 h;
	real32 s;
	real32 v;
};

hsv rgb2hsv(Color4f in)
{
	hsv out;
	real32 min, max, delta;

	min = in.R < in.G ? in.R : in.G;
	min = min < in.B ? min : in.B;

	max = in.R > in.G ? in.R : in.G;
	max = min > in.B ? min : in.B;

	out.v = max;
	delta = max - min;
	if (max > 0.0) {
		out.s = (delta / max);
	}
	else {
		out.s = 0.0;
		out.h = NAN;
		return out;
	}
	if (in.R >= max)
		out.h = (in.G - in.B) / delta;
	else if (in.G >= max)
		out.h = 2.0f + (in.B - in.R) / delta;
	else
		out.h = 4.0f + (in.R - in.G) / delta;

	out.h *= 60.0f;

	if (out.h < 0.0)
		out.h += 360.0f;

	return out;
}
Color4f hsv2rgb(hsv in)
{
	Color4f out;
	real32 hh, p, q, t, ff;
	int32 i;

	if (in.s <= 0.0f) {  
		out.R = in.v;
		out.G = in.v;
		out.B = in.v;
		return out;
	}

	hh = in.h;
	if (hh >= 360.0f) hh = 0.0f;
	hh /= 60.0f;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0f - in.s);
	q = in.v * (1.0f - (in.s * ff));
	t = in.v * (1.0f - (in.s * (1.0f - ff)));

	switch (i) {
	case 0:
		out.R = in.v;
		out.G = t;
		out.B = p;
		break;
	case 1:
		out.R = q;
		out.G = in.v;
		out.B = p;
		break;
	case 2:
		out.R = p;
		out.G = in.v;
		out.B = t;
		break;

	case 3:
		out.R = p;
		out.G = q;
		out.B = in.v;
		break;
	case 4:
		out.R = t;
		out.G = p;
		out.B = in.v;
		break;
	case 5:
	default:
		out.R = in.v;
		out.G = p;
		out.B = q;
		break;
	}
	return out;
}

Color4f LerpColor(Color4f min_color, Color4f max_color, real32 step)
{
	//hsv ca = rgb2hsv(min_color);
	//hsv cb = rgb2hsv(max_color);
	//hsv result;

	//result.h = ca.h * (1 - step) + cb.h * step;
	//result.s = ca.s * (1 - step) + cb.s * step;
	//result.v = ca.v * (1 - step) + cb.v * step;

	//return hsv2rgb(result);

	return lerp(min_color, max_color, step);
}

const Color4f Color4f::RED = Color4f(1.0f, 0.0f, 0.0f, 0.1f);
const Color4f Color4f::GREEN = Color4f(0.0f, 1.0f, 0.0f, 0.1f);
const Color4f Color4f::BLUE = Color4f(0.0f, 0.0f, 1.0f, 0.1f);
const Color4f Color4f::BLACK = Color4f(0.0f, 0.0f, 0.0f, 0.1f);
const Color4f Color4f::CYAN = Color4f(0.0f, 1.0f, 1.0f, 0.1f);
const Color4f Color4f::MAGENTA = Color4f(1.0f, 0.0f, 1.0f, 0.1f);
const Color4f Color4f::YELLOW = Color4f(1.0f, 1.0f, 0.0f, 0.1f);
const Color4f Color4f::WHITE = Color4f(1.0f, 1.0f, 1.0f, 0.1f);