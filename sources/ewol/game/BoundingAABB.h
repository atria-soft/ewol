/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_BOUNDING_AABB_H__
#define __GAME_BOUNDING_AABB_H__

#include "ewol/game/Bounding.h"
#include "etk/math/Vector3D.h"

namespace game
{
	class BoundingAABB : public Bounding
	{
		private :
			vec3    m_PointStart;
			vec3    m_PointStop;
		public:
			/**
			 * @biref Main constructor.
			 * @param[in] mode Bounding mode.
			 */
			BoundingAABB(void);
			/**
			 * @biref Main constructor.
			 */
			virtual ~BoundingAABB(void);
			/**
			 * @brief Update Bounding properties.
			 */
			virtual void Update(game::MeshObject& object);
	};
}


#endif
