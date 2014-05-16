/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD 3 clauses (see license file)
 */

#ifndef __EWOL_OBJECT_SHARED_H__
#define __EWOL_OBJECT_SHARED_H__

#include <ewol/debug.h>

namespace ewol {
	namespace object {
		template<typename T>
			class Shared {
				private:
					T* m_pointer;
				public:
					Shared() :
					  m_pointer(nullptr) {
						// nothing to do ...
					}
					Shared(T* _pointer) :
					  m_pointer(_pointer) {
						if (m_pointer == nullptr) {
							return;
						}
						m_pointer->objRefCountIncrement();
					}
					~Shared() {
						reset();
					}
					// shared to private constructor
					Shared(const Owner<T>& _obj) :
					  m_pointer(nullptr) {
						m_pointer = _obj.get();
						if (m_pointer == nullptr) {
							return;
						}
						m_pointer->objRefCountIncrement();
					}
					// copy constructor
					Shared(const Shared& _obj) :
					  m_pointer(nullptr) {
						m_pointer = _obj.get();
						if (m_pointer == nullptr) {
							return;
						}
						m_pointer->objRefCountIncrement();
					}
					// Move Constructor
					Shared(Shared&& _obj) :
					  m_pointer(nullptr) {
						// transfert pointer
						m_pointer = _obj.m_pointer;
						_obj.m_pointer = nullptr;
					}
					bool hasOwner() {
						if (m_pointer == nullptr) {
							return false;
						}
						return m_pointer->getRefOwner();
					}
					
					Shared& operator=(const Shared<T>& _obj) noexcept {
						if(this == &_obj) {
							return *this;
						}
						reset();
						m_pointer = _obj.get();
						if (m_pointer != nullptr) {
							m_pointer->objRefCountIncrement();
						}
						return *this;
					}
					
					void reset() {
						if (m_pointer == nullptr) {
							return;
						}
						if (m_pointer->m_objRefCount <= 0) {
							EWOL_ERROR("Object is already removed");
						} else if (m_pointer->m_objRefCount == 1) {
							EWOL_ERROR("Remove object (in shared)");
							delete m_pointer;
						} else {
							m_pointer->objRefCountDecrement();
						}
						m_pointer = nullptr;
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
			};
			template<typename T> Shared<T> makeShared(T* _pointer) {
				return Shared<T>(_pointer);
			}
	};
	// section to compare shared pointer of an object with an other
	
	//! @not in doc
	template<typename T, typename T2>
	inline bool operator==(const object::Shared<T>& _obj, const object::Shared<T2>& _obj2) noexcept {
		return _obj.get() == _obj2.get();
	}
	//! @not in doc
	template<typename T2>
	inline bool operator==(const object::Shared<T2>& _obj, std::nullptr_t) noexcept {
		return _obj.get() == NULL;
	}
	//! @not in doc
	template<typename T2>
	inline bool operator==(std::nullptr_t, const object::Shared<T2>& _obj) noexcept {
		return _obj.get() == NULL;
	}
	//! @not in doc
	template<typename T, typename T2, typename = typename
	       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
	inline bool operator==(const object::Shared<T>& _obj, const T2* _obj2) noexcept {
		return _obj.get() == _obj2;
	}
	//! @not in doc
	template<typename T, typename T2, typename = typename
	       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
	inline bool operator==(const T* _obj, const object::Shared<T2>& _obj2) noexcept {
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
	template<typename T, typename T2>
	inline bool operator!=(const object::Shared<T>& _obj, const object::Shared<T2>& _obj2) noexcept {
		return _obj.get() != _obj2.get();
	}
	//! @not in doc
	template<typename T>
	inline bool operator!=(const object::Shared<T>& _obj, std::nullptr_t) noexcept {
		return _obj.get() != NULL;
	}
	//! @not in doc
	template<typename T>
	inline bool operator!=(std::nullptr_t, const object::Shared<T>& _obj) noexcept {
		return _obj.get() != NULL;
	}
	//! @not in doc
	template<typename T, typename T2, typename = typename
	       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
	inline bool operator!=(const object::Shared<T>& _obj, const T2* _obj2) noexcept {
		return _obj.get() != _obj2;
	}
	//! @not in doc
	template<typename T, typename T2, typename = typename
	       std::enable_if<std::is_convertible<T*, T2*>::value>::type>
	inline bool operator!=(const T* _obj, const object::Shared<T2>& _obj2) noexcept {
		return _obj != _obj2.get();
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
	
	//! @not in doc
	template<typename T>
	inline void swap(object::Shared<T>& _obj, object::Shared<T>& _obj2) noexcept {
		_obj2.swap(_obj);
	}
	
	//! @not in doc
	template<typename T2, typename T>
	inline object::Shared<T2> static_pointer_cast(const object::Shared<T>& _obj) noexcept {
		return object::Shared<T2>(_obj, static_cast<T2*>(_obj.get()));
	}
	
	//! @not in doc
	template<typename T2, typename T>
	inline object::Shared<T2> const_pointer_cast(const object::Shared<T>& _obj) noexcept {
		return object::Shared<T2>(_obj, const_cast<T2*>(_obj.get()));
	}
	
	//! @not in doc
	template<typename T2, typename T>
	inline object::Shared<T2> dynamic_pointer_cast(const object::Shared<T>& _obj) noexcept {
		if (T2* obj = dynamic_cast<T2*>(_obj.get())) {
			return object::Shared<T2>(_obj, obj);
		}
		return object::Shared<T2>();
	}
};

#endif