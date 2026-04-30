//
// Created by rodrigo on 29/04/2026.
//

#include "../include/MouseHandling.h"

#include "glad/glad.h"
float quadVertices[] = {
	// positions
	// texCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,-1.0f, 0.0f, 0.0f,
	 1.0f,-1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	 1.0f,-1.0f, 1.0f, 0.0f,
	 1.0f, 1.0f, 1.0f, 1.0f
	};
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
	//it will create the frame buffer
	glGenFramebuffers(1,&color_buffer);
	glGenTextures(1,&color_texture_buffer);
	glGenRenderbuffers(1,&color_depth_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER,color_buffer);
	glBindTexture(GL_TEXTURE_2D,color_texture_buffer);
	//it's a texture that will cover the entire viewport
	glTexImage2D(GL_TEXTURE_2D,0,GL_R32UI,width,height,0,GL_RED_INTEGER,GL_UNSIGNED_BYTE,nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//binds the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,color_texture_buffer,0);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindRenderbuffer(GL_RENDERBUFFER,color_depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,color_depth_buffer);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" <<
		std::endl;
	}
	else {
		std::cout << "OK" << std::endl;
	}

}