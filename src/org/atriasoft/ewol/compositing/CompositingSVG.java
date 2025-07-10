/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import org.atriasoft.egami.ImageByte;
import org.atriasoft.egami.ImageByteRGBA;
import org.atriasoft.egami.ToolImage;
import org.atriasoft.esvg.EsvgDocument;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.backend3d.OpenGL.RenderMode;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.gale.resource.ResourceTexture2;
import org.atriasoft.gale.resource.ResourceTextureFile;
import org.atriasoft.gale.resource.ResourceVirtualArrayObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class CompositingSVG extends Compositing {
	private static final Logger LOGGER = LoggerFactory.getLogger(CompositingSVG.class);
	public static final int NB_VBO = 3;
	public static final int SIZE_AUTO = 0;
	public static final int VBO_ID_COLOR = 2;
	// VBO table property:
	public static final int VBO_ID_COORD = 0;
	public static final int VBO_ID_COORD_TEX = 1;
	private float angle = 0; //!< Angle to set at the axes
	private Color color = new Color(1, 1, 1); //!< The text foreground color
	private String svgData;
	protected int oGLMatrixProjection = -1; //!< openGL id on the element (Projection matrix)
	protected int oGLMatrixTransformation = -1; //!< openGL id on the element (transformation matrix)
	protected int oGLMatrixView = -1; //!< openGL id on the element (view matrix)
	private ResourceProgram oGLprogram = null; //!< pointer on the opengl display program
	private Vector2f position = Vector2f.ZERO; //!< The current position to draw
	private Vector2i requestSize = new Vector2i(2, 2);
	
	private ResourceTexture2 resourceImage = null; //!< texture resources
	private ResourceVirtualArrayObject vbo = null;
	
	private Color[] vboDataColors = null;
	private Vector2f[] vboDataCoords = null;
	private Vector2f[] vboDataCoordsTex = null;
	
	public CompositingSVG() {
		this("<svg></svg>", CompositingSVG.SIZE_AUTO);
	}
	
	public CompositingSVG(final String data, final int size) {
		this.svgData = data;
		// Create the VBO:
		this.vbo = ResourceVirtualArrayObject.createDynamic();
		if (this.vbo == null) {
			LOGGER.error("can not instanciate VBO ...");
			return;
		}
		// TO facilitate some debugs we add a name of the VBO:
		this.vbo.setName("[VBO] of " + this.getClass().getCanonicalName());
		setSource(data, size);
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
		this.position = Vector2f.ZERO;
		this.color = Color.WHITE;
		this.angle = 0;
	}
	
	/**
	 * draw All the registered text in the current element on openGL
	 * @param disableDepthTest disable the Depth test for display
	 */
	@Override
	public void draw(final boolean disableDepthTest) {
		/*
		if (this.VBO.bufferSize(this.vboIdCoord) <= 0) {
			//LOGGER.warn("Nothink to draw...");
			return;
		}
		*/
		if (this.resourceImage == null) {
			return;
		}
		if (this.oGLprogram == null) {
			LOGGER.error("No shader ...");
			return;
		}
		//LOGGER.warn("Display image : " + this.VBO.bufferSize(this.vboIdCoord));
		if (disableDepthTest) {
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		} else {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
		}
		// set Matrix : translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		this.oGLprogram.use();
		this.vbo.bindForRendering();
		this.oGLprogram.uniformMatrix(this.oGLMatrixProjection, projMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrixTransformation, this.matrixApply);
		this.oGLprogram.uniformMatrix(this.oGLMatrixView, camMatrix);
		// TextureID
		this.resourceImage.bindForRendering(0);
		this.vbo.renderArrays(RenderMode.TRIANGLE);
		this.vbo.unBindForRendering();
		this.oGLprogram.unUse();
	}
	
	@Override
	public void flush() {
		this.vbo.setPosition(this.vboDataCoords);
		this.vbo.setTextureCoordinate(this.vboDataCoordsTex);
		this.vbo.setColors(this.vboDataColors);
		this.vbo.setVertexCount(this.vboDataCoords.length);
		this.vbo.flush();
	}
	
	/**
	 * get the current display position (sometime needed in the gui control)
	 * @return the current position.
	 */
	public Vector2f getPos() {
		return this.position;
	}
	
	/**
	 * get the source image registered size in the file (<0 when multiple size image)
	 * @return tre image registered size
	 */
	public Vector2i getRealSize() {
		if (this.resourceImage == null) {
			return Vector2i.ZERO;
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
		this.oGLprogram = ResourceProgram.create(new Uri("DATA", "textured3D.vert", "ewol"),
				new Uri("DATA", "textured3D.frag", "ewol"));
		if (this.oGLprogram != null) {
			this.oGLMatrixTransformation = this.oGLprogram.getUniform("in_matrixTransformation");
			this.oGLMatrixProjection = this.oGLprogram.getUniform("in_matrixProjection");
			this.oGLMatrixView = this.oGLprogram.getUniform("in_matrixView");
		}
	}
	
	public void print(final Vector2f size) {
		printPart(size, Vector2f.ZERO, Vector2f.ONE);
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
		LOGGER.trace("     openGLSize=" + openGLSize + " usableSize=" + usefullSize + " start=" + sourcePosStart
				+ " stop=" + sourcePosStop);

		this.vboDataColors = new Color[6];
		this.vboDataCoords = new Vector2f[6];
		this.vboDataCoordsTex = new Vector2f[6];
		
		if (this.angle == 0.0f) {
			Vector2f point = this.position;
			int indexElem = 0;
			
			Vector2f tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStop.x(), sourcePosStop.y());
			point = new Vector2f(this.position.x() + size.x(), this.position.y(), 0);
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStop.x(), sourcePosStart.y());
			point = new Vector2f(this.position.x() + size.x(), this.position.y() + size.y(), 0);
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStart.x(), sourcePosStart.y());
			point = new Vector2f(this.position.x(), this.position.y() + size.y(), 0);
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
			point = new Vector2f(this.position.x(), this.position.y(), 0);
			this.vboDataCoords[indexElem] = point;
			this.vboDataCoordsTex[indexElem] = tex;
			this.vboDataColors[indexElem] = this.color;
			
			return;
		}
		
		final Vector2f center = this.position.add(new Vector2f(size.x(), size.y(), 0)).divide(2.0f);
		
		final Vector2f limitedSize = new Vector2f(size.x() * 0.5f, size.y() * 0.5f, 0.0f);
		
		Vector2f point = Vector2f.ZERO;
		
		Vector2f tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
		
		int indexElem = 0;
		
		point = new Vector2f(-limitedSize.x(), -limitedSize.y(), 0);
		point = point.rotateNew(new Vector2f(0, 0, 1), this.angle).add(center);
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStop.x(), sourcePosStop.y());
		point = new Vector2f(limitedSize.x(), -limitedSize.y(), 0);
		point = point.rotateNew(new Vector2f(0, 0, 1), this.angle).add(center);
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStop.x(), sourcePosStart.y());
		point = new Vector2f(limitedSize.x(), limitedSize.y(), 0);
		point = point.rotateNew(new Vector2f(0, 0, 1), this.angle).add(center);
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStart.x(), sourcePosStart.y());
		point = new Vector2f(-limitedSize.x(), limitedSize.y(), 0);
		point = point.rotateNew(new Vector2f(0, 0, 1), this.angle).add(center);
		this.vboDataCoords[indexElem] = point;
		this.vboDataCoordsTex[indexElem] = tex;
		this.vboDataColors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
		point = new Vector2f(-limitedSize.x(), -limitedSize.y(), 0);
		point = point.rotateNew(new Vector2f(0, 0, 1), this.angle).add(center);
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
	
	/**
	 * set the Color of the current foreground font
	 * @param color Color to set on foreground (for next print)
	 */
	public void setColor(final Color color) {
		this.color = color;
	}
	
	public void setPos(final Vector2f pos) {
		setPos(new Vector2f(pos.x(), pos.y(), 0));
	}
	
	/**
	 * set position for the next text writen
	 * @param pos Position of the text (in 3D)
	 */
	public void setPos(final Vector2f pos) {
		this.position = pos;
	}
	
	public void setRelPos(final Vector2f pos) {
		setRelPos(new Vector2f(pos.x(), pos.y(), 0));
	}
	
	/**
	 * set relative position for the next text writen
	 * @param pos ofset apply of the text (in 3D)
	 */
	public void setRelPos(final Vector2f pos) {
		this.position = this.position.add(pos);
	}
	
	public void setSource(final ImageByteRGBA image) {
		clear();
		this.svgData = null;
		this.requestSize = image.getSize();
		this.resourceImage = new ResourceTexture2();
		this.resourceImage.set(image);
	}
	
	public void setSource(final String data) {
		setSource(data, 32);
	}
	
	public void setSource(final String data, final int size) {
		setSource(data, new Vector2i(size, size));
	}
	
	public void setSource(final String data, final Vector2i size) {
		clear();

		final EsvgDocument doc = new EsvgDocument();
		doc.parse(data);
		final ImageByte tmp = ToolImage.convertImageByte(doc.renderImageFloatRGBA(size));
		if (tmp == null) {
			LOGGER.error("Can not load the Raw SVG ... ");
			return;
		}
		this.resourceImage.set(tmp);
		
		if (this.svgData.equals(data) && this.requestSize.x() == size.x() && this.requestSize.y() == size.y()) {
			// Nothing to do ...
			return;
		}
		final ResourceTextureFile resource = this.resource;
		final ResourceTexture2 resourceTex = this.resourceImage;
		this.svgData = data;
		this.requestSize = size;
		this.resource = null;
		this.resourceImage = null;
		
		final Vector2i tmpSize = new Vector2i(size.x(), size.y());

		// link to new one
		this.resource = ResourceTexture2.create();
		if (this.resource == null) {
			LOGGER.error("Can not get Image resource");
		}
	}
	
}
