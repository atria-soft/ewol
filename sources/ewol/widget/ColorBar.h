/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COLORBAR_H__
#define __EWOL_COLORBAR_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <etk/Color.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>


namespace ewol {
	namespace widget {
		class ColorBar :public ewol::Widget {
			public:
				// Event list of properties
				static const char * const eventChange;
			public:
				ColorBar(void);
				virtual ~ColorBar(void);
				etk::Color<> getCurrentColor(void);
				void setCurrentColor(etk::Color<> _newOne);
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element
				etk::Color<> m_currentColor;
				vec2 m_currentUserPos;
			protected: // Derived function
				virtual void onDraw(void);
			public: // Derived function
				virtual void calculateMinMaxSize(void);
				virtual void onRegenerateDisplay(void);
				virtual bool onEventInput(const ewol::event::Input& _event);
		};
	};
};

#endif
