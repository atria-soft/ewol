/**
 *******************************************************************************
 * @file os/eSystemInput.h
 * @brief Input (mouse,finger) abstraction layer (header)
 * @author Edouard DUPIN
 * @date 00/04/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */



#ifndef __EWOL_SYSTEM_INPUT_H__
#define __EWOL_SYSTEM_INPUT_H__

#include <ewol/widget/Widget.h>

namespace ewol
{
	
	// internal structure
	typedef struct {
		bool            isUsed;
		int32_t         destinationInputId;
		int64_t         lastTimeEvent;
		ewol::Widget*   curentWidgetEvent;
		Vector2D<float> origin;
		Vector2D<float> size;
		Vector2D<float> downStart;
		bool            isDown;
		bool            isInside;
		int32_t         nbClickEvent; // 0 .. 1 .. 2 .. 3
	} InputPoperty_ts;
	
	typedef struct {
		int32_t sepatateTime;
		int32_t DpiOffset;
	} inputLimit_ts;
	
	class eSystemInput
	{
		private:
			int32_t         m_dpi;
			inputLimit_ts   m_eventInputLimit;
			inputLimit_ts   m_eventMouseLimit;
			void CalculateLimit(void);
			#define MAX_MANAGE_INPUT         (10)
			InputPoperty_ts m_eventInputSaved[MAX_MANAGE_INPUT];
			InputPoperty_ts m_eventMouseSaved[MAX_MANAGE_INPUT];
			void CleanElement(InputPoperty_ts *eventTable, int32_t idInput);
			bool localEventInput(ewol::inputType_te type, ewol::Widget* destWidget, int32_t IdInput, ewol::eventInputType_te typeEvent, Vector2D<float> pos);
			int32_t localGetDestinationId(ewol::inputType_te type, ewol::Widget* destWidget, int32_t realInputId);
		public:
			eSystemInput(void);
			~eSystemInput(void);
			void Reset(void);
			void SetDpi(int32_t newDPI);
			
			// note if id<0 ==> the it was finger event ...
			void Motion(ewol::inputType_te type, int pointerID, Vector2D<float>  pos );
			void State(ewol::inputType_te type, int pointerID, bool isDown, Vector2D<float>  pos);
			
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			void OnObjectRemove(ewol::EObject * removeObject);
			/**
			 * @brief a new layer on the windows is set ==> might remove all the property of the current element ...
			 * @param ---
			 * @return ---
			 */
			void NewLayerSet(void);
	};
};
#endif


