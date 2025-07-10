/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.gale.resource.ResourceVirtualArrayObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class CompositingDrawing extends Compositing {
	private static final Logger LOGGER = LoggerFactory.getLogger(CompositingDrawing.class);

	protected static int vboIdColor = 1;
	protected static int vboIdCoord = 0;
	private boolean clippingEnable = false; // !< true if the clipping must be activated
	private Vector3f clippingPosStart = Vector3f.ZERO; // !< Clipping start position
	private Vector3f clippingPosStop = Vector3f.ZERO; // !< Clipping stop position
	private Color color = Color.BLACK; // !< The text foreground color
	private Color colorBg = Color.NONE; // !< The text background color
	//private int oGLMatrix = -1; // !< openGL id on the element (transformation matrix)
	//private int oGLMatrixPosition = -1; // !< position matrix
	private int oGLMatrixProjection = -1; //!< openGL id on the element (Projection matrix)
	private int oGLMatrixTransformation = -1; //!< openGL id on the element (transformation matrix)
	private int oGLMatrixView = -1; //!< openGL id on the element (view matrix)
	private ResourceProgram oGLprogram; // !< pointer on the opengl display program
	private final List<Color> outColors = new ArrayList<>();
	private final List<Vector3f> outTriangles = new ArrayList<>();

	private Vector3f position = new Vector3f(0, 0, 0); // !< The current position to draw

	private float thickness = 0; // !< when drawing line and other things

	private final Vector3f[] triangle = new Vector3f[3]; // !< Register every system with a combinaison of tiangle

	private final Color[] tricolor = new Color[3]; // !< Register every the associated color foreground

	private int triElement = 0; // !< special counter of the single dot generated
	//protected ResourceVirtualBufferObject vbo;
	protected ResourceVirtualArrayObject vbo;

	// internal API for the generation abstraction of triangles
	/**
	 * Basic ructor
	 */
	public CompositingDrawing() {
		loadProgram();
		for (int iii = 0; iii < 3; iii++) {
			this.triangle[iii] = this.position;
			this.tricolor[iii] = this.color;
		}
		// Create the VBO:
		this.vbo = ResourceVirtualArrayObject.createDynamic();
		// TO facilitate some debugs we add a name of the VBO:
		this.vbo.setName("[VBO] of ewol::compositing::Area");
	}

	/**
	 * add a point reference at the current position (this is a vertex
	 *        reference at the current position
	 */
	public void addVertex() {
		internalSetColor(this.color);
		setPoint(this.position);
	}

	/**
	 * draw a 2D circle with the specify rafdius parameter.
	 * @param radius Distence to the dorder
	 * @param angleStart start angle of this circle ([0..2PI] otherwise ==> disable)
	 * @param angleStop stop angle of this circle ([0..2PI] otherwise ==> disable)
	 */
	public void circle(final float radius) {
		circle(radius, 0);
	}

	public void circle(final float radius, final float angleStart) {
		circle(radius, angleStart, 2.0f * FMath.PI);
	}

	public void circle(float radius, final float angleStart, float angleStop) {
		resetCount();

		if (radius < 0) {
			radius *= -1;
		}
		angleStop = angleStop - angleStart;

		int nbOcurence = (int) radius;
		if (nbOcurence < 10) {
			nbOcurence = 10;
		}

		// display background :
		if (this.colorBg.a() != 0) {
			internalSetColor(this.colorBg);
			for (int iii = 0; iii < nbOcurence; iii++) {
				setPoint(new Vector3f(this.position.x(), this.position.y(), 0));

				final float angleOne = angleStart + (angleStop * iii / nbOcurence);
				float offsety = FMath.sin(angleOne) * radius;
				float offsetx = FMath.cos(angleOne) * radius;

				setPoint(new Vector3f(this.position.x() + offsetx, this.position.y() + offsety, 0));

				final float angleTwo = angleStart + (angleStop * (iii + 1) / nbOcurence);
				offsety = FMath.sin(angleTwo) * radius;
				offsetx = FMath.cos(angleTwo) * radius;

				setPoint(new Vector3f(this.position.x() + offsetx, this.position.y() + offsety, 0));
			}
		}

		// show if we have a border :
		if (this.thickness == 0 || this.color.a() == 0) {
			return;
		}
		internalSetColor(this.color);
		for (int iii = 0; iii < nbOcurence; iii++) {

			final float angleOne = angleStart + (angleStop * iii / nbOcurence);
			final float offsetExty = FMath.sin(angleOne) * (radius + this.thickness / 2);
			final float offsetExtx = FMath.cos(angleOne) * (radius + this.thickness / 2);
			final float offsetInty = FMath.sin(angleOne) * (radius - this.thickness / 2);
			final float offsetIntx = FMath.cos(angleOne) * (radius - this.thickness / 2);

			final float angleTwo = angleStart + (angleStop * (iii + 1) / nbOcurence);
			final float offsetExt2y = FMath.sin(angleTwo) * (radius + this.thickness / 2);
			final float offsetExt2x = FMath.cos(angleTwo) * (radius + this.thickness / 2);
			final float offsetInt2y = FMath.sin(angleTwo) * (radius - this.thickness / 2);
			final float offsetInt2x = FMath.cos(angleTwo) * (radius - this.thickness / 2);

			setPoint(new Vector3f(this.position.x() + offsetIntx, this.position.y() + offsetInty, 0));
			setPoint(new Vector3f(this.position.x() + offsetExtx, this.position.y() + offsetExty, 0));
			setPoint(new Vector3f(this.position.x() + offsetExt2x, this.position.y() + offsetExt2y, 0));

			setPoint(new Vector3f(this.position.x() + offsetExt2x, this.position.y() + offsetExt2y, 0));
			setPoint(new Vector3f(this.position.x() + offsetInt2x, this.position.y() + offsetInt2y, 0));
			setPoint(new Vector3f(this.position.x() + offsetIntx, this.position.y() + offsetInty, 0));
		}
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
		this.outTriangles.clear();
		this.outColors.clear();

		// reset temporal variables :
		this.position = Vector3f.ZERO;

		this.clippingPosStart = Vector3f.ZERO;
		this.clippingPosStop = Vector3f.ZERO;
		this.clippingEnable = false;

		this.color = Color.BLACK;
		this.colorBg = Color.NONE;

		for (int iii = 0; iii < 3; iii++) {
			this.triangle[iii] = this.position;
			this.tricolor[iii] = this.color;
		}
	}

	/**
	 * draw All the refistered text in the current element on openGL
	 */
	@Override
	public void draw(final boolean disableDepthTest) {
		if (this.oGLprogram == null) {
			LOGGER.error("No shader ...");
			return;
		}
		// set Matrix : translation/positionMatrix
		final Matrix4f projMatrix = OpenGL.getMatrix();
		final Matrix4f camMatrix = OpenGL.getCameraMatrix();
		this.oGLprogram.use();
		this.vbo.bindForRendering();
		this.oGLprogram.uniformMatrix(this.oGLMatrixProjection, projMatrix);
		this.oGLprogram.uniformMatrix(this.oGLMatrixTransformation, this.matrixApply);
		this.oGLprogram.uniformMatrix(this.oGLMatrixView, camMatrix);
		// Request the draw of the elements:
		this.vbo.renderArrays(OpenGL.RenderMode.TRIANGLE);
		this.vbo.unBindForRendering();
		this.oGLprogram.unUse();
	}

	@Override
	public void flush() {
		// push data on the VBO
		this.vbo.setPosition(this.outTriangles.toArray(Vector3f[]::new));
		this.vbo.setColors(this.outColors.toArray(Color[]::new));
		this.vbo.setVertexCount(this.outTriangles.size());
	}

	/**
	 * Lunch the generation of triangle
	 */
	private void generateTriangle() {
		this.triElement = 0;
		this.outTriangles.add(this.triangle[0]);
		this.outTriangles.add(this.triangle[1]);
		this.outTriangles.add(this.triangle[2]);
		this.outColors.add(this.tricolor[0]);
		this.outColors.add(this.tricolor[1]);
		this.outColors.add(this.tricolor[2]);
	}

	/**
	 * Get the foreground color of the font.
	 * @return Foreground color.
	 */
	public Color getColor() {
		return this.color;
	}

	/**
	 * Get the background color of the font.
	 * @return Background color.
	 */
	public Color getColorBg() {
		return this.colorBg;
	}

	/**
	 * get the current display position (sometime needed in the gui control)
	 * @return the current position.
	 */
	public Vector3f getPos() {
		return this.position;
	}

	/**
	 * set the Color of the current triangle drawing
	 * @param color Color to current dots generated
	 */
	private void internalSetColor(final Color color) {
		if (this.triElement < 1) {
			this.tricolor[0] = color;
		}
		if (this.triElement < 2) {
			this.tricolor[1] = color;
		}
		if (this.triElement < 3) {
			this.tricolor[2] = color;
		}
	}

	/**
	 * Relative drawing a line (special vector)
	 * @param vect Vector of the current line.
	 */
	public void lineRel(final float xxx, final float yyy) {
		lineTo(this.position.add(new Vector3f(xxx, yyy, 0)));
	}

	public void lineRel(final float xxx, final float yyy, final float zzz) {
		lineTo(this.position.add(new Vector3f(xxx, yyy, zzz)));
	}

	public void lineRel(final Vector2f vect) {
		lineRel(new Vector3f(vect.x(), vect.y(), 0));
	}

	public void lineRel(final Vector3f vect) {
		lineTo(this.position.add(vect));
	}

	public void lineTo(final float xxx, final float yyy) {
		lineTo(new Vector3f(xxx, yyy, 0));
	}

	public void lineTo(final float xxx, final float yyy, final float zzz) {
		lineTo(new Vector3f(xxx, yyy, zzz));
	}

	public void lineTo(final Vector2f dest) {
		lineTo(new Vector3f(dest.x(), dest.y(), 0));
	}

	/**
	 * draw a line to a specific position
	 * @param dest Position of the end of the line.
	 */
	public void lineTo(final Vector3f dest) {
		resetCount();
		internalSetColor(this.color);
		// LOGGER.trace("DrawLine : " + this.position + " to " + dest);
		if (this.position.x() == dest.x() && this.position.y() == dest.y() && this.position.z() == dest.z()) {
			// LOGGER.warn("Try to draw a line width 0");
			return;
		}
		// teta = tan-1(oposer/adjacent)
		float teta = 0;
		if (this.position.x() <= dest.x()) {
			teta = FMath.atan((dest.y() - this.position.y()) / (dest.x() - this.position.x()));
		} else {
			teta = FMath.PI + FMath.atan((dest.y() - this.position.y())) / (dest.x() - this.position.x());
		}
		if (teta < 0) {
			teta += 2 * FMath.PI;
		} else if (teta > 2 * FMath.PI) {
			teta -= 2 * FMath.PI;
		}
		// LOGGER.debug("teta = " + (teta*180/(FMath.PI)) + " deg." );
		final float offsety = FMath.sin(teta - FMath.PI / 2) * (this.thickness / 2);
		final float offsetx = FMath.cos(teta - FMath.PI / 2) * (this.thickness / 2);
		setPoint(new Vector3f(this.position.x() - offsetx, this.position.y() - offsety, this.position.z()));
		setPoint(new Vector3f(this.position.x() + offsetx, this.position.y() + offsety, this.position.z()));
		setPoint(new Vector3f(dest.x() + offsetx, dest.y() + offsety, this.position.z()));

		setPoint(new Vector3f(dest.x() + offsetx, dest.y() + offsety, dest.z()));
		setPoint(new Vector3f(dest.x() - offsetx, dest.y() - offsety, dest.z()));
		setPoint(new Vector3f(this.position.x() - offsetx, this.position.y() - offsety, dest.z()));
		// update the system position :
		this.position = dest;
	}

	/**
	 * load the openGL program and get all the ID needed
	 */
	private void loadProgram() {
		// remove previous loading ... in case
		unLoadProgram();
		// oad the new ...
		this.oGLprogram = ResourceProgram.create(new Uri("DATA", "color3.vert", "ewol"),
				new Uri("DATA", "color3.frag", "ewol"));
		// get the shader resource :
		if (this.oGLprogram != null) {
			//this.oGLPosition = this.oGLprogram.getAttribute("in_coord3d");
			//this.oGLColor = this.oGLprogram.getAttribute("in_color");
			this.oGLMatrixTransformation = this.oGLprogram.getUniform("in_matrixTransformation");
			this.oGLMatrixProjection = this.oGLprogram.getUniform("in_matrixProjection");
			this.oGLMatrixView = this.oGLprogram.getUniform("in_matrixView");
		}
	}

	public void rectangle(final float xxx, final float yyy) {
		rectangle(new Vector3f(xxx, yyy, 0));
	}

	public void rectangle(final float xxx, final float yyy, final float zzz) {
		rectangle(new Vector3f(xxx, yyy, zzz));
	}

	public void rectangle(final Vector2f dest) {
		rectangle(new Vector3f(dest.x(), dest.y(), 0));
	}

	/**
	 * draw a 2D rectangle to the position requested.
	 * @param dest Position the the end of the rectangle
	 */
	public void rectangle(final Vector3f dest) {
		resetCount();
		internalSetColor(this.color);
		/*
		 * Bitmap position xA xB yC *------* | | | | yD *------*
		 */
		float dxA = this.position.x();
		float dxB = dest.x();
		if (dxA > dxB) {
			// inverse order :
			final float tmp = dxA;
			dxA = dxB;
			dxB = tmp;
		}
		float dyC = this.position.y();
		float dyD = dest.y();
		if (dyC > dyD) {
			// inverse order :
			final float tmp = dyC;
			dyC = dyD;
			dyD = tmp;
		}
		if (this.clippingEnable) {
			if (dxA < this.clippingPosStart.x()) {
				dxA = this.clippingPosStart.x();
			}
			if (dxB > this.clippingPosStop.x()) {
				dxB = this.clippingPosStop.x();
			}
			if (dyC < this.clippingPosStart.y()) {
				dyC = this.clippingPosStart.y();
			}
			if (dyD > this.clippingPosStop.y()) {
				dyD = this.clippingPosStop.y();
			}
		}
		if (dyC >= dyD || dxA >= dxB) {
			return;
		}
		setPoint(new Vector3f(dxA, dyD, 0));
		setPoint(new Vector3f(dxA, dyC, 0));
		setPoint(new Vector3f(dxB, dyC, 0));

		setPoint(new Vector3f(dxB, dyC, 0));
		setPoint(new Vector3f(dxB, dyD, 0));
		setPoint(new Vector3f(dxA, dyD, 0));
	}

	public void rectangleWidth(final float xxx, final float yyy) {
		rectangleWidth(new Vector3f(xxx, yyy, 0));
	}

	public void rectangleWidth(final float xxx, final float yyy, final float zzz) {
		rectangleWidth(new Vector3f(xxx, yyy, zzz));
	}

	public void rectangleWidth(final Vector2f size) {
		rectangleWidth(new Vector3f(size.x(), size.y(), 0));
	}

	/**
	 * draw a 2D rectangle to the requested size.
	 * @param size size of the rectangle
	 */
	public void rectangleWidth(final Vector3f size) {
		rectangle(this.position.add(size));
	}

	/**
	 * in case of some error the count can be reset
	 */
	private void resetCount() {
		this.triElement = 0;
	}

	public void setClipping(final Vector2f pos, final Vector2f posEnd) {
		setClipping(new Vector3f(pos.x(), pos.y(), -1), new Vector3f(posEnd.x(), posEnd.y(), 1));
	}

	/**
	 * Request a clipping area for the text (next draw only)
	 * @param pos Start position of the clipping
	 * @param posEnd End position of the clipping
	 */
	public void setClipping(final Vector3f pos, final Vector3f posEnd) {
		// note the internal system all time request to have a bounding all time in the
		// same order
		this.clippingPosStop = Vector3f.max(pos, posEnd);
		this.clippingPosStart = Vector3f.min(pos, posEnd);
		this.clippingEnable = true;
	}

	/**
	 * enable/Disable the clipping (without lose the current clipping
	 *        position)
	 * newMode The new status of the clipping
	 */
	public void setClippingMode(final boolean newMode) {
		this.clippingEnable = newMode;
	}

	public void setClippingWidth(final Vector2f pos, final Vector2f width) {
		setClippingWidth(new Vector3f(pos.x(), pos.y(), -1), new Vector3f(width.x(), width.y(), 2));
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

	/**
	 * set the background color of the font (for selected Text (not the
	 *        global BG))
	 * @param color Color to set on background (for next print)
	 */
	public void setColorBg(final Color color) {
		this.colorBg = color;
	}

	/**
	 * internal add of the specific point
	 * @param point The requeste dpoint to add
	 */
	private void setPoint(final Vector3f point) {
		this.triangle[this.triElement] = point;
		this.triElement++;
		if (this.triElement >= 3) {
			generateTriangle();
		}
		this.vbo.flush();
	}

	public void setPos(final float xxx, final float yyy) {
		setPos(new Vector3f(xxx, yyy, 0));
	}

	public void setPos(final float xxx, final float yyy, final float zzz) {
		setPos(new Vector3f(xxx, yyy, zzz));
	}

	public void setPos(final Vector2f pos) {
		setPos(new Vector3f(pos.x(), pos.y(), 0));
	}

	/**
	 * set position for the next text written
	 * @param pos Position of the text (in 3D)
	 */
	public void setPos(final Vector3f pos) {
		this.position = pos;
	}

	public void setRelPos(final float xxx, final float yyy) {
		this.position = this.position.add(xxx, yyy, 0);
	}

	/**
	 * set relative position for the next text writen
	 * @param pos ofset apply of the text (in 3D)
	 */
	public void setRelPos(final float xxx, final float yyy, final float zzz) {
		this.position = this.position.add(xxx, yyy, zzz);
	}

	public void setRelPos(final Vector2f pos) {
		setRelPos(new Vector3f(pos.x(), pos.y(), 0));
	}

	public void setRelPos(final Vector3f pos) {
		this.position = this.position.add(pos);
	}

	/**
	 * Specify the line thickness for the next elements
	 * @param thickness The thickness desired for the next print
	 */
	public void setThickness(final float thickness) {
		this.thickness = thickness;
		// thickness must be positive
		if (this.thickness < 0) {
			this.thickness *= -1;
		}
	}

	/**
	 * Un-Load the openGL program and get all the ID needed
	 */
	private void unLoadProgram() {
		this.oGLprogram = null;
	}

}