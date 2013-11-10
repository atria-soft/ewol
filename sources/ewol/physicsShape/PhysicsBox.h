/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PHYSICS_BOX_H__
#define __EWOL_PHYSICS_BOX_H__


#include <etk/types.h>
#include <ewol/physicsShape/PhysicsShape.h>

namespace ewol {
	class PhysicsBox : public ewol::PhysicsShape {
		public:
			PhysicsBox(void) {};
			virtual ~PhysicsBox(void) {};
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {};
		public:
			virtual enum type getType(void) {
				return ewol::PhysicsShape::box;
			};
		private:
			vec3 m_size; // Box size property in X, Y and Z
		public:
			const vec3& getSize(void) const {
				return m_size;
			};
		public:
			virtual const PhysicsBox* toBox(void) const {
				return this;
			};
			virtual PhysicsBox* toBox(void) {
				return this;
			};
	};
};

#endif
