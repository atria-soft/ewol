/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_TOOL_H__
#define __ETK_TOOL_H__

#include <etk/Types.h>
#include <etk/UString.h>

namespace etk {
	namespace tool {
		float   frand(float a, float b);
		int32_t irand(int32_t a, int32_t b);
		void SortList(etk::Vector<etk::UString *> &m_listDirectory);
		bool strnCmpNoCase(const char * input1, const char * input2, int32_t maxLen);
		etk::UString SimplifyPath(etk::UString input);
	};
};

#endif
