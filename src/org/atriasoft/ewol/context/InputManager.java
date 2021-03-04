/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once
#include <ewol/widget/Widget.hpp>

#define MAX_MANAGE_INPUT (15)

namespace ewol {
	namespace context {
		/**
		 * @brief internal structure
		 * @not_in_doc
		 */
		class InputPoperty {
			public:
				boolean isUsed;
				int destinationInputId;
				echrono::Clock lastTimeEvent;
				WeakReference<Widget> curentWidgetEvent;
				Vector2f origin;
				Vector2f size;
				Vector2f downStart;
				Vector2f posEvent;
				boolean isDown;
				boolean isInside;
				int nbClickEvent; // 0 .. 1 .. 2 .. 3
		};
		
		/**
		 * @brief internal structure
		 * @not_in_doc
		 */
		class InputLimit {
			public:
				echrono::Duration sepatateTime;
				int DpiOffset;
		};
		class Context;
		class InputManager{
			// special grab pointer mode : 
			private:
				WeakReference<Widget> this.grabWidget; //!< widget that grab the curent pointer.
			private:
				int this.dpi;
				InputLimit this.eventInputLimit;
				InputLimit this.eventMouseLimit;
				void calculateLimit();
				InputPoperty this.eventInputSaved[MAX_MANAGE_INPUT];
				InputPoperty this.eventMouseSaved[MAX_MANAGE_INPUT];
				void abortElement(InputPoperty* _eventTable, int _idInput, KeyType _type);
				void cleanElement(InputPoperty* _eventTable, int _idInput);
				/**
				 * @brief generate the event on the destinated widget.
				 * @param[in] _type Type of the event that might be sended.
				 * @param[in] _destWidget Pointer on the requested widget that element might be sended
				 * @param[in] _IdInput Id of the event (PC : [0..9] and touch : [1..9])
				 * @param[in] _typeEvent type of the eventg generated
				 * @param[in] _pos position of the event
				 * @return true if event has been greped
				 */
				boolean localEventInput(KeyType _type,
				                     Widget _destWidget,
				                     int _IdInput,
				                     KeyStatus _typeEvent,
				                     Vector2f _pos);
				/**
				 * @brief convert the system event id in the correct EWOL id depending of the system management mode
				 *        This function find the next input id unused on the specifiic widget
				 *             == > on PC, the ID does not change (GUI is not the same)
				 * @param[in] _type Type of the kay event.
				 * @param[in] _destWidget Pointer of the widget destination
				 * @param[in] _realInputId system Id
				 * @return the ewol input id
				 */
				int localGetDestinationId(KeyType _type,
				                              Widget _destWidget,
				                              int _realInputId);
			private:
				EwolContext this.context;
			public:
				InputManager(EwolContext _context);
				~InputManager();
				void setDpi(int _newDPI);
				
				// note if id<0  == > the it was finger event ...
				void motion(KeyType _type, int _pointerID, Vector2f _pos );
				void state(KeyType _type, int _pointerID, boolean _isDown, Vector2f _pos);
			public:
				/**
				 * @brief a new layer on the windows is set  == > might remove all the property of the current element ...
				 */
				void newLayerSet();
				/**
				 * @brief This is to transfert the event from one widget to another one
				 * @param _source the widget where the event came from
				 * @param _destination the widget where the event mitgh be generated now
				 */
				void transfertEvent(Widget _source, Widget _destination);
				/**
				 * @brief This fonction lock the pointer properties to move in relative instead of absolute
				 * @param[in] _widget The widget that lock the pointer events
				 */
				void grabPointer(Widget _widget);
				/**
				 * @brief This fonction un-lock the pointer properties to move in relative instead of absolute
				 */
				void unGrabPointer();
			private:
				KeySpecial this.specialKey;
			public:
				void setLastKeyboardSpecial( KeySpecial _specialKey) {
					this.specialKey = _specialKey;
				}
		};
	};
};

