/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */



#include <ewol/game/Element.h>
#include <ewol/renderer/ResourceManager.h>

static int32_t uniqueId = 0;


game::Element::Element(etk::UString meshResource) :
	m_resource(NULL),
	m_bounding(NULL),
	m_matrixNeedUpdate(true),
	m_scale(1,1,1),
	m_mass(0.0f),
	m_uniqueId(uniqueId),
	m_groupId(0),
	m_type(0),
	m_visible(true)
{
	ewol::MeshObj* tmpObject = NULL;
	// get a resources :
	if (meshResource != "") {
		ewol::resource::Keep(meshResource, tmpObject);
		m_resource = tmpObject;
	}
	uniqueId++;
	m_bounding = game::CreateBounding(game::BoundingModeAABB);
}

game::Element::~Element(void)
{
	if (NULL != m_resource) {
		ewol::MeshObj* tmpObject = static_cast<ewol::MeshObj*>(m_resource);
		ewol::resource::Release(tmpObject);
		m_resource = NULL;
	}
	if (NULL != m_bounding) {
		delete(m_bounding);
		m_bounding = NULL;
	}
}

void game::Element::Draw(void)
{
	if (NULL != m_resource) {
		//EWOL_DEBUG("draw " << m_uniqueId);
		m_resource->Draw(GetMatrix());
	}
}

void game::Element::DrawDebug(void)
{
	if (NULL != m_bounding) {
		//EWOL_DEBUG("draw bounding" << m_uniqueId);
		m_bounding->Draw();
	}
}


bool game::Element::ArtificialIntelligence(int32_t deltaMicroSecond)
{
	return false;
}


void game::Element::ProcessGravityClear(void)
{
	m_gravityForce = vec3(0,0,0);
}


void game::Element::ProcessGravity(float delta, game::Gravity& gravity)
{
	if (0==m_mass) {
		return;
	}
	if (gravity.m_uniform==true) {
		m_gravityForce += gravity.m_vect;//*m_mass;
	} else {
		vec3 vectSeparation = m_position - gravity.m_vect;
		float squareDistance = vectSeparation.GetSquaredLength();
		// calculate gravity equation : 
		float force = game::GravityConst * gravity.m_mass * m_mass / squareDistance;
		vectSeparation.Normalize();
		// generate the force vector
		vectSeparation *= force;
		m_gravityForce += vectSeparation;
	}
}


void game::Element::ProcessPosition(float delta)
{
	vec3 m_speed0(m_speed);
	vec3 curentAcceleration(m_gravityForce + m_userAcceleration);
	m_speed += curentAcceleration*delta;
	vec3 tmpPos = m_position +m_speed0*delta + curentAcceleration*delta*delta/2.0f ;
	if (m_position != tmpPos) {
		m_position = tmpPos;
		m_matrixNeedUpdate = true;
		//EWOL_DEBUG("modify m_position=" << m_position << "m   m_speed=" << m_speed << "m/s m_gravityForce=" << m_gravityForce << "+" << m_userAcceleration << "m/s2");
	}
}

