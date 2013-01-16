/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/renderer/ResourceManager.h>
#include <ewol/renderer/resources/VirtualBufferObject.h>

ewol::VirtualBufferObject::VirtualBufferObject(const etk::UString& accesMode, int32_t nbElement): 
	ewol::Resource(),
	m_exist(false),
	m_nbVBO(nbElement)
{
	for (int32_t iii=0; iii<NB_VBO_MAX; iii++) {
		m_vbo[iii]=0;
	}
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
	if (false==m_exist) {
		// Allocate and assign a Vertex Array Object to our handle
		glGenBuffers(m_nbVBO, m_vbo);
	}
	m_exist = true;
	for (int32_t iii=0; iii<m_nbVBO; iii++) {
		EWOL_INFO("VBO    : Add [" << m_uniqueId << "]=" << m_buffer[iii].Size() << "*sizeof(float) OGl_Id=" << m_vbo[iii]);
		// select the buffer to set data inside it ...
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[iii]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_buffer[iii].Size(), &((m_buffer[iii])[0]), GL_STATIC_DRAW);
	}
	// un-bind it to permet to have no erreor in the next display ...
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ewol::VirtualBufferObject::RemoveContext(void)
{
	if (true==m_exist) {
		EWOL_INFO("VBO: Remove [" << m_uniqueId << "] OGl_Id=" << m_vbo[0]
			                                                   << "/" << m_vbo[1]
			                                                   << "/" << m_vbo[2]
			                                                   << "/" << m_vbo[3]
			                                                   << "/" << m_vbo[4]
			                                                   << "/" << m_vbo[5]
			                                                   << "/" << m_vbo[6]
			                                                   << "/" << m_vbo[7]
			                                                   << "/" << m_vbo[8]
			                                                   << "/" << m_vbo[9]);
		glDeleteBuffers(m_nbVBO, m_vbo);
		m_exist = false;
		for (int32_t iii=0; iii<NB_VBO_MAX; iii++) {
			m_vbo[iii] = 0;
		}
	}
}

void ewol::VirtualBufferObject::RemoveContextToLate(void)
{
	m_exist = false;
	for (int32_t iii=0; iii<NB_VBO_MAX; iii++) {
		m_vbo[iii] = 0;
	}
}

void ewol::VirtualBufferObject::Reload(void)
{
	RemoveContext();
	UpdateContext();
}


void ewol::VirtualBufferObject::Flush(void)
{
	// request to the manager to be call at the next update ...
	ewol::resource::Update(this);
}

