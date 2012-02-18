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
			virtual bool   CalculateMinSize(void);
			void SetSize(etkFloat_t size);
		private:
			etkFloat_t m_size;
	};
};

#endif
