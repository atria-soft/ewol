/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#ifndef __TARGET_OS__Web

#include <etk/types.hpp>
#include <gale/resource/Resource.hpp>
#include <ewol/resource/Image.hpp>
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
				virtual void draw(std::vector<vec3>& _vertices,
				                  const etk::Color<float>& _color,
				                  bool _updateDepthBuffer=true,
				                  bool _depthtest=true);
				virtual void draw(std::vector<vec3>& _vertices,
				                  const etk::Color<float>& _color,
				                  mat4& _transformationMatrix,
				                  bool _updateDepthBuffer=true,
				                  bool _depthtest=true);
				virtual void drawLine(std::vector<vec3>& _vertices,
				                      const etk::Color<float>& _color,
				                      mat4& _transformationMatrix,
				                      bool _updateDepthBuffer=true,
				                      bool _depthtest=true);
			public:
				void drawSphere(float _radius,
				                int _lats,
				                int _longs,
				                mat4& _transformationMatrix,
				                const etk::Color<float>& _tmpColor);
				void drawSquare(const vec3& _size, // halph size
				                mat4& _transformationMatrix,
				                const etk::Color<float>& _tmpColor);
		};
	};
};

#endif
