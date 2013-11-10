/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PHYSICS_CYLINDER_H__
#define __EWOL_PHYSICS_CYLINDER_H__


#include <etk/types.h>
#include <ewol/physicsShape/PhysicsShape.h>


namespace ewol {
	class PhysicsCylinder : public ewol::PhysicsShape {
		public:
			PhysicsCylinder(void) {};
			virtual ~PhysicsCylinder(void) {};
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {};
		public:
			virtual enum type getType(void) {
				return ewol::PhysicsShape::cylinder;
			};
		private:
			vec3 m_size;
		public:
			vec3 getSize(void) const {
				return m_size;
			};
		public:
			virtual const PhysicsCylinder* toCylinder(void) const {
				return this;
			};
			virtual PhysicsCylinder* toCylinder(void) {
				return this;
			};
	};
};

#endif
