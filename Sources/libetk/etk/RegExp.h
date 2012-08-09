/**
 *******************************************************************************
 * @file etk/RegExp.h
 * @brief Ewol Tool Kit : Regular expression annalyser (header)
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

#ifndef __TK_REG_EXP_H__
#define __TK_REG_EXP_H__

#include <etk/Types.h>
#include <etk/DebugInternal.h>
#include <etk/Memory.h>
#include <etk/UString.h>
#include <etk/VectorType.h>

namespace etk {

/*
normal mode :
	(...)				sub element is separate with |
	\d					Digits									[0-9]
	\D					NOT a digit								[^0-9]
	\l					Letters									[a-zA-Z]
	\L					NOT a Letter							[^a-zA-Z]
	\s					Whitespace								[ \t\n\r\f\v]
	\S					NOT Whitespace							[^ \t\n\r\f\v]
	\w					"Word" character						[a-zA-Z0-9_]
	\W					NOT a "Word" character					[^a-zA-Z0-9_]
	\@					at the start or the end					not in the parsing of element ==> check if \w is not present   (other regExp will be <> ...)
	[anjdi] or [a-gt-j]	range
	.					dot										[^\x00-\x08\x0A-\x1F\x7F]
==> TODO :
	$					End / Start of line of line 			==> ce sera un truc suplémentaire comme le \@
	^in the []			invertion of the range element

multiplicity :
	*     ==> {0, 2147483647}
	?     ==> {0, 1}
	+     ==> {1, 2147483647}
	{x}   ==> {x, x}
	{x,y} ==> {x, y}
*/


// internal define to permit to have all needed system
#define REGEXP_OPCODE_PTHESE_IN			(-300)	/* ( */
#define REGEXP_OPCODE_PTHESE_OUT		( 300)	/* ) */
#define REGEXP_OPCODE_BRACKET_IN		(-301)	/* [ */
#define REGEXP_OPCODE_BRACKET_OUT		( 301)	/* ] */
#define REGEXP_OPCODE_BRACE_IN			(-302)	/* { */
#define REGEXP_OPCODE_BRACE_OUT			( 302)	/* } */
#define REGEXP_OPCODE_TO				(-305)	/* - */
#define REGEXP_OPCODE_STAR				(-306)	/* * */
#define REGEXP_OPCODE_DOT				(-307)	/* . */
#define REGEXP_OPCODE_QUESTION			(-308)	/* ? */
#define REGEXP_OPCODE_PLUS				(-309)	/* + */
#define REGEXP_OPCODE_PIPE				(-310)	/* | */
#define REGEXP_OPCODE_START_OF_LINE		(-311)	/* ^  this is also NOT, but not manage */
#define REGEXP_OPCODE_END_OF_LINE		(-312)	/* $ */
#define REGEXP_OPCODE_DIGIT				( 313)	/* \d */
#define REGEXP_OPCODE_DIGIT_NOT			(-313)	/* \D */
#define REGEXP_OPCODE_LETTER			( 314)	/* \l */
#define REGEXP_OPCODE_LETTER_NOT		(-314)	/* \L */
#define REGEXP_OPCODE_SPACE				( 315)	/* \s */
#define REGEXP_OPCODE_SPACE_NOT			(-315)	/* \S */
#define REGEXP_OPCODE_WORD				( 316)	/* \w */
#define REGEXP_OPCODE_WORD_NOT			(-316)	/* \W */
#define REGEXP_OPCODE_NO_CHAR			(-317)	/* \@ */

typedef struct {
	bool		haveBackSlash;
	char		inputValue;
	int16_t		newValue;
}convertionTable_ts;

extern const convertionTable_ts constConvertionTable[];
extern const int32_t constConvertionTableSize;

void DisplayData(etk::VectorType<char> &data);
void DisplayElem(etk::VectorType<int16_t> &data, int32_t start=0, int32_t stop=0x7FFFFFFF);
char * levelSpace(int32_t level);
int32_t GetLenOfPTheseElem(etk::VectorType<int16_t> &data, int32_t startPos);
int32_t GetLenOfPThese(etk::VectorType<int16_t> &data, int32_t startPos);
int32_t GetLenOfBracket(etk::VectorType<int16_t> &data, int32_t startPos);
int32_t GetLenOfBrace(etk::VectorType<int16_t> &data, int32_t startPos);
int32_t GetLenOfNormal(etk::VectorType<int16_t> &data, int32_t startPos);
bool ParseBrace(etk::VectorType<int16_t> &data, int32_t &min, int32_t &max);


#undef __class__
#define __class__	"etk::RegExpNode"

/**
 *	@brief Node Elements for every-one
 */
template<class CLASS_TYPE> class RegExpNode{
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNode(void)
		{
			SetMult(1,1);
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		virtual ~RegExpNode(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		virtual int32_t Generate(etk::VectorType<int16_t> &data)
		{
			return 0;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		virtual bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		virtual void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@???@ {" << GetMultMin() << "," << GetMultMax() << "}  subdata="; DisplayElem(m_RegExpData););
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void SetMult(int32_t min, int32_t max)
		{
			m_multipleMin = etk_max(min, 0);
			m_multipleMax = etk_max(max, 1);
		}
	protected:
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		int32_t GetMultMin(void)
		{
			return m_multipleMin;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		int32_t GetMultMax(void)
		{
			return m_multipleMax;
		};
	protected :
		int32_t                 m_multipleMin;      //!< minimum repetition (included)
		int32_t                 m_multipleMax;      //!< maximum repetition (included)
		// Data Section ... (can have no data...)
		etk::VectorType<int16_t>   m_RegExpData;       //!< data to parse and compare in some case ...
};

#undef __class__
#define __class__	"etk::RegExpNodeValue"

template<class CLASS_TYPE> class RegExpNodeValue : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeValue(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeValue(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		int32_t Generate(etk::VectorType<int16_t> &data)
		{
			RegExpNode<CLASS_TYPE>::m_RegExpData = data;
			//TK_DEBUG("Request Parse \"Value\" data="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
			m_data.Clear();
			for (int32_t i=0; i<RegExpNode<CLASS_TYPE>::m_RegExpData.Size(); i++) {
				m_data.PushBack((char)RegExpNode<CLASS_TYPE>::m_RegExpData[i]);
			}
			return data.Size();
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : Value{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			if (0==m_data.Size()) {
				TK_ERROR("No data inside type elemTypeValue");
				return false;
			}
			//TK_DEBUG("check element value : '" << m_data[0] << "'");
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind == true; j++) {
				int32_t ofset = 0;
				int32_t k;
				for (k=0; findLen+k<lenMax && k < m_data.Size(); k++) {
					if (m_data[k] != data[currentPos+findLen+k]) {
						tmpFind=false;
						break;
					}
					ofset++;
				}
				if (k != (int32_t)m_data.Size()) {
					// parsing not ended ...
					tmpFind=false;
				}
				// Update local ofset of data
				if (true == tmpFind) {
					findLen += ofset;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@Value@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData); etk::cout<< " data: "; DisplayData(m_data); );
		};
	protected :
		// SubNodes :
		etk::VectorType<char> m_data;
};
#undef __class__
#define __class__	"etk::RegExpNodeBracket"

template<class CLASS_TYPE> class RegExpNodeBracket : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeBracket(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeBracket(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		int32_t Generate(etk::VectorType<int16_t> &data)
		{
			RegExpNode<CLASS_TYPE>::m_RegExpData = data;
			//TK_DEBUG("Request Parse [...] data="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
			m_data.Clear();
			
			char lastElement = 'a';
			bool multipleElement = false;
			//
			for (int32_t k=0; k<RegExpNode<CLASS_TYPE>::m_RegExpData.Size(); k++) {
				if (RegExpNode<CLASS_TYPE>::m_RegExpData[k] == REGEXP_OPCODE_TO && multipleElement == true) {
					TK_ERROR("Can not have 2 consecutive - in [...]");
					return 0;
				} else if (multipleElement == true) {
					char j='\0';
					for (j=lastElement+1; j <= (char)RegExpNode<CLASS_TYPE>::m_RegExpData[k]; j++) {
						m_data.PushBack(j);
					}
					multipleElement = false;
				} else if(RegExpNode<CLASS_TYPE>::m_RegExpData[k] == REGEXP_OPCODE_TO) {
					multipleElement = true;
				} else {
					lastElement = (char)RegExpNode<CLASS_TYPE>::m_RegExpData[k];
					m_data.PushBack(lastElement);
				}
			}
			// check size ...
			if (m_data.Size() == 0) {
				TK_ERROR("No data inside [...] ");
				return 0;
			}
			return data.Size();
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : [...]{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			if (0==m_data.Size()) {
				TK_ERROR("No data inside type elemTypeValue");
				return false;
			}
			//TK_DEBUG("one of element value List : "; DisplayData(element->m_data););
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				int32_t i;
				tmpFind=false;
				for (i=0; i<m_data.Size(); i++) {
					if (m_data[i] == data[currentPos+j]) {
						findLen += 1;
						tmpFind=true;
						break;
					}
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@[...]@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData); etk::cout<< " data: "; DisplayData(m_data); );
		};
	protected :
		// SubNodes :
		etk::VectorType<char> m_data;
};
#undef __class__
#define __class__	"etk::RegExpNodeDigit"

template<class CLASS_TYPE> class RegExpNodeDigit : public RegExpNode<CLASS_TYPE> {
	public :
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeDigit(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeDigit(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : Digit{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "} : "<< data[currentPos] << " lenMax=" << lenMax);
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				char tmpVal = data[currentPos+j];
				//TK_DEBUG("compare : " << tmpVal);
				if(    '0' <= tmpVal
				    && '9' >= tmpVal)
				{
					//TK_DEBUG("find ++");
					findLen += 1;
				} else {
					tmpFind=false;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@Digit@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};
#undef __class__
#define __class__	"etk::RegExpNodeDigitNot"

template<class CLASS_TYPE> class RegExpNodeDigitNot : public RegExpNode<CLASS_TYPE> {
	public :
		RegExpNodeDigitNot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeDigitNot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : DigitNot{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				char tmpVal = data[currentPos+j];
				if(		'0' > tmpVal
					||	'9' < tmpVal)
				{
					findLen += 1;
				} else {
					tmpFind=false;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@DigitNot@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};
#undef __class__
#define __class__	"etk::RegExpNodeLetter"

template<class CLASS_TYPE> class RegExpNodeLetter : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeLetter(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeLetter(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : Letter{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				char tmpVal = data[currentPos+j];
				if(		(		'a' <= tmpVal
							&&	'z' >= tmpVal	)
					||	(		'A' <= tmpVal
							&&	'Z' >= tmpVal	))
				{
					findLen += 1;
				} else {
					tmpFind=false;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@Letter@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};
#undef __class__
#define __class__	"etk::RegExpNodeLetterNot"

template<class CLASS_TYPE> class RegExpNodeLetterNot : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeLetterNot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeLetterNot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : LetterNot{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				char tmpVal = data[currentPos+j];
				if(		(		'a' > tmpVal
							&&	'Z' < tmpVal	)
					||	'A' > tmpVal
					||	'z' < tmpVal	)
				{
					findLen += 1;
				} else {
					tmpFind=false;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@LetterNot@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};
#undef __class__
#define __class__	"etk::RegExpNodeWhiteSpace"

template<class CLASS_TYPE> class RegExpNodeWhiteSpace : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeWhiteSpace(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeWhiteSpace(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : Space{" << m_multipleMin << "," << m_multipleMax << "}");
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				char tmpVal = data[currentPos+j];
				if(		' '  == tmpVal
					||	'\t' == tmpVal
					||	'\n' == tmpVal
					||	'\r' == tmpVal
					||	'\f' == tmpVal
					||	'\v' == tmpVal	)
				{
					findLen += 1;
				} else {
					tmpFind=false;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@Space@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};
#undef __class__
#define __class__	"etk::RegExpNodeWhiteSpaceNot"

template<class CLASS_TYPE> class RegExpNodeWhiteSpaceNot : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeWhiteSpaceNot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeWhiteSpaceNot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : SpaceNot{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				char tmpVal = data[currentPos+j];
				if(		' '  != tmpVal
					&&	'\t' != tmpVal
					&&	'\n' != tmpVal
					&&	'\r' != tmpVal
					&&	'\f' != tmpVal
					&&	'\v' != tmpVal	)
				{
					findLen += 1;
				} else {
					tmpFind=false;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@SpaceNot@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};
#undef __class__
#define __class__	"etk::RegExpNodeWordChar"

template<class CLASS_TYPE> class RegExpNodeWordChar : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeWordChar(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeWordChar(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : Word{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				char tmpVal = data[currentPos+j];
				if(		(		'a' <= tmpVal
							&&	'z' >= tmpVal	)
					||	(		'A' <= tmpVal
							&&	'Z' >= tmpVal	)
					||	(		'0' <= tmpVal
							&&	'9' >= tmpVal	))
				{
					findLen += 1;
				} else {
					tmpFind=false;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@Word@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};
#undef __class__
#define __class__	"etk::RegExpNodeWordCharNot"

template<class CLASS_TYPE> class RegExpNodeWordCharNot : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeWordCharNot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeWordCharNot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : WordNot{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				char tmpVal = data[currentPos+j];
				if(		(		'A' > tmpVal
							&&	'9' < tmpVal	)
					||	(		'a' > tmpVal
							&&	'Z' < tmpVal	)
					||	'0' > tmpVal
					||	'z' < tmpVal	)
				{
					findLen += 1;
				} else {
					tmpFind=false;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@WordNot@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};
#undef __class__
#define __class__	"etk::RegExpNodeDot"

template<class CLASS_TYPE> class RegExpNodeDot : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeDot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeDot(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : '.'{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			// equivalent a : [^\x00-\x08\x0A-\x1F\x7F]
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind ==true && j < lenMax; j++) {
				char tmpVal = data[currentPos+j];
				if(		(		0x08 < tmpVal
							&&	0x0A > tmpVal	)
					||	(		0x1F < tmpVal
							&&	0x7F > tmpVal	)
					||	(		0x7F < tmpVal
							&&	0xFF > tmpVal	))
				{
					findLen += 1;
				} else {
					tmpFind=false;
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@.@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};

#undef __class__
#define __class__	"etk::RegExpNodeSOL"

template<class CLASS_TYPE> class RegExpNodeSOL : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeSOL(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeSOL(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			TK_INFO("Parse node : SOL{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@SOL@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};

#undef __class__
#define __class__	"etk::RegExpNodeEOL"

template<class CLASS_TYPE> class RegExpNodeEOL : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodeEOL(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodeEOL(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			TK_INFO("Parse node : EOL{" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@EOL@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		};
};

typedef struct {
	int32_t start;
	int32_t stop;
}elementPos_ts;

#undef __class__
#define __class__	"etk::RegExpNodePTheseElem"

template<class CLASS_TYPE> class RegExpNodePThese;

template<class CLASS_TYPE> class RegExpNodePTheseElem : public RegExpNode<CLASS_TYPE> {
	public :
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodePTheseElem(void)
		{
			
		};
		
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodePTheseElem(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		int32_t Generate(etk::VectorType<int16_t> &data)
		{
			RegExpNode<CLASS_TYPE>::m_RegExpData = data;
			//TK_DEBUG("Request Parse (elem) data="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
		
			int32_t pos = 0;
			int32_t elementSize = 0;
			etk::VectorType<int16_t> tmpData;
			while (pos < RegExpNode<CLASS_TYPE>::m_RegExpData.Size()) {
				tmpData.Clear();
				switch (RegExpNode<CLASS_TYPE>::m_RegExpData[pos])
				{
					case REGEXP_OPCODE_PTHESE_IN:
						{
							elementSize=GetLenOfPThese(RegExpNode<CLASS_TYPE>::m_RegExpData, pos);
							for (int32_t k=pos+1; k<pos+elementSize+1; k++) {
								tmpData.PushBack(RegExpNode<CLASS_TYPE>::m_RegExpData[k]);
							}
							RegExpNodePThese<CLASS_TYPE> * myElem = new RegExpNodePThese<CLASS_TYPE>();
							(void)myElem->Generate(tmpData);
							// add to the subnode list : 
							m_subNode.PushBack(myElem);
							// move current position ...
							pos += elementSize+1;
						}
						break;
					case REGEXP_OPCODE_PTHESE_OUT:
						TK_ERROR("Impossible case :  ')' " << pos);
						return false;
				
					case REGEXP_OPCODE_BRACKET_IN:
						{
							elementSize=GetLenOfBracket(RegExpNode<CLASS_TYPE>::m_RegExpData, pos);
							for (int32_t k=pos+1; k<pos+elementSize+1; k++) {
								tmpData.PushBack(RegExpNode<CLASS_TYPE>::m_RegExpData[k]);
							}
							RegExpNodeBracket<CLASS_TYPE> * myElem = new RegExpNodeBracket<CLASS_TYPE>();
							(void)myElem->Generate(tmpData);
							// add to the subnode list : 
							m_subNode.PushBack(myElem);
							// move current position ...
							pos += elementSize+1;
						}
						break;
					case REGEXP_OPCODE_BRACKET_OUT:
						TK_ERROR("Impossible case :  ']' " << pos);
						return false;
				
					case REGEXP_OPCODE_BRACE_IN:
						{
							elementSize=GetLenOfBrace(RegExpNode<CLASS_TYPE>::m_RegExpData, pos);
							for (int32_t k=pos+1; k<pos+elementSize+1; k++) {
								tmpData.PushBack(RegExpNode<CLASS_TYPE>::m_RegExpData[k]);
							}
							int32_t min = 0;
							int32_t max = 0;
							if (false == ParseBrace(tmpData, min, max)) {
								return false;
							}
							SetMultiplicityOnLastNode(min, max);
							pos += elementSize+1;
						}
						break;
					case REGEXP_OPCODE_BRACE_OUT:
						TK_ERROR("Impossible case :  '}' " << pos);
						return false;
				
					case REGEXP_OPCODE_TO:
						TK_ERROR("Impossible case :  '-' " << pos);
						return false;
				
					case REGEXP_OPCODE_STAR:
						SetMultiplicityOnLastNode(0, 0x7FFFFFFF);
						break;
				
					case REGEXP_OPCODE_QUESTION:
						SetMultiplicityOnLastNode(0, 1);
						break;
				
					case REGEXP_OPCODE_PLUS:
						SetMultiplicityOnLastNode(1, 0x7FFFFFFF);
						break;
				
					case REGEXP_OPCODE_PIPE:
						TK_ERROR("Impossible case :  '|' " << pos);
						return false;
				
					case REGEXP_OPCODE_DOT:
						m_subNode.PushBack(new RegExpNodeDot<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_START_OF_LINE:
						m_subNode.PushBack(new RegExpNodeSOL<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_END_OF_LINE:
						m_subNode.PushBack(new RegExpNodeEOL<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_DIGIT:
						m_subNode.PushBack(new RegExpNodeDigit<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_DIGIT_NOT:
						m_subNode.PushBack(new RegExpNodeDigitNot<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_LETTER:
						m_subNode.PushBack(new RegExpNodeLetter<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_LETTER_NOT:
						m_subNode.PushBack(new RegExpNodeLetterNot<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_SPACE:
						m_subNode.PushBack(new RegExpNodeWhiteSpace<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_SPACE_NOT:
						m_subNode.PushBack(new RegExpNodeWhiteSpaceNot<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_WORD:
						m_subNode.PushBack(new RegExpNodeWordChar<CLASS_TYPE>());
						break;
				
					case REGEXP_OPCODE_WORD_NOT:
						m_subNode.PushBack(new RegExpNodeWordCharNot<CLASS_TYPE>());
						break;
		
					default:
						{
							elementSize=GetLenOfNormal(RegExpNode<CLASS_TYPE>::m_RegExpData, pos);
							for (int32_t k=pos; k<pos+elementSize; k++) {
								tmpData.PushBack(RegExpNode<CLASS_TYPE>::m_RegExpData[k]);
							}
							RegExpNodeValue<CLASS_TYPE> * myElem = new RegExpNodeValue<CLASS_TYPE>();
							(void)myElem->Generate(tmpData);
							// add to the subnode list : 
							m_subNode.PushBack(myElem);
							// move current position ...
							pos += elementSize-1;
						}
						break;
				}
				pos++;
			}
			return data.Size();
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : (Elem){" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			// NOTE 1 : Must done only one time in EVERY case ...
			// NOTE 2 : All element inside must be OK
			if (0 == m_subNode.Size()) {
				return false;
			}
			int32_t tmpCurrentPos = currentPos;
			for (int32_t i=0; i<m_subNode.Size(); i++) {
				int32_t tmpFindLen;
				if (false == m_subNode[i]->Parse(data, tmpCurrentPos, lenMax, tmpFindLen)) {
					findLen = 0;
					return false;
				} else {
					tmpCurrentPos += tmpFindLen;
				}
			}
			findLen = tmpCurrentPos - currentPos;
			return true;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			TK_INFO("Find NODE : " << levelSpace(level) << "@(Elem)@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
			for(int32_t i=0; i<m_subNode.Size(); i++) {
				m_subNode[i]->Display(level+1);
			}
		};
	protected :
		// SubNodes :
		etk::VectorType<RegExpNode<CLASS_TYPE>*> m_subNode;
	private :
		/**
		 * @brief Set the number of repeate time on a the last node in the list ...
		 *
		 * @param[in] min		Minimum of the multiplicity
		 * @param[in] max		Maximum of the multiplicity
		 *
		 * @return true if we find the node, false otherwise
		 *
		 */
		bool SetMultiplicityOnLastNode(int32_t min, int32_t max)
		{
			if (0==m_subNode.Size()) {
				TK_ERROR("Set multiplicity on an inexistant element ....");
				return false;
			}
			RegExpNode<CLASS_TYPE> * myNode = m_subNode[m_subNode.Size()-1];
			if (NULL==myNode) {
				TK_ERROR("INTERNAL error ==> node not generated");
				return false;
			}
			myNode->SetMult(min, max);
			return true;
		}
};

#undef __class__
#define __class__	"etk::RegExpNodePThese"

template<class CLASS_TYPE> class RegExpNodePThese : public RegExpNode<CLASS_TYPE> {
	public :
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExpNodePThese(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExpNodePThese(void)
		{
			
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		int32_t Generate(etk::VectorType<int16_t> &data)
		{
			RegExpNode<CLASS_TYPE>::m_RegExpData = data;
			//TK_DEBUG("Request Parse (...) data="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
			//Find all the '|' in the string (and at the good level ...) 
			int32_t pos = 0;
			int32_t elementSize = GetLenOfPTheseElem(RegExpNode<CLASS_TYPE>::m_RegExpData, pos);
			// generate all the "elemTypePTheseElem" of the Node
			while (elementSize>0) {
				// geerate output deta ...
				etk::VectorType<int16_t> tmpData;
				for (int32_t k=pos; k<pos+elementSize; k++) {
					tmpData.PushBack(RegExpNode<CLASS_TYPE>::m_RegExpData[k]);
				}
				RegExpNodePTheseElem<CLASS_TYPE> * myElem = new RegExpNodePTheseElem<CLASS_TYPE>();
				(void)myElem->Generate(tmpData);
				// add to the subnode list : 
				m_subNode.PushBack(myElem);
				pos += elementSize+1;
				//TK_DEBUG("plop="; DisplayElem(data, pos, pos+1););
				elementSize = GetLenOfPTheseElem(RegExpNode<CLASS_TYPE>::m_RegExpData, pos);
				//TK_DEBUG("find " << elementSize << " elements");
			}
			if (0 == pos && 0 == elementSize) {
				TK_ERROR("No data in the (...) element at " << pos);
				return false;
			}
			return data.Size();
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Parse(CLASS_TYPE &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
		{
			findLen = 0;
			//TK_INFO("Parse node : (...){" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}");
			if (0 == m_subNode.Size()) {
				return false;
			}
			bool tmpFind = true;
			int32_t j;
			for (j=0; j<RegExpNode<CLASS_TYPE>::m_multipleMax && tmpFind == true ; j++) {
				tmpFind = false;
				for (int32_t i=0; i<m_subNode.Size(); i++) {
					int32_t tmpFindLen;
					if (true == m_subNode[i]->Parse(data, currentPos+findLen, lenMax, tmpFindLen)) {
						findLen += tmpFindLen;
						tmpFind = true;
					}
				}
			}
			if(		j>=RegExpNode<CLASS_TYPE>::m_multipleMin
				&&	j<=RegExpNode<CLASS_TYPE>::m_multipleMax
				&&	findLen>0	)
			{
				//TK_DEBUG("find " << findLen);
				return true;
			} else if( 0 == RegExpNode<CLASS_TYPE>::m_multipleMin ) {
				//TK_DEBUG("find size=0");
				return true;
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(int32_t level)
		{
			if (-1 == level) {
				TK_INFO("regExp :"; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
			} else {
				TK_INFO("Find NODE : " << levelSpace(level) << "@(...)@ {" << RegExpNode<CLASS_TYPE>::m_multipleMin << "," << RegExpNode<CLASS_TYPE>::m_multipleMax << "}  subdata="; DisplayElem(RegExpNode<CLASS_TYPE>::m_RegExpData););
				for(int32_t i=0; i<m_subNode.Size(); i++) {
					m_subNode[i]->Display(level+1);
				}
			}
		};
		
	protected :
		// SubNodes :
		etk::VectorType<RegExpNode<CLASS_TYPE>*> m_subNode;
		//int32_t						m_posPthese;		//!< position of the element is detected in the output element
};
#undef __class__
#define __class__	"etk::RegExp"

// Regular expression manager
template<class CLASS_TYPE> class RegExp {
	// public API : 
	public:
		// create the regular expression
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExp(const char *exp)
		{
			m_isOk = false;
			m_areaFind.start=0;
			m_areaFind.stop=0;
			m_notBeginWithChar = false;
			m_notEndWithChar = false;
			SetRegExp(exp);
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExp(etk::UString &exp)
		{
			m_isOk = false;
			m_areaFind.start=0;
			m_areaFind.stop=0;
			m_notBeginWithChar = false;
			m_notEndWithChar = false;
			SetRegExp(exp);
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		RegExp(void)
		{
			m_isOk = false;
			m_areaFind.start=0;
			m_areaFind.stop=0;
			m_notBeginWithChar = false;
			m_notEndWithChar = false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		~RegExp(void)
		{
			// TODO : remove all under nodes...
			m_isOk = false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void SetRegExp(const char *exp)
		{
			TK_CHECK_INOUT(exp);
			etk::UString expressionRequested = exp;
			SetRegExp(expressionRequested);
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void SetRegExp(etk::UString &expressionRequested)
		{
			m_expressionRequested = expressionRequested;		// TODO : Must be deprecated ...
			etk::VectorType<int16_t> tmpExp;
			
			//TK_DEBUG("Parse RegExp : " << expressionRequested.c_str() );
			m_isOk = false;
			m_areaFind.start=0;
			m_areaFind.stop=0;
			m_notBeginWithChar = false;
			m_notEndWithChar = false;
			
			// TODO : Check this ... ==> could create some errors ...
			char * exp = expressionRequested.Utf8Data();
			int32_t regExpLen = strlen(exp);
			// change in the regular Opcode ==> replace \x with the corect element ... x if needed
			int32_t iii;
			int32_t countBraceIn = 0;
			int32_t countBraceOut = 0;
			int32_t countPTheseIn = 0;
			int32_t countPTheseOut = 0;
			int32_t countBracketIn = 0;
			int32_t countBracketOut = 0;
			for (iii=0; iii<regExpLen; iii++) {
				if ('\\' == exp[iii]) {
					if(iii+1>=regExpLen) {
						TK_ERROR("Dangerous parse of the element pos " << iii << " \\ with nothing after");
						// TODO : Generate Exeption ...
						return;
					}
					int32_t j;
					// Find the element in the list...
					for (j=0; j<constConvertionTableSize; j++) {
						if(		true == constConvertionTable[j].haveBackSlash 
							&&	exp[iii+1] == constConvertionTable[j].inputValue)
						{
							tmpExp.PushBack(constConvertionTable[j].newValue);
							break;
						}
					}
					// check error : 
					if (j==constConvertionTableSize) {
						TK_ERROR("Dangerous parse of the \\ " << exp[iii+1] << " at element " << iii);
						// TODO : Generate Exeption ...
						return;
					}
					// less one char in the regular expression ...
					iii++;
				} else {
					if ('(' == exp[iii]) {
						countPTheseIn++;
					} else if (')' == exp[iii]) {
						countPTheseOut++;
					} else if ('[' == exp[iii]) {
						countBracketIn++;
					} else if (']' == exp[iii]) {
						countBracketOut++;
					} else if ('{' == exp[iii]) {
						countBraceIn++;
					} else if ('}' == exp[iii]) {
						countBraceOut++;
					}
					int32_t j;
					// find the element in the list...
					for (j=0; j<constConvertionTableSize; j++) {
						if(		false == constConvertionTable[j].haveBackSlash 
							&&	exp[iii] == constConvertionTable[j].inputValue)
						{
							tmpExp.PushBack(constConvertionTable[j].newValue);
							break;
						}
					}
					// not find : normal element
					if (j==constConvertionTableSize) {
						tmpExp.PushBack( ((int16_t)exp[iii]) & 0x00FF);
					}
				}
			}
			
			// count the number of '(' and ')'
			if (countPTheseIn != countPTheseOut ) {
				TK_ERROR("Error in the number of '('=" << countPTheseIn << " and ')'=" << countPTheseOut << " elements");
				return;
			}
			// count the number of '{' and '}'
			if (countBraceIn != countBraceOut ) {
				TK_ERROR("Error in the number of '{'=" << countBraceIn << " and '}'=" << countBraceOut << " elements");
				return;
			}
			// count the number of '[' and ']'
			if (countBracketIn != countBracketOut ) {
				TK_ERROR("Error in the number of '['=" << countBracketIn << " and ']'=" << countBracketOut << " elements");
				return;
			}
			// need to check if all () [] and {} is well set ...
			if (false == CheckGoodPosition(tmpExp) ) {
				return;
			}
			
			//TK_DEBUG("Main element :"; DisplayElem(tmpExp, 0, tmpExp.Size()); );
			if(		tmpExp.Size()>0
				&&	REGEXP_OPCODE_NO_CHAR == tmpExp[0])
			{
				//TK_DEBUG("=> must not begin with char");
				m_notBeginWithChar = true;
				// remove element
				tmpExp.Erase(0);
			}
			if(		tmpExp.Size()>0
				&&	REGEXP_OPCODE_NO_CHAR == tmpExp[tmpExp.Size()-1])
			{
				//TK_DEBUG("=> must not end with char");
				m_notEndWithChar = true;
				// remove element
				tmpExp.Erase(tmpExp.Size()-1);
			}
			
			if (tmpExp.Size() != m_exprRootNode.Generate(tmpExp) ) {
				return;
			}
			// TODO : optimize node here ...
			
			//Display();
		
			// all OK ... play again 
			m_isOk = true;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		etk::UString	GetRegExp(void)
		{
			return m_expressionRequested;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool GetStatus(void)
		{
			return m_isOk;
		};
		// process the regular expression
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool Process( CLASS_TYPE   &SearchIn,
		              int32_t      startPos,
		              int32_t      endPos,
		              char         escapeChar=0)
		{
			if (false == m_isOk) {
				return false;
			}
			int32_t buflen = SearchIn.Size();
			if (endPos > buflen) {
				endPos = buflen;
			}
			if (startPos > endPos) {
				return false;
			}
			int32_t i = 0;
			for (i=startPos; i<endPos; i++) {
				int32_t findLen=0;
				int32_t maxlen = endPos-i;
				if (true == m_notBeginWithChar) {
					if (i>0) {
						char tmpVal = SearchIn[i-1];
						if(    (    'a' <= tmpVal
						         && 'z' >= tmpVal )
						    || (    'A' <= tmpVal
						         && 'Z' >= tmpVal )
						    || (    '0' <= tmpVal
						         && '9' >= tmpVal )
						    || (    '_' == tmpVal ) )
						{
							// go on the next char ...
							continue;
						}
					}
				}
				if (true == m_exprRootNode.Parse(SearchIn, i, maxlen, findLen)) {
					if(		0!=escapeChar
						&&	i>0)
					{
						if (escapeChar == (char)SearchIn[i-1]) {
							//==> detected escape char ==> try find again ...
							continue;
						}
					}
					// Check end :
					if (true == m_notEndWithChar) {
						if (i+findLen < SearchIn.Size() ) {
							char tmpVal = SearchIn[i+findLen];
							if(    (    'a' <= tmpVal
							         && 'z' >= tmpVal )
							    || (    'A' <= tmpVal
							         && 'Z' >= tmpVal )
							    || (    '0' <= tmpVal
							         && '9' >= tmpVal )
							    || (    '_' == tmpVal ) )
							{
								// go on the next char ...
								continue;
							}
						}
					}
					m_areaFind.start = i;
					m_areaFind.stop  = i + findLen;
					/*
					if (i == 812) {
						std::cout << std::endl;
						for(int32_t k=startPos; k<endPos; k++){
							std::cout << SearchIn[k];
						}
						std::cout << std::endl;
					}
					TK_DEBUG("Find RegExp at position : " << i << " startpos=" << startPos << " endPos=" << endPos << " with size :" << findLen << " type : "; );
					m_exprRootNode.Display(-1);
					TK_DEBUG("---------------------------------------------------");
					*/
					return true;
				}
			}
			return false;
		};
		
		bool ProcessOneElement( CLASS_TYPE   &SearchIn,
		                        int32_t      startPos,
		                        int32_t      endPos,
		                        char         escapeChar=0)
		{
			if (false == m_isOk) {
				return false;
			}
			int32_t buflen = SearchIn.Size();
			if (endPos > buflen) {
				endPos = buflen;
			}
			if (startPos > endPos) {
				return false;
			}
			int32_t findLen=0;
			int32_t maxlen = endPos-startPos;
			if (true == m_notBeginWithChar) {
				if (startPos>0) {
					char tmpVal = SearchIn[startPos-1];
					if(    (    'a' <= tmpVal
					         && 'z' >= tmpVal )
					    || (    'A' <= tmpVal
					         && 'Z' >= tmpVal )
					    || (    '0' <= tmpVal
					         && '9' >= tmpVal )
					    || (    '_' == tmpVal ) )
					{
						// go on the next char ...
						return false;
					}
				}
			}
			if (true == m_exprRootNode.Parse(SearchIn, startPos, maxlen, findLen)) {
				if(		0!=escapeChar
					&&	startPos>0)
				{
					if (escapeChar == (char)SearchIn[startPos-1]) {
						//==> detected escape char ==> try find again ...
						return false;
					}
				}
				// Check end :
				if (true == m_notEndWithChar) {
					if (startPos+findLen < SearchIn.Size() ) {
						char tmpVal = SearchIn[startPos+findLen];
						if(    (    'a' <= tmpVal
						         && 'z' >= tmpVal )
						    || (    'A' <= tmpVal
						         && 'Z' >= tmpVal )
						    || (    '0' <= tmpVal
						         && '9' >= tmpVal )
						    || (    '_' == tmpVal ) )
						{
							// go on the next char ...
							return false;
						}
					}
				}
				m_areaFind.start = startPos;
				m_areaFind.stop  = startPos + findLen;
				return true;
			}
			return false;
		};
		
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		int32_t	Start(void)
		{
			return m_areaFind.start;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		int32_t	Stop(void)
		{
			return m_areaFind.stop;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		void Display(void)
		{
			m_exprRootNode.Display(0);
		};
	// internal parameters
	private:
		etk::UString                     m_expressionRequested;      // TODO : Remove ...
		elementPos_ts                   m_areaFind;                 //!< position around selection
		RegExpNodePThese<CLASS_TYPE>    m_exprRootNode;             //!< The tree where data is set
		bool                            m_isOk;                     //!< Known if we can process with this regExp
		bool                            m_notBeginWithChar;         //!< The regular expression must not have previously a char [a-zA-Z0-9_]
		bool                            m_notEndWithChar;           //!< The regular expression must not have after the end a char [a-zA-Z0-9_]
	// internal access
	private:
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool CheckGoodPosition(etk::VectorType<int16_t> tmpExp, int32_t &pos)
		{
			int16_t curentCode = tmpExp[pos];
			int16_t endCode = REGEXP_OPCODE_PTHESE_OUT;
			char *input = (char*)"(...)";
			if (REGEXP_OPCODE_BRACKET_IN == curentCode) {
				endCode = REGEXP_OPCODE_BRACKET_OUT;
				input = (char*)"[...]";
			} else if (REGEXP_OPCODE_BRACE_IN == curentCode){
				endCode = REGEXP_OPCODE_BRACE_OUT;
				input = (char*)"{x,x}";
			}
			pos++;
			if (pos >= (int32_t)tmpExp.Size()) {
				TK_ERROR("ended with: ( or { or [ ... not permited");
				return false;
			}
			//TK_DEBUG(" ==> Find ELEMENT : ([{");
			// case dependent : 
			if(		REGEXP_OPCODE_BRACKET_IN == curentCode
				||	REGEXP_OPCODE_BRACE_IN == curentCode) {
				while(pos< (int32_t)tmpExp.Size()) {
					//TK_DEBUG("check : " << tmpExp[pos]);
					// if we find the end : 
					if (endCode == tmpExp[pos]) {
						return true;
					} else {
						// otherwise, we check the error in the element ...
						char *find = NULL;
						switch (tmpExp[pos])
						{
							case REGEXP_OPCODE_PTHESE_IN:		find = (char*)"(";			break;
							case REGEXP_OPCODE_BRACKET_IN:		find = (char*)"[";			break;
							case REGEXP_OPCODE_BRACE_IN:		find = (char*)"{";			break;
							case REGEXP_OPCODE_PTHESE_OUT:		find = (char*)")";			break;
							case REGEXP_OPCODE_BRACKET_OUT:		find = (char*)"]";			break;
							case REGEXP_OPCODE_BRACE_OUT:		find = (char*)"}";			break;
							case REGEXP_OPCODE_STAR:			find = (char*)"*";			break;
							case REGEXP_OPCODE_DOT:				find = (char*)".";			break;
							case REGEXP_OPCODE_QUESTION:		find = (char*)"?";			break;
							case REGEXP_OPCODE_PLUS:			find = (char*)"+";			break;
							case REGEXP_OPCODE_PIPE:			find = (char*)"|";			break;
							case REGEXP_OPCODE_START_OF_LINE:	find = (char*)"^";			break;
							case REGEXP_OPCODE_END_OF_LINE:		find = (char*)"$";			break;
							case REGEXP_OPCODE_DIGIT:			find = (char*)"\\d";		break;
							case REGEXP_OPCODE_DIGIT_NOT:		find = (char*)"\\D";		break;
							case REGEXP_OPCODE_LETTER:			find = (char*)"\\l";		break;
							case REGEXP_OPCODE_LETTER_NOT:		find = (char*)"\\L";		break;
							case REGEXP_OPCODE_SPACE:			find = (char*)"\\s";		break;
							case REGEXP_OPCODE_SPACE_NOT:		find = (char*)"\\S";		break;
							case REGEXP_OPCODE_WORD:			find = (char*)"\\w";		break;
							case REGEXP_OPCODE_WORD_NOT:		find = (char*)"\\W";		break;
							case REGEXP_OPCODE_NO_CHAR:			find = (char*)"\\@";		break;
							default:														break;
						}
						if (NULL != find) {
							(void)input;
							TK_ERROR("can not have : '" << find << "' inside " << input << " element");
							return false;
						}
					}
					pos++;
				}
			} else {
				while(pos< (int32_t)tmpExp.Size()) {
					if (endCode == tmpExp[pos]) {
						// find the last element
						return true;
					} else if (	REGEXP_OPCODE_BRACE_OUT	== tmpExp[pos]) {
						TK_ERROR("find } inside a (...) without start {");
						return false;
					} else if (	REGEXP_OPCODE_BRACKET_OUT	== tmpExp[pos]) {
						TK_ERROR("find ] inside a (...) without start [");
						return false;
					} else {
						if(		REGEXP_OPCODE_PTHESE_IN	== tmpExp[pos]
							||	REGEXP_OPCODE_BRACKET_IN	== tmpExp[pos]
							||	REGEXP_OPCODE_BRACE_IN		== tmpExp[pos])
						{
							if (false==CheckGoodPosition(tmpExp, pos) ) {
								return false;
							}
						}
					}
					pos++;
				}
			}
			
			// we did not find the cloder . ...
			if (endCode == REGEXP_OPCODE_BRACKET_OUT) {
				TK_ERROR("Missing ']' at the end");
			}
			if (endCode == REGEXP_OPCODE_BRACE_OUT) {
				TK_ERROR("Missing '}' at the end");
			}
			if (endCode == REGEXP_OPCODE_PTHESE_OUT) {
				TK_ERROR("Missing ')' at the end");
			}
			return false;
		};
		
		/**
		 * @brief
		 * @param[in,out] 
		 * @return
		 */
		bool CheckGoodPosition(etk::VectorType<int16_t> tmpExp)
		{
			int32_t pos = 0;
			while (pos < (int32_t)tmpExp.Size()) {
				//TK_DEBUG("check : " << tmpExp[pos]);
				if(		REGEXP_OPCODE_PTHESE_IN	== tmpExp[pos]
					||	REGEXP_OPCODE_BRACKET_IN	== tmpExp[pos]
					||	REGEXP_OPCODE_BRACE_IN		== tmpExp[pos])
				{
					// attention the i position change inside the finction...
					if (false==CheckGoodPosition(tmpExp, pos) ) {
						TK_ERROR("Error at position : " << pos+1 );
						return false;
					} else {
						//TK_DEBUG(" <== Find ELEMENT : ]})");
					}
				} else if(REGEXP_OPCODE_PTHESE_OUT	== tmpExp[pos]) {
					TK_ERROR("can find ')' with no start : ')'");
					return false;
				} else if(REGEXP_OPCODE_BRACKET_OUT	== tmpExp[pos]) {
					TK_ERROR("can find ']' with no start : '['");
					return false;
				} else if(REGEXP_OPCODE_BRACE_OUT	== tmpExp[pos]) {
					TK_ERROR("can find '}' with no start : '{'");
					return false;
				}
				pos++;
			}
			return true;
		};


};

}; // end of etk namespace

#undef __class__
#define __class__	(NULL)

#endif
