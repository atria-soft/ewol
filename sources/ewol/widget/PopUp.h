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
#include <draw/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Shaper.h>

namespace widget {
	class PopUp : public widget::Container
	{
		public:
			static void Init(void);
			static void UnInit(void);
			// Config list of properties
			static const char* const configShaper;
			static const char* const configRemoveOnExternClick;
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
			 * @brief Set the shaper name (use the contructer one this permit to not noad unused shaper)
			 * @param[in] _shaperName The new shaper filename
			 */
			void SetShaperName(const etk::UString& _shaperName);
		private:
			bool m_closeOutEvent; //!< ratio progression of a sliding
		public:
			/**
			 * @brief Request the Auto-remove when the event input is set outside the widget
			 * @param[in] _state New status
			 */
			void SetRemoveOnExternClick(bool _state) { m_closeOutEvent = _state; };
			/**
			 * @brief Get the status of the request the Auto-remove when the event input is set outside the widget.
			 * @return the status of the removing
			 */
			bool GetRemoveOnExternClick(void) const { return m_closeOutEvent; };
		private:
			float m_slidingProgress; //!< ratio progression of a sliding
		public:
			/**
			 * @brief 
			 */
			
		protected: // Derived function
			virtual void OnDraw(void);
			virtual bool OnSetConfig(const ewol::EConfig& _conf);
			virtual bool OnGetConfig(const char* _config, etk::UString& _result) const;
		public: // Derived function
			virtual void OnRegenerateDisplay(void);
			virtual void CalculateSize(const vec2& _availlable);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			//virtual void CalculateMinMaxSize(void);
			virtual const char * const GetObjectType(void) { return "ewol::PopUp"; };
			virtual ewol::Widget* GetWidgetAtPos(const vec2& pos);
	};
	
};

#endif
