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
			virtual bool OnDraw(void);
		// list properties ...
		private:
			int32_t        m_paddingSizeX;
			int32_t        m_paddingSizeY;
			int32_t        m_displayStartRaw;           //!< Current starting diaplayed raw
			int32_t        m_displayCurrentNbLine;      //!< Number of line in the display
		public:
			virtual void   OnRegenerateDisplay(void);
			virtual bool   OnEventInput(int32_t IdInput, eventInputType_te typeEvent, eventPosition_ts pos);
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
};

#endif
