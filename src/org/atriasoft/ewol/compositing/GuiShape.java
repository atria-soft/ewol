package org.atriasoft.ewol.compositing;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.atriasoft.egami.ImageByte;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.resource.ResourceConfigFile;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.backend3d.OpenGL.Flag;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.gale.resource.ResourceTexture2;
import org.atriasoft.loader3d.resources.ResourceMesh;
import org.atriasoft.loader3d.resources.ResourcePaletteFile;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * @brief the Shaper system is a basic theme configuration for every widget, it corresponds at a background display described by a pool of files
 */
// TODO : load image
// TODO : Abstaraction between states (call by name and the system greate IDs
public class GuiShape extends Compositing {
	private static final Logger LOGGER = LoggerFactory.getLogger(GuiShape.class);
	
	private class SpecificValues {
		public Matrix4f transform = Matrix4f.IDENTITY;
		public Vector3f offsetScaleInside = Vector3f.ZERO;
		public Vector3f offsetScaleOutside = Vector3f.ZERO;
		public ResourceMesh mesh = null;
	}

	private static final int SHAPER_POS_BOTTOM = 3;
	private static final int SHAPER_POS_LEFT = 0;
	private static final int SHAPER_POS_RIGHT = 2;
	private static final int SHAPER_POS_TOP = 1;
	private int confIdChangeTime = -1; //!< ConfigFile padding transition time property
	private int confIdPaletteFile = -1; //!< Palette of the display
	private final int[] confIdPaddingIn = new int[4]; //!< Padding in property : X-left X-right Y-top Y-buttom
	private final int[] confIdPaddingOut = new int[4]; //!< Padding out property : X-left X-right Y-top Y-buttom

	// External theme configuration:
	private ResourceConfigFile config = null; //!< pointer on the config file resources
	private int confObjectFile = -1; //!< Config Id of the object file to display
	private int confObjectFile2 = -1; //!< Config Id of the object file to display
	private int confProgramFileFrag = -1; //!< ConfigFile opengGl program Name
	private int confProgramFileVert = -1; //!< ConfigFile opengGl program Name
	private final List<Vector2i> listAssiciatedId = new ArrayList<>(); //!< Correlation ID between ColorProperty (Y) and OpenGL Program (X)
	// internal needed data :
	private GuiShapeMode nextStatusRequested = GuiShapeMode.NONE; //!< when status is changing, this represent the next step of it
	private int oGLMatrixProjection = -1; //!< openGL id on the element (Projection matrix)
	private int oGLMatrixTransformation = -1; //!< openGL id on the element (transformation matrix)
	private int oGLMatrixView = -1; //!< openGL id on the element (view matrix)
	private int oGLPaletteOffset = -1; //!< openGL id on the element (offset for the palet rendering)
	private int oGLOffsetScaleInside = -1;
	private int oGLOffsetScaleOutside = -1;

	// openGL shaders programs:
	private ResourceProgram oGLprogram = null; //!< pointer on the opengl display program
	// For the Image :
	private ResourcePaletteFile palette;
	private ResourceTexture2 texture;
	private Padding sizeObject = Padding.ZERO;
	private int stateActivate = -1; //!< Activate state of the element
	private GuiShapeMode stateNew = GuiShapeMode.NORMAL; //!< destination state
	private GuiShapeMode stateOld = GuiShapeMode.NORMAL; //!< previous state

	private Uri uri; //!< Name of the configuration of the shaper.
	private final SpecificValues[] valueSpecific = new SpecificValues[2];

	// dynamic change:
	private float stateTransition = 0; //!< working state between 2 states

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
		// Load data from the configuration file:
		loadConfigFile();
		loadUpdateObjectSize();
		loadPalette();
		loadProgram();
	}

	/**
	 * @brief change the current status in an other
	 * @param _newStatusId the next new status requested
	 * @return true The widget must call this fuction periodicly (and redraw itself)
	 * @return false No need to request the periodic call.
	 */
	public boolean changeStatusIn(final GuiShapeMode newStatusId) {
		if (newStatusId != this.stateNew) {
			this.nextStatusRequested = newStatusId;
			return true;
		}
		if (this.stateNew != this.stateOld) {
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
	 * @brief draw All the registered text in the current element on openGL
	 */
	@Override
	public void draw(final boolean disableDepthTest) {
		draw(null, disableDepthTest);
	}

	public void draw(final boolean disableDepthTest, final int idMesh) {
		draw(null, disableDepthTest, idMesh);
	}

	public void draw(final ResourceTexture2 secondaryTexture, final boolean disableDepthTest) {
		this.draw(secondaryTexture, disableDepthTest, 0);
	}

	public void draw(final ResourceTexture2 secondaryTexture, final boolean disableDepthTest, final int idMesh) {
		if (this.config == null) {
			// this is a normal case ... the user can choice to have no config basic file ...
			return;
		}
		if (idMesh == 0 && this.valueSpecific[0] == null) {
			LOGGER.error("No Object (0) to display ...");
			return;
		} else if (idMesh == 1 && this.valueSpecific[1] == null) {
			LOGGER.error("No Object (1) to display ...");
			return;
		} else if (idMesh < 0 && idMesh > 1) {
			LOGGER.error("No Object (" + idMesh + ") to display [0..1]");
			System.exit(-1);
			return;
		}
		if (this.oGLprogram == null) {
			LOGGER.error("No shader ...");
			return;
		}
		OpenGL.enable(Flag.flag_depthTest);
		// set Matrix : translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		final Matrix4f tmpMatrix = this.matrixApply.multiply(this.valueSpecific[idMesh].transform);
		this.oGLprogram.use();
		this.valueSpecific[idMesh].mesh.bindForRendering();
		this.oGLprogram.uniformMatrix(this.oGLMatrixProjection, projMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrixTransformation, tmpMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrixView, camMatrix);

		final Set<String> layers = this.valueSpecific[idMesh].mesh.getLayers();
		LOGGER.trace("get layers:" + layers);
		// Texture:
		final float imageDelta = (float) 1 / ResourcePaletteFile.getHeight();
		float basicValue = this.stateOld.getValue() / ResourcePaletteFile.getHeight();
		if (this.stateOld != this.stateNew) {
			if (this.stateOld == GuiShapeMode.NORMAL) {
				if (this.stateNew == GuiShapeMode.OVER) {
					basicValue += imageDelta * this.stateTransition;
				} else if (this.stateNew == GuiShapeMode.SELECT) {
					basicValue += imageDelta * 3.0f - imageDelta * this.stateTransition;
				}
			} else if (this.stateOld == GuiShapeMode.OVER) {
				if (this.stateNew == GuiShapeMode.NORMAL) {
					basicValue -= imageDelta * this.stateTransition;
				} else if (this.stateNew == GuiShapeMode.SELECT) {
					basicValue += imageDelta * this.stateTransition;
				}
			} else if (this.stateOld == GuiShapeMode.SELECT) {
				if (this.stateNew == GuiShapeMode.NORMAL) {
					basicValue += imageDelta * this.stateTransition;
				} else if (this.stateNew == GuiShapeMode.OVER) {
					basicValue -= imageDelta * this.stateTransition;
				}
			}
		}
		LOGGER.trace(
				"colorDelta = " + basicValue + " old = " + this.stateOld + "(" + this.stateOld.getValue() * imageDelta
						+ ") new = " + this.stateNew + "(" + this.stateNew.getValue() * imageDelta + ")");
		this.oGLprogram.uniformFloat(this.oGLPaletteOffset, basicValue);

		//LOGGER.trace("plop: " + this.offsetScaleOutside);
		//LOGGER.trace("plop: " + this.offsetScaleInside);
		this.oGLprogram.uniformVector(this.oGLOffsetScaleInside, this.valueSpecific[idMesh].offsetScaleInside);
		this.oGLprogram.uniformVector(this.oGLOffsetScaleOutside, this.valueSpecific[idMesh].offsetScaleOutside);

		this.texture.bindForRendering(0);
		this.valueSpecific[idMesh].mesh.render("palette");
		if (secondaryTexture != null) {
			this.oGLprogram.uniformFloat(this.oGLPaletteOffset, 0);
			secondaryTexture.bindForRendering(0);
			this.valueSpecific[idMesh].mesh.render("gui_dynamic_1");

		}
		// Request the draw of the elements:
		this.valueSpecific[idMesh].mesh.render();

		this.valueSpecific[idMesh].mesh.unBindForRendering();
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
	public GuiShapeMode getCurrentDisplayedStatus() {
		return this.stateNew;
	}

	/**
	 * @brief get the next displayed status of the shaper
	 * @return The next status Id (-1 if no status in next)
	 */
	public GuiShapeMode getNextDisplayedStatus() {
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
			out = new Padding(this.config.getNumber(this.confIdPaddingIn[GuiShape.SHAPER_POS_LEFT]),
					this.config.getNumber(this.confIdPaddingIn[GuiShape.SHAPER_POS_TOP]),
					this.config.getNumber(this.confIdPaddingIn[GuiShape.SHAPER_POS_RIGHT]),
					this.config.getNumber(this.confIdPaddingIn[GuiShape.SHAPER_POS_BOTTOM]));
		}
		return out;
	}

	public Padding getPaddingOut() {
		Padding out = Padding.ZERO;
		if (this.config != null) {
			out = new Padding(this.config.getNumber(this.confIdPaddingOut[GuiShape.SHAPER_POS_LEFT]),
					this.config.getNumber(this.confIdPaddingOut[GuiShape.SHAPER_POS_TOP]),
					this.config.getNumber(this.confIdPaddingOut[GuiShape.SHAPER_POS_RIGHT]),
					this.config.getNumber(this.confIdPaddingOut[GuiShape.SHAPER_POS_BOTTOM]));
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
	 * @brief get the current transition status
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

	private void loadConfigFile() {
		if (this.uri.isEmpty()) {
			LOGGER.debug("no Shaper set for loading resources ...");
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
			this.confObjectFile2 = this.config.request("object-file-2");
			this.confIdPaletteFile = this.config.request("palette");
		}
	}

	protected void loadPalette() {
		final String paletteFile = this.config.getString(this.confIdPaletteFile);
		final Uri paletteFileInterface = Uri.valueOf(paletteFile);
		this.palette = ResourcePaletteFile.create(paletteFileInterface);
		this.texture = ResourceTexture2.createNamed("TEXTURE_OF_PALETTE:" + paletteFile);
		if (this.texture == null) {
			LOGGER.error("can not instanciate Texture ...");
		}
		// element already called
		loadPaletteUpdate();
		// for next update (realTime reload)
		this.palette.onUpdate(() -> {
			loadPaletteUpdate();
		});
	}

	protected void loadPaletteUpdate() {
		LOGGER.warn("update palet environnement");
		final ImageByte img = this.palette.getImageByte();
		//IOgami.storePNG(new Uri("/home/heero/000000000aaaaplopppp.png"), img);
		this.texture.set(img);
	}

	/**
	 * load the openGL program and get all the ID needed
	 */
	private void loadProgram() {
		if (this.config == null) {
			LOGGER.debug("no Shaper set for loading resources ...");
			return;
		}
		final String basicShaderFileVert = this.config.getString(this.confProgramFileVert);
		final String basicShaderFileFrag = this.config.getString(this.confProgramFileFrag);
		if (!basicShaderFileVert.isEmpty() && !basicShaderFileFrag.isEmpty()) {
			this.oGLprogram = ResourceProgram.create(Uri.valueOf(basicShaderFileVert),
					Uri.valueOf(basicShaderFileFrag));
			if (this.oGLprogram != null) {
				this.oGLMatrixTransformation = this.oGLprogram.getUniform("in_matrixTransformation");
				this.oGLMatrixProjection = this.oGLprogram.getUniform("in_matrixProjection");
				this.oGLMatrixView = this.oGLprogram.getUniform("in_matrixView");
				this.oGLPaletteOffset = this.oGLprogram.getUniform("in_offsetPalette");
				this.oGLOffsetScaleInside = this.oGLprogram.getUniform("in_offsetScaleInside");
				this.oGLOffsetScaleOutside = this.oGLprogram.getUniform("in_offsetScaleOutside");
			}
		}
	}

	/**
	 * load the openGL program and get all the ID needed
	 */
	private void loadUpdateObjectSize() {
		if (this.config == null) {
			LOGGER.debug("no Shaper set for loading resources ...");
			return;
		}
		final String objectFile = this.config.getString(this.confObjectFile);
		if (!objectFile.isEmpty()) {
			final int idMesh = 0;
			this.valueSpecific[idMesh] = new SpecificValues();
			this.valueSpecific[idMesh].mesh = ResourceMesh.create(Uri.valueOf(objectFile));
			final List<Vector3f> verticesToModify = this.valueSpecific[idMesh].mesh.getGeneratedPosition();
			float top = 0;
			float bottom = 0;
			float left = 0;
			float right = 0;
			float back = 0;
			float font = 0;
			// estimate size of border:
			if (verticesToModify == null) {
				LOGGER.error("Element is null : verticesToModify 1");
				System.exit(-1);
				return;
			}
			for (final Vector3f element : verticesToModify) {
				left = Math.min(left, element.x());
				right = Math.max(right, element.x());
				top = Math.min(top, element.y());
				bottom = Math.max(bottom, element.y());
				back = Math.min(back, element.z());
				font = Math.max(font, element.z());
			}
			this.sizeObject = new Padding(Math.abs(left), Math.abs(top), Math.abs(right), Math.abs(bottom));
		} else {
			final int idMesh = 0;
			this.valueSpecific[idMesh] = null;
		}
		final String objectFile2 = this.config.getString(this.confObjectFile2);
		if (!objectFile2.isEmpty()) {
			final int idMesh = 1;
			this.valueSpecific[idMesh] = new SpecificValues();
			this.valueSpecific[idMesh].mesh = ResourceMesh.create(Uri.valueOf(objectFile2));
			final List<Vector3f> verticesToModify = this.valueSpecific[idMesh].mesh.getGeneratedPosition();
			float top = 0;
			float bottom = 0;
			float left = 0;
			float right = 0;
			float back = 0;
			float font = 0;
			// estimate size of border:
			if (verticesToModify == null) {
				LOGGER.error("Element is null : verticesToModify 2");
				System.exit(-1);
				return;
			}
			for (final Vector3f element : verticesToModify) {
				left = Math.min(left, element.x());
				right = Math.max(right, element.x());
				top = Math.min(top, element.y());
				bottom = Math.max(bottom, element.y());
				back = Math.min(back, element.z());
				font = Math.max(font, element.z());
			}
		} else {
			final int idMesh = 1;
			this.valueSpecific[idMesh] = null;
		}
	}

	/**
	 * @brief Same as the widfget periodic call (this is for change display)
	 * @param event The current time of the call.
	 * @return true The widget must call this fuction periodicly (and redraw itself)
	 * @return false No need to request the periodic call.
	 */
	public boolean periodicCall(final EventTime event) {
		LOGGER.trace("call=" + event.getTimeDeltaCallSecond() + "s state transition=" + this.stateTransition
				+ " speedTime=" + this.config.getNumber(this.confIdChangeTime));
		// start :
		if (this.stateTransition >= 1.0) {
			this.stateOld = this.stateNew;
			if ((this.nextStatusRequested == GuiShapeMode.NONE) || (this.nextStatusRequested == this.stateOld)) {
				this.nextStatusRequested = GuiShapeMode.NONE;
				// disable periodic call ...
				return false;
			}
			this.stateNew = this.nextStatusRequested;
			this.nextStatusRequested = GuiShapeMode.NONE;
			this.stateTransition = 0.0f;
			LOGGER.trace("     ##### START #####  ");
			return true;
		}
		if (this.stateTransition < 1.0) {
			// check if no new state requested:
			if (this.nextStatusRequested != GuiShapeMode.NONE && this.stateTransition < 0.5) {
				// invert sources with destination
				final GuiShapeMode tmppp = this.stateOld;
				this.stateOld = this.stateNew;
				this.stateNew = tmppp;
				this.stateTransition = 1.0f - this.stateTransition;
				if (this.nextStatusRequested == this.stateNew) {
					this.nextStatusRequested = GuiShapeMode.NONE;
				}
			}
			float timeRelativity = 0.0f;
			if (this.config != null) {
				timeRelativity = (float) (this.config.getNumber(this.confIdChangeTime) / 1000.0f);
			}
			this.stateTransition += event.getTimeDeltaCallSecond() / timeRelativity;
			//stateTransition += _event.getDeltaCall();
			this.stateTransition = FMath.avg(0.0f, this.stateTransition, 1.0f);
			LOGGER.trace("relative=" + timeRelativity + " Transition : " + this.stateTransition);
		}
		return true;
	}

	/**
	 * @brief Set activate state of the element
	 * @param _status New activate status
	 */
	public void setActivateState(final int status) {
		this.stateActivate = status;
	}

	public void setShape(final int idMesh, final Vector3f origin, final Vector3f size) {
		final Padding tmp = getPadding();
		setShape(idMesh, origin, size, origin.add(tmp.left(), tmp.bottom(), 0), size.less(tmp.x(), tmp.y(), 0));
	}

	public void setShape(
			final int idMesh,
			final Vector3f origin,
			final Vector3f size,
			final Vector3f insidePos,
			final Vector3f insideSize) {
		final Vector3f halfSize = insideSize.multiply(0.5f);
		this.valueSpecific[idMesh].offsetScaleOutside = halfSize;
		this.valueSpecific[idMesh].offsetScaleInside = halfSize.add(this.sizeObject.x() * 0.25f,
				this.sizeObject.y() * 0.25f, 0);
		/*
		List<Vector3f> verticesToModify = this.mesh.getGeneratedPosition();
		float[] newVertices = new float[verticesToModify.size()*3];
		for (int iii=0; iii<newVertices.length; ++iii) {
			Vector3f tmp = verticesToModify.get(iii);
			newVertices[iii*3+0] = getUpdatedPos(tmp.x(), halfSize.x());
			newVertices[iii*3+1] = getUpdatedPos(tmp.y(), halfSize.y());
			newVertices[iii*3+2] =  getUpdatedPos(tmp.z(), halfSize.z());
		}
		this.mesh.setModifiedPosition(newVertices);
		*/
		this.valueSpecific[idMesh].transform = Matrix4f.createMatrixTranslate(
				new Vector3f(origin.x() + size.x() * 0.5f, origin.y() + size.y() * 0.5f, origin.z() + size.z() * 0.5f));
	}

	//	private float getUpdatedPos(final float value, final float halfsize) {
	//		if (value <= 0.0f) {
	//			return value - halfsize;
	//		}
	//		return value + halfsize;
	//	}

	// @previous
	public void setShape(final Vector2f origin, final Vector2f size) {
		final Padding tmp = getPadding();
		setShape(origin, size, origin.add(tmp.left(), tmp.bottom()), size.less(tmp.x(), tmp.y()));
	}

	/**
	 * @brief set the shape property:
	 *
	 *   ********************************************************************************
	 *   *                                                                         size *
	 *   *                                                                              *
	 *   *        * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *       *
	 *   *                                                                              *
	 *   *        |                                                             |       *
	 *   *             ***************************************************              *
	 *   *        |    *                                                 *      |       *
	 *   *             *                                                 *              *
	 *   *        |    *     * - - - - - - - - - - - - - - - - - - *     *      |       *
	 *   *             *                                 insideSize      *              *
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
	 *   *             *      insidePos                                  *              *
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
	 *   origin
	 *
	 *
	 * @param center Center of the object
	 * @param size Size of the display
	 */
	public void setShape(
			final Vector2f origin,
			final Vector2f size,
			final Vector2f insidePos,
			final Vector2f insideSize) {
		//LOGGER.error("Set shape property : origin=" + origin + " size=" + size + "  in-pos=" + insidePos + "  in-size=" + insideSize);
		final Vector2f halfSize = insideSize.multiply(0.5f);
		final Vector3f offsetScaleOutside = new Vector3f(halfSize.x(), halfSize.y(), 1.0f);
		final Vector3f offsetScaleInside = new Vector3f(halfSize.x() + this.sizeObject.x() * 0.25f,
				halfSize.y() + this.sizeObject.y() * 0.25f, 1.0f);
		/*
		List<Vector3f> verticesToModify = this.mesh.getGeneratedPosition();
		float[] newVertices = new float[verticesToModify.size()*3];
		for (int iii=0; iii<verticesToModify.size(); ++iii) {
			Vector3f tmp = verticesToModify.get(iii);
			newVertices[iii*3+0] = getUpdatedPos(tmp.x(), halfSize.x());
			newVertices[iii*3+1] = getUpdatedPos(tmp.y(), halfSize.y());
			newVertices[iii*3+2] = tmp.z();
		}
		//this.transform = Matrix4f.createMatrixRotate(new Vector3f(1.0f, 0.0f, 0.0f), -FMath.PI*0.30f);
		//this.transform = this.transform.multiply(Matrix4f.createMatrixTranslate(new Vector3f(origin.x() + size.x() * 0.5f, origin.y() + size.y() * 0.5f, 0.0f)));
		this.mesh.setModifiedPosition(newVertices);
		 */
		final Matrix4f transform = Matrix4f
				.createMatrixTranslate(new Vector3f(origin.x() + size.x() * 0.5f, origin.y() + size.y() * 0.5f, 0.0f));
		for (int iii = 0; iii < 2; iii++) {
			if (this.valueSpecific[iii] == null) {
				continue;
			}
			this.valueSpecific[iii].offsetScaleOutside = offsetScaleOutside;
			this.valueSpecific[iii].offsetScaleInside = offsetScaleInside;
			this.valueSpecific[iii].transform = transform;
		}
	}

	public void setShape(final Vector3f origin, final Vector3f size) {
		final Padding tmp = getPadding();
		setShape(origin, size, origin.add(tmp.left(), tmp.bottom(), 0), size.less(tmp.x(), tmp.y(), 0));
	}

	public void setShape(
			final Vector3f origin,
			final Vector3f size,
			final Vector3f insidePos,
			final Vector3f insideSize) {
		final Vector3f halfSize = insideSize.multiply(0.5f);
		final Vector3f offsetScaleOutside = halfSize;
		final Vector3f offsetScaleInside = halfSize.add(this.sizeObject.x() * 0.25f, this.sizeObject.y() * 0.25f, 0);
		/*
		List<Vector3f> verticesToModify = this.mesh.getGeneratedPosition();
		float[] newVertices = new float[verticesToModify.size()*3];
		for (int iii=0; iii<newVertices.length; ++iii) {
			Vector3f tmp = verticesToModify.get(iii);
			newVertices[iii*3+0] = getUpdatedPos(tmp.x(), halfSize.x());
			newVertices[iii*3+1] = getUpdatedPos(tmp.y(), halfSize.y());
			newVertices[iii*3+2] =  getUpdatedPos(tmp.z(), halfSize.z());
		}
		this.mesh.setModifiedPosition(newVertices);
		*/
		final Matrix4f transform = Matrix4f.createMatrixTranslate(
				new Vector3f(origin.x() + size.x() * 0.5f, origin.y() + size.y() * 0.5f, origin.z() + size.z() * 0.5f));
		for (int iii = 0; iii < 2; iii++) {
			if (this.valueSpecific[iii] == null) {
				continue;
			}
			this.valueSpecific[iii].offsetScaleOutside = offsetScaleOutside;
			this.valueSpecific[iii].offsetScaleInside = offsetScaleInside;
			this.valueSpecific[iii].transform = transform;
		}
	}

	/**
	 * @brief change the shaper Source
	 * @param _uri New file of the shaper
	 */
	public void setSource(final Uri uri) {
		clear();
		unLoadProgram();
		this.uri = uri;
		loadConfigFile();
		loadUpdateObjectSize();
		loadPalette();
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
		this.texture = null;
		this.config = null;
		for (int iii = 0; iii < 4; ++iii) {
			this.confIdPaddingOut[iii] = -1;
			this.confIdPaddingIn[iii] = -1;
		}
		this.confIdChangeTime = -1;
		this.listAssiciatedId.clear();
	}
}
