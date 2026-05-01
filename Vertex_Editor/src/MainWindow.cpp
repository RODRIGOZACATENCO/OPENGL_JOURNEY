//
// Created by rodrigo on 30/04/2026.
//
#include "glad/glad.h"
#include "../include/MainWindow.h"
#include <GLFW/glfw3.h>

void MainWindow::mainRenderPass() {
	int width,height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);



}
/*Setup all the meshes RenderInfo to show on the screen */
void MainWindow::renderSetup() {
	for (auto mesh:meshes) {
		RenderInfo *ri=mesh.second.second;
		Mesh* m=mesh.second.first;
		glGenVertexArrays(1,&ri->VAO);
		glGenBuffers(1,&ri->VBO);
		glGenBuffers(1,&ri->EBO);
		glBindBuffer(GL_ARRAY_BUFFER,ri->VBO);
		glBufferData(GL_ARRAY_BUFFER,m->vertices.size()*sizeof(Vertex),m->vertices.data(),GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ri->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,m->render_indices.size()*sizeof(unsigned int),m->render_indices.data(),GL_STATIC_DRAW);

		glBindVertexArray(ri->VAO);
		glBindBuffer(GL_ARRAY_BUFFER,ri->VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ri->EBO);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,point));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		glBindVertexArray(0);
	}


}


