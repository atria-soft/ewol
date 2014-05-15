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
#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/object/Object.h>

#define MAX_RESOURCE_LEVEL (5)

namespace ewol {
	namespace resource {
		class Manager;
	};
	// class resources is pure virtual
	class Resource : public ewol::Object {
		public:
			Resource() :
			  m_counter(1),
			  m_resourceLevel(MAX_RESOURCE_LEVEL-1) {
				addObjectType("ewol::Resource");
				setStatusResource(true);
			};
			Resource(const std::string& _name) :
			  ewol::Object(_name),
			  m_counter(1),
			  m_resourceLevel(MAX_RESOURCE_LEVEL-1) {
				addObjectType("ewol::Resource");
				setStatusResource(true);
			};
			virtual ~Resource() {
				
			};
		private:
			uint32_t m_counter; //!< number of time the element was loaded.
		public:
			void increment() {
				m_counter++;
			};
			bool decrement() {
				m_counter--;
				return (m_counter == 0)?true:false;
			};
			int32_t getCounter() {
				return m_counter;
			};
		protected:
			uint8_t m_resourceLevel; //!< Level of the resource ==> for updata priority [0..5] 0 must be update first.
		public:
			uint8_t  getResourceLevel() {
				return m_resourceLevel;
			};
			virtual void updateContext();
			virtual void removeContext();
			virtual void removeContextToLate();
			virtual void reload();
			static ewol::resource::Manager& getManager();
	};
};



#endif

