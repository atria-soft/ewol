/**
 *******************************************************************************
 * @file etk/RegExp.cpp
 * @brief Ewol Tool Kit : Regular expression annalyser (sources)
 * @author Edouard DUPIN
 * @date 04/04/2011
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

#include <etk/RegExp.h>



const etk::convertionTable_ts etk::constConvertionTable[] = {
	// haveBackSlash, inputValue, newValue
	{ false			, '('		, REGEXP_OPCODE_PTHESE_IN},
	{ true			, '('		, (int16_t)'('},
	{ false			, ')'		, REGEXP_OPCODE_PTHESE_OUT},
	{ true			, ')'		, (int16_t)')'},
	{ false			, '['		, REGEXP_OPCODE_BRACKET_IN},
	{ true			, '['		, (int16_t)'['},
	{ false			, ']'		, REGEXP_OPCODE_BRACKET_OUT},
	{ true			, ']'		, (int16_t)']'},
	{ false			, '{'		, REGEXP_OPCODE_BRACE_IN},
	{ true			, '{'		, (int16_t)'{'},
	{ false			, '}'		, REGEXP_OPCODE_BRACE_OUT},
	{ true			, '}'		, (int16_t)'}'},
	{ false			, '-'		, REGEXP_OPCODE_TO},
	{ true			, '-'		, (int16_t)'-'},
	{ false			, '*'		, REGEXP_OPCODE_STAR},
	{ true			, '*'		, (int16_t)'*'},
	{ false			, '.'		, REGEXP_OPCODE_DOT},
	{ true			, '.'		, (int16_t)'.'},
	{ false			, '?'		, REGEXP_OPCODE_QUESTION},
	{ true			, '?'		, (int16_t)'?'},
	{ false			, '+'		, REGEXP_OPCODE_PLUS},
	{ true			, '+'		, (int16_t)'+'},
	{ false			, '|'		, REGEXP_OPCODE_PIPE},
	{ true			, '|'		, (int16_t)'|'},
	{ false			, '^'		, REGEXP_OPCODE_START_OF_LINE},
	{ true			, '^'		, (int16_t)'^'},
	{ false			, '$'		, REGEXP_OPCODE_END_OF_LINE},
	{ true			, '$'		, (int16_t)'$'},
	{ true			, 'd'		, REGEXP_OPCODE_DIGIT},
	{ true			, 'D'		, REGEXP_OPCODE_DIGIT_NOT},
	{ true			, 'l'		, REGEXP_OPCODE_LETTER},
	{ true			, 'L'		, REGEXP_OPCODE_LETTER_NOT},
	{ true			, 's'		, REGEXP_OPCODE_SPACE},
	{ true			, 'S'		, REGEXP_OPCODE_SPACE_NOT},
	{ true			, 'w'		, REGEXP_OPCODE_WORD},
	{ true			, 'W'		, REGEXP_OPCODE_WORD_NOT},
	{ true			, 'a'		, (int16_t)'\a'},
	{ true			, 'b'		, (int16_t)'\b'},
	{ true			, 'e'		, 0x001B}, // Escape character <Esc>
	{ true			, 'f'		, (int16_t)'\f'},
	{ true			, 'n'		, (int16_t)'\n'},
	{ true			, 'r'		, (int16_t)'\r'},
	{ true			, 't'		, (int16_t)'\t'},
	{ true			, 'v'		, (int16_t)'\v'},
	{ true			, '\\'		, (int16_t)'\\'},
	{ true			, '&'		, (int16_t)'&'},
	{ true			, '0'		, (int16_t)'\0'},
	{ true			, '@'		, REGEXP_OPCODE_NO_CHAR},
};
const int32_t etk::constConvertionTableSize = sizeof(etk::constConvertionTable) / sizeof(etk::convertionTable_ts) ;


/**
 * @brief Display the internal data of a node
 *
 * @param[in] data	element do display in the console
 *
 * @return ---
 *
 */
void etk::DisplayData(etk::Vector<char> &data)
{
	int32_t i;
	for (i=0; i<(int32_t)data.Size() ; i++) {
		etk::cout<< (char)(data[i]&0x00FF );
	}
}

/**
 * @brief Display of a part of the Regexp element
 *
 * @param[in] data		Vector where data is constain
 * @param[in] start		Position where the display might start
 * @param[in] stop		Position where the display might stop
 *
 * @return ---
 *
 */
void etk::DisplayElem(etk::Vector<int16_t> &data, int32_t start, int32_t stop)
{
	int32_t i;
	etk::cout<< ETK_BASH_COLOR_NORMAL;
	for (i=start; i<(int32_t)data.Size() && i<stop ; i++) {
		switch(data[i])
		{
			case REGEXP_OPCODE_PTHESE_IN:		etk::cout<<ETK_BASH_COLOR_RED		<< (char*)"(" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_PTHESE_OUT:		etk::cout<<ETK_BASH_COLOR_RED		<< (char*)")" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_BRACKET_IN:		etk::cout<<ETK_BASH_COLOR_YELLOW	<< (char*)"[" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_BRACKET_OUT:		etk::cout<<ETK_BASH_COLOR_YELLOW	<< (char*)"]" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_TO:				etk::cout<<ETK_BASH_COLOR_YELLOW	<< (char*)"-" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_BRACE_IN:		etk::cout<<ETK_BASH_COLOR_GREEN		<< (char*)"{" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_BRACE_OUT:		etk::cout<<ETK_BASH_COLOR_GREEN		<< (char*)"}" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_STAR:			etk::cout<<ETK_BASH_COLOR_BLUE		<< (char*)"*" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_DOT:				etk::cout<<ETK_BASH_COLOR_BLUE		<< (char*)"." << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_QUESTION:		etk::cout<<ETK_BASH_COLOR_BLUE		<< (char*)"?" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_PLUS:			etk::cout<<ETK_BASH_COLOR_BLUE		<< (char*)"+" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_PIPE:			etk::cout<<ETK_BASH_COLOR_BLUE		<< (char*)"|" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_NO_CHAR:			etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"@" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_START_OF_LINE:	etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"^" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_END_OF_LINE:		etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"$" << ETK_BASH_COLOR_NORMAL;		break;
			case REGEXP_OPCODE_DIGIT:			etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\d" << ETK_BASH_COLOR_NORMAL;	break;
			case REGEXP_OPCODE_DIGIT_NOT:		etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\D" << ETK_BASH_COLOR_NORMAL;	break;
			case REGEXP_OPCODE_LETTER:			etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\l" << ETK_BASH_COLOR_NORMAL;	break;
			case REGEXP_OPCODE_LETTER_NOT:		etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\L" << ETK_BASH_COLOR_NORMAL;	break;
			case REGEXP_OPCODE_SPACE:			etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\s" << ETK_BASH_COLOR_NORMAL;	break;
			case REGEXP_OPCODE_SPACE_NOT:		etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\S" << ETK_BASH_COLOR_NORMAL;	break;
			case REGEXP_OPCODE_WORD:			etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\w" << ETK_BASH_COLOR_NORMAL;	break;
			case REGEXP_OPCODE_WORD_NOT:		etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\W" << ETK_BASH_COLOR_NORMAL;	break;
			case '\n':							etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\n" << ETK_BASH_COLOR_NORMAL;	break;
			case '\t':							etk::cout<<ETK_BASH_COLOR_MAGENTA	<< (char*)"\\t" << ETK_BASH_COLOR_NORMAL;	break;
			default:							etk::cout<< (char)(data[i]&0x00FF );											break;
		}
	}
}
char * etk::levelSpace(int32_t level)
{
	switch(level)
	{
		case 0:		return (char*)"";
		case 1:		return (char*)"  ";
		case 2:		return (char*)"    ";
		case 3:		return (char*)"      ";
		case 4:		return (char*)"        ";
		case 5:		return (char*)"          ";
		case 6:		return (char*)"            ";
		case 7:		return (char*)"              ";
		case 8:		return (char*)"                ";
		case 9:		return (char*)"                  ";
		case 10:	return (char*)"                    ";
		case 11:	return (char*)"                      ";
		case 12:	return (char*)"                        ";
		case 13:	return (char*)"                          ";
		case 14:	return (char*)"                            ";
		case 15:	return (char*)"                              ";
		case 16:	return (char*)"                                ";
		default:	return (char*)"                                  ";
	}
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
int32_t etk::GetLenOfPTheseElem(etk::Vector<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;
	int32_t nbOpen = 0;
	// special case of the (...) or | ==> we search '|' or ')'
	if(		REGEXP_OPCODE_PTHESE_OUT	== data[pos]
		||	REGEXP_OPCODE_PIPE			== data[pos]) {
		return 0;
	}
	// find size ...
	while (pos < (int32_t)data.Size() ) {
		if(REGEXP_OPCODE_PTHESE_IN == data[pos]) {
			// find a sub section : 
			nbOpen++;
		} else if(0 < nbOpen) {
			if (REGEXP_OPCODE_PTHESE_OUT == data[pos])
			{
				nbOpen--;
				if (0 > nbOpen) {
					TK_ERROR("Error in the (...) find element at "<< pos);
					return -1;
				}
			}
		} else if(		REGEXP_OPCODE_PTHESE_OUT	== data[pos]
					||	REGEXP_OPCODE_PIPE			== data[pos])
		{
			// Find the end of the (...)
			// just return the size inside
			int32_t sizeInside = pos - startPos;
			if (0 >= sizeInside) {
				TK_ERROR("Error in the (...) no data at "<< pos-1);
				return -1;
			} else {
					return sizeInside;
			}
		}
		pos++;
	}
	return pos - startPos;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
int32_t etk::GetLenOfPThese(etk::Vector<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;
	int32_t nbOpen = 0;
	// special case of the (...) or | ==> we search '|' or ')'
	if( REGEXP_OPCODE_PTHESE_OUT == data[pos]) {
		return 0;
	} else if( REGEXP_OPCODE_PTHESE_IN == data[pos])
	{
		pos++;
		// find size ...
		while (pos < (int32_t)data.Size() ) {
			if(REGEXP_OPCODE_PTHESE_IN == data[pos]) {
				// find a sub section : 
				nbOpen++;
			} else if(0 < nbOpen) {
				if (REGEXP_OPCODE_PTHESE_OUT == data[pos])
				{
					nbOpen--;
					if (0 > nbOpen) {
						TK_ERROR("Error in the (...) find element at "<< pos);
						return -1;
					}
				}
			} else if( REGEXP_OPCODE_PTHESE_OUT	== data[pos])
			{
				// Find the end of the (...)
				// just return the size inside
				int32_t sizeInside = pos - startPos-1;
				if (0 >= sizeInside) {
					TK_ERROR("Error in the (...) no data at "<< pos-1);
					return -1;
				} else {
					return sizeInside;
				}
			}
			pos++;
		}
	} else {
		return -1;
	}
	return 0;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
int32_t etk::GetLenOfBracket(etk::Vector<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;
	// special case of the (...) or | ==> we search '|' or ')'
	if( REGEXP_OPCODE_BRACKET_OUT == data[pos]) {
		return 0;
	} else if( REGEXP_OPCODE_BRACKET_IN == data[pos]) {
		pos++;
		// find size ...
		while (pos < (int32_t)data.Size() ) {
			if(REGEXP_OPCODE_BRACKET_OUT == data[pos]) {
				// Find the end of the [...]
				// just return the size inside
				int32_t sizeInside = pos - startPos -1 ;
				if (0 >= sizeInside) {
					TK_ERROR("Error in the [...] no data at "<< pos-1);
					return sizeInside;
				} else {
					return sizeInside;
				}
			} else if(		REGEXP_OPCODE_TO != data[pos]
						&&	(		0 > data[pos]
								||	0xFF < data[pos]) )
			{
				TK_ERROR("Error in the [...] not permited element at "<< pos << " '" << (char)data[pos] << "'");
				return false;
			}
			pos++;
		}
	} else {
		return -1;
	}
	return 0;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
int32_t etk::GetLenOfBrace(etk::Vector<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;
	// special case of the (...) or | ==> we search '|' or ')'
	if( REGEXP_OPCODE_BRACE_OUT == data[pos]) {
		return 0;
	} else if( REGEXP_OPCODE_BRACE_IN == data[pos]) {
		pos++;
		// find size ...
		while (pos < (int32_t)data.Size() ) {
			if(REGEXP_OPCODE_BRACE_OUT == data[pos]) {
				// Find the end of the [...]
				// just return the size inside
				int32_t sizeInside = pos - startPos -1 ;
				if (0 >= sizeInside) {
					TK_ERROR("Error in the {...} no data at "<< pos-1);
					return sizeInside;
				} else {
					return sizeInside;
				}
			} else if(		',' != data[pos]
						&&	(		'0' > data[pos]
								||	'9' < data[pos]) )
			{
				TK_ERROR("Error in the {...} not permited element at "<< pos << " '" << (char)data[pos] << "'");
				return false;
			}
			pos++;
		}
	} else {
		return -1;
	}
	return 0;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
int32_t etk::GetLenOfNormal(etk::Vector<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;

	// find size ...
	while (pos < (int32_t)data.Size() ) {
		switch(data[pos])
		{
			case REGEXP_OPCODE_PTHESE_IN:
			case REGEXP_OPCODE_PTHESE_OUT:
			case REGEXP_OPCODE_BRACKET_IN:
			case REGEXP_OPCODE_BRACKET_OUT:
			case REGEXP_OPCODE_BRACE_IN:
			case REGEXP_OPCODE_BRACE_OUT:
			case REGEXP_OPCODE_TO:
			case REGEXP_OPCODE_STAR:
			case REGEXP_OPCODE_DOT:
			case REGEXP_OPCODE_QUESTION:
			case REGEXP_OPCODE_PLUS:
			case REGEXP_OPCODE_PIPE:
			case REGEXP_OPCODE_START_OF_LINE:
			case REGEXP_OPCODE_END_OF_LINE:
			case REGEXP_OPCODE_DIGIT:
			case REGEXP_OPCODE_DIGIT_NOT:
			case REGEXP_OPCODE_LETTER:
			case REGEXP_OPCODE_LETTER_NOT:
			case REGEXP_OPCODE_SPACE:
			case REGEXP_OPCODE_SPACE_NOT:
			case REGEXP_OPCODE_WORD:
			case REGEXP_OPCODE_WORD_NOT:
				{
					// just return the size inside
					int32_t sizeInside = pos - startPos;
					if (0 >= sizeInside) {
						TK_ERROR("Error in the normal data :  no data ...");
					}
					return sizeInside;
				}
				break;
			default :
				// nothing to do ...
				break;
		}
		pos++;
	}
	return pos - startPos ;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
bool etk::ParseBrace(etk::Vector<int16_t> &data, int32_t &min, int32_t &max)
{
	//TK_INFO("parse {...} in "; DisplayElem(data); );
	int32_t k=0;
	
	int32_t firstElement = 0;
	int32_t SecondElement = 0;
	
	while(k<data.Size()) {
		if (',' == (char)data[k]) {
			k++;
			break;
		} if ('}' == (char)data[k]) {
			SecondElement = firstElement;
			goto allIsSet;
		} else if ('0' <= (char)data[k] && '9' >= (char)data[k]) {
			firstElement *=10;
			firstElement += (char)data[k] - '0';
		} else {
			TK_ERROR("Can not parse this element " << (char)data[k] << " at pos " << k);
			return false;
		}
		k++;
	}
	if (k==data.Size()) {
		SecondElement = firstElement;
	}
	while(k<data.Size()) {
		if (',' == (char)data[k]) {
			TK_ERROR("Can not find a second , in {} at pos " << k);
			return false;
		} if ('}' == (char)data[k]) {
			goto allIsSet;
		} else if ('0' <= (char)data[k] && '9' >= (char)data[k]) {
			SecondElement *=10;
			SecondElement += (char)data[k] - '0';
		} else {
			TK_ERROR("Can not parse this element " << (char)data[k] << " at pos " << k);
			return false;
		}
		k++;
	}
	
allIsSet:
	if (SecondElement == 0 && firstElement != 0) {
		min = 0;
		max = firstElement;
	} else {
		min = firstElement;
		max = SecondElement;
	}
	if (min > max) {
		TK_ERROR("Minimum=" << min << " can not be < maximum=" << max );
		return false;
	}
	return true;
}


