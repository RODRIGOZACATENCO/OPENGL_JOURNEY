// Created by rodrigo on 30/04/2026.
//

#ifndef VERTEXEDITOR_MAINWINDOW_H
#define VERTEXEDITOR_MAINWINDOW_H
#include <GLFW/glfw3.h>

#include "Mesh.h"

/*vertex editor main window
 *render pass that shows  the object
 *color picking pass that renders color ID's
 *gimball camera that rotates around the object
 *soft simple lighting around the object
*/

struct RenderInfo {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	unsigned int SSBO;//sends the selected faces to the shader
	glm::mat4 model;
};

struct FramebufferInfo {
	unsigned int FBO;
	unsigned int texture;
	unsigned int DBO;
};


class MainWindow {
public:
	bool update_faces_selected=false;
	bool SetupRun=false;
	int width,height;
	GLFWwindow *window;

	FramebufferInfo color_picking_framebuffer_info;
	Shader *shader,*color_picking_shader;
	std::map<std::string,std::pair<Mesh*,RenderInfo*>> meshes;
	glm::mat4 view;
	glm::mat4 projection;
	MainWindow(GLFWwindow *window) {
		this->window = window;
	}
    void use();
    void mainRenderPass();
	void colorPickingPass();
	void FramebufferSetup();
	void meshRenderSetup();
	unsigned int mouseDetection();//returns the ID of the clicked face, 0 if nothing was clicked
	void setWindow(GLFWwindow *window) {
		this->window = window;
	}
	void setViewMatrix(glm::mat4 viewm){
		this->view = viewm;
	}
	void setProjectionMatrix(glm::mat4 projectionm) {
		this->projection = projectionm;
	}
	void addMesh(Mesh *mesh,std::string name,glm::mat4 model) {
		RenderInfo *render_info=new RenderInfo();
		render_info->model=model;
		meshes.insert({name,{mesh,render_info}});
	}
	void setModelMatrix(std::string name,glm::mat4 model) {
		if (meshes.find(name) != meshes.end()) {
			meshes[name].second->model=model;
		}
		else {
			std::cout<<"Mesh "<<name<<" not found"<<std::endl;
		}
	}
private:

};


#endif //VERTEXEDITOR_MAINWINDOW_H