/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/game/Gravity.h>



game::Gravity::Gravity(void) :
	m_uniform(false),
	m_mass(0),
	m_vect(0,0,0)
{
	
}

game::Gravity::Gravity(bool uniform, float mass, vec3 vect) :
	m_uniform(uniform),
	m_mass(mass),
	m_vect(vect)
{
	
}

// gravity force is :
// F = G*m1*m2 / radius^2.

extern const float game::GravityConst = 0.0000000000667f;
// Earth
extern const float game::earthMass = 5973600000000000000000000.0f; // in kg
extern const float game::earthRadius = 6378137.0f; // in meter
extern const game::Gravity game::gravityEarth(true, earthMass, vec3(0, -(float)((GravityConst*earthMass)/(earthRadius*earthRadius)), 0) );
// Mars
extern const float game::marsMass = 25000000000000000.0f; // in kg
extern const float game::marsRadius = 3396200.0f; // in meter
extern const game::Gravity game::gravityMars(true, marsMass, vec3(0, -(float)((GravityConst*marsMass)/(marsRadius*marsRadius)), 0) );



