/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/game/Camera.h>
#include <ewol/debug.h>


void game::Camera::Update(void)
{
	m_matrix.Identity();
	m_matrix.Rotate(vec3(0,0,1), M_PI/2.0 );
	m_matrix.Rotate(vec3(0,1,0), M_PI/2.0 );
	m_matrix.Rotate(vec3(1,0,0), m_angles.x );
	m_matrix.Rotate(vec3(0,1,0), m_angles.y );
	m_matrix.Rotate(vec3(0,0,1), m_angles.z );
	vec3 tmpp = m_position * -1;
	m_matrix.Translate(tmpp);
	m_needUpdate = false;
	//EWOL_DEBUG("camera: pos=" << m_position << " angle=" << m_angles);
}

game::Camera::Camera(vec3 pos, vec3 angles) :
	m_position(pos),
	m_angles(angles)
{
	m_needUpdate = true;
}

void game::Camera::SetPosition(vec3 pos)
{
	m_needUpdate = true;
	m_position = pos;
}

void game::Camera::SetAngle(vec3 angles)
{
	m_needUpdate = true;
	m_angles = angles;
}


mat4& game::Camera::GetMatrix(void)
{
	if(m_needUpdate==true) {
		Update();
	}
	return m_matrix;
};
