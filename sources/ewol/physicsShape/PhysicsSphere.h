/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PHYSICS_SPHERE_H__
#define __EWOL_PHYSICS_SPHERE_H__


#include <etk/types.h>
#include <ewol/physicsShape/PhysicsShape.h>


namespace ewol {
	class PhysicsSphere : public ewol::PhysicsShape {
		public:
			PhysicsSphere(void) {};
			virtual ~PhysicsSphere(void) {};
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {};
		public:
			virtual enum type getType(void) {
				return ewol::PhysicsShape::sphere;
			};
		private:
			float m_radius; // props["radius"] = obj.scale.x
		public:
			float getRadius(void) const { return m_radius; };
		private:
			virtual const PhysicsSphere* toSphere(void) const {
				return this;
			};
			virtual PhysicsSphere* toSphere(void) {
				return this;
			};
	};
};

#endif
