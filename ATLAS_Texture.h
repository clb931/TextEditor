#ifndef ATLAS_TEXTURE_H

namespace ATLAS
{
	class Texture
	{
	public:
		Texture(const char *file_path);
		~Texture();

		void SetName(const char *name);
		char *GetName();

		uint8 *data;
		uint32 width;
		uint32 height;
		uint32 bpp;
		uint32 id;

	private:
		struct BMP{ uint8 b, g, r; };

		char m_Name[64];
	};
}

#define ATLAS_TEXTURE_H
#endif