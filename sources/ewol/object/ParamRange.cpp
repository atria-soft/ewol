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
template<> std::string ewol::object::Param<int16_t>::getType() const {
	return "int16_t";
}
template<> std::string ewol::object::Param<int16_t>::getValueSpecific(const int16_t& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<int16_t>::set(const int16_t& _newVal) {
	if (m_min == m_max) {
		m_value = _newVal;
	} else {
		m_value = std::avg(m_min, _newVal, m_max);
	}
}
template<> void ewol::object::Param<int16_t>::setString(const std::string& _newVal) {
	set(std::stoi(_newVal));
}
template<> std::string ewol::object::Param<int16_t>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " [" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// uint16_t
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<uint16_t>::getType() const {
	return "uint16_t";
}
template<> std::string ewol::object::Param<uint16_t>::getValueSpecific(const uint16_t& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<uint16_t>::set(const uint16_t& _newVal) {
	if (m_min == m_max) {
		m_value = _newVal;
	} else {
		m_value = std::avg(m_min, _newVal, m_max);
	}
}
template<> void ewol::object::Param<uint16_t>::setString(const std::string& _newVal) {
	set(std::stoi(_newVal));
}
template<> std::string ewol::object::Param<uint16_t>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " [" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// uint32_t
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<uint32_t>::getType() const {
	return "uint32_t";
}
template<> std::string ewol::object::Param<uint32_t>::getValueSpecific(const uint32_t& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<uint32_t>::set(const uint32_t& _newVal) {
	if (m_min == m_max) {
		m_value = _newVal;
	} else {
		m_value = std::avg(m_min, _newVal, m_max);
	}
}
template<> void ewol::object::Param<uint32_t>::setString(const std::string& _newVal) {
	set(std::stoul(_newVal));
}
template<> std::string ewol::object::Param<uint32_t>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " [" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// int32_t
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<int32_t>::getType() const {
	return "int32_t";
}
template<> std::string ewol::object::Param<int32_t>::getValueSpecific(const int32_t& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<int32_t>::set(const int32_t& _newVal) {
	if (m_min == m_max) {
		m_value = _newVal;
	} else {
		m_value = std::avg(m_min, _newVal, m_max);
	}
}
template<> void ewol::object::Param<int32_t>::setString(const std::string& _newVal) {
	set(std::stol(_newVal));
}
template<> std::string ewol::object::Param<int32_t>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " [" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// uint64_t
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<uint64_t>::getType() const {
	return "uint64_t";
}
template<> std::string ewol::object::Param<uint64_t>::getValueSpecific(const uint64_t& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<uint64_t>::set(const uint64_t& _newVal) {
	if (m_min == m_max) {
		m_value = _newVal;
	} else {
		m_value = std::avg(m_min, _newVal, m_max);
	}
}
template<> void ewol::object::Param<uint64_t>::setString(const std::string& _newVal) {
	set(std::stoull(_newVal));
}
template<> std::string ewol::object::Param<uint64_t>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " [" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// Int64_t
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<int64_t>::getType() const {
	return "int64_t";
}
template<> std::string ewol::object::Param<int64_t>::getValueSpecific(const int64_t& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<int64_t>::set(const int64_t& _newVal) {
	if (m_min == m_max) {
		m_value = _newVal;
	} else {
		m_value = std::avg(m_min, _newVal, m_max);
	}
}
template<> void ewol::object::Param<int64_t>::setString(const std::string& _newVal) {
	set(std::stoll(_newVal));
}
template<> std::string ewol::object::Param<int64_t>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " [" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] default=" + std::to_string(m_default);
}


//////////////////////////////////////////////////////////////////////////////////
// Float
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<float>::getType() const {
	return "float";
}
template<> std::string ewol::object::Param<float>::getValueSpecific(const float& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<float>::set(const float& _newVal) {
	if (m_min == m_max) {
		m_value = _newVal;
	} else {
		m_value = std::avg(m_min, _newVal, m_max);
	}
}
template<> void ewol::object::Param<float>::setString(const std::string& _newVal) {
	set(std::stof(_newVal));
}
template<> std::string ewol::object::Param<float>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " [" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] default=" + std::to_string(m_default);
}


//////////////////////////////////////////////////////////////////////////////////
// Double
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<double>::getType() const {
	return "double";
}
template<> std::string ewol::object::Param<double>::getValueSpecific(const double& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<double>::set(const double& _newVal) {
	if (m_min == m_max) {
		m_value = _newVal;
	} else {
		m_value = std::avg(m_min, _newVal, m_max);
	}
}
template<> void ewol::object::Param<double>::setString(const std::string& _newVal) {
	set(std::stod(_newVal));
}
template<> std::string ewol::object::Param<double>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " [" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// Boolean
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<bool>::getType() const {
	return "bool";
}
template<> std::string ewol::object::Param<bool>::getValueSpecific(const bool& _valueRequested) const {
	if (_valueRequested == true) {
		return "true";
	}
	return "false";
}
template<> void ewol::object::Param<bool>::set(const bool& _newVal) {
	m_value = _newVal;
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
template<> std::string ewol::object::Param<bool>::getInfo() const {
	return getType() + " default=" + (m_default?"true":"false");
}

//////////////////////////////////////////////////////////////////////////////////
// string
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<std::string>::getType() const {
	return "string";
}
template<> std::string ewol::object::Param<std::string>::getValueSpecific(const std::string& _valueRequested) const {
	return _valueRequested;
}
template<> void ewol::object::Param<std::string>::set(const std::string& _newVal) {
	m_value = _newVal;
}
template<> void ewol::object::Param<std::string>::setString(const std::string& _newVal) {
	set(_newVal);
}
template<> std::string ewol::object::Param<std::string>::getInfo() const {
	return getType() + " default=" + m_default;
}

//////////////////////////////////////////////////////////////////////////////////
// vec2
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<vec2>::getType() const {
	return "vec2";
}
template<> std::string ewol::object::Param<vec2>::getValueSpecific(const vec2& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<vec2>::set(const vec2& _newVal) {
	m_value = _newVal;
	if (m_min != m_max) {
		m_value.setMin(m_min);
		m_value.setMin(m_max);
	}
}
template<> void ewol::object::Param<vec2>::setString(const std::string& _newVal) {
	set(vec2(_newVal));
}
template<> std::string ewol::object::Param<vec2>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " x[" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] "
	                 + " default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// ivec2
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<ivec2>::getType() const {
	return "ivec2";
}
template<> std::string ewol::object::Param<ivec2>::getValueSpecific(const ivec2& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<ivec2>::set(const ivec2& _newVal) {
	m_value = _newVal;
	if (m_min != m_max) {
		m_value.setMin(m_min);
		m_value.setMin(m_max);
	}
}
template<> void ewol::object::Param<ivec2>::setString(const std::string& _newVal) {
	set(ivec2(_newVal));
}
template<> std::string ewol::object::Param<ivec2>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " x[" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] "
	                 + " default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// uivec2
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<uivec2>::getType() const {
	return "uivec2";
}
template<> std::string ewol::object::Param<uivec2>::getValueSpecific(const uivec2& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<uivec2>::set(const uivec2& _newVal) {
	m_value = _newVal;
	if (m_min != m_max) {
		m_value.setMin(m_min);
		m_value.setMin(m_max);
	}
}
template<> void ewol::object::Param<uivec2>::setString(const std::string& _newVal) {
	set(uivec2(_newVal));
}
template<> std::string ewol::object::Param<uivec2>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " x[" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] "
	                 + " default=" + std::to_string(m_default);
}


//////////////////////////////////////////////////////////////////////////////////
// bvec2
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<bvec2>::getType() const {
	return "bvec2";
}
template<> std::string ewol::object::Param<bvec2>::getValueSpecific(const bvec2& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<bvec2>::set(const bvec2& _newVal) {
	m_value = _newVal;
}
template<> void ewol::object::Param<bvec2>::setString(const std::string& _newVal) {
	set(bvec2(_newVal));
}
template<> std::string ewol::object::Param<bvec2>::getInfo() const {
	return getType() + " default=" + std::to_string(m_default);
}
/*
//////////////////////////////////////////////////////////////////////////////////
// vec3
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<vec3>::getType() const {
	return "vec3";
}
template<> std::string ewol::object::Param<vec3>::getValueSpecific(const vec3& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<vec3>::set(const vec3& _newVal) {
	m_value = _newVal;
	if (m_min != m_max) {
		m_value.setMin(m_min);
		m_value.setMin(m_max);
	}
}
template<> void ewol::object::Param<vec3>::setString(const std::string& _newVal) {
	set(vec3(_newVal));
}
template<> std::string ewol::object::Param<vec3>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " x[" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] "
	                 + " default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// ivec3
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<ivec3>::getType() const {
	return "ivec3";
}
template<> std::string ewol::object::Param<ivec3>::getValueSpecific(const ivec3& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<ivec3>::set(const ivec3& _newVal) {
	m_value = _newVal;
	if (m_min != m_max) {
		m_value.setMin(m_min);
		m_value.setMin(m_max);
	}
}
template<> void ewol::object::Param<ivec3>::setString(const std::string& _newVal) {
	set(ivec3(_newVal));
}
template<> std::string ewol::object::Param<ivec3>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " x[" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] "
	                 + " default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// uivec3
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<uivec3>::getType() const {
	return "uivec3";
}
template<> std::string ewol::object::Param<uivec3>::getValueSpecific(const uivec3& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<uivec3>::set(const uivec3& _newVal) {
	m_value = _newVal;
	if (m_min != m_max) {
		m_value.setMin(m_min);
		m_value.setMin(m_max);
	}
}
template<> void ewol::object::Param<uivec3>::setString(const std::string& _newVal) {
	set(uivec3(_newVal));
}
template<> std::string ewol::object::Param<uivec3>::getInfo() const {
	if (m_min == m_max) {
		return getType() + " default=" + std::to_string(m_default);
	}
	return getType() + " x[" + std::to_string(m_min) + ".." + std::to_string(m_max) + "] "
	                 + " default=" + std::to_string(m_default);
}

//////////////////////////////////////////////////////////////////////////////////
// bvec3
//////////////////////////////////////////////////////////////////////////////////
template<> std::string ewol::object::Param<bvec3>::getType() const {
	return "bvec3";
}
template<> std::string ewol::object::Param<bvec3>::getValueSpecific(const bvec3& _valueRequested) const {
	return std::to_string(_valueRequested);
}
template<> void ewol::object::Param<bvec3>::set(const bvec3& _newVal) {
	m_value = _newVal;
}
template<> void ewol::object::Param<bvec3>::setString(const std::string& _newVal) {
	set(bvec3(_newVal));
}
template<> std::string ewol::object::Param<bvec3>::getInfo() const {
	return getType() + " default=" + std::to_string(m_default);
}
*/
