/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PHYSICS_BOX_H__
#define __EWOL_PHYSICS_BOX_H__


#include <etk/types.h>
#include <ewol/renderer/resources/physicsShape/PhysicsShape.h>

namespace ewol
{
	class PhysicsBox : public ewol::PhysicsShape
	{
		public:
			PhysicsBox(void) {};
			virtual ~PhysicsBox(void) {};
		public:
			virtual bool Parse(const char* _line);
			virtual void Display(void) {};
		public:
			virtual type_te GetType(void) { return ewol::PhysicsShape::box; };
		private:
			vec3 m_size;
	};
};

#endif
