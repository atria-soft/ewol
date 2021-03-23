/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import org.atriasoft.egami.Image;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.ResourceTexture2;
import org.atriasoft.ewol.resource.ResourceTextureFile;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.backend3d.OpenGL.RenderMode;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.gale.resource.ResourceVirtualBufferObject;

class CompositingImage extends Compositing {
	public static final int sizeAuto = 0;
	// VBO table property:
	public static final int vboIdCoord = 0;
	public static final int vboIdCoordTex = 1;
	public static final int vboIdColor = 2;
	public static final int NB_VBO = 3;
	private Uri filename;
	private Vector2i requestSize = new Vector2i(2, 2);
	private Vector3f position = new Vector3f(0, 0, 0); //!< The current position to draw
	private Vector3f clippingPosStart = new Vector3f(0, 0, 0); //!< Clipping start position
	private Vector3f clippingPosStop = new Vector3f(0, 0, 0); //!< Clipping stop position
	private boolean clippingEnable = true; //!< true if the clipping must be activated
	
	private Color color = new Color(1, 1, 1); //!< The text foreground color
	private float angle = 0; //!< Angle to set at the axes
	private ResourceProgram GLprogram = null; //!< pointer on the opengl display program
	private int GLPosition = -1; //!< openGL id on the element (vertex buffer)
	private int GLMatrix = -1; //!< openGL id on the element (transformation matrix)
	private int GLColor = -1; //!< openGL id on the element (color buffer)
	private int GLtexture = -1; //!< openGL id on the element (Texture position)
	private int GLtexID = -1; //!< openGL id on the element (texture ID)
	
	private ResourceTextureFile resource = null; //!< texture resources
	private ResourceTexture2 resourceImage = null; //!< texture resources
	private ResourceVirtualBufferObject VBO = null;
	
	/**
	 * generic ructor
	 * @param _uri URI of the file that might be loaded
	 * @param _df enable distance field mode
	 * @param _size for the image when Verctorial image loading is requested
	 */
	public CompositingImage() {
		this(new Uri(""), sizeAuto);
	}
	
	public CompositingImage(final Uri _uri, final int _size) {
		this.filename = _uri;
		// Create the VBO:
		this.VBO = ResourceVirtualBufferObject.create(NB_VBO);
		if (this.VBO == null) {
			Log.error("can not instanciate VBO ...");
			return;
		}
		// TO facilitate some debugs we add a name of the VBO:
		this.VBO.setName("[VBO] of ewol::compositing::Image");
		setSource(_uri, _size);
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
		this.VBO.clear();
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
	 * @param _disableDepthTest disable the Depth test for display
	 */
	@Override
	public void draw(final boolean _disableDepthTest) {
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
		if (this.GLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		//Log.warning("Display image : " + this.VBO.bufferSize(this.vboIdCoord));
		if (_disableDepthTest == true) {
			OpenGL.disable(OpenGL.Flag.flag_depthTest);
		} else {
			OpenGL.enable(OpenGL.Flag.flag_depthTest);
		}
		// set Matrix : translation/positionMatrix
		final Matrix4f tmpMatrix = OpenGL.getMatrix().multiply(this.matrixApply);
		this.GLprogram.use();
		this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
		// TextureID
		if (this.resourceImage != null) {
			this.resourceImage.bindForRendering(0);
		} else if (this.resource != null) {
			this.resource.bindForRendering(0);
		} else {
			Log.error("FONT type error Request normal and display distance field ...");
		}
		// position:
		this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, CompositingImage.vboIdCoord);
		// Texture:
		this.GLprogram.sendAttributePointer(this.GLtexture, this.VBO, CompositingImage.vboIdCoordTex);
		// color:
		this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, CompositingImage.vboIdColor);
		// Request the draw of the elements:
		OpenGL.drawArrays(RenderMode.triangle, 0, this.VBO.bufferSize(CompositingImage.vboIdCoord));
		
		this.GLprogram.unUse();
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
			return new Vector2i(0, 0);
		}
		if (this.resource != null) {
			return this.resource.getRealSize();
		}
		if (this.resourceImage != null) {
			return this.resourceImage.getUsableSize();
		}
		return new Vector2i(0, 0);
	};
	
	/**
	 * Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
	 * @return the validity od the resources.
	 */
	public boolean hasSources() {
		return this.resource != null;
	};
	
	/**
	 * load the openGL program and get all the ID needed
	 */
	private void loadProgram() {
		// get the shader resource:
		this.GLPosition = 0;
		this.GLprogram = ResourceProgram.create(new Uri("DATA", "textured3D.vert", "ewol"), new Uri("DATA", "textured3D.frag", "ewol"));
		if (this.GLprogram != null) {
			this.GLPosition = this.GLprogram.getAttribute("EW_coord3d");
			this.GLColor = this.GLprogram.getAttribute("EW_color");
			this.GLtexture = this.GLprogram.getAttribute("EW_texture2d");
			this.GLMatrix = this.GLprogram.getUniform("EW_MatrixTransformation");
			this.GLtexID = this.GLprogram.getUniform("EW_texID");
		}
	};
	
	public void print(final Vector2f _size) {
		printPart(_size, new Vector2f(0, 0), new Vector2f(1, 1));
	}
	
	/**
	 * add a compleate of the image to display with the requested size
	 * @param _size size of the output image
	 */
	public void print(final Vector2i _size) {
		print(new Vector2f(_size.x(), _size.y()));
	};
	
	/**
	 * add a part of the image to display with the requested size
	 * @param _size size of the output image
	 * @param _sourcePosStart Start position in the image [0..1] (can be bigger but this repeate the image).
	 * @param _sourcePosStop Stop position in the image [0..1] (can be bigger but this repeate the image).
	 */
	public void printPart(final Vector2f _size, final Vector2f _sourcePosStart, final Vector2f _sourcePosStop) {
		if (this.resource == null) {
			return;
		}
		final Vector2f openGLSize = new Vector2f(this.resource.getOpenGlSize().x(), this.resource.getOpenGlSize().y());
		final Vector2i usefullSize = this.resource.getUsableSize();
		final Vector2f ratio = new Vector2f(usefullSize.x() / openGLSize.x(), usefullSize.y() / openGLSize.y());
		final Vector2f sourcePosStart = _sourcePosStart.multiply(ratio);
		final Vector2f sourcePosStop = _sourcePosStop.multiply(ratio);
		Log.verbose("     openGLSize=" + openGLSize + " usableSize=" + usefullSize + " start=" + sourcePosStart + " stop=" + sourcePosStop);
		
		if (this.angle == 0.0f) {
			Vector3f point = this.position;
			
			final Vector3f[] coords = new Vector3f[6];
			final Vector2f[] coordsTex = new Vector2f[6];
			final Color[] colors = new Color[6];
			int indexElem = 0;
			
			Vector2f tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
			coords[indexElem] = point;
			coordsTex[indexElem] = tex;
			colors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStop.x(), sourcePosStop.y());
			point = new Vector3f(this.position.x() + _size.x(), this.position.y(), 0);
			coords[indexElem] = point;
			coordsTex[indexElem] = tex;
			colors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStop.x(), sourcePosStart.y());
			point = new Vector3f(this.position.x() + _size.x(), this.position.y() + _size.y(), 0);
			coords[indexElem] = point;
			coordsTex[indexElem] = tex;
			colors[indexElem] = this.color;
			indexElem++;
			
			coords[indexElem] = point;
			coordsTex[indexElem] = tex;
			colors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStart.x(), sourcePosStart.y());
			point = new Vector3f(this.position.x(), this.position.y() + _size.y(), 0);
			coords[indexElem] = point;
			coordsTex[indexElem] = tex;
			colors[indexElem] = this.color;
			indexElem++;
			
			tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
			point = new Vector3f(this.position.x(), this.position.y(), 0);
			coords[indexElem] = point;
			coordsTex[indexElem] = tex;
			colors[indexElem] = this.color;
			indexElem++;
			
			this.VBO.setVboData(CompositingImage.vboIdCoord, coords);
			this.VBO.setVboData(CompositingImage.vboIdCoordTex, coordsTex);
			this.VBO.setVboData(CompositingImage.vboIdColor, colors);
			
			this.VBO.flush();
			return;
		}
		
		final Vector3f center = this.position.add(new Vector3f(_size.x(), _size.y(), 0)).divide(2.0f);
		
		final Vector3f limitedSize = new Vector3f(_size.x() * 0.5f, _size.y() * 0.5f, 0.0f);
		
		Vector3f point = new Vector3f(0, 0, 0);
		
		Vector2f tex = new Vector2f(_sourcePosStart.x(), sourcePosStop.y());
		
		final Vector3f[] coords = new Vector3f[6];
		final Vector2f[] coordsTex = new Vector2f[6];
		final Color[] colors = new Color[6];
		int indexElem = 0;
		
		point = new Vector3f(-limitedSize.x(), -limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		coords[indexElem] = point;
		coordsTex[indexElem] = tex;
		colors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStop.x(), sourcePosStop.y());
		point = new Vector3f(limitedSize.x(), -limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		coords[indexElem] = point;
		coordsTex[indexElem] = tex;
		colors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStop.x(), sourcePosStart.y());
		point = new Vector3f(limitedSize.x(), limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		coords[indexElem] = point;
		coordsTex[indexElem] = tex;
		colors[indexElem] = this.color;
		indexElem++;
		
		coords[indexElem] = point;
		coordsTex[indexElem] = tex;
		colors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStart.x(), sourcePosStart.y());
		point = new Vector3f(-limitedSize.x(), limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		coords[indexElem] = point;
		coordsTex[indexElem] = tex;
		colors[indexElem] = this.color;
		indexElem++;
		
		tex = new Vector2f(sourcePosStart.x(), sourcePosStop.y());
		point = new Vector3f(-limitedSize.x(), -limitedSize.y(), 0);
		point = point.rotateNew(new Vector3f(0, 0, 1), this.angle).add(center);
		coords[indexElem] = point;
		coordsTex[indexElem] = tex;
		colors[indexElem] = this.color;
		indexElem++;
		
		this.VBO.setVboData(CompositingImage.vboIdCoord, coords);
		this.VBO.setVboData(CompositingImage.vboIdCoordTex, coordsTex);
		this.VBO.setVboData(CompositingImage.vboIdColor, colors);
		
		this.VBO.flush();
	};
	
	/**
	 * set a unique rotation of this element (not set in the rotate Generic system)
	 * @param _angle Angle to set in radiant.
	 */
	public void setAngle(final float _angleRad) {
		this.angle = _angleRad;
	};
	
	void setClipping(final Vector2f _pos, final Vector2f _posEnd) {
		setClipping(new Vector3f(_pos.x(), _pos.y(), 0), new Vector3f(_posEnd.x(), _posEnd.y(), 0));
	};
	
	/**
	 * Request a clipping area for the text (next draw only)
	 * @param _pos Start position of the clipping
	 * @param _posEnd End position of the clipping
	 */
	public void setClipping(final Vector3f _pos, final Vector3f _posEnd) {
		this.clippingPosStart = FMath.min(_pos, _posEnd);
		this.clippingPosStop = FMath.max(_pos, _posEnd);
		this.clippingEnable = true;
	}
	
	/**
	 * enable/Disable the clipping (without lose the current clipping position)
	 * _newMode The new status of the clipping
	 */
	public void setClippingMode(final boolean _newMode) {
		this.clippingEnable = _newMode;
	};
	
	public void setClippingWidth(final Vector2f _pos, final Vector2f _width) {
		setClippingWidth(new Vector3f(_pos.x(), _pos.y(), 0), new Vector3f(_width.x(), _width.y(), 0));
	};
	
	/**
	 * Request a clipping area for the text (next draw only)
	 * @param _pos Start position of the clipping
	 * @param _width Width size of the clipping
	 */
	public void setClippingWidth(final Vector3f _pos, final Vector3f _width) {
		setClipping(_pos, _pos.add(_width));
	}
	
	/**
	 * set the Color of the current foreground font
	 * @param _color Color to set on foreground (for next print)
	 */
	public void setColor(final Color _color) {
		this.color = _color;
	};
	
	public void setPos(final Vector2f _pos) {
		setPos(new Vector3f(_pos.x(), _pos.y(), 0));
	}
	
	/**
	 * set position for the next text writen
	 * @param _pos Position of the text (in 3D)
	 */
	public void setPos(final Vector3f _pos) {
		this.position = _pos;
	}
	
	public void setRelPos(final Vector2f _pos) {
		setRelPos(new Vector3f(_pos.x(), _pos.y(), 0));
	}
	
	/**
	 * set relative position for the next text writen
	 * @param _pos ofset apply of the text (in 3D)
	 */
	public void setRelPos(final Vector3f _pos) {
		this.position.add(_pos);
	}
	
	public void setSource(final Image _image) {
		clear();
		this.filename = null;
		this.requestSize = _image.getSize();
		this.resourceImage = new ResourceTexture2();
		this.resourceImage.set(_image);
	}
	
	/**
	 * change the image Source  == > can not be done to display 2 images at the same time ...
	 * @param _uri New file of the Image
	 * @param _size for the image when Verctorial image loading is requested
	 */
	public void setSource(final Uri _uri) {
		setSource(_uri, 32);
	}
	
	public void setSource(final Uri _uri, final int _size) {
		setSource(_uri, new Vector2i(_size, _size));
	}
	
	public void setSource(final Uri _uri, final Vector2i _size) {
		clear();
		if (this.filename == _uri && this.requestSize.x() == _size.x() && this.requestSize.y() == _size.y()) {
			// Nothing to do ...
			return;
		}
		final ResourceTextureFile resource = this.resource;
		final ResourceTexture2 resourceTex = this.resourceImage;
		this.filename = _uri;
		this.requestSize = _size;
		this.resource = null;
		this.resourceImage = null;
		
		final Vector2i tmpSize = new Vector2i(_size.x(), _size.y());
		// note that no image can be loaded...
		if (_uri.isEmpty() == false) {
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
