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


namespace widget {
	class WSlider :public widget::ContainerN
	{
		public:
			static void Init(void);
			static void UnInit(void);
			// Event list of properties
			static const char* const eventStartSlide;
			static const char* const eventStopSlide;
			// Config list of properties
			static const char* const configMode;
			typedef enum {
				sladingTransitionVert,
				sladingTransitionHori,
				sladingTransition_count,
			} sladingMode_te;
		public:
			WSlider(void);
			virtual ~WSlider(void);
		private:
			int32_t m_windowsSources; //!< widget source viewed
			int32_t m_windowsDestination; //!< widget destinated viewed
			float m_slidingProgress; //!< ratio progression of a sliding
			int64_t m_lastPeriodicCall;
			float m_transitionSpeed; //!< speed of the transition (default 1 ==> 1s)
			sladingMode_te m_transitionSlide; //!< mode to slide the widgets
		public:
			/** 
			 * @brief Select a new subwidget to display
			 * @param[in] _id Id of the subwidget requested
			 */
			void SubWidgetSelectSet(int32_t _id);
			/** 
			 * @brief Select a new subwidget to display
			 * @param[in] _widgetPointer Pointer on the widget selected (must be added before)
			 */
			void SubWidgetSelectSet(ewol::Widget* _widgetPointer);
			/** 
			 * @brief Select a new subwidget to display
			 * @param[in] _widgetName Name of the subwidget name
			 */
			void SubWidgetSelectSet(const etk::UString& _widgetName);
			/** 
			 * @brief Set a new mode of sliding element
			 * @param[in] _mode new display mode
			 */
			void SetTransitionMode(sladingMode_te _mode);
			/** 
			 * @brief Get a new mode of sliding element
			 * @return The current sliding mode
			 */
			sladingMode_te GetTransitionMode(void) { return m_transitionSlide; };
			
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::WSlider"; };
			virtual void CalculateSize(const vec2& _availlable);
			virtual void SystemDraw(const ewol::DrawProperty& _displayProp);
			virtual void OnRegenerateDisplay(void);
			virtual void PeriodicCall(int64_t _localTime);
			virtual bool OnSetConfig(const ewol::EConfig& _conf);
			virtual bool OnGetConfig(const char* _config, etk::UString& _result) const;
	};
	
	etk::CCout& operator <<(etk::CCout& _os, const widget::WSlider::sladingMode_te _obj);
};

#endif
