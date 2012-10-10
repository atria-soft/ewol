/**
 *******************************************************************************
 * @file ewol/openGL/VirtualBufferObject.h
 * @brief ewol openGl virtualBufferObject system (header)
 * @author Edouard DUPIN
 * @date 10/10/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __OPEN_GL__VIRTUAL_BUFFER_OBJECT_H__
#define __OPEN_GL__VIRTUAL_BUFFER_OBJECT_H__
	#include <etk/Types.h>
	#include <ewol/Debug.h>
	#include <ewol/Resource.h>
	#include <ewol/openGL/openGL.h>
	
	namespace ewol
	{
		class VirtualBufferObject : public ewol::Resource
		{
			private :
				bool               m_exist;
				GLuint             m_vbo;
			public:
				// data that is availlable in the VBO system ...
				etk::Vector<float> m_buffer;
			public:
				VirtualBufferObject(etk::UString& accesMode);
				virtual ~VirtualBufferObject(void);
				const char* GetType(void) { return "ewol::VirtualBufferObject"; };
				GLuint GetGL_ID(void) { return m_vbo; };
				etk::Vector<float>& GetRefByffer(void) { return m_buffer; };
				void RetreiveData(void);
				void UpdateContext(void);
				void RemoveContext(void);
				void RemoveContextToLate(void);
				void Reload(void);
		};
	};
#endif

