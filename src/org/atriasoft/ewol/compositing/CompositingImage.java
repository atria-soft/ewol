/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import org.atriasoft.egami.ImageByteRGBA;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.backend3d.OpenGL.RenderMode;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.gale.resource.ResourceTexture2;
import org.atriasoft.gale.resource.ResourceTextureFile;
import org.atriasoft.gale.resource.ResourceVirtualBufferObject;

public class CompositingImage extends Compositing {
	public static final int NB_VBO = 3;
	public static final int SIZE_AUTO = 0;
	public static final int VBO_ID_COLOR = 2;
	// VBO table property:
	public static final int VBO_ID_COORD = 0;
	public static final int VBO_ID_COORD_TEX = 1;
	private float angle = 0; //!< Angle to set at the axes
	private boolean clippingEnable = true; //!< true if the clipping must be activated
	private Vector3f clippingPosStart = new Vector3f(0, 0, 0); //!< Clipping start position
	private Vector3f clippingPosStop = new Vector3f(0, 0, 0); //!< Clipping stop position
	private Color color = new Color(1, 1, 1); //!< The text foreground color
	private Uri filename;
	private int oGLColor = -1; //!< openGL id on the element (color buffer)
	private int oGLMatrix = -1; //!< openGL id on the element (transformation matrix)
	private int oGLPosition = -1; //!< openGL id on the element (vertex buffer)
	private ResourceProgram oGLprogram = null; //!< pointer on the opengl display program
	private int oGLtexID = -1; //!< openGL id on the element (texture ID)
	private int oGLtexture = -1; //!< openGL id on the element (Texture position)
	private Vector3f position = new Vector3f(0, 0, 0); //!< The current position to draw
	private Vector2i requestSize = new Vector2i(2, 2);
	
	private ResourceTextureFile resource = null; //!< texture resources
	private ResourceTexture2 resourceImage = null; //!< texture resources
	private ResourceVirtualBufferObject vbo = null;
	
	final Color[] vboDataColors = new Color[6];
	final Vector3f[] vboDataCoords = new Vector3f[6];
	final Vector2f[] vboDataCoordsTex = new Vector2f[6];
	
	public CompositingImage() {
		this(new Uri("DATA", ""), CompositingImage.SIZE_AUTO);
	}
	
	/**
	 * generic ructor
	 * @param uri URI of the file that might be loaded
	 * @param size for the image when Verctorial image loading is requested
	 */
	public CompositingImage(final Uri uri, final int size) {
		this.filename = uri;
		// Create the VBO:
		this.vbo = ResourceVirtualBufferObject.create(CompositingImage.NB_VBO);
		if (this.vbo == null) {
			Log.error("can not instanciate VBO ...");
			return;
		}
		// TO facilitate some debugs we add a name of the VBO:
		this.vbo.setName("[VBO] of ewol::compositing::Image");
		setSource(uri, size);
		loadProgram();
	}
	
	/**
	 * clear alll tre registered element in the current element
	 */
	@Override
	public void clear() {
		// call upper class
		super.clear();
		// reset Buffer :
		this.vbo.clear();
		// reset temporal variables :
		this.position = new Vector3f(0, 0, 0);
		this.clippingPosStart = new Vector3f(0, 0, 0);
		this.clippingPosStop = new Vector3f(0, 0, 0);
		this.clippingEnable = false;
		this.color = Color.WHITE;
		this.angle = 0;
	}
	
	/**
	 * draw All the refistered text in the current element on openGL
	 * @param disableDepthTest disable the Depth test for display
	 */
	@Override
	public void draw(final boolean disableDepthTest) {
		/*
		if (this.VBO.bufferSize(this.vboIdCoord) <= 0) {
			//Log.warning("Nothink to draw...");
			return;
		}
		*/
		if (this.resource == null && this.resourceImage == null) {
			// this is a normale case ... the user can choice to have no image ...
			return;
		}
		if (this.oGLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		//Log.warning("Display image : " + this.VBO.bufferSize(this.vboIdCoord));
		if (disableDepthTest) {
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		} else {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
		}
		// set Matrix : translation/positionMatrix
		final Matrix4f tmpMatrix = OpenGL.getMatrix().multiply(this.matrixApply);
		this.oGLprogram.use();
		this.oGLprogram.uniformMatrix(this.oGLMatrix, tmpMatrix);
		// TextureID
		if (this.resourceImage != null) {
			this.resourceImage.bindForRendering(0);
		} else if (this.resource != null) {
			this.resource.bindForRendering(0);
		} else {
			Log.error("FONT type error Request normal and display distance field ...");
		}
		// position:
		this.oGLprogram.sendAttributePointer(this.oGLPosition, this.vbo, CompositingImage.VBO_ID_COORD);
		// Texture:
		this.oGLprogram.sendAttributePointer(this.oGLtexture, this.vbo, CompositingImage.VBO_ID_COORD_TEX);
		// color:
		this.oGLprogram.sendAttributePointer(this.oGLColor, this.vbo, CompositingImage.VBO_ID_COLOR);
		// Request the draw of the elements:
		OpenGL.drawArrays(RenderMode.TRIANGLE, 0, this.vbo.bufferSize(CompositingImage.VBO_ID_COORD));
		
		this.oGLprogram.unUse();
	}
	
	@Override
	public void flush() {
		
		this.vbo.setVboData(CompositingImage.VBO_ID_COORD, this.vboDataCoords);
		this.vbo.setVboData(CompositingImage.VBO_ID_COORD_TEX, this.vboDataCoordsTex);
		this.vbo.setVboData(CompositingImage.VBO_ID_COLOR, this.vboDataColors);
		
		this.vbo.flush();
	}
	
	/**
	 * get the current display position (sometime needed in the gui control)
	 * @return the current position.
	 */
	public Vector3f getPos() {
		return this.position;
	}
	
	/**
	 * get the source image registered size in the file (<0 when multiple size image)
	 * @return tre image registered size
	 */
	public Vector2i getRealSize() {
		if (this.resource == null && this.resourceImage == null) {
			return Vector2i.ZERO;
		}
		if (this.resource != null) {
			return this.resource.getRealSize();
		}
		return this.resourceImage.getUsableSize();
	}
	
	/**
	 * Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
	 * @return the validity od the resources.
	 */
	public boolean hasSources() {
		return this.resource != null;
	}
	
	/**
	 * load the openGL program and get all the ID needed
	 */
	private void loadProgram() {
		// get the shader resource:
		this.oGLPosition = 0;
		this.oGLprogram = ResourceProgram.create(new Uri("DATA", "textured3D.vert", "ewol"), new Uri("DATA", "textured3D.frag", "ewol"));
		if (this.oGLprogram != null) {
			this.oGLPosition = this.oGLprogram.getAttribute("in_coord3d");
			this.oGLColor = this.oGLprogram.getAttribute("in_color");
			this.oGLtexture = this.oGLprogram.getAttribute("in_texture2d");
			this.oGLMatrix = this.oGLprogram.getUniform("in_MatrixTransformation");
			this.oGLtexID = this.oGLprogram.getUniform("in_texID");
		}
	}
	
	public void print(final Vector2f size) {
		printPart(size, new Vector2f(0, 0), new Vector2f(1, 1));
	}
	
	/**
	 * add a compleate of the image to display with the requested size
	 * @param size size of the output image
	 */
	public void print(final Vector2i size) {
		print(new Vector2f(size.x(), size.y()));
	}
	
	/**
	 * add a part of the image to display with the requested size
	 * @param size size of the output image
	 * @param sourcePosStart Start position in the image [0..1] (can be bigger but this repeate the image).
	 * @param sourcePosStop Stop position in the image [0..1] (can be bigger but this repeate the image).
	 */
	public void printPart(final Vector2f size, final Vector2f sourcePosStartIn, final Vector2f sourcePosStopIn) {
		if (this.resource == null) {
			return;
		}
		final Vector2f openGLSize = new Vector2f(this.resource.getOpenGlSize().x(), this.resource.getOpenGlSize().y());
		final Vector2i usefullSize = this.resource.getUsableSize();
		final Vector2f ratio = new Vector2f(usefullSize.x() / openGLSize.x(), usefullSize.y() / openGLSize.y());
		final Vector2f sourcePosStart = sourcePosStartIn.multiply(ratio);
		final Vector2f sourcePosStop = sourcePosStopIn.multiply(ratio);
		Log.verbose("     openGLSize=" + openGLSize + " usableSize=" + usefullSize + " start=" + sourcePosStart + " stop=" + sourcePosStop);
		
		if (this.angle == 0.0f) {
			Vector3f point = this.position;
			int indexElem = 0;
			
			Vector2f tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStop.x(), sourcePosStop.y());
			point = new Vector3f(this.position.x() + size.x(), this.position.y(), 0);
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStop.x(), sourcePosStart.y());
			point = new Vector3f(this.position.x() + size.x(), this.position.y() + size.y(), 0);
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStart.x(), sourcePosStart.y());
			point = new Vector3f(this.position.x(), this.position.y() + size.y(), 0);
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
			point = new Vector3f(this.position.x(), this.position.y(), 0);
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			
			return;
		}
		
		final Vector3f center = this.position.add(new Vector3f(size.x(), size.y(), 0)).divide(2.0f);
		
		final Vector3f limitedSize = new Vector3f(size.x() * 0.5f, size.y() * 0.5f, 0.0f);
		
		Vector3f point = new Vector3f(0, 0, 0);
		
		Vector2f tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
		
		int indexElem = 0;
		
		point = new Vector3f(-limitedSize.x(), -limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStop.x(), sourcePosStop.y());
		point = new Vector3f(limitedSize.x(), -limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStop.x(), sourcePosStart.y());
		point = new Vector3f(limitedSize.x(), limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStart.x(), sourcePosStart.y());
		point = new Vector3f(-limitedSize.x(), limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
		point = new Vector3f(-limitedSize.x(), -limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		
	}
	
	/**
	 * set a unique rotation of this element (not set in the rotate Generic system)
	 * @param angleRad Angle to set in radiant.
	 */
	public void setAngle(final float angleRad) {
		this.angle = angleRad;
	}
	
	void setClipping(final Vector2f pos, final Vector2f posEnd) {
		setClipping(new Vector3f(pos.x(), pos.y(), 0), new Vector3f(posEnd.x(), posEnd.y(), 0));
	}
	
	/**
	 * Request a clipping area for the text (next draw only)
	 * @param pos Start position of the clipping
	 * @param posEnd End position of the clipping
	 */
	public void setClipping(final Vector3f pos, final Vector3f posEnd) {
		this.clippingPosStart = FMath.min(pos, posEnd);
		this.clippingPosStop = FMath.max(pos, posEnd);
		this.clippingEnable = true;
	}
	
	/**
	 * enable/Disable the clipping (without lose the current clipping position)
	 * newMode The new status of the clipping
	 */
	public void setClippingMode(final boolean newMode) {
		this.clippingEnable = newMode;
	}
	
	public void setClippingWidth(final Vector2f pos, final Vector2f width) {
		setClippingWidth(new Vector3f(pos.x(), pos.y(), 0), new Vector3f(width.x(), width.y(), 0));
	}
	
	/**
	 * Request a clipping area for the text (next draw only)
	 * @param pos Start position of the clipping
	 * @param width Width size of the clipping
	 */
	public void setClippingWidth(final Vector3f pos, final Vector3f width) {
		setClipping(pos, pos.add(width));
	}
	
	/**
	 * set the Color of the current foreground font
	 * @param color Color to set on foreground (for next print)
	 */
	public void setColor(final Color color) {
		this.color = color;
	}
	
	public void setPos(final Vector2f pos) {
		setPos(new Vector3f(pos.x(), pos.y(), 0));
	}
	
	/**
	 * set position for the next text writen
	 * @param pos Position of the text (in 3D)
	 */
	public void setPos(final Vector3f pos) {
		this.position = pos;
	}
	
	public void setRelPos(final Vector2f pos) {
		setRelPos(new Vector3f(pos.x(), pos.y(), 0));
	}
	
	/**
	 * set relative position for the next text writen
	 * @param pos ofset apply of the text (in 3D)
	 */
	public void setRelPos(final Vector3f pos) {
		this.position = this.position.add(pos);
	}
	
	public void setSource(final ImageByteRGBA image) {
		clear();
		this.filename = null;
		this.requestSize = image.getSize();
		this.resourceImage = new ResourceTexture2();
		this.resourceImage.set(image);
	}
	
	/**
	 * change the image Source  == > can not be done to display 2 images at the same time ...
	 * @param uri New file of the Image
	 */
	public void setSource(final Uri uri) {
		setSource(uri, 32);
	}
	
	public void setSource(final Uri uri, final int size) {
		setSource(uri, new Vector2i(size, size));
	}
	
	public void setSource(final Uri uri, final Vector2i size) {
		clear();
		if (this.filename == uri && this.requestSize.x() == size.x() && this.requestSize.y() == size.y()) {
			// Nothing to do ...
			return;
		}
		final ResourceTextureFile resource = this.resource;
		final ResourceTexture2 resourceTex = this.resourceImage;
		this.filename = uri;
		this.requestSize = size;
		this.resource = null;
		this.resourceImage = null;
		
		final Vector2i tmpSize = new Vector2i(size.x(), size.y());
		// note that no image can be loaded...
		if (!uri.isEmpty()) {
			// link to new one
			this.resource = ResourceTextureFile.create(this.filename, tmpSize);
			if (this.resource == null) {
				Log.error("Can not get Image resource");
			}
		}
		if (this.resource == null && this.resourceImage == null) {
			if (resource != null) {
				Log.warning("Retrive previous resource");
				this.resource = resource;
			}
			if (resourceTex != null) {
				Log.warning("Retrive previous resource (image)");
				this.resourceImage = resourceTex;
			}
		}
	}
	
}
