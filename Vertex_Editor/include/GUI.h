//
// Created by rodrigo on 29/04/2026.
//

#ifndef VERTEXEDITOR_GUI_H
#define VERTEXEDITOR_GUI_H
#include "ShaderHandler.h"
enum GUIState
{
	FACE_EDITING,
	VERTEX_EDITING,
	EDGE_EDITING
	
};
struct MainGUI_state
{
	bool isFaceSelectionActive = false;
	bool isVertexSelectionActive = false;
	bool isEdgeSelectionActive = false;
};

class GUI{

public:
	unsigned int currentState=FACE_EDITING;
	MainGUI_state main_state;
	void showMainWindowGUI();


};


#endif //VERTEXEDITOR_GUI_H