/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/widget/ContextMenu.hpp>

namespace ewol {
	namespace widget {
		class MenuElement {
			public : 
				MenuElement() { };
				int32_t m_localId;
				int32_t m_parentId;
				ewol::WidgetWeak m_widgetPointer;
				std::string m_label;
				std::string m_image;
				std::string m_message;
		};
		class Menu;
		using MenuShared = ememory::SharedPtr<ewol::widget::Menu>;
		using MenuWeak = ememory::WeakPtr<ewol::widget::Menu>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Menu :public ewol::widget::Sizer {
			public:
				esignal::Signal<std::string> signalSelect; // event on a menu button or ...
			protected:
				Menu();
			public:
				DECLARE_WIDGET_FACTORY(Menu, "Menu");
				virtual ~Menu();
			private:
				void subWidgetRemoveAll() override;
				int32_t subWidgetAdd(ewol::WidgetShared _newWidget) override;
				void subWidgetRemove(ewol::WidgetShared _newWidget) override;
				void subWidgetUnLink(ewol::WidgetShared _newWidget) override;
				bool loadXML(const exml::Element& _node) override;
			private:
				std::vector<ewol::widget::MenuElement> m_listElement;
				int32_t m_staticId; // unique ID for every element of the menu ...
				ewol::widget::ContextMenuWeak m_widgetContextMenu;
				int32_t get(const std::string& _label);
			public:
				void clear();
				int32_t addTitle(const std::string& _label, const std::string& _image="", const std::string& _message = "");
				int32_t add(int32_t _parent, const std::string& _label, const std::string& _image="", const std::string& _message = "");
				int32_t addSpacer();
				void remove(int32_t _id);
			private:
				void onButtonPressed(ewol::widget::ButtonWeak _button);
		};
	};
};

