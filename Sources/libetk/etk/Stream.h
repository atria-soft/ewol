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
#include <string.h>
#include <pthread.h>
#include <etk/Types.h>

#if defined(__PLATFORM__Android)
#	include <android/log.h>
#	define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "EWOL", __VA_ARGS__))
#	define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "EWOL", __VA_ARGS__))
#	define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "EWOL", __VA_ARGS__))
#endif

#define MAX_LOG_SIZE		(16000)
#define MAX_LOG_SIZE_TMP	(512)



//regular colors
#define ETK_BASH_COLOR_BLACK			"\e[0;30m"
#define ETK_BASH_COLOR_RED				"\e[0;31m"
#define ETK_BASH_COLOR_GREEN			"\e[0;32m"
#define ETK_BASH_COLOR_YELLOW			"\e[0;33m"
#define ETK_BASH_COLOR_BLUE				"\e[0;34m"
#define ETK_BASH_COLOR_MAGENTA			"\e[0;35m"
#define ETK_BASH_COLOR_CYAN				"\e[0;36m"
#define ETK_BASH_COLOR_WHITE			"\e[0;37m"
//emphasized (bolded) colors
#define ETK_BASH_COLOR_BOLD_BLACK		"\e[1;30m"
#define ETK_BASH_COLOR_BOLD_RED			"\e[1;31m"
#define ETK_BASH_COLOR_BOLD_GREEN		"\e[1;32m"
#define ETK_BASH_COLOR_BOLD_YELLOW		"\e[1;33m"
#define ETK_BASH_COLOR_BOLD_BLUE		"\e[1;34m"
#define ETK_BASH_COLOR_BOLD_MAGENTA		"\e[1;35m"
#define ETK_BASH_COLOR_BOLD_CYAN		"\e[1;36m"
#define ETK_BASH_COLOR_BOLD_WHITE		"\e[1;37m"
//background colors
#define ETK_BASH_COLOR_BG_BLACK			"\e[40m"
#define ETK_BASH_COLOR_BG_RED			"\e[41m"
#define ETK_BASH_COLOR_BG_GREEN			"\e[42m"
#define ETK_BASH_COLOR_BG_YELLOW		"\e[43m"
#define ETK_BASH_COLOR_BG_BLUE			"\e[44m"
#define ETK_BASH_COLOR_BG_MAGENTA		"\e[45m"
#define ETK_BASH_COLOR_BG_CYAN			"\e[46m"
#define ETK_BASH_COLOR_BG_WHITE			"\e[47m"
// Return to the normal color setings
#define ETK_BASH_COLOR_NORMAL			"\e[0m"
//go to the Top of bash
#define ETK_BASH_GO_TOP					"\e[0;0f"


namespace etk{
	class CEndl{};
	class CHex{};
	class CStart{};
	typedef enum {
		LOG_LEVEL_NONE,
		LOG_LEVEL_CRITICAL,
		LOG_LEVEL_ERROR,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_INFO,
		LOG_LEVEL_DEBUG,
		LOG_LEVEL_VERBOSE
	} logLevel_te;
	class CCout{
		private:
			bool             hex;
			logLevel_te      m_requestedLevel;
			bool             m_writing;
			char             m_tmpChar[MAX_LOG_SIZE+1];
			char             tmp[MAX_LOG_SIZE_TMP];
			pthread_mutex_t  m_mutex;
		public:
			CCout(){
				hex=false;
				m_writing = true;
				m_requestedLevel = LOG_LEVEL_ERROR;
				memset(m_tmpChar, 0, (MAX_LOG_SIZE+1)*sizeof(char));
				pthread_mutex_init(&m_mutex, NULL);
			};
			~CCout() {
				pthread_mutex_destroy(&m_mutex);
			};
			
			CCout& operator << (int t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%d", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (unsigned int t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%u", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			/*
			CCout& operator << (uniChar_t t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%c", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			*/
			CCout& operator << (long t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%ld", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (long long t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%lld", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (double t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%f", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (float t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%f", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (char * t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%s", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (const char * t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%s", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (char t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "%c", t);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (bool t) {
				if (true == m_writing) {
					if (t) {
						strncat(m_tmpChar, "true", MAX_LOG_SIZE);
					} else {
						strncat(m_tmpChar, "false", MAX_LOG_SIZE);
					}
				}
				return *this;
			}
			CCout& operator << (coord2D_ts t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "(%f,%f)", t.x, t.y);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (coord3D_ts t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "(%f,%f,%f)", t.x, t.y, t.z);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (texCoord_ts t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "{%f,%f}", t.u, t.v);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (color_ts t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "#%02X%02X%02X%02X", t.red, t.green, t.blue, t.alpha);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (clipping_ts t) {
				if (true == m_writing) {
					snprintf(tmp, MAX_LOG_SIZE_TMP, "origin=(%f,%f) size=(%f,%f)", t.x, t.y, t.w, t.h);
					strncat(m_tmpChar, tmp, MAX_LOG_SIZE);
				}
				return *this;
			}
			CCout& operator << (CStart ccc) {
				pthread_mutex_lock(&m_mutex);
				return *this;
			}
			void SetLevel(logLevel_te ccc) {
				m_requestedLevel = ccc;
			}
			CCout& operator << (logLevel_te ccc) {
				if (ccc <= m_requestedLevel) {
					m_writing = true;
				} else {
					m_writing = false;
				}
				if (true == m_writing) {
					switch (ccc)
					{
						case LOG_LEVEL_CRITICAL:
							strncat(m_tmpChar, ETK_BASH_COLOR_BOLD_RED, MAX_LOG_SIZE);
							strncat(m_tmpChar, "[C]", MAX_LOG_SIZE);
							break;
						case LOG_LEVEL_ERROR:
							strncat(m_tmpChar, ETK_BASH_COLOR_MAGENTA, MAX_LOG_SIZE);
							strncat(m_tmpChar, "[E]", MAX_LOG_SIZE);
							break;
						case LOG_LEVEL_WARNING:
							strncat(m_tmpChar, ETK_BASH_COLOR_BOLD_RED, MAX_LOG_SIZE);
							strncat(m_tmpChar, "[W]", MAX_LOG_SIZE);
							break;
						case LOG_LEVEL_INFO:
							strncat(m_tmpChar, ETK_BASH_COLOR_CYAN, MAX_LOG_SIZE);
							strncat(m_tmpChar, "[I]", MAX_LOG_SIZE);
							break;
						case LOG_LEVEL_DEBUG:
							strncat(m_tmpChar, ETK_BASH_COLOR_YELLOW, MAX_LOG_SIZE);
							strncat(m_tmpChar, "[D]", MAX_LOG_SIZE);
							break;
						case LOG_LEVEL_VERBOSE:
							strncat(m_tmpChar, ETK_BASH_COLOR_WHITE, MAX_LOG_SIZE);
							strncat(m_tmpChar, "[V]", MAX_LOG_SIZE);
							break;
						default:
							strncat(m_tmpChar, "[?]", MAX_LOG_SIZE);
							break;
					}
				}
				return *this;
			}
			CCout& operator << (etk::CEndl t) {
				if (true == m_writing) {
					strncat(m_tmpChar, ETK_BASH_COLOR_NORMAL, MAX_LOG_SIZE);
					strncat(m_tmpChar, "\n", MAX_LOG_SIZE);
					m_tmpChar[MAX_LOG_SIZE] = '\0';
#if defined(__PLATFORM__Android)
					LOGI("%s", m_tmpChar);
#else
					printf("%s", m_tmpChar);
#endif
					memset(m_tmpChar, 0, (MAX_LOG_SIZE+1)*sizeof(char));
				}
				pthread_mutex_unlock(&m_mutex);
				return *this;
			}
	};
	extern etk::CCout cout;
	extern etk::CEndl endl;
	extern etk::CHex hex;
	extern etk::CStart cstart;
}
#endif

