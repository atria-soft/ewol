/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Scene.h>
#include <math.h>
#include <ewol/renderer/openGL.h>
#include <etk/math/Matrix4.h>

#undef __class__
#define __class__	"Scene"

widget::Scene::Scene(game::Engine* gameEngine) :
	m_gameEngine(gameEngine),
	m_isRunning(true),
	m_lastCallTime(-1),
	modeMoving(0)
{
	SetCanHaveFocus(true);
	PeriodicCallSet(true);
	m_zoom = 1.0/1000.0;
	m_ratioTime = 1.0f;
}


widget::Scene::~Scene(void)
{
	
}


void widget::Scene::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
	}
}


void widget::Scene::Pause(void)
{
	m_isRunning = false;
}


void widget::Scene::Resume(void)
{
	m_isRunning = true;
}


void widget::Scene::PauseToggle(void)
{
	if(true==m_isRunning) {
		m_isRunning=false;
	} else {
		m_isRunning=true;
	}
}


void widget::Scene::OnDraw(ewol::DrawProperty& displayProp)
{
	if (NULL != m_gameEngine) {
		m_gameEngine->Draw(displayProp);
	}
}


void widget::Scene::PeriodicCall(int64_t localTime)
{
	double curentTime=(double)localTime/1000000.0;
	// small hack to change speed ...
	if (m_ratioTime != 1) {
		double ellapseTime = curentTime - m_lastCallTime;
		ellapseTime *= m_ratioTime;
		m_lastCallTime = curentTime - ellapseTime;
	}
	// First time : 
	if (-1 == m_lastCallTime) {
		m_lastCallTime = curentTime;
	}
	// check if the processing is availlable
	if (false == m_isRunning) {
		m_lastCallTime = curentTime;
		MarkToRedraw();
		return;
	}
	// cut the processing in small slot of time to prevent error in the real-time Display (Android call us between 30 to 60 fps)
	float deltaTime = (float) (curentTime - m_lastCallTime);
	//EWOL_DEBUG("Time: m_lastCallTime=" << m_lastCallTime << " deltaTime=" << deltaTime);
	if (NULL != m_gameEngine) {
		if (true == m_isRunning) {
			m_gameEngine->Process(m_lastCallTime, deltaTime);
		}
	}
	m_lastCallTime = curentTime;
	MarkToRedraw();
}


void widget::Scene::GenDraw(ewol::DrawProperty displayProp)
{
	ewol::openGL::Push();
	// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
	glViewport( m_origin.x,
	            m_origin.y,
	            m_size.x,
	            m_size.y);
	float ratio = m_size.x / m_size.y;
	//EWOL_INFO("ratio : " << ratio);
	mat4 tmpProjection;// = etk::matPerspective( M_PI/2.0, ratio, -1, 1);
	
	if (ratio >= 1.0) {
		tmpProjection = etk::matOrtho(-ratio, ratio, -1, 1, -1, 1);
	} else {
		ratio = 1.0/ratio;
		tmpProjection = etk::matOrtho(-1, 1, -ratio, ratio, -1, 1);
	}
	
	mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, m_zoom) );
	mat4 tmpMat = tmpProjection * tmpScale * m_camera.GetMatrix();
	// set internal matrix system :
	ewol::openGL::SetMatrix(tmpMat);
	
	// Call the widget drawing methode
	displayProp.m_origin = m_origin;
	displayProp.m_size = m_size;
	// Call the widget drawing methode
	OnDraw(displayProp);
	
	ewol::openGL::Pop();
}


vec2 widget::Scene::RelativePosition(vec2  pos)
{
	// Remove origin of the widget
	pos.x -= m_origin.x;
	pos.y -= m_origin.y;
	// move the position at the center (openGl system
	pos.x -= m_size.x/2;
	pos.y -= m_size.y/2;
	// scale the position with the ratio display of the screen
	float ratio = m_size.x / m_size.y;
	if (ratio >= 1.0) {
		pos.x /= m_size.x;
		pos.x *= ratio;
		pos.y /= m_size.y;
	} else {
		ratio = 1.0/ratio;
		pos.x /= m_size.x;
		pos.y /= m_size.y;
		pos.y *= ratio;
	}
	// integrate zoom
	pos.x /= m_zoom;
	pos.y /= m_zoom;
	// all the position are half the size due to the fact -1 --> 1
	pos.x *= 2;
	pos.y *= 2;
	
	return pos;
};


bool widget::Scene::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te statusEvent, vec2 pos)
{
	vec2 relativePos = RelativePosition(pos);
	//EWOL_DEBUG("type : " << type << " IdInput=" << IdInput << " " << "status=" << statusEvent << " RelPos=" << relativePos);
	
	if (type == ewol::keyEvent::typeMouse) {
		if (4 == IdInput && ewol::keyEvent::statusUp == statusEvent) {
			m_zoom *= 0.91;
		} else if (5 == IdInput && ewol::keyEvent::statusUp == statusEvent) {
			m_zoom *= 1.1;
		} else if (1 == IdInput) {
			if(modeMoving==1 && ewol::keyEvent::statusUp == statusEvent) {
				modeMoving = 0;
			} else if (modeMoving==0 && ewol::keyEvent::statusDown == statusEvent) {
				modeMoving = 1;
				oldCursorPos = relativePos;
			} else if (modeMoving==1 && ewol::keyEvent::statusMove == statusEvent) {
				vec2 offset = relativePos - oldCursorPos;
				vec3 oldPos = m_camera.GetPosition();
				oldPos.x += offset.x;
				oldPos.y += offset.y;
				m_camera.SetPosition(oldPos);
				oldCursorPos = relativePos;
			}
		} else if (3 == IdInput) {
			if(modeMoving==3 && ewol::keyEvent::statusUp == statusEvent) {
				modeMoving = 0;
			} else if (modeMoving==0 && ewol::keyEvent::statusDown == statusEvent) {
				modeMoving = 3;
				oldCursorPos = relativePos;
			} else if (modeMoving==3 && ewol::keyEvent::statusMove == statusEvent) {
				vec2 offset = relativePos - oldCursorPos;
				offset *= M_PI/(360.0f*6);
				vec3 oldAngles = m_camera.GetAngle();
				oldAngles.x -= offset.y;
				oldAngles.y += offset.x;
				m_camera.SetAngle(oldAngles);
				oldCursorPos = relativePos;
			}
		}
	} else if (type == ewol::keyEvent::typeFinger) {
		
	}
	// note : we did not parse the oether media ...

	return false;
}


bool widget::Scene::OnEventKb(ewol::keyEvent::status_te statusEvent, uniChar_t unicodeData)
{
	/*
	EWOL_DEBUG("KB EVENT : \"" << unicodeData << "\"" << "type=" << statusEvent);
	if (statusEvent == ewol::ewol::keyEvent::statusDown) {
		
	}
	*/
	return false;
}


bool widget::Scene::OnEventKbMove(ewol::keyEvent::status_te statusEvent, ewol::keyEvent::keyboard_te specialKey)
{
	/*
	if (statusEvent == ewol::ewol::keyEvent::statusDown) {
		MarkToRedraw();
	}
	return true;
	*/
	return false;
}

