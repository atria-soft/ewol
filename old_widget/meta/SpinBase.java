/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/ContainerN.hpp>
#include <ewol/widget/Manager.hpp>
#include <etk/Color.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Entry.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/Sizer.hpp>

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
		class SpinBase;
		using SpinBase = ememory::Ptr<ewol::widget::SpinBase>;
		using SpinBaseWeak = ememory::WeakPtr<ewol::widget::SpinBase>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class SpinBase : public ewol::widget::Sizer {
			public: // properties list:
				eproperty::Value<etk::Uri> propertyShape; //!< Shape of the widget
				eproperty::List<enum ewol::widget::spinPosition> propertySpinMode; //!< How to display the spin base
			public:
				UN_DECLARE_FACTORY(SpinBase);
			protected:
				ememory::Ptr<ewol::resource::ConfigFile> this.config;
				int this.confIdEntryShaper;
				int this.confIdUpShaper;
				int this.confIdDownShaper;
				int this.confIdUpData;
				int this.confIdDownData;
			protected:
				/**
				 * Constructor
				 * @param _mode The mode to display the elements
				 */
				SpinBase();
				void init() ;
			public:
				/**
				 * Destructor
				 */
				 ~SpinBase();
			protected:
				ewol::widget::Entry this.widgetEntry;
				ewol::widget::Button this.widgetButtonDown;
				ewol::widget::Button this.widgetButtonUp;
				 void updateGui();
			public:
				 boolean loadXML( exml::Element _node) ;
			protected:
				 void onChangePropertySpinMode();
				 void onChangePropertyShape();
		};
	}
}

