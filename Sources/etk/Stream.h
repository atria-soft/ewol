/**
 *******************************************************************************
 * @file etk/Stream.h
 * @brief Ewol Tool Kit : Basic etk::Stream for Log system ... (header)
 * @author Edouard DUPIN
 * @date 02/12/2011
 * @par Project
 * Ewol TK
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


#ifndef __ETK_STREAM_H__
#define __ETK_STREAM_H__

#include <cstdio>
#include <typeinfo>

namespace etk{
	class CEndl{};
	class CHex{};
	class CCout{
		private:
			bool hex;
		public:
		CCout(){
			hex=false;
		};
		
		~CCout() { };
		
		CCout& operator << (int t) {
			printf("%d", t);
			return *this;
		}
		CCout& operator << (unsigned int t) {
			printf("%d", t);
			return *this;
		}
		CCout& operator << (long t) {
			printf("%ld", t);
			return *this;
		}
		CCout& operator << (double t) {
			printf("%f", t);
			return *this;
		}
		CCout& operator << (float t) {
			printf("%f", t);
			return *this;
		}
		CCout& operator << (char * t) {
			printf("%s", t);
			return *this;
		}
		CCout& operator << (const char * t) {
			printf("%s", t);
			return *this;
		}
		CCout& operator << (char t) {
			printf("%c", t);
			return *this;
		}
		CCout& operator << (etk::CEndl t) {
			printf("\n");
			return *this;
		}
	};
	extern etk::CCout cout;
	extern etk::CEndl endl;
	extern etk::CHex hex;
}
#endif

