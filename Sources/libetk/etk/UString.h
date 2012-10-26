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

#include <etk/DebugInternal.h>
#include <etk/Stream.h>
#include <etk/Vector.h>

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
			UString(char inputData);
			UString(int inputData);
			UString(unsigned int inputData);
			UString(float inputData);
			UString(double inputData);
			UString(const etk::UString &etkS);
			//UString(const uniChar_t inputData);
			// destructor : 
			~UString(void);
			
			/*****************************************************
			 *    = assigment
			 *****************************************************/
			const etk::UString& operator= (const etk::UString &etkS );
			const etk::UString& operator= (etk::Vector<char> inputData);
			const etk::UString& operator= (etk::Vector<int8_t> inputData);
			const etk::UString& operator= (etk::Vector<uniChar_t> inputData);
			/*****************************************************
			 *    == operator
			 *****************************************************/
			bool  operator== (const etk::UString& etkS) const;
			/*****************************************************
			 *    != operator
			 *****************************************************/
			bool  operator!= (const etk::UString& etkS) const;
			/*****************************************************
			 *    > < >= <= operator
			 *****************************************************/
			bool  operator>  (const etk::UString& etkS) const;
			bool  operator>= (const etk::UString& etkS) const;
			bool  operator<  (const etk::UString& etkS) const;
			bool  operator<= (const etk::UString& etkS) const;
			/*****************************************************
			 *    += operator
			 *****************************************************/
			const etk::UString& operator+= (const etk::UString &etkS);
			/*****************************************************
			 *    + operator
			 *****************************************************/
			etk::UString operator+ (const etk::UString &etkS);
			/*****************************************************
			 *    << operator
			 *****************************************************/
			/*
			const etk::UString& operator <<= (const char input);
			const etk::UString& operator <<= (const int input);
			const etk::UString& operator <<= (const unsigned int input);
			*/
			/*****************************************************
			 *    >> operator
			 *****************************************************/
			
			/*****************************************************
			 *    Cout << operator
			 *****************************************************/
			friend etk::CCout& operator <<( etk::CCout &os,const etk::UString &obj);
			/*****************************************************
			 *    [] operator
			 *****************************************************/
			const uniChar_t& operator[] (int32_t pos) const {
				return m_data[pos];
			}
			uniChar_t& operator[] (int32_t pos) {
				return m_data[pos];
			}
			
			/*****************************************************
			 *    toolbox
			 *****************************************************/
			// Start With ...
			bool          StartWith(const etk::UString& data);
			// End With ...
			bool          EndWith(const etk::UString& data);
			// Find element
			int32_t       FindForward(const char      data, int32_t startPos=0);
			int32_t       FindForward(const uniChar_t data, int32_t startPos=0);
			int32_t       FindBack(const char         data, int32_t startPos=0x7FFFFFFF);
			int32_t       FindBack(const uniChar_t    data, int32_t startPos=0x7FFFFFFF);
			
			bool          IsEmpty(void) const;
			int32_t       Size(void) const;
			
			/*****************************************************
			 *    Generic modification function
			 *****************************************************/
			void          Add(int32_t currentID, const char* inputData);
			void          Add(int32_t currentID, const uniChar_t* inputData);
			void          Add(int32_t currentID, const uniChar_t  inputData);
			void          Remove(int32_t currentID, int32_t len);
			void          Clear(void);
			
			etk::Vector<uniChar_t> GetVector(void);
			uniChar_t *            pointer(void) { return &m_data[0]; };
			// generate temporary allocation (auto unallocated...)
			char *                 c_str(void);
			
			// Sting operation :
			etk::UString  Extract(int32_t posStart=0, int32_t posEnd=0x7FFFFFFF);
	
		private :
			etk::Vector<uniChar_t> m_data;     //!< internal data is stored in the Unicode properties ...
			etk::Vector<char>      m_dataUtf8; //!< Tmp data for the Utf8Data() function
	};

	etk::CCout& operator <<(etk::CCout &os, const etk::UString &obj);

}

int32_t strlen(const uniChar_t * data);



#endif

