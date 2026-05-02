// Created by rodrigo on 30/04/2026.
//

#ifndef VERTEXEDITOR_MAINWINDOW_H
#define VERTEXEDITOR_MAINWINDOW_H
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "GUI.h"

/*vertex editor main window
 *render pass that shows  the object
 *color picking pass that renders color ID's
 *gimball camera that rotates around the object
 *soft simple lighting around the object
*/


struct FramebufferInfo {
	unsigned int FBO;
	unsigned int texture;
	unsigned int DBO;
};


class MainWindow {
public:
	unsigned int SSBO;//buffer to send the selected faces to the shader

	unsigned int total_meshes=0;
	bool update_faces_selected=false;//flag to update the selected faces in the shader
	bool SetupRun=false;//flag to run the setup only once
	int width,height;//window dimensions
	GLFWwindow *window;
	GUI *gui;
	FramebufferInfo color_picking_framebuffer_info;//framebuffer for color picking pass



	Shader *shader,*color_picking_shader;//shaders for the main render pass and the color picking pass
	std::map<std::string,unsigned int> mesh_name_to_id;//maps the mesh ID used in the color picking pass to the mesh name used in the main render pass
	std::vector<Mesh*> meshes;
	std::vector<int> face_selection_array;//list of all selected faces IDs in order 
	glm::mat4 view;
	glm::mat4 projection;

	MainWindow(GLFWwindow *window) {
		this->window = window;
	}
    void use();
    std::pair<int, int> getCursorPositionInViewport(GLFWwindow *window);
    void mainRenderPass();
	void addFacesToSelectionArray(Mesh* mesh);
    void colorPickingPass();
	void FramebufferSetup();
	void meshRenderSetup();
	void updateFacesSelected(unsigned int face_id,unsigned int mesh_id);
	void cleanup();

	void resizeFramebuffer(int width, int height);
	void mouseCallback(GLFWwindow *window, int button, int action, int mods);
	std::tuple<unsigned int,unsigned int,unsigned int> faceDetection();
	void setWindow(GLFWwindow *window) {
		this->window = window;
	}

	void setGui(GUI *gui) {
		this->gui = gui;
	}
	void setViewMatrix(glm::mat4 viewm){
		this->view = viewm;
	}
	void setProjectionMatrix(glm::mat4 projectionm) {
		this->projection = projectionm;
	}
	void addMesh(Mesh *mesh,std::string name,glm::mat4 model) {
		mesh->render_info.model=model;
		meshes.push_back(mesh);
		mesh_name_to_id[name] = meshes.size() - 1;
	}
	void setModelMatrix(std::string name,glm::mat4 model) {
		meshes[mesh_name_to_id[name]]->render_info.model=model;

	}
private:

};


#endif //VERTEXEDITOR_MAINWINDOW_H