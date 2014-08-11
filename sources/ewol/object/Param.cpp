/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/object/Param.h>

//////////////////////////////////////////////////////////////////////////////////
// int16_t
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<int16_t>::getType() {
	return "int16_t";
}

template<> std::string ewol::object::Param<int16_t>::getValueSpecific(const int16_t& _valueRequested) {
	return std::to_string(_valueRequested);
}

template<> void ewol::object::Param<int16_t>::setString(const std::string& _newVal) {
	set(std::stoi(_newVal));
}

//////////////////////////////////////////////////////////////////////////////////
// uint32_t 
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<uint32_t>::getType() {
	return "uint32_t";
}

template<> std::string ewol::object::Param<uint32_t>::getValueSpecific(const uint32_t& _valueRequested) {
	return std::to_string(_valueRequested);
}

template<> void ewol::object::Param<uint32_t>::setString(const std::string& _newVal) {
	set(std::stoul(_newVal));
}

//////////////////////////////////////////////////////////////////////////////////
// int32_t
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<int32_t>::getType() {
	return "int32_t";
}

template<> std::string ewol::object::Param<int32_t>::getValueSpecific(const int32_t& _valueRequested) {
	return std::to_string(_valueRequested);
}

template<> void ewol::object::Param<int32_t>::setString(const std::string& _newVal) {
	set(std::stol(_newVal));
}

//////////////////////////////////////////////////////////////////////////////////
// uint64_t
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<uint64_t>::getType() {
	return "uint64_t";
}

template<> std::string ewol::object::Param<uint64_t>::getValueSpecific(const uint64_t& _valueRequested) {
	return std::to_string(_valueRequested);
}

template<> void ewol::object::Param<uint64_t>::setString(const std::string& _newVal) {
	set(std::stoull(_newVal));
}

//////////////////////////////////////////////////////////////////////////////////
// Int64_t
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<int64_t>::getType() {
	return "int64_t";
}

template<> std::string ewol::object::Param<int64_t>::getValueSpecific(const int64_t& _valueRequested) {
	return std::to_string(_valueRequested);
}

template<> void ewol::object::Param<int64_t>::setString(const std::string& _newVal) {
	set(std::stoll(_newVal));
}

//////////////////////////////////////////////////////////////////////////////////
// Float
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<float>::getType() {
	return "float";
}

template<> std::string ewol::object::Param<float>::getValueSpecific(const float& _valueRequested) {
	return std::to_string(_valueRequested);
}

template<> void ewol::object::Param<float>::setString(const std::string& _newVal) {
	set(std::stof(_newVal));
}


//////////////////////////////////////////////////////////////////////////////////
// Double
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<double>::getType() {
	return "double";
}

template<> std::string ewol::object::Param<double>::getValueSpecific(const double& _valueRequested) {
	return std::to_string(_valueRequested);
}

template<> void ewol::object::Param<double>::setString(const std::string& _newVal) {
	set(std::stod(_newVal));
}


//////////////////////////////////////////////////////////////////////////////////
// Boolean
//////////////////////////////////////////////////////////////////////////////////

template<> std::string ewol::object::Param<bool>::getValueSpecific(const bool& _valueRequested) {
	if (_valueRequested == true) {
		return "true";
	}
	return "false";
}

template<> void ewol::object::Param<bool>::setString(const std::string& _newVal) {
	if(    std::tolower(_newVal) == "true"
	    || std::tolower(_newVal) == "enable"
	    || _newVal == "1") {
		set(true);
	} else {
		set(false);
	}
}


//////////////////////////////////////////////////////////////////////////////////
// string
//////////////////////////////////////////////////////////////////////////////////


template<> std::string ewol::object::Param<std::string>::getValueSpecific(const std::string& _valueRequested) {
	return _valueRequested;
}

template<> void ewol::object::Param<std::string>::setString(const std::string& _newVal) {
	set(_newVal);
}


