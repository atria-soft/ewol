/**
 *******************************************************************************
 * @file ewol/widget/Menu.h
 * @brief ewol Menu widget system (header)
 * @author Edouard DUPIN
 * @date 16/02/2012
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

#ifndef __EWOL_MENU_H__
#define __EWOL_MENU_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>
#include <ewol/widget/SizerHori.h>

namespace ewol {
	
	class MenuElement {
		public : 
			int32_t        m_localId;
			int32_t        m_parentId;
			int32_t        m_widgetId;
			etk::UString   m_label;
			etk::UString   m_image;
			const char *   m_generateEvent;
			etk::UString   m_message;
	};
	class Menu :public ewol::SizerHori
	{
		public:
			Menu(void);
			virtual ~Menu(void);
		private:
			virtual void   SubWidgetRemoveAll(void);
			virtual void   SubWidgetAdd(ewol::Widget* newWidget);
			virtual void   SubWidgetRemove(ewol::Widget* newWidget);
			virtual void   SubWidgetUnLink(ewol::Widget* newWidget);
		private:
			etk::VectorType<MenuElement*>  m_listElement;
			int32_t                        m_staticId;
			int32_t                        m_popUpId;
		public:
			void           Clear(void);
			int32_t        AddTitle(etk::UString label, etk::UString image="", const char * generateEvent = NULL, const etk::UString message = "");
			int32_t        Add(int32_t parent, etk::UString label, etk::UString image="", const char * generateEvent = NULL, const etk::UString message = "");
			void           AddSpacer(void);
			
			virtual bool   OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * data, etkFloat_t x, etkFloat_t y);
	};
};

#endif
