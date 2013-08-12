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
			vec4 GetQuaternion(void) const { return m_quaternion; };
		private:
			vec3 m_origin;
		public:
			vec3 GetOrigin(void) const { return m_origin; };
		public:
			bool IsBox(void) { return GetType()==ewol::PhysicsShape::box; };
			bool IsCylinder(void) { return GetType()==ewol::PhysicsShape::cylinder; };
			bool IsCapsule(void) { return GetType()==ewol::PhysicsShape::capsule; };
			bool IsCone(void) { return GetType()==ewol::PhysicsShape::cone; };
			bool IsConvexHull(void) { return GetType()==ewol::PhysicsShape::convexHull; };
			bool IsSphere(void) { return GetType()==ewol::PhysicsShape::sphere; };
			
			virtual const PhysicsBox* ToBox(void) const { return NULL; };
			virtual PhysicsBox* ToBox(void) { return NULL; };
			
			virtual const PhysicsCylinder* ToCylinder(void) const { return NULL; };
			virtual PhysicsCylinder* ToCylinder(void) { return NULL; };
			
			virtual const PhysicsCapsule* ToCapsule(void) const { return NULL; };
			virtual PhysicsCapsule* ToCapsule(void) { return NULL; };
			
			virtual const PhysicsCone* ToCone(void) const { return NULL; };
			virtual PhysicsCone* ToCone(void) { return NULL; };
			
			virtual const PhysicsConvexHull* ToConvexHull(void) const { return NULL; };
			virtual PhysicsConvexHull* ToConvexHull(void) { return NULL; };
			
			virtual const PhysicsSphere* ToSphere(void) const { return NULL; };
			virtual PhysicsSphere* ToSphere(void) { return NULL; };
			
	};
};


#endif

