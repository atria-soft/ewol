/**
 *******************************************************************************
 * @file ewol/widget/SizerVert.h
 * @brief ewol vertical sizer widget system (header)
 * @author Edouard DUPIN
 * @date 07/11/2011
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

#ifndef __EWOL_SIZER_VERT_H__
#define __EWOL_SIZER_VERT_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/Widget.h>

namespace ewol {
	class SizerVert :public ewol::Widget
	{
		public:
			SizerVert(void);
			virtual ~SizerVert(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolSizerVert"; };
		public:
			virtual bool   CalculateSize(float availlableX, float availlableY); // this generate the current size ...
			virtual bool   CalculateMinSize(void); //update the min Size ... and the expend parameters for the sizer
			virtual void   SetMinSize(float x=-1, float y=-1);
			virtual void   SetExpendX(bool newExpend=false);
			virtual bool   CanExpentX(void);
			virtual void   SetExpendY(bool newExpend=false);
			virtual bool   CanExpentY(void);
			void           LockExpendContamination(bool lockExpend=false);
		private:
			bool                           m_lockExpendContamination;
			etk::VectorType<ewol::Widget*> m_subWidget[NB_BOUBLE_BUFFER];
		public:
			virtual void   SubWidgetRemoveAll(void);
			virtual void   SubWidgetAdd(ewol::Widget* newWidget);
			virtual void   SubWidgetRemove(ewol::Widget* newWidget);
			virtual void   SubWidgetUnLink(ewol::Widget* newWidget);
		protected:
			virtual void   OnDraw(DrawProperty& displayProp);
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @param[in] pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 */
			virtual ewol::Widget * GetWidgetAtPos(Vector2D<float>  pos);
			/**
			 * @brief Event generated to inform a flip-flop has occured on the current widget
			 * @param ---
			 * @return ---
			 */
			virtual void   OnFlipFlopEvent(void);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			virtual void OnObjectRemove(ewol::EObject * removeObject);
	};
	
};

#endif
