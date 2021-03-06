/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector2D.hpp>

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
	etk::Stream& operator <<(etk::Stream& _os, const ewol::DrawProperty& _obj);
	
}
