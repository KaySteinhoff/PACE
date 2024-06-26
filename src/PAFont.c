#include <PACE.h>

PAFont* LoadFont(const char *path)
{
	PAFont *font = malloc(sizeof(PAFont));

	if(!font)
		return NULL;

	if(FT_Init_FreeType(&font->fl))
	{
		free(font);
		return NULL;
	}

	if(FT_New_Face(font->fl, path, 0, &font->ff))
	{
		free(font);
		return NULL;
	}

	FT_Set_Pixel_Sizes(font->ff, 0, 128);

	if(FT_Load_Char(font->ff, '0', FT_LOAD_RENDER))
	{
		free(font);
		return NULL;
	}

	font->chars = malloc(128*sizeof(struct Character));

	if(!font->chars)
	{
		free(font);
		return NULL;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for(unsigned char c = 0; c < 128; ++c)
	{
		if(FT_Load_Char(font->ff, c, FT_LOAD_RENDER))
		{
			printf("Failed to load character %c!\n", c);
			continue;
		}

		glGenTextures(1, &font->chars[c].texture);
		glBindTexture(GL_TEXTURE_2D, font->chars[c].texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			font->ff->glyph->bitmap.width,
			font->ff->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			font->ff->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		font->chars[c].sx = font->ff->glyph->bitmap.width;
		font->chars[c].sy = font->ff->glyph->bitmap.rows;
		font->chars[c].bx = font->ff->glyph->bitmap_left;
		font->chars[c].by = font->ff->glyph->bitmap_top;
		font->chars[c].advance = font->ff->glyph->advance.x;
	}

	FT_Done_Face(font->ff);
	FT_Done_FreeType(font->fl);

	return font;
}
