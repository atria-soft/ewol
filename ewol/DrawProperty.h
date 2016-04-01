/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/math/Vector2D.h>

namespace ewol {
	/**
	 * @not_in_doc
	 */
	class DrawProperty{
		/*
		                                                          /--> m_windowsSize
		      *--------------------------------------------------*
		      |                                           g       |
		      |                                                  |
		      |                                    m_size        |
		      |                                   /              |
		      |              o-------------------o               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              o-------------------o               |
		      |             /                                    |
		      |     m_origin                                     |
		      |                                                  |
		      *--------------------------------------------------*
		     /
		   (0,0)
		 */
		public :
			ivec2 m_windowsSize; //!< Windows compleate size
			ivec2 m_origin; //!< Windows clipping upper widget (can not be <0)
			ivec2 m_size; //!< Windows clipping upper widget (can not be <0 and >m_windowsSize)
			void limit(const vec2& _origin, const vec2& _size);
	};
	std::ostream& operator <<(std::ostream& _os, const ewol::DrawProperty& _obj);
	
}
