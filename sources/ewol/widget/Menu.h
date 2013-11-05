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
#include <ewol/widget/Sizer.h>
#include <ewol/widget/ContextMenu.h>

namespace widget {
	class MenuElement {
		public : 
			MenuElement(void) : m_widgetPointer(NULL) { };
			int32_t m_localId;
			int32_t m_parentId;
			ewol::EObject* m_widgetPointer;
			etk::UString m_label;
			etk::UString m_image;
			const char *m_generateEvent;
			etk::UString m_message;
	};
	class Menu :public widget::Sizer {
		public:
			Menu(void);
			virtual ~Menu(void);
			// Derived functionv
			virtual const char * const getObjectType(void) {
				return "ewol::menu";
			};
		private:
			virtual void subWidgetRemoveAll(void);
			virtual int32_t subWidgetAdd(ewol::Widget* _newWidget);
			virtual void subWidgetRemove(ewol::Widget* _newWidget);
			virtual void subWidgetUnLink(ewol::Widget* _newWidget);
		private:
			etk::Vector<widget::MenuElement*> m_listElement;
			int32_t m_staticId; // unique ID for every element of the menu ...
			widget::ContextMenu* m_widgetContextMenu;
		public:
			void clear(void);
			int32_t addTitle(etk::UString _label, etk::UString _image="", const char * _generateEvent = NULL, const etk::UString _message = "");
			int32_t add(int32_t parent, etk::UString _label, etk::UString _image="", const char * _generateEvent = NULL, const etk::UString _message = "");
			void addSpacer(void);
			// Derived function
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
			virtual void onObjectRemove(ewol::EObject * _removeObject);
	};
};

#endif
