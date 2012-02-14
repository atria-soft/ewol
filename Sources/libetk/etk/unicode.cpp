/**
 *******************************************************************************
 * @file unicode.cpp
 * @brief Editeur De N'ours : Abstraction Charset layer (Sources)
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

// cf : http://unicode.org/fr/charts/symbols.html#CombiningDiacriticalMarks

#include <etk/Types.h>
#include <etk/Debug.h>
#include <etk/unicodeTable.h>
#include <etk/unicode.h>



// transform ISO <==> Unicode
void unicode::convertIsoToUnicode(charset_te inputCharset, char input_ISO, uniChar_t & output_Unicode)
{
	switch(inputCharset)
	{
		case EDN_CHARSET_ISO_8859_1:		output_Unicode = TableIso8859_1[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_2:		output_Unicode = TableIso8859_2[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_3:		output_Unicode = TableIso8859_3[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_4:		output_Unicode = TableIso8859_4[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_5:		output_Unicode = TableIso8859_5[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_6:		output_Unicode = TableIso8859_6[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_7:		output_Unicode = TableIso8859_7[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_8:		output_Unicode = TableIso8859_8[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_9:		output_Unicode = TableIso8859_9[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_10:		output_Unicode = TableIso8859_10[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_11:		output_Unicode = TableIso8859_11[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_13:		output_Unicode = TableIso8859_13[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_14:		output_Unicode = TableIso8859_14[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_15:		
			output_Unicode = TableIso8859_15[(uint32_t)input_ISO&0xFF];
			#if 0
			if ((uint32_t)input_ISO&0xFF == 0xb0) {
				printf("Change Element : 0x%02x ==> 0x%08x \n", (uint32_t)input_ISO&0xFF, output_Unicode);
			}
			#endif
			break;
		default :
			TK_WARNING("Unknow charset ... " << inputCharset);
			output_Unicode = '?';
			break;
	}
}


void unicode::convertUnicodeToIso(charset_te inputCharset, uniChar_t input_Unicode, char & output_ISO)
{
	const uniChar_t *tmpTable = NULL;
	switch(inputCharset)
	{
		case EDN_CHARSET_ISO_8859_1:		tmpTable = TableIso8859_1;		break;
		case EDN_CHARSET_ISO_8859_2:		tmpTable = TableIso8859_2;		break;
		case EDN_CHARSET_ISO_8859_3:		tmpTable = TableIso8859_3;		break;
		case EDN_CHARSET_ISO_8859_4:		tmpTable = TableIso8859_4;		break;
		case EDN_CHARSET_ISO_8859_5:		tmpTable = TableIso8859_5;		break;
		case EDN_CHARSET_ISO_8859_6:		tmpTable = TableIso8859_6;		break;
		case EDN_CHARSET_ISO_8859_7:		tmpTable = TableIso8859_7;		break;
		case EDN_CHARSET_ISO_8859_8:		tmpTable = TableIso8859_8;		break;
		case EDN_CHARSET_ISO_8859_9:		tmpTable = TableIso8859_9;		break;
		case EDN_CHARSET_ISO_8859_10:		tmpTable = TableIso8859_10;		break;
		case EDN_CHARSET_ISO_8859_11:		tmpTable = TableIso8859_11;		break;
		case EDN_CHARSET_ISO_8859_13:		tmpTable = TableIso8859_13;		break;
		case EDN_CHARSET_ISO_8859_14:		tmpTable = TableIso8859_14;		break;
		case EDN_CHARSET_ISO_8859_15:		tmpTable = TableIso8859_15;		break;
		default :
			TK_WARNING("Unknow charset ... " <<  inputCharset);
			output_ISO = '?';
			return;
	}
	int32_t i;
	for (i=0; i<256; i++) {
		if (tmpTable[i] == input_Unicode) {
			output_ISO = (char)i;
			return;
		}
	}
}


int32_t unicode::convertIsoToUnicode(charset_te inputCharset, etk::VectorType<char>& input_ISO, etk::VectorType<uniChar_t>& output_Unicode)
{
	TK_WARNING("TODO : not coded...");
	return 0;
}


int32_t unicode::convertUnicodeToIso(charset_te inputCharset, etk::VectorType<uniChar_t>& input_Unicode, etk::VectorType<char>&    output_ISO)
{
	TK_WARNING("TODO : not coded...");
	return 0;
}


static uint32_t unicodeToUtf8(uniChar_t value)
{
	uint32_t output = 0;
	if (127 >= value) {
		output = value;
	} else if (2047 >= value) {
		// output ==> 00000000 00000000 110xxxxx 10xxxxxx
		// input ==>  -------- -------- -----222 22111111
		output = 0x0000C080;
		output+= (value & 0x000007C0)<<2;
		output+=  value & 0x0000003F;
	} else if (65535 >= value) {
		// output ==> 00000000 1110xxxx 10xxxxxx 10xxxxxx
		// input ==>  -------- -------- 33332222 22111111
		output = 0x00E08080;
		output+= (value & 0x0000F000)<<4;
		output+= (value & 0x00000FC0)<<2;
		output+=  value & 0x0000003F;
	} else if (1114111 >= value) {
		// output ==> 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		// input ==>  -------- ---44433 33332222 22111111
		output = 0xF0808080;
		output+= (value & 0x001C0000)<<6;
		output+= (value & 0x0003F000)<<4;
		output+= (value & 0x00000FC0)<<2;
		output+=  value & 0x0000003F;
	} else {
		TK_ERROR("NON UTF8 caracter input...");
		return 0;
	}
	//printf("utf8convertion : %d=%08x ==> %08x\n",value, value, output);
	return output;
}


// Transform UTF-8 <==> Unicode
void unicode::convertUnicodeToUtf8(uniChar_t input_Unicode, char * output_UTF8)
{
	uint32_t value = unicodeToUtf8(input_Unicode);
	if (0xFF >= value) {
		output_UTF8[0] = (char)value;
		output_UTF8[1] = '\0';
	} else if (0xFFFF >= value) {
		output_UTF8[0] = (char)((value>>8)  & 0x000000FF);
		output_UTF8[1] = (char)value;
		output_UTF8[2] = '\0';
	} else if (0xFFFFFF >= value) {
		output_UTF8[0] = (char)((value>>16) & 0x000000FF);
		output_UTF8[1] = (char)((value>>8)  & 0x000000FF);
		output_UTF8[2] = (char)value;
		output_UTF8[3] = '\0';
	} else {
		output_UTF8[0] = (char)((value>>24) & 0x000000FF);
		output_UTF8[1] = (char)((value>>16) & 0x000000FF);
		output_UTF8[2] = (char)((value>>8)  & 0x000000FF);
		output_UTF8[3] = (char)value;
		output_UTF8[4] = '\0';
	}
}



void unicode::convertUtf8ToUnicode(char * input_UTF8, uniChar_t &output_Unicode)
{
	int32_t len = strlen(input_UTF8);
	output_Unicode = 0;
	switch (len) {
		case 1:
			output_Unicode = (uint8_t)(input_UTF8[0]) & 0x7F;
			break;
		case 2:
			output_Unicode  = (((uint8_t)input_UTF8[0]) & 0x1F)<< 6;
			output_Unicode +=  ((uint8_t)input_UTF8[1]) & 0x3F;
			break;
		case 3:
			output_Unicode  = (((uint8_t)input_UTF8[0]) & 0x0F)<< 12;
			output_Unicode += (((uint8_t)input_UTF8[1]) & 0x3F)<< 6;
			output_Unicode +=  ((uint8_t)input_UTF8[2]) & 0x3F;
			break;
		default:
			output_Unicode  = (((uint8_t)input_UTF8[0]) & 0x07)<< 18;
			output_Unicode += (((uint8_t)input_UTF8[1]) & 0x3F)<< 12;
			output_Unicode += (((uint8_t)input_UTF8[2]) & 0x3F)<< 6;
			output_Unicode +=  ((uint8_t)input_UTF8[3]) & 0x3F;
			break;
	}
}


int32_t unicode::convertUnicodeToUtf8(const etk::VectorType<uniChar_t>& input_Unicode, etk::VectorType<char>& output_UTF8)
{
	char output[10];
	
	for (int32_t iii=0; iii<input_Unicode.Size(); iii++) {
		unicode::convertUnicodeToUtf8(input_Unicode[iii], output);
		char * tmp = output ;
		while (*tmp != '\0') {
			output_UTF8.PushBack(*tmp);
			tmp++;
		}
	}
	output_UTF8.PushBack('\0');
	return output_UTF8.Size()-1;
}


int32_t unicode::convertUtf8ToUnicode(etk::VectorType<char>& input_UTF8, etk::VectorType<uniChar_t>& output_Unicode)
{
	char tmpData[20];
	int32_t pos = 0;
	while (pos < input_UTF8.Size()) {
		int32_t lenMax = input_UTF8.Size() - pos;
		//4 case
		if(    1<=lenMax
		    && 0x00 == (input_UTF8[pos+0] & 0x80) )
		{
			tmpData[0] = input_UTF8[pos+0];
			tmpData[1] = '\0';
			pos += 1;
		} else if(		2<=lenMax
					&&	0xC0 == (input_UTF8[pos+0] & 0xE0)
					&&	0x80 == (input_UTF8[pos+1] & 0xC0) ) {
			tmpData[0] = input_UTF8[pos+0];
			tmpData[1] = input_UTF8[pos+1];
			tmpData[2] = '\0';
			pos += 1;
		} else if(		3<=lenMax
					&&	0xE0 == (input_UTF8[pos+0] & 0xF0)
					&&	0x80 == (input_UTF8[pos+1] & 0xC0)
					&&	0x80 == (input_UTF8[pos+2] & 0xC0)) {
			tmpData[0] = input_UTF8[pos+0];
			tmpData[1] = input_UTF8[pos+1];
			tmpData[2] = input_UTF8[pos+2];
			tmpData[3] = '\0';
			pos += 1;
		} else if(		4<=lenMax
					&&	0xF0 == (input_UTF8[pos+0] & 0xF8)
					&&	0x80 == (input_UTF8[pos+1] & 0xC0)
					&&	0x80 == (input_UTF8[pos+2] & 0xC0)
					&&	0x80 == (input_UTF8[pos+3] & 0xC0)) {
			tmpData[0] = input_UTF8[pos+0];
			tmpData[1] = input_UTF8[pos+1];
			tmpData[2] = input_UTF8[pos+2];
			tmpData[3] = input_UTF8[pos+3];
			tmpData[4] = '\0';
			pos += 1;
		} else {
			tmpData[0] = '\0';
			pos += 1;
		}
		uniChar_t tmpUnicode;
		convertUtf8ToUnicode(tmpData, tmpUnicode);
		output_Unicode.PushBack(tmpUnicode);
	}
	return 0;
}

int32_t unicode::convertUtf8ToUnicode(char * input_UTF8, etk::VectorType<uniChar_t>& output_Unicode)
{
	char tmpData[20];
	int32_t pos = 0;
	if (NULL == input_UTF8) {
		return 0;
	}
	int32_t len = strlen(input_UTF8);
	while (pos < len) {
		int32_t lenMax = len - pos;
		//4 case
		if(    1<=lenMax
		    && 0x00 == (input_UTF8[pos+0] & 0x80) )
		{
			tmpData[0] = input_UTF8[pos+0];
			tmpData[1] = '\0';
			pos += 1;
		} else if(		2<=lenMax
					&&	0xC0 == (input_UTF8[pos+0] & 0xE0)
					&&	0x80 == (input_UTF8[pos+1] & 0xC0) ) {
			tmpData[0] = input_UTF8[pos+0];
			tmpData[1] = input_UTF8[pos+1];
			tmpData[2] = '\0';
			pos += 1;
		} else if(		3<=lenMax
					&&	0xE0 == (input_UTF8[pos+0] & 0xF0)
					&&	0x80 == (input_UTF8[pos+1] & 0xC0)
					&&	0x80 == (input_UTF8[pos+2] & 0xC0)) {
			tmpData[0] = input_UTF8[pos+0];
			tmpData[1] = input_UTF8[pos+1];
			tmpData[2] = input_UTF8[pos+2];
			tmpData[3] = '\0';
			pos += 1;
		} else if(		4<=lenMax
					&&	0xF0 == (input_UTF8[pos+0] & 0xF8)
					&&	0x80 == (input_UTF8[pos+1] & 0xC0)
					&&	0x80 == (input_UTF8[pos+2] & 0xC0)
					&&	0x80 == (input_UTF8[pos+3] & 0xC0)) {
			tmpData[0] = input_UTF8[pos+0];
			tmpData[1] = input_UTF8[pos+1];
			tmpData[2] = input_UTF8[pos+2];
			tmpData[3] = input_UTF8[pos+3];
			tmpData[4] = '\0';
			pos += 1;
		} else {
			tmpData[0] = '\0';
			pos += 1;
		}
		uniChar_t tmpUnicode;
		convertUtf8ToUnicode(tmpData, tmpUnicode);
		output_Unicode.PushBack(tmpUnicode);
	}
	return 0;
}


// Transform ISO <==> UTF-8
void unicode::convertIsoToUtf8(charset_te inputCharset,  char input_ISO, char * output_UTF8)
{
	uniChar_t tmpUnicode;
	// concert Iso in UniCode
	convertIsoToUnicode(inputCharset, input_ISO, tmpUnicode );
	// convert UniCode in Utf-8
	convertUnicodeToUtf8(tmpUnicode, output_UTF8);
}


void unicode::convertUtf8ToIso(charset_te inputCharset,  char * input_UTF8, char & output_ISO)
{
	uniChar_t tmpUnicode;
	// convert Utf-8 in UniCode
	convertUtf8ToUnicode(input_UTF8, tmpUnicode);
	// concert UniCode in Iso
	convertUnicodeToIso(inputCharset, tmpUnicode, output_ISO);
}


int32_t unicode::convertIsoToUtf8(charset_te inputCharset, etk::VectorType<char>& input_ISO, etk::VectorType<char>& output_UTF8)
{
	TK_WARNING("TODO : not coded...");
	return 0;
}


int32_t unicode::convertUtf8ToIso(charset_te inputCharset, etk::VectorType<char>& input_UTF8, etk::VectorType<char>& output_ISO)
{
	TK_WARNING("TODO : not coded...");
	return 0;
}

/**
 * @brief Get the number of element of the curent UTF8 char (in the curent Buffer)
 *
 * @param[in]			data			pointer on the curent CHAR string (pointer on the allocated buffer)
 * @param[out]			size			Nb of char use in this UTF8 [0..4]
 * @param[out]			baseValid		true : the ase format of the UTF8 is CORRECT
 *
 * @return ---
 *
 */
void unicode::Utf8_SizeElement(const char * data, int32_t lenMax , uint8_t &size, bool &baseValid)
{
	TK_ASSERT(0 <= lenMax, "size can not be < 0 ...");
	if (0 > lenMax) {
		size = 0;
		baseValid = false;
		return;
	}
	//4 case
	if (	1<=lenMax
		&&	0x00 == (data[0] & 0x80) ) {
		// One Char Element
		size = 1;
		baseValid = true;
	} else if(		2<=lenMax
				&&	0xC0 == (data[0] & 0xE0)
				&&	0x80 == (data[1] & 0xC0) ) {
		size = 2;
		baseValid = true;
	} else if(		3<=lenMax
				&&	0xE0 == (data[0] & 0xF0)
				&&	0x80 == (data[1] & 0xC0)
				&&	0x80 == (data[2] & 0xC0)) {
		size = 3;
		baseValid = true;
	} else if(		4<=lenMax
				&&	0xF0 == (data[0] & 0xF8)
				&&	0x80 == (data[1] & 0xC0)
				&&	0x80 == (data[2] & 0xC0)
				&&	0x80 == (data[3] & 0xC0)) {
		size = 4;
		baseValid = true;
	} else {
		// return only one error Caracter ...
		baseValid = false;
		size = 1;
	}
}

#if 0 // Remove for the moment ...
/**
 * @brief Get the number of element of the previous UTF8 char (in the curent Buffer) 
 *
 * @param[in]			data			pointer on the curent CHAR string (pointer on the allocated buffer) (the curent char is not check)
 * @param[out]			size			Nb of char use in this UTF8 [0..4]
 * @param[out]			baseValid		true : the ase format of the UTF8 is CORRECT
 *
 * @return ---
 *
 */
static void Utf8_SizePreviousElement(const char * data, int32_t lenMax, uint8_t &size, bool &baseValid)
{
	EDN_ASSERT(0 <= lenMax, "size can not be < 0 ...");
	if (0 > lenMax) {
		size = 0;
		baseValid = false;
		return;
	}
	//4 case
	if (	1<=lenMax
		&&	0x00 == (data[-1] & 0x80) ) {
		// One Char Element
		size = 1;
		baseValid = true;
	} else if(		2<=lenMax
				&&	0xC0 == (data[-2] & 0xE0)
				&&	0x80 == (data[-1] & 0xC0) ) {
		size = 2;
		baseValid = true;
	} else if(		3<=lenMax
				&&	0xE0 == (data[-3] & 0xF0)
				&&	0x80 == (data[-2] & 0xC0)
				&&	0x80 == (data[-1] & 0xC0)) {
		size = 3;
		baseValid = true;
	} else if(		4<=lenMax
				&&	0xF0 == (data[-4] & 0xF8)
				&&	0x80 == (data[-3] & 0xC0)
				&&	0x80 == (data[-2] & 0xC0)
				&&	0x80 == (data[-1] & 0xC0)) {
		size = 4;
		baseValid = true;
	} else {
		// return only one error Caracter ...
		baseValid = false;
		size = 1;
	}
}
#endif

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
 /*
static uint32_t Utf8_GetValue(UTF8Element_ts &Element)
{
	uint32_t value = 0;
	const char * data = m_data + Element.CharPosition;
	//4 case
	switch(Element.CharSize)
	{
		case 1:
			value = data[0] & 0x7F;
			break;
		case 2:
			value  = (data[0] & 0x1F)<< 6;
			value +=  data[1] & 0x3F;
			break;
		case 3:
			value  = (data[0] & 0x0F)<< 12;
			value += (data[1] & 0x3F)<< 6;
			value +=  data[2] & 0x3F;
			break;
		case 4:
			value  = (data[0] & 0x07)<< 18;
			value += (data[1] & 0x3F)<< 12;
			value += (data[2] & 0x3F)<< 6;
			value +=  data[3] & 0x3F;
			break;
		default:
			// return only one error Caracter ...
			EDN_ASSERT(false, "impossible case....");
			break;
	}
	// check the validity of the UTF8 ...
	if(		(		0xD800 <= value 
				&&	0xDFFF >= value )
		||	(		0xFDD0 <= value 
				&&	0xFDEF >= value )
		||	(		0xFFFE <= value 
				&&	0xFFFF >= value )
		||	(		0x1FFFE <= value 
				&&	0x1FFFF >= value )
		||	(		0x2FFFE <= value 
				&&	0xDFFFF >= value )
		||	(		0xEFFFE <= value 
				&&	0xEFFFF >= value )
		||	(		0xFFFFE <= value 
				&&	0xFFFFF >= value )
		||	(		0x10FFFE <= value 
				&&	0x10FFFF >= value ) )
	{
		// overwrite the UTF8 validity ==> this is not a diaplayable element
		Element.ValidUTF8 = false;
		return value;
	}
	
	return value;
}
*/


int32_t unicode::strUtf8Len(const char *input_UTF8)
{
	int32_t count = 0;
	int32_t size = strlen(input_UTF8);
	uint8_t tmpSize;
	bool baseValid;
	while (size > 0) {
		Utf8_SizeElement(input_UTF8, size , tmpSize, baseValid);
		input_UTF8 += tmpSize;
		size -= tmpSize;
		count++;
	}
	return count;
}



// **************************************************************************************************************
//       simple convertion optention
// **************************************************************************************************************

#if 0

Procedure de recuperation des charset sans ce casser les ...

// generate the basic file
	FILE * mfile = NULL;
	mfile = fopen("fichierIsoBase", "wb");
	if (NULL == mfile) {
		EDN_ERROR("Error to create file");
		return false;
	}
	char newline = '\n';
	for(int32_t i=0x20; i<0x100; i++) {
		char plop = i;
		fwrite(&plop, sizeof(char), 1, mfile);
		fwrite(&newline, sizeof(char), 1, mfile);
	}
	fclose(mfile);
// console script to convert files :
iconv -c --from-code=ISO-8859-1 --to-code=UTF-8 -o fichierUTF8_iso-1 fichierIsoBase
iconv -c --from-code=ISO-8859-2 --to-code=UTF-8 -o fichierUTF8_iso-2 fichierIsoBase
iconv -c --from-code=ISO-8859-3 --to-code=UTF-8 -o fichierUTF8_iso-3 fichierIsoBase
iconv -c --from-code=ISO-8859-4 --to-code=UTF-8 -o fichierUTF8_iso-4 fichierIsoBase
iconv -c --from-code=ISO-8859-5 --to-code=UTF-8 -o fichierUTF8_iso-5 fichierIsoBase
iconv -c --from-code=ISO-8859-6 --to-code=UTF-8 -o fichierUTF8_iso-6 fichierIsoBase
iconv -c --from-code=ISO-8859-7 --to-code=UTF-8 -o fichierUTF8_iso-7 fichierIsoBase
iconv -c --from-code=ISO-8859-8 --to-code=UTF-8 -o fichierUTF8_iso-8 fichierIsoBase
iconv -c --from-code=ISO-8859-9 --to-code=UTF-8 -o fichierUTF8_iso-9 fichierIsoBase
iconv -c --from-code=ISO-8859-10 --to-code=UTF-8 -o fichierUTF8_iso-10 fichierIsoBase
iconv -c --from-code=ISO-8859-11 --to-code=UTF-8 -o fichierUTF8_iso-11 fichierIsoBase
iconv -c --from-code=ISO-8859-12 --to-code=UTF-8 -o fichierUTF8_iso-12 fichierIsoBase
iconv -c --from-code=ISO-8859-13 --to-code=UTF-8 -o fichierUTF8_iso-13 fichierIsoBase
iconv -c --from-code=ISO-8859-14 --to-code=UTF-8 -o fichierUTF8_iso-14 fichierIsoBase
iconv -c --from-code=ISO-8859-15 --to-code=UTF-8 -o fichierUTF8_iso-15 fichierIsoBase

// NOTE : Le format 12 n'existe pas ...
	FILE * mfileout = NULL;
	mfileout = fopen("outputGeneration.c", "wb");
	if (NULL == mfileout) {
		EDN_ERROR("Error to create file");
		return false;
	}
	
	char * inputFileData[] = {
		"fichierUTF8_iso-1",
		"fichierUTF8_iso-2",
//		"fichierUTF8_iso-3",
		"fichierUTF8_iso-4",
		"fichierUTF8_iso-5",
/*		"fichierUTF8_iso-6",
		"fichierUTF8_iso-7",
		"fichierUTF8_iso-8",
		"fichierUTF8_iso-9",
		"fichierUTF8_iso-10",
		"fichierUTF8_iso-11",
		"fichierUTF8_iso-13",
		"fichierUTF8_iso-14",
*/
		"fichierUTF8_iso-15"
	};
	
	for (int32_t k=0; k<5; k++) {
		FILE * mfile = NULL;
		mfile = fopen(inputFileData[k], "rb");
		if (NULL == mfile) {
			EDN_ERROR("Error to open file");
			return false;
		}
		char data[255] ;
		fprintf(mfileout, "\tTYPESTRUCT_TS  %s[] = {\n\t\t", inputFileData[k]);
		for(int32_t i=0x0; i<0x10; i++) {
			fprintf(mfileout, "0x%08X, ", i);
		}
		fprintf(mfileout, "\n\t\t");
		for(int32_t i=0x10; i<0x20; i++) {
			fprintf(mfileout, "0x%08X, ", i);
		}
		for(int32_t i=0x20; i<0x100; i++) {
			if (0==i%16) {
				fprintf(mfileout, "\n\t\t");
			}
			fgets(data, 25, mfile );
			data[strlen(data)-1] = '\0';
			EDN_INFO("sizeofLine=" << strlen(data) << " data=\"" << data << "\"");
			// convert in int : 
			int32_t valUTF8 = 0;
			int32_t valUnicode = 0;
			switch (strlen(data)) {
				case 1:
					valUTF8 = (uint8_t) (data[0]);
					valUnicode = (uint8_t)(data[0]) & 0x7F;
					break;
				case 2:
					valUTF8  = (uint8_t) (data[0]) << 8;
					valUTF8 += (uint8_t) (data[1]);
					valUnicode  = (((uint8_t)data[0]) & 0x1F)<< 6;
					valUnicode +=  ((uint8_t)data[1]) & 0x3F;
					break;
				case 3:
					valUTF8  = (uint8_t) (data[0]) << 16;
					valUTF8 += (uint8_t) (data[1]) << 8;
					valUTF8 += (uint8_t) (data[2]);
					valUnicode  = (((uint8_t)data[0]) & 0x0F)<< 12;
					valUnicode += (((uint8_t)data[1]) & 0x3F)<< 6;
					valUnicode +=  ((uint8_t)data[2]) & 0x3F;
					break;
				default:
					valUTF8  = (uint8_t) (data[0]) <<24;
					valUTF8 += (uint8_t) (data[1]) << 16;
					valUTF8 += (uint8_t) (data[2]) << 8;
					valUTF8 += (uint8_t) (data[3]);
					valUnicode  = (((uint8_t)data[0]) & 0x07)<< 18;
					valUnicode += (((uint8_t)data[1]) & 0x3F)<< 12;
					valUnicode += (((uint8_t)data[2]) & 0x3F)<< 6;
					valUnicode +=  ((uint8_t)data[3]) & 0x3F;
					break;
			}
			fprintf(mfileout, "0x%08X, ", valUnicode);
		}
		fprintf(mfileout, "\n\t};\n\n");
		fclose(mfile);
	}
	fclose(mfileout);

#endif


