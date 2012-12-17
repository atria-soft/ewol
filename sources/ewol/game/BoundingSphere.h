/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_BOUNDING_SPHERE_H__
#define __GAME_BOUNDING_SPHERE_H__

#include "ewol/game/Bounding.h"
#include "etk/math/Vector3D.h"

namespace game
{
	class BoundingSphere : public Bounding
	{
		private :
			vec3  m_position; //!< Position of the center of the bounding
			float m_radius;   //!< Radius of the Bounding
		public:
			/**
			 * @biref Main constructor.
			 * @param[in] mode Bounding mode.
			 */
			BoundingSphere(void);
			/**
			 * @biref Main constructor.
			 */
			virtual ~BoundingSphere(void);
			/**
			 * @brief Update Bounding properties.
			 */
			virtual void Update(game::MeshObject& object, game::MeshProperty& property);
	};
}


#endif
