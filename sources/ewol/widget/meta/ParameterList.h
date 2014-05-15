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



namespace ewol {
	namespace widget {
		class elementPL {
			public :
				bool m_group;
				std::string m_label;
				int32_t m_refId;
				std::string m_image;
				elementPL(std::string& _label, int32_t _refId, std::string& _image, bool _isGroup) :
				  m_group(_isGroup),
				  m_label(_label),
				  m_refId(_refId),
				  m_image(_image) {
					
				};
				~elementPL() {};
		};
		
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ParameterList :public ewol::widget::WidgetScrolled {
			public:
				// Event list of properties
				static const char * const eventSelect;
			private:
				int32_t m_idSelected;
				std::vector<ewol::widget::elementPL *> m_list;
			public:
				ParameterList();
				virtual ~ParameterList();
				void setLabel(std::string _newLabel);
			// drawing capabilities ....
			private:
				std::vector<ewol::Compositing*> m_listOObject; //!< generic element to display...
			public:
				void addOObject(ewol::Compositing* _newObject, int32_t _pos=-1);
				void clearOObjectList();
			// list properties ...
			private:
				int32_t m_paddingSizeX;
				int32_t m_paddingSizeY;
				int32_t m_displayStartRaw; //!< Current starting diaplayed raw
				int32_t m_displayCurrentNbLine; //!< Number of line in the display
			public:
				void menuAdd(std::string& _label, int32_t _refId, std::string& _image);
				void menuAddGroup(std::string& _label);
				void menuClear();
				void menuSeparator();
				
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual void calculateMinMaxSize();
			protected: // Derived function
				virtual void onGetFocus();
				virtual void onLostFocus();
				virtual void onDraw();
		};
	};
};


#endif

