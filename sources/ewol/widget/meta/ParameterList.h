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
			void setLabel(etk::UString newLabel);
		// drawing capabilities ....
		private:
			etk::Vector<ewol::Compositing*> m_listOObject;   //!< generic element to display...
		public:
			void addOObject(ewol::Compositing* newObject, int32_t pos=-1);
			void clearOObjectList(void);
		// list properties ...
		private:
			int32_t m_paddingSizeX;
			int32_t m_paddingSizeY;
			int32_t m_displayStartRaw; //!< Current starting diaplayed raw
			int32_t m_displayCurrentNbLine; //!< Number of line in the display
		public:
			void menuAdd(etk::UString& label, int32_t refId, etk::UString& image);
			void menuAddGroup(etk::UString& label);
			void menuClear(void);
			void menuSeparator(void);
			
		public: // Derived function
			virtual const char * const getObjectType(void) { return "EwolParameterList"; };
			virtual void onRegenerateDisplay(void);
			virtual bool onEventInput(const ewol::EventInput& _event);
			virtual void calculateMinMaxSize(void);
		protected: // Derived function
			virtual void onGetFocus(void);
			virtual void onLostFocus(void);
			virtual void onDraw(void);
	};
};


#endif

