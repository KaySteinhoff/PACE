#include <PACE.h>

PAText* CreateText(int x, int y, const char *text, int fontSize, PAFont *font);

IPADraw newText(int x, int y, const char *text, int fontSize, PAFont *font)
{
	PAText *data = CreateText(x, y, text, fontSize, font);
	if(!data)
		return (IPADraw){ 0 };

	return (IPADraw){
		.typeTag = TYPE_TAG_PATEXT,
		.data = data
	};
}

void TextDraw(void *raw_data)
{
	PAText *this = (PAText*)raw_data;
	this->width = 0;
	this->height = 0;
	int tmpX = this->x;

	glUseProgram(this->shader->ID);
	glBindVertexArray(this->vao);

	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(this->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)GetInstance()->currentCamera->uiMatrix);
	glUniform3f(this->colorUniform, this->color[0], this->color[1], this->color[2]);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->vao);

	float verts[24] = {
		0, 0, 0, 0,
		0, 0, 0, 1,
		0, 0, 1, 1,

		0, 0, 0, 0,
		0, 0, 1, 1,
		0, 0, 1, 0
	};
	for(char *c = (char*)this->text; *c != '\0'; ++c)
	{
		struct Character ch = this->font->chars[(unsigned char)*c];

		float xpos = tmpX + ch.bx;
		float ypos = this->y - ch.by;

		//Triangle 1
		verts[0] = xpos;
		verts[1] = ypos;

		verts[4] = xpos;
		verts[5] = ypos + ch.sy;

		verts[8] = xpos + ch.sx;
		verts[9] = ypos + ch.sy;

		//Triangle 2
		verts[12] = xpos;
		verts[13] = ypos;

		verts[16] = xpos + ch.sx;
		verts[17] = ypos + ch.sy;

		verts[20] = xpos + ch.sx;
		verts[21] = ypos;

		glBindTexture(GL_TEXTURE_2D, ch.texture);
		glBindBuffer(GL_ARRAY_BUFFER, this->shader->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		tmpX += (ch.advance >> 6);
		this->width += (ch.advance >> 6);

		if(ch.sy > this->height)
			this->height = ch.sy;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

PAText* CreateText(int x, int y, const char *text, int fontSize, PAFont *font)
{
	PAText *textObj = malloc(sizeof(PAText));

	if(!textObj)
		return NULL;

	textObj->shader = CompileShader("#version 110\nattribute vec4 coord;varying vec2 texcoord;uniform mat4 perspective;void main(void){gl_Position = perspective* vec4(coord.xy, 0, 1);texcoord = coord.zw;}",
					"#version 110\nvarying vec2 texcoord;uniform sampler2D tex;uniform vec3 color;void main(){vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(tex, texcoord).r);gl_FragColor = vec4(color, 1.0) * sampled;}");
	if(!textObj->shader)
	{
		free(textObj);
		return NULL;
	}

	textObj->x = x;
	textObj->y = y;
	textObj->text = text;
	textObj->fontSize = fontSize;
	textObj->color[0] = 1;
	textObj->color[1] = 1;
	textObj->color[2] = 1;
	textObj->font = font;

	textObj->colorUniform = glGetUniformLocation(textObj->shader->ID, "color");

	glGenVertexArrays(1, &textObj->vao);
//	glGenBuffers(1, &textObj->vbo);

	glBindVertexArray(textObj->vao);
	glBindBuffer(GL_ARRAY_BUFFER, textObj->shader->vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return textObj;
}

void SetTextColor(PAText *obj, GLfloat r, GLfloat g, GLfloat b)
{
	obj->color[0] = r;
	obj->color[1] = g;
	obj->color[2] = b;
}
