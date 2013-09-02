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


namespace ewol
{
	class PhysicsCylinder : public ewol::PhysicsShape
	{
		public:
			PhysicsCylinder(void) {};
			virtual ~PhysicsCylinder(void) {};
		public:
			virtual bool Parse(const char* _line);
			virtual void Display(void) {};
		public:
			virtual type_te GetType(void) { return ewol::PhysicsShape::cylinder; };
		private:
			vec3 m_size;
		public:
			vec3 GetSize(void) const { return m_size; };
		public:
			virtual const PhysicsCylinder* ToCylinder(void) const { return this; };
			virtual PhysicsCylinder* ToCylinder(void) { return this; };
	};
};

#endif
