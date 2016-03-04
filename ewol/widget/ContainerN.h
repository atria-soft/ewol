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
#include <list>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the Cotainer widget is a widget that have an only one subWidget
		 */
		class ContainerN : public ewol::Widget {
			public: // properties:
				eproperty::Value<bvec2> propertyLockExpand; //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
			protected:
				std::list<std::shared_ptr<ewol::Widget>> m_subWidget;
			protected:
				/**
				 * @brief Constructor
				 */
				ContainerN();
				void init();
				void init(const std::unordered_map<std::string,eproperty::Variant>& _listProperty);
			public:
				/**
				 * @brief Destructor
				 */
				virtual ~ContainerN();
			protected:
				bvec2 m_subExpend; //!< reference of the sub element expention requested.
				// herited function
				virtual bvec2 canExpand();
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
				virtual void subWidgetReplace(const std::shared_ptr<ewol::Widget>& _oldWidget,
				                              const std::shared_ptr<ewol::Widget>& _newWidget);
				/**
				 * @brief add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param[in] _newWidget the element pointer
				 * @return the ID of the set element
				 */
				virtual int32_t subWidgetAdd(std::shared_ptr<ewol::Widget> _newWidget);
				//! @previous
				inline  int32_t subWidgetAddBack(std::shared_ptr<ewol::Widget> _newWidget) {
					return subWidgetAdd(_newWidget);
				};
				//! @previous
				inline  int32_t subWidgetAddEnd(std::shared_ptr<ewol::Widget> _newWidget) {
					return subWidgetAdd(_newWidget);
				};
				/**
				 * @brief add at start position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param[in] _newWidget the element pointer
				 * @return the ID of the set element
				 */
				virtual int32_t subWidgetAddStart(std::shared_ptr<ewol::Widget> _newWidget);
				//! @previous
				inline  int32_t subWidgetAddFront(std::shared_ptr<ewol::Widget> _newWidget) {
					return subWidgetAddStart(_newWidget);
				};
				/**
				 * @brief remove definitly a widget from the system and this layer.
				 * @param[in] _newWidget the element pointer.
				 */
				virtual void subWidgetRemove(std::shared_ptr<ewol::Widget> _newWidget);
				/**
				 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...)
				 * @param[in] _newWidget the element pointer.
				 */
				virtual void subWidgetUnLink(std::shared_ptr<ewol::Widget> _newWidget);
			public:// Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual void onChangeSize();
				virtual void calculateMinMaxSize();
				virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos);
				virtual std::shared_ptr<ewol::Object> getSubObjectNamed(const std::string& _objectName);
				virtual bool loadXML(const std::shared_ptr<const exml::Element>& _node);
				virtual void setOffset(const vec2& _newVal);
				virtual void requestDestroyFromChild(const std::shared_ptr<Object>& _child);
			protected:
				virtual void onChangePropertyLockExpand();
		};
	};
};
