/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_BOUNDING_OBB_H__
#define __GAME_BOUNDING_OBB_H__

#include "ewol/game/Bounding.h"
#include "etk/math/Vector3D.h"

namespace game
{
	class BoundingOBB : public Bounding
	{
		private :
			vec3    m_PointCenter;
			vec3    m_radius;
			vec3    m_angle;
		public:
			/**
			 * @biref Main constructor.
			 * @param[in] mode Bounding mode.
			 */
			BoundingOBB(game::MeshObject& object, game::MeshProperty& property);
			/**
			 * @biref Main constructor.
			 */
			virtual ~BoundingOBB(void);
			/**
			 * @brief Update Bounding properties.
			 */
			virtual void Update(game::MeshObject& object, game::MeshProperty& property);
	};
}


#endif
