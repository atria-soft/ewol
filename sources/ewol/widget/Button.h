/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#ifndef __EWOL_BUTTON_H__
#define __EWOL_BUTTON_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Image.h>
#include <ewol/compositing/Shaper.h>


namespace widget {
	/**
	 * @brief a composed button is a button with an inside composed with the specify XML element ==> this permit to generate standard element simple
	 */
	class Button : public ewol::Widget
	{
		public:
			static void Init(void);
			static void UnInit(void);
			// Event list of properties
			static const char* const eventPressed;
			static const char* const eventDown;
			static const char* const eventUp;
			static const char* const eventEnter;
			static const char* const eventLeave;
			static const char* const eventValue;
			// Config list of properties
			static const char* const configToggle;
			static const char* const configLock;
			static const char* const configValue;
			typedef enum {
				lockNone, //!< normal status of the button
				lockWhenPressed, //!< When the state is set in pressed, the status stay in this one
				lockWhenReleased, //!< When the state is set in not pressed, the status stay in this one
				lockAccess, //!< all event are trashed ==> acctivity of the button is disable
			} buttonLock_te;
		private:
			ewol::Shaper m_shaper; //!< Compositing theme.
		public:
			/**
			 * @brief Constructor
			 * @param[in] _newLabel Button Label to display
			 */
			Button(const etk::UString& _shaperName="THEME:GUI:widgetButton.conf");
			/**
			 * @brief Destructor
			 */
			virtual ~Button(void);
			/**
			 * @brief Set the shaper name (use the contructer one this permit to not noad unused shaper)
			 * @param[in] _shaperName The new shaper filename
			 */
			void SetShaperName(const etk::UString& _shaperName);
		protected:
			ewol::Widget* m_subWidget[2]; //!< subwidget of the button
		public:
			/**
			 * @brief Specify the current widget
			 * @param[in] _subWidget Widget to add normal
			 */
			void SetSubWidget(ewol::Widget* _subWidget);
			/**
			 * @brief Specify the current widget
			 * @param[in] _subWidget Widget to add Toggle
			 */
			void SetSubWidgetToggle(ewol::Widget* _subWidget);
			/**
			 * @brief Get the current displayed composition
			 * @return The base widget
			 */
			ewol::Widget* GetSubWidget(void) const { return m_subWidget[0]; };
			/**
			 * @brief Get the current displayed composition
			 * @return The toggle widget
			 */
			ewol::Widget* GetSubWidgetToggle(void) const { return m_subWidget[1]; };
		protected:
			bool m_value; //!< Current state of the button.
		public:
			/**
			 * @brief Set the currentValue of the Button (pressed or not)
			 * @note Work only in toggle mode
			 * @param[in] _val New value of the button
			 */
			void SetValue(bool _val);
			/**
			 * @brief Get the current button value.
			 * @return True : The button is pressed.
			 * @return false : The button is released.
			 */
			bool GetValue(void) const { return m_value; };
		protected:
			buttonLock_te m_lock; //!< Current lock state of the button.
		public:
			/**
			 * @brief Set the button lock state.
			 * @param[in] _lock New lock mode of the button
			 */
			void SetLock(buttonLock_te _lock);
			/**
			 * @brief Get the current button lock value.
			 * @return The requested lock mode
			 */
			buttonLock_te GetLock(void) const { return m_lock; };
		protected:
			bool m_toggleMode; //!< The button is able to toggle.
		public:
			/**
			 * @brief Change the toggle mode.
			 * @param[in] _togg New toggle mode
			 */
			void SetToggleMode(bool _togg);
			/**
			 * @brief Get the current toggle mode.
			 * @return the current toggle mode.
			 */
			bool GetToggleMode(void) const { return m_toggleMode; };
		private:
			bool m_mouseHover; //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
			bool m_buttonPressed; //!< Flag to know if the button is curently pressed.
			// hover area :
			vec2 m_selectableAreaPos; //!< Start position of the events
			vec2 m_selectableAreaSize; //!< Size of the event positions
		private:
			/**
			 * @brief Internal system to Change the property of the current status
			 * @param[in] _newStatusId new state
			 */
			void ChangeStatusIn(int32_t _newStatusId);
			/**
			 * @brief update the status with the internal satte of the button ...
			 */
			void CheckStatus(void);
		protected: // Derived function
			virtual void OnDraw(void);
			virtual bool OnSetConfig(const ewol::EConfig& _conf);
			virtual bool OnGetConfig(const char* _config, etk::UString& _result) const;
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "widget::Button"; };
			virtual void CalculateMinMaxSize(void);
			virtual void CalculateSize(const vec2& _availlable);
			virtual void OnRegenerateDisplay(void);
			virtual void SystemDraw(const ewol::DrawProperty& _displayProp);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual bool OnEventEntry(const ewol::EventEntry& _event);
			virtual bool LoadXML(TiXmlNode* _node);
		private: // derived function
			virtual void PeriodicCall(int64_t _localTime);
	};
};

#endif
