/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.resource.Resource;
import org.atriasoft.gale.resource.ResourceProgram;

/**
 * @brief simple display of Colored3DObject ==> for DEBUG only Not availlable on ALL platform (like webGL)
 */
public class RefactorColored3DObject extends Resource {
	protected ResourceProgram GLprogram;
	protected int GLPosition;
	protected int GLMatrix;
	protected int GLColor;
	
	public RefactorColored3DObject() {
		super();
		// get the shader resource :
		this.GLPosition = 0;
		this.GLprogram = ResourceProgram.create(new Uri("DATA:///simple3D.vert?lib=ewol"), new Uri("DATA:///simple3D.frag?lib=ewol"));
		if (this.GLprogram != null) {
			this.GLPosition = this.GLprogram.getAttribute("EW_coord3d");
			this.GLColor = this.GLprogram.getUniform("EW_color");
			this.GLMatrix = this.GLprogram.getUniform("EW_MatrixTransformation");
		}
	}
	
	@Override
	public void cleanUp() {
		// TODO Auto-generated method stub
		
	}
	
	public void draw(final List<Vector3f> _vertices, final Color _color) {
		draw(_vertices, _color, true, true);
	}
	
	public void draw(final List<Vector3f> _vertices, final Color _color, final boolean _updateDepthBuffer, final boolean _depthtest) {
		if (_vertices.size() <= 0) {
			return;
		}
		if (this.GLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		if (true == _depthtest) {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
			if (false == _updateDepthBuffer) {
				OpenGL.setDeathMask(false);
			}
		}
		
		//Log.debug("    display " + this.coord.size() + " elements" );
		this.GLprogram.use();
		// set Matrix: translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		final Matrix4f tmpMatrix = projMatrix.multiplyNew(camMatrix);
		this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
		// position :
		this.GLprogram.sendAttribute(this.GLPosition, 3/*x,y,z,unused*/, ResourceProgram.storeDataInFloatBufferVector3f(_vertices), 3);
		// color :
		this.GLprogram.uniformColor(this.GLColor, _color);
		// Request the draw od the elements: 
		OpenGL.drawArrays(OpenGL.RenderMode.triangle, 0, _vertices.size());
		this.GLprogram.unUse();
		// Request the draw od the elements: 
		//glDrawArrays(GL_LINES, 0, vertices.size());
		//this.GLprogram.UnUse();
		if (true == _depthtest) {
			if (false == _updateDepthBuffer) {
				OpenGL.setDeathMask(true);
				;
			}
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		}
	}
	
	public void draw(final List<Vector3f> _vertices, final Color _color, final Matrix4f _transformationMatrix) {
		draw(_vertices, _color, _transformationMatrix, true, true);
	}
	
	public void draw(final List<Vector3f> _vertices, final Color _color, final Matrix4f _transformationMatrix, final boolean _updateDepthBuffer, final boolean _depthtest) {
		if (_vertices.size() <= 0) {
			return;
		}
		if (this.GLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		if (true == _depthtest) {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
			if (false == _updateDepthBuffer) {
				OpenGL.setDeathMask(false);
			}
		}
		//Log.debug("    display " + this.coord.size() + " elements" );
		this.GLprogram.use();
		// set Matrix: translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		final Matrix4f tmpMatrix = projMatrix.multiplyNew(camMatrix).multiply(_transformationMatrix);
		this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
		// position :
		this.GLprogram.sendAttribute(this.GLPosition, 3/*x,y,z*/, ResourceProgram.storeDataInFloatBufferVector3f(_vertices), 3); // TODO : check 4->3
		// color :
		this.GLprogram.uniformColor(this.GLColor, _color);
		// Request the draw od the elements: 
		OpenGL.drawArrays(OpenGL.RenderMode.triangle, 0, _vertices.size());
		this.GLprogram.unUse();
		if (true == _depthtest) {
			if (false == _updateDepthBuffer) {
				OpenGL.setDeathMask(true);
			}
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		}
	}
	
	public void drawCapsule(final float _radius, final float _size, int _lats, final int _longs, final Matrix4f _transformationMatrix, final Color _tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		_lats = _lats / 2 * 2;
		
		// center to border (TOP)
		float offset = _size * 0.5f;
		for (int iii = _lats / 2 + 1; iii <= _lats; ++iii) {
			final float lat0 = (float) (Math.PI) * (-0.5f + (float) (iii - 1) / _lats);
			final float z0 = (float) (_radius * Math.sin(lat0));
			final float zr0 = (float) (_radius * Math.cos(lat0));
			
			final float lat1 = (float) (Math.PI) * (-0.5f + (float) (iii) / _lats);
			final float z1 = (float) (_radius * Math.sin(lat1));
			final float zr1 = (float) (_radius * Math.cos(lat1));
			
			for (int jjj = 0; jjj < _longs; ++jjj) {
				float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / _longs;
				float x = (float) Math.cos(lng);
				float y = (float) Math.sin(lng);
				final Vector3f v1 = new Vector3f(x * zr1, y * zr1, z1 + offset);
				final Vector3f v4 = new Vector3f(x * zr0, y * zr0, z0 + offset);
				
				lng = 2 * (float) (Math.PI) * (jjj) / _longs;
				x = (float) Math.cos(lng);
				y = (float) Math.sin(lng);
				final Vector3f v2 = new Vector3f(x * zr1, y * zr1, z1 + offset);
				final Vector3f v3 = new Vector3f(x * zr0, y * zr0, z0 + offset);
				tmpVertices.add(v1);
				tmpVertices.add(v2);
				tmpVertices.add(v3);
				
				tmpVertices.add(v1);
				tmpVertices.add(v3);
				tmpVertices.add(v4);
			}
		}
		// Cylinder
		for (int jjj = 0; jjj < _longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / _longs;
			
			final float z = _size * 0.5f;
			
			float x = (float) (Math.cos(lng) * _radius);
			float y = (float) (Math.sin(lng) * _radius);
			final Vector3f v2 = new Vector3f(x, y, z);
			final Vector3f v2b = new Vector3f(x, y, -z);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / _longs;
			x = (float) (Math.cos(lng) * _radius);
			y = (float) (Math.sin(lng) * _radius);
			final Vector3f v3 = new Vector3f(x, y, z);
			final Vector3f v3b = new Vector3f(x, y, -z);
			
			tmpVertices.add(v2);
			tmpVertices.add(v3);
			tmpVertices.add(v3b);
			
			tmpVertices.add(v2);
			tmpVertices.add(v3b);
			tmpVertices.add(v2b);
		}
		// center to border (BUTTOM)
		offset = -_size * 0.5f;
		for (int iii = 0; iii <= _lats / 2; ++iii) {
			final float lat0 = (float) (Math.PI) * (-0.5f + (float) (iii - 1) / _lats);
			final float z0 = (float) (_radius * Math.sin(lat0));
			final float zr0 = (float) (_radius * Math.cos(lat0));
			
			final float lat1 = (float) (Math.PI) * (-0.5f + (float) (iii) / _lats);
			final float z1 = (float) (_radius * Math.sin(lat1));
			final float zr1 = (float) (_radius * Math.cos(lat1));
			
			for (int jjj = 0; jjj < _longs; ++jjj) {
				float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / _longs;
				float x = (float) Math.cos(lng);
				float y = (float) Math.sin(lng);
				final Vector3f v1 = new Vector3f(x * zr1, y * zr1, z1 + offset);
				final Vector3f v4 = new Vector3f(x * zr0, y * zr0, z0 + offset);
				
				lng = 2 * (float) (Math.PI) * (jjj) / _longs;
				x = (float) Math.cos(lng);
				y = (float) Math.sin(lng);
				final Vector3f v2 = new Vector3f(x * zr1, y * zr1, z1 + offset);
				final Vector3f v3 = new Vector3f(x * zr0, y * zr0, z0 + offset);
				tmpVertices.add(v1);
				tmpVertices.add(v2);
				tmpVertices.add(v3);
				
				tmpVertices.add(v1);
				tmpVertices.add(v3);
				tmpVertices.add(v4);
			}
		}
		draw(tmpVertices, _tmpColor, _transformationMatrix);
	}
	
	public void drawCone(final float _radius, final float _size, final int _lats, final int _longs, final Matrix4f _transformationMatrix, final Color _tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		// center to border (TOP)
		for (int jjj = 0; jjj < _longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / _longs;
			final Vector3f v1 = new Vector3f(0.0f, 0.0f, -_size / 2);
			
			float x = (float) (Math.cos(lng) * _radius);
			float y = (float) (Math.sin(lng) * _radius);
			final Vector3f v2 = new Vector3f(x, y, _size / 2);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / _longs;
			x = (float) (Math.cos(lng) * _radius);
			y = (float) (Math.sin(lng) * _radius);
			final Vector3f v3 = new Vector3f(x, y, _size / 2);
			tmpVertices.add(v1);
			tmpVertices.add(v3);
			tmpVertices.add(v2);
		}
		// center to border (BUTTOM)
		for (int jjj = 0; jjj < _longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / _longs;
			
			final Vector3f v1 = new Vector3f(0.0f, 0.0f, _size / 2);
			
			float x = (float) (Math.cos(lng) * _radius);
			float y = (float) (Math.sin(lng) * _radius);
			final Vector3f v2 = new Vector3f(x, y, _size / 2);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / _longs;
			x = (float) (Math.cos(lng) * _radius);
			y = (float) (Math.sin(lng) * _radius);
			final Vector3f v3 = new Vector3f(x, y, _size / 2);
			tmpVertices.add(v1);
			tmpVertices.add(v2);
			tmpVertices.add(v3);
		}
		draw(tmpVertices, _tmpColor, _transformationMatrix);
	}
	
	public void drawCubeLine(final Vector3f _min, final Vector3f _max, final Color _color, final Matrix4f _transformationMatrix) {
		drawCubeLine(_min, _max, _color, _transformationMatrix, true, true);
	}
	
	public void drawCubeLine(final Vector3f _min, final Vector3f _max, final Color _color, final Matrix4f _transformationMatrix, final boolean _updateDepthBuffer, final boolean _depthtest) {
		final List<Vector3f> vertices = new ArrayList<>();
		vertices.add(new Vector3f(_min.x, _min.y, _min.z));
		vertices.add(new Vector3f(_max.x, _min.y, _min.z));
		
		vertices.add(new Vector3f(_max.x, _min.y, _min.z));
		vertices.add(new Vector3f(_max.x, _min.y, _max.z));
		
		vertices.add(new Vector3f(_max.x, _min.y, _max.z));
		vertices.add(new Vector3f(_min.x, _min.y, _max.z));
		
		vertices.add(new Vector3f(_min.x, _min.y, _max.z));
		vertices.add(new Vector3f(_min.x, _min.y, _min.z));
		
		vertices.add(new Vector3f(_min.x, _max.y, _min.z));
		vertices.add(new Vector3f(_max.x, _max.y, _min.z));
		
		vertices.add(new Vector3f(_max.x, _max.y, _min.z));
		vertices.add(new Vector3f(_max.x, _max.y, _max.z));
		
		vertices.add(new Vector3f(_max.x, _max.y, _max.z));
		vertices.add(new Vector3f(_min.x, _max.y, _max.z));
		
		vertices.add(new Vector3f(_min.x, _max.y, _max.z));
		vertices.add(new Vector3f(_min.x, _max.y, _min.z));
		
		vertices.add(new Vector3f(_min.x, _min.y, _min.z));
		vertices.add(new Vector3f(_min.x, _max.y, _min.z));
		
		vertices.add(new Vector3f(_max.x, _min.y, _min.z));
		vertices.add(new Vector3f(_max.x, _max.y, _min.z));
		
		vertices.add(new Vector3f(_max.x, _min.y, _max.z));
		vertices.add(new Vector3f(_max.x, _max.y, _max.z));
		
		vertices.add(new Vector3f(_min.x, _min.y, _max.z));
		vertices.add(new Vector3f(_min.x, _max.y, _max.z));
		
		drawLine(vertices, _color, _transformationMatrix, _updateDepthBuffer, _depthtest);
	}
	
	public void drawCylinder(final float _radius, final float _size, final int _lats, final int _longs, final Matrix4f _transformationMatrix, final Color _tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		// center to border (TOP)
		
		// center to border (TOP)
		for (int jjj = 0; jjj < _longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / _longs;
			
			final float z = _size * 0.5f;
			final Vector3f v1 = new Vector3f(0.0f, 0.0f, z);
			
			float x = (float) (Math.cos(lng) * _radius);
			float y = (float) (Math.sin(lng) * _radius);
			final Vector3f v2 = new Vector3f(x, y, z);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / _longs;
			x = (float) (Math.cos(lng) * _radius);
			y = (float) (Math.sin(lng) * _radius);
			final Vector3f v3 = new Vector3f(x, y, z);
			tmpVertices.add(v1);
			tmpVertices.add(v3);
			tmpVertices.add(v2);
		}
		// Cylinder
		for (int jjj = 0; jjj < _longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / _longs;
			
			final float z = _size * 0.5f;
			
			float x = (float) (Math.cos(lng) * _radius);
			float y = (float) (Math.sin(lng) * _radius);
			final Vector3f v2 = new Vector3f(x, y, z);
			final Vector3f v2b = new Vector3f(x, y, -z);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / _longs;
			x = (float) (Math.cos(lng) * _radius);
			y = (float) (Math.sin(lng) * _radius);
			final Vector3f v3 = new Vector3f(x, y, z);
			final Vector3f v3b = new Vector3f(x, y, -z);
			
			tmpVertices.add(v2);
			tmpVertices.add(v3);
			tmpVertices.add(v3b);
			
			tmpVertices.add(v2);
			tmpVertices.add(v3b);
			tmpVertices.add(v2b);
		}
		// center to border (BUTTOM)
		for (int jjj = 0; jjj < _longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / _longs;
			
			final float z = _size * -0.5f;
			final Vector3f v1 = new Vector3f(0.0f, 0.0f, z);
			
			float x = (float) (Math.cos(lng) * _radius);
			float y = (float) (Math.sin(lng) * _radius);
			final Vector3f v2 = new Vector3f(x, y, z);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / _longs;
			x = (float) (Math.cos(lng) * _radius);
			y = (float) (Math.sin(lng) * _radius);
			final Vector3f v3 = new Vector3f(x, y, z);
			tmpVertices.add(v1);
			tmpVertices.add(v2);
			tmpVertices.add(v3);
		}
		draw(tmpVertices, _tmpColor, _transformationMatrix);
	}
	
	public void drawLine(final List<Vector3f> _vertices, final Color _color, final Matrix4f _transformationMatrix) {
		drawLine(_vertices, _color, _transformationMatrix, true, true);
	}
	
	public void drawLine(final List<Vector3f> _vertices, final Color _color, final Matrix4f _transformationMatrix, final boolean _updateDepthBuffer, final boolean _depthtest) {
		if (_vertices.size() <= 0) {
			return;
		}
		if (this.GLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		if (true == _depthtest) {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
			if (false == _updateDepthBuffer) {
				OpenGL.setDeathMask(false);
			}
		}
		//Log.debug("    display " + this.coord.size() + " elements" );
		this.GLprogram.use();
		// set Matrix: translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		final Matrix4f tmpMatrix = projMatrix.multiplyNew(camMatrix).multiply(_transformationMatrix);
		this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
		// position :
		this.GLprogram.sendAttribute(this.GLPosition, 3/*x,y,z*/, ResourceProgram.storeDataInFloatBufferVector3f(_vertices), 3);// TODO : check 4->3
		// color :
		this.GLprogram.uniformColor(this.GLColor, _color);
		// Request the draw od the elements: 
		OpenGL.drawArrays(OpenGL.RenderMode.line, 0, _vertices.size());
		this.GLprogram.unUse();
		if (true == _depthtest) {
			if (false == _updateDepthBuffer) {
				OpenGL.setDeathMask(true);
			}
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		}
	}
	
	public void drawSphere(final float _radius, final int _lats, final int _longs, final Matrix4f _transformationMatrix, final Color _tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		for (int iii = 0; iii <= _lats; ++iii) {
			final float lat0 = (float) (Math.PI) * (-0.5f + (float) (iii - 1) / _lats);
			final float z0 = (float) (_radius * Math.sin(lat0));
			final float zr0 = (float) (_radius * Math.cos(lat0));
			
			final float lat1 = (float) (Math.PI) * (-0.5f + (float) (iii) / _lats);
			final float z1 = (float) (_radius * Math.sin(lat1));
			final float zr1 = (float) (_radius * Math.cos(lat1));
			
			for (int jjj = 0; jjj < _longs; ++jjj) {
				float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / _longs;
				float x = (float) Math.cos(lng);
				float y = (float) Math.sin(lng);
				final Vector3f v1 = new Vector3f(x * zr1, y * zr1, z1);
				final Vector3f v4 = new Vector3f(x * zr0, y * zr0, z0);
				
				lng = 2 * (float) (Math.PI) * (jjj) / _longs;
				x = (float) Math.cos(lng);
				y = (float) Math.sin(lng);
				final Vector3f v2 = new Vector3f(x * zr1, y * zr1, z1);
				final Vector3f v3 = new Vector3f(x * zr0, y * zr0, z0);
				
				tmpVertices.add(v1);
				tmpVertices.add(v2);
				tmpVertices.add(v3);
				
				tmpVertices.add(v1);
				tmpVertices.add(v3);
				tmpVertices.add(v4);
			}
		}
		draw(tmpVertices, _tmpColor, _transformationMatrix);
	}
	
	public void drawSquare(final Vector3f _size, final Matrix4f _transformationMatrix, final Color _tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		final int[] indices = { 0, 1, 2, 3, 2, 1, 4, 0, 6, 6, 0, 2, 5, 1, 4, 4, 1, 0, 7, 3, 1, 7, 1, 5, 5, 4, 7, 7, 4, 6, 7, 2, 3, 7, 6, 2 };
		final Vector3f[] vertices = { new Vector3f(_size.x, _size.y, _size.z), new Vector3f(-_size.x, _size.y, _size.z), new Vector3f(_size.x, -_size.y, _size.z),
				new Vector3f(-_size.x, -_size.y, _size.z), new Vector3f(_size.x, _size.y, -_size.z), new Vector3f(-_size.x, _size.y, -_size.z), new Vector3f(_size.x, -_size.y, -_size.z),
				new Vector3f(-_size.x, -_size.y, -_size.z) };
		tmpVertices.clear();
		for (int iii = 0; iii < 36; iii += 3) {
			// normal calculation :
			//btVector3 normal = (vertices[indices[iii+2]]-vertices[indices[iii]]).cross(vertices[indices[iii+1]]-vertices[indices[iii]]);
			//normal.normalize ();
			tmpVertices.add(vertices[indices[iii]]);
			tmpVertices.add(vertices[indices[iii + 1]]);
			tmpVertices.add(vertices[indices[iii + 2]]);
		}
		draw(tmpVertices, _tmpColor, _transformationMatrix);
	}
	
	public void drawTriangles(final List<Vector3f> _vertex, final List<Integer> _indice, final Matrix4f _transformationMatrix, final Color _tmpColor) {
		drawTriangles(_vertex, _indice, _transformationMatrix, _tmpColor, new Vector3f(0.0f, 0.0f, 0.1f));
	}
	
	public void drawTriangles(final List<Vector3f> _vertex, final List<Integer> _indice, final Matrix4f _transformationMatrix, final Color _tmpColor, final Vector3f _offset) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		for (int iii = 0; iii < _indice.size() / 3; ++iii) {
			tmpVertices.add(_vertex.get(_indice.get(iii * 3 + 0)).addNew(_offset));
			tmpVertices.add(_vertex.get(_indice.get(iii * 3 + 1)).addNew(_offset));
			tmpVertices.add(_vertex.get(_indice.get(iii * 3 + 2)).addNew(_offset));
			//Log.info("  indices " + _indice[iii*3 + 0] + " " + _indice[iii*3 + 1] + " " + _indice[iii*3 + 2]);
			//Log.info(" triangle " + _vertex[_indice[iii*3 + 0]] + " " + _vertex[_indice[iii*3 + 1]] + " " + _vertex[_indice[iii*3 + 2]]);
		}
		//Log.info("display " + tmpVertices.size() + " vertices form " + _indice.size());
		draw(tmpVertices, _tmpColor, _transformationMatrix);
	}
}
