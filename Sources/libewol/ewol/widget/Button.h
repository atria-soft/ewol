/**
 *******************************************************************************
 * @file ewol/widget/Button.h
 * @brief ewol Button widget system (header)
 * @author Edouard DUPIN
 * @date 07/11/2011
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

#ifndef __EWOL_BUTTON_H__
#define __EWOL_BUTTON_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventButtonPressed;
extern const char * const ewolEventButtonDown;
extern const char * const ewolEventButtonUp;
extern const char * const ewolEventButtonEnter;
extern const char * const ewolEventButtonLeave;

namespace ewol {
	typedef enum {
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_CENTER,
	} textAlignement_te;
	class Button :public ewol::Drawable
	{
		public:
			Button(void);
			Button(etk::UString newLabel);
			/**
			 * @brief Check if the object has the specific type.
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type of the object we want to check
			 * @return true if the object is compatible, otherwise false
			 */
			virtual bool CheckObjectType(const char * const objectType);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void);
			void Init(void);
			virtual ~Button(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
			etk::UString   GetLabel(void) {return m_label;};
			void           SetImage(etk::UString imageName);
			void           SetValue(bool val);
			bool           GetValue(void);
			void           SetAlignement(textAlignement_te typeAlign);
			void           SetPadding(coord2D_ts newPadding);
		private:
			bool               m_hasAnImage;
			etk::UString       m_imageSelected;
			textAlignement_te  m_alignement;
			coord2D_ts         m_padding;
			etk::UString       m_label;
			color_ts           m_textColorFg;  //!< Text color
			color_ts           m_textColorBg;  //!< Background color
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos);
			virtual bool OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData);
	};
	
	/**
	 * @brief Initilise the basic widget property ==> due to the android system
	 * @note all widget that have template might have this initializer ...
	 * @param ---
	 * @return ---
	 */
	void WIDGET_ButtonInit(void);
	
	extern const char * const TYPE_EOBJECT_WIDGET_BUTTON;
	
};
#define EWOL_CAST_WIDGET_BUTTON(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_BUTTON,ewol::Button,curentPointer)

#endif
