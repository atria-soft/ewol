/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Container.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/compositing/Shaper.hpp>
#include <ewol/widget/Manager.hpp>

namespace ewol {
	namespace widget {
		class PopUp;
		using PopUpShared = ememory::SharedPtr<ewol::widget::PopUp>;
		using PopUpWeak = ememory::WeakPtr<ewol::widget::PopUp>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class PopUp : public ewol::widget::Container {
			public: // properties
				eproperty::Value<std::string> propertyShape; //!< Compositing theme.
				eproperty::Value<bvec2> propertyLockExpand; //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
				eproperty::Value<bool> propertyCloseOutEvent; //!< ratio progression of a sliding
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _shaperName Shaper file properties
				 */
				PopUp();
				void init() override;
			public:
				DECLARE_WIDGET_FACTORY(PopUp, "PopUp");
				/**
				 * @brief Destructor
				 */
				virtual ~PopUp();
			protected:
				ewol::compositing::Shaper m_shaper; //!< Compositing theme.
			protected:
				void onDraw() override;
			public:
				void periodicCall(const ewol::event::Time& _event) override;
				void systemDraw(const ewol::DrawProperty& _displayProp) override;
				void onRegenerateDisplay() override;
				void onChangeSize() override;
				bool onEventInput(const ewol::event::Input& _event) override;
				ewol::WidgetShared getWidgetAtPos(const vec2& _pos) override;
			protected:
				bool onStartAnnimation(enum ewol::Widget::annimationMode _mode) override;
				void onStopAnnimation() override;
			protected:
				virtual void onChangePropertyShape();
				virtual void onChangePropertyLockExpand();
		};
	};
};

