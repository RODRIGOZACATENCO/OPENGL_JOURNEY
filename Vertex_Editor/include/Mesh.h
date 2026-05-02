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
	std::map<std::pair<int,int>,int> edge_lookup;
	std::vector<unsigned int>render_indices;
	std::vector<int> face_is_selected;

	void process_mesh(std::vector<float>*vertices, std::vector<int>*faces);
	void show_mesh_structure();

	Mesh(std::vector<float>*vertices, std::vector<int>*faces){
		process_mesh(vertices, faces);
		setupRenderIndices();
	}
	void updateFaceSelection(int face_index);

private:
	void setupRenderIndices();
	std::vector<glm::vec3> colors;
	glm::vec3 randomRGB();
};
#endif //VERTEXEDITOR_HALFEDGE_H