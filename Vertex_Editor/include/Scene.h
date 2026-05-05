//
// Created by rodrigo on 04/05/2026.
//

#ifndef VERTEXEDITOR_SCENE_H
#define VERTEXEDITOR_SCENE_H
#include <map>
#include<glm/glm.hpp>

#include "GUI.h"
#include "Mesh.h"

//Scene class stores al relevant information of the meshes in the viewport, this includes
/*
 *all mesh data
 *model, view and projection matrices of the scene
 *render information of each mesh (VAO,VBO,EBO, etc )
 *
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
class Scene {
 	private:
	glm::mat4 view;
	glm::mat4 projection;
	std::vector<int> face_selection_array;//list of all selected faces IDs in order
	std::vector<int> vertex_selection_array;//list of all selected vertex IDs in order
	std::vector<int> edge_selection_array;//list of all selected edge IDs in order
	unsigned int total_meshes=0;
	std::map<Mesh*,RenderInfo> mesh_to_render_info;//maps each mesh to its render info (VAO,VBO,EBO,model matrix)
	std::map<std::string,unsigned int> mesh_name_to_id;//maps the mesh ID used in the color picking pass to the mesh name used in the main render pass
	std::vector<Mesh*> meshes;
	unsigned int selected_elements_ssbo = 0;//buffer to send the selected faces to the shader
	public:
	bool sceneIsReady(std::string* out_error=nullptr)const;
	Scene(glm::mat4 view,glm::mat4 projection) {
		this->view = view;
		this->projection = projection;
	}
	void meshArraysSetup(Mesh* mesh);

	void updateSelectionBuffer(GUIState state);

	//given the buffer type to erase, it will reset its array to all zeros and update the ssbo
	void resetSelectionBuffer(GUIState type);//type can be FACE_EDITING, VERTEX_EDITING or EDGE_EDITING

	void meshRenderSetup(Mesh* mesh);
/*when a mesh is added, we need to:
 *add its info
 *store the render_info in the map
 *store the mesh
 *store the mesh name in the map
 *set up all render information
 *update face,vertex and edge selection arrays
 *
 *update selected_elements_ssbo - size of the scene
 *
 */
	void addMesh(Mesh *mesh,std::string name,glm::mat4 model) {
		RenderInfo render_info;
		render_info.model=model;
		mesh_to_render_info[mesh]=render_info;//store the render info for the mesh
		meshes.push_back(mesh);
		mesh_name_to_id[name] = meshes.size() - 1;
		total_meshes++;
		meshRenderSetup(mesh);
		meshArraysSetup(mesh);		//update selection arrays and ssbo
	}
	void setModelMatrix(std::string name,glm::mat4 model) {
		unsigned int mesh_id=mesh_name_to_id[name];
		mesh_to_render_info[meshes[mesh_id]].model=model;
	}
	void setViewMatrix(glm::mat4 view){
		this->view = view;
	}
	glm::mat4 & getViewMatrix() {
		return view;
	}
	glm::mat4 &getProjectionMatrix() {
		return projection;
	}
	void setProjectionMatrix(glm::mat4 projection) {
		this->projection = projection;
	}
	std::vector<Mesh *>* getMeshes() {
		return &meshes;
	}
	RenderInfo getRenderInfo(Mesh *mesh) {
		return mesh_to_render_info[mesh];
	}
	std::vector<int>* getFaceSelectionArray() {
		return &face_selection_array;
	}
	std::vector<int>* getVertexSelectionArray() {
		return &vertex_selection_array;
	}
	std::vector<int>* getEdgeSelectionArray() {
		return &edge_selection_array;
	}
	void updateFacesSelected(unsigned int face_id,unsigned int mesh_id);
	void updateVerticesSelected(unsigned int vertex_id,unsigned int mesh_id);
	void updateEdgesSelected(unsigned int edge_id,unsigned int mesh_id);

	//cleans up all memory allocated on this class
	void cleanup();

};


#endif //VERTEXEDITOR_SCENE_H