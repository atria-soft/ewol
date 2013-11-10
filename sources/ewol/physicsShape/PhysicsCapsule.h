/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PHYSICS_CAPSULE_H__
#define __EWOL_PHYSICS_CAPSULE_H__


#include <etk/types.h>
#include <ewol/physicsShape/PhysicsShape.h>


namespace ewol {
	class PhysicsCapsule : public ewol::PhysicsShape {
		public:
			PhysicsCapsule(void) {};
			virtual ~PhysicsCapsule(void) {};
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {};
		public:
			virtual enum type getType(void) {
				return ewol::PhysicsShape::capsule;
			};
		private:
			float m_radius;
		public:
			float getRadius(void) const {
				return m_radius;
			};
		private:
			float m_height;
		public:
			float getHeight(void) const {
				return m_height;
			};
		public:
			virtual const PhysicsCapsule* toCapsule(void) const {
				return this;
			};
			virtual PhysicsCapsule* toCapsule(void) {
				return this;
			};
	};
};


#endif
