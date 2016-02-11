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
			protected:
				eproperty::Value<ewol::compositing::Shaper> m_shaper; //!< Compositing theme.
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
				/**
				 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper)
				 * @param[in] _shaperName The new shaper filename
				 */
				void setShaperName(const std::string& _shaperName);
			protected:
				eproperty::Value<bvec2> m_lockExpand; //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
			public:
				/**
				 * @brief Limit the expend properties to the current widget (no contamination)
				 * @param[in] _lockExpend Lock mode of the expend properties
				 */
				void lockExpand(const bvec2& _lockExpand) {
					m_lockExpand.set(_lockExpand);
				}
			private:
				eproperty::Value<bool> m_closeOutEvent; //!< ratio progression of a sliding
			public:
				/**
				 * @brief Request the Auto-remove when the event input is set outside the widget
				 * @param[in] _state New status
				 */
				void setRemoveOnExternClick(bool _state) {
					m_closeOutEvent.set(_state);
				};
				/**
				 * @brief get the status of the request the Auto-remove when the event input is set outside the widget.
				 * @return the status of the removing
				 */
				bool getRemoveOnExternClick() const {
					return m_closeOutEvent;
				};
			protected: // Derived function
				virtual void onDraw();
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
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
		};
	};
};

