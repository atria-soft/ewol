/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_ELEMENT_H__
#define __GAME_ELEMENT_H__


#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/Vector.h>
#include <ewol/debug.h>
#include <ewol/game/MeshProperty.h>
#include <ewol/renderer/resources/Mesh.h>

namespace game
{
	class Element
	{
		protected:
			ewol::Mesh*         m_resource;   //!< Resource to display the element.
			game::MeshProperty  m_property;   //!< display property f the element.
		protected:
			uint32_t m_uniqueId; //!< General element ID (uint16_t, because all is reference with the groupId like this only a uint32_t reference an element)
			uint32_t m_groupId;  //!< General group Id More than 65000 group can be really interesting to create supid game ...
			int32_t  m_type;     //!< type of this element
			bool     m_visible;  //!< This is to know if the element is displayed or not ==> TODO : check if usefull ...
			float    m_mass;     //!< Current element Mass ==> for the physical calculation
		public:
			/**
			 * @brief Basic constructor.
			 * @param[in] meshResource Resource name.
			 */
			Element(etk::UString meshResource);
			/**
			 * @brief Basic destructor.
			 */
			virtual ~Element(void);
			/**
			 * @brief Draw the element.
			 */
			virtual void Draw(void);
			/**
			 * @brief Process IA of this element.
			 * @param[in] deltaMicroSecond delta from the last call.
			 * @return true if this element must be destroyed
			 */
			virtual bool ArtificialIntelligence(int32_t deltaMicroSecond);
	};
};


#endif
