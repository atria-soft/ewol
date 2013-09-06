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
	AddEventId(ewolEventMeshPressed);
	m_meshName = filename;
	// Limit event at 1:
	SetMouseLimit(1);
	if (filename!="") {
		m_object = ewol::Mesh::Keep(m_meshName);
		if (NULL == m_object) {
			EWOL_ERROR("Can not load the resource : \"" << m_meshName << "\"");
		}
	}
}


widget::Mesh::~Mesh(void)
{
	ewol::Mesh::Release(m_object);
}
void widget::Mesh::OnDraw(void)
{
	mat4 transformationMatrix =   etk::matTranslate(vec3(0,0,-m_cameraDistance))
	                            * etk::matTranslate(m_position)
	                            * etk::matRotate(vec3(1,0,0),m_angle.x())
	                            * etk::matRotate(vec3(0,1,0),m_angle.y())
	                            * etk::matRotate(vec3(0,0,1),m_angle.z());
	if (NULL != m_object) {
		m_object->Draw(transformationMatrix);
	}
}

void widget::Mesh::SystemDraw(const ewol::DrawProperty& displayProp)
{
	ewol::openGL::Push();
	// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
	glViewport( m_origin.x(),
	            m_origin.y(),
	            m_size.x(),
	            m_size.y());
	float ratio = m_size.x() / m_size.y();
	//EWOL_INFO("ratio : " << ratio);
	mat4 tmpProjection = etk::matPerspective(M_PI/3.0, ratio, 0.5, 100);
	//mat4 tmpMat = tmpProjection * m_camera.GetMatrix();
	// set internal matrix system :
	//ewol::openGL::SetMatrix(tmpMat);
	ewol::openGL::SetMatrix(tmpProjection);
	
	OnDraw();
	ewol::openGL::Pop();
}

void widget::Mesh::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
	}
}

void widget::Mesh::PeriodicCall(const ewol::EventTime& _event)
{
	m_angle += m_angleSpeed*_event.GetDeltaCall();
	MarkToRedraw();
}

bool widget::Mesh::OnEventInput(const ewol::EventInput& _event)
{
	//EWOL_DEBUG("Event on BT ...");
	if (1 == _event.GetId()) {
		if(ewol::keyEvent::statusSingle == _event.GetStatus()) {
			GenerateEventId(ewolEventMeshPressed);
			return true;
		}
	}
	return false;
}

void widget::Mesh::SetFile(const etk::UString& _filename)
{
	if(    _filename!=""
	    && m_meshName != _filename ) {
		ewol::Mesh::Release(m_object);
		m_meshName = _filename;
		m_object = ewol::Mesh::Keep(m_meshName);
		if (NULL == m_object) {
			EWOL_ERROR("Can not load the resource : \"" << m_meshName << "\"");
		}
	}
	MarkToRedraw();
}

void widget::Mesh::SetPosition(const vec3& pos)
{
	m_position = pos;
	MarkToRedraw();
}

void widget::Mesh::SetAngle(const vec3& angle)
{
	m_angle = angle;
	MarkToRedraw();
}

void widget::Mesh::SetAngleSpeed(const vec3& speed)
{
	if (speed!=vec3(0,0,0)) {
		PeriodicCallEnable();
	} else {
		PeriodicCallDisable();
	}
	m_angleSpeed = speed;
	MarkToRedraw();
}

void widget::Mesh::SetDistance(float distance)
{
	m_cameraDistance = distance;
	MarkToRedraw();
}
