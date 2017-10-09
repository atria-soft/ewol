/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#ifndef __TARGET_OS__Web

#include <etk/types.hpp>
#include <gale/resource/Resource.hpp>
#include <ewol/resource/TextureFile.hpp>
#include <gale/resource/Program.hpp>

namespace ewol {
	namespace resource {
		/**
		 * @brief simple display of Colored3DObject ==> for DEBUG only Not availlable on ALL platform (like webGL)
		 */
		class Colored3DObject : public gale::Resource {
			protected:
				ememory::SharedPtr<gale::resource::Program> m_GLprogram;
				int32_t m_GLPosition;
				int32_t m_GLMatrix;
				int32_t m_GLColor;
			protected:
				Colored3DObject();
				void init();
			public:
				DECLARE_RESOURCE_FACTORY(Colored3DObject);
				virtual ~Colored3DObject();
			public:
				virtual void draw(const etk::Vector<vec3>& _vertices,
				                  const etk::Color<float>& _color,
				                  bool _updateDepthBuffer=true,
				                  bool _depthtest=true);
				virtual void draw(const etk::Vector<vec3>& _vertices,
				                  const etk::Color<float>& _color,
				                  mat4& _transformationMatrix,
				                  bool _updateDepthBuffer=true,
				                  bool _depthtest=true);
				virtual void drawLine(etk::Vector<vec3>& _vertices,
				                      const etk::Color<float>& _color,
				                      mat4& _transformationMatrix,
				                      bool _updateDepthBuffer=true,
				                      bool _depthtest=true);
				virtual void drawCubeLine(const vec3& _min,
				                          const vec3& _max,
				                          const etk::Color<float>& _color,
				                          mat4& _transformationMatrix,
				                          bool _updateDepthBuffer=true,
				                          bool _depthtest=true);
			public:
				void drawSquare(const vec3& _size,
				                mat4& _transformationMatrix,
				                const etk::Color<float>& _tmpColor);
				void drawSphere(float _radius,
				                int _lats,
				                int _longs,
				                mat4& _transformationMatrix,
				                const etk::Color<float>& _tmpColor);
				void drawCylinder(float _radius,
				                  float _size,
				                  int _lats,
				                  int _longs,
				                  mat4& _transformationMatrix,
				                  const etk::Color<float>& _tmpColor);
				void drawCapsule(float _radius,
				                 float _size,
				                 int _lats,
				                 int _longs,
				                 mat4& _transformationMatrix,
				                 const etk::Color<float>& _tmpColor);
				void drawCone(float _radius,
				              float _size,
				              int _lats,
				              int _longs,
				              mat4& _transformationMatrix,
				              const etk::Color<float>& _tmpColor);
				void drawTriangles(const etk::Vector<vec3>& _vertex,
				                   const etk::Vector<uint32_t>& _indice,
				                   mat4& _transformationMatrix,
				                   const etk::Color<float>& _tmpColor,
				                   const vec3& _offset=vec3(0,0,0.1));
		};
	};
};

#endif
