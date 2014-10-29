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
				std::string m_message;
		};
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Menu :public ewol::widget::Sizer {
			public:
				ewol::Signal<std::string> signalSelect; // event on a menu button or ...
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
				int32_t get(const std::string& _label);
			public:
				void clear();
				int32_t addTitle(const std::string& _label, const std::string& _image="", const std::string& _message = "");
				int32_t add(int32_t _parent, const std::string& _label, const std::string& _image="", const std::string& _message = "");
				int32_t addSpacer();
				void remove(int32_t _id);
			private:
				void onButtonPressed(std::weak_ptr<ewol::widget::Button> _button);
		};
	};
};

#endif
