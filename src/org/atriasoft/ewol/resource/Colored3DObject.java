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
				ememory::Ptr<gale::resource::Program> this.GLprogram;
				int this.GLPosition;
				int this.GLMatrix;
				int this.GLColor;
			protected:
				Colored3DObject();
				void init();
			public:
				DECLARE_RESOURCE_FACTORY(Colored3DObject);
				 ~Colored3DObject();
			public:
				 void draw( List<Vector3f> _vertices,
				                   etk::Color<float> _color,
				                  boolean _updateDepthBuffer=true,
				                  boolean _depthtest=true);
				 void draw( List<Vector3f> _vertices,
				                   etk::Color<float> _color,
				                  mat4 _transformationMatrix,
				                  boolean _updateDepthBuffer=true,
				                  boolean _depthtest=true);
				 void drawLine(List<Vector3f> _vertices,
				                       etk::Color<float> _color,
				                      mat4 _transformationMatrix,
				                      boolean _updateDepthBuffer=true,
				                      boolean _depthtest=true);
				 void drawCubeLine( Vector3f _min,
				                           Vector3f _max,
				                           etk::Color<float> _color,
				                          mat4 _transformationMatrix,
				                          boolean _updateDepthBuffer=true,
				                          boolean _depthtest=true);
			public:
				void drawSquare( Vector3f _size,
				                mat4 _transformationMatrix,
				                 etk::Color<float> _tmpColor);
				void drawSphere(float _radius,
				                int _lats,
				                int _longs,
				                mat4 _transformationMatrix,
				                 etk::Color<float> _tmpColor);
				void drawCylinder(float _radius,
				                  float _size,
				                  int _lats,
				                  int _longs,
				                  mat4 _transformationMatrix,
				                   etk::Color<float> _tmpColor);
				void drawCapsule(float _radius,
				                 float _size,
				                 int _lats,
				                 int _longs,
				                 mat4 _transformationMatrix,
				                  etk::Color<float> _tmpColor);
				void drawCone(float _radius,
				              float _size,
				              int _lats,
				              int _longs,
				              mat4 _transformationMatrix,
				               etk::Color<float> _tmpColor);
				void drawTriangles( List<Vector3f> _vertex,
				                    List<uint> _indice,
				                   mat4 _transformationMatrix,
				                    etk::Color<float> _tmpColor,
				                    Vector3f _offset=Vector3f(0,0,0.1));
		};
	};
};

#endif
