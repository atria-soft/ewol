/**
 *******************************************************************************
 * @file etk/String.h
 * @brief Ewol Tool Kit : normal sting management... (header)
 * @author Edouard DUPIN
 * @date 26/01/2011
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

#ifndef __ETK_STRING_H__
#define __ETK_STRING_H__

#include <etk/Stream.h>
#include <etk/VectorType.h>

namespace etk
{
	class String
	{
		public:
			// Constructeurs
			String(void);
			String(const char myInput);
			String(const char* inputData, int32_t len = -1);//, bool noAllocation=false);
			void Set(const char* inputData, int32_t len=-1);
			// basic convertion integer en string
			String(int inputData);
			String(unsigned int inputData);
			//String(const wchar_t *inputData);
			String(const etk::String &etkS);
			// destructor : 
			~String(void);
			
			const etk::String& operator= (const etk::String &etkS );	// assigment
			const etk::String& operator= (const char * inputData);
			const etk::String& operator= (etk::VectorType<int8_t> inputData);
			bool  operator== (const etk::String& etkS) const;			// == operator
			bool  operator== (const char * inputData) const;
			bool  operator!= (const etk::String& etkS) const;			// != operator
			bool  operator!= (const char * inputData) const;
			const etk::String& operator+= (const etk::String &etkS);	// += operator
			const etk::String& operator+= (const char * inputData);
			etk::String operator+ (const etk::String &etkS);			// + operator
			etk::String operator+ (const char * inputData);
			//operator const char *()
			friend etk::CCout& operator <<( etk::CCout &os,const etk::String &obj);
	
			bool          IsEmpty(void) const;
			int32_t       Size(void) const;
	
			void          Add(int32_t currentID, const char* inputData);
			void          Remove(int32_t currentID, int32_t len);
			void          Clear(void);
	
			etk::VectorType<int8_t> GetVector(void);
			char *                  c_str(void) { return (char*)&m_data[0]; };
	
			// Sting operation :
			int32_t       FindForward(const char element, int32_t startPos=0);
			int32_t       FindBack(const char element, int32_t startPos=0x7FFFFFFF);
			etk::String   Extract(int32_t posStart=0, int32_t posEnd=0x7FFFFFFF);
	
		private :
			etk::VectorType<int8_t> m_data;
	};

	void TestUntaire_String(void);
	
	etk::CCout& operator <<(etk::CCout &os, const etk::String &obj);
	
}





#endif

