/**
 *******************************************************************************
 * @file ewol/widget/List.h
 * @brief ewol list widget system (header)
 * @author Edouard DUPIN
 * @date 27/12/2011
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

#ifndef __EWOL_LIST_H__
#define __EWOL_LIST_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/WidgetScrolled.h>
#include <ewol/widget/Drawable.h>

namespace ewol {
	class List :public ewol::WidgetScrooled
	{
		public:
			List(void);
			void Init(void);
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
			virtual ~List(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
		// Drawing capabilities ....
		private:
			etk::VectorType<ewol::OObject*> m_listOObject[NB_BOUBLE_BUFFER];   //!< generic element to display...
		public:
			void    AddOObject(ewol::OObject* newObject, int32_t pos=-1);
			void    ClearOObjectList(void);
		protected:
			virtual void OnDraw(void);
		// list properties ...
		private:
			int32_t        m_paddingSizeX;
			int32_t        m_paddingSizeY;
			int32_t        m_displayStartRaw;           //!< Current starting diaplayed raw
			int32_t        m_displayCurrentNbLine;      //!< Number of line in the display
		public:
			virtual void   OnRegenerateDisplay(void);
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool   OnEventInput(int32_t IdInput, eventInputType_te typeEvent, coord2D_ts pos);
		protected:
			// function call to display the list :
			virtual color_ts GetBasicBG(void) {
				color_ts bg;
				bg.red = 1.0;
				bg.green = 1.0;
				bg.blue = 1.0;
				bg.alpha = 1.0;
				return bg;
			}
			virtual uint32_t GetNuberOfColomn(void) {
				return 0;
			};
			virtual bool GetTitle(int32_t colomn, etk::UString &myTitle, color_ts &fg, color_ts &bg) {
				myTitle = "";
				return false;
			};
			virtual uint32_t GetNuberOfRaw(void) {
				return 0;
			};
			virtual bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, color_ts &fg, color_ts &bg) {
				myTextToWrite = "";
				fg.red = 0.0;
				fg.green = 0.0;
				fg.blue = 0.0;
				fg.alpha = 1.0;
				if (raw % 2) {
					bg.red = 1.0;
					bg.green = 1.0;
					bg.blue = 1.0;
					bg.alpha = 1.0;
				} else {
					bg.red = 0.5;
					bg.green = 0.5;
					bg.blue = 0.5;
					bg.alpha = 1.0;
				}
				return false;
			};
			virtual bool OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent,  int32_t colomn, int32_t raw, etkFloat_t x, etkFloat_t y) {
				return false;
			}
		protected:
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_LIST;
	
};

#define EWOL_CAST_WIDGET_LIST(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_LIST,ewol::List,curentPointer)

#endif
