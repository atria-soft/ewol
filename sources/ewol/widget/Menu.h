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
#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/ContextMenu.h>

namespace ewol {
	namespace widget {
		class MenuElement {
			public : 
				MenuElement() : m_widgetPointer(nullptr) { };
				int32_t m_localId;
				int32_t m_parentId;
				ewol::object::Shared<ewol::Object> m_widgetPointer;
				std::string m_label;
				std::string m_image;
				const char *m_generateEvent;
				std::string m_message;
		};
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Menu :public ewol::widget::Sizer {
			public:
				Menu();
				virtual ~Menu();
			private:
				virtual void subWidgetRemoveAll();
				virtual int32_t subWidgetAdd(ewol::object::Shared<ewol::Widget> _newWidget);
				virtual void subWidgetRemove(ewol::object::Shared<ewol::Widget> _newWidget);
				virtual void subWidgetUnLink(ewol::object::Shared<ewol::Widget> _newWidget);
			private:
				std::vector<ewol::widget::MenuElement*> m_listElement;
				int32_t m_staticId; // unique ID for every element of the menu ...
				ewol::widget::ContextMenu* m_widgetContextMenu;
			public:
				void clear();
				int32_t addTitle(std::string _label, std::string _image="", const char * _generateEvent = nullptr, const std::string _message = "");
				int32_t add(int32_t parent, std::string _label, std::string _image="", const char * _generateEvent = nullptr, const std::string _message = "");
				void addSpacer();
				// Derived function
				virtual void onReceiveMessage(const ewol::object::Message& _msg);
				virtual void onObjectRemove(ewol::object::Shared<ewol::Object> _removeObject);
		};
	};
};

#endif
