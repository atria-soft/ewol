/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/openGL/VirtualBufferObject.h>



ewol::VirtualBufferObject::VirtualBufferObject(etk::UString& accesMode): 
	ewol::Resource(),
	m_exist(false),
	m_vbo(0)
{
	m_resourceLevel = 3;
	EWOL_DEBUG("OGL : load VBO mode\"" << accesMode << "\"");
	// load data from file "all the time ..."
	
	/*
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	*/
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
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_buffer.Size(), &m_buffer[0], GL_STATIC_DRAW);
		}
	} else {
		// create the Buffer
		if (m_buffer.Size()>0) {
			glGenBuffers(1, &m_vbo);
			m_exist = true;
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_buffer.Size(), &m_buffer[0], GL_STATIC_DRAW);
		}
		// Note we did not create the buffer when no data is needed
	}
}

void ewol::VirtualBufferObject::RemoveContext(void)
{
	if (true==m_exist) {
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
