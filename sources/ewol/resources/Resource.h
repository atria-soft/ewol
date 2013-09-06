/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <etk/types.h>
#include <etk/UString.h>
#include <ewol/debug.h>

#define MAX_RESOURCE_LEVEL (5)

namespace ewol
{
	class ResourceManager;
	// class resources is pure virtual
	class Resource {
		/*
		public:
			void* operator new(size_t elmeentSize);
			void  operator delete(void* elementPointer);
		*/
		private:
			static uint32_t m_valBase;
		protected:
			etk::UString m_name;
			uint32_t     m_counter;
			uint32_t     m_uniqueId;
			uint8_t      m_resourceLevel;
		public:
			Resource(void) :
				m_name(""),
				m_counter(1),
				m_resourceLevel(MAX_RESOURCE_LEVEL-1)
			{
				m_uniqueId = m_valBase;
				m_valBase++;
			};
			Resource(const etk::UString& _filename) :
				m_name(_filename),
				m_counter(1),
				m_resourceLevel(MAX_RESOURCE_LEVEL-1)
			{
				m_uniqueId = m_valBase;
				m_valBase++;
			};
			virtual ~Resource(void) { };
			virtual bool HasName(const etk::UString& _fileName)
			{
				EWOL_VERBOSE("G : check : " << _fileName << " ?= " << m_name << " = " << (_fileName==m_name) );
				return _fileName==m_name;
			};
			virtual etk::UString GetName(void) { return m_name; };
			void Increment(void) { m_counter++; };
			bool Decrement(void) { m_counter--; return (m_counter==0)?true:false; };
			int32_t GetCounter(void) { return m_counter; };
			uint32_t GetUID(void) { return m_uniqueId; };
			uint8_t  GetResourceLevel(void) { return m_resourceLevel; };
			virtual const char* GetType(void) { return "unknow"; };
			virtual void UpdateContext(void);
			virtual void RemoveContext(void);
			virtual void RemoveContextToLate(void);
			virtual void Reload(void);
			
			static ewol::ResourceManager& GetManager(void);
	};
};



#endif

