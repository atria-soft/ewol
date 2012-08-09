/**
 *******************************************************************************
 * @file unicode.h
 * @brief Editeur De N'ours : Abstraction Charset layer (header)
 * @author Edouard DUPIN
 * @date 18/01/2012
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

#ifndef __UNICODE_H__
#define __UNICODE_H__

#include <etk/Types.h>
#include <etk/VectorType.h>
namespace unicode {
	typedef enum {
		EDN_CHARSET_UTF8,
		EDN_CHARSET_ISO_8859_1,
		EDN_CHARSET_ISO_8859_2,
		EDN_CHARSET_ISO_8859_3,
		EDN_CHARSET_ISO_8859_4,
		EDN_CHARSET_ISO_8859_5,
		EDN_CHARSET_ISO_8859_6,
		EDN_CHARSET_ISO_8859_7,
		EDN_CHARSET_ISO_8859_8,
		EDN_CHARSET_ISO_8859_9,
		EDN_CHARSET_ISO_8859_10,
		EDN_CHARSET_ISO_8859_11,
		EDN_CHARSET_ISO_8859_13,
		EDN_CHARSET_ISO_8859_14,
		EDN_CHARSET_ISO_8859_15,
	} charset_te;
	
	// transform ISO <==> Unicode
	void    convertIsoToUnicode(charset_te inputCharset, char                        input_ISO,     uniChar_t &                 output_Unicode);
	void    convertUnicodeToIso(charset_te inputCharset, uniChar_t                   input_Unicode, char &                      output_ISO);
	int32_t convertIsoToUnicode(charset_te inputCharset, etk::VectorType<char>&      input_ISO,     etk::VectorType<uniChar_t>& output_Unicode);
	int32_t convertIsoToUnicode(charset_te inputCharset, etk::VectorType<int8_t>&      input_ISO,     etk::VectorType<uniChar_t>& output_Unicode);
	int32_t convertUnicodeToIso(charset_te inputCharset, etk::VectorType<uniChar_t>& input_Unicode, etk::VectorType<char>&      output_ISO);
	int32_t convertUnicodeToIso(charset_te inputCharset, etk::VectorType<uniChar_t>& input_Unicode, etk::VectorType<int8_t>&      output_ISO);
	// Transform UTF-8 <==> Unicode
	void    convertUnicodeToUtf8(                        uniChar_t                   input_Unicode, char *                      output_UTF8);
	void    convertUtf8ToUnicode(                        char *                      input_UTF8,    uniChar_t&                  output_Unicode);
	int32_t convertUnicodeToUtf8(                        const etk::VectorType<uniChar_t>& input_Unicode, etk::VectorType<char>&      output_UTF8);
	int32_t convertUnicodeToUtf8(                        const etk::VectorType<uniChar_t>& input_Unicode, etk::VectorType<int8_t>&      output_UTF8);
	int32_t convertUtf8ToUnicode(                        etk::VectorType<char>&      input_UTF8,    etk::VectorType<uniChar_t>& output_Unicode);
	int32_t convertUtf8ToUnicode(                        etk::VectorType<int8_t>&    input_UTF8,    etk::VectorType<uniChar_t>& output_Unicode);
	int32_t convertUtf8ToUnicode(                        char *                      input_UTF8,    etk::VectorType<uniChar_t>& output_Unicode);
	// Transform ISO <==> UTF-8
	void    convertIsoToUtf8(  charset_te inputCharset,  char                      input_ISO,     char *                    output_UTF8);
	void    convertUtf8ToIso(  charset_te inputCharset,  char *                    input_UTF8,    char &                    output_ISO);
	int32_t convertIsoToUtf8(  charset_te inputCharset,  etk::VectorType<char>&    input_ISO,     etk::VectorType<char>&    output_UTF8);
	int32_t convertUtf8ToIso(  charset_te inputCharset,  etk::VectorType<char>&    input_UTF8,    etk::VectorType<char>&    output_ISO);
	
	void    Utf8_SizeElement(const char * data, int32_t lenMax , uint8_t &size, bool &baseValid);
	int32_t strUtf8Len(const char *input_UTF8);
}

#endif

