#include <PACEErrorHandling.h>
#include <PACE.h>

extern PACE *instance;

unsigned int TYPE_TAG_PATEXT = 0;

float verts[24] = {
	0, 0, 0, 0,
	0, 0, 0, 1,
	0, 0, 1, 1,

	0, 0, 0, 0,
	0, 0, 1, 1,
	0, 0, 1, 0
};

IPADraw newText(PAText *text)
{
	if(!text)
		return (IPADraw){ 0 };

	return (IPADraw){
		.typeTag = TYPE_TAG_PATEXT,
		.visible = 1,
		.data = text
	};
}

void TextDraw(void *raw_data, mat4x4 perspective)
{
	PAText *this = (PAText*)raw_data;
	this->width = 0;
	this->height = 0;
	int tmpX = this->x;

	glUseProgram(this->shader.ID);
	glBindVertexArray(this->vao);

	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(this->shader.perspectiveLocation, 1, GL_FALSE, (const GLfloat*)instance->currentCamera->uiMatrix);
	glUniform3f(this->colorUniform, this->color[0], this->color[1], this->color[2]);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->vao);

	for(char *c = (char*)this->text; *c != '\0'; ++c)
	{
		struct Character ch = this->font->chars[(unsigned char)*c];

		float xpos = tmpX + ch.bx*(this->fontSize/100.0);
		float ypos = this->y - ch.by*(this->fontSize/100.0) + this->fontSize;

		//Triangle 1
		verts[0] = xpos;
		verts[1] = ypos;

		verts[4] = xpos;
		verts[5] = ypos + ch.sy*(this->fontSize/100.0);

		verts[8] = xpos + ch.sx*(this->fontSize/100.0);
		verts[9] = ypos + ch.sy*(this->fontSize/100.0);

		//Triangle 2
		verts[12] = xpos;
		verts[13] = ypos;

		verts[16] = xpos + ch.sx*(this->fontSize/100.0);
		verts[17] = ypos + ch.sy*(this->fontSize/100.0);

		verts[20] = xpos + ch.sx*(this->fontSize/100.0);
		verts[21] = ypos;

		glBindTexture(GL_TEXTURE_2D, ch.texture);
		glBindBuffer(GL_ARRAY_BUFFER, this->shader.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		tmpX += (ch.advance >> 6)*(this->fontSize/100.0);
		this->width += (ch.advance >> 6)*(this->fontSize/100.0);

		if(ch.sy*(this->fontSize/100.0) > this->height)
			this->height = ch.sy*(this->fontSize/100.0);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextCalculateSize(void *raw_data)
{
	PAText *this = (PAText*)raw_data;
	this->width = 0;
	this->height = 0;

	for(char *c = (char*)this->text; *c != '\0'; ++c)
	{
		struct Character ch = this->font->chars[(unsigned char)*c];
		this->width += (ch.advance >> 6)*(this->fontSize/100.0);
		if(ch.sy*(this->fontSize/100.0) > this->height)
			this->height = ch.sy*(this->fontSize/100.0);
	}
}

unsigned int CreatePAText(PAText *text, int x, int y, const char *txt, int fontSize, PAFont *font)
{
	if(!text || !txt)
		return PACE_ERR_NULL_REFERENCE;

	unsigned int err = 0;
	PAShader vertex = 0, fragment = 0;
	if((err = CompileShader(&vertex, GL_VERTEX_SHADER, "#version 110\nattribute vec4 coord;varying vec2 texcoord;uniform mat4 perspective;void main(void){gl_Position = perspective* vec4(coord.xy, 0, 1);texcoord = coord.zw;}")))
		return err;
	if((err = CompileShader(&fragment, GL_FRAGMENT_SHADER, "#version 110\nvarying vec2 texcoord;uniform sampler2D tex;uniform vec3 color;void main(){vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(tex, texcoord).r);gl_FragColor = vec4(color, 1.0) * sampled;}")))
		return err;
	if((err = CreatePAMaterial(&text->shader, 2, vertex, fragment)))
		return err;

	text->x = x;
	text->y = y;
	text->text = txt;
	text->fontSize = fontSize;
	text->color[0] = 1;
	text->color[1] = 1;
	text->color[2] = 1;
	text->font = font;

	text->colorUniform = glGetUniformLocation(text->shader.ID, "color");

	glGenVertexArrays(1, &text->vao);

	glBindVertexArray(text->vao);
	glBindBuffer(GL_ARRAY_BUFFER, text->shader.vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return PACE_ERR_SUCCESS;
}

void SetTextColor(PAText *obj, GLfloat r, GLfloat g, GLfloat b)
{
	obj->color[0] = r;
	obj->color[1] = g;
	obj->color[2] = b;
}
