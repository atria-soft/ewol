/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WINDOWS_H__
#define __EWOL_WINDOWS_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <etk/Color.h>

namespace ewol {
	
	class Windows :public ewol::Widget
	{
		public:
			Windows(void);
			virtual ~Windows(void);
		// internal event at ewol system : 
		public:
			void sysDraw(void);
			void sysOnShow(void) {};
			void sysOnHide(void) {};
			void sysOnKill(void) {};
		public:
			virtual void onShow(void) { };
			virtual void onHide(void) { };
			virtual bool onKill(void) { return true; };
			virtual void onReduce(void) { };
			virtual void on(void) { };
		private:
			bool m_hasDecoration;
		public:
			void setDecorationDisable(void)
			{
				m_hasDecoration = false;
			}
			
			void setDecorationEnable(void)
			{
				m_hasDecoration = true;
			}
		private:
			ewol::Widget* m_subWidget;
			etk::Vector<ewol::Widget*> m_popUpWidgetList;
		public:
			void setSubWidget(ewol::Widget * widget);
			void popUpWidgetPush(ewol::Widget * widget);
		private:
			etk::Color<float> m_backgroundColor; //!< reset color of the Main windows
		public:
			/**
			 * @brief get the background color.
			 * @return A reference on the color
			 */
			const etk::Color<float>& getBackgroundColor(void) { return m_backgroundColor; };
			/**
			 * @brief set the background color.
			 * @param[IN] the new requested color.
			 */
			void setBackgroundColor(const etk::Color<float>& _color);
		protected: // Derived function
			virtual void systemDraw(const ewol::drawProperty& _displayProp);
		public: // Derived function
			virtual const char * const getObjectType(void) { return "ewol::Windows"; };
			virtual void onRegenerateDisplay(void);
			virtual void onObjectRemove(ewol::EObject * removeObject);
			virtual void calculateSize(const vec2& availlable);
			virtual ewol::Widget * getWidgetAtPos(const vec2& pos);
			void setTitle(const etk::UString& _title);
	};
	
};

#endif

