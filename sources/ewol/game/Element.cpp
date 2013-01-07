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
	UpdateMatrix();
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
		//EWOL_DEBUG("[" << m_uniqueId << "]   ==> colisionDetection");
		return;
	}
	vec3 m_speed0(m_speed);
	vec3 curentAcceleration(m_gravityForce + m_userAcceleration);
	m_speed += curentAcceleration*delta;
	vec3 tmpPos = m_position +m_speed*delta;
	if (m_position != tmpPos) {
		//EWOL_DEBUG("[" << m_uniqueId << "] pos=" << m_position << "m ==>" << tmpPos << "m");
		// TODO : Detect collision of other elements ...
		vec3 size = m_bounding.Size();
		vec3 move = tmpPos - m_position;
		vec3 moveAbs = move;
		moveAbs.Abs();
		if(    (    0 < size.x
		         && moveAbs.x > size.x*0.75 )
		    || (    0 < size.y
		         && moveAbs.y > size.y*0.75 )
		    || (    0 < size.z
		         && moveAbs.z > size.z*0.75 )) {
			int32_t maxIterations = 0;
			// generate a subdivide channel to prevent tunel effects ...
			// Get the smallest axis to test : 
			if(    moveAbs.x >= moveAbs.y
			    && moveAbs.x >= moveAbs.z) {
				//Test on X :
				//estimate the number of subdivision needed in recursive mode :
				maxIterations = moveAbs.x/abs(size.x);
				//EWOL_DEBUG("[" << m_uniqueId << "] XXX move with dist=" << move << "m <" << size*0.75 << "m");
			} else if (    moveAbs.y >= moveAbs.x
			            && moveAbs.y >= moveAbs.z) {
				//Test on Y :
				//estimate the number of subdivision needed in recursive mode :
				maxIterations = moveAbs.y/abs(size.y);
				//EWOL_DEBUG("[" << m_uniqueId << "] YYY move with dist=" << move << "m <" << size*0.75 << "m");
			} else {
				//Test on Z :
				//estimate the number of subdivision needed in recursive mode :
				maxIterations = moveAbs.z/abs(size.z);
				//EWOL_DEBUG("[" << m_uniqueId << "] ZZZ move with dist=" << move << "m <" << size*0.75 << "m");
			}
			vec3 deltaMove = move/maxIterations;
			vec3 tmppppPos = m_position;
			game::BoundingAABB tmpBounding;
			
			// linear test :
			for (int32_t iii=0; iii<maxIterations-1 ; iii++) {
				tmppppPos+=deltaMove;
				mat4 transformMatrix =   etk::matTranslate(tmppppPos)
				                       * etk::matScale(m_scale)
				                       * m_displayRotation;
				tmpBounding.Update(m_resource->m_object, transformMatrix);
				if (true == m_engine->HasCollision(tmpBounding, this)) {
					tmpPos = tmppppPos;
					// the tunnel effect is catch ...
					EWOL_CRITICAL("[" << m_uniqueId << "] Tunel effect catch : (" << iii << "/" << maxIterations-1 << ") ==> " << tmppppPos);
					break;
				}
			}
		}
		//EWOL_DEBUG("[" << m_uniqueId << "] New pos:" << tmpPos << "m");
		vec3 moving = tmpPos - m_position;
		m_position = tmpPos;
		UpdateMatrix();
		m_engine->ObjectMove(this, moving);
	}
}

void game::Element::UpdateMatrix(void)
{
	m_matrix =   etk::matTranslate(m_position)
	           * etk::matScale(m_scale)
	           * m_displayRotation;
	m_bounding.Update(m_resource->m_object, m_matrix);
}

void game::Element::Identity(void)
{
	m_position = vec3(0,0,0);
	m_speed = vec3(0,0,0);
	m_displayRotation.Identity();
	m_matrix.Identity();
}

void game::Element::Translate(etk::Vector3D<float> vect)
{
	m_position += vect;
	UpdateMatrix();
}

void game::Element::Scale(etk::Vector3D<float> vect)
{
	m_scale = vect;
	UpdateMatrix();
}

void game::Element::Scale(float proportion)
{
	m_scale = vec3(proportion,proportion,proportion);
	UpdateMatrix();
}

void game::Element::Rotate(etk::Vector3D<float> vect, float angleRad)
{
	m_displayRotation = etk::matRotate(vect, angleRad) * m_displayRotation;
	UpdateMatrix();
}

mat4& game::Element::GetMatrix(void)
{
	return m_matrix;
}


void game::Element::CollisionDetected(game::Element* elementCollide, vec3& penetrate)
{
	m_position -= penetrate;
	UpdateMatrix();
}

