/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_PARAMETER_LIST_H__
#define __EWOL_WIDGET_PARAMETER_LIST_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/WidgetScrolled.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventParameterListSelect;


namespace widget {
	
	class elementPL
	{
		public :
			bool            m_group;
			etk::UString    m_label;
			int32_t         m_refId;
			etk::UString    m_image;
			elementPL(etk::UString& label, int32_t refId, etk::UString& image, bool isGroup) :
				m_group(isGroup),
				m_label(label),
				m_refId(refId),
				m_image(image)
			{ };
			~elementPL(void) {};
	};
	
	class ParameterList :public widget::WidgetScrooled
	{
		private:
			int32_t m_idSelected;
			etk::Vector<widget::elementPL *> m_list;
		public:
			ParameterList(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolParameterList"; };
			virtual ~ParameterList(void);
			// Derived function
			virtual void CalculateMinMaxSize(void);
			void SetLabel(etk::UString newLabel);
		// Drawing capabilities ....
		private:
			etk::Vector<ewol::Compositing*> m_listOObject;   //!< generic element to display...
		public:
			void AddOObject(ewol::Compositing* newObject, int32_t pos=-1);
			void ClearOObjectList(void);
		protected:
			// Derived function
			void OnDraw(ewol::DrawProperty& displayProp);
		// list properties ...
		private:
			int32_t m_paddingSizeX;
			int32_t m_paddingSizeY;
			int32_t m_displayStartRaw; //!< Current starting diaplayed raw
			int32_t m_displayCurrentNbLine; //!< Number of line in the display
		public:
			// Derived function
			void OnRegenerateDisplay(void);
			// Derived function
			bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, const vec2& pos);
		protected:
			// Derived function
			void OnGetFocus(void);
			// Derived function
			void OnLostFocus(void);
		public:
			void MenuAdd(etk::UString& label, int32_t refId, etk::UString& image);
			void MenuAddGroup(etk::UString& label);
			void MenuClear(void);
			void MenuSeparator(void);
	};
};

#define EWOL_CAST_WIDGET_PARAMETER_LIST(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_PARAMETER_LIST,ParameterList,curentPointer)

#endif

