/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PHYSICS_CONE_H__
#define __EWOL_PHYSICS_CONE_H__


#include <etk/types.h>
#include <ewol/physicsShape/PhysicsShape.h>


namespace ewol {
	class PhysicsCone : public ewol::PhysicsShape {
		public:
			PhysicsCone(void) {};
			virtual ~PhysicsCone(void) {};
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {};
		public:
			virtual enum type getType(void) {
				return ewol::PhysicsShape::cone;
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
			virtual const PhysicsCone* toCone(void) const {
				return this;
			};
			virtual PhysicsCone* toCone(void) {
				return this;
			};
	};
};


#endif
