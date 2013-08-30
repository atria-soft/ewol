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
#include <ewol/renderer/resources/physicsShape/PhysicsShape.h>


namespace ewol
{
	class PhysicsCapsule : public ewol::PhysicsShape
	{
		public:
			PhysicsCapsule(void) {};
			virtual ~PhysicsCapsule(void) {};
		public:
			virtual bool Parse(const char* _line);
			virtual void Display(void) {};
		public:
			virtual type_te GetType(void) { return ewol::PhysicsShape::capsule; };
		private:
			float m_radius;
		public:
			float GetRadius(void) const { return m_radius; };
		private:
			float m_height;
		public:
			float GetHeight(void) const { return m_height; };
		public:
			virtual const PhysicsCapsule* ToCapsule(void) const { return this; };
			virtual PhysicsCapsule* ToCapsule(void) { return this; };
	};
};


#endif
