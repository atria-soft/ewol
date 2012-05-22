/**
 *******************************************************************************
 * @file eventInputManagement.h
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



#ifndef __EWOL_EVENT_INPUT_MANAGEMENT_H__
#define __EWOL_EVENT_INPUT_MANAGEMENT_H__

namespace ewol
{
	typedef enum {
		INPUT_TYPE_UNKNOW,
		INPUT_TYPE_MOUSE,
		INPUT_TYPE_FINGER,
	} inputType_te;
	namespace eventInput
	{
		void Init(void);
		void UnInit(void);
		// note if id<0 ==> the it was finger event ...
		void Motion(ewol::inputType_te type, int pointerID, coord2D_ts pos );
		void State(ewol::inputType_te type, int pointerID, bool isDown, coord2D_ts pos);
		
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
	}
}
#endif


