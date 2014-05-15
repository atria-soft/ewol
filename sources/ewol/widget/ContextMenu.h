/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_CONTEXT_MENU_H__
#define __EWOL_CONTEXT_MENU_H__

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ContextMenu : public ewol::Widget::Container {
			public:
				enum markPosition {
					markTop,
					markRight,
					markButtom,
					markLeft,
					markNone
				};
			public:
				static void init(ewol::object::Shared<ewol::Widget::Manager> _widgetManager);
				// Config list of properties
				static const char* const configArrowPosition;
				static const char* const configArrowMode;
				static const char* const configShaper;
			public:
				ContextMenu(const std::string& _shaperName="THEME:GUI:ContextMenu.json");
				virtual ~ContextMenu();
			private:
				ewol::compositing::Shaper m_shaper; //!< Compositing theme.
			public:
				/**
				 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper)
				 * @param[in] _shaperName The new shaper filename
				 */
				void setShaperName(const std::string& _shaperName);
			private:
				// TODO : Rework the displayer ....
				ewol::compositing::Drawing m_compositing;
				etk::Color<> m_colorBackGroung;
				etk::Color<> m_colorBorder;
				
				float m_offset;
			private:
				vec2 m_arrowPos;
				enum markPosition m_arrawBorder;
			public:
				void setPositionMark(enum markPosition position, vec2 arrowPos);
			protected: // Derived function
				virtual void onDraw();
				virtual bool onSetConfig(const ewol::object::Config& _conf);
				virtual bool onGetConfig(const char* _config, std::string& _result) const;
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual void calculateSize(const vec2& _availlable);
				virtual void calculateMinMaxSize();
				virtual ewol::object::Shared<ewol::Widget> getWidgetAtPos(const vec2& _pos);
		};
	};
};

#endif
