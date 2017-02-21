/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <list>

namespace ewol {
	namespace widget {
		class ContainerN;
		using ContainerNShared = ememory::SharedPtr<ewol::widget::ContainerN>;
		using ContainerNWeak = ememory::WeakPtr<ewol::widget::ContainerN>;
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the Cotainer widget is a widget that have an only one subWidget
		 */
		class ContainerN : public ewol::Widget {
			public: // properties:
				eproperty::Value<bvec2> propertyLockExpand; //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
			protected:
				std::list<ewol::WidgetShared> m_subWidget;
			protected:
				/**
				 * @brief Constructor
				 */
				ContainerN();
			public:
				/**
				 * @brief Destructor
				 */
				virtual ~ContainerN();
			protected:
				bvec2 m_subExpend; //!< reference of the sub element expention requested.
				// herited function
				virtual bvec2 canExpand() override;
			public:
				/**
				 * @brief remove all sub element from the widget.
				 */
				virtual void subWidgetRemoveAll();
				/**
				 * @brief remove all sub element from the widget (delayed to prevent remove in the callbback).
				 */
				virtual void subWidgetRemoveAllDelayed();
				/**
				 * @brief Replace a old subwidget with a new one.
				 * @param[in] _oldWidget The widget to replace.
				 * @param[in] _newWidget The widget to set.
				 */
				virtual void subWidgetReplace(ewol::WidgetShared _oldWidget,
				                              ewol::WidgetShared _newWidget);
				/**
				 * @brief add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param[in] _newWidget the element pointer
				 * @return the ID of the set element
				 */
				virtual int32_t subWidgetAdd(ewol::WidgetShared _newWidget);
				//! @previous
				inline  int32_t subWidgetAddBack(ewol::WidgetShared _newWidget) {
					return subWidgetAdd(_newWidget);
				};
				//! @previous
				inline  int32_t subWidgetAddEnd(ewol::WidgetShared _newWidget) {
					return subWidgetAdd(_newWidget);
				};
				/**
				 * @brief add at start position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param[in] _newWidget the element pointer
				 * @return the ID of the set element
				 */
				virtual int32_t subWidgetAddStart(ewol::WidgetShared _newWidget);
				//! @previous
				inline  int32_t subWidgetAddFront(ewol::WidgetShared _newWidget) {
					return subWidgetAddStart(_newWidget);
				};
				/**
				 * @brief remove definitly a widget from the system and this layer.
				 * @param[in] _newWidget the element pointer.
				 */
				virtual void subWidgetRemove(ewol::WidgetShared _newWidget);
				/**
				 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...)
				 * @param[in] _newWidget the element pointer.
				 */
				virtual void subWidgetUnLink(ewol::WidgetShared _newWidget);
			public:
				void systemDraw(const ewol::DrawProperty& _displayProp) override;
				void onRegenerateDisplay() override;
				void onChangeSize() override;
				void calculateMinMaxSize() override;
				ewol::WidgetShared getWidgetAtPos(const vec2& _pos) override;
				ewol::ObjectShared getSubObjectNamed(const std::string& _objectName) override;
				bool loadXML(const exml::Element& _node) override;
				void setOffset(const vec2& _newVal) override;
				void requestDestroyFromChild(const ewol::ObjectShared& _child) override;
				void drawWidgetTree(int32_t _level=0) override;
			protected:
				virtual void onChangePropertyLockExpand();
		};
	};
};
