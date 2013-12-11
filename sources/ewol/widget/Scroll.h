/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_SCROLL_H__
#define __EWOL_WIDGET_SCROLL_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Scroll : public widget::Container {
			public:
				enum highSpeedMode {
					speedModeDisable,
					speedModeInit,
					speedModeEnableFinger,     // Specific for touchpad
					speedModeEnableHorizontal, // Specific for mouse
					speedModeEnableVertical,   // Specific for mouse
					speedModeGrepEndEvent
				};
			public:
				// Cinfig parameter list:
				static const char* const configLimit;
			public:
				static void init(ewol::WidgetManager& _widgetManager);
			private:
				ewol::Drawing m_draw; // TODO : change in shaper ...  == > better for annimation and dynamic display ...
			protected:
				vec2 m_limit;
			private:
				float m_pixelScrolling;
				vec2 m_highSpeedStartPos;
				enum highSpeedMode m_highSpeedMode;
				int32_t m_highSpeedButton;
				enum ewol::keyEvent::type m_highSpeedType;
			public:
				Scroll(void);
				virtual ~Scroll(void);
				/**
				 * @brief set the limit of scrolling
				 * @note This permit to scoll element upper the end of the display
				 * @param[in] _limit scrolling limit [0..1] (represent a pourcent)
				 */
				void setLimit(const vec2& _limit);
				/**
				 * @brief get the limit of scrolling
				 * @return scrolling limit
				 */
				const vec2& getLimit(void) const { return m_limit; };
				
			public: // Derived function
				void calculateMinMaxSize(void);
				virtual void onRegenerateDisplay(void);
				virtual bool onEventInput(const ewol::EventInput& _event);
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual ewol::Widget* getWidgetAtPos(const vec2& _pos);
			protected: // Derived function
				virtual void onDraw(void);
				virtual bool onSetConfig(const ewol::EConfig& _conf);
				virtual bool onGetConfig(const char* _config, std::string& _result) const;
		};
	};
};

#endif
