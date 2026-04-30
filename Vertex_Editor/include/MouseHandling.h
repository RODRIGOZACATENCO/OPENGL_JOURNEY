//
// Created by rodrigo on 29/04/2026.
//

#ifndef VERTEXEDITOR_MOUSEHANDLING_H
#define VERTEXEDITOR_MOUSEHANDLING_H
#include "ShaderHandler.h"


class MouseHandling {
public:
	float x_pos, y_pos;
	void colorKeyMapping();//color key mapping to select face
	MouseHandling(Shader *draw_shader,int width,int height)
	:shader(draw_shader) {
		setup(width,height);
	}
	void setup(int width,int height);
	void updateViewPortSize(int width,int height);
	Shader *shader;
	unsigned int color_buffer;
	unsigned int color_texture_buffer;
	unsigned int color_depth_buffer;

};



#endif //VERTEXEDITOR_MOUSEHANDLING_H