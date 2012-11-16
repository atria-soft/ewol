/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#ifndef __EWOL_BUTTON_H__
#define __EWOL_BUTTON_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/oObject/OObject.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Text.h>

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



namespace ewol {
	typedef enum {
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_CENTER,
	} textAlignement_te;
	class Button : public ewol::Widget
	{
		private:
			// External theme config:
			ewol::SimpleConfigFile*     m_config;
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
			ewol::Text                 m_displayText;
			ewol::OObject2DTextured*   m_oObjectImage;
			bool                       m_hasAnImage;
			etk::UString               m_imageSelected;
			textAlignement_te          m_alignement;
			etk::UString               m_label;
			draw::Color                m_textColorFg;  //!< Text color
		public:
			Button(void);
			Button(etk::UString newLabel);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolButton"; };
			void Init(void);
			virtual ~Button(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
			etk::UString   GetLabel(void) {return m_label;};
			void           SetImage(etk::UString imageName);
			void           SetValue(bool val);
			bool           GetValue(void);
			void           SetAlignement(textAlignement_te typeAlign);
			void           SetColorFg(draw::Color newColor) { m_textColorFg = newColor; };
		public:
			virtual void OnRegenerateDisplay(void);
			virtual void OnDraw(DrawProperty& displayProp);
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, etk::Vector2D<float>  pos);
			virtual bool OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData);
		private:
			int32_t m_nextStatusRequested;
			void ChangeStatusIn(int32_t newStatusId);
			int64_t m_time;
			/**
			 * @brief Periodic call of this widget
			 * @param localTime curent system time
			 * @return ---
			 */
			virtual void PeriodicCall(int64_t localTime);
	};
	
	/**
	 * @brief Initilise the basic widget property ==> due to the android system
	 * @note all widget that have template might have this initializer ...
	 * @param ---
	 * @return ---
	 */
	void WIDGET_ButtonInit(void);
	
};

#endif
