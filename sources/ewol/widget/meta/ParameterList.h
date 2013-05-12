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
			virtual ~ParameterList(void);
			void SetLabel(etk::UString newLabel);
		// Drawing capabilities ....
		private:
			etk::Vector<ewol::Compositing*> m_listOObject;   //!< generic element to display...
		public:
			void AddOObject(ewol::Compositing* newObject, int32_t pos=-1);
			void ClearOObjectList(void);
		// list properties ...
		private:
			int32_t m_paddingSizeX;
			int32_t m_paddingSizeY;
			int32_t m_displayStartRaw; //!< Current starting diaplayed raw
			int32_t m_displayCurrentNbLine; //!< Number of line in the display
		public:
			void MenuAdd(etk::UString& label, int32_t refId, etk::UString& image);
			void MenuAddGroup(etk::UString& label);
			void MenuClear(void);
			void MenuSeparator(void);
			
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "EwolParameterList"; };
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual void CalculateMinMaxSize(void);
		protected: // Derived function
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
			virtual void OnDraw(void);
	};
};

#define EWOL_CAST_WIDGET_PARAMETER_LIST(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_PARAMETER_LIST,ParameterList,curentPointer)

#endif

