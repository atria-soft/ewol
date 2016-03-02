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
#include <ewol/widget/Widget.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
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
				void init(const std::string& _shaperName="{ewol}THEME:GUI:PopUp.json");
			public:
				DECLARE_WIDGET_FACTORY(PopUp, "PopUp");
				/**
				 * @brief Destructor
				 */
				virtual ~PopUp();
			protected:
				ewol::compositing::Shaper m_shaper; //!< Compositing theme.
			protected: // Derived function
				virtual void onDraw();
			public: // Derived function
				virtual void periodicCall(const ewol::event::Time& _event);
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual void onChangeSize();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos);
			protected:
				virtual bool onStartAnnimation(enum ewol::Widget::annimationMode _mode);
				virtual void onStopAnnimation();
			protected:
				virtual void onChangePropertyShape();
				virtual void onChangePropertyLockExpand();
		};
	};
};

