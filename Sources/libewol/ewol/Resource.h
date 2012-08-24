/**
 *******************************************************************************
 * @file ewol/Resources.h
 * @brief ewol resources template (header)
 * @author Edouard DUPIN
 * @date 24/08/2012
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

#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <etk/Types.h>
#include <ewol/Debug.h>

namespace ewol
{
	// class resources is pure virtual
	class Resource {
			etk::UString m_name;
			uint32_t     m_counter;
		public:
			Resource(etk::UString& filename) : m_name(filename), m_counter(1) { };
			virtual ~Resource(void) { };
			bool HasName(etk::UString& fileName) { return fileName==m_name; };
			etk::UString GetName(void) { return m_name; };
			void Increment(void) { m_counter++; };
			bool Decrement(void) { m_counter--; return (m_counter==0)?true:false; };
			virtual const char* GetType(void)=0;
	};
};



#endif

