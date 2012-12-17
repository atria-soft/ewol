/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_BOUNDING_PLANE_H__
#define __GAME_BOUNDING_PLANE_H__

#include "ewol/game/Bounding.h"
#include "etk/math/Vector3D.h"

namespace game
{
	class BoundingPlane : public Bounding
	{
		private :
			vec3  m_equation;
		public:
			/**
			 * @biref Main constructor.
			 * @param[in] mode Bounding mode.
			 */
			BoundingPlane(void);
			/**
			 * @biref Main constructor.
			 */
			virtual ~BoundingPlane(void);
			/**
			 * @brief Update Bounding properties.
			 */
			virtual void Update(game::MeshObject& object, game::MeshProperty& property);
	};
}


#endif
