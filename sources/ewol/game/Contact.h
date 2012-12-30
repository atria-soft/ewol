/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_CONTACT_H__
#define __GAME_CONTACT_H__


#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <ewol/game/Element.h>

namespace game
{
	class Contact
	{
		protected:
			vec3           m_position;
			game::Element* m_element[2];
		public:
			/**
			 * @brief Basic constructor.
			 */
			Contact(void);
			/**
			 * @brief Basic destructor.
			 */
			virtual ~Contact(void);
			
			/**
			 * Recopy operator.
			 */
			const Contact& operator= (const Contact& obj ) {
				if( this == &obj ) {
					return *this;
				}
				// recopy all the element ...
				m_position = obj.m_position;
				m_element[0] = obj.m_element[0];
				m_element[1] = obj.m_element[1];
				return *this;
			}
	};
};


#endif
