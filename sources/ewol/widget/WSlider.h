/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_W_SLIDER_H__
#define __EWOL_W_SLIDER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace ewol {
	class WSlider :public ewol::Widget
	{
		public:
			WSlider(void);
			virtual ~WSlider(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolWSlider"; };
		private:
			etk::Vector2D<bool> m_underExpend; // expend of the uner elements ...
		public:
			virtual bool   CalculateSize(float availlableX, float availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
			virtual void   SetMinSise(float x=-1, float y=-1);
			virtual bool   CanExpentX(void);
			virtual bool   CanExpentY(void);
			void           LockExpendContamination(bool lockExpend=false);
		private:
			bool                           m_lockExpendContamination;
			etk::Vector<ewol::Widget*> m_subWidget;
			int32_t                        m_windowsSources;     // widget source viewed
			int32_t                        m_windowsDestination; // widget destinated viewed
			int32_t                        m_slidingProgress;    // ratio progression of a sliding
		public:
			virtual void    SubWidgetRemoveAll(void);
			virtual void    SubWidgetAdd(ewol::Widget* newWidget);
			virtual void    SubWidgetRemove(ewol::Widget* newWidget);
			virtual void    SubWidgetUnLink(ewol::Widget* newWidget);
			        void    SubWidgetSelectSet(int32_t id);
			        int32_t SubWidgetSelectGet(void) { return (int32_t)m_slidingProgress; };
			        int32_t SubWidgetNumber(void) { return m_subWidget.Size(); };
		protected:
			virtual void   OnDraw(DrawProperty& displayProp);
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @param[in] pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 */
			virtual ewol::Widget * GetWidgetAtPos(etk::Vector2D<float>  pos);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			/**
			 * @brief Periodic call of this widget
			 * @param localTime curent system time
			 * @return ---
			 */
			virtual void PeriodicCall(int64_t localTime);
	};
	
};

#endif
