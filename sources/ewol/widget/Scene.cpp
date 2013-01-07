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

#define  WALK_FLAG_FORWARD  (1<<0)
#define  WALK_FLAG_BACK     (1<<1)
#define  WALK_FLAG_LEFT     (1<<2)
#define  WALK_FLAG_RIGHT    (1<<3)
#define  WALK_FLAG_CAUTION  (1<<4)


widget::Scene::Scene(game::Engine* gameEngine) :
	m_dynamicsWorld(NULL),
	m_camera(vec3(-6,0,2), vec3(0,0,0)),
	//m_gameEngine(gameEngine),
	m_isRunning(true),
	m_lastCallTime(-1),
	m_walk(0)
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

#define  WALK_FLAG_FORWARD  (1<<0)
#define  WALK_FLAG_BACK     (1<<1)
#define  WALK_FLAG_LEFT     (1<<2)
#define  WALK_FLAG_RIGHT    (1<<3)
#define  WALK_FLAG_CAUTION  (1<<4)

static const float l_walkRatio = 6.666f;
static const float l_walkLateralRatio = 3.333f;

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
	if (m_walk!=0) {
		if(    (m_walk&WALK_FLAG_FORWARD)!=0
		    && (m_walk&WALK_FLAG_BACK)!=0) {
			// request back and forward in the same time ... this is really bad ....
		} else if ( (m_walk&WALK_FLAG_FORWARD)!=0) {
			vec3 angles = m_camera.GetAngle();
			angles.x = cosf(angles.z);
			angles.y = -sinf(angles.z);
			angles.z = 0;
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			vec3 pos = m_camera.GetPosition();
			// walk is 6 km/h
			pos += angles*l_walkRatio*deltaTime;
			m_camera.SetPosition(pos);
		} else if ( (m_walk&WALK_FLAG_BACK)!=0) {
			vec3 angles = m_camera.GetAngle();
			angles.x = -cosf(angles.z);
			angles.y = sinf(angles.z);
			angles.z = 0;
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			vec3 pos = m_camera.GetPosition();
			// walk is 6 km/h
			pos += angles*l_walkRatio*deltaTime;
			m_camera.SetPosition(pos);
		}
		
		if(    (m_walk&WALK_FLAG_LEFT)!=0
		    && (m_walk&WALK_FLAG_RIGHT)!=0) {
			// request left and right in the same time ... this is really bad ....
		} else if ( (m_walk&WALK_FLAG_LEFT)!=0) {
			vec3 angles = m_camera.GetAngle();
			angles.x = cosf(angles.z-M_PI/2.0);
			angles.y = -sinf(angles.z-M_PI/2.0);
			angles.z = 0;
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			vec3 pos = m_camera.GetPosition();
			// lateral walk is 4 km/h
			pos += angles*l_walkLateralRatio*deltaTime;
			m_camera.SetPosition(pos);
		} else if ( (m_walk&WALK_FLAG_RIGHT)!=0) {
			vec3 angles = m_camera.GetAngle();
			angles.x = -cosf(angles.z-M_PI/2.0);
			angles.y = sinf(angles.z-M_PI/2.0);
			angles.z = 0;
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			vec3 pos = m_camera.GetPosition();
			// lateral walk is 4 km/h
			pos += angles*l_walkLateralRatio*deltaTime;
			m_camera.SetPosition(pos);
		}
	}
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
	mat4 tmpProjection = etk::matPerspective( M_PI/3.0, ratio, 1, 4000);
	ewol::openGL::SetCameraMatrix(m_camera.GetMatrix());
	//mat4 tmpMat = tmpProjection * m_camera.GetMatrix();
	// set internal matrix system :
	//ewol::openGL::SetMatrix(tmpMat);
	ewol::openGL::SetMatrix(tmpProjection);
	
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
	//EWOL_DEBUG("type : " << type << " IdInput=" << IdInput << " " << "status=" << statusEvent << " RelPos=" << relativePos);
	
	if (type == ewol::keyEvent::typeMouse) {
		if (0 != IdInput) {
			KeepFocus();
			GrabCursor();
			SetCursor(ewol::cursorNone);
		}
		if (true == GetGrabStatus() ) {
			if (ewol::keyEvent::statusMove == statusEvent) {
				pos *= M_PI/(360.0f*6);
				vec3 oldAngles = m_camera.GetAngle();
				oldAngles.z += pos.x;
				oldAngles.y += pos.y;
				m_camera.SetAngle(oldAngles);
			}
		}
	} else if (type == ewol::keyEvent::typeFinger) {
		KeepFocus();
	}
	// note : we did not parse the oether media ...

	return false;
}


bool widget::Scene::OnEventKb(ewol::keyEvent::status_te statusEvent, uniChar_t unicodeData)
{
	
	EWOL_DEBUG("KB EVENT : \"" << unicodeData << "\"" << "type=" << statusEvent);
	
	// escape case :
	if(unicodeData == 27) {
		if (statusEvent == ewol::keyEvent::statusDown) {
			UnGrabCursor();
			SetCursor(ewol::cursorArrow);
		}
		return false;
	}
	if (false == GetGrabStatus()) {
		GrabCursor();
		SetCursor(ewol::cursorNone);
	}
	if(    unicodeData == 'z'
	    || unicodeData == 'Z') {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_FORWARD;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_FORWARD) != 0) {
				m_walk -= WALK_FLAG_FORWARD;
			}
		}
	}
	if(    unicodeData == 's'
	    || unicodeData == 'S') {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_BACK;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_BACK) != 0) {
				m_walk -= WALK_FLAG_BACK;
			}
		}
	}
	if(    unicodeData == 'q'
	    || unicodeData == 'Q') {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_LEFT;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_LEFT) != 0) {
				m_walk -= WALK_FLAG_LEFT;
			}
		}
	}
	if(    unicodeData == 'd'
	    || unicodeData == 'D') {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_RIGHT;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_RIGHT) != 0) {
				m_walk -= WALK_FLAG_RIGHT;
			}
		}
	}
	EWOL_DEBUG("m_walk=" << m_walk);
	return false;
}


bool widget::Scene::OnEventKbMove(ewol::keyEvent::status_te statusEvent, ewol::keyEvent::keyboard_te specialKey)
{
	if (specialKey == ewol::keyEvent::keyboardUp) {
		EWOL_DEBUG("test ..." << specialKey << "  " << statusEvent);
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_FORWARD;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_FORWARD) != 0) {
				m_walk -= WALK_FLAG_FORWARD;
			}
		}
	}
	if (specialKey == ewol::keyEvent::keyboardDown) {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_BACK;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_BACK) != 0) {
				m_walk -= WALK_FLAG_BACK;
			}
		}
	}
	if (specialKey == ewol::keyEvent::keyboardLeft) {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_LEFT;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_LEFT) != 0) {
				m_walk -= WALK_FLAG_LEFT;
			}
		}
	}
	if (specialKey == ewol::keyEvent::keyboardRight) {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_RIGHT;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_RIGHT) != 0) {
				m_walk -= WALK_FLAG_RIGHT;
			}
		}
	}
	EWOL_DEBUG("m_walk=" << m_walk);
	return false;
}


void widget::Scene::OnGetFocus(void)
{
	//GrabCursor();
}

void widget::Scene::OnLostFocus(void)
{
	UnGrabCursor();
}


