/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __OPEN_GL__PROGRAM_H__
#define __OPEN_GL__PROGRAM_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/resources/Resource.h>
#include <ewol/renderer/resources/Shader.h>
#include <ewol/renderer/resources/VirtualBufferObject.h>

namespace ewol
{
	/**
	 * @brief In a openGL program we need some data to communicate with them, we register all the name requested by the user in this structure:
	 * @note Register all requested element permit to abstract the fact that some element does not exist and remove control of existance from upper code.
	 *       This is important to note when the Program is reloaded the elements availlable can change.
	 */
	class progAttributeElement
	{
		public :
			etk::UString m_name;        //!< Name of the element
			GLint        m_elementId;   //!< OpenGl Id if this element ==> can not exist ==> @ref m_isLinked
			bool         m_isAttribute; //!< true if it was an attribute element, otherwite it was an uniform
			bool         m_isLinked;    //!< if this element does not exist this is false
	};
	/**
	 * @brief Program is a compilation of some fragment Shader and vertex Shader. This construct automaticly this assiciation
	 * The input file must have the form : "myFile.prog"
	 * The data is simple :
	 * <pre>
	 * # Comment line ... paid attention at the space at the end of lines, they are considered like a part of the file ...
	 * # The folder is automaticly get from the program file basic folder
	 * filename1.vert
	 * filename2.frag
	 * filename3.vert
	 * filename4.frag
	 * </pre>
	 */
	class Program : public ewol::Resource
	{
		private :
			bool                                    m_exist;       //!< the file existed
			GLuint                                  m_program;     //!< openGL id of the current program
			etk::Vector<ewol::Shader*>              m_shaderList;  //!< List of all the shader loaded
			etk::Vector<ewol::progAttributeElement> m_elementList; //!< List of all the attribute requested by the user
			bool                                    m_hasTexture;  //!< A texture has been set to the current shader
		public:
			/**
			 * @brief Contructor of an opengl Program.
			 * @param[in] filename Standard file name format. see @ref etk::FSNode
			 */
			Program(const etk::UString& filename);
			/**
			 * @brief Destructor, remove the current Program.
			 */
			virtual ~Program(void);
			/**
			 * @brief Generic function that get the resouces name of his type.
			 * @return The define char of his name.
			 */
			const char* GetType(void) { return "ewol::Program"; };
			/**
			 * @brief User request an attribute on this program.
			 * @note The attribute is send to the fragment shaders
			 * @param[in] elementName Name of the requested attribute.
			 * @return An abstract ID of the current attribute (this value is all time availlable, even if the program will be reloaded)
			 */
			int32_t GetAttribute(etk::UString elementName);
			/**
			 * @brief Send attribute table to the spefified ID attribure (not send if does not really exist in the OpenGL program).
			 * @param[in] idElem Id of the Attribute that might be sended.
			 * @param[in] nbElement Specifies the number of elements that are to be modified.
			 * @param[in] pointer Pointer on the data that might be sended.
			 * @param[in] jumpBetweenSample Number of byte to jump between 2 vertex (this permit to enterlace informations)
			 */
			void SendAttribute(int32_t idElem,
			                   int32_t nbElement,
			                   void* pointer,
			                   int32_t jumpBetweenSample=0);
			void SendAttributePointer(int32_t idElem,
			                          int32_t nbElement,
			                          ewol::VirtualBufferObject* vbo,
			                          int32_t index,
			                          int32_t jumpBetweenSample=0,
			                          int32_t offset=0);
			/**
			 * @brief User request an Uniform on this program.
			 * @note uniform value is availlable for all the fragment shader in the program (only one value for all)
			 * @param[in] elementName Name of the requested uniform.
			 * @return An abstract ID of the current uniform (this value is all time availlable, even if the program will be reloaded)
			 */
			int32_t GetUniform(etk::UString elementName);
			/**
			 * @brief Send a uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] nbElement Specifies the number of elements that are to be modified. This should be 1 if the targeted uniform variable is not an array, and 1 or more if it is an array.
			 * @param[in] pointer Pointer on the data that might be sended
			 * @param[in] transpose Transpose the matrix (needed all the taime in the normal openGl access (only not done in the openGL-ES2 due to the fact we must done it ourself)
			 */
			void UniformMatrix4fv(int32_t idElem, int32_t nbElement, mat4 pointer, bool transpose=true);
			/**
			 * @brief Send 1 float uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] value1 Value to send at the Uniform
			 */
			void Uniform1f(int32_t idElem, float value1);
			/**
			 * @brief Send 2 float uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] value1 Value to send at the Uniform
			 * @param[in] value2 Value to send at the Uniform
			 */
			void Uniform2f(int32_t idElem, float value1, float value2);
			/**
			 * @brief Send 3 float uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] value1 Value to send at the Uniform
			 * @param[in] value2 Value to send at the Uniform
			 * @param[in] value3 Value to send at the Uniform
			 */
			void Uniform3f(int32_t idElem, float value1, float value2, float value3);
			/**
			 * @brief Send 4 float uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] value1 Value to send at the Uniform
			 * @param[in] value2 Value to send at the Uniform
			 * @param[in] value3 Value to send at the Uniform
			 * @param[in] value4 Value to send at the Uniform
			 */
			void Uniform4f(int32_t idElem, float value1, float value2, float value3, float value4);
			
			/**
			 * @brief Send 1 signed integer uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] value1 Value to send at the Uniform
			 */
			void Uniform1i(int32_t idElem, int32_t value1);
			/**
			 * @brief Send 2 signed integer uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] value1 Value to send at the Uniform
			 * @param[in] value2 Value to send at the Uniform
			 */
			void Uniform2i(int32_t idElem, int32_t value1, int32_t value2);
			/**
			 * @brief Send 3 signed integer uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] value1 Value to send at the Uniform
			 * @param[in] value2 Value to send at the Uniform
			 * @param[in] value3 Value to send at the Uniform
			 */
			void Uniform3i(int32_t idElem, int32_t value1, int32_t value2, int32_t value3);
			/**
			 * @brief Send 4 signed integer uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] value1 Value to send at the Uniform
			 * @param[in] value2 Value to send at the Uniform
			 * @param[in] value3 Value to send at the Uniform
			 * @param[in] value4 Value to send at the Uniform
			 */
			void Uniform4i(int32_t idElem, int32_t value1, int32_t value2, int32_t value3, int32_t value4);
			
			/**
			 * @brief Send "vec1" uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] nbElement Number of element sended
			 * @param[in] value Pointer on the data
			 */
			void Uniform1fv(int32_t idElem, int32_t nbElement, float *value);
			/**
			 * @brief Send "vec2" uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] nbElement Number of element sended
			 * @param[in] value Pointer on the data
			 */
			void Uniform2fv(int32_t idElem, int32_t nbElement, float *value);
			/**
			 * @brief Send "vec3" uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] nbElement Number of element sended
			 * @param[in] value Pointer on the data
			 */
			void Uniform3fv(int32_t idElem, int32_t nbElement, float *value);
			/**
			 * @brief Send "vec4" uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] nbElement Number of element sended
			 * @param[in] value Pointer on the data
			 */
			void Uniform4fv(int32_t idElem, int32_t nbElement, float *value);
			
			/**
			 * @brief Send "ivec1" uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] nbElement Number of element sended
			 * @param[in] value Pointer on the data
			 */
			void Uniform1iv(int32_t idElem, int32_t nbElement, int32_t *value);
			/**
			 * @brief Send "ivec2" uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the Attribute that might be sended.
			 * @param[in] nbElement Number of element sended
			 * @param[in] value Pointer on the data
			 */
			void Uniform2iv(int32_t idElem, int32_t nbElement, int32_t *value);
			/**
			 * @brief Send "ivec3" uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] nbElement Number of element sended
			 * @param[in] value Pointer on the data
			 */
			void Uniform3iv(int32_t idElem, int32_t nbElement, int32_t *value);
			/**
			 * @brief Send "ivec4" uniform element to the spefified ID (not send if does not really exist in the OpenGL program)
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] nbElement Number of element sended
			 * @param[in] value Pointer on the data
			 */
			void Uniform4iv(int32_t idElem, int32_t nbElement, int32_t *value);
			
			/**
			 * @brief Request the processing of this program
			 */
			void Use(void);
			/**
			 * @brief Set the testure Id on the specify uniform element.
			 * @param[in] idElem Id of the uniform that might be sended.
			 * @param[in] textureOpenGlID Real openGL texture ID
			 */
			void SetTexture0(int32_t idElem, GLint textureOpenGlID);
			/**
			 * @brief Stop the processing of this program
			 */
			void UnUse(void);
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

