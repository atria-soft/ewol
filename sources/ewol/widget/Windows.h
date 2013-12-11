/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WINDOWS_H__
#define __EWOL_WINDOWS_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <etk/Color.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Windows : public ewol::widget::Widget {
			public:
				Windows(void);
				virtual ~Windows(void);
			// internal event at ewol system :
			public:
				void sysDraw(void);
				void sysOnShow(void) {};
				void sysOnHide(void) {};
				void sysOnKill(void) {};
			public:
				virtual void onShow(void) { };
				virtual void onHide(void) { };
				virtual bool onKill(void) {
					return true;
				};
				virtual void onReduce(void) { };
				virtual void on(void) { };
			private:
				bool m_hasDecoration;
			public:
				void setDecorationDisable(void) {
					m_hasDecoration = false;
				}
				
				void setDecorationEnable(void) {
					m_hasDecoration = true;
				}
			private:
				ewol::Widget* m_subWidget;
				std::vector<ewol::Widget*> m_popUpWidgetList;
			public:
				void setSubWidget(ewol::Widget* _widget);
				void popUpWidgetPush(ewol::Widget* _widget);
			private:
				etk::Color<float> m_backgroundColor; //!< reset color of the Main windows
			public:
				/**
				 * @brief get the background color.
				 * @return A reference on the color
				 */
				const etk::Color<float>& getBackgroundColor(void) {
					return m_backgroundColor;
				};
				/**
				 * @brief set the background color.
				 * @param[IN] the new requested color.
				 */
				void setBackgroundColor(const etk::Color<float>& _color);
			protected: // Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
			public: // Derived function
				virtual void onRegenerateDisplay(void);
				virtual void onObjectRemove(ewol::EObject * _removeObject);
				virtual void calculateSize(const vec2& _availlable);
				virtual ewol::Widget * getWidgetAtPos(const vec2& _pos);
				void setTitle(const std::string& _title);
			public:
				enum popUpMessageType {
					messageTypeInfo, //!< information message pop-up
					messageTypeWarning, //!< warning message pop-up
					messageTypeError, //!< Error message pop-up
					messageTypeCritical //!< Critical message pop-up
				};
				/**
				 * @brief Create a simple pop-up message on the screen for application error.
				 * @param[in] _type Type of the error.
				 * @param[in] _message message to display (decorated text)
				 */
				virtual void createPopUpMessage(enum popUpMessageType _type, const std::string& _message);
				/**
				 * @brief Create a simple information message
				 * @param[in] _message message to display (decorated text)
				 */
				void displayInfoMessage(const std::string& _message) {
					createPopUpMessage(messageTypeInfo, _message);
				}
				/**
				 * @brief Create a simple warning message
				 * @param[in] _message message to display (decorated text)
				 */
				void displayWarningMessage(const std::string& _message) {
					createPopUpMessage(messageTypeWarning, _message);
				}
				/**
				 * @brief Create a simple error message
				 * @param[in] _message message to display (decorated text)
				 */
				void displayErrorMessage(const std::string& _message) {
					createPopUpMessage(messageTypeError, _message);
				}
				/**
				 * @brief Create a simple critical message
				 * @param[in] _message message to display (decorated text)
				 */
				void displayCriticalMessage(const std::string& _message) {
					createPopUpMessage(messageTypeCritical, _message);
				}
		};
	};
};

#endif

