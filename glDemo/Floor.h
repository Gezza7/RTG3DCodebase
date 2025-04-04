#pragma once

#include "core.h"
#include "GameObject.h"
class Model;

class Floor : public GameObject {

private:

	GLuint				m_numFaces = 0;
	GLuint				m_vao = 0;

	GLuint				m_vertexBuffer;
	GLuint				m_colourBuffer;
	GLuint				m_indexBuffer;
	

	string m_ShaderName, m_TexName, m_ModelName;

	GLuint m_texture;

	Model* m_model;


public:

	Floor();
	~Floor(); 

	void Load(ifstream& _file);
	void render();
	void tick(float _dt);
	virtual void preRender();
	virtual void Init(Scene* _scene);
};
