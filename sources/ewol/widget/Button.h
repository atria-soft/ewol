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

extern const char * const ewolEventButtonPressed;
extern const char * const ewolEventButtonDown;
extern const char * const ewolEventButtonUp;
extern const char * const ewolEventButtonEnter;
extern const char * const ewolEventButtonLeave;
extern const char * const ewolEventButtonValue;


namespace widget {
	/**
	 * @brief a composed button is a button with an inside composed with the specify XML element ==> this permit to generate standard element simple
	 */
	class Button : public ewol::Widget
	{
		private:
			ewol::Shaper m_shaper; //!< Compositing theme.
			ewol::Widget* m_subWidget[2]; //!< subwidget of the button
			bool m_toggleMode; //!< The button is able to toggle.
			bool m_value; //!< Current state of the button.
			bool m_mouseHover; //!< Flag to know where the mouse is (inside the displayed widget (if not fill)).
			bool m_buttonPressed; //!< Flag to know if the button is curently pressed.
			int32_t m_imageDisplaySize; //!< Display size of the Image.
			// hover area :
			vec2 m_selectableAreaPos; //!< Start position of the events
			vec2 m_selectableAreaSize; //!< Size of the event positions
		public:
			/**
			 * @brief Constructor
			 * @param[in] newLabel Button Label to display
			 */
			Button(const etk::UString& shaperName="THEME:GUI:widgetButton.conf");
			/**
			 * @brief Destructor
			 */
			virtual ~Button(void);
			/**
			 * @brief Set the shaper name (use the contructer one this permit to not noad unused shaper)
			 * @param[in] shaperName The new shaper filename
			 */
			void SetShaperName(const etk::UString& shaperName);
			/**
			 * @brief Specify the current composition string
			 * @param[in] newLabel The string that might be displayed
			 */
			void SetSubWidget(ewol::Widget* subWidget, bool forToggle=false);
			/**
			 * @brief Get the current displayed composition
			 * @return The displayed string.
			 */
			ewol::Widget* GetSubWidget(bool fromToggle=false);
			/**
			 * @brief Set the currentValue of the Button (pressed or not)
			 * @note Work only in toggle mode
			 * @param[in] val New value of the button
			 */
			void SetValue(bool val);
			/**
			 * @brief Get the current button value.
			 * @return True : The button is pressed.
			 * @return false : The button is released.
			 */
			bool GetValue(void);
			/**
			 * @brief Change the Toggle mode.
			 * @param[in] togg New toggle mode
			 */
			void SetToggleMode(bool togg);
		private:
			/**
			 * @brief Internal system to Change the property of the current status
			 * @param[in] new state
			 */
			void ChangeStatusIn(int32_t newStatusId);
		public:
			// Derived function
			virtual const char * const GetObjectType(void) { return "widget::Button"; };
			virtual bool CalculateMinSize(void);
			virtual bool CalculateSize(float availlableX, float availlableY);
			virtual void OnRegenerateDisplay(void);
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos);
			virtual bool OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData);
		private:
			// derived function
			virtual void PeriodicCall(int64_t localTime);
	};
};

#endif
