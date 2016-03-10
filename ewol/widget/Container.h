/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace ewol {
	namespace widget {
		class Container;
		using ContainerShared = ememory::SharedPtr<ewol::widget::Container>;
		using ContainerWeak = ememory::WeakPtr<ewol::widget::Container>;
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the Cotainer widget is a widget that have an only one subWidget
		 */
		class Container : public ewol::Widget {
			protected:
				ewol::WidgetShared m_subWidget;
			protected:
				/**
				 * @brief Constructor
				 */
				Container();
			public:
				/**
				 * @brief Destructor
				 */
				virtual ~Container();
			public:
				/**
				 * @brief get the main node widget
				 * @return the requested pointer on the node
				 */
				ewol::WidgetShared getSubWidget();
				/**
				 * @brief set the subWidget node widget.
				 * @param[in] _newWidget The widget to add.
				 */
				void setSubWidget(ewol::WidgetShared _newWidget);
				/**
				 * @brief Replace a old subwidget with a new one.
				 * @param[in] _oldWidget The widget to replace.
				 * @param[in] _newWidget The widget to set.
				 */
				virtual void subWidgetReplace(const ewol::WidgetShared& _oldWidget,
				                              const ewol::WidgetShared& _newWidget);
				/**
				 * @brief remove the subWidget node (async).
				 */
				void subWidgetRemove();
				/**
				 * @brief Unlink the subwidget Node.
				 */
				void subWidgetUnLink();
			public: // Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual void onChangeSize();
				virtual void calculateMinMaxSize();
				virtual ewol::WidgetShared getWidgetAtPos(const vec2& _pos);
				virtual ewol::ObjectShared getSubObjectNamed(const std::string& _objectName);
				virtual bool loadXML(const std::shared_ptr<const exml::Element>& _node);
				virtual void setOffset(const vec2& _newVal);
				virtual void requestDestroyFromChild(const ewol::ObjectShared& _child);
		};
	};
};
