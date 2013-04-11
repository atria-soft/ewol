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
			static void Init(void);
			static void UnInit(void);
		public:
			Image(const etk::UString& dataFile="", int32_t size=-1);
			virtual ~Image(void);
			void SetFile(etk::UString newFile);
			void SetPadding(vec2 newPadding);
		private:
			etk::UString m_imageSelected;
			vec2  m_padding;
			draw::Color m_textColorBg;  //!< Background color
			int32_t m_imageSize;
		public:
			// Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::Image"; };
			virtual void CalculateMinMaxSize(void);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, const vec2& pos);
	};
};


#endif
