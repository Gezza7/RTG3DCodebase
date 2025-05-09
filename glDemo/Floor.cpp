#include "Floor.h"
#include "stringHelp.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "AIModel.h"

using namespace std;
using namespace glm;


// Example data for cube model






// Packed vertex buffer for cube
static float positionArray[] = {
	// Back face (Green)
	-1.0f,  1.0f, -1.0f, 1.0f,  // 4
	 1.0f,  1.0f, -1.0f, 1.0f,  // 5
	 1.0f, -1.0f, -1.0f, 1.0f,  // 6
	-1.0f, -1.0f, -1.0f, 1.0f,  // 7

	// Front face (Red)
	-1.0f,  1.0f,  1.0f, 1.0f,  // 0
	 1.0f,  1.0f,  1.0f, 1.0f,  // 1
	 1.0f, -1.0f,  1.0f, 1.0f,  // 2
	-1.0f, -1.0f,  1.0f, 1.0f,  // 3



	// Top face (Blue)
	-1.0f,  1.0f, -1.0f, 1.0f,  // 8
	 1.0f,  1.0f, -1.0f, 1.0f,  // 9
	 1.0f,  1.0f,  1.0f, 1.0f,  //10
	-1.0f,  1.0f,  1.0f, 1.0f,  //11

	// Bottom face (Yellow)
	-1.0f, -1.0f, -1.0f, 1.0f,  //12
	 1.0f, -1.0f, -1.0f, 1.0f,  //13
	 1.0f, -1.0f,  1.0f, 1.0f,  //14
	-1.0f, -1.0f,  1.0f, 1.0f,  //15

	// Right face (Cyan)
	 1.0f,  1.0f, -1.0f, 1.0f,  //16
	 1.0f,  1.0f,  1.0f, 1.0f,  //17
	 1.0f, -1.0f,  1.0f, 1.0f,  //18
	 1.0f, -1.0f, -1.0f, 1.0f,  //19

	 // Left face (Magenta)
	 -1.0f,  1.0f, -1.0f, 1.0f,  //20
	 -1.0f,  1.0f,  1.0f, 1.0f,  //21
	 -1.0f, -1.0f,  1.0f, 1.0f,  //22
	 -1.0f, -1.0f, -1.0f, 1.0f,  //23
};

static float texCoords[] = {
	// Back face (Green)
	 0.0f,  0.0f,  0.0f,  // 4
	 0.0f,  0.0f,  0.0f,  // 5
	 0.0f,  0.0f,  0.0f,  // 6
	 0.0f,  0.0f,  0.0f,  // 7

	// Front face (Red)
	 0.0f,  0.0f,  0.0f,   // 4
	 0.0f,  0.0f,  0.0f,   // 5
	 0.0f,  0.0f,  0.0f,   // 6
	 0.0f,  0.0f,  0.0f,   // 3



	// Top face (Blue)
	 1.0f,  1.0f,  1.0f,  // 8		9, 8, 11, 11, 10, 9, // Top (CCW)
	 0.5f,  0.5f,  0.5f,  // 9
	 0.0f,  0.0f,  0.0f,  //10
	 0.75f,  0.75f,  0.75f,  //11

	// Bottom face (Yellow)
	0.0f,  0.0f,  0.0f,  // 4
	 0.0f,  0.0f,  0.0f, // 5
	 0.0f,  0.0f,  0.0f,  // 6
	 0.0f,  0.0f,  0.0f,  //15

	// Right face (Cyan)
	 0.0f,  0.0f,  0.0f,  // 4
	 0.0f,  0.0f,  0.0f,  // 5
	 0.0f,  0.0f,  0.0f,  // 6
	 0.0f,  0.0f,  0.0f,  //19

	 // Left face (Magenta)
	 0.0f,  0.0f,  0.0f,   // 4
	 0.0f,  0.0f,  0.0f,  // 5
	 0.0f,  0.0f,  0.0f,  // 6
	 0.0f,  0.0f,  0.0f,  //23
};


// Packed colour buffer for principle axes model
static float colourArray[] = {
	// Front face (Red)
	1.0f, 0.0f, 0.0f, 1.0f, // Vertex 1
	1.0f, 0.0f, 0.0f, 1.0f, // Vertex 2
	1.0f, 0.0f, 0.0f, 1.0f, // Vertex 3
	1.0f, 0.0f, 0.0f, 1.0f, // Vertex 4

	// Back face (Green)
	0.0f, 1.0f, 0.0f, 1.0f, // Vertex 5
	0.0f, 1.0f, 0.0f, 1.0f, // Vertex 6
	0.0f, 1.0f, 0.0f, 1.0f, // Vertex 7
	0.0f, 1.0f, 0.0f, 1.0f, // Vertex 8

	// Top face (Blue)
	0.0f, 0.0f, 1.0f, 1.0f, // Vertex 1
	0.0f, 0.0f, 1.0f, 1.0f, // Vertex 2
	0.0f, 0.0f, 1.0f, 1.0f, // Vertex 6
	0.0f, 0.0f, 1.0f, 1.0f, // Vertex 5

	// Bottom face (Yellow)
	1.0f, 1.0f, 0.0f, 1.0f, // Vertex 4
	1.0f, 1.0f, 0.0f, 1.0f, // Vertex 3
	1.0f, 1.0f, 0.0f, 1.0f, // Vertex 7
	1.0f, 1.0f, 0.0f, 1.0f, // Vertex 8

	// Right face (Cyan)
	0.0f, 1.0f, 1.0f, 1.0f, // Vertex 2
	0.0f, 1.0f, 1.0f, 1.0f, // Vertex 6
	0.0f, 1.0f, 1.0f, 1.0f, // Vertex 7
	0.0f, 1.0f, 1.0f, 1.0f, // Vertex 3

	// Left face (Magenta)
	1.0f, 0.0f, 1.0f, 1.0f, // Vertex 1
	1.0f, 0.0f, 1.0f, 1.0f, // Vertex 5
	1.0f, 0.0f, 1.0f, 1.0f, // Vertex 8
	1.0f, 0.0f, 1.0f, 1.0f, // Vertex 4
};


// Line list topology to render principle axes
static unsigned int indexArray[] = {
	//different faces need different rotations because opengl will see CCW faces as front facing and CW as backfacing
	//both are needed for a cube to have faces viewable on each side
	0, 1, 2,  2, 3, 0,   // Front (CW)
	5, 4, 7,  7, 6, 5,   // Back (CCW)
	9, 8, 11, 11, 10, 9, // Top (CCW)
	12, 13, 14, 14, 15, 12, // Bottom (CW)
	16, 17, 18, 18, 19, 16, // Right (CW)
	21, 20, 23, 23, 22, 21  // Left (CCW)
};



Floor::Floor() {

	m_numFaces = 6 * 2;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// setup vbo for position attribute
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionArray), positionArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	// setup vbo for colour attribute
	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colourArray), colourArray, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(4);

	// setup vbo for cube) index buffer
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

	glBindVertexArray(0);
}


Floor::~Floor() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_colourBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}


void Floor::render() {

	


	glEnable(GL_TEXTURE_2D);
	glTexCoordPointer(3, GL_FLOAT, 0, texCoords);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_numFaces * 3, GL_UNSIGNED_INT, (const GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
	

	//glBindVertexArray(m_vao);
	//glDrawElements(GL_TRIANGLES, m_numFaces * 3, GL_UNSIGNED_INT, (const GLvoid*)0);

}

void Floor::Load(ifstream& _file)
{
	GameObject::Load(_file);
	StringHelp::String(_file, "MODEL", m_ModelName);
	StringHelp::String(_file, "TEXTURE", m_TexName);
	StringHelp::String(_file, "SHADER", m_ShaderName);

}

void Floor::tick(float _dt)
{
	GameObject::Tick(_dt);
}


void Floor::preRender()
{
	GameObject::PreRender();
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}


void Floor::Init(Scene* _scene)
{
	m_ShaderProg = _scene->GetShader(m_ShaderName)->GetProg();
	m_texture = _scene->GetTexture(m_TexName)->GetTexID();
	//m_model = _scene->GetModel(m_ModelName);
}
