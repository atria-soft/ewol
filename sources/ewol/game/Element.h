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

namespace game
{
	class Element
	{
		private:
			//ewol::??? m_resource;   //!< Resource to display the element.
		protected:
			vec3     m_position;    //!< Current position of the element.
			vec3     m_speed;       //!< Speed of the element.
			vec3     m_orientation; //!< Display orientation ==> speed does not generate the orientation.
			uint32_t m_uniqueId;    //!< General element ID (uint16_t, because all is reference with the groupId like this only a uint32_t reference an element)
			uint32_t m_groupId;     //!< General group Id More than 65000 group can be really interesting to create supid game ...
			int32_t  m_type;        //!< type of this element
			bool     m_visible;     //!< This is to know if the element is displayed or not ==> TODO : check if usefull ...
			float    m_mass;        //!< Current element Mass ==> for the physical calculation
		public:
			/**
			 * @brief Basic constructor.
			 */
			Element(void);
			/**
			 * @brief Basic destructor.
			 */
			~Element(void);
			
	};
};


#endif
