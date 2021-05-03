package org.atriasoft.ewol.compositing;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.ResourceConfigFile;
import org.atriasoft.ewol.resource.ResourceTextureFile;
import org.atriasoft.ewol.resource.TextureFilter;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.backend3d.OpenGL.Flag;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.loader3d.resources.ResourceStaticMeshObjBynamic;

/**
 * @brief the Shaper system is a basic theme configuration for every widget, it corresponds at a background display described by a pool of files
 */
// TODO : load image
// TODO : Abstaraction between states (call by name and the system greate IDs
public class GuiShape extends Compositing {
	private static final int SHAPER_POS_BOTTOM = 3;
	private static final int SHAPER_POS_LEFT = 0;
	private static final int SHAPER_POS_RIGHT = 2;
	private static final int SHAPER_POS_TOP = 1;
	private int confIdChangeTime = -1; //!< ConfigFile padding transition time property
	private int confIdImagePaletteFile = -1; //!< Palette of the display
	private int confIdImagePaletteMode = -1; //!< Mode to load the image file ... 'nearest', 'linear'
	private final int[] confIdPaddingIn = new int[4]; //!< Padding in property : X-left X-right Y-top Y-buttom
	private final int[] confIdPaddingOut = new int[4]; //!< Padding out property : X-left X-right Y-top Y-buttom
	// External theme config:
	private ResourceConfigFile config = null; //!< pointer on the config file resources
	
	private int confObjectFile = -1; //!< Config Id of the object file to display
	private int confProgramFileFrag = -1; //!< ConfigFile opengGl program Name
	private int confProgramFileVert = -1; //!< ConfigFile opengGl program Name
	private final List<Vector2i> listAssiciatedId = new ArrayList<>(); //!< Correlation ID between ColorProperty (Y) and OpenGL Program (X)
	// internal needed data :
	private int nextStatusRequested = -1; //!< when status is changing, this represent the next step of it
	private int oGLMatrixProjection = -1; //!< openGL id on the element (Projection matrix)
	private int oGLMatrixTransformation = -1; //!< openGL id on the element (transformation matrix)
	private int oGLMatrixView = -1; //!< openGL id on the element (view matrix)
	// openGL shaders programs:
	private ResourceProgram oGLprogram = null; //!< pointer on the opengl display program
	private int oGLtexID = -1; //!< openGL id on the element (texture image)
	// For the Image :
	private ResourceTextureFile resourceTexture = null; //!< texture resources (for the image)
	private ResourceStaticMeshObjBynamic shape = null;
	private Padding sizeObject = Padding.ZERO;
	private int stateActivate = -1; //!< Activate state of the element
	private final int stateNew = -1; //!< destination state
	private final int stateOld = -1; //!< previous state
	private final float stateTransition = 0; //!< working state between 2 states
	private Matrix4f transform = Matrix4f.IDENTITY;
	private Uri uri; //!< Name of the configuration of the shaper.
	private float[] verticesToModify = {};
	
	/**
				 * @brief generic constructor
				 * @param _uri URI of the file that might be loaded
				 */
	public GuiShape(final Uri uri) {
		this.uri = uri;
		for (int iii = 0; iii < 4; ++iii) {
			this.confIdPaddingOut[iii] = -1;
			this.confIdPaddingIn[iii] = -1;
		}
		loadProgram();
	}
	
	/**
	 * @brief change the current status in an other
	 * @param _newStatusId the next new status requested
	 * @return true The widget must call this fuction periodicly (and redraw itself)
	 * @return false No need to request the periodic call.
	 */
	public boolean changeStatusIn(final int newStatusId) {
		if (newStatusId != this.stateNew) {
			this.nextStatusRequested = newStatusId;
			return true;
		}
		if (this.nextStatusRequested != -1 || this.stateNew != this.stateOld) {
			return true;
		}
		return false;
	}
	
	/**
	 * @brief clear alll tre registered element in the current element
	 */
	@Override
	public void clear() {}
	
	/**
	 * @brief draw All the refistered text in the current element on openGL
	 */
	@Override
	public void draw(final boolean disableDepthTest) {
		if (this.config == null) {
			// this is a normal case ... the user can choice to have no config basic file ...
			return;
		}
		if (this.oGLprogram == null) {
			Log.error("No shader ...");
		}
		OpenGL.enable(Flag.flag_depthTest);
		// set Matrix : translation/positionMatrix
		Matrix4f projMatrix = OpenGL.getMatrix();
		Matrix4f camMatrix = OpenGL.getCameraMatrix();
		Matrix4f tmpMatrix = this.matrixApply.multiply(this.transform);
		this.oGLprogram.use();
		this.shape.bindForRendering();
		this.oGLprogram.uniformMatrix(this.oGLMatrixProjection, projMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrixTransformation, tmpMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrixView, camMatrix);
		// Texture:
		this.oGLprogram.setTexture0(this.oGLtexID, this.resourceTexture.getRendererId());
		// Request the draw of the elements:
		this.shape.render();
		
		this.shape.unBindForRendering();
		this.oGLprogram.unUse();
		OpenGL.disable(Flag.flag_depthTest);
	}
	
	@Override
	public void flush() {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * @brief get the padding declared by the user in the config file
	 * @return the padding property
	 */
	public Padding getBorder() {
		return this.sizeObject;
	}
	
	/**
	 * @brief get the current displayed status of the shaper
	 * @return The Status Id
	 */
	public int getCurrentDisplayedStatus() {
		return this.stateNew;
	}
	
	/**
	 * @brief get the next displayed status of the shaper
	 * @return The next status Id (-1 if no status in next)
	 */
	public int getNextDisplayedStatus() {
		return this.nextStatusRequested;
	}
	
	/**
	 * @brief get the padding declared by the user in the config file
	 * @return the padding property
	 */
	public Padding getPadding() {
		return getPaddingOut().add(getBorder()).add(getPaddingIn());
	}
	
	public Padding getPaddingIn() {
		Padding out = Padding.ZERO;
		if (this.config != null) {
			out = new Padding(this.config.getNumber(this.confIdPaddingIn[GuiShape.SHAPER_POS_LEFT]), this.config.getNumber(this.confIdPaddingIn[GuiShape.SHAPER_POS_TOP]),
					this.config.getNumber(this.confIdPaddingIn[GuiShape.SHAPER_POS_RIGHT]), this.config.getNumber(this.confIdPaddingIn[GuiShape.SHAPER_POS_BOTTOM]));
		}
		return out;
	}
	
	public Padding getPaddingOut() {
		Padding out = Padding.ZERO;
		if (this.config != null) {
			out = new Padding(this.config.getNumber(this.confIdPaddingOut[GuiShape.SHAPER_POS_LEFT]), this.config.getNumber(this.confIdPaddingOut[GuiShape.SHAPER_POS_TOP]),
					this.config.getNumber(this.confIdPaddingOut[GuiShape.SHAPER_POS_RIGHT]), this.config.getNumber(this.confIdPaddingOut[GuiShape.SHAPER_POS_BOTTOM]));
		}
		return out;
	}
	
	/**
	 * @brief get the shaper file Source
	 * @return the shapper file name
	 */
	public Uri getSource() {
		return this.uri;
	}
	
	/**
	 * @brief get the current trasion status
	 * @return value of the transition status (0.0f when no activity)
	 */
	public float getTransitionStatus() {
		return this.stateTransition;
	}
	
	/**
	 * @brief Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
	 * @return the validity od the resources.
	 */
	public boolean hasSources() {
		return this.oGLprogram != null;
	}
	
	/**
				 * @brief load the openGL program and get all the ID needed
				 */
	private void loadProgram() {
		if (this.uri.isEmpty()) {
			Log.debug("no Shaper set for loading resources ...");
			return;
		}
		this.config = ResourceConfigFile.create(this.uri);
		if (this.config != null) {
			this.confIdPaddingOut[GuiShape.SHAPER_POS_LEFT] = this.config.request("padding-out-left");
			this.confIdPaddingOut[GuiShape.SHAPER_POS_RIGHT] = this.config.request("padding-out-right");
			this.confIdPaddingOut[GuiShape.SHAPER_POS_TOP] = this.config.request("padding-out-top");
			this.confIdPaddingOut[GuiShape.SHAPER_POS_BOTTOM] = this.config.request("padding-out-buttom");
			this.confIdPaddingIn[GuiShape.SHAPER_POS_LEFT] = this.config.request("padding-in-left");
			this.confIdPaddingIn[GuiShape.SHAPER_POS_RIGHT] = this.config.request("padding-in-right");
			this.confIdPaddingIn[GuiShape.SHAPER_POS_TOP] = this.config.request("padding-in-top");
			this.confIdPaddingIn[GuiShape.SHAPER_POS_BOTTOM] = this.config.request("padding-in-buttom");
			this.confIdChangeTime = this.config.request("change-time");
			this.confProgramFileVert = this.config.request("program-vert");
			this.confProgramFileFrag = this.config.request("program-frag");
			this.confObjectFile = this.config.request("object-file");
			this.confIdImagePaletteFile = this.config.request("image-palette");
			this.confIdImagePaletteMode = this.config.request("image-palette-load-mode");
		}
		String objectFile = this.config.getString(this.confObjectFile);
		if (!objectFile.isEmpty()) {
			this.shape = ResourceStaticMeshObjBynamic.create(Uri.valueOf(objectFile));
			this.verticesToModify = this.shape.getVertices();
			float top = 0;
			float bottom = 0;
			float left = 0;
			float right = 0;
			float back = 0;
			float font = 0;
			// estimate size of border:
			for (int iii = 0; iii < this.verticesToModify.length - 2; iii += 3) {
				left = Math.min(left, this.verticesToModify[iii]);
				right = Math.max(right, this.verticesToModify[iii]);
				top = Math.min(top, this.verticesToModify[iii + 1]);
				bottom = Math.max(bottom, this.verticesToModify[iii + 1]);
				back = Math.min(back, this.verticesToModify[iii + 2]);
				font = Math.max(font, this.verticesToModify[iii + 2]);
			}
			this.sizeObject = new Padding(Math.abs(left), Math.abs(top), Math.abs(right), Math.abs(bottom));
		}
		String paletteMode = this.config.getString(this.confIdImagePaletteMode, "linear");
		String paletteFile = this.config.getString(this.confIdImagePaletteFile);
		if (!paletteFile.isEmpty()) {
			this.resourceTexture = ResourceTextureFile.create(Uri.valueOf(paletteFile));
			if (paletteMode.equals("nearest")) {
				this.resourceTexture.setFilterMode(TextureFilter.NEAREST);
			} else {
				this.resourceTexture.setFilterMode(TextureFilter.LINEAR);
			}
		}
		String basicShaderFileVert = this.config.getString(this.confProgramFileVert);
		String basicShaderFileFrag = this.config.getString(this.confProgramFileFrag);
		if (!basicShaderFileVert.isEmpty() && !basicShaderFileFrag.isEmpty()) {
			this.oGLprogram = ResourceProgram.create(Uri.valueOf(basicShaderFileVert), Uri.valueOf(basicShaderFileFrag));
			if (this.oGLprogram != null) {
				this.oGLMatrixTransformation = this.oGLprogram.getUniform("in_matrixTransformation");
				this.oGLMatrixProjection = this.oGLprogram.getUniform("in_matrixProjection");
				this.oGLMatrixView = this.oGLprogram.getUniform("in_matrixView");
				// for the texture ID : 
				this.oGLtexID = this.oGLprogram.getUniform("in_textureBase");
			}
		}
	}
	
	/**
	 * @brief Same as the widfget periodic call (this is for change display)
	 * @param event The current time of the call.
	 * @return true The widget must call this fuction periodicly (and redraw itself)
	 * @return false No need to request the periodic call.
	 */
	public boolean periodicCall(final EventTime event) {
		Log.verbose("call=" + event + "state transition=" + this.stateTransition + " speedTime=" + this.config.getNumber(this.confIdChangeTime));
		return true;
	}
	
	/**
	 * @brief Set activate state of the element
	 * @param _status New activate status
	 */
	public void setActivateState(final int status) {
		this.stateActivate = status;
	}
	
	// @previous
	public void setShape(final Vector2f origin, final Vector2f size) {
		Padding tmp = getPadding();
		setShape(origin, size, origin.add(tmp.left(), tmp.bottom()), size.less(tmp.x(), tmp.y()));
	}
	
	/**
	 * @brief set the shape property:
	 * 
	 *   ********************************************************************************
	 *   *                                                                        _size *
	 *   *                                                                              *
	 *   *        * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *       *
	 *   *                                                                              *
	 *   *        |                                                             |       *
	 *   *             ***************************************************              *
	 *   *        |    *                                                 *      |       *
	 *   *             *                                                 *              *
	 *   *        |    *     * - - - - - - - - - - - - - - - - - - *     *      |       *
	 *   *             *                                _insideSize      *              *
	 *   *        |    *     |                                     |     *      |       *
	 *   *             *                                                 *              *
	 *   *        |    *     |                                     |     *      |       *
	 *   *             *                                                 *              *
	 *   *        |    *     |                                     |     *      |       *
	 *   *             *                                                 *              *
	 *   *        |    *     |                                     |     *      |       *
	 *   *             *                                                 *              *
	 *   *        |    *     |                                     |     *      |       *
	 *   *             *                                                 *              *
	 *   *        |    *     |                                     |     *      |       *
	 *   *             *      _insidePos                                 *              *
	 *   *        |    *     * - - - - - - - - - - - - - - - - - - *     *      |       *
	 *   *             *                                                 *              *
	 *   *        |    ***************************************************      |       *
	 *   *                                                                              *
	 *   *        |                                                             |       *
	 *   *                                                                              *
	 *   *        * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *       *
	 *   *                                                                              *
	 *   *                                                                              *
	 *   ********************************************************************************
	 *   _origin
	 *
	 *
	 * @param center Center of the object
	 * @param size Size of the display
	 */
	public void setShape(final Vector2f origin, final Vector2f size, final Vector2f insidePos, final Vector2f insideSize) {
		//Log.error("Set shape property : origin=" + origin + " size=" + size + "  in-pos=" + insidePos + "  in-size=" + insideSize);
		Vector2f halfSize = insideSize.multiply(0.5f);
		float[] newVertices = Arrays.copyOf(this.verticesToModify, this.verticesToModify.length);
		for (int iii = 0; iii < newVertices.length - 2; iii += 3) {
			if (newVertices[iii] <= 0.0f) {
				newVertices[iii] -= halfSize.x();
			} else {
				newVertices[iii] += halfSize.x();
			}
			if (newVertices[iii + 1] <= 0.0f) {
				newVertices[iii + 1] -= halfSize.y();
			} else {
				newVertices[iii + 1] += halfSize.y();
			}
		}
		this.transform = Matrix4f.createMatrixTranslate(new Vector3f(origin.x() + size.x() * 0.5f, origin.y() + size.y() * 0.5f, 0.0f));
		this.shape.setVertices(newVertices);
		this.shape.flush();
	}
	
	public void setShape(final Vector3f origin, final Vector3f size) {
		Padding tmp = getPadding();
		setShape(origin, size, origin.add(tmp.left(), tmp.bottom(), 0), size.less(tmp.x(), tmp.y(), 0));
	}
	
	public void setShape(final Vector3f origin, final Vector3f size, final Vector3f insidePos, final Vector3f insideSize) {
		Vector3f halfSize = insideSize.multiply(0.5f);
		float[] newVertices = Arrays.copyOf(this.verticesToModify, this.verticesToModify.length);
		for (int iii = 0; iii < newVertices.length - 2; iii += 3) {
			if (newVertices[iii] <= 0.0f) {
				newVertices[iii] -= halfSize.x();
			} else {
				newVertices[iii] += halfSize.x();
			}
			if (newVertices[iii + 1] <= 0.0f) {
				newVertices[iii + 1] -= halfSize.y();
			} else {
				newVertices[iii + 1] += halfSize.y();
			}
			if (newVertices[iii + 2] <= 0.0f) {
				newVertices[iii + 2] -= halfSize.z();
			} else {
				newVertices[iii + 2] += halfSize.z();
			}
		}
		this.transform = Matrix4f.createMatrixTranslate(new Vector3f(origin.x() + size.x() * 0.5f, origin.y() + size.y() * 0.5f, origin.z() + size.z() * 0.5f));
		this.shape.setVertices(newVertices);
	}
	
	/**
	 * @brief change the shaper Source
	 * @param _uri New file of the shaper
	 */
	public void setSource(final Uri uri) {
		clear();
		unLoadProgram();
		this.uri = uri;
		loadProgram();
	}
	
	/**
	 * @brief Change the current state
	 * @param _newState Current state of the configuration
	 * @return true Need redraw.
	 * @return false No need redraw.
	 */
	public boolean setState(final int newState) {
		if (this.stateActivate == newState) {
			return false;
		}
		this.stateActivate = newState;
		return true;
	}
	
	/**
				 * @brief Un-Load the openGL program and get all the ID needed
				 */
	
	private void unLoadProgram() {
		this.oGLprogram = null;
		this.resourceTexture = null;
		this.config = null;
		for (int iii = 0; iii < 4; ++iii) {
			this.confIdPaddingOut[iii] = -1;
			this.confIdPaddingIn[iii] = -1;
		}
		this.confIdChangeTime = -1;
		this.listAssiciatedId.clear();
	}
}
