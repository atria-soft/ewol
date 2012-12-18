/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_GRAVITY_H__
#define __GAME_GRAVITY_H__


#include <etk/types.h>
#include <etk/math/Vector3D.h>



namespace game
{
	class Gravity
	{
		public:
			bool  m_uniform; //!< The gravity is not a stelar object
			float m_mass;    //!< Masse of the stelar object.
			vec3  m_vect;    //!< Position of the stelar object or gravity(acceleration) force when it is uniform.
			Gravity(void);
			Gravity(bool uniform, float mass, vec3 vect);
	};
	extern const float GravityConst;
	// Earth
	extern const float earthMass;
	extern const float earthRadius;
	extern const Gravity gravityEarth;
	// Mars
	extern const float marsMass;
	extern const float marsRadius;
	extern const Gravity gravityMars;
	
};

#endif

