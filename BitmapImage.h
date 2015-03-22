#ifndef BITMAP_IMAGE_H

class BitmapImage
{
public:
	BitmapImage(const char *file_path);
	~BitmapImage();

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

#define BITMAP_IMAGE_H
#endif