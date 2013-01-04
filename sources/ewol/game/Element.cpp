/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */



#include <ewol/game/Element.h>
#include <ewol/game/Engine.h>
#include <ewol/renderer/ResourceManager.h>

static int32_t uniqueId = 0;


game::Element::Element(etk::UString meshResource) :
	m_engine(NULL),
	m_resource(NULL),
	m_matrixNeedUpdate(true),
	m_scale(1,1,1),
	m_speedMax(2000000, 2000000, 2000000),
	m_mass(0.0f),
	m_uniqueId(uniqueId),
	m_groupId(0),
	m_type(0),
	m_visible(true),
	m_static(false)
{
	ewol::MeshObj* tmpObject = NULL;
	// get a resources :
	if (meshResource != "") {
		ewol::resource::Keep(meshResource, tmpObject);
		m_resource = tmpObject;
	}
	uniqueId++;
}

game::Element::~Element(void)
{
	if (NULL != m_resource) {
		ewol::MeshObj* tmpObject = static_cast<ewol::MeshObj*>(m_resource);
		ewol::resource::Release(tmpObject);
		m_resource = NULL;
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
	//EWOL_DEBUG("draw bounding" << m_uniqueId);
	m_bounding.Draw();
}


bool game::Element::ArtificialIntelligence(float deltaMicroSecond)
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
	if(true == m_bounding.GetContactStatus()) {
		return;
	}
	vec3 m_speed0(m_speed);
	vec3 curentAcceleration(m_gravityForce + m_userAcceleration);
	m_speed += curentAcceleration*delta;
	vec3 tmpPos = m_position +m_speed*delta;
	if (m_position != tmpPos) {
		// TODO : Detect collision of other elements ...
		vec3 size = m_bounding.Size();
		vec3 move = tmpPos - m_position;
		vec3 moveAbs = move;
		moveAbs.Abs();
		if(    (    0 < size.x
		         && moveAbs.x > 2*size.x )
		    || (    0 < size.y
		         && moveAbs.y > 2*size.y )
		    || (    0 < size.z
		         && moveAbs.z > 2*size.z )) {
			int32_t maxIterations = 0;
			// generate a subdivide channel to prevent tunel effects ...
			// Get the smallest axis to test : 
			if(    moveAbs.x >= moveAbs.y
			    && moveAbs.x >= moveAbs.z) {
				//Test on X :
				//estimate the number of subdivision needed in recursive mode :
				maxIterations = moveAbs.x/abs(size.x);
				EWOL_DEBUG("XXX move with dist=" << move << "m <" << size*2 << "m");
			} else if (    moveAbs.y >= moveAbs.x
			            && moveAbs.y >= moveAbs.z) {
				//Test on Y :
				//estimate the number of subdivision needed in recursive mode :
				maxIterations = moveAbs.y/abs(size.y);
				EWOL_DEBUG("YYY move with dist=" << move << "m <" << size*2 << "m");
			} else {
				//Test on Z :
				//estimate the number of subdivision needed in recursive mode :
				maxIterations = moveAbs.z/abs(size.z);
				EWOL_DEBUG("ZZZ move with dist=" << move << "m <" << size*2 << "m");
			}
			vec3 deltaMove = move/maxIterations;
			vec3 tmppppPos = m_position;
			game::BoundingAABB tmpBounding;
			// linear test :
			for (int32_t iii=0; iii<maxIterations-1 ; iii++) {
				tmppppPos+=deltaMove;
				tmpBounding.Update(m_resource->m_object, m_displayRotation, tmppppPos, m_scale);
				if (true == m_engine->HasCollision(tmpBounding, this)) {
					tmpPos = tmppppPos;
					// the tunnel effect is catch ...
					EWOL_CRITICAL("Tunel effect catch : (" << iii << "/" << maxIterations-1 << ")");
					break;
				}
			}
		}
		m_position = tmpPos;
		m_matrixNeedUpdate = true;
	}
}

