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
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolProgressBar"; };
			virtual bool   CalculateMinSize(void);
			void           ValueSet(float val);
			float     ValueGet(void);
			void           SetColor(draw::Color newColor) { m_textColorFg = newColor; };
		private:
			float         m_value;           //!< % used
			draw::Color           m_textColorFg;     //!< forder bar color
			draw::Color           m_textColorBgOn;   //!< bar color enable
			draw::Color           m_textColorBgOff;  //!< bar color disable
		public:
			virtual void   OnRegenerateDisplay(void);
	};
	
};

#endif
