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


namespace ewol {
	class PhysicsBox;
	class PhysicsCylinder;
	class PhysicsCapsule;
	class PhysicsCone;
	class PhysicsConvexHull;
	class PhysicsSphere;
	
	class PhysicsShape {
		public:
			static PhysicsShape* create(const std::string& _name);
		public:
			enum type {
				unknow,
				box,
				capsule,
				cone,
				convexHull,
				cylinder,
				sphere
			};
		public:
			PhysicsShape(void) : m_quaternion(1,0,0,0), m_origin(0,0,0) {
				
			};
			virtual ~PhysicsShape(void) {
				
			};
		public:
			virtual enum type getType(void) {
				return ewol::PhysicsShape::unknow;
			};
			
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {
				
			};
		private:
			vec4 m_quaternion;
		public:
			const vec4& getQuaternion(void) const {
				return m_quaternion;
			};
		private:
			vec3 m_origin;
		public:
			const vec3& getOrigin(void) const {
				return m_origin;
			};
		public:
			bool isBox(void) {
				return getType() == ewol::PhysicsShape::box;
			};
			bool isCylinder(void) {
				return getType() == ewol::PhysicsShape::cylinder;
			};
			bool isCapsule(void) {
				return getType() == ewol::PhysicsShape::capsule;
			};
			bool isCone(void) {
				return getType() == ewol::PhysicsShape::cone;
			};
			bool isConvexHull(void) {
				return getType() == ewol::PhysicsShape::convexHull;
			};
			bool isSphere(void) {
				return getType() == ewol::PhysicsShape::sphere;
			};
			
			virtual const PhysicsBox* toBox(void) const {
				return NULL;
			};
			virtual PhysicsBox* toBox(void) {
				return NULL;
			};
			
			virtual const PhysicsCylinder* toCylinder(void) const {
				return NULL;
			};
			virtual PhysicsCylinder* toCylinder(void) {
				return NULL;
			};
			
			virtual const PhysicsCapsule* toCapsule(void) const {
				return NULL;
			};
			virtual PhysicsCapsule* toCapsule(void) {
				return NULL;
			};
			
			virtual const PhysicsCone* toCone(void) const {
				return NULL;
			};
			virtual PhysicsCone* toCone(void) {
				return NULL;
			};
			
			virtual const PhysicsConvexHull* toConvexHull(void) const {
				return NULL;
			};
			virtual PhysicsConvexHull* toConvexHull(void) {
				return NULL;
			};
			
			virtual const PhysicsSphere* toSphere(void) const {
				return NULL;
			};
			virtual PhysicsSphere* toSphere(void) {
				return NULL;
			};
	};
};


#endif

