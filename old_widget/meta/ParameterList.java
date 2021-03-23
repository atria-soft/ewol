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
				boolean this.group;
				String this.label;
				int this.refId;
				String this.image;
				elementPL(String _label, int _refId, String _image, boolean _isGroup) :
				  this.group(_isGroup),
				  this.label(_label),
				  this.refId(_refId),
				  this.image(_image) {
					
				};
				 ~elementPL() {};
		};
		class ParameterList;
		using ParameterList = ememory::Ptr<ewol::widget::ParameterList>;
		using ParameterListWeak = ememory::WeakPtr<ewol::widget::ParameterList>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ParameterList :public ewol::widget::WidgetScrolled {
			public: //signals
				esignal::Signal<int> signalSelect;
			public: // properties
				
			private:
				int this.idSelected;
				List<ememory::Ptr<ewol::widget::elementPL>> this.list;
			protected:
				ParameterList();
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(ParameterList, "ParameterList");
				 ~ParameterList();
				void setLabel(String _newLabel);
			// drawing capabilities ....
			private:
				List<ememory::Ptr<ewol::Compositing>> this.listOObject; //!< generic element to display...
			public:
				void addOObject( ememory::Ptr<ewol::Compositing> _newObject, int _pos=-1);
				void clearOObjectList();
			// list properties ...
			private:
				int this.paddingSizeX;
				int this.paddingSizeY;
				int this.displayStartRaw; //!< Current starting diaplayed raw
				int this.displayCurrentNbLine; //!< Number of line in the display
			public:
				void menuAdd(String _label, int _refId, String _image);
				void menuAddGroup(String _label);
				void menuClear();
				void menuSeparator();
				
			public:
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
				void calculateMinMaxSize() ;
			protected:
				void onGetFocus() ;
				void onLostFocus() ;
				void onDraw() ;
		};
	};
};


