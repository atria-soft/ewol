/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_MENU_H__
#define __EWOL_MENU_H__

#include <etk/types.h>
#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/ContextMenu.h>

namespace ewol {
	namespace widget {
		class MenuElement {
			public : 
				MenuElement() { };
				int32_t m_localId;
				int32_t m_parentId;
				std::weak_ptr<ewol::Widget> m_widgetPointer;
				std::string m_label;
				std::string m_image;
				const char* m_generateEvent;
				std::string m_message;
		};
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Menu :public ewol::widget::Sizer {
			protected:
				Menu();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(Menu, "Menu");
				virtual ~Menu();
			private:
				virtual void subWidgetRemoveAll();
				virtual int32_t subWidgetAdd(std::shared_ptr<ewol::Widget> _newWidget);
				virtual void subWidgetRemove(std::shared_ptr<ewol::Widget> _newWidget);
				virtual void subWidgetUnLink(std::shared_ptr<ewol::Widget> _newWidget);
			private:
				std::vector<ewol::widget::MenuElement> m_listElement;
				int32_t m_staticId; // unique ID for every element of the menu ...
				std::weak_ptr<ewol::widget::ContextMenu> m_widgetContextMenu;
			public:
				void clear();
				int32_t addTitle(std::string _label, std::string _image="", const char * _generateEvent = nullptr, const std::string _message = "");
				int32_t add(int32_t _parent, std::string _label, std::string _image="", const char * _generateEvent = nullptr, const std::string _message = "");
				void addSpacer();
				// Derived function
				virtual void onReceiveMessage(const ewol::object::Message& _msg);
			private:
				void onButtonPressed(std::weak_ptr<ewol::widget::Button> _button);
		};
	};
};

#endif
