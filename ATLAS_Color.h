#ifndef ATLAS_COLOR_H

typedef uint32 Color32;
class Color
{
public:
	Color(real32 r = 1.0f, real32 g = 1.0f, real32 b = 1.0f, real32 a = 0.0f);

	Color32 toColor32() const;
	Color &operator+=(const Color &rhs);
	Color &operator-=(const Color &rhs);
	Color &operator*=(const Color &rhs);
	Color &operator/=(const Color &rhs);
	Color &operator*=(real32 rhs);
	Color &operator/=(real32 rhs);
	Color operator~();

	friend Color operator+(Color lhs, const Color &rhs);
	friend Color operator-(Color lhs, const Color &rhs);
	friend Color operator*(Color lhs, const Color &rhs);
	friend Color operator/(Color lhs, const Color &rhs);
	friend Color operator*(Color lhs, real32 f);
	friend Color operator/(Color lhs, real32 f);

	real32 R, G, B, A;

	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color BLACK;
	static const Color CYAN;
	static const Color MAGENTA;
	static const Color YELLOW;
	static const Color WHITE;

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
};

Color operator+(Color bottom, const Color &top);

char *GetBlendModeName(Color::BlendMode blend_mode);
Color Blend(Color bottom, Color top, Color::BlendMode blend_mode);
Color BlendNormal(Color bottom, Color top);
Color BlendAdd(Color bottom, Color top);
Color BlendSubtract(Color bottom, Color top);
Color BlendMultiply(Color bottom, Color top);
Color BlendDivide(Color bottom, Color top);
Color BlendScreen(Color bottom, Color top);
Color BlendOverlay(Color bottom, Color top);
Color BlendDodge(Color bottom, Color top);
Color BlendBurn(Color bottom, Color top);
Color BlendDifference(Color bottom, Color top);
Color BlendDarken(Color bottom, Color top);
Color BlendLighten(Color bottom, Color top);

Color LerpColor(Color min, Color max, real32 step);

#define ATLAS_COLOR_H
#endif