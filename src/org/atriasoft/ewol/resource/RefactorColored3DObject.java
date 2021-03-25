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
 * simple display of Colored3DObject ==> for DEBUG only Not availlable on
 *        ALL platform (like webGL)
 */
public class RefactorColored3DObject extends Resource {
	protected int oGLColor;
	protected int oGLMatrix;
	protected int oGLPosition;
	protected ResourceProgram oGLprogram;
	
	public RefactorColored3DObject() {
		// get the shader resource :
		this.oGLPosition = 0;
		this.oGLprogram = ResourceProgram.create(new Uri("DATA", "simple3D.vert", "ewol"), new Uri("DATA", "simple3D.frag", "ewol"));
		if (this.oGLprogram != null) {
			this.oGLPosition = this.oGLprogram.getAttribute("EWcoord3d");
			this.oGLColor = this.oGLprogram.getUniform("EWcolor");
			this.oGLMatrix = this.oGLprogram.getUniform("EWMatrixTransformation");
		}
	}
	
	@Override
	public void cleanUp() {
		// TODO Auto-generated method stub
		
	}
	
	public void draw(final List<Vector3f> vertices, final Color color) {
		draw(vertices, color, true, true);
	}
	
	public void draw(final List<Vector3f> vertices, final Color color, final boolean updateDepthBuffer, final boolean depthtest) {
		if (vertices.size() <= 0) {
			return;
		}
		if (this.oGLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		if (depthtest) {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
			if (!updateDepthBuffer) {
				OpenGL.setDeathMask(false);
			}
		}
		
		// Log.debug(" display " + this.coord.size() + " elements" );
		this.oGLprogram.use();
		// set Matrix: translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		final Matrix4f tmpMatrix = projMatrix.multiply(camMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrix, tmpMatrix);
		// position :
		this.oGLprogram.sendAttribute(this.oGLPosition, 3/* x,y,z,unused */, ResourceProgram.storeDataInFloatBufferVector3f(vertices), 3);
		// color :
		this.oGLprogram.uniformColor(this.oGLColor, color);
		// Request the draw od the elements:
		OpenGL.drawArrays(OpenGL.RenderMode.triangle, 0, vertices.size());
		this.oGLprogram.unUse();
		// Request the draw od the elements:
		// glDrawArrays(GLLINES, 0, vertices.size());
		// this.GLprogram.UnUse();
		if (depthtest) {
			if (!updateDepthBuffer) {
				OpenGL.setDeathMask(true);
				
			}
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		}
	}
	
	public void draw(final List<Vector3f> vertices, final Color color, final Matrix4f transformationMatrix) {
		draw(vertices, color, transformationMatrix, true, true);
	}
	
	public void draw(final List<Vector3f> vertices, final Color color, final Matrix4f transformationMatrix, final boolean updateDepthBuffer, final boolean depthTest) {
		if (vertices.size() <= 0) {
			return;
		}
		if (this.oGLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		if (depthTest) {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
			if (!updateDepthBuffer) {
				OpenGL.setDeathMask(false);
			}
		}
		// Log.debug(" display " + this.coord.size() + " elements" );
		this.oGLprogram.use();
		// set Matrix: translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		final Matrix4f tmpMatrix = projMatrix.multiply(camMatrix).multiply(transformationMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrix, tmpMatrix);
		// position :
		this.oGLprogram.sendAttribute(this.oGLPosition, 3/* x,y,z */, ResourceProgram.storeDataInFloatBufferVector3f(vertices), 3); // TODO : check 4->3
		// color :
		this.oGLprogram.uniformColor(this.oGLColor, color);
		// Request the draw od the elements:
		OpenGL.drawArrays(OpenGL.RenderMode.triangle, 0, vertices.size());
		this.oGLprogram.unUse();
		if (depthTest) {
			if (!updateDepthBuffer) {
				OpenGL.setDeathMask(true);
			}
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		}
	}
	
	public void drawCapsule(final float radius, final float size, int lats, final int longs, final Matrix4f transformationMatrix, final Color tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		lats = lats / 2 * 2;
		
		// center to border (TOP)
		float offset = size * 0.5f;
		for (int iii = lats / 2 + 1; iii <= lats; ++iii) {
			final float lat0 = (float) (Math.PI) * (-0.5f + (float) (iii - 1) / lats);
			final float z0 = (float) (radius * Math.sin(lat0));
			final float zr0 = (float) (radius * Math.cos(lat0));
			
			final float lat1 = (float) (Math.PI) * (-0.5f + (float) (iii) / lats);
			final float z1 = (float) (radius * Math.sin(lat1));
			final float zr1 = (float) (radius * Math.cos(lat1));
			
			for (int jjj = 0; jjj < longs; ++jjj) {
				float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / longs;
				float x = (float) Math.cos(lng);
				float y = (float) Math.sin(lng);
				final Vector3f v1 = new Vector3f(x * zr1, y * zr1, z1 + offset);
				final Vector3f v4 = new Vector3f(x * zr0, y * zr0, z0 + offset);
				
				lng = 2 * (float) (Math.PI) * (jjj) / longs;
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
		for (int jjj = 0; jjj < longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / longs;
			
			final float z = size * 0.5f;
			
			float x = (float) (Math.cos(lng) * radius);
			float y = (float) (Math.sin(lng) * radius);
			final Vector3f v2 = new Vector3f(x, y, z);
			final Vector3f v2b = new Vector3f(x, y, -z);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / longs;
			x = (float) (Math.cos(lng) * radius);
			y = (float) (Math.sin(lng) * radius);
			final Vector3f v3 = new Vector3f(x, y, z);
			final Vector3f v3b = new Vector3f(x, y, -z);
			
			tmpVertices.add(v2);
			tmpVertices.add(v3);
			tmpVertices.add(v3b);
			
			tmpVertices.add(v2);
			tmpVertices.add(v3b);
			tmpVertices.add(v2b);
		}
		// center to border (BOTTOM)
		offset = -size * 0.5f;
		for (int iii = 0; iii <= lats / 2; ++iii) {
			final float lat0 = (float) (Math.PI) * (-0.5f + (float) (iii - 1) / lats);
			final float z0 = (float) (radius * Math.sin(lat0));
			final float zr0 = (float) (radius * Math.cos(lat0));
			
			final float lat1 = (float) (Math.PI) * (-0.5f + (float) (iii) / lats);
			final float z1 = (float) (radius * Math.sin(lat1));
			final float zr1 = (float) (radius * Math.cos(lat1));
			
			for (int jjj = 0; jjj < longs; ++jjj) {
				float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / longs;
				float x = (float) Math.cos(lng);
				float y = (float) Math.sin(lng);
				final Vector3f v1 = new Vector3f(x * zr1, y * zr1, z1 + offset);
				final Vector3f v4 = new Vector3f(x * zr0, y * zr0, z0 + offset);
				
				lng = 2 * (float) (Math.PI) * (jjj) / longs;
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
		draw(tmpVertices, tmpColor, transformationMatrix);
	}
	
	public void drawCone(final float radius, final float size, final int lats, final int longs, final Matrix4f transformationMatrix, final Color tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		// center to border (TOP)
		for (int jjj = 0; jjj < longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / longs;
			final Vector3f v1 = new Vector3f(0.0f, 0.0f, -size / 2);
			
			float x = (float) (Math.cos(lng) * radius);
			float y = (float) (Math.sin(lng) * radius);
			final Vector3f v2 = new Vector3f(x, y, size / 2);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / longs;
			x = (float) (Math.cos(lng) * radius);
			y = (float) (Math.sin(lng) * radius);
			final Vector3f v3 = new Vector3f(x, y, size / 2);
			tmpVertices.add(v1);
			tmpVertices.add(v3);
			tmpVertices.add(v2);
		}
		// center to border (BOTTOM)
		for (int jjj = 0; jjj < longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / longs;
			
			final Vector3f v1 = new Vector3f(0.0f, 0.0f, size / 2);
			
			float x = (float) (Math.cos(lng) * radius);
			float y = (float) (Math.sin(lng) * radius);
			final Vector3f v2 = new Vector3f(x, y, size / 2);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / longs;
			x = (float) (Math.cos(lng) * radius);
			y = (float) (Math.sin(lng) * radius);
			final Vector3f v3 = new Vector3f(x, y, size / 2);
			tmpVertices.add(v1);
			tmpVertices.add(v2);
			tmpVertices.add(v3);
		}
		draw(tmpVertices, tmpColor, transformationMatrix);
	}
	
	public void drawCubeLine(final Vector3f min, final Vector3f max, final Color color, final Matrix4f transformationMatrix) {
		drawCubeLine(min, max, color, transformationMatrix, true, true);
	}
	
	public void drawCubeLine(final Vector3f min, final Vector3f max, final Color color, final Matrix4f transformationMatrix, final boolean updateDepthBuffer, final boolean depthtest) {
		final List<Vector3f> vertices = new ArrayList<>();
		vertices.add(new Vector3f(min.x(), min.y(), min.z()));
		vertices.add(new Vector3f(max.x(), min.y(), min.z()));
		
		vertices.add(new Vector3f(max.x(), min.y(), min.z()));
		vertices.add(new Vector3f(max.x(), min.y(), max.z()));
		
		vertices.add(new Vector3f(max.x(), min.y(), max.z()));
		vertices.add(new Vector3f(min.x(), min.y(), max.z()));
		
		vertices.add(new Vector3f(min.x(), min.y(), max.z()));
		vertices.add(new Vector3f(min.x(), min.y(), min.z()));
		
		vertices.add(new Vector3f(min.x(), max.y(), min.z()));
		vertices.add(new Vector3f(max.x(), max.y(), min.z()));
		
		vertices.add(new Vector3f(max.x(), max.y(), min.z()));
		vertices.add(new Vector3f(max.x(), max.y(), max.z()));
		
		vertices.add(new Vector3f(max.x(), max.y(), max.z()));
		vertices.add(new Vector3f(min.x(), max.y(), max.z()));
		
		vertices.add(new Vector3f(min.x(), max.y(), max.z()));
		vertices.add(new Vector3f(min.x(), max.y(), min.z()));
		
		vertices.add(new Vector3f(min.x(), min.y(), min.z()));
		vertices.add(new Vector3f(min.x(), max.y(), min.z()));
		
		vertices.add(new Vector3f(max.x(), min.y(), min.z()));
		vertices.add(new Vector3f(max.x(), max.y(), min.z()));
		
		vertices.add(new Vector3f(max.x(), min.y(), max.z()));
		vertices.add(new Vector3f(max.x(), max.y(), max.z()));
		
		vertices.add(new Vector3f(min.x(), min.y(), max.z()));
		vertices.add(new Vector3f(min.x(), max.y(), max.z()));
		
		drawLine(vertices, color, transformationMatrix, updateDepthBuffer, depthtest);
	}
	
	public void drawCylinder(final float radius, final float size, final int lats, final int longs, final Matrix4f transformationMatrix, final Color tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		// center to border (TOP)
		
		// center to border (TOP)
		for (int jjj = 0; jjj < longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / longs;
			
			final float z = size * 0.5f;
			final Vector3f v1 = new Vector3f(0.0f, 0.0f, z);
			
			float x = (float) (Math.cos(lng) * radius);
			float y = (float) (Math.sin(lng) * radius);
			final Vector3f v2 = new Vector3f(x, y, z);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / longs;
			x = (float) (Math.cos(lng) * radius);
			y = (float) (Math.sin(lng) * radius);
			final Vector3f v3 = new Vector3f(x, y, z);
			tmpVertices.add(v1);
			tmpVertices.add(v3);
			tmpVertices.add(v2);
		}
		// Cylinder
		for (int jjj = 0; jjj < longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / longs;
			
			final float z = size * 0.5f;
			
			float x = (float) (Math.cos(lng) * radius);
			float y = (float) (Math.sin(lng) * radius);
			final Vector3f v2 = new Vector3f(x, y, z);
			final Vector3f v2b = new Vector3f(x, y, -z);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / longs;
			x = (float) (Math.cos(lng) * radius);
			y = (float) (Math.sin(lng) * radius);
			final Vector3f v3 = new Vector3f(x, y, z);
			final Vector3f v3b = new Vector3f(x, y, -z);
			
			tmpVertices.add(v2);
			tmpVertices.add(v3);
			tmpVertices.add(v3b);
			
			tmpVertices.add(v2);
			tmpVertices.add(v3b);
			tmpVertices.add(v2b);
		}
		// center to border (BOTTOM)
		for (int jjj = 0; jjj < longs; ++jjj) {
			float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / longs;
			
			final float z = size * -0.5f;
			final Vector3f v1 = new Vector3f(0.0f, 0.0f, z);
			
			float x = (float) (Math.cos(lng) * radius);
			float y = (float) (Math.sin(lng) * radius);
			final Vector3f v2 = new Vector3f(x, y, z);
			
			lng = 2.0f * (float) (Math.PI) * (jjj) / longs;
			x = (float) (Math.cos(lng) * radius);
			y = (float) (Math.sin(lng) * radius);
			final Vector3f v3 = new Vector3f(x, y, z);
			tmpVertices.add(v1);
			tmpVertices.add(v2);
			tmpVertices.add(v3);
		}
		draw(tmpVertices, tmpColor, transformationMatrix);
	}
	
	public void drawLine(final List<Vector3f> vertices, final Color color, final Matrix4f transformationMatrix) {
		drawLine(vertices, color, transformationMatrix, true, true);
	}
	
	public void drawLine(final List<Vector3f> vertices, final Color color, final Matrix4f transformationMatrix, final boolean updateDepthBuffer, final boolean depthTest) {
		if (vertices.size() <= 0) {
			return;
		}
		if (this.oGLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		if (depthTest) {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
			if (!updateDepthBuffer) {
				OpenGL.setDeathMask(false);
			}
		}
		// Log.debug(" display " + this.coord.size() + " elements" );
		this.oGLprogram.use();
		// set Matrix: translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		final Matrix4f tmpMatrix = projMatrix.multiply(camMatrix).multiply(transformationMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrix, tmpMatrix);
		// position :
		this.oGLprogram.sendAttribute(this.oGLPosition, 3/* x,y,z */, ResourceProgram.storeDataInFloatBufferVector3f(vertices), 3);// TODO check 4->3
		// color :
		this.oGLprogram.uniformColor(this.oGLColor, color);
		// Request the draw od the elements:
		OpenGL.drawArrays(OpenGL.RenderMode.line, 0, vertices.size());
		this.oGLprogram.unUse();
		if (depthTest) {
			if (!updateDepthBuffer) {
				OpenGL.setDeathMask(true);
			}
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		}
	}
	
	public void drawSphere(final float radius, final int lats, final int longs, final Matrix4f transformationMatrix, final Color tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		for (int iii = 0; iii <= lats; ++iii) {
			final float lat0 = (float) (Math.PI) * (-0.5f + (float) (iii - 1) / lats);
			final float z0 = (float) (radius * Math.sin(lat0));
			final float zr0 = (float) (radius * Math.cos(lat0));
			
			final float lat1 = (float) (Math.PI) * (-0.5f + (float) (iii) / lats);
			final float z1 = (float) (radius * Math.sin(lat1));
			final float zr1 = (float) (radius * Math.cos(lat1));
			
			for (int jjj = 0; jjj < longs; ++jjj) {
				float lng = 2.0f * (float) (Math.PI) * (jjj - 1) / longs;
				float x = (float) Math.cos(lng);
				float y = (float) Math.sin(lng);
				final Vector3f v1 = new Vector3f(x * zr1, y * zr1, z1);
				final Vector3f v4 = new Vector3f(x * zr0, y * zr0, z0);
				
				lng = 2 * (float) (Math.PI) * (jjj) / longs;
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
		draw(tmpVertices, tmpColor, transformationMatrix);
	}
	
	public void drawSquare(final Vector3f size, final Matrix4f transformationMatrix, final Color tmpColor) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		final int[] indices = { 0, 1, 2, 3, 2, 1, 4, 0, 6, 6, 0, 2, 5, 1, 4, 4, 1, 0, 7, 3, 1, 7, 1, 5, 5, 4, 7, 7, 4, 6, 7, 2, 3, 7, 6, 2 };
		final Vector3f[] vertices = { new Vector3f(size.x(), size.y(), size.z()), new Vector3f(-size.x(), size.y(), size.z()), new Vector3f(size.x(), -size.y(), size.z()),
				new Vector3f(-size.x(), -size.y(), size.z()), new Vector3f(size.x(), size.y(), -size.z()), new Vector3f(-size.x(), size.y(), -size.z()), new Vector3f(size.x(), -size.y(), -size.z()),
				new Vector3f(-size.x(), -size.y(), -size.z()) };
		for (int iii = 0; iii < 36; iii += 3) {
			// normal calculation :
			// btVector3 normal =
			// (vertices[indices[iii+2]]-vertices[indices[iii]]).cross(vertices[indices[iii+1]]-vertices[indices[iii]]);
			// normal.normalize ();
			tmpVertices.add(vertices[indices[iii]]);
			tmpVertices.add(vertices[indices[iii + 1]]);
			tmpVertices.add(vertices[indices[iii + 2]]);
		}
		draw(tmpVertices, tmpColor, transformationMatrix);
	}
	
	public void drawTriangles(final List<Vector3f> vertex, final List<Integer> indice, final Matrix4f transformationMatrix, final Color tmpColor) {
		drawTriangles(vertex, indice, transformationMatrix, tmpColor, new Vector3f(0.0f, 0.0f, 0.1f));
	}
	
	public void drawTriangles(final List<Vector3f> vertex, final List<Integer> indice, final Matrix4f transformationMatrix, final Color tmpColor, final Vector3f offset) {
		final List<Vector3f> tmpVertices = new ArrayList<>();
		for (int iii = 0; iii < indice.size() / 3; ++iii) {
			tmpVertices.add(vertex.get(indice.get(iii * 3)).add(offset));
			tmpVertices.add(vertex.get(indice.get(iii * 3 + 1)).add(offset));
			tmpVertices.add(vertex.get(indice.get(iii * 3 + 2)).add(offset));
			// Log.info(" indices " + indice[iii*3 + 0] + " " + indice[iii*3 + 1] + " " +
			// indice[iii*3 + 2]);
			// Log.info(" triangle " + vertex[indice[iii*3 + 0]] + " " +
			// vertex[indice[iii*3 + 1]] + " " + vertex[indice[iii*3 + 2]]);
		}
		// Log.info("display " + tmpVertices.size() + " vertices form " +
		// indice.size());
		draw(tmpVertices, tmpColor, transformationMatrix);
	}
}
