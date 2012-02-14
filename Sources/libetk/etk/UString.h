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

#ifndef __ETK_USTRING_H__
#define __ETK_USTRING_H__

#include <etk/Stream.h>
#include <etk/VectorType.h>

namespace etk
{
	class UString
	{
		public:
			// Constructeurs
			UString(void);
			UString(const uniChar_t*  inputData, int32_t len = -1);
			UString(const char*       inputData, int32_t len = -1);
			void Set(const uniChar_t* inputData, int32_t len=-1);
			void Set(const char*      inputData, int32_t len=-1);
			// basic convertion integer en string
			UString(int inputData);
			UString(unsigned int inputData);
			UString(const etk::UString &etkS);
			// destructor : 
			~UString(void);
			
			/*****************************************************
			 *    = assigment
			 *****************************************************/
			const etk::UString& operator= (const etk::UString &etkS );
			const etk::UString& operator= (const char * inputData);
			const etk::UString& operator= (const uniChar_t * inputData);
			const etk::UString& operator= (etk::VectorType<char> inputData);
			const etk::UString& operator= (etk::VectorType<uniChar_t> inputData);
			/*****************************************************
			 *    == operator
			 *****************************************************/
			bool  operator== (const etk::UString& etkS) const;
			bool  operator== (const uniChar_t * inputData) const;
			bool  operator== (const char * inputData) const;
			/*****************************************************
			 *    != operator
			 *****************************************************/
			bool  operator!= (const etk::UString& etkS) const;
			bool  operator!= (const uniChar_t * inputData) const;
			bool  operator!= (const char * inputData) const;
			/*****************************************************
			 *    += operator
			 *****************************************************/
			const etk::UString& operator+= (const etk::UString &etkS);
			const etk::UString& operator+= (const char * inputData);
			const etk::UString& operator+= (const uniChar_t * inputData);
			/*****************************************************
			 *    + operator
			 *****************************************************/
			etk::UString operator+ (const etk::UString &etkS);
			etk::UString operator+ (const char * inputData);
			etk::UString operator+ (const uniChar_t * inputData);
			/*****************************************************
			 *    * operator
			 *****************************************************/
			friend etk::CCout& operator <<( etk::CCout &os,const etk::UString &obj);
	
			bool          IsEmpty(void) const;
			int32_t       Size(void) const;
	
			void          Add(int32_t currentID, const char* inputData);
			void          Add(int32_t currentID, const uniChar_t* inputData);
			void          Remove(int32_t currentID, int32_t len);
			void          Clear(void);
	
			etk::VectorType<uniChar_t> GetVector(void);
			uniChar_t *                pointer(void) { return &m_data[0]; };
	
			// Sting operation :
			int32_t       FindForward(const uniChar_t element, int32_t startPos=0);
			int32_t       FindBack(const uniChar_t element, int32_t startPos=0x7FFFFFFF);
			etk::UString  Extract(int32_t posStart=0, int32_t posEnd=0x7FFFFFFF);
	
		private :
			etk::VectorType<uniChar_t> m_data;
	};

	etk::CCout& operator <<(etk::CCout &os, const etk::UString &obj);

}

int32_t strlen(const uniChar_t * data);



#endif

