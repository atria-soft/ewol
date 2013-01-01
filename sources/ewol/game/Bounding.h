/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_BOUNDING_H__
#define __GAME_BOUNDING_H__

#include <ewol/debug.h>
#include <ewol/game/MeshObject.h>
#include <etk/math/Matrix4.h>
//#include <ewol/game/Contact.h>

namespace game
{
	typedef enum {
		BoundingModePlane,  //!< plane Bounding.
		BoundingModeSphere, //!< Sphere.
		BoundingModeAABB,   //!< Anti-aligned Bounding Boxes.
		BoundingModeOBB,    //!< Oriented Bounding Box.
		// TODO : Add more if needed to implement
	} boundingMode;
	
	class Bounding
	{
		protected:
			boundingMode m_mode;           //!< bounding mode of this system.
			bool         m_hasContact;     //!< this bounding is on contact with something else ...
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
			 * @brief Draw the bounding ==> for test ...
			 */
			virtual void Draw(void) {};
			/**
			 * @brief Detect the colision positions.
			 */
			//virtual void GenerateContact(game::Element* ourElement, game::Bounding* otherbounding, game::Element* otherElements, etk::Vector<game::Contact>& contactList);
			virtual bool HasContact(game::Bounding& otherbounding) { return false; };
			/**
			 * @brief Set the contact property at a specific value ...
			 */
			void SetContactMode(bool newStatus) { m_hasContact=newStatus; };
			
			bool GetContactStatus(void) { return m_hasContact; };
	};
	
	Bounding* CreateBounding(boundingMode mode);
	
};

#endif
