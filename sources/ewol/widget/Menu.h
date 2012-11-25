/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_MENU_H__
#define __EWOL_MENU_H__

#include <etk/types.h>
#include <etk/UString.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/ContextMenu.h>

namespace widget {
	
	class MenuElement {
		public : 
			MenuElement(void) : m_widgetPointer(NULL) { };
			int32_t        m_localId;
			int32_t        m_parentId;
			ewol::EObject* m_widgetPointer;
			etk::UString   m_label;
			etk::UString   m_image;
			const char *   m_generateEvent;
			etk::UString   m_message;
	};
	class Menu :public widget::SizerHori
	{
		public:
			Menu(void);
			virtual ~Menu(void);
			// Derived functionv
			virtual const char * const GetObjectType(void) { return "EwolMenu"; };
		private:
			virtual void   SubWidgetRemoveAll(void);
			virtual void   SubWidgetAdd(ewol::Widget* newWidget);
			virtual void   SubWidgetRemove(ewol::Widget* newWidget);
			virtual void   SubWidgetUnLink(ewol::Widget* newWidget);
		private:
			etk::Vector<widget::MenuElement*> m_listElement;
			int32_t                           m_staticId;      // unique ID for every element of the menu ...
			widget::ContextMenu*              m_widgetContextMenu;
		public:
			void           Clear(void);
			int32_t        AddTitle(etk::UString label, etk::UString image="", const char * generateEvent = NULL, const etk::UString message = "");
			int32_t        Add(int32_t parent, etk::UString label, etk::UString image="", const char * generateEvent = NULL, const etk::UString message = "");
			void           AddSpacer(void);
			// Derived function
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
			// Derived function
			virtual void OnObjectRemove(ewol::EObject * removeObject);
	};
	
};

#endif
