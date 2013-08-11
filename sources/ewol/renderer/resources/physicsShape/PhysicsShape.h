/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PHYSICS_SHAPE_H__
#define __EWOL_PHYSICS_SHAPE_H__


#include <etk/types.h>
#include <etk/UString.h>
#include <etk/math/Vector4D.h>
#include <etk/math/Vector3D.h>


namespace ewol
{
	class PhysicsBox;
	class PhysicsCylinder;
	class PhysicsCapsule;
	class PhysicsCone;
	class PhysicsConvexHull;
	class PhysicsSphere;
	
	class PhysicsShape
	{
		public:
			static PhysicsShape* Create(const etk::UString& _name);
		public:
			typedef enum {
				unknow,
				box,
				capsule,
				cone,
				convexHull,
				cylinder,
				sphere
			} type_te;
		public:
			PhysicsShape(void) {};
			virtual ~PhysicsShape(void) {};
		public:
			virtual type_te GetType(void) { return ewol::PhysicsShape::unknow; };
			
		public:
			virtual bool Parse(const char* _line);
			virtual void Display(void) {};
		private:
			vec4 m_quaternion;
		public:
			
		private:
			vec3 m_origin;
		public:
			
		public:
			//virtual const PhysicsBox* ToBox(void) { return ;
	};
};


#endif

