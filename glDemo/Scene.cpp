#include "Scene.h"
#include "GameObject.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "ArcballCamera.h"
#include "LightFactory.h"
#include "Light.h"
#include "ModelFactory.h"
#include "model.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObjectFactory.h"
#include <assert.h>
#include "Cube2.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	//TODO: We are being really naught and not deleting everything as we finish
	//what shoudl really go here and in similar places throughout the code base?
}

//tick all my Game Objects, lights and cameras
void Scene::Update(float _dt)
{
	//update all lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all cameras
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		Cube2* cube = dynamic_cast<Cube2*>(*it);
		if (cube)
		{
			cube->tick(_dt);
		}
		else
		{
			(*it)->Tick(_dt);
		}
		
	}
	m_useCamera->move(camW, camS, camA, camD, _dt);
}

void Scene::AddGameObject(GameObject* _new)
{
	m_GameObjects.push_back(_new);
}

//I want THAT Game Object by name
GameObject* Scene::GetGameObject(string _GOName)
{
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetName() == _GOName)
		{
			return (*it);
		}
	}
	printf("Unknown Game Object NAME : %s \n", _GOName.c_str());
	assert(0);
	return nullptr;
}

Camera* Scene::GetCamera(string _camName)
{
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		if ((*it)->GetName() == _camName)
		{
			return (*it);
		}
	}
	printf("Unknown Camera NAME : %s \n", _camName.c_str());
	assert(0);
	return nullptr;
}

Light* Scene::GetLight(string _lightName)
{
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		if ((*it)->GetName() == _lightName)
		{
			return (*it);
		}
	}
	printf("Unknown Light NAME : %s \n", _lightName.c_str());
	assert(0);
	return nullptr;
}

Texture* Scene::GetTexture(string _texName)
{
	for (list<Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		if ((*it)->GetName() == _texName)
		{
			return (*it);
		}
	}
	printf("Unknown Texture NAME : %s \n", _texName.c_str());
	assert(0);
	return nullptr;
}

Model* Scene::GetModel(string _modelName)
{
	for (list<Model*>::iterator it = m_Models.begin(); it != m_Models.end(); it++)
	{
		if ((*it)->GetName() == _modelName)
		{
			return (*it);
		}
	}
	printf("Unknown Model NAME : %s \n", _modelName.c_str());
	assert(0);
	return nullptr;
}

Shader* Scene::GetShader(string _shaderName)
{
	for (list<Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); it++)
	{
		if ((*it)->GetName() == _shaderName)
		{
			return (*it);
		}
	}
	printf("Unknown Shader NAME : %s \n", _shaderName.c_str());
	assert(0);
	return nullptr;
}


//Render Everything
void Scene::Render()
{
	//TODO: Set up for the Opaque Render Pass will go here
	//check out the example stuff back in main.cpp to see what needs setting up here
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++) 
	{
		if ((*it)->GetRP() & RP_OPAQUE)// TODO: note the bit-wise operation. Why?
		{

			
			
			//set shader program using
			GLuint SP = (*it)->GetShaderProg();
			glUseProgram(SP);


			//set up for uniform shader values for current camera
			ArcballCamera* arcballCam = dynamic_cast<ArcballCamera*>(m_useCamera);
			if (arcballCam == nullptr)
			{
				m_useCamera->SetRenderValues(SP);
			}
			else
			{
				if ((*it)->GetName() == "BEAST")
				{
					//arcballCam->_beastPos = (*it)->getPos();
					arcballCam->_pos = m_useCamera->GetPos();
				}
				arcballCam->setRenderValuesArcballCamera(SP);
			}


			//loop through setting up uniform shader values for anything else
			SetShaderUniforms(SP);


			
			Cube2* cube = dynamic_cast<Cube2*>(*it); 
			if (cube)
			{
				cube->preRender();
				cube->render();
			}
			else
			{

				//set any uniform shader values for the actual model
				(*it)->PreRender();
				//actually render the GameObject
				(*it)->Render();
			}
			
			
		}
	}
	//s_cube->render();
	//TODO: now do the same for RP_TRANSPARENT here
}

void Scene::SetShaderUniforms(GLuint _shaderprog)
{
	//everything needs to know about all the lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->SetRenderValues(_shaderprog);
	}

}

void Scene::Load(ifstream& _file)
{
	string dummy;

	//load Cameras
	_file >> dummy >> m_numCameras; _file.ignore(256, '\n');
	cout << "CAMERAS : " << m_numCameras << endl;
	for (int i = 0; i < m_numCameras; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Camera* newCam = CameraFactory::makeNewCam(type);

		ArcballCamera* abCam = dynamic_cast<ArcballCamera*>(newCam);
		if (abCam)
		{
			abCam->Load(_file); 
			abCam->loadArcball(_file);
		}
		else
		{
			newCam->Load(_file);
		}
		
		
		m_Cameras.push_back(newCam);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Lights
	_file >> dummy >> m_numLights; _file.ignore(256, '\n');
	cout << "LIGHTS : " << m_numLights << endl;
	for (int i = 0; i < m_numLights; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Light* newLight = LightFactory::makeNewLight(type);
		newLight->Load(_file);

		m_Lights.push_back(newLight);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Models
	_file >> dummy >> m_numModels; _file.ignore(256, '\n');
	cout << "MODELS : " << m_numModels << endl;
	for (int i = 0; i < m_numModels; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Model* newModel = ModelFactory::makeNewModel(type);
		newModel->Load(_file);

		m_Models.push_back(newModel);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Textures
	_file >> dummy >> m_numTextures; _file.ignore(256, '\n');
	cout << "TEXTURES : " << m_numTextures << endl;
	for (int i = 0; i < m_numTextures; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		m_Textures.push_back(new Texture(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Shaders
	_file >> dummy >> m_numShaders; _file.ignore(256, '\n');
	cout << "SHADERS : " << m_numShaders << endl;
	for (int i = 0; i < m_numShaders; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		m_Shaders.push_back(new Shader(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load GameObjects
	_file >> dummy >> m_numGameObjects; _file.ignore(256, '\n');
	cout << "GAMEOBJECTS : " << m_numGameObjects << endl;
	for (int i = 0; i < m_numGameObjects; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		GameObject* newGO = GameObjectFactory::makeNewGO(type);
		newGO->Load(_file);

		m_GameObjects.push_back(newGO);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}


}

void Scene::Init()
{
	//initial aspect ratio
	m_aspectRatio = 1.0f;
	//initialise all cameras
	//scene is passed down here to allow for linking of cameras to game objects
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
	{
		(*it)->Init(100, 100, this);// TODO: set correct screen sizes here

		//if a camera is called MAIN
		//this will be the starting camera used
		if ((*it)->GetName() == "MAIN")
		{
			m_useCamera = (*it);
			m_useCameraIndex = count;
		}
		count++;
	}

	//if no MAIN camera just use the first one
	if (!m_useCamera)
	{
		m_useCamera = (*m_Cameras.begin());
		m_useCameraIndex = 0;
	}

	//set up links between everything and GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Init(this);
	}

	//initialy set the camera aspect ratio
	//m_useCamera->setAspectRatio(m_aspectRatio);
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end();it++)
	{
		//(*it)->setAspectRatio(m_aspectRatio);
		ArcballCamera* abCam = dynamic_cast<ArcballCamera*>(*it); 
		if (abCam) 
		{
			abCam->setAspect(m_aspectRatio); 
		}
		else
		{
			(*it)->setAspectRatio(m_aspectRatio); 
		}
	}


	camW, camS, camA, camD = false;
}


void Scene::setAspectRatio(float m_newAspectRatio)		//use dynamic_cast to cast *it to a ArcballCamera then call the function to change aspect ratio :3
{
	//when the sapect ratio is changed this function is called to change the aspect ratio variable in the camera class
	
	//m_useCamera->setAspectRatio(m_newAspectRatio);
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end();it++) 
	{
		/*
		if (count <4)
		{
			(*it)->setAspectRatio(m_newAspectRatio);
		}
		count++;
		*/
		ArcballCamera* abCam = dynamic_cast<ArcballCamera*>(*it); 
		if (abCam) 
		{
			abCam->setAspect(m_newAspectRatio);
		}
		else
		{
			(*it)->setAspectRatio(m_newAspectRatio); 
		}
	}
}

void Scene::itterateCamera()
{
	m_useCameraIndex++;
	m_useCameraIndex %= m_Cameras.size();
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		if (m_useCameraIndex == count)
		{
			m_useCamera = *it;
			printf("Current use camera : camera %d\n", m_useCameraIndex);
		}
		count++;
	}
}

void Scene::iterateLookAt()
{
	int count = 0;
	m_lookAtIndex++;
	m_lookAtIndex %= m_GameObjects.size();

	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if (m_lookAtIndex == count)
		{
			m_useCamera->setLookAt((*it)->getPos());
		}
		count++;
	}

}
