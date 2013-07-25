/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_LIST_H__
#define __EWOL_LIST_H__

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/WidgetScrolled.h>
#include <ewol/compositing/Compositing.h>

namespace widget {
	class List : public widget::WidgetScrooled
	{
		public:
			List(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "ewol::List"; };
			virtual ~List(void);
			virtual void CalculateMinMaxSize(void);
			void SetLabel(etk::UString _newLabel);
		// Drawing capabilities ....
		private:
			etk::Vector<ewol::Compositing*> m_listOObject;   //!< generic element to display...
			etk::Vector<ivec2 > m_lineSize;
		public:
			void AddOObject(ewol::Compositing* _newObject, int32_t _pos=-1);
			void ClearOObjectList(void);
		// list properties ...
		private:
			int32_t m_paddingSizeX;
			int32_t m_paddingSizeY;
			int32_t m_displayStartRaw; //!< Current starting diaplayed raw
			int32_t m_displayCurrentNbLine; //!< Number of line in the display
			int32_t m_nbVisibleRaw; // set the number of visible raw (calculate don display)
		protected:
			// function call to display the list :
			virtual etk::Color<> GetBasicBG(void) {
				return etk::Color<>(0xFFFFFFFF);
			}
			virtual uint32_t GetNuberOfColomn(void) {
				return 1;
			};
			virtual bool GetTitle(int32_t _colomn, etk::UString& _myTitle, etk::Color<> &_fg, etk::Color<> &_bg) {
				_myTitle = "";
				return false;
			};
			virtual uint32_t GetNuberOfRaw(void) {
				return 0;
			};
			virtual bool GetElement(int32_t _colomn, int32_t _raw, etk::UString &_myTextToWrite, etk::Color<> &_fg, etk::Color<> &_bg) {
				_myTextToWrite = "";
				_bg = 0xFFFFFFFF;
				_fg = 0x000000FF;
				if (_raw % 2) {
					_bg = 0xFFFFFFFF;
				} else {
					_bg = 0x7F7F7FFF;
				}
				return false;
			};
			virtual bool OnItemEvent(int32_t _IdInput, ewol::keyEvent::status_te _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y) {
				return false;
			}
			/**
			 * @brief Set a raw visible in the main display
			 * @param[in] _id Id of the raw that might be visible.
			 */
			void SetRawVisible(int32_t _id);
		protected: // Derived function
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
			virtual void OnDraw(void);
		public: // Derived function
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
	};
	
};

#endif
