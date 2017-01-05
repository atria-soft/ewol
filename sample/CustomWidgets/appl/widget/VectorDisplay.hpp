/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

//! [EWOL_SAMPLE_CW_HEADER]

#include <ewol/widget/Widget.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>

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
			public:
				void onDraw() override;
				void onRegenerateDisplay() override;
			//! [EWOL_SAMPLE_CW_HEADER]
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
				void periodicEvent(const ewol::event::Time& _event);
		};
	}
}

