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
#include <etk/math/Vector2D.h>
#include <ewol/debug.h>
#include <ewol/renderer/resources/Resource.h>
#include <ewol/renderer/openGL.h>

#define NB_VBO_MAX   (10)

namespace ewol
{
	/**
	 * @brief VirtualBufferObject is a specific resources for opengl, this load the data directly in the graphic card ad keep these inside
	 */
	class VirtualBufferObject : public ewol::Resource
	{
		private :
			bool               m_exist;  //!< This data is availlable in the Graphic card
			int32_t            m_nbVBO;  //! number of simultaneous VBO
			GLuint             m_vbo[NB_VBO_MAX]; //!< OpenGl ID of this VBO
			etk::Vector<float> m_buffer[NB_VBO_MAX]; //!< data that is availlable in the VBO system ...
		public:
			/**
			 * @brief Constructor of this VBO.
			 * @param[in] accesMode Acces mode : ???
			 */
			VirtualBufferObject(const etk::UString& accesMode, int32_t nbElement=4);
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
			GLuint GetGL_ID(int32_t id) { return m_vbo[id]; };
			/**
			 * @brief Get a reference on hte buffer data for this VBO.
			 * @param[in] id Id of the buffer requested
			 * @return A reference on the data.
			 */
			etk::Vector<float>& GetRefBuffer(int32_t id) { return m_buffer[id]; };
			/**
			 * @brief push data on a buffer with a custum type :
			 * @param[in] id Id of the buffer requested.
			 * @param[in] data Direct data that might be set.
			 */
			void PushOnBuffer(int32_t id, const ivec3& data);
			/**
			 * @brief push data on a buffer with a custum type :
			 * @param[in] id Id of the buffer requested.
			 * @param[in] data Direct data that might be set.
			 */
			void PushOnBuffer(int32_t id, const vec3& data);
			/**
			 * @brief push data on a buffer with a custum type :
			 * @param[in] id Id of the buffer requested.
			 * @param[in] data Direct data that might be set.
			 */
			void PushOnBuffer(int32_t id, const ivec2& data);
			/**
			 * @brief push data on a buffer with a custum type :
			 * @param[in] id Id of the buffer requested.
			 * @param[in] data Direct data that might be set.
			 */
			void PushOnBuffer(int32_t id, const vec2& data);
			/**
			 * @brief Get the data from the graphic card.
			 */
			void RetreiveData(void);
			/**
			 * @brief Send the data to the graphic card.
			 */
			void Flush(void);
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

