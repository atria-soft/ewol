/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/WidgetScrolled.hpp>

namespace ewol {
	namespace widget {
		class elementPL {
			public :
				bool m_group;
				etk::String m_label;
				int32_t m_refId;
				etk::String m_image;
				elementPL(etk::String& _label, int32_t _refId, etk::String& _image, bool _isGroup) :
				  m_group(_isGroup),
				  m_label(_label),
				  m_refId(_refId),
				  m_image(_image) {
					
				};
				virtual ~elementPL() {};
		};
		class ParameterList;
		using ParameterListShared = ememory::SharedPtr<ewol::widget::ParameterList>;
		using ParameterListWeak = ememory::WeakPtr<ewol::widget::ParameterList>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ParameterList :public ewol::widget::WidgetScrolled {
			public: //signals
				esignal::Signal<int32_t> signalSelect;
			public: // properties
				
			private:
				int32_t m_idSelected;
				etk::Vector<ememory::SharedPtr<ewol::widget::elementPL>> m_list;
			protected:
				ParameterList();
				void init() override;
			public:
				DECLARE_WIDGET_FACTORY(ParameterList, "ParameterList");
				virtual ~ParameterList();
				void setLabel(etk::String _newLabel);
			// drawing capabilities ....
			private:
				etk::Vector<ememory::SharedPtr<ewol::Compositing>> m_listOObject; //!< generic element to display...
			public:
				void addOObject(const ememory::SharedPtr<ewol::Compositing>& _newObject, int32_t _pos=-1);
				void clearOObjectList();
			// list properties ...
			private:
				int32_t m_paddingSizeX;
				int32_t m_paddingSizeY;
				int32_t m_displayStartRaw; //!< Current starting diaplayed raw
				int32_t m_displayCurrentNbLine; //!< Number of line in the display
			public:
				void menuAdd(etk::String& _label, int32_t _refId, etk::String& _image);
				void menuAddGroup(etk::String& _label);
				void menuClear();
				void menuSeparator();
				
			public:
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
				void calculateMinMaxSize() override;
			protected:
				void onGetFocus() override;
				void onLostFocus() override;
				void onDraw() override;
		};
	};
};


