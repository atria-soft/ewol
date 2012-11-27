/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#ifndef __EWOL_BUTTON_H__
#define __EWOL_BUTTON_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Image.h>
#include <ewol/compositing/Shaper.h>

extern const char * const ewolEventButtonPressed;
extern const char * const ewolEventButtonDown;
extern const char * const ewolEventButtonUp;
extern const char * const ewolEventButtonEnter;
extern const char * const ewolEventButtonLeave;

namespace widget {
	typedef enum {
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_CENTER,
	} textAlignement_te;
	class Button : public ewol::Widget
	{
		private:
			ewol::Shaper                m_shaper;
			ewol::Text                  m_displayText;
			ewol::Image                 m_displayImage;
			ewol::Image                 m_displayImageToggle;
			textAlignement_te           m_alignement;
			etk::UString                m_label;
			draw::Color                 m_textColorFg;  //!< Text color
		public:
			Button(etk::UString newLabel="No Label");
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolButton"; };
			virtual ~Button(void);
			// Derived function
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
			etk::UString   GetLabel(void) {return m_label;};
			void           SetImage(etk::UString imageName);
			void           SetImageToggle(etk::UString imageName);
			void           SetValue(bool val);
			bool           GetValue(void);
			void           SetAlignement(textAlignement_te typeAlign);
			void           SetColorFg(draw::Color newColor) { m_textColorFg = newColor; };
		public:
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
			// Derived function
			virtual bool OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData);
		private:
			void ChangeStatusIn(int32_t newStatusId);
			// Derived function
			virtual void PeriodicCall(int64_t localTime);
	};
};

#endif
