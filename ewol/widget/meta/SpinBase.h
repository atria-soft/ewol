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
			public: // properties list:
				eproperty::Value<std::string> propertyShape; //!< Shape of the widget
				eproperty::List<enum ewol::widget::spinPosition> propertySpinMode; //!< How to display the spin base
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
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _mode The mode to display the elements
				 */
				SpinBase();
				void init(enum ewol::widget::spinPosition _mode,
				          const std::string& _shaperName);
			public:
				/**
				 * @brief Destructor
				 */
				virtual ~SpinBase();
			protected:
				std::shared_ptr<ewol::widget::Entry> m_widgetEntry;
				std::shared_ptr<ewol::widget::Button> m_widgetButtonDown;
				std::shared_ptr<ewol::widget::Button> m_widgetButtonUp;
				virtual void updateGui();
			public: // Derived function
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
				virtual bool loadXML(const std::shared_ptr<const exml::Element>& _node);
		};
	}
}

