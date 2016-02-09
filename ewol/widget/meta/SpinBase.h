/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/Manager.h>
#include <etk/Color.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Sizer.h>

namespace ewol {
	namespace widget {
		enum spinPosition {
			/** show like:
			 *    *------------------------*
			 *    |                        |
			 *    *------------------------*
			 */
			spinPosition_noneNone,
			/** show like:
			 *    *--------------------*---*
			 *    |                    |   |
			 *    *--------------------*---*
			 */
			spinPosition_noneRight,
			/** show like:
			 *    *---*--------------------*
			 *    |   |                    |
			 *    *---*--------------------*
			 */
			spinPosition_leftNone,
			/** show like:
			 *    *---*----------------*---*
			 *    |   |                |   |
			 *    *---*----------------*---*
			 */
			spinPosition_leftRight,
			/** show like:
			 *    *---*---*----------------*
			 *    |   |   |                |
			 *    *---*---*----------------*
			 */
			spinPosition_leftLeft,
			/** show like:
			 *    *----------------*---*---*
			 *    |                |   |   |
			 *    *----------------*---*---*
			 */
			spinPosition_RightRight
		};
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class SpinBase : public ewol::widget::Sizer {
			public:
				/**
				 * @brief Main call of recording the widget on the List of "widget named creator"
				 */
				static void init(ewol::widget::Manager& _widgetManager);
			public:
			protected:
				std::shared_ptr<ewol::resource::ConfigFile> m_config;
				int32_t m_confIdEntryShaper;
				int32_t m_confIdUpShaper;
				int32_t m_confIdDownShaper;
				int32_t m_confIdUpData;
				int32_t m_confIdDownData;
				ewol::parameter::List<enum ewol::widget::spinPosition> m_spinMode; //!< How to display the spin base
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _mode The mode to display the elements
				 */
				SpinBase();
				void init(enum ewol::widget::spinPosition _mode=ewol::widget::spinPosition_RightRight,
				          const std::string& _shaperName="{ewol}THEME:GUI:Spin.json");
			public:
				/**
				 * @brief Destructor
				 */
				virtual ~SpinBase();
				/**
				 * @brief set the mode to display elements.
				 * @param[in] _mode The mode to display the elements.
				 */
				void setMode(enum ewol::widget::spinPosition _mode) {
					m_spinMode.set(_mode);
				}
				/**
				 * @brief get the mode to display elements.
				 * @return The current mode to display the elements.
				 */
				enum ewol::widget::spinPosition getMode() const {
					return m_spinMode;
				}
			protected:
				std::shared_ptr<ewol::widget::Entry> m_widgetEntry;
				std::shared_ptr<ewol::widget::Button> m_widgetButtonDown;
				std::shared_ptr<ewol::widget::Button> m_widgetButtonUp;
				virtual void updateGui();
			public: // Derived function
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
		};
	}
}

