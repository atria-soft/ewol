/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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

