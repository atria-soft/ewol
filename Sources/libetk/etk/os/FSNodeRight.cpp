/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <etk/Types.h>
#include <etk/DebugInternal.h>
#include <etk/os/FSNodeRight.h>

// Right Flags :
enum {
	RIGHT_OTHER_EXECUTE = 1 << 0,
	RIGHT_OTHER_WRITE   = 1 << 1,
	RIGHT_OTHER_READ    = 1 << 2,
	RIGHT_GROUP_EXECUTE = 1 << 3,
	RIGHT_GROUP_WRITE   = 1 << 4,
	RIGHT_GROUP_READ    = 1 << 5,
	RIGHT_USER_EXECUTE  = 1 << 6,
	RIGHT_USER_WRITE    = 1 << 7,
	RIGHT_USER_READ     = 1 << 8,
};

etk::FSNodeRight::FSNodeRight(void) :
	m_rights(0)
{
	
}
etk::FSNodeRight::FSNodeRight(int16_t newRight) :
	m_rights(newRight&0x01FF)
{
	
}
// copy operator :
const etk::FSNodeRight& etk::FSNodeRight::operator= (const etk::FSNodeRight &obj )
{
	m_rights = obj.m_rights;
	return *this;
}

// set right :
const etk::FSNodeRight& etk::FSNodeRight::operator= (const int32_t newVal )
{
	m_rights = newVal&0x01FF;
	return *this;
}

// User
bool etk::FSNodeRight::IsUserReadable(void) const
{
	return ((m_rights&RIGHT_USER_READ)!=0)?true:false;
}

bool etk::FSNodeRight::IsUserWritable(void) const
{
	return ((m_rights&RIGHT_USER_WRITE)!=0)?true:false;
}

bool etk::FSNodeRight::IsUserRunable(void) const
{
	return ((m_rights&RIGHT_USER_EXECUTE)!=0)?true:false;
}

void etk::FSNodeRight::SetUserReadable(bool newStatus)
{
	// reset the flag :
	m_rights &= (0xFFFFFFFF - RIGHT_USER_READ);
	if (true == newStatus) {
		m_rights |= RIGHT_USER_READ;
	}
}

void etk::FSNodeRight::SetUserWritable(bool newStatus)
{
	// reset the flag :
	m_rights &= (0xFFFFFFFF - RIGHT_USER_WRITE);
	if (true == newStatus) {
		m_rights |= RIGHT_USER_WRITE;
	}
}

void etk::FSNodeRight::SetUserRunable(bool newStatus)
{
	// reset the flag :
	m_rights &= (0xFFFFFFFF - RIGHT_USER_EXECUTE);
	if (true == newStatus) {
		m_rights |= RIGHT_USER_EXECUTE;
	}
}

// group
bool etk::FSNodeRight::IsGroupReadable(void) const
{
	return ((m_rights&RIGHT_GROUP_READ)!=0)?true:false;
}

bool etk::FSNodeRight::IsGroupWritable(void) const
{
	return ((m_rights&RIGHT_GROUP_WRITE)!=0)?true:false;
}

bool etk::FSNodeRight::IsGroupRunable(void) const
{
	return ((m_rights&RIGHT_GROUP_EXECUTE)!=0)?true:false;
}

void etk::FSNodeRight::SetGroupReadable(bool newStatus)
{
	// reset the flag :
	m_rights &= (0xFFFFFFFF - RIGHT_GROUP_READ);
	if (true == newStatus) {
		m_rights |= RIGHT_GROUP_READ;
	}
}

void etk::FSNodeRight::SetGroupWritable(bool newStatus)
{
	// reset the flag :
	m_rights &= (0xFFFFFFFF - RIGHT_GROUP_WRITE);
	if (true == newStatus) {
		m_rights |= RIGHT_GROUP_WRITE;
	}
}

void etk::FSNodeRight::SetGroupRunable(bool newStatus)
{
	// reset the flag :
	m_rights &= (0xFFFFFFFF - RIGHT_GROUP_EXECUTE);
	if (true == newStatus) {
		m_rights |= RIGHT_GROUP_EXECUTE;
	}
}

// other
bool etk::FSNodeRight::IsOtherReadable(void) const
{
	return ((m_rights&RIGHT_OTHER_READ)!=0)?true:false;
}

bool etk::FSNodeRight::IsOtherWritable(void) const
{
	return ((m_rights&RIGHT_OTHER_WRITE)!=0)?true:false;
}

bool etk::FSNodeRight::IsOtherRunable(void) const
{
	return ((m_rights&RIGHT_OTHER_EXECUTE)!=0)?true:false;
}

void etk::FSNodeRight::SetOtherReadable(bool newStatus)
{
	// reset the flag :
	m_rights &= (0xFFFFFFFF - RIGHT_OTHER_READ);
	if (true == newStatus) {
		m_rights |= RIGHT_OTHER_READ;
	}
}

void etk::FSNodeRight::SetOtherWritable(bool newStatus)
{
	// reset the flag :
	m_rights &= (0xFFFFFFFF - RIGHT_OTHER_WRITE);
	if (true == newStatus) {
		m_rights |= RIGHT_OTHER_WRITE;
	}
}

void etk::FSNodeRight::SetOtherRunable(bool newStatus)
{
	// reset the flag :
	m_rights &= (0xFFFFFFFF - RIGHT_OTHER_EXECUTE);
	if (true == newStatus) {
		m_rights |= RIGHT_OTHER_EXECUTE;
	}
}
etk::UString etk::FSNodeRight::GetRight(void) const
{
	etk::UString tmp;
	if (true==IsUserReadable()) {
		tmp += "r";
	} else {
		tmp += "-";
	}
	if (true==IsUserWritable()) {
		tmp += "w";
	} else {
		tmp += "-";
	}
	if (true==IsUserRunable()) {
		tmp += "x";
	} else {
		tmp += "-";
	}
	if (true==IsGroupReadable()) {
		tmp += "r";
	} else {
		tmp += "-";
	}
	if (true==IsGroupWritable()) {
		tmp += "w";
	} else {
		tmp += "-";
	}
	if (true==IsGroupRunable()) {
		tmp += "x";
	} else {
		tmp += "-";
	}
	if (true==IsOtherReadable()) {
		tmp += "r";
	} else {
		tmp += "-";
	}
	if (true==IsOtherWritable()) {
		tmp += "w";
	} else {
		tmp += "-";
	}
	if (true==IsOtherRunable()) {
		tmp += "x";
	} else {
		tmp += "-";
	}
	return tmp;
}


etk::CCout& etk::operator <<(etk::CCout &os, const etk::FSNodeRight &obj)
{
	os << obj.GetRight();
	return os;
};

