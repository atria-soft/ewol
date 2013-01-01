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
#include "etk/math/Matrix4.h"
#include "ewol/renderer/resources/Colored3DObject.h"

namespace game
{
	class BoundingAABB : public Bounding
	{
		private :
			vec3    m_PointStart;
			vec3    m_PointStop;
			#ifdef DEBUG
				ewol::Colored3DObject*    m_displayBounding;
				etk::Vector<vec3>         m_vertices;
			#endif
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
			// herited methodes
			virtual void Update(game::MeshObject& object, mat4& transformMatrix);
			// herited methodes
			virtual void Draw(void);
			// herited methodes
			virtual bool HasContact(game::Bounding& otherbounding);
	};
}


#endif
