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
			Vector2i m_windowsSize; //!< Windows compleate size
			Vector2i m_origin; //!< Windows clipping upper widget (can not be <0)
			Vector2i m_size; //!< Windows clipping upper widget (can not be <0 and >m_windowsSize)
			void limit(const Vector2f& _origin, const Vector2f& _size);
	};
	etk::Stream& operator <<(etk::Stream& _os, const ewol::DrawProperty& _obj);
	
}
