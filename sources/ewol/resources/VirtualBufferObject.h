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
#include <ewol/resources/Resource.h>
#include <ewol/renderer/openGL.h>

#define NB_VBO_MAX   (20)

namespace ewol
{
	/**
	 * @brief VirtualBufferObject is a specific resources for opengl, this load the data directly in the graphic card ad keep these inside
	 */
	class VirtualBufferObject : public ewol::Resource
	{
		private :
			int32_t m_nbVBO;
			bool m_exist;  //!< This data is availlable in the Graphic card
			GLuint m_vbo[NB_VBO_MAX]; //!< openGl ID of this VBO
			bool m_vboUsed[NB_VBO_MAX]; //!< true if the VBO is allocated or used ...
			etk::Vector<float> m_buffer[NB_VBO_MAX]; //!< data that is availlable in the VBO system ...
		protected:
			/**
			 * @brief Constructor of this VBO.
			 * @param[in] accesMode Acces mode : ???
			 */
			VirtualBufferObject(int32_t _number);
			/**
			 * @brief Destructor of this VBO.
			 */
			virtual ~VirtualBufferObject(void);
		public:
			/**
			 * @brief Generic function that get the resouces name of his type.
			 * @return The define char of his name.
			 */
			const char* getType(void) { return "ewol::VirtualBufferObject"; };
			/**
			 * @brief get the real openGL ID.
			 * @return the Ogl id reference of this VBO.
			 */
			GLuint getGL_ID(int32_t id) { return m_vbo[id]; };
			/**
			 * @brief get a reference on hte buffer data for this VBO.
			 * @param[in] id Id of the buffer requested
			 * @return A reference on the data.
			 */
			etk::Vector<float>& getRefBuffer(int32_t id) { m_vboUsed[id] = true; return m_buffer[id]; };
			/**
			 * @brief push data on a buffer with a custum type :
			 * @param[in] id Id of the buffer requested.
			 * @param[in] data Direct data that might be set.
			 */
			void pushOnBuffer(int32_t id, const vec3& data);
			vec3 getOnBufferVec3(int32_t id, int32_t elementID);
			int32_t sizeOnBufferVec3(int32_t id);
			/**
			 * @brief push data on a buffer with a custum type :
			 * @param[in] id Id of the buffer requested.
			 * @param[in] data Direct data that might be set.
			 */
			void pushOnBuffer(int32_t id, const vec2& data);
			vec2 getOnBufferVec2(int32_t id, int32_t elementID);
			int32_t sizeOnBufferVec2(int32_t id);
			/**
			 * @brief get the data from the graphic card.
			 */
			void retreiveData(void);
			/**
			 * @brief Send the data to the graphic card.
			 */
			void flush(void);
			/**
			 * @brief This load/reload the data in the opengl context, needed when removed previously.
			 */
			void updateContext(void);
			/**
			 * @brief remove the data from the opengl context.
			 */
			void removeContext(void);
			/**
			 * @brief Special android spec! It inform us that all context is removed and after notify us...
			 */
			void removeContextToLate(void);
			/**
			 * @brief Relode the shader from the file. used when a request of resouces reload is done.
			 * @note this is really usefull when we tested the new themes or shader developpements.
			 */
			void reload(void);
		public:
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _number Number of VBO needed
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static ewol::VirtualBufferObject* keep(int32_t _number);
			/**
			 * @brief release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void release(ewol::VirtualBufferObject*& _object);
	};
};
#endif

