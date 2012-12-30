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
	Bounding(game::BoundingModeAABB),
	m_PointStart(-1,-1,-1),
	m_PointStop(1,1,1)
{
	#ifdef DEBUG
		if (false == ewol::resource::Keep(m_displayBounding) ) {
			EWOL_DEBUG("Can not keep ewol::Colored3DObject ...");
		}
		// color never change ...
		draw::Color tmpColorNormal(0xFF000055);
		draw::Colorf tmpColor(tmpColorNormal);
		vec3 tmpPos(0,0,0);
		for(int32_t iii=0; iii<36; iii++) {
			m_color.PushBack(tmpColor);
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


void game::BoundingAABB::Update(game::MeshObject& object, mat4& transformMatrix)
{
	for (int32_t iii=0; iii<object.m_vertices.Size(); iii++) {
		vec3 point = transformMatrix * object.m_vertices[iii];
		if (0 == iii) {
			m_PointStart = point;
			m_PointStop = point;
		} else {
			if (m_PointStart.x > point.x) {
				m_PointStart.x = point.x;
			} else if (m_PointStop.x < point.x) {
				m_PointStop.x = point.x;
			}
			
			if (m_PointStart.y > point.y) {
				m_PointStart.y = point.y;
			} else if (m_PointStop.y < point.y) {
				m_PointStop.y = point.y;
			}
			
			if (m_PointStart.z > point.z) {
				m_PointStart.z = point.z;
			} else if (m_PointStop.z < point.z) {
				m_PointStop.z = point.z;
			}
		}
	}
	#ifdef DEBUG
		vec3 tmpBB(0.001,0.001,0.001);
		vec3 tmpStart = m_PointStart - tmpBB;
		vec3 tmpStop = m_PointStop + tmpBB;
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
		if (0 != m_vertices.Size()) {
			if (NULL != m_displayBounding) {
				m_displayBounding->Draw(m_vertices, m_color, false);
			}
		} else {
			EWOL_DEBUG("Bounding size is not correct...");
		}
	#endif
}



