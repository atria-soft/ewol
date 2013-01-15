/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __OPEN_GL__VIRTUAL_BUFFER_OBJECT_H__
#define __OPEN_GL__VIRTUAL_BUFFER_OBJECT_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/renderer/resources/Resource.h>
#include <ewol/renderer/openGL.h>

namespace ewol
{
	/**
	 * @brief VirtualBufferObject is a specific resources for opengl, this load the data directly in the graphic card ad keep these inside
	 */
	// TODO : use and test it
	class VirtualBufferObject : public ewol::Resource
	{
		private :
			bool               m_exist;  //!< This data is availlable in the Graphic card
			GLuint             m_vbo;    //!< OpenGl ID of this VBO
			GLuint      m_vao;
			etk::Vector<float> m_buffer; //!< data that is availlable in the VBO system ...
		public:
			/**
			 * @brief Constructor of this VBO.
			 * @param[in] accesMode Acces mode : ???
			 */
			VirtualBufferObject(const etk::UString& accesMode);
			/**
			 * @brief Destructor of this VBO.
			 */
			virtual ~VirtualBufferObject(void);
			/**
			 * @brief Generic function that get the resouces name of his type.
			 * @return The define char of his name.
			 */
			const char* GetType(void) { return "ewol::VirtualBufferObject"; };
			/**
			 * @brief Get the real OpenGL ID.
			 * @return the Ogl id reference of this VBO.
			 */
			GLuint GetGL_ID(void) { return m_vbo; };
			/**
			 * @brief Get a reference on hte buffer data for this VBO.
			 * @return A reference on the data.
			 */
			etk::Vector<float>& GetRefBuffer(void) { return m_buffer; };
			/**
			 * @brief Get the data from the graphic card.
			 */
			void RetreiveData(void);
			/**
			 * @brief Send the data to the graphic card.
			 */
			void Flush(void) { UpdateContext(); };
			/**
			 * @brief This load/reload the data in the opengl context, needed when removed previously.
			 */
			void UpdateContext(void);
			/**
			 * @brief Remove the data from the opengl context.
			 */
			void RemoveContext(void);
			/**
			 * @brief Special android spec! It inform us that all context is removed and after notify us...
			 */
			void RemoveContextToLate(void);
			/**
			 * @brief Relode the shader from the file. used when a request of resouces reload is done.
			 * @note this is really usefull when we tested the new themes or shader developpements.
			 */
			void Reload(void);
	};
};
#endif

