/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/Padding.hpp>

namespace ewol {
	namespace widget {
		class Container2;
		using Container2Shared = ememory::SharedPtr<ewol::widget::Container2>;
		using Container2Weak = ememory::WeakPtr<ewol::widget::Container2>;
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the Cotainer widget is a widget that have an only one subWidget
		 */
		class Container2 : public ewol::Widget {
			protected:
				ewol::WidgetShared m_subWidget[2]; //!< 2 subwidget possible
				int32_t m_idWidgetDisplayed; //!< current widget displayed
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _subElement Widget to set on the normal position
				 * @param[in] _subElementToggle Widget to set on the toggle position
				 */
				Container2();
			public:
				/**
				 * @brief Destructor
				 */
				virtual ~Container2();
			private:
				/**
				 * @brief Specify the current widget
				 * @param[in] _subWidget Widget to add normal
				 * @param[in] _idWidget Id of the widget to set
				 */
				void setSubWidget(ewol::WidgetShared _subWidget, int32_t _idWidget);
			public:
				/**
				 * @brief Specify the current widget
				 * @param[in] _subWidget Widget to add normal
				 */
				void setSubWidget(ewol::WidgetShared _subWidget) {
					setSubWidget(_subWidget, 0);
				}
				/**
				 * @brief Specify the current toggle widget
				 * @param[in] _subWidget Widget to add Toggle
				 */
				void setSubWidgetToggle(ewol::WidgetShared _subWidget) {
					setSubWidget(_subWidget, 1);
				}
			private:
				/**
				 * @brief get the current displayed composition
				 * @param[in] _idWidget Id of the widget to set
				 * @return The base widget
				 */
				ewol::WidgetShared getSubWidget(int32_t _idWidget) const {
					return m_subWidget[_idWidget];
				};
			public:
				/**
				 * @brief get the current displayed composition
				 * @return The base widget
				 */
				ewol::WidgetShared getSubWidget() const {
					return getSubWidget(0);
				};
				/**
				 * @brief get the current displayed composition
				 * @return The toggle widget
				 */
				ewol::WidgetShared getSubWidgetToggle() const {
					return getSubWidget(1);
				};
			private:
				/**
				 * @brief remove the subWidget node (async).
				 * @param[in] _idWidget Id of the widget to set
				 */
				void subWidgetRemove(int32_t _idWidget);
			public:
				/**
				 * @brief remove the subWidget node (async).
				 */
				void subWidgetRemove() {
					subWidgetRemove(0);
				}
				/**
				 * @brief remove the subWidget Toggle node (async).
				 */
				void subWidgetRemoveToggle() {
					subWidgetRemove(1);
				}
			private:
				/**
				 * @brief Unlink the subwidget Node.
				 * @param[in] _idWidget Id of the widget to set
				 */
				void subWidgetUnLink(int32_t _idWidget);
			public:
				/**
				 * @brief Unlink the subwidget Node.
				 */
				void subWidgetUnLink() {
					subWidgetUnLink(0);
				}
				/**
				 * @brief Unlink the subwidget Toggle Node.
				 */
				void subWidgetUnLinkToggle() {
					subWidgetUnLink(1);
				}
			protected:
				/**
				 * @brief Parent set the possible diplay size of the current widget whith his own possibilities
				 *        By default this save the widget available size in the widget size
				 * @param[in] _padding Padding of the widget.
				 * @note : INTERNAL EWOL SYSTEM
				 */
				virtual ewol::Padding onChangeSizePadded(const ewol::Padding& _padding = ewol::Padding(0,0,0,0));
				/**
				 * @brief calculate the minimum and maximum size (need to estimate expend properties of the widget)
				 * @param[in] _padding Padding of the widget.
				 * @note : INTERNAL EWOL SYSTEM
				 */
				virtual void calculateMinMaxSizePadded(const ewol::Padding& _padding = ewol::Padding(0,0,0,0));
				/**
				 * @brief Called when parsing a XML and detect the presence of a second Widget
				 */
				virtual void onDetectPresenceToggleWidget() {}
				/**
				 * @brief convert ID of the widget if not existed
				 * @param[in] _id Id of the widget to display.
				 * @return the id of the widget displayable
				 */
				int32_t convertId(int32_t _id) {
					if (m_subWidget[_id] == null) {
						return (_id+1)%2;
					}
					return _id;
				}
				/**
				 * @brief Replace a old subwidget with a new one.
				 * @param[in] _oldWidget The widget to replace.
				 * @param[in] _newWidget The widget to set.
				 */
				virtual void subWidgetReplace(const ewol::WidgetShared& _oldWidget,
				                              const ewol::WidgetShared& _newWidget);
			public:
				void systemDraw(const ewol::DrawProperty& _displayProp) override;
				void onRegenerateDisplay() override;
				void onChangeSize() override {
					onChangeSizePadded();
				}
				void calculateMinMaxSize() override {
					calculateMinMaxSizePadded();
				}
				ewol::ObjectShared getSubObjectNamed(const etk::String& _objectName) override;
				bool loadXML(const exml::Element& _node) override;
				void setOffset(const vec2& _newVal) override;
				void requestDestroyFromChild(const ewol::ObjectShared& _child) override;
				void drawWidgetTree(int32_t _level=0) override;
		};
	};
};
