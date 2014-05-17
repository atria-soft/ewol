/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_W_SLIDER_H__
#define __EWOL_W_SLIDER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class WSlider :public ewol::widget::ContainerN {
			public:
				static void init(ewol::widget::Manager& _widgetManager);
				// Event list of properties
				static const char* const eventStartSlide;
				static const char* const eventStopSlide;
				// Config list of properties
				// TODO : remove the dynamic transition and set this in annimation ...
				static const char* const configMode;
				static const char* const configSpeed;
				enum sladingMode {
					sladingTransitionVert,
					sladingTransitionHori,
					sladingTransition_count,
				};
			public:
				WSlider();
				virtual ~WSlider();
			private:
				int32_t m_windowsSources; //!< widget source viewed
				int32_t m_windowsDestination; //!< widget destinated viewed
				int32_t m_windowsRequested; //!< widget destination requested when change in modification in progress
				float m_slidingProgress; //!< ratio progression of a sliding
			protected:
				/**
				 * @brief Generate the move on the specific vector ID (This is not a public acces, because the vector can have some null pointer inside ...)
				 * @param[in] _id Id in the vector
				 */
				void subWidgetSelectSetVectorId(int32_t _id);
			public:
				/** 
				 * @brief Select a new subwidget to display
				 * @param[in] _id Id of the subwidget requested
				 */
				void subWidgetSelectSet(int32_t _id);
				/** 
				 * @brief Select a new subwidget to display
				 * @param[in] _widgetPointer Pointer on the widget selected (must be added before)
				 */
				void subWidgetSelectSet(ewol::object::Shared<ewol::Widget> _widgetPointer);
				/** 
				 * @brief Select a new subwidget to display
				 * @param[in] _widgetName Name of the subwidget name
				 */
				void subWidgetSelectSet(const std::string& _widgetName);
			private:
				float m_transitionSpeed; //!< speed of the transition (default 1  == > 1s)
			public:
				/** 
				 * @brief set transition speed element.
				 * @param[in] _timeSecond number of second needed to do the transition.
				 */
				void setTransitionSpeed(float _timeSecond) {
					m_transitionSpeed = _timeSecond;
				};
				/** 
				 * @brief get transition speed element.
				 * @return number of second needed to do the transition.
				 */
				float getTransitionSpeed() const {
					return m_transitionSpeed;
				};
			private:
				enum sladingMode m_transitionSlide; //!< mode to slide the widgets
			public:
				/** 
				 * @brief set a new mode of sliding element
				 * @param[in] _mode new display mode
				 */
				void setTransitionMode(enum sladingMode _mode);
				/** 
				 * @brief get a new mode of sliding element
				 * @return The current sliding mode
				 */
				enum sladingMode getTransitionMode() {
					return m_transitionSlide;
				};
			public: // Derived function
				virtual void calculateSize(const vec2& _availlable);
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual ewol::object::Shared<ewol::Widget> getWidgetAtPos(const vec2& _pos);
				virtual void periodicCall(const ewol::event::Time& _event);
				virtual bool onSetConfig(const ewol::object::Config& _conf);
				virtual bool onGetConfig(const char* _config, std::string& _result) const;
		};
	};
	std::ostream& operator <<(std::ostream& _os, const enum ewol::widget::WSlider::sladingMode _obj);
};

#endif
