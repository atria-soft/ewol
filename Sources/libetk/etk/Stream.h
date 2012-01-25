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

#define MAX_LOG_SIZE		(16000)
#define MAX_LOG_SIZE_TMP	(16000)

namespace etk{
	class CEndl{};
	class CHex{};
	class CCout{
		private:
			bool hex;
		public:
	private:
		char m_tmpChar[MAX_LOG_SIZE+1];
		char tmp[MAX_LOG_SIZE_TMP];
	public:
		CCout(){
			hex=false;
			memset(m_tmpChar, 0, (MAX_LOG_SIZE+1)*sizeof(char));
		};
		~CCout() { };
		CCout& operator << (int t) {
			snprintf(tmp, MAX_LOG_SIZE_TMP, "%d", t);
			strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
			return *this;
		}
		CCout& operator << (unsigned int t) {
			snprintf(tmp, MAX_LOG_SIZE_TMP, "%d", t);
			strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
			return *this;
		}
		CCout& operator << (long t) {
			snprintf(tmp, MAX_LOG_SIZE_TMP, "%ld", t);
			strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
			return *this;
		}
		CCout& operator << (double t) {
			snprintf(tmp, MAX_LOG_SIZE_TMP, "%f", t);
			strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
			return *this;
		}
		CCout& operator << (float t) {
			snprintf(tmp, MAX_LOG_SIZE_TMP, "%f", t);
			strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
			return *this;
		}
		CCout& operator << (char * t) {
			snprintf(tmp, MAX_LOG_SIZE_TMP, "%s", t);
			strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
			return *this;
		}
		CCout& operator << (const char * t) {
			snprintf(tmp, MAX_LOG_SIZE_TMP, "%s", t);
			strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
			return *this;
		}
		CCout& operator << (char t) {
			snprintf(tmp, MAX_LOG_SIZE_TMP, "%c", t);
			strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
			return *this;
		}
		CCout& operator << (etk::CEndl t) {
			strncat(m_tmpChar, "\n", MAX_LOG_SIZE);
			m_tmpChar[MAX_LOG_SIZE] = '\0';
#if defined(__PLATFORM__Android)
			LOGI("%s", m_tmpChar);
#else
			printf("%s", m_tmpChar);
#endif
			memset(m_tmpChar, 0, (MAX_LOG_SIZE+1)*sizeof(char));
			return *this;
		}
	};
	extern etk::CCout cout;
	extern etk::CEndl endl;
	extern etk::CHex hex;
}
#endif

