/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/WidgetScrolled.h>
#include <ewol/compositing/Compositing.h>

namespace ewol {
	namespace widget {
		class List;
		using ListShared = ememory::SharedPtr<ewol::widget::List>;
		using ListWeak = ememory::WeakPtr<ewol::widget::List>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class List : public ewol::widget::WidgetScrolled {
			protected:
				List();
			public:
				virtual ~List();
				virtual void calculateMinMaxSize();
				void setLabel(std::string _newLabel);
			// drawing capabilities ....
			private:
				std::vector<ewol::Compositing*> m_listOObject;   //!< generic element to display...
				std::vector<ivec2 > m_lineSize;
			public:
				void addOObject(ewol::Compositing* _newObject, int32_t _pos=-1);
				void clearOObjectList();
			// list properties ...
			private:
				int32_t m_paddingSizeX;
				int32_t m_paddingSizeY;
				int32_t m_displayStartRaw; //!< Current starting diaplayed raw
				int32_t m_displayCurrentNbLine; //!< Number of line in the display
				int32_t m_nbVisibleRaw; // set the number of visible raw (calculate don display)
			protected:
				// function call to display the list :
				virtual etk::Color<> getBasicBG() {
					return etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF);
				}
				virtual uint32_t getNuberOfColomn() {
					return 1;
				};
				virtual bool getTitle(int32_t _colomn, std::string& _myTitle, etk::Color<> &_fg, etk::Color<> &_bg) {
					_myTitle = "";
					return false;
				};
				virtual uint32_t getNuberOfRaw() {
					return 0;
				};
				virtual bool getElement(int32_t _colomn, int32_t _raw, std::string &_myTextToWrite, etk::Color<> &_fg, etk::Color<> &_bg) {
					_myTextToWrite = "";
					_bg = etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF);
					_fg = etk::Color<>(0x00, 0x00, 0x00, 0xFF);
					if (_raw % 2) {
						_bg = etk::Color<>(0xFF, 0xFF, 0xFF, 0xFF);
					} else {
						_bg = etk::Color<>(0x7F, 0x7F, 0x7F, 0xFF);
					}
					return false;
				};
				virtual bool onItemEvent(int32_t _IdInput, enum gale::key::status _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y) {
					return false;
				}
				/**
				 * @brief set a raw visible in the main display
				 * @param[in] _id Id of the raw that might be visible.
				 */
				void setRawVisible(int32_t _id);
			protected: // Derived function
				virtual void onGetFocus();
				virtual void onLostFocus();
				virtual void onDraw();
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
		};
	};
};

