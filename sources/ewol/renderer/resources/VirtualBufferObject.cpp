/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/renderer/resources/VirtualBufferObject.h>



ewol::VirtualBufferObject::VirtualBufferObject(const etk::UString& accesMode): 
	ewol::Resource(),
	m_exist(false),
	m_vbo(0)
{
	m_resourceLevel = 3;
	EWOL_DEBUG("OGL : load VBO mode=\"" << accesMode << "\"");
}


ewol::VirtualBufferObject::~VirtualBufferObject(void)
{
	RemoveContext();
}


void ewol::VirtualBufferObject::RetreiveData(void)
{
	EWOL_ERROR("TODO ... ");
}


void ewol::VirtualBufferObject::UpdateContext(void)
{
	if (true==m_exist) {
		// update the data
		if (m_buffer.Size()<=0) {
			RemoveContext();
		} else {
			EWOL_INFO("VBO: Update [" << m_uniqueId << "]=" << m_buffer.Size() << "*sizeof(float) OGl_Id=" << m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_buffer.Size(), &m_buffer[0], GL_STATIC_DRAW);
			//
		}
	} else {
		// create the Buffer
		if (m_buffer.Size()>0) {
			// Allocate and assign a Vertex Array Object to our handle
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
			EWOL_INFO("VBO: Add [" << m_uniqueId << "]=" << m_buffer.Size() << "*sizeof(float) OGl_Id=" << m_vbo);
			m_exist = true;
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_buffer.Size(), &m_buffer[0], GL_STATIC_DRAW);
			//glEnableVertexAttribArray(0);
			//glDisableVertexAttribArray(0);
		}
		// Note we did not create the buffer when no data is needed
	}
}

void ewol::VirtualBufferObject::RemoveContext(void)
{
	if (true==m_exist) {
		EWOL_INFO("VBO: Remove [" << m_uniqueId << "]=" << m_buffer.Size() << "*sizeof(float) OGl_Id=" << m_vbo);
		glDeleteBuffers(1, &m_vbo);
		m_exist = false;
		m_vbo = 0;
	}
}

void ewol::VirtualBufferObject::RemoveContextToLate(void)
{
	m_exist = false;
	m_vbo = 0;
}

void ewol::VirtualBufferObject::Reload(void)
{
	RemoveContext();
	UpdateContext();
}
