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
#include <ewol/renderer/EObject.h>

#define MAX_RESOURCE_LEVEL (5)

namespace ewol {
	class ResourceManager;
	// class resources is pure virtual
	class Resource : public ewol::EObject {
		public:
			Resource(void) :
			  m_counter(1),
			  m_resourceLevel(MAX_RESOURCE_LEVEL-1) {
				addObjectType("ewol::Resource");
				setStatusResource(true);
			};
			Resource(const std::string& _name) :
			  ewol::EObject(_name),
			  m_counter(1),
			  m_resourceLevel(MAX_RESOURCE_LEVEL-1) {
				addObjectType("ewol::Resource");
				setStatusResource(true);
			};
			virtual ~Resource(void) {
				
			};
		private:
			uint32_t m_counter; //!< number of time the element was loaded.
		public:
			void increment(void) {
				m_counter++;
			};
			bool decrement(void) {
				m_counter--;
				return (m_counter == 0)?true:false;
			};
			int32_t getCounter(void) {
				return m_counter;
			};
		protected:
			uint8_t m_resourceLevel; //!< Level of the resource ==> for updata priority [0..5] 0 must be update first.
		public:
			uint8_t  getResourceLevel(void) {
				return m_resourceLevel;
			};
			virtual void updateContext(void);
			virtual void removeContext(void);
			virtual void removeContextToLate(void);
			virtual void reload(void);
			static ewol::ResourceManager& getManager(void);
	};
};



#endif

