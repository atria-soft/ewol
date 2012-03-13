/**
 *******************************************************************************
 * @file ewol/widget/Spacer.h
 * @brief ewol Spacer widget system (header)
 * @author Edouard DUPIN
 * @date 29/12/2011
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

#ifndef __EWOL_SPACER_H__
#define __EWOL_SPACER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/Drawable.h>

namespace ewol {
	class Spacer :public ewol::Drawable
	{
		public:
			Spacer(void);
			virtual ~Spacer(void);
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
			void SetSize(etkFloat_t size);
		private:
			etkFloat_t m_size;
	};
	
	extern const char * const TYPE_EOBJECT_WIDGET_SPACER;
	
};

#define EWOL_CAST_WIDGET_SPACER(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_SPACER,ewol::Spacer,curentPointer)

#endif
