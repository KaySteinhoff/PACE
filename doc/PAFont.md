# PAFont

## Struct
```C
typedef struct PAFont
{
	FT_Library fl;
	FT_Face ff;
	FT_GlyshSlot fg;

	struct Character
	{
		GLuint texture;
		float sx, sy;
		float bx, by;
		unsigned int advance;
	}*chars;
}PAFont;
```

__Fields__
|Fieldname|Utility|
|---|---|
|fl|A FT_Library instance to manage the loaded font file|
|ff|A FT_Face instance to render font glyphs on load|
|fg|A FT_GlyphSlot to measure font glyphs on load|
|chars|A pointer to preloaded glyphs|

## Functions

### CreatePATexture

```C
unsigned int LoadFont(PAFont *font, const char *path);
```

__Arguments__

__Description__

__Example__

