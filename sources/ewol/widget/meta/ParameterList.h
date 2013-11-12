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

extern const char * const ewolEventParameterListSelect;


namespace widget {
	class elementPL {
		public :
			bool            m_group;
			std::string    m_label;
			int32_t         m_refId;
			std::string    m_image;
			elementPL(std::string& _label, int32_t _refId, std::string& _image, bool _isGroup) :
			  m_group(_isGroup),
			  m_label(_label),
			  m_refId(_refId),
			  m_image(_image) {
				
			};
			~elementPL(void) {};
	};
	
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class ParameterList :public widget::WidgetScrooled {
		private:
			int32_t m_idSelected;
			std::vector<widget::elementPL *> m_list;
		public:
			ParameterList(void);
			virtual ~ParameterList(void);
			void setLabel(std::string _newLabel);
		// drawing capabilities ....
		private:
			std::vector<ewol::Compositing*> m_listOObject; //!< generic element to display...
		public:
			void addOObject(ewol::Compositing* _newObject, int32_t _pos=-1);
			void clearOObjectList(void);
		// list properties ...
		private:
			int32_t m_paddingSizeX;
			int32_t m_paddingSizeY;
			int32_t m_displayStartRaw; //!< Current starting diaplayed raw
			int32_t m_displayCurrentNbLine; //!< Number of line in the display
		public:
			void menuAdd(std::string& _label, int32_t _refId, std::string& _image);
			void menuAddGroup(std::string& _label);
			void menuClear(void);
			void menuSeparator(void);
			
		public: // Derived function
			virtual const char * const getObjectType(void) {
				return "widgetParameterList";
			};
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

