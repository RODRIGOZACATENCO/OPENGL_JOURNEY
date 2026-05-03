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
struct RenderInfo
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int edge_VAO;
	unsigned int edge_EBO;

	glm::mat4 model;
};

struct FramebufferInfo {
	unsigned int FBO;
	unsigned int texture;
	unsigned int DBO;
};


class MainWindow {
public:
	MainWindow(GLFWwindow *window) {
		this->window = window;
	}
	unsigned int SSBO;//buffer to send the selected faces to the shader

	unsigned int total_meshes=0;
	uint update_selected_buffer=0;//flag to update the selected buffer on the GPU when a new face is selected or deselected
	bool is_window_ready=false;//flag to run the setup only once
	int width,height;//window dimensions
	GLFWwindow *window;
	GUI *gui;
	FramebufferInfo color_picking_framebuffer_info;//framebuffer for color picking pass



	Shader *main_shader,*color_picking_shader;//shaders for the main render pass and the color picking pass
	Shader *vertex_pass_shader, *edge_pass_shader;//shader for the vertex selection pass
	Shader *main_pass_edge_shader;
	std::map<Mesh*,RenderInfo> mesh_to_render_info;//maps each mesh to its render info (VAO,VBO,EBO,model matrix)
	std::map<std::string,unsigned int> mesh_name_to_id;//maps the mesh ID used in the color picking pass to the mesh name used in the main render pass
	std::vector<Mesh*> meshes;
	std::vector<int> face_selection_array;//list of all selected faces IDs in order 
	std::vector<int> vertex_selection_array;//list of all selected vertex IDs in order
	std::vector<int> edge_selection_array;//list of all selected edge IDs in order
	void setupSelectionArrays(Mesh* mesh); 

	glm::mat4 view;
	glm::mat4 projection;


    void use();
    std::pair<int, int> getCursorPositionInViewport(GLFWwindow *window);



	void selectionBufferPass();
    void mainRenderPass();
	void VertexRenderPass();

	void FramebufferSetup();
	void meshRenderSetup();

	void updateFacesSelected(unsigned int face_id,unsigned int mesh_id);
	void updateVerticesSelected(unsigned int vertex_id,unsigned int mesh_id);
	void updateEdgesSelected(unsigned int edge_id,unsigned int mesh_id);
	void resetSelectionBuffer(unsigned int type);//type can be FACE_EDITING,VERTEX_EDITING or EDGE_EDITING

	void cleanup();

	void resizeFramebuffer(int width, int height);

	std::optional<std::tuple<unsigned int,unsigned int,unsigned int> >faceDetection();
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
		RenderInfo render_info;
		render_info.model=model;
		mesh_to_render_info[mesh]=render_info;//store the render info for the mesh
		meshes.push_back(mesh);
		mesh_name_to_id[name] = meshes.size() - 1;
	}
	void setModelMatrix(std::string name,glm::mat4 model) {
		unsigned int mesh_id=mesh_name_to_id[name];
		mesh_to_render_info[meshes[mesh_id]].model=model;

	}
private:

};


#endif //VERTEXEDITOR_MAINWINDOW_H