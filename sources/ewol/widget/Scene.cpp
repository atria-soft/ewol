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
	m_lastCallTime(-1)
{
	SetCanHaveFocus(true);
	PeriodicCallSet(true);
	m_zoom = 1.0;
}


widget::Scene::~Scene(void)
{
	
}


void widget::Scene::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		/*
		// clean elements
		for (int32_t iii=0; iii<m_sceneElement.animated.Size(); iii++) {
			if (NULL != m_sceneElement.animated[iii]) {
				m_sceneElement.animated[iii]->Clear();
			}
		}
		for (int32_t jjj=0; jjj<MAX_GROUP_NUMBER; jjj++) {
			for (int32_t iii=0; iii<m_sceneElement.listAnimatedElements[jjj].Size(); iii++) {
				if (NULL != m_sceneElement.listAnimatedElements[jjj][iii]) {
					// find an empty slot ...
					m_sceneElement.listAnimatedElements[jjj][iii]->Draw();
				}
			}
		}
		*/
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
	/*
	//EWOL_ERROR(" On draw : " << m_currentDrawId);
	// draw background :
	// TODO : ...
	if (NULL != m_sceneElement.background) {
		m_sceneElement.background->Draw();
	}
	//background
	// draw elements
	for (int32_t iii=0; iii<m_sceneElement.animated.Size(); iii++) {
		if (NULL != m_sceneElement.animated[iii]) {
			m_sceneElement.animated[iii]->Draw();
		}
	}
	*/
	if (NULL != m_gameEngine) {
		m_gameEngine->Draw(displayProp);
	}
}


void widget::Scene::PeriodicCall(int64_t localTime)
{
	// First time : 
	if (-1 == m_lastCallTime) {
		m_lastCallTime = localTime;
	}
	// check if the processing is availlable
	if (false == m_isRunning) {
		m_lastCallTime = localTime;
		MarkToRedraw();
		return;
	}
	// cut the processing in small slot of time to prevent error in the real-time Display (Android call us between 30 to 60 fps)
	int32_t deltaTime = (int32_t) (localTime - m_lastCallTime);
	if (NULL != m_gameEngine) {
		m_gameEngine->Process(m_lastCallTime, deltaTime);
	}
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
	if (true) {
		mat4 tmpTranslate = etk::matTranslate(vec3(-m_size.x/2, -m_size.y/2, -1.0f));
		mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, 1.0f));
		mat4 tmpProjection = etk::matPerspective(-m_size.x/2, m_size.x/2, -m_size.y/2, m_size.y/2, -1, 1);
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		ewol::openGL::SetMatrix(tmpMat);
		// Call the widget drawing methode
		displayProp.m_origin = m_origin;
		displayProp.m_size = m_size;
	} else {
		m_zoom = 1.0/1000.0;
		//EWOL_INFO("ratio : " << ratio);
		mat4 tmpProjection;
		
		if (ratio >= 1.0) {
			tmpProjection = etk::matPerspective(-ratio, ratio, -1, 1, -1, 1);
		} else {
			ratio = 1.0/ratio;
			tmpProjection = etk::matPerspective(-1, 1, -ratio, ratio, -1, 1);
		}
		mat4 tmpScale = etk::matScale(vec3(m_zoom, m_zoom, m_zoom) );
		mat4 tmpMat = tmpProjection * tmpScale;
		// set internal matrix system :
		ewol::openGL::SetMatrix(tmpMat);
		// Clear the screen with transparency ...
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
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

