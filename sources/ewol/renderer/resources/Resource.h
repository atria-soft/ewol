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
	// class resources is pure virtual
	class Resource {
		/*
		public:
			void* operator new(size_t elmeentSize);
			void  operator delete(void* elementPointer);
		*/
		private:
			static uint32_t valBase;
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
				m_uniqueId = valBase;
				valBase++;
			};
			Resource(const etk::UString& filename) :
				m_name(filename),
				m_counter(1),
				m_resourceLevel(MAX_RESOURCE_LEVEL-1)
			{
				m_uniqueId = valBase;
				valBase++;
			};
			virtual ~Resource(void) { };
			virtual bool HasName(const etk::UString& fileName)
			{
				EWOL_VERBOSE("G : check : " << fileName << " ?= " << m_name << " = " << (fileName==m_name) );
				return fileName==m_name;
			};
			virtual etk::UString GetName(void) { return m_name; };
			void Increment(void) { m_counter++; };
			bool Decrement(void) { m_counter--; return (m_counter==0)?true:false; };
			int32_t GetCounter(void) { return m_counter; };
			virtual const char* GetType(void) { return "unknow"; };
			virtual void UpdateContext(void) { EWOL_DEBUG("Not Set for : [" << m_uniqueId << "]" << m_name << " loaded " << m_counter << " time(s)"); };
			virtual void RemoveContext(void) { EWOL_DEBUG("Not Set for : [" << m_uniqueId << "]" << m_name << " loaded " << m_counter << " time(s)"); };
			virtual void RemoveContextToLate(void) { EWOL_DEBUG("Not Set for : [" << m_uniqueId << "]" << m_name << " loaded " << m_counter << " time(s)"); };
			virtual void Reload(void) { EWOL_DEBUG("Not Set for : [" << m_uniqueId << "]" << m_name << " loaded " << m_counter << " time(s)"); };
			uint32_t GetUID(void) { return m_uniqueId; };
			uint8_t  GetResourceLevel(void) { return m_resourceLevel; };
	};
};



#endif

