/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD 3 clauses (see license file)
 */

#ifndef __EWOL_OBJECT_OWNER_H__
#define __EWOL_OBJECT_OWNER_H__

#include <ewol/debug.h>

namespace ewol {
	namespace object {
		template<typename T>
			class Owner {
				private:
					T* m_pointer;
				public:
					Owner() :
					  m_pointer(nullptr) {
						// nothing to do ...
					}
					Owner(T* _pointer) :
					  m_pointer(_pointer) {
						if (m_pointer == nullptr) {
							return;
						}
						m_pointer->objRefCountIncrement();
					}
					~Owner() {
						reset();
					}
					// copy constructor
					Owner(const Owner& _obj) :
					  m_pointer(nullptr) {
						EWOL_CRITICAL("You can not user copy operator for Owner reference...");
					}
					// Move Constructor
					Owner(Owner&& _obj) :
					  m_pointer(nullptr) {
						// transfert pointer
						m_pointer = _obj.m_pointer;
						_obj.m_pointer = nullptr;
					}
					// shared to private constructor
					template<typename T2, typename = typename
					       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
					Owner(const Shared<T2>& _obj) :
					  m_pointer(nullptr) {
						m_pointer = _obj.get();
						if (m_pointer == nullptr) {
							return;
						}
						m_pointer->objRefCountIncrement();
						#ifdef DEBUG
							m_pointer->incOwnerCount();
						#endif
					}
					template<typename T2, typename = typename
					       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
					Owner& operator=(const Owner<T2>& _obj) noexcept {
						if(this == &_obj) {
							return *this;
						}
						reset();
						m_pointer = _obj.get();
						if (m_pointer != nullptr) {
							m_pointer->objRefCountIncrement();
							#ifdef DEBUG
								m_pointer->incOwnerCount();
							#endif
						}
						return *this;
					}
					template<typename T2, typename = typename
					       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
					Owner& operator=(const Shared<T2>& _obj) noexcept {
						reset();
						m_pointer = _obj.get();
						if (m_pointer != nullptr) {
							m_pointer->objRefCountIncrement();
							#ifdef DEBUG
								m_pointer->incOwnerCount();
							#endif
						}
						return *this;
					}
					
					void reset() {
						if (m_pointer == nullptr) {
							return;
						}
						//etk::log::displayBacktrace();
						// To prevent cyclisme
						T* tmp = m_pointer;
						m_pointer = nullptr;
						tmp->removeObject();
						if (tmp->m_objRefCount <= 0) {
							EWOL_WARNING("Object will be already removed");
						} else if (tmp->m_objRefCount == 1) {
							EWOL_VERBOSE("Remove object (in Owner)");
							delete tmp;
						} else {
							tmp->objRefCountDecrement();
							#ifdef DEBUG
								tmp->decOwnerCount();
							#endif
						}
					}
					void resetShared() {
						if (m_pointer == nullptr) {
							return;
						}
						// To prevent cyclisme
						T* tmp = m_pointer;
						m_pointer = nullptr;
						if (tmp->m_objRefCount <= 0) {
							EWOL_WARNING("Object will be already removed");
						} else if (tmp->m_objRefCount == 1) {
							EWOL_VERBOSE("Remove object (in Owner)");
							delete tmp;
						} else {
							tmp->objRefCountDecrement();
							#ifdef DEBUG
								tmp->decOwnerCount();
							#endif
						}
					}
					T* get() noexcept {
						return m_pointer;
					}
					T* get() const noexcept {
						return m_pointer;
					}
					T& operator*() const noexcept {
						return *m_pointer;
					}
					T* operator->() const noexcept {
						return m_pointer;
					}
					operator ewol::object::Shared<T>() const noexcept {
						return m_pointer;
					}
			};
	};
	// section to compare Owner pointer of an object with an other
	
	//! @not in doc
	template<typename T, typename T2>
	inline bool operator==(const object::Owner<T>& _obj, const object::Owner<T2>& _obj2) noexcept {
		return _obj.get() == _obj2.get();
	}
	//! @not in doc
	template<typename T2>
	inline bool operator==(const object::Owner<T2>& _obj, std::nullptr_t) noexcept {
		return _obj.get() == nullptr;
	}
	//! @not in doc
	template<typename T2>
	inline bool operator==(std::nullptr_t, const object::Owner<T2>& _obj) noexcept {
		return _obj.get() == nullptr;
	}
	//! @not in doc
	template<typename T, typename T2, typename = typename
	       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
	inline bool operator==(const object::Owner<T>& _obj, const T2* _obj2) noexcept {
		return _obj.get() == _obj2;
	}
	//! @not in doc
	template<typename T, typename T2, typename = typename
	       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
	inline bool operator==(const T* _obj, const object::Owner<T2>& _obj2) noexcept {
		return _obj == _obj2.get();
	}
	//! @not in doc
	template<typename T, typename T2>
	inline bool operator==(const object::Owner<T>& _obj, const object::Shared<T2>& _obj2) noexcept {
		return _obj.get() == _obj2.get();
	}
	//! @not in doc
	template<typename T, typename T2>
	inline bool operator==(const object::Shared<T>& _obj, const object::Owner<T2>& _obj2) noexcept {
		return _obj.get() == _obj2.get();
	}
	//! @not in doc
	template<typename T, typename T2, typename = typename
	       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
	inline bool operator!=(const object::Owner<T>& _obj, const T2* _obj2) noexcept {
		return _obj.get() != _obj2;
	}
	//! @not in doc
	template<typename T, typename T2, typename = typename
	       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
	inline bool operator!=(const T* _obj, const object::Owner<T2>& _obj2) noexcept {
		return _obj != _obj2.get();
	}
	
	//! @not in doc
	template<typename T, typename T2>
	inline bool operator!=(const object::Owner<T>& _obj, const object::Owner<T2>& _obj2) noexcept {
		return _obj.get() != _obj2.get();
	}
	//! @not in doc
	template<typename T>
	inline bool operator!=(const object::Owner<T>& _obj, std::nullptr_t) noexcept {
		return _obj.get() != nullptr;
	}
	//! @not in doc
	template<typename T>
	inline bool operator!=(std::nullptr_t, const object::Owner<T>& _obj) noexcept {
		return _obj.get() != nullptr;
	}
	//! @not in doc
	template<typename T, typename T2>
	inline bool operator!=(const object::Owner<T>& _obj, const object::Shared<T2>& _obj2) noexcept {
		return _obj.get() != _obj2.get();
	}
	//! @not in doc
	template<typename T, typename T2>
	inline bool operator!=(const object::Shared<T>& _obj, const object::Owner<T2>& _obj2) noexcept {
		return _obj.get() != _obj2.get();
	}
	/*
	template<typename T, typename T2, typename = typename
	       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
	object::Shared<T> operator=(const object::Owner<T2>& _obj) {
		return _obj.get();
	}
	*/
	
	
	//! @not in doc
	template<typename T>
	inline void swap(object::Owner<T>& _obj, object::Owner<T>& _obj2) noexcept {
		_obj2.swap(_obj);
	}
	
	//! @not in doc
	template<typename T2, typename T>
	inline object::Owner<T2> static_pointer_cast(const object::Owner<T>& _obj) noexcept {
		return object::Owner<T2>(_obj, static_cast<T2*>(_obj.get()));
	}
	
	//! @not in doc
	template<typename T2, typename T>
	inline object::Owner<T2> const_pointer_cast(const object::Owner<T>& _obj) noexcept {
		return object::Owner<T2>(_obj, const_cast<T2*>(_obj.get()));
	}
	
	//! @not in doc
	template<typename T2, typename T>
	inline object::Owner<T2> dynamic_pointer_cast(const object::Owner<T>& _obj) noexcept {
		if (T2* obj = dynamic_cast<T2*>(_obj.get())) {
			return object::Owner<T2>(_obj, obj);
		}
		return object::Owner<T2>();
	}
};
#endif

