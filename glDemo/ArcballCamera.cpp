
#include "ArcballCamera.h"
#include "stringHelp.h"
#include "helper.h"

using namespace std;
using namespace glm;

//
// Private API
//

// update position, orientation and view matrices when camera rotation and radius is modified
void ArcballCamera::calculateDerivedValues() {

	const float theta_ = glm::radians<float>(m_theta);
	const float phi_ = glm::radians<float>(m_phi);

	// calculate position vector
	//_pos = glm::vec4(sinf(phi_) * cosf(-theta_) * m_radius, sinf(-theta_) * m_radius, cosf(phi_) * cosf(-theta_) * m_radius, 1.0f);

	// calculate orientation basis R
	//R = glm::eulerAngleY(phi_) * glm::eulerAngleX(theta_);
		
	// calculate view and projection transform matrices
	//m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0,-m_radius)) * glm::eulerAngleX(-theta_) * glm::eulerAngleY(-phi_);
	// 
	//vec3 camPos = glm::vec3(-m_phi, - m_theta, m_radius);// 
	
	//make a free cam using 

	float x = m_radius * sin(m_theta/500) * cos(m_phi/500);
	float y = m_radius * cos(m_theta/500);
	float z = m_radius * sin(m_theta/500) * sin(m_phi/500);
	glm::vec3 camPos = lookAt + glm::vec3(x, y, z);
	//camPos = camPos * glm::eulerAngleX(-theta_);//*glm::eulerAngleY(-phi_);


	m_viewMatrix = glm::lookAt(camPos, lookAt, vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::radians<float>(m_fovY), m_aspect, m_nearPlane, m_farPlane);
	//theta and phi used to rotate camera and calculate the viewmatrix use these to change the camera position as well so it will move based on where it is looking not the cardinal axies
	//add button to make camera look at other gameobjects
}

void ArcballCamera::breakPoint()
{
	printf("");
}
//
// Public method implementation
//

// ArcballCamera constructors

// initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)
ArcballCamera::ArcballCamera() {

	m_theta = 0.0f;
	m_phi = 0.0f;
	m_radius = 15.0f;

	m_fovY = 55.0f;
	m_aspect = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 500.0f;

	//F = ViewFrustum(55.0f, 1.0f, 0.1f, 500.0f);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
	
}



// create a camera with orientation <theta, phi> representing Euler angles specified in degrees and Euclidean distance 'init_radius' from the origin.  The frustum / viewplane projection coefficients are defined in init_fovy, specified in degrees spanning the entire vertical field of view angle, init_aspect (w/h ratio), init_nearPlane and init_farPlane.  If init_farPlane = 0.0 (as determined by equalf) then the resulting frustum represents an infinite perspective projection.  This is the default
ArcballCamera::ArcballCamera(float _theta, float _phi, float _radius, float _fovY, float _aspect, float _nearPlane, float _farPlane) {

	this->m_theta = _theta;
	this->m_phi = _phi;
	this->m_radius = std::max<float>(0.0f, _radius);

	this->m_fovY = _fovY;
	this->m_aspect = _aspect;
	this->m_nearPlane = _nearPlane;
	this->m_farPlane = _farPlane;

	//F = ViewFrustum(init_fovy, init_aspect, init_nearPlane, init_farPlane);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
}


#pragma region Accessor methods for stored values

// return the pivot rotation around the x axis (theta) in degrees
float ArcballCamera::getTheta() {

	return m_theta;
}

// return the pivot rotation around the y axis (phi) in degrees
float ArcballCamera::getPhi() {

	return m_phi;
}

void ArcballCamera::rotateCamera(float _dTheta, float _dPhi) {

	float temp_theta = m_theta; 

	m_theta += _dTheta; 
	m_phi += _dPhi;

	//if theta goes behond 0 or Pi*500 then it will revert theta so that it will stop it going directly above or below the object
	if (m_theta < 1 || m_theta > 1570)
	{
		m_theta = temp_theta;
	}

	

	calculateDerivedValues();
}

float ArcballCamera::getRadius() {

	return m_radius;
}

void ArcballCamera::scaleRadius(float _s) {

	m_radius *= _s;
	calculateDerivedValues();
}

void ArcballCamera::incrementRadius(float _i) {

	m_radius = std::max<float>(m_radius + _i, 0.0f);
	calculateDerivedValues();
}

float ArcballCamera::getFovY() {

	return m_fovY;
}

void ArcballCamera::setFovY(float _fovY) {

	this->m_fovY = _fovY;
	calculateDerivedValues();
}

float ArcballCamera::getAspect() {

	return m_aspect;
}

void ArcballCamera::setAspect(float _aspect) {

	this->m_aspect = _aspect;
	calculateDerivedValues();
}

float ArcballCamera::getNearPlaneDistance() {

	return m_nearPlane;
}

void ArcballCamera::setNearPlaneDistance(float _nearPlaneDistance) {

	this->m_nearPlane = _nearPlaneDistance;
	calculateDerivedValues();
}

float ArcballCamera::getFarPlaneDistance() {

	return m_farPlane;
}

void ArcballCamera::setFarPlaneDistance(float _farPlaneDistance) {

	this->m_farPlane = _farPlaneDistance;
	calculateDerivedValues();
}


void ArcballCamera::loadArcball(ifstream& _file)  
{
	//some valuse setup by base class camera

	//StringHelp::String(_file, "NAME", m_name);
	//StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	//StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
	
	//StringHelp::Float(_file, "FOV", m_fovY);
	//StringHelp::Float(_file, "NEAR", m_nearPlane);
	//StringHelp::Float(_file, "FAR", m_farPlane);

	//values setup by arcballCamera
	StringHelp::Float(_file, "THETA", m_theta);
	StringHelp::Float(_file, "PHI", m_phi);
	StringHelp::Float(_file, "RADIUS", m_radius);
	m_aspect = 1.0f;
	calculateDerivedValues();
}

void ArcballCamera::setRenderValuesArcballCamera(unsigned int _prog)
{
	
	
	mat4 cameraView = m_viewMatrix * translate(identity<mat4>(), _pos); 
	
	GLint loc;
	//matrix for the view transform
	if (Helper::SetUniformLocation(_prog, "viewMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)&cameraView);

	//matrix for the projection transform
	if (Helper::SetUniformLocation(_prog, "projMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)&(m_projectionMatrix));

	//the current camera is at this position
	if (Helper::SetUniformLocation(_prog, "camPos", &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetPos()));
	

}
#pragma endregion


#pragma region Accessor methods for derived values

// return the camera location in world coordinate space
//glm::vec4 ArcballCamera::getPosition() {
//
//	return cameraPos;
//}

// return a const reference to the camera's orientation matrix in world coordinate space
//glm::mat4 ArcballCamera::getOrientationBasis() {
//
//	return R;
//}

// return a const reference to the view transform matrix for the camera
glm::mat4 ArcballCamera::viewTransform() {

	return m_viewMatrix;
}

// return a const reference the projection transform for the camera
glm::mat4 ArcballCamera::projectionTransform() {

	return m_projectionMatrix;
}

void ArcballCamera::setLookAt(glm::vec3 newLookAt)
{
	lookAt = newLookAt;
	calculateDerivedValues();
	//m_theta = m_theta;
	//m_phi = m_phi;
}

void ArcballCamera::move(bool w, bool s, bool a, bool d, float _dt)
{
	if (freecam)
	{
		glm::vec3 folward = { m_radius *sin(m_theta / 500) * cos(m_phi / 500), m_radius *cos(m_theta / 500), m_radius* sin(m_theta / 500) * cos(m_phi / 500) };
		glm::vec3 right = glm::normalize(glm::cross(folward, glm::vec3(0.0f, 1.0f, 0.0f)));
		if (w)
		{
			m_pos = (m_pos + folward * movementSpeed);
		}
		if (s)
		{
			m_pos = (m_pos - folward * movementSpeed);
		}
		if (a)
		{
			m_pos = (m_pos - right * movementSpeed);
		}
		if (d)
		{
			m_pos = (m_pos + right * movementSpeed); 
		}
	}

	//theta is Y 
	//phi is X
}



#pragma endregion