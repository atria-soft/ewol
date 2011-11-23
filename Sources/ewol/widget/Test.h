/**
 *******************************************************************************
 * @file ewolTest.h
 * @brief ewol test widget system (header)
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

#ifndef __EWOL_TEST_H__
#define __EWOL_TEST_H__

#include <etkTypes.h>
#include <ewolDebug.h>
#include <ewolWidget.h>

namespace ewol {
	class Test :public ewol::Widget
	{
		public:
			Test(void);
			virtual ~Test(void);
			virtual bool   CalculateMinSize(void);
		public:
			virtual void   OnRegenerateDisplay(void);
		public:
			virtual bool OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y);
		private:
			int32_t m_elementID;
	};
};

#endif
