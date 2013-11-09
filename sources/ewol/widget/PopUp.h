/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_POP_UP_H__
#define __EWOL_POP_UP_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/WidgetManager.h>

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class PopUp : public widget::Container {
		public:
			static void init(ewol::WidgetManager& _widgetManager);
			// Config list of properties
			static const char* const configShaper;
			static const char* const configRemoveOnExternClick;
			static const char* const configAnimation;
			static const char* const configLockExpand;
		private:
			ewol::Shaper m_shaper; //!< Compositing theme.
		public:
			/**
			 * @brief Constructor
			 * @param[in] _shaperName Shaper file properties
			 */
			PopUp(const etk::UString& _shaperName="THEME:GUI:widgetPopUp.conf");
			/**
			 * @brief Destructor
			 */
			virtual ~PopUp(void);
			/**
			 * @brief set the shaper name (use the contructer one this permit to not noad unused shaper)
			 * @param[in] _shaperName The new shaper filename
			 */
			void setShaperName(const etk::UString& _shaperName);
		protected:
			bvec2 m_lockExpand; //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
		public:
			/**
			 * @brief Limit the expend properties to the current widget (no contamination)
			 * @param[in] _lockExpend Lock mode of the expend properties
			 */
			void lockExpand(const bvec2& _lockExpand);
		private:
			bool m_closeOutEvent; //!< ratio progression of a sliding
		public:
			/**
			 * @brief Request the Auto-remove when the event input is set outside the widget
			 * @param[in] _state New status
			 */
			void setRemoveOnExternClick(bool _state) {
				m_closeOutEvent = _state;
			};
			/**
			 * @brief get the status of the request the Auto-remove when the event input is set outside the widget.
			 * @return the status of the removing
			 */
			bool getRemoveOnExternClick(void) const {
				return m_closeOutEvent;
			};
		private:
			//float m_slidingProgress; //!< ratio progression of a sliding
		public:
			typedef enum {
				animationNone, //!< No annimation
				animationIncrease, //!< element came strart with no size and increase with the corect size.
			} animation_te;
		private:
			animation_te m_animation; //!< Methode add and remove element (animation)
		public:
			/**
			 * @brief set an animation mode for the new element set in the Widget container.
			 * @param[in] _animation The new animation mode.
			 */
			void setAnimationMode(animation_te _animation);
			/**
			 * @brief get the current animation mode.
			 * @return The animation mode.
			 */
			animation_te getAnimationMode(void) const {
				return m_animation;
			};
			
		protected: // Derived function
			virtual void onDraw(void);
			virtual bool onSetConfig(const ewol::EConfig& _conf);
			virtual bool onGetConfig(const char* _config, etk::UString& _result) const;
		public: // Derived function
			virtual void periodicCall(const ewol::EventTime& _event);
			virtual void systemDraw(const ewol::DrawProperty& _displayProp);
			virtual void onRegenerateDisplay(void);
			virtual void calculateSize(const vec2& _available);
			virtual bool onEventInput(const ewol::EventInput& _event);
			//virtual void calculateMinMaxSize(void);
			virtual const char * const getObjectType(void) {
				return "ewol::PopUp";
			};
			virtual ewol::Widget* getWidgetAtPos(const vec2& pos);
	};
	
};

#endif
