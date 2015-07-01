/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#ifndef __APPL_WIDGET_VECTOR_DISPLAY_H__
#define __APPL_WIDGET_VECTOR_DISPLAY_H__

#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>

namespace appl {
	namespace widget {
		class VectorDisplay : public ewol::Widget {
			private:
				ewol::compositing::Drawing m_draw; //!< drawing instance
			protected:
				//! @brief constructor
				VectorDisplay();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(VectorDisplay, "VectorDisplay");
				//! @brief destructor
				virtual ~VectorDisplay();
			private:
				std::vector<float> m_data; //!< data that might be displayed
			public:
				void setValue(const std::vector<float>& _data);
			private:
				bool m_autoDisplay;
			public:
				void ToggleAuto();
			private:
				float m_minVal; //!< display minimum value
				float m_maxVal; //!< display maximum value
			public: // herited function
				virtual void onDraw();
				virtual void onRegenerateDisplay();
				virtual void periodicCall(const ewol::event::Time& _event);
		};
	};
};

#endif
