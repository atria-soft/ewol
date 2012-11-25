/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_IMAGE_H__
#define __EWOL_IMAGE_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <draw/Color.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventImagePressed;

namespace widget {
	class Image :public widget::Drawable
	{
		public:
			Image(etk::UString dataFile, int32_t size=-1); // automatic considering in the appl Data older
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolImage"; };
			void Init(void);
			virtual ~Image(void);
			// Derived function
			virtual bool   CalculateMinSize(void);
			void           SetFile(etk::UString newFile);
			void           SetPadding(etk::Vector2D<float>  newPadding);
		private:
			etk::UString          m_imageSelected;
			etk::Vector2D<float>  m_padding;
			draw::Color           m_textColorBg;  //!< Background color
			int32_t               m_imageSize;
		public:
			// Derived function
			virtual void   OnRegenerateDisplay(void);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
	};
};


#endif
