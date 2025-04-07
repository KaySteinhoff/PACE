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

### LoadFont

```C
unsigned int LoadFont(PAFont *font, const char *path);
```

__Arguments__
|Name|Utility|
|---|---|
|font|A pointer to the font that is to be loaded into|
|path|The path to the font file that is to be loaded|

__Description__<br>
Loads the given font file into the provided font object.<br>

__Example__
```C
PAFont arial = { 0 };
unsigned int err = 0;
if((err = LoadFont(&arial, "arial.ttf")))
	return err;
```
