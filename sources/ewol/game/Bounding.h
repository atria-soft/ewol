/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_BOUNDING_H__
#define __GAME_BOUNDING_H__

#include "ewol/debug.h"
#include "ewol/game/MeshObject.h"
#include <etk/math/Matrix4.h>

namespace game
{
	typedef enum {
		BoundingModeNone,   //!< No Bounding.
		BoundingModePlane,  //!< plane Bounding.
		BoundingModeAABB,   //!< Anti-aligned Bounding Boxes.
		BoundingModeSphere, //!< Sphere.
		BoundingModeOBB,    //!< Oriented Bounding Box.
		// TODO : Add more if needed to implement
	} boundingMode;
	
	class Bounding
	{
		protected :
			boundingMode m_mode;           //!< bounding mode of this system.
		public:
			/**
			 * @biref Main constructor.
			 * @param[in] mode Bounding mode.
			 */
			Bounding(boundingMode mode);
			/**
			 * @biref Main constructor.
			 */
			virtual ~Bounding(void);
			/**
			 * @brief Get the bounding type
			 * @return the bounding type;
			 */
			boundingMode GetType(void) { return m_mode; };
			/**
			 * @brief Update Bounding properties.
			 */
			virtual void Update(game::MeshObject& object, mat4& transformMatrix) {};
			/**
			 * Draw the bounding ==> for test ...
			 */
			virtual void Draw(void) {};
	};
	
	Bounding* CreateBounding(boundingMode mode);
	
};

#endif
