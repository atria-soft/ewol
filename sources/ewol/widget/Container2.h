/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_CONTAINER_2_H__
#define __EWOL_WIDGET_CONTAINER_2_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/Padding.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the Cotainer widget is a widget that have an only one subWidget
		 */
		class Container2 : public ewol::Widget {
			protected:
				ewol::Widget* m_subWidget[2]; //!< 2 subwidget possible
				int32_t m_idWidgetDisplayed; //!< current widget displayed
			public:
				/**
				 * @brief Constructor
				 * @param[in] _subElement Widget to set on the normal position
				 * @param[in] _subElementToggle Widget to set on the toggle position
				 */
				Container2(ewol::Widget* _subElement = NULL, ewol::Widget* _subElementToggle = NULL);
				/**
				 * @brief Destructor
				 */
				~Container2(void);
			private:
				/**
				 * @brief Specify the current widget
				 * @param[in] _subWidget Widget to add normal
				 * @param[in] _idWidget Id of the widget to set
				 */
				void setSubWidget(ewol::Widget* _subWidget, int32_t _idWidget);
			public:
				/**
				 * @brief Specify the current widget
				 * @param[in] _subWidget Widget to add normal
				 */
				void setSubWidget(ewol::Widget* _subWidget) {
					setSubWidget(_subWidget, 0);
				}
				/**
				 * @brief Specify the current toggle widget
				 * @param[in] _subWidget Widget to add Toggle
				 */
				void setSubWidgetToggle(ewol::Widget* _subWidget) {
					setSubWidget(_subWidget, 1);
				}
			private:
				/**
				 * @brief get the current displayed composition
				 * @param[in] _idWidget Id of the widget to set
				 * @return The base widget
				 */
				ewol::Widget* getSubWidget(int32_t _idWidget) const {
					return m_subWidget[_idWidget];
				};
			public:
				/**
				 * @brief get the current displayed composition
				 * @return The base widget
				 */
				ewol::Widget* getSubWidget(void) const {
					return getSubWidget(0);
				};
				/**
				 * @brief get the current displayed composition
				 * @return The toggle widget
				 */
				ewol::Widget* getSubWidgetToggle(void) const {
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
				void subWidgetRemove(void) {
					subWidgetRemove(0);
				}
				/**
				 * @brief remove the subWidget Toggle node (async).
				 */
				void subWidgetRemoveToggle(void) {
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
				void subWidgetUnLink(void) {
					subWidgetUnLink(0);
				}
				/**
				 * @brief Unlink the subwidget Toggle Node.
				 */
				void subWidgetUnLinkToggle(void) {
					subWidgetUnLink(1);
				}
			protected:
				/**
				 * @brief Parent set the possible diplay size of the current widget whith his own possibilities
				 *        By default this save the widget available size in the widget size
				 * @param[in] _available Available x&y pixel size
				 * @param[in] _padding Padding of the widget.
				 * @note : INTERNAL EWOL SYSTEM
				 */
				virtual ewol::Padding calculateSizePadded(const vec2& _availlable, const ewol::Padding& _padding = ewol::Padding(0,0,0,0));
				/**
				 * @brief calculate the minimum and maximum size (need to estimate expend properties of the widget)
				 * @param[in] _padding Padding of the widget.
				 * @note : INTERNAL EWOL SYSTEM
				 */
				virtual void calculateMinMaxSizePadded(const ewol::Padding& _padding = ewol::Padding(0,0,0,0));
				/**
				 * @brief Called when parsing a XML and detect the presence of a second Widget
				 */
				virtual void onDetectPresenceToggleWidget(void) {}
				/**
				 * @brief convert ID of the widget if not existed
				 * @param[in] _id Id of the widget to display.
				 * @return the id of the widget displayable
				 */
				int32_t convertId(int32_t _id) {
					if (m_subWidget[_id] == NULL) {
						return (_id+1)%2;
					}
					return _id;
				}
			public: // Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay(void);
				virtual void onObjectRemove(ewol::Object* _removeObject);
				virtual void calculateSize(const vec2& _availlable) {
					calculateSizePadded(_availlable);
				}
				virtual void calculateMinMaxSize(void) {
					calculateMinMaxSizePadded();
				}
				//virtual ewol::Widget* getWidgetAtPos(const vec2& _pos);
				virtual ewol::Widget* getWidgetNamed(const std::string& _widgetName);
				virtual bool loadXML(exml::Element* _node);
				virtual void setOffset(const vec2& _newVal);
		};
	};
};

#endif
