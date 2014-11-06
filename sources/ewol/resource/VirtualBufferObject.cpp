/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resource/Manager.h>
#include <ewol/resource/VirtualBufferObject.h>

#undef __class__
#define __class__ "resource::VirtualBufferObject"

void ewol::resource::VirtualBufferObject::init(int32_t _number) {
	ewol::Resource::init();
	m_nbVBO = std::avg(1, _number, NB_VBO_MAX);
	for (size_t iii=0; iii<NB_VBO_MAX; iii++) {
		m_vbo[iii]=0;
		m_vboUsed[iii]=false;
		m_vboSizeDataOffset[iii]=-1;
	}
	EWOL_DEBUG("OGL : load VBO count=\"" << _number << "\"");
}

ewol::resource::VirtualBufferObject::VirtualBufferObject() {
	addObjectType("ewol::VirtualBufferObject");
	m_resourceLevel = 3;
}

ewol::resource::VirtualBufferObject::~VirtualBufferObject() {
	removeContext();
}

void ewol::resource::VirtualBufferObject::retreiveData() {
	EWOL_ERROR("TODO ... ");
}

void ewol::resource::VirtualBufferObject::updateContext() {
	if (false == m_exist) {
		// Allocate and assign a Vertex Array Object to our handle
		glGenBuffers(m_nbVBO, m_vbo);
	}
	m_exist = true;
	for (size_t iii=0; iii<m_nbVBO; iii++) {
		EWOL_INFO("VBO    : add [" << getId() << "]=" << m_buffer[iii].size() << "*sizeof(float) OGl_Id=" << m_vbo[iii]);
		if (true == m_vboUsed[iii]) {
			// select the buffer to set data inside it ...
			if (m_buffer[iii].size()>0) {
				glBindBuffer(GL_ARRAY_BUFFER, m_vbo[iii]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_buffer[iii].size(), &((m_buffer[iii])[0]), GL_STATIC_DRAW);
			}
		}
	}
	// un-bind it to permet to have no erreor in the next display ...
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ewol::resource::VirtualBufferObject::removeContext() {
	if (true == m_exist) {
		EWOL_INFO("VBO: remove [" << getId() << "] OGl_Id=" << m_vbo[0]
		                                             << "/" << m_vbo[1]
		                                             << "/" << m_vbo[2]
		                                             << "/" << m_vbo[3]);
		glDeleteBuffers(m_nbVBO, m_vbo);
		m_exist = false;
		for (size_t iii=0; iii<NB_VBO_MAX; iii++) {
			m_vbo[iii] = 0;
		}
	}
}

void ewol::resource::VirtualBufferObject::removeContextToLate() {
	m_exist = false;
	for (size_t iii=0; iii<NB_VBO_MAX; iii++) {
		m_vbo[iii] = 0;
	}
}

void ewol::resource::VirtualBufferObject::reload() {
	removeContext();
	updateContext();
}

void ewol::resource::VirtualBufferObject::flush() {
	// request to the manager to be call at the next update ...
	getManager().update(std::dynamic_pointer_cast<ewol::Resource>(shared_from_this()));
}

void ewol::resource::VirtualBufferObject::pushOnBuffer(int32_t _id, const vec3& _data) {
	if (m_vboSizeDataOffset[_id] == -1) {
		m_vboSizeDataOffset[_id] = 3;
	} else if (m_vboSizeDataOffset[_id] != 3) {
		EWOL_WARNING("set multiType in VBO (Not supported ==> TODO : Maybe update it");
		return;
	}
	m_vboUsed[_id] = true;
	m_buffer[_id].push_back(_data.x());
	m_buffer[_id].push_back(_data.y());
	m_buffer[_id].push_back(_data.z());
}

vec3 ewol::resource::VirtualBufferObject::getOnBufferVec3(int32_t _id, int32_t _elementID) {
	if ((size_t)_elementID*3 > m_buffer[_id].size()) {
		return vec3(0,0,0);
	}
	return vec3(m_buffer[_id][3*_elementID],
	            m_buffer[_id][3*_elementID+1],
	            m_buffer[_id][3*_elementID+2]);
}

int32_t ewol::resource::VirtualBufferObject::bufferSize(int32_t _id) {
	return m_buffer[_id].size()/m_vboSizeDataOffset[_id];
}
int32_t ewol::resource::VirtualBufferObject::getElementSize(int32_t _id) {
	return m_vboSizeDataOffset[_id];
}

void ewol::resource::VirtualBufferObject::pushOnBuffer(int32_t _id, const vec2& _data) {
	if (m_vboSizeDataOffset[_id] == -1) {
		m_vboSizeDataOffset[_id] = 2;
	} else if (m_vboSizeDataOffset[_id] != 2) {
		EWOL_WARNING("set multiType in VBO (Not supported ==> TODO : Maybe update it");
		return;
	}
	m_vboUsed[_id] = true;
	m_buffer[_id].push_back(_data.x());
	m_buffer[_id].push_back(_data.y());
}

vec2 ewol::resource::VirtualBufferObject::getOnBufferVec2(int32_t _id, int32_t _elementID) {
	if ((size_t)_elementID*2 > m_buffer[_id].size()) {
		return vec2(0,0);
	}
	return vec2(m_buffer[_id][2*_elementID],
	            m_buffer[_id][2*_elementID+1]);
}


void ewol::resource::VirtualBufferObject::pushOnBuffer(int32_t _id, const etk::Color<float,4>& _data) {
	if (m_vboSizeDataOffset[_id] == -1) {
		m_vboSizeDataOffset[_id] = 4;
	} else if (m_vboSizeDataOffset[_id] != 4) {
		EWOL_WARNING("set multiType in VBO (Not supported ==> TODO : Maybe update it");
		return;
	}
	m_vboUsed[_id] = true;
	m_buffer[_id].push_back(_data.r());
	m_buffer[_id].push_back(_data.g());
	m_buffer[_id].push_back(_data.b());
	m_buffer[_id].push_back(_data.a());
}

void ewol::resource::VirtualBufferObject::pushOnBuffer(int32_t _id, const etk::Color<float,3>& _data) {
	if (m_vboSizeDataOffset[_id] == -1) {
		m_vboSizeDataOffset[_id] = 3;
	} else if (m_vboSizeDataOffset[_id] != 3) {
		EWOL_WARNING("set multiType in VBO (Not supported ==> TODO : Maybe update it");
		return;
	}
	m_vboUsed[_id] = true;
	m_buffer[_id].push_back(_data.r());
	m_buffer[_id].push_back(_data.g());
	m_buffer[_id].push_back(_data.b());
}

void ewol::resource::VirtualBufferObject::pushOnBuffer(int32_t _id, const etk::Color<float,2>& _data) {
	if (m_vboSizeDataOffset[_id] == -1) {
		m_vboSizeDataOffset[_id] = 2;
	} else if (m_vboSizeDataOffset[_id] != 2) {
		EWOL_WARNING("set multiType in VBO (Not supported ==> TODO : Maybe update it");
		return;
	}
	m_vboUsed[_id] = true;
	m_buffer[_id].push_back(_data.r());
	m_buffer[_id].push_back(_data.g());
}

void ewol::resource::VirtualBufferObject::pushOnBuffer(int32_t _id, const etk::Color<float,1>& _data) {
	if (m_vboSizeDataOffset[_id] == -1) {
		m_vboSizeDataOffset[_id] = 1;
	} else if (m_vboSizeDataOffset[_id] != 1) {
		EWOL_WARNING("set multiType in VBO (Not supported ==> TODO : Maybe update it");
		return;
	}
	m_vboUsed[_id] = true;
	m_buffer[_id].push_back(_data.r());
}

