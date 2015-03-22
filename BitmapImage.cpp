#include "Stdafx.h"
#include "BitmapImage.h"

uint32 texture_count = 0;

BitmapImage::BitmapImage(const char *file_path) : data(nullptr)
{
	char str[256] = "";
	FILE *pFile;
	BMP colors;

	Log("Openging file...\n");
	fopen_s(&pFile, file_path, "rb");
	if (pFile) {
		fseek(pFile, sizeof(short) * 9, SEEK_SET);
		fread(&width, sizeof(int), 1, pFile);
		fread(&height, sizeof(int), 1, pFile);
		fseek(pFile, sizeof(short), SEEK_CUR);
		fread(&bpp, sizeof(short), 1, pFile);
		fseek(pFile, sizeof(int) * 6, SEEK_CUR);

		sprintf_s(str, 256, "\tDimensions: %i x %i\nBPP: %i\n", width, height, bpp);
		Log(str);

		if (bpp <= 8)
			for (uint32 a = 0; a < bpp; a++)
				fseek(pFile, sizeof(int), SEEK_CUR);

		data = (uint8 *)malloc(width * height * 4);
		memset(data, 0, width * height * 4);

		sprintf_s(str, 256, "\tSize: %.2fKB\n", (width * height) / 1024.0f);
		Log(str);
		uint32 j = 0;
		for (uint32 i = 0; i < width * height; ++i) {
			fread(&colors, sizeof(BMP), 1, pFile);

			data[j + 0] = colors.r;
			data[j + 1] = colors.g;
			data[j + 2] = colors.b;
			data[j + 3] = 0;

			j += 4;
		}

		id = texture_count++;
		strcpy_s(m_Name, 64, file_path);

		Log("Clossing File...\n");
		fclose(pFile);
	}
	else {
		Log("***Error***\n");
		Log("File does not exist.\n");
	}
}
BitmapImage::~BitmapImage()
{
	Log("Freeing BitmapImage: ");
	Log(m_Name);
	Log("\n");

	if (data)
		free(data);
}

void BitmapImage::SetName(const char *name)
{
	strcpy_s(m_Name, 64, name);
}
char *BitmapImage::GetName()
{
	return m_Name;
}