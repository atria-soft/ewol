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
#include <ewol/renderer/resources/physicsShape/PhysicsShape.h>


namespace ewol
{
	class PhysicsSphere : public ewol::PhysicsShape
	{
		public:
			PhysicsSphere(void) {};
			virtual ~PhysicsSphere(void) {};
		public:
			virtual bool Parse(const char* _line);
			virtual void Display(void) {};
		public:
			virtual type_te GetType(void) { return ewol::PhysicsShape::sphere; };
		private:
			float m_radius; // props["radius"] = obj.scale.x
	};
};

#endif
