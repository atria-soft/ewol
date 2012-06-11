/**
 *******************************************************************************
 * @file ewol/widget/ProgressBar.h
 * @brief ewol Progress bar widget system (header)
 * @author Edouard DUPIN
 * @date 14/04/2012
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

#ifndef __EWOL_PROGRESS_BAR_H__
#define __EWOL_PROGRESS_BAR_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>


namespace ewol {
	class ProgressBar :public ewol::Drawable
	{
		public:
			ProgressBar(void);
			virtual ~ProgressBar(void);
			/**
			 * @brief Check if the object has the specific type.
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type of the object we want to check
			 * @return true if the object is compatible, otherwise false
			 */
			virtual bool CheckObjectType(const char * const objectType);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void);
			virtual bool   CalculateMinSize(void);
			void           ValueSet(float val);
			float     ValueGet(void);
			void           SetColor(color_ts newColor) { m_textColorFg = newColor; };
		private:
			float         m_value;           //!< % used
			color_ts           m_textColorFg;     //!< forder bar color
			color_ts           m_textColorBgOn;   //!< bar color enable
			color_ts           m_textColorBgOff;  //!< bar color disable
		public:
			virtual void   OnRegenerateDisplay(void);
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_PROGRESS_BAR;
	
};

#define EWOL_CAST_WIDGET_PROGRESS_BAR(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_PROGRESS_BAR,ewol::ProgressBar,curentPointer)

#endif
