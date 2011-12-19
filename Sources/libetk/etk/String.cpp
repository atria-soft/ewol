/**
 *******************************************************************************
 * @file etk/String.cpp
 * @brief Ewol Tool Kit : normal sting management... (sources)
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

#include <etk/String.h>
#include <etk/Memory.h>


#undef __class__
#define __class__	"etk::String"

etk::CCout& etk::operator <<(etk::CCout &os, const etk::String &obj)
{
	os << (char*)&obj.m_data[0];
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
etk::String::~String(void)
{
	m_data.Clear();
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
etk::String::String(void)
{
	//TK_INFO("new etk::String()");
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
etk::String::String(const char myInput)
{
	m_data.Clear();
	m_data.PushBack(myInput);
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
etk::String::String(const char* inputData, int32_t len)
{
	m_data.Clear();
	m_data.PushBack('\0');
	Set(inputData, len);
}

void etk::String::Set(const char * inputData, int32_t len)
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
		m_data.PushBack((int8_t*)inputData, len);
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
etk::String::String(int inputData)
{
	char tmpVal[256];
	// generate the string : 
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
etk::String::String(unsigned int inputData)
{
	char tmpVal[256];
	// generate the string : 
	sprintf(tmpVal, "%d", inputData);
	// set the internal data : 
	m_data.Clear();
	m_data.PushBack('\0');
	Set(tmpVal);
}

etk::String::String(const etk::String &etkS)
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
const etk::String& etk::String::operator= (const etk::String &etkS )
{
	//TK_INFO("OPERATOR de recopie");
	if( this != &etkS ) // avoid copy to itself
	{
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
const etk::String& etk::String::operator= (const char * inputData)
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
const etk::String& etk::String::operator= (etk::VectorType<int8_t> inputData)
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


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool etk::String::operator== (const etk::String& etkS) const
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
bool etk::String::operator== (const char * inputData) const
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
bool etk::String::operator!= (const etk::String& etkS) const
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
bool etk::String::operator!= (const char * inputData) const
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
const etk::String& etk::String::operator+= (const etk::String &etkS)
{
	if (0 < etkS.Size()) {
		// remove the last '\0'
		m_data.PopBack();
		// copy the data ...
		m_data += etkS.m_data;
		// This previous include the \0 in case of the 2 string are different...
		if( this == &etkS ) {
			// add the removed end string
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
const etk::String& etk::String::operator+= (const char * inputData)
{
	//TK_INFO("        string(arg) : \"" << inputData << "\"");
	//TK_INFO("        string(direct) : \"" << m_data << "\"");
	int32_t len = strlen(inputData);
	
	if (len != 0) {
		// remove the last '\0'
		m_data.PopBack();
		// copy the data ...
		m_data.PushBack((int8_t*)inputData, len+1 );
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
etk::String etk::String::operator+ (const etk::String &etkS)
{
	etk::String temp;
	//TK_INFO("        string(arg) : \"" << etkS.m_data << "\"");
	//TK_INFO("        string(direct) : \"" << m_data << "\"");
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
etk::String etk::String::operator+ (const char * inputData)
{
	etk::String temp;
	//TK_INFO("        string(arg) : \"" << inputData << "\"");
	//TK_INFO("        string(direct) : \"" << m_data << "\"");
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
bool etk::String::IsEmpty(void) const
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
int32_t etk::String::Size(void) const
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
void etk::String::Add(int32_t currentID, const char* inputData)
{
	// get the input lenght
	int32_t len = strlen(inputData);
	if (0 == len) {
		TK_WARNING("no data to add on the current string");
		return;
	} else if (currentID < 0) {
		TK_WARNING("Curent ID(" << currentID << ") < 0   ==> Add at the start");
		currentID = 0;
	} else if (currentID > Size() ) {
		TK_ERROR("Curent ID(" << currentID << ") > maxSize ... (" << Size() << ")  ==> add at the end ...");
		m_data.PushBack((int8_t*)inputData, len);
		return;
	}
	m_data.Insert(currentID, (int8_t*)inputData, len);
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
void etk::String::Remove(int32_t currentID, int32_t len)
{
	if (0 >= len) {
		TK_ERROR("no data to remove on the current string");
		return;
	}
	// TODO : check the size of the data
	m_data.EraseLen(currentID, len);
}


/**
 * @brief Remove all element in the string
 *
 * @param ---
 *
 * @return ---
 *
 */
void etk::String::Clear(void)
{
	m_data.Clear();
	m_data.PushBack('\0');
}



/**
 * @brief find the first accurence after the position indicated
 *
 * @param[in] element Element that might be find in the string
 * @param[in] startPos Stert position to begin the search
 *
 * @return the position of the first occurence or -1 if not find...
 *
 */
int32_t etk::String::FindForward(const char element, int32_t startPos)
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
 * @param[in] element Element that might be find in the string
 * @param[in] startPos Stert position to begin the search
 *
 * @return the position of the first occurence begining by the end or -1 if not find...
 *
 */
int32_t etk::String::FindBack(const char element, int32_t startPos)
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
 * @return the extracted string
 *
 */
etk::String etk::String::Extract(int32_t posStart, int32_t posEnd)
{
	etk::String out;
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
 * @brief Get a basic vector in int8 data with no \0 at the end of the string
 *
 * @param ---
 *
 * @return The desired vector with data
 *
 */
etk::VectorType<int8_t> etk::String::GetVector(void)
{
	etk::VectorType<int8_t> out = m_data;
	out.PopBack();
	return out;
}



/**
 * @brief Unitary test for the string system
 *
 * @param ---
 *
 * @return ---
 *
 */
void etk::TestUntaire_String(void)
{
	TK_WARNING("*********************************************************");
	TK_WARNING("**  Test Unitaire 'etkString'  (START)");
	TK_WARNING("*********************************************************");
	
	int32_t iddd = 0;
	etk::String * monString = new etk::String();
	TK_INFO("phase : " << iddd++ << " : \"" << *monString << "\"");
	delete(monString);
	
	monString = new etk::String("test de direct data");
	TK_INFO("phase : " << iddd++ << " : \"" << *monString << "\"");
	delete(monString);
	
	monString = new etk::String("test de direct data", 7);
	TK_INFO("phase : " << iddd++ << " : \"" << *monString << "\"");
	delete(monString);
	
	int32_t testId = -6789;
	monString = new etk::String(testId);
	TK_INFO("phase : " << iddd++ << " : \"" << *monString << "\"");
	delete(monString);
	
	uint32_t testId2 = 12345;
	monString = new etk::String((unsigned int)testId2);
	TK_INFO("phase : " << iddd++ << " : \"" << *monString << "\"");
	delete(monString);
	
	etk::String plop = "otherString";
	monString = new etk::String(plop);
	TK_INFO("phase : " << iddd++ << " : \"" << *monString << "\"");
	delete(monString);
	
	
	etk::String s1 = "test de base ...";
	s1 += s1;
	TK_INFO("phase : " << iddd++ << " : \"" << s1 << "\"");
	s1 += " plop 2 ";
	TK_INFO("phase : " << iddd++ << " : \"" << s1 << "\"");
	s1 += plop;
	TK_INFO("phase : " << iddd++ << " : \"" << s1 << "\"");
	s1 = plop;
	TK_INFO("phase : " << iddd++ << " : \"" << s1 << "\"");
	s1 = "test direct 44";
	TK_INFO("phase : " << iddd++ << " : \"" << s1 << "\"");
	etk::VectorType<int8_t> vb1;
	vb1.PushBack('v');
	vb1.PushBack('b');
	vb1.PushBack('1');
	s1 = vb1;
	TK_INFO("phase : " << iddd++ << " : \"" << s1 << "\"");
	vb1.Clear();
	vb1.PushBack('v');
	vb1.PushBack('b');
	vb1.PushBack('2');
	vb1.PushBack('\0');
	s1 = vb1;
	TK_INFO("phase : " << iddd++ << " : \"" << s1 << "\"");
	
	if (s1 == "vb2") {
		TK_INFO("phase : " << iddd++ << " : == OK");
	} else {
		TK_ERROR("phase : " << iddd++ << " : == ERROR");
	}
	
	
	if (s1 == "vb3") {
		TK_ERROR("phase : " << iddd++ << " : == ERROR");
	} else {
		TK_INFO("phase : " << iddd++ << " : == OK");
	}
	
	
	if (s1 != "vb3") {
		TK_INFO("phase : " << iddd++ << " : == OK");
	} else {
		TK_ERROR("phase : " << iddd++ << " : == ERROR");
	}
	
	
	if (s1 != "vb2") {
		TK_ERROR("phase : " << iddd++ << " : == ERROR");
	} else {
		TK_INFO("phase : " << iddd++ << " : == OK");
	}
	
	
	etk::String s2 = "vb2";
	etk::String s3 = "vb3";
	
	if (s1 == s2) {
		TK_INFO("phase : " << iddd++ << " : == OK");
	} else {
		TK_ERROR("phase : " << iddd++ << " : == ERROR");
	}
	
	
	if (s1 == s3) {
		TK_ERROR("phase : " << iddd++ << " : == ERROR");
	} else {
		TK_INFO("phase : " << iddd++ << " : == OK");
	}
	
	
	if (s1 != s3) {
		TK_INFO("phase : " << iddd++ << " : == OK");
	} else {
		TK_ERROR("phase : " << iddd++ << " : == ERROR");
	}
	
	
	if (s1 != s2) {
		TK_ERROR("phase : " << iddd++ << " : == ERROR");
	} else {
		TK_INFO("phase : " << iddd++ << " : == OK");
	}
	TK_WARNING("*********************************************************");
	TK_WARNING("**  Test Unitaire 'etkString'  (STOP)");
	TK_WARNING("*********************************************************");
}

