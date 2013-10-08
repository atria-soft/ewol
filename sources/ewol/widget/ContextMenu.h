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
#include <ewol/widget/WidgetManager.h>

namespace widget {
	typedef enum {
		CONTEXT_MENU_MARK_TOP,
		CONTEXT_MENU_MARK_RIGHT,
		CONTEXT_MENU_MARK_BOTTOM,
		CONTEXT_MENU_MARK_LEFT,
		CONTEXT_MENU_MARK_NONE
	}markPosition_te;
	class ContextMenu : public widget::Container
	{
		public:
			static void init(ewol::WidgetManager& _widgetManager);
			// Config list of properties
			static const char* const configArrowPosition;
			static const char* const configArrowMode;
			static const char* const configShaper;
		public:
			ContextMenu(const etk::UString& _shaperName="THEME:GUI:widgetContextMenu.conf");
			virtual ~ContextMenu(void);
		private:
			ewol::Shaper m_shaper; //!< Compositing theme.
		public:
			/**
			 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper)
			 * @param[in] _shaperName The new shaper filename
			 */
			void setShaperName(const etk::UString& _shaperName);
		private:
			// TODO : Rework the displayer ....
			ewol::Drawing m_compositing;
			etk::Color<> m_colorBackGroung;
			etk::Color<> m_colorBorder;
			
			float m_offset;
		private:
			vec2 m_arrowPos;
			markPosition_te m_arrawBorder;
		public:
			void setPositionMark(markPosition_te position, vec2 arrowPos);
		protected: // Derived function
			virtual void onDraw(void);
			virtual bool onSetConfig(const ewol::EConfig& _conf);
			virtual bool onGetConfig(const char* _config, etk::UString& _result) const;
		public: // Derived function
			virtual void onRegenerateDisplay(void);
			virtual bool onEventInput(const ewol::EventInput& _event);
			virtual void calculateSize(const vec2& availlable);
			virtual void calculateMinMaxSize(void);
			virtual const char * const getObjectType(void) { return "ewol::ContextMenu"; };
			virtual ewol::Widget* getWidgetAtPos(const vec2& pos);
	};
	
};

#endif
