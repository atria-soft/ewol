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
#include <ewol/renderer/resources/physicsShape/PhysicsShape.h>


namespace ewol
{
	class PhysicsConvexHull : public ewol::PhysicsShape
	{
		public:
			PhysicsConvexHull(void) {};
			virtual ~PhysicsConvexHull(void) {};
		public:
			virtual bool Parse(const char* _line);
			virtual void Display(void) {};
		public:
			virtual type_te GetType(void) { return ewol::PhysicsShape::convexHull; };
		private:
			vec3 m_scale;
		public:
			vec3 GetScale(void) const { return m_scale; };
		private:
			etk::Vector<vec3> m_points;
			/*
				mesh = obj.to_mesh( bpy.context.scene, True, 'PREVIEW' )
				props["points"] = ""
				for v in mesh.vertices:
					props["points"] += "" + out_point3( v.co ) + "|"
				props["points"] = props["points"].rstrip("|")
			*/
		public:
			virtual const PhysicsConvexHull* ToConvexHull(void) const { return this; };
			virtual PhysicsConvexHull* ToConvexHull(void) { return this; };
	};
};


#endif
