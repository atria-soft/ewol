/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>

namespace appl {
	namespace widget {
		class VectorDisplay : public ewol::Widget {
			protected:
				ewol::compositing::Drawing m_draw; //!< drawing instance
			protected:
				//! @brief constructor
				VectorDisplay();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(VectorDisplay, "VectorDisplay");
				//! @brief destructor
				virtual ~VectorDisplay();
			protected:
				std::vector<float> m_data; //!< data that might be displayed
			public:
				void setValue(const std::vector<float>& _data);
			protected:
				bool m_autoDisplay;
			public:
				void ToggleAuto();
			protected:
				float m_minVal; //!< display minimum value
				float m_maxVal; //!< display maximum value
			public:
				void onDraw() override;
				void onRegenerateDisplay() override;
				void periodicEvent(const ewol::event::Time& _event);
		};
	}
}

