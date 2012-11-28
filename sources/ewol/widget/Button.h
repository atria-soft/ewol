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
	class Button : public ewol::Widget
	{
		private:
			ewol::Shaper                m_shaper;             //!< Compositing theme.
			ewol::Text                  m_displayText;        //!< compositing Text.
			ewol::Image                 m_displayImage;       //!< Image to display in normal mode.
			ewol::Image                 m_displayImageToggle; //!< Image to display in toggle mode.
			etk::UString                m_label;              //!< Labe to display in normal mode.
			etk::UString                m_labelToggle;        //!< Label to display when toggle mode is set ("" whenit is the same).
			bool                        m_toggleMode;         //!< The button is able to toggle.
			bool                        m_value;              //!< Current state of the button.
		public:
			/**
			 * @brief Constructor
			 * @param[in] newLabel Button Label to display
			 */
			Button(etk::UString newLabel="No Label");
			/**
			 * @brief Destructor
			 */
			virtual ~Button(void);
			/**
			 * @brief Specify the current label of the Button
			 * @param[in] newLabel The string that might be displayed
			 */
			void SetLabel(etk::UString newLabel);
			/**
			 * @brief Get the current displayed text
			 * @return The displayed string.
			 */
			etk::UString GetLabel(void);
			/**
			 * @brief Specify the current label of the Button when the button value is true
			 * @param[in] newLabel The string that might be displayed
			 */
			void SetLabelToggle(etk::UString newLabel);
			/**
			 * @brief Get the current displayed text when the button value is true
			 * @return The displayed string.
			 */
			etk::UString GetLabelToggle(void);
			/**
			 * @brief Set an image to set at the button.
			 * @param[in] imageName Filename of the image.
			 */
			void SetImage(etk::UString imageName);
			/**
			 * @brief Set the image when button is pressed.
			 * @param[in] imageName Filename of the image.
			 */
			void SetImageToggle(etk::UString imageName);
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
		public:
			// Derived function
			virtual const char * const GetObjectType(void) { return "widget::Button"; };
			// Derived function
			virtual bool   CalculateMinSize(void);
			// Derived function
			virtual void OnRegenerateDisplay(void);
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			// Derived function
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
			// Derived function
			virtual bool OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData);
		private:
			void ChangeStatusIn(int32_t newStatusId);
			// Derived function
			virtual void PeriodicCall(int64_t localTime);
	};
};

#endif
