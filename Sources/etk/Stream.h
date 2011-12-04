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

#if defined(__PLATFORM__Android)
#	include <string.h>
#	include <android/log.h>
#	define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "====> EWOL", __VA_ARGS__))
#	define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "====> EWOL", __VA_ARGS__))
#	define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "====> EWOL", __VA_ARGS__))
#endif

namespace etk{
	class CEndl{};
	class CHex{};
	class CCout{
		private:
			bool hex;
		public:
#if defined(__PLATFORM__Android)
	private:
		char m_tmpChar[2048];
	public:
		CCout(){
			hex=false;
			memset(m_tmpChar, 0, 2048*sizeof(char));
		};
		~CCout() { };
		CCout& operator << (int t) {
			char tmp[256];
			sprintf(tmp,"%d", t);
			strcat(m_tmpChar, tmp);
			return *this;
		}
		CCout& operator << (unsigned int t) {
			char tmp[256];
			sprintf(tmp, "%d", t);
			strcat(m_tmpChar, tmp);
			return *this;
		}
		CCout& operator << (long t) {
			char tmp[256];
			sprintf(tmp, "%ld", t);
			strcat(m_tmpChar, tmp);
			return *this;
		}
		CCout& operator << (double t) {
			char tmp[256];
			sprintf(tmp, "%f", t);
			strcat(m_tmpChar, tmp);
			return *this;
		}
		CCout& operator << (float t) {
			char tmp[256];
			sprintf(tmp, "%f", t);
			strcat(m_tmpChar, tmp);
			return *this;
		}
		CCout& operator << (char * t) {
			char tmp[256];
			sprintf(tmp, "%s", t);
			strcat(m_tmpChar, tmp);
			return *this;
		}
		CCout& operator << (const char * t) {
			char tmp[256];
			sprintf(tmp, "%s", t);
			strcat(m_tmpChar, tmp);
			return *this;
		}
		CCout& operator << (char t) {
			char tmp[256];
			sprintf(tmp, "%c", t);
			strcat(m_tmpChar, tmp);
			return *this;
		}
		CCout& operator << (etk::CEndl t) {
			strcat(m_tmpChar, "\n");
			LOGI("%s", m_tmpChar);
			memset(m_tmpChar, 0, 2048*sizeof(char));
			return *this;
		}
#else
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
#endif
	};
	extern etk::CCout cout;
	extern etk::CEndl endl;
	extern etk::CHex hex;
}
#endif

