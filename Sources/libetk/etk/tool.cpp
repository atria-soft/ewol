/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/tool.h>
#include <etk/os/FSNode.h>
// for the rand ...
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

float etk::tool::frand(float a, float b)
{
	return ( rand()/(float)RAND_MAX ) * (b-a) + a;
}


int32_t etk::tool::irand(int32_t a, int32_t b)
{
	return (int32_t)(( rand()/(float)RAND_MAX ) * ((float)b-(float)a) + (float)a);
}


void etk::tool::SortList(etk::Vector<etk::UString *> &m_listDirectory)
{
	etk::Vector<etk::UString *> tmpList = m_listDirectory;
	m_listDirectory.Clear();
	for(int32_t iii=0; iii<tmpList.Size(); iii++) {
		
		int32_t findPos = 0;
		for(int32_t jjj=0; jjj<m_listDirectory.Size(); jjj++) {
			//EWOL_DEBUG("compare : \""<<*tmpList[iii] << "\" and \"" << *m_listDirectory[jjj] << "\"");
			if (*tmpList[iii] > *m_listDirectory[jjj]) {
				findPos = jjj+1;
			}
		}
		//EWOL_DEBUG("position="<<findPos);
		m_listDirectory.Insert(findPos, tmpList[iii]);
	}
}


bool etk::tool::strnCmpNoCase(const char * input1, const char * input2, int32_t maxLen)
{
	int32_t iii=0;
	while ('\0' != *input1 && '\0' != *input2 && iii < maxLen) {
		char in1 = *input1;
		char in2 = *input2;
		if (in1 != in2) {
			if (in1 <= 'Z' && in1 >= 'A') {
				in1 = in1 - 'A' + 'a';
			}
			if (in2 <= 'Z' && in2 >= 'A') {
				in2 = in2 - 'A' + 'a';
			}
			if (in1 != in2) {
				return false;
			}
		}
		iii++;
		input1++;
		input2++;
	}
	return true;
}


etk::UString etk::tool::SimplifyPath(etk::UString input)
{
	int32_t findStartPos = input.FindForward('/') + 1;
	int32_t findPos = input.FindForward('/', findStartPos);
	//TK_DEBUG("Siplify : \"" << input << "\"");
	int32_t preventBadCode = 0;
	while (findPos!=-1)
	{
		//TK_DEBUG("      string=\"" << input << "\"");
		//TK_DEBUG("      '/' @" << findPos);
		if (input.Size()<findPos+1) {
			// no more element ...
			break;
		}
		if(    input[findPos+1] == '/'
		    || (    input[findPos+1] == '.'
		         && input.Size()==findPos+2 )) {
			// cleane the element path
			input.Remove(findPos+1, 1);
			//TK_DEBUG("      Remove // string=\"" << input << "\"");
		} else {
			if (input.Size()<findPos+2) {
				// no more element ...
				break;
			}
			if(    input[findPos+1] == '.'
			    && input[findPos+2] == '.') {
				// cleane the element path
				input.Remove(findStartPos, findPos+3 - findStartPos );
				//TK_DEBUG("      Remove xxx/.. string=\"" << input << "\"");
			} else if(    input[findPos+1] == '.'
			           && input[findPos+2] == '/') {
				// cleane the element path
				input.Remove(findPos+1, 2);
				//TK_DEBUG("      Remove ./ string=\"" << input << "\"");
			} else {
				findStartPos = findPos+1;
			}
		}
		findPos = input.FindForward('/', findStartPos);
		preventBadCode++;
		if (preventBadCode>5000) {
			TK_CRITICAL("ERROR when getting the small path ... this is loop prevention...");
			break;
		}
	}
	#ifndef __TARGET_OS__Windows
		// for the target that supported the Realpath system :
		char buf[MAX_FILE_NAME];
		memset(buf, 0, MAX_FILE_NAME);
		char * ok = realpath(input.c_str(), buf);
		if (!ok) {
			TK_ERROR("Error to get the real path");
			input = "/";
		} else {
			input = buf;
		}
	#endif
	
	//TK_DEBUG("   ==> \"" << input << "\"");
	return input;
}

