/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/debug.hpp>
#include <ewol/compositing/Compositing.hpp>
#include <gale/resource/Program.hpp>
#include <ewol/resource/TextureFile.hpp>
#include <ewol/resource/ImageDF.hpp>

namespace ewol {
	namespace compositing {
		class Image : public ewol::Compositing {
			public:
				static const int32_t sizeAuto;
			private:
				etk::Uri m_filename;
				ivec2 m_requestSize;
				vec3 m_position; //!< The current position to draw
				vec3 m_clippingPosStart; //!< Clipping start position
				vec3 m_clippingPosStop; //!< Clipping stop position
				bool m_clippingEnable; //!< true if the clipping must be activated
			private:
				etk::Color<float,4> m_color; //!< The text foreground color
				float m_angle; //!< Angle to set at the axes
			private:
				ememory::SharedPtr<gale::resource::Program> m_GLprogram; //!< pointer on the opengl display program
				int32_t m_GLPosition; //!< openGL id on the element (vertex buffer)
				int32_t m_GLMatrix; //!< openGL id on the element (transformation matrix)
				int32_t m_GLColor; //!< openGL id on the element (color buffer)
				int32_t m_GLtexture; //!< openGL id on the element (Texture position)
				int32_t m_GLtexID; //!< openGL id on the element (texture ID)
			private:
				bool m_distanceFieldMode; //!< select distance field mode
				ememory::SharedPtr<ewol::resource::TextureFile> m_resource; //!< texture resources
				ememory::SharedPtr<ewol::resource::Texture> m_resourceImage; //!< texture resources
				ememory::SharedPtr<ewol::resource::ImageDF> m_resourceDF; //!< texture resources
				static const int32_t m_vboIdCoord;
				static const int32_t m_vboIdCoordTex;
				static const int32_t m_vboIdColor;
				ememory::SharedPtr<gale::resource::VirtualBufferObject> m_VBO;
			private:
				/**
				 * @brief load the openGL program and get all the ID needed
				 */
				void loadProgram();
			public:
				/**
				 * @brief generic constructor
				 * @param[in] _uri URI of the file that might be loaded
				 * @param[in] _df enable distance field mode
				 * @param[in] _size for the image when Verctorial image loading is requested
				 */
				Image(const etk::Uri& _uri="",
				      bool _df=false,
				      int32_t _size=ewol::compositing::Image::sizeAuto);
				/**
				 * @brief generic destructor
				 */
				virtual ~Image();
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 * @param[in] _disableDepthTest disable the Depth test for display
				 */
				void draw(bool _disableDepthTest=true);
				/**
				 * @brief clear alll tre registered element in the current element
				 */
				void clear();
				/**
				 * @brief get the current display position (sometime needed in the gui control)
				 * @return the current position.
				 */
				const vec3& getPos() {
					return m_position;
				};
				/**
				 * @brief set position for the next text writen
				 * @param[in] _pos Position of the text (in 3D)
				 */
				void setPos(const vec3& _pos) {
					m_position = _pos;
				};
				inline void setPos(const vec2& _pos) {
					setPos(vec3(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief set relative position for the next text writen
				 * @param[in] _pos ofset apply of the text (in 3D)
				 */
				void setRelPos(const vec3& _pos) {
					m_position += _pos;
				};
				inline void setRelPos(const vec2& _pos) {
					setRelPos(vec3(_pos.x(),_pos.y(),0));
				};
				/**
				 * @brief set the Color of the current foreground font
				 * @param[in] _color Color to set on foreground (for next print)
				 */
				void setColor(const etk::Color<>& _color) {
					m_color = _color;
				};
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in] _pos Start position of the clipping
				 * @param[in] _width Width size of the clipping
				 */
				void setClippingWidth(const vec3& _pos, vec3 _width) {
					setClipping(_pos, _pos+_width);
				};
				inline void setClippingWidth(const vec2& _pos, const vec2& _width) {
					setClippingWidth(vec3(_pos.x(),_pos.y(),0), vec3(_width.x(),_width.y(),0));
				};
				/**
				 * @brief Request a clipping area for the text (next draw only)
				 * @param[in] _pos Start position of the clipping
				 * @param[in] _posEnd End position of the clipping
				 */
				void setClipping(const vec3& _pos, vec3 _posEnd);
				inline void setClipping(const vec2& _pos, const vec2& _posEnd) {
					setClipping(vec3(_pos.x(),_pos.y(),0), vec3(_posEnd.x(),_posEnd.y(),0));
				};
				/**
				 * @brief enable/Disable the clipping (without lose the current clipping position)
				 * @brief _newMode The new status of the clipping
				 */
				void setClippingMode(bool _newMode) {
					m_clippingEnable = _newMode;
				};
				/**
				 * @brief set a unique rotation of this element (not set in the rotate Generic system)
				 * @param[in] _angle Angle to set in radiant.
				 */
				void setAngle(float _angleRad);
				/**
				 * @brief add a compleate of the image to display with the requested size
				 * @param[in] _size size of the output image
				 */
				void print(const ivec2& _size) {
					print(vec2(_size.x(),_size.y()));
				};
				void print(const vec2& _size);
				/**
				 * @brief add a part of the image to display with the requested size
				 * @param[in] _size size of the output image
				 * @param[in] _sourcePosStart Start position in the image [0..1] (can be bigger but this repeate the image).
				 * @param[in] _sourcePosStop Stop position in the image [0..1] (can be bigger but this repeate the image).
				 */
				void printPart(const vec2& _size,
				               vec2 _sourcePosStart,
				               vec2 _sourcePosStop);
				/**
				 * @brief change the image Source  == > can not be done to display 2 images at the same time ...
				 * @param[in] _uri New file of the Image
				 * @param[in] _size for the image when Verctorial image loading is requested
				 */
				void setSource(const etk::Uri& _uri, int32_t _size=32) {
					setSource(_uri, vec2(_size,_size));
				};
				void setSource(const etk::Uri& _uri, const vec2& _size);
				void setSource(egami::Image _image);
				/**
				 * @brief Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
				 * @return the validity od the resources.
				 */
				bool hasSources();
				/**
				 * @brief get the source image registered size in the file (<0 when multiple size image)
				 * @return tre image registered size
				 */
				vec2 getRealSize();
			public:
				/**
				 * @brief Set render mode of the image
				 * @param[in] _mode Activation of distance field mode
				 */
				void setDistanceFieldMode(bool _mode);
				/**
				 * @brief Get the render methode.
				 * @return The render mode of the image.
				 */
				bool getDistanceFieldMode() const {
					return m_distanceFieldMode;
				}
		};
	};
};

