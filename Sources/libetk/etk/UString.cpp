/**
 *******************************************************************************
 * @file etk/UString.cpp
 * @brief Ewol Tool Kit : normal Unicode string management... (sources)
 * @author Edouard DUPIN
 * @date 14/02/2012
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

#include <etk/UString.h>
#include <etk/Memory.h>
#include <etk/unicode.h>

int32_t strlen(const uniChar_t * data)
{
	if (NULL == data) {
		return 0;
	}
	int32_t iii=0;
	while (*data != 0) {
		data++;
		iii++;
		if (iii > 0x7FFFFFF0) {
			return iii;
		}
	}
	return iii;
}

#undef __class__
#define __class__	"etk::UString"

etk::CCout& etk::operator <<(etk::CCout &os, const etk::UString &obj)
{
	etk::VectorType<char> output_UTF8;
	unicode::convertUnicodeToUtf8(obj.m_data, output_UTF8);
	output_UTF8.PushBack('\0');
	os << &output_UTF8[0];
	return os;
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::UString::~UString(void)
{
	m_data.Clear();
	m_dataUtf8.Clear();
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::UString::UString(void)
{
	//TK_INFO("new etk::UString()");
	m_data.Clear();
	m_data.PushBack('\0');
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::UString::UString(const char* inputData, int32_t len)
{
	m_data.Clear();
	m_data.PushBack('\0');
	Set(inputData, len);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::UString::UString(const uniChar_t* inputData, int32_t len)
{
	m_data.Clear();
	m_data.PushBack('\0');
	Set(inputData, len);
}

void etk::UString::Set(const char * inputData, int32_t len)
{
	if (NULL == inputData) {
		// nothing to add ... 
		return;
	}
	// overwrite the len if needed : 
	if ((-1) == len) {
		len = strlen(inputData);
	}
	// convert the string
	etk::VectorType<char> tmpChar;
	for (int32_t iii=0; iii<len; iii++) {
		tmpChar.PushBack(inputData[iii]);
	}
	// add it ...
	if (len != 0) {
		// remove the last '\0'
		m_data.PopBack();
		// copy the data ...
		unicode::convertUtf8ToUnicode(tmpChar, m_data);
		// add the last '\0'
		m_data.PushBack('\0');
	}
}

void etk::UString::Set(const uniChar_t * inputData, int32_t len)
{
	if (NULL == inputData) {
		// nothing to add ...
		return;
	}
	// overwrite the len if needed :
	if ((-1) == len) {
		len = strlen(inputData);
	}
	
	if (len != 0) {
		// remove the last '\0'
		m_data.PopBack();
		// copy the data ...
		m_data.PushBack(inputData, len);
		// add the last '\0'
		m_data.PushBack('\0');
	}
}
/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::UString::UString(int inputData)
{
	char tmpVal[256];
	// generate the UString : 
	sprintf(tmpVal, "%d", inputData);
	// set the internal data : 
	m_data.Clear();
	m_data.PushBack('\0');
	Set(tmpVal);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::UString::UString(unsigned int inputData)
{
	char tmpVal[256];
	// generate the UString : 
	sprintf(tmpVal, "%d", inputData);
	// set the internal data : 
	m_data.Clear();
	m_data.PushBack('\0');
	Set(tmpVal);
}


etk::UString::UString(const etk::UString &etkS)
{
	//etk_INFO("Constructeur de recopie");
	m_data = etkS.m_data;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const etk::UString& etk::UString::operator= (const etk::UString &etkS )
{
	//TK_INFO("OPERATOR de recopie");
	if( this != &etkS ) {
		m_data = etkS.m_data;
	}
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const etk::UString& etk::UString::operator= (const char * inputData)
{
	m_data.Clear();
	m_data.PushBack('\0');
	if (NULL == inputData) {
		return *this;
	}
	// calculate the size : 
	uint32_t len = strlen(inputData);
	// check the new size ...
	if (len > 0 ) {
		// copy all data : 
		Set(inputData, len);
	}
	return *this;
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const etk::UString& etk::UString::operator= (const uniChar_t * inputData)
{
	m_data.Clear();
	m_data.PushBack('\0');
	if (NULL == inputData) {
		return *this;
	}
	// calculate the size : 
	int32_t len = strlen(inputData);
	// check the new size ...
	if (len > 0 ) {
		// copy all data : 
		Set(inputData, len);
	}
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const etk::UString& etk::UString::operator= (etk::VectorType<char> inputData)
{
	etk::VectorType<uniChar_t> output_Unicode;
	unicode::convertUtf8ToUnicode(inputData, output_Unicode);
	*this = output_Unicode;
	return *this;
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const etk::UString& etk::UString::operator= (etk::VectorType<uniChar_t> inputData)
{
	m_data = inputData;
	if (m_data.Size()>0) {
		if (m_data[m_data.Size()-1] != '\0') {
			m_data.PushBack('\0');
		}
	}
	//TK_DEBUG("m_dataLen="<<m_dataLen << " m_dataLenUTF8="<<m_dataLenUTF8 << " description=" << m_data);
	return *this;
}



bool etk::UString::operator> (const etk::UString& etkS) const
{
	if( this != &etkS ) {
		for (int32_t iii=0; iii < m_data.Size() && iii < etkS.m_data.Size(); iii++) {
			if (m_data[iii] > etkS.m_data[iii] ) {
				return true;
			}
		}
		if (m_data.Size() > etkS.m_data.Size()) {
			return true;
		}
	}
	return false;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool etk::UString::operator== (const etk::UString& etkS) const
{
	if( this != &etkS ) {
		if (etkS.m_data.Size() != m_data.Size()) {
			//TK_DEBUG(" not the same size : " << etkS.m_data.Size() << "!=" << m_data.Size());
			return false;
		}
		for (int32_t iii= 0; iii<m_data.Size(); iii++) {
			//TK_DEBUG("     check : " << etkS.m_data[iii] << "!=" << m_data[iii]);
			if (etkS.m_data[iii]!= m_data[iii]){
				return false;
			}
		}
		return true;
	}
	return true;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool etk::UString::operator== (const char * inputData) const
{
	etk::UString tmpString(inputData);
	return *this == tmpString;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool etk::UString::operator== (const uniChar_t * inputData) const
{
	// calculate the size : 
	int32_t len = strlen(inputData);
	if (len+1 != m_data.Size()) {
		return false;
	}
	for (int32_t iii= 0; iii<m_data.Size(); iii++) {
		if (inputData[iii]!= m_data[iii]){
			return false;
		}
	}
	return true;
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool etk::UString::operator!= (const etk::UString& etkS) const
{
	return !(*this == etkS);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool etk::UString::operator!= (const char * inputData) const
{
	return !(*this == inputData);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const etk::UString& etk::UString::operator+= (const etk::UString &etkS)
{
	if (0 < etkS.Size()) {
		// remove the last '\0'
		m_data.PopBack();
		// copy the data ...
		m_data += etkS.m_data;
		// This previous include the \0 in case of the 2 UString are different...
		if( this == &etkS ) {
			// add the removed end UString
			m_data.PushBack('\0');
		}
	}
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const etk::UString& etk::UString::operator+= (const uniChar_t * inputData)
{
	//TK_INFO("        UString(arg) : \"" << inputData << "\"");
	//TK_INFO("        UString(direct) : \"" << m_data << "\"");
	int32_t len = strlen(inputData);
	
	if (len != 0) {
		// remove the last '\0'
		m_data.PopBack();
		// copy the data ...
		m_data.PushBack(inputData, len+1 );
	}
	return *this;
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
const etk::UString& etk::UString::operator+= (const char * inputData)
{
	etk::UString tmpString(inputData);
	*this += tmpString;
	return *this;
}

const etk::UString& etk::UString::operator+= (const char data)
{
	etk::UString tmpString(data);
	*this += tmpString;
	return *this;
}

const etk::UString& etk::UString::operator+= (const int data)
{
	etk::UString tmpString(data);
	*this += tmpString;
	return *this;
}

const etk::UString& etk::UString::operator+= (const unsigned int data)
{
	etk::UString tmpString(data);
	*this += tmpString;
	return *this;
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::UString etk::UString::operator+ (const etk::UString &etkS)
{
	etk::UString temp;
	//TK_INFO("        UString(arg) : \"" << etkS.m_data << "\"");
	//TK_INFO("        UString(direct) : \"" << m_data << "\"");
	temp += *this;
	temp += etkS;
	return temp;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::UString etk::UString::operator+ (const char * inputData)
{
	etk::UString temp;
	//TK_INFO("        UString(arg) : \"" << inputData << "\"");
	//TK_INFO("        UString(direct) : \"" << m_data << "\"");
	temp += *this;
	temp += inputData;
	return temp;
}
/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::UString etk::UString::operator+ (const uniChar_t * inputData)
{
	etk::UString temp;
	//TK_INFO("        UString(arg) : \"" << inputData << "\"");
	//TK_INFO("        UString(direct) : \"" << m_data << "\"");
	temp += *this;
	temp += inputData;
	return temp;
}





/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool etk::UString::IsEmpty(void) const
{
	if(1 >= m_data.Size() ) {
		return true;
	} else {
		return false;
	}
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
int32_t etk::UString::Size(void) const
{
	if (m_data.Size() == 0) {
		return 0;
	} else {
		return m_data.Size() - 1;
	}
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
void etk::UString::Add(int32_t currentID, const char* inputData)
{
	etk::UString tmpString(inputData);
	Add(currentID, tmpString.pointer() );
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
void etk::UString::Add(int32_t currentID, const uniChar_t* inputData)
{
	// get the input lenght
	int32_t len = strlen(inputData);
	if (0 == len) {
		TK_WARNING("no data to add on the current UString");
		return;
	} else if (currentID < 0) {
		TK_WARNING("Curent ID(" << currentID << ") < 0   ==> Add at the start");
		currentID = 0;
	} else if (currentID > Size() ) {
		TK_ERROR("Curent ID(" << currentID << ") > maxSize ... (" << Size() << ")  ==> add at the end ...");
		m_data.PushBack(inputData, len);
		return;
	}
	m_data.Insert(currentID, inputData, len);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
void etk::UString::Remove(int32_t currentID, int32_t len)
{
	if (0 >= len) {
		TK_ERROR("no data to remove on the current UString");
		return;
	}
	// TODO : check the size of the data
	m_data.EraseLen(currentID, len);
}


/**
 * @brief Remove all element in the UString
 *
 * @param ---
 *
 * @return ---
 *
 */
void etk::UString::Clear(void)
{
	m_data.Clear();
	m_data.PushBack('\0');
}



/**
 * @brief find the first accurence after the position indicated
 *
 * @param[in] element Element that might be find in the UString
 * @param[in] startPos Stert position to begin the search
 *
 * @return the position of the first occurence or -1 if not find...
 *
 */
int32_t etk::UString::FindForward(const char element, int32_t startPos)
{
	return FindForward((uniChar_t)element, startPos);
}
int32_t etk::UString::FindForward(const uniChar_t element, int32_t startPos)
{
	if (startPos < 0) {
		startPos = 0;
	} else if (startPos >= Size() ) {
		return -1;
	}
	for (int32_t iii=startPos; iii< Size(); iii++) {
		if (m_data[iii] == element) {
			return iii;
		}
	}
	return -1;
}


/**
 * @brief find the first accurence before the position indicated.
 *
 * @param[in] element Element that might be find in the UString
 * @param[in] startPos Stert position to begin the search
 *
 * @return the position of the first occurence begining by the end or -1 if not find...
 *
 */
int32_t etk::UString::FindBack(const char element, int32_t startPos)
{
	return FindBack((uniChar_t)element, startPos);
}
int32_t etk::UString::FindBack(const uniChar_t element, int32_t startPos)
{
	if (startPos < 0) {
		return -1;
	} else if (startPos >= Size() ) {
		startPos = Size();
	}
	for (int32_t iii=startPos; iii>=0; iii--) {
		if (m_data[iii] == element) {
			return iii;
		}
	}
	return -1;
}


/**
 * @brief Extract data from the data between two position
 *
 * @param[in] posStart Start position where to extract data
 * @param[in] posEnd End position where to extract data
 *
 * @return the extracted UString
 *
 */
etk::UString etk::UString::Extract(int32_t posStart, int32_t posEnd)
{
	etk::UString out;
	if (posStart < 0) {
		posStart = 0;
	} else if (posStart >= Size() ) {
		return out;
	}
	if (posEnd < 0) {
		return out;
	} else if (posEnd >= Size() ) {
		posEnd = Size();
	}
	out.m_data = m_data.Extract(posStart, posEnd);
	out.m_data.PushBack('\0');
	return out;
}


/**
 * @brief Get a basic vector in int8 data with no \0 at the end of the UString
 *
 * @param ---
 *
 * @return The desired vector with data
 *
 */
etk::VectorType<uniChar_t> etk::UString::GetVector(void)
{
	etk::VectorType<uniChar_t> out = m_data;
	out.PopBack();
	return out;
}


// Start With ...
bool etk::UString::StartWith(const char* data)
{
	etk::UString tmpString(data);
	return StartWith(tmpString);
}

bool etk::UString::StartWith(const uniChar_t* data)
{
	if (NULL == data) {
		return false;
	}
	int32_t len = strlen(data);
	if (len == 0) {
		return false;
	}
	if (len > Size()) {
		return false;
	}
	for (int32_t iii=0; iii<len; iii++) {
		if (data[iii] != m_data[iii]) {
			return false;
		}
	}
	return true;
}

bool etk::UString::StartWith(const etk::UString& data)
{
	if (data.Size() == 0) {
		return false;
	}
	if (data.Size() > Size()) {
		return false;
	}
	for (int32_t iii=0; iii<data.Size(); iii++) {
		if (data[iii] != m_data[iii]) {
			return false;
		}
	}
	return true;
}



bool etk::UString::EndWith(const char* data)
{
	etk::UString tmpString(data);
	return EndWith(tmpString);
}

bool etk::UString::EndWith(const uniChar_t* data)
{
	if (NULL == data) {
		return false;
	}
	int32_t len = strlen(data);
	if (len == 0) {
		return false;
	}
	if (len > Size()) {
		return false;
	}
	for( int32_t iii=Size()-1, jjj=len-1;
	     iii>=0 && jjj>=0;
	     iii--, jjj--) {
		if (data[jjj] != m_data[iii]) {
			return false;
		}
	}
	return true;
}

bool etk::UString::EndWith(const etk::UString& data)
{
	if (data.Size() == 0) {
		return false;
	}
	if (data.Size() > Size()) {
		return false;
	}
	for( int32_t iii=Size()-1, jjj=data.Size()-1;
	     iii>=0 && jjj>=0;
	     iii--, jjj--) {
		if (data[jjj] != m_data[iii]) {
			return false;
		}
	}
	return true;
}


char * etk::UString::Utf8Data(void)
{
	// UTF8 generation :
	m_dataUtf8.Clear();
	unicode::convertUnicodeToUtf8(m_data, m_dataUtf8);
	m_dataUtf8.PushBack('\0');
	
	return &m_dataUtf8[0];
}


