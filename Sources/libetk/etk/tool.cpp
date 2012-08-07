/**
 *******************************************************************************
 * @file etk/tool.cpp
 * @brief Ewol Tool Kit : generique tools (Sources)
 * @author Edouard DUPIN
 * @date 26/06/2012
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

#include <etk/tool.h>
// for the rand ...
#include <time.h>
#include <math.h>

float etk::tool::frand(float a, float b)
{
	return ( rand()/(float)RAND_MAX ) * (b-a) + a;
}


int32_t etk::tool::irand(int32_t a, int32_t b)
{
	return (int32_t)(( rand()/(float)RAND_MAX ) * ((float)b-(float)a) + (float)a);
}


void etk::tool::SortList(std::vector<etk::UString *> &m_listDirectory)
{
	std::vector<etk::UString *> tmpList = m_listDirectory;
	m_listDirectory.clear();
	for(int32_t iii=0; iii<tmpList.size(); iii++) {
		
		int32_t findPos = 0;
		for(int32_t jjj=0; jjj<m_listDirectory.size(); jjj++) {
			//EWOL_DEBUG("compare : \""<<*tmpList[iii] << "\" and \"" << *m_listDirectory[jjj] << "\"");
			if (*tmpList[iii] > *m_listDirectory[jjj]) {
				findPos = jjj+1;
			}
		}
		//EWOL_DEBUG("position="<<findPos);
		m_listDirectory.insert(m_listDirectory.begin()+findPos, tmpList[iii]);
	}
}