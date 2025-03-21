#pragma once

#include "core.h"
#include "GameObject.h"

class Cube2 : public GameObject{

private:

	GLuint				m_numFaces = 0;
	GLuint				m_vao = 0;

	GLuint				m_vertexBuffer;
	GLuint				m_colourBuffer;
	GLuint				m_indexBuffer;

	string m_ShaderName, m_TexName, m_ModelName;  

	GLuint m_texture;  
	

public:

	Cube2();
	~Cube2();

	void Load(ifstream& _file);
	void render();
	void tick(float _dt);
	virtual void preRender();
	virtual void Init(Scene* _scene);
};
