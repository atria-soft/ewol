/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <vector>
#include <exml/exml.h>
#include <mutex>
#include <memory>

namespace ewol {
	template<class TYPE>
	using SharedPtr = std::shared_ptr<TYPE>;
	template<class TYPE>
	using WeakPtr = std::weak_ptr<TYPE>;
	template<class TYPE>
	using EnableSharedFromThis = std::enable_shared_from_this<TYPE>;
	/*
	template<class TYPE>
	using DynamicPointerCastZZ = std::dynamic_pointer_cast<TYPE>;
	*/
}

