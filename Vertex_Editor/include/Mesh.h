 //
// Created by rodrigo on 27/04/2026.
//

#ifndef VERTEXEDITOR_HALFEDGE_H
#define VERTEXEDITOR_HALFEDGE_H
#include <format>
#include <map>
#include<glm/glm.hpp>
#include<vector>
#include<deque>
#include "ShaderHandler.h"



struct HalfEdge;
struct Vertex {
	glm::vec3 point;
	int halfedge;
};

struct Edge {
	int halfedge;
};

struct Face {
	int halfedge;
};

struct HalfEdge {
	int next;//next half edge
	int prev;//previous half edge
	int twin;//reversed half edge
	int vertex;
	int edge;
	int face;
};
class Mesh {
public:
	std::vector<HalfEdge> half_edges;
	std::vector<Face> faces;
	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	std::map<std::pair<int,int>,int> edge_lookup;//maps a pair of vertex indices to the edge index, used to find twin half edges	
	std::vector<unsigned int>face_render_indices;//list of vertex indices in the order they should be rendered to form the faces

	std::vector<unsigned int>edge_render_indices;//list of vertex indices in the order they should be rendered to form the edges

	void process_mesh(std::vector<float>*vertices, std::vector<int>*faces);
	void show_mesh_structure();

	Mesh(std::vector<float>*vertices, std::vector<int>*faces){
		process_mesh(vertices, faces);
		setuFaceRenderIndices();
		setupEdgeRenderIndices();
	}
	void updateFaceSelection(int face_index);
	void setuFaceRenderIndices();
	void setupEdgeRenderIndices();

private:

	std::vector<glm::vec3> colors;
	glm::vec3 randomRGB();
};
#endif //VERTEXEDITOR_HALFEDGE_H