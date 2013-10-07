/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Mesh.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/resources/ResourceManager.h>


extern const char * const ewolEventMeshPressed    = "ewol-mesh-Pressed";

#undef __class__
#define __class__	"Mesh"



widget::Mesh::Mesh(const etk::UString& filename) :
	m_meshName(filename),
	m_object(NULL),
	m_position(0,0,0),
	m_angle(0,0,0),
	m_angleSpeed(0,0,0),
	m_cameraDistance(10.0)
{
	addEventId(ewolEventMeshPressed);
	m_meshName = filename;
	// Limit event at 1:
	setMouseLimit(1);
	if (filename!="") {
		m_object = ewol::Mesh::keep(m_meshName);
		if (NULL == m_object) {
			EWOL_ERROR("Can not load the resource : \"" << m_meshName << "\"");
		}
	}
}


widget::Mesh::~Mesh(void)
{
	ewol::Mesh::release(m_object);
}
void widget::Mesh::onDraw(void)
{
	mat4 transformationMatrix =   etk::matTranslate(vec3(0,0,-m_cameraDistance))
	                            * etk::matTranslate(m_position)
	                            * etk::matRotate(vec3(1,0,0),m_angle.x())
	                            * etk::matRotate(vec3(0,1,0),m_angle.y())
	                            * etk::matRotate(vec3(0,0,1),m_angle.z());
	if (NULL != m_object) {
		m_object->draw(transformationMatrix);
	}
}

void widget::Mesh::systemDraw(const ewol::drawProperty& displayProp)
{
	ewol::openGL::push();
	// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
	glViewport( m_origin.x(),
	            m_origin.y(),
	            m_size.x(),
	            m_size.y());
	float ratio = m_size.x() / m_size.y();
	//EWOL_INFO("ratio : " << ratio);
	mat4 tmpProjection = etk::matPerspective(M_PI/3.0, ratio, 0.5, 100);
	//mat4 tmpMat = tmpProjection * m_camera.getMatrix();
	// set internal matrix system :
	//ewol::openGL::setMatrix(tmpMat);
	ewol::openGL::setMatrix(tmpProjection);
	
	onDraw();
	ewol::openGL::pop();
}

void widget::Mesh::onRegenerateDisplay(void)
{
	if (true == needRedraw()) {
		
	}
}

void widget::Mesh::periodicCall(const ewol::EventTime& _event)
{
	m_angle += m_angleSpeed*_event.getDeltaCall();
	markToRedraw();
}

bool widget::Mesh::onEventInput(const ewol::EventInput& _event)
{
	//EWOL_DEBUG("Event on BT ...");
	if (1 == _event.getId()) {
		if(ewol::keyEvent::statusSingle == _event.getStatus()) {
			generateEventId(ewolEventMeshPressed);
			return true;
		}
	}
	return false;
}

void widget::Mesh::setFile(const etk::UString& _filename)
{
	if(    _filename!=""
	    && m_meshName != _filename ) {
		ewol::Mesh::release(m_object);
		m_meshName = _filename;
		m_object = ewol::Mesh::keep(m_meshName);
		if (NULL == m_object) {
			EWOL_ERROR("Can not load the resource : \"" << m_meshName << "\"");
		}
	}
	markToRedraw();
}

void widget::Mesh::setPosition(const vec3& pos)
{
	m_position = pos;
	markToRedraw();
}

void widget::Mesh::setAngle(const vec3& angle)
{
	m_angle = angle;
	markToRedraw();
}

void widget::Mesh::setAngleSpeed(const vec3& speed)
{
	if (speed!=vec3(0,0,0)) {
		periodicCallEnable();
	} else {
		periodicCallDisable();
	}
	m_angleSpeed = speed;
	markToRedraw();
}

void widget::Mesh::setDistance(float distance)
{
	m_cameraDistance = distance;
	markToRedraw();
}
