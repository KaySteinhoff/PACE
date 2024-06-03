#include <PACE.h>

int LoadFont(const chare *path, const char *key)
{
	PAFont *font = malloc(sizeof(PAFont));

	if(!font)
		return 1;

	if(FT_Init_FreeType(&font->fl))
		return 2;

	if(FT_New_Face(font->fl, path, 0, &font->ff))
		return 3;

	if(FT_Load_char(font->ff, '0', FT_LOAD_RENDER))
		return -1;

	font->chars = malloc(128*sizeof(struct Character));

	if(!font->chars)
		return 1;

	GLuint textures[128];
	glGenTextures(128, &textures[0]);
	for(unsigned char c = 0; c < 128; ++c)
	{
		if(FT_Load_Char(font->ff, c, FT_LOAD_RENDER))
		{
			printf("Failed to load character %c!\n", c);
			continue;
		}

		glBindTexture(GL_TEXTURE_2D, textures[c]);
		glTexImage2d(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			font->ff.glyph->bitmap.width,
			font->ff.glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			font->ff.glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		font->chars[c].texture = textures[c];
		font->chars[c].sx = font->ff.glyph->bitmap.width;
		font->chars[c].sy = font->ff.glyph->bitmap.rows;
		font->chars[c].bx = font->ff.glyph->bitmap_left;
		font->chars[c].by = font->ff.glyph->bitmap_top;
		font->chars[c].advance = font->ff.glyph->advance.x;
	}

	FT_Done_Face(font->ff);
	FT_Done_FreeType(font->fl);
}
