



#ifndef __EWOL_EVENT_INPUT_MANAGEMENT_H__
#define __EWOL_EVENT_INPUT_MANAGEMENT_H__

namespace ewol
{
	namespace eventInput
	{
		void Init(void);
		void UnInit(void);
		void Motion(int pointerID, coord2D_ts pos );
		void State(int pointerID, bool isDown, coord2D_ts pos);
		
		/**
		 * @brief Inform object that an other object is removed ...
		 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
		 * @note : Sub classes must call this class
		 * @return ---
		 */
		void OnObjectRemove(ewol::EObject * removeObject);
	}
}
#endif


