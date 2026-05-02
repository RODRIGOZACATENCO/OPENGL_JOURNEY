//
// Created by rodrigo on 29/04/2026.
//

#include "../include/MouseHandling.h"

#include "glad/glad.h"

void MouseHandling::colorKeyMapping() {

}

void MouseHandling::updateViewPortSize(int width, int height) {
	glBindTexture(GL_TEXTURE_2D, color_texture_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind when done

	// 2. Bind and resize the depth renderbuffer attachment
	glBindRenderbuffer(GL_RENDERBUFFER, color_depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind when done
}

void MouseHandling::setup(int width,int height) {
	

}