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

extern const char * const ewolEventButtonPressed;
extern const char * const ewolEventButtonDown;
extern const char * const ewolEventButtonUp;
extern const char * const ewolEventButtonEnter;
extern const char * const ewolEventButtonLeave;


namespace ewol {
	class WidgetPosProperty
	{
		public:
			etk::Vector2D<float> m_insidePos;
			etk::Vector2D<float> m_insideSize;
	};
	
	class GLWidgetPosProperty
	{
		public:
			int32_t m_size;
			int32_t m_insidePos;
			int32_t m_insideSize;
	};
	// DATA
	class WidgetStateProperty
	{
		public:
			int32_t m_stateOld;
			int32_t m_stateNew;
			float   m_transition;
	};
	// ID of the UNIFORM element
	class GLWidgetStateProperty
	{
		public:
			int32_t m_stateOld;
			int32_t m_stateNew;
			int32_t m_transition;
	};
};



namespace widget {
	typedef enum {
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_CENTER,
	} textAlignement_te;
	class Button : public ewol::Widget
	{
		private:
			// External theme config:
			ewol::ConfigFile*           m_config;
			int32_t                     m_confIdPaddingX;
			int32_t                     m_confIdPaddingY;
			int32_t                     m_confIdChangeTime;
			// OpenGL shaders programs:
			ewol::Program*              m_GLprogram;
			int32_t                     m_GLPosition;
			int32_t                     m_GLMatrix;
			// widget property
			ewol::GLWidgetPosProperty   m_GLwidgetProperty; // id of the uniform
			ewol::WidgetPosProperty     m_widgetProperty;   // structure of this uniform
			// state property
			ewol::GLWidgetStateProperty m_GLstatus;
			ewol::WidgetStateProperty   m_status;
			
			etk::Vector<etk::Vector2D<float> > m_coord; //!< internal coord of the object
			void SetPoint(float x, float y);
			void Rectangle(float x, float y, float w, float h);
		private:
			ewol::Text                  m_displayText;
			ewol::Image                 m_displayImage;
			ewol::Image                 m_displayImageToggle;
			textAlignement_te           m_alignement;
			etk::UString                m_label;
			draw::Color                 m_textColorFg;  //!< Text color
		public:
			Button(void);
			Button(etk::UString newLabel);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolButton"; };
			void Init(void);
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
			int32_t m_nextStatusRequested;
			void ChangeStatusIn(int32_t newStatusId);
			int64_t m_time;
			// Derived function
			virtual void PeriodicCall(int64_t localTime);
	};
};

#endif
