#ifndef COLOR4F_H


enum BlendMode
{
	BLEND_NORMAL,
	BLEND_ADD,
	BLEND_SUBTRACT,
	BLEND_MULTIPLY,
	BLEND_DIVIDE,
	BLEND_SCREEN,
	BLEND_OVERLAY,
	BLEND_COLOR_DOGE,
	BLEND_COLOR_BURN,
	BLEND_DIFFERENCE,
	BLEND_DARKEN,
	BLEND_LIGHTEN
};

typedef uint32 Color32;
class Color4f
{
public:
	Color4f(real32 r = 1.0f, real32 g = 1.0f, real32 b = 1.0f, real32 a = 0.0f);
	Color4f(uint32 c);

	Color32 toColor32() const;
	Color4f operator+=(const Color4f &top);
	Color4f operator-(const Color4f &top) const;
	Color4f operator*(const Color4f &top) const;
	Color4f operator/(const Color4f &top) const;
	Color4f operator*(real32 f) const;
	Color4f operator/(real32 f) const;
	Color4f operator~();

	friend Color4f operator+(Color4f bottom, const Color4f &top);

	real32 R, G, B, A;

	static const Color4f RED;
	static const Color4f GREEN;
	static const Color4f BLUE;
	static const Color4f BLACK;
	static const Color4f CYAN;
	static const Color4f MAGENTA;
	static const Color4f YELLOW;
	static const Color4f WHITE;
};

Color4f operator+(Color4f bottom, const Color4f &top);

char *GetBlendModeName(BlendMode blend_mode);
Color4f Blend(Color4f bottom, Color4f top, BlendMode blend_mode);
Color4f BlendNormal(Color4f bottom, Color4f top);
Color4f BlendAdd(Color4f bottom, Color4f top);
Color4f BlendSubtract(Color4f bottom, Color4f top);
Color4f BlendMultiply(Color4f bottom, Color4f top);
Color4f BlendDivide(Color4f bottom, Color4f top);
Color4f BlendScreen(Color4f bottom, Color4f top);
Color4f BlendOverlay(Color4f bottom, Color4f top);
Color4f BlendDodge(Color4f bottom, Color4f top);
Color4f BlendBurn(Color4f bottom, Color4f top);
Color4f BlendDifference(Color4f bottom, Color4f top);
Color4f BlendDarken(Color4f bottom, Color4f top);
Color4f BlendLighten(Color4f bottom, Color4f top);

Color4f LerpColor(Color4f min, Color4f max, real32 step);

#define COLOR4F_H
#endif