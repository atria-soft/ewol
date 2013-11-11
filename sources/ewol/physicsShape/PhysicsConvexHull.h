/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PHYSICS_CONVEX_HULL_H__
#define __EWOL_PHYSICS_CONVEX_HULL_H__


#include <etk/types.h>
#include <ewol/physicsShape/PhysicsShape.h>


namespace ewol {
	class PhysicsConvexHull : public ewol::PhysicsShape {
		public:
			PhysicsConvexHull(void) {};
			virtual ~PhysicsConvexHull(void) {};
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {};
		public:
			virtual enum type getType(void) {
				return ewol::PhysicsShape::convexHull;
			};
		private:
			vec3 m_scale;
		public:
			vec3 getScale(void) const {
				return m_scale;
			};
		private:
			std::vector<vec3> m_points;
		public:
			const std::vector<vec3>& getPointList(void) const {
				return m_points;
			};
		public:
			virtual const PhysicsConvexHull* toConvexHull(void) const {
				return this;
			};
			virtual PhysicsConvexHull* toConvexHull(void) {
				return this;
			};
	};
};


#endif
