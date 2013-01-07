/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_BOUNDING_AABB_H__
#define __GAME_BOUNDING_AABB_H__

#include "etk/math/Vector3D.h"
#include "etk/math/Matrix4.h"
#include "ewol/renderer/resources/Colored3DObject.h"

namespace game
{
	class BoundingAABB
	{
		private :
			vec3    m_PointStart;
			vec3    m_PointStop;
			bool    m_hasContact;     //!< this bounding is on contact with something else ...
			vec3    m_oldUserPosition; // this is due to the fact object are never centered ...
			#ifdef DEBUG
				ewol::Colored3DObject*    m_displayBounding;
				etk::Vector<vec3>         m_vertices;
			#endif
		public:
			/**
			 * @biref Main constructor.
			 */
			BoundingAABB(void);
			/**
			 * @biref Main constructor.
			 */
			virtual ~BoundingAABB(void);
			/**
			 * @brief Update Bounding properties.
			 */
			virtual void Update(game::MeshObject& object, mat4& transformMatrix);
			/**
			 * @brief Draw the bounding ==> for test ...
			 */
			virtual void Draw(void);
			/**
			 * @brief Detect the colision positions.
			 */
			virtual bool HasContact(game::BoundingAABB& otherbounding);
			/**
			 * @brief Calculate the penetration size in 2 bounding.
			 */
			virtual vec3 CalculatePetetration(game::BoundingAABB& otherbounding, vec3& movingVect);
			/**
			 * @brief Set the contact property at a specific value ...
			 */
			void SetContactMode(bool newStatus) { m_hasContact=newStatus; };
			/**
			 * @brief Get the current contact status
			 */
			bool GetContactStatus(void) { return m_hasContact; };
			
			vec3 Size(void) { return m_PointStop-m_PointStart; };
	};
}


#endif
