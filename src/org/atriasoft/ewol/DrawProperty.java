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
		                                                          /-. this.windowsSize
		      *--------------------------------------------------*
		      |                                           g       |
		      |                                                  |
		      |                                    this.size        |
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
		      |     this.origin                                     |
		      |                                                  |
		      *--------------------------------------------------*
		     /
		   (0,0)
		 */
		public :
			Vector2i this.windowsSize; //!< Windows compleate size
			Vector2i this.origin; //!< Windows clipping upper widget (can not be <0)
			Vector2i this.size; //!< Windows clipping upper widget (can not be <0 and >this.windowsSize)
			void limit( Vector2f _origin,  Vector2f _size);
	};
	etk::Stream operator +(etk::Stream _os,  ewol::DrawProperty _obj);
	
}
