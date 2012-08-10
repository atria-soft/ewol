/**
 *******************************************************************************
 * @file ewol/widget/ButtonImage.h
 * @brief ewol Button Image widget system (header)
 * @author Edouard DUPIN
 * @date 06/06/2012
  * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_BUTTON_IMAGE_H__
#define __EWOL_BUTTON_IMAGE_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>
#include <ewol/widget/Button.h>

namespace ewol {
	class ButtonImage :public ewol::Widget
	{
		public:
			ButtonImage(etk::UString image, color_ts col=0xFFFFFFFF);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolButtonImage"; };
			void Init(void);
			virtual ~ButtonImage(void);
			void           SetImage(etk::UString imageName, color_ts col=0xFFFFFFFF);
			void           SetImageBG(etk::UString imageName, color_ts col=0xFFFFFFFF);
			void           SetImageSelected(etk::UString imageName, color_ts col=0xFFFFFFFF);
			void           SetValue(bool val);
			bool           GetValue(void);
			void           SetToggleMode(bool val);
			bool           GetToggleMode(void);
		private:
			etk::UString             m_image;
			bool                     m_resetNeeded;
			ewol::OObject2DTextured* m_OOImage;
			color_ts                 m_color;
			
			etk::UString             m_imageBg1;
			ewol::OObject2DTextured* m_OOImageBg1;
			color_ts                 m_colorBg1;
			
			etk::UString             m_imageBg2;
			ewol::OObject2DTextured* m_OOImageBG2;
			color_ts                 m_colorBg2;
			
			bool                     m_over;
			bool                     m_down;
			bool                     m_value;
			bool                     m_toggleMode;
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float>  pos);
			virtual bool OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData);
			virtual void OnDraw(DrawProperty& displayProp);
	};
	
	/**
	 * @brief Initilise the basic widget property ==> due to the android system
	 * @note all widget that have template might have this initializer ...
	 * @param ---
	 * @return ---
	 */
	void WIDGET_ButtonImageInit(void);
	
};

#endif
