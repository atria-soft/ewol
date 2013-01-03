/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/game/BoundingAABB.h>
#include <etk/math/Matrix4.h>
#include <ewol/renderer/ResourceManager.h>


game::BoundingAABB::BoundingAABB(void) :
	m_hasContact(false),
	m_center(0,0,0),
	m_size(1,1,1),
	m_oldUserPosition(0,0,0)
{
	#ifdef DEBUG
		if (false == ewol::resource::Keep(m_displayBounding) ) {
			EWOL_DEBUG("Can not keep ewol::Colored3DObject ...");
		}
		vec3 tmpPos(0,0,0);
		for(int32_t iii=0; iii<36; iii++) {
			m_vertices.PushBack(tmpPos);
		}
	#endif
}

game::BoundingAABB::~BoundingAABB(void)
{
	#ifdef DEBUG
		ewol::resource::Release(m_displayBounding);
	#endif
}


void game::BoundingAABB::Update(game::MeshObject& object, mat4& rotation, vec3& position, vec3& scale)
{
	vec3 pointStart;
	vec3 pointStop;
	mat4 transformMatrix =   etk::matTranslate(position)
	                       * etk::matScale(scale)
	                       * rotation;
	for (int32_t iii=0; iii<object.m_vertices.Size(); iii++) {
		vec3 point = transformMatrix * object.m_vertices[iii];
		if (0 == iii) {
			pointStart = point;
			pointStop = point;
		} else {
			if (pointStart.x > point.x) {
				pointStart.x = point.x;
			} else if (pointStop.x < point.x) {
				pointStop.x = point.x;
			}
			
			if (pointStart.y > point.y) {
				pointStart.y = point.y;
			} else if (pointStop.y < point.y) {
				pointStop.y = point.y;
			}
			
			if (pointStart.z > point.z) {
				pointStart.z = point.z;
			} else if (pointStop.z < point.z) {
				pointStop.z = point.z;
			}
		}
	}
	m_size = (pointStop - pointStart) / 2;
	m_center = pointStop - m_size;
	m_oldUserPosition = position;
	
	#ifdef DEBUG
		vec3 tmpBB(0.001,0.001,0.001);
		vec3 tmpStart = pointStart - tmpBB;
		vec3 tmpStop = pointStop + tmpBB;
		// (start) X / Y
		vec3 tmpPos = tmpStart;
		m_vertices[0] = tmpStart;
		tmpPos.x = tmpStop.x;
		m_vertices[1] = tmpPos;
		tmpPos.y = tmpStop.y;
		m_vertices[2] = tmpPos;
		
		m_vertices[3] = tmpStart;
		m_vertices[4] = tmpPos;
		tmpPos.x = tmpStart.x;
		m_vertices[5] = tmpPos;
		
		// (start) Y / Z
		tmpPos = tmpStart;
		m_vertices[6] = tmpStart;
		tmpPos.z = tmpStop.z;
		m_vertices[7] = tmpPos;
		tmpPos.y = tmpStop.y;
		m_vertices[8] = tmpPos;
		
		m_vertices[9] = tmpStart;
		m_vertices[10] = tmpPos;
		tmpPos.z = tmpStart.z;
		m_vertices[11] = tmpPos;
		
		// (start) X / Z
		tmpPos = tmpStart;
		m_vertices[12] = tmpStart;
		tmpPos.x = tmpStop.x;
		m_vertices[13] = tmpPos;
		tmpPos.z = tmpStop.z;
		m_vertices[14] = tmpPos;
		
		m_vertices[15] = tmpStart;
		m_vertices[16] = tmpPos;
		tmpPos.x = tmpStart.x;
		m_vertices[17] = tmpPos;
		
		
		// (stop) X / Y
		tmpPos = tmpStop;
		m_vertices[18] = tmpStop;
		tmpPos.x = tmpStart.x;
		m_vertices[19] = tmpPos;
		tmpPos.y = tmpStart.y;
		m_vertices[20] = tmpPos;
		
		m_vertices[21] = tmpStop;
		m_vertices[22] = tmpPos;
		tmpPos.x = tmpStop.x;
		m_vertices[23] = tmpPos;
		
		// (stop) Y / Z
		tmpPos = tmpStop;
		m_vertices[24] = tmpStop;
		tmpPos.z = tmpStart.z;
		m_vertices[25] = tmpPos;
		tmpPos.y = tmpStart.y;
		m_vertices[26] = tmpPos;
		
		m_vertices[27] = tmpStop;
		m_vertices[28] = tmpPos;
		tmpPos.z = tmpStop.z;
		m_vertices[29] = tmpPos;
		
		// (stop) X / Z
		tmpPos = tmpStop;
		m_vertices[30] = tmpStop;
		tmpPos.x = tmpStart.x;
		m_vertices[31] = tmpPos;
		tmpPos.z = tmpStart.z;
		m_vertices[32] = tmpPos;
		
		m_vertices[33] = tmpStop;
		m_vertices[34] = tmpPos;
		tmpPos.x = tmpStop.x;
		m_vertices[35] = tmpPos;
	#endif
}


void game::BoundingAABB::Draw(void)
{
	#ifdef DEBUG
		draw::Colorf color(0.0, 1.0, 0.0, 0.2);
		if (true == m_hasContact) {
			color = draw::Colorf(1.0, 0.0, 0.0, 0.2);
		}
		if (0 != m_vertices.Size()) {
			if (NULL != m_displayBounding) {
				m_displayBounding->Draw(m_vertices, color, false);
			}
		} else {
			EWOL_DEBUG("Bounding size is not correct...");
		}
	#endif
}


bool game::BoundingAABB::HasContact(game::BoundingAABB& obj)
{
	if(    (abs(m_center.x-obj.m_center.x) > m_size.x+obj.m_size.x)
	    || (abs(m_center.y-obj.m_center.y) > m_size.y+obj.m_size.y)
	    || (abs(m_center.z-obj.m_center.z) > m_size.z+obj.m_size.z) ) {
		return false;
	}
	return true;
}


