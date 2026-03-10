/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.BorderRadius;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Insets;
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

public abstract class CompositingDrawing extends CompositingDraw {
	private static final Logger LOGGER = LoggerFactory.getLogger(CompositingDrawing.class);

	/** Anti-aliasing fringe width in pixels for smooth edges on curved shapes. */
	private static final float AA_FRINGE = 1.0f;

	protected static int vboIdColor = 1;
	protected static int vboIdCoord = 0;
	protected boolean clippingEnable = false; // !< true if the clipping must be activated
	protected Vector3f clippingPosStart = Vector3f.ZERO; // !< Clipping start position
	protected Vector3f clippingPosStop = Vector3f.ZERO; // !< Clipping stop position
	protected Color color = Color.BLACK; // !< The text foreground color
	protected Color colorBg = Color.NONE; // !< The text background color
	//private int oGLMatrix = -1; // !< openGL id on the element (transformation matrix)
	//private int oGLMatrixPosition = -1; // !< position matrix
	protected int oGLMatrixProjection = -1; //!< openGL id on the element (Projection matrix)
	protected int oGLMatrixTransformation = -1; //!< openGL id on the element (transformation matrix)
	protected int oGLMatrixView = -1; //!< openGL id on the element (view matrix)
	protected ResourceProgram oGLprogram; // !< pointer on the opengl display program
	protected final List<Color> outColors = new ArrayList<>();
	protected final List<Vector3f> outTriangles = new ArrayList<>();
	
	protected Vector3f position = new Vector3f(0, 0, 0); // !< The current position to draw
	
	protected float thickness = 0; // !< when drawing line and other things
	
	protected final Vector3f[] triangle = new Vector3f[3]; // !< Register every system with a combinaison of tiangle
	
	protected final Color[] tricolor = new Color[3]; // !< Register every the associated color foreground
	
	protected int triElement = 0; // !< special counter of the single dot generated
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

	public void circleBorderRaw(
			final Vector3f centerPos,
			final float radius,
			final float thickness,
			final float angleStart,
			final float angleStop) {
		circleBorderRaw(centerPos, radius, thickness, thickness, angleStart, angleStop);
	}
	
	public void circleBorderRaw(
			final Vector3f centerPos,
			final float radius,
			float thicknessStart,
			float thicknessStop,
			final float angleStart,
			final float angleStop) {
		final Color opaqueColor = this.tricolor[0];
		resetCount();
		if (thicknessStart < 0.001 && thicknessStop < 0.001) {
			return;
		}
		if (radius < 0.001) {
			return;
		}
		if (thicknessStart > radius) {
			thicknessStart = radius;
		}
		if (thicknessStop > radius) {
			thicknessStop = radius;
		}
		int nbOcurence = (int) (radius * 2);
		if (nbOcurence < 20) {
			nbOcurence = 20;
		}
		final Color fringeColor = withZeroAlpha(opaqueColor);
		for (int iii = 0; iii < nbOcurence; iii++) {
			final float ratio = (float) iii / (float) nbOcurence;
			final float thickness = thicknessStart + ((thicknessStop - thicknessStart) * ratio);
			final float angleOne = angleStart + (angleStop * ratio);
			final float angleTwo = angleStart + (angleStop * (iii + 1) / nbOcurence);

			final float cosOne = FMath.cos(angleOne);
			final float sinOne = FMath.sin(angleOne);
			final float cosTwo = FMath.cos(angleTwo);
			final float sinTwo = FMath.sin(angleTwo);

			final float outerRadius = radius;
			final float innerRadius = radius - thickness;
			// Fringe ramps up from 0 at arc edges to full after AA_FRINGE_ANGLE
			final float arcAngle = angleStop * ratio;
			final float arcAngleEnd = angleStop * (1.0f - ratio);
			final float minEdgeAngle = Math.min(arcAngle, arcAngleEnd);
			final float AA_FRINGE_ANGLE = 15.0f * FMath.PI / 180.0f;
			final float fringeScale = Math.min(1.0f, minEdgeAngle / AA_FRINGE_ANGLE);
			final float fringe = AA_FRINGE * fringeScale;
			// Fringe extends outward from original border edges
			final float outerFringeRadius = outerRadius + fringe;
			final float innerFringeRadius = Math.max(0f, innerRadius - fringe);

			// Solid border quad at original size
			final Vector3f ext1 = new Vector3f(centerPos.x() + cosOne * outerRadius, centerPos.y() + sinOne * outerRadius, 0);
			final Vector3f ext2 = new Vector3f(centerPos.x() + cosTwo * outerRadius, centerPos.y() + sinTwo * outerRadius, 0);
			final Vector3f int1 = new Vector3f(centerPos.x() + cosOne * innerRadius, centerPos.y() + sinOne * innerRadius, 0);
			final Vector3f int2 = new Vector3f(centerPos.x() + cosTwo * innerRadius, centerPos.y() + sinTwo * innerRadius, 0);

			internalSetColor(opaqueColor);
			setPoint(int1);
			setPoint(ext1);
			setPoint(ext2);
			setPoint(ext2);
			setPoint(int2);
			setPoint(int1);

			if (fringe > 0.3f) {
				// Outer fringe: outer(opaque) → outerFringe(transparent)
				final Vector3f fExt1 = new Vector3f(centerPos.x() + cosOne * outerFringeRadius, centerPos.y() + sinOne * outerFringeRadius, 0);
				final Vector3f fExt2 = new Vector3f(centerPos.x() + cosTwo * outerFringeRadius, centerPos.y() + sinTwo * outerFringeRadius, 0);

				internalSetColor(opaqueColor);
				setPoint(ext1);
				internalSetColor(fringeColor);
				setPoint(fExt1);
				setPoint(fExt2);
				internalSetColor(fringeColor);
				setPoint(fExt2);
				internalSetColor(opaqueColor);
				setPoint(ext2);
				setPoint(ext1);

				// Inner fringe: inner(opaque) → innerFringe(transparent)
				final Vector3f fInt1 = new Vector3f(centerPos.x() + cosOne * innerFringeRadius, centerPos.y() + sinOne * innerFringeRadius, 0);
				final Vector3f fInt2 = new Vector3f(centerPos.x() + cosTwo * innerFringeRadius, centerPos.y() + sinTwo * innerFringeRadius, 0);

				internalSetColor(opaqueColor);
				setPoint(int1);
				internalSetColor(fringeColor);
				setPoint(fInt1);
				setPoint(fInt2);
				internalSetColor(fringeColor);
				setPoint(fInt2);
				internalSetColor(opaqueColor);
				setPoint(int2);
				setPoint(int1);
			}
		}
		internalSetColor(opaqueColor);
	}
	
	public void circleRaw(final Vector3f centerPos, final float radius, final float angleStart, final float angleStop) {
		circleRaw(centerPos, radius, radius, angleStart, angleStop, true);
	}

	public void circleRaw(
			final Vector3f centerPos,
			final float radiusStart,
			final float radiusStop,
			final float angleStart,
			final float angleStop) {
		circleRaw(centerPos, radiusStart, radiusStop, angleStart, angleStop, true);
	}

	private void circleRaw(
			final Vector3f centerPos,
			final float radiusStart,
			final float radiusStop,
			final float angleStart,
			final float angleStop,
			final boolean antiAlias) {
		if (radiusStart < 0.001 && radiusStop < 0.001) {
			return;
		}
		final Color opaqueColor = this.tricolor[0];
		resetCount();
		int nbOcurence = (int) (FMath.max(radiusStart, radiusStop) * 2);
		if (nbOcurence < 20) {
			nbOcurence = 20;
		}
		final Color fringeColor = antiAlias ? withZeroAlpha(opaqueColor) : null;
		final float invertOccurence = 1.0f / nbOcurence;
		final float AA_FRINGE_ANGLE = 15.0f * FMath.PI / 180.0f;
		for (int iii = 0; iii < nbOcurence; iii++) {
			final float ratio = (float) iii * invertOccurence;
			final float radius = radiusStart + ((radiusStart - radiusStop) * iii * invertOccurence);
			final float angleOne = angleStart + (angleStop * iii * invertOccurence);
			final float angleTwo = angleStart + (angleStop * (iii + 1) * invertOccurence);

			final float cosOne = FMath.cos(angleOne);
			final float sinOne = FMath.sin(angleOne);
			final float cosTwo = FMath.cos(angleTwo);
			final float sinTwo = FMath.sin(angleTwo);

			// Solid triangle at original radius
			internalSetColor(opaqueColor);
			setPoint(new Vector3f(centerPos.x(), centerPos.y(), 0));
			setPoint(new Vector3f(centerPos.x() + cosOne * radius, centerPos.y() + sinOne * radius, 0));
			setPoint(new Vector3f(centerPos.x() + cosTwo * radius, centerPos.y() + sinTwo * radius, 0));

			if (antiAlias) {
				// Fringe ramps up from 0 at arc edges to full after AA_FRINGE_ANGLE
				final float arcAngle = angleStop * ratio;
				final float arcAngleEnd = angleStop * (1.0f - ratio);
				final float minEdgeAngle = Math.min(arcAngle, arcAngleEnd);
				final float fringeScale = Math.min(1.0f, minEdgeAngle / AA_FRINGE_ANGLE);
				final float fringe = AA_FRINGE * fringeScale;

				if (fringe > 0.3f) {
					final float fringeRadius = radius + fringe;
					final Vector3f e1 = new Vector3f(centerPos.x() + cosOne * radius, centerPos.y() + sinOne * radius, 0);
					final Vector3f e2 = new Vector3f(centerPos.x() + cosTwo * radius, centerPos.y() + sinTwo * radius, 0);
					final Vector3f f1 = new Vector3f(centerPos.x() + cosOne * fringeRadius, centerPos.y() + sinOne * fringeRadius, 0);
					final Vector3f f2 = new Vector3f(centerPos.x() + cosTwo * fringeRadius, centerPos.y() + sinTwo * fringeRadius, 0);

					internalSetColor(opaqueColor);
					setPoint(e1);
					internalSetColor(fringeColor);
					setPoint(f1);
					setPoint(f2);
					internalSetColor(fringeColor);
					setPoint(f2);
					internalSetColor(opaqueColor);
					setPoint(e2);
					setPoint(e1);
				}
			}
		}
		internalSetColor(opaqueColor);
	}

	public void circle(float radius, final float angleStart, float angleStop) {
		resetCount();
		
		if (radius < 0) {
			radius *= -1;
		}
		angleStop = angleStop - angleStart;
		
		final boolean hasBorder = this.thickness != 0 && this.color.a() != 0;
		// display background :
		if (this.colorBg.a() != 0) {
			internalSetColor(this.colorBg);
			circleRaw(this.position, radius, radius, angleStart, angleStop, !hasBorder);
		}

		// show if we have a border :
		if (!hasBorder) {
			return;
		}
		internalSetColor(this.color);
		circleBorderRaw(this.position, radius, this.thickness, angleStart, angleStop);
	}
	
	/**
	 * clear all the registered element in the current element
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
		OpenGL.enable(OpenGL.Flag.flag_blend);
		OpenGL.blendFuncAuto();
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
		OpenGL.disable(OpenGL.Flag.flag_blend);
	}
	
	@Override
	public void flush() {
		// push data on the VBO
		this.vbo.setPosition(this.outTriangles.toArray(Vector3f[]::new));
		this.vbo.setColors(this.outColors.toArray(Color[]::new));
		this.vbo.setVertexCount(this.outTriangles.size());
		// Request GPU upload
		this.vbo.flush();
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
	private static Color withZeroAlpha(final Color c) {
		return new Color(c.r(), c.g(), c.b(), 0f);
	}

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
		rectangle(dest.toVector3f());
	}
	
	/**
	 * draw a 2D rectangle to the position requested.
	 * @param dest Position the the end of the rectangle
	 */
	public void rectangle(final Vector3f dest) {
		internalSetColor(this.color);
		rectangleRaw(this.position, dest);
	}
	
	public void rectangleRaw(final Vector3f startPos, final Vector3f endPos) {
		resetCount();
		/*
		 * Bitmap position xA     xB
		 *              yC *------*
		 *                 |      |
		 *                 |      |
		 *              yD *------*
		 */
		float dxA = startPos.x();
		float dxB = endPos.x();
		if (dxA > dxB) {
			// inverse order :
			final float tmp = dxA;
			dxA = dxB;
			dxB = tmp;
		}
		float dyC = startPos.y();
		float dyD = endPos.y();
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
	
	public void drawQuad(final Vector3f pos1, final Vector3f pos2, final Vector3f pos3, final Vector3f pos4) {
		resetCount();
		setPoint(pos1);
		setPoint(pos2);
		setPoint(pos3);
		
		setPoint(pos1);
		setPoint(pos3);
		setPoint(pos4);
	}

	public void rectangleBorder(final Vector2f dest, final float borderWidth) {
		rectangleBorder(dest.toVector3f(), borderWidth);
	}

	public void rectangleBorder(final Vector3f dest, final float borderWidth) {
		/*
		 * Bitmap position xA             xB
		 *              yC *--------------* buttom
		 *                 |              |
		 *                 |   xpA    xpB |
		 *             ypC |   *------*   |
		 *                 |   |      |   |
		 *                 |   |      |   |
		 *      Left       |   |      |   |
		 *                 |   |      |   |
		 *             ypD |   *------*   |
		 *                 |              |
		 *                 |              |
		 *              yD *--------------* Top
		 */
		resetCount();
		internalSetColor(this.colorBg);
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
		float dxpA = dxA + borderWidth * 0.5f;
		float dxpB = dxB - borderWidth * 0.5f;
		float dypC = dyC + borderWidth * 0.5f;
		float dypD = dyD - borderWidth * 0.5f;
		dxA = dxA - borderWidth * 0.5f;
		dxB = dxB + borderWidth * 0.5f;
		dyC = dyC - borderWidth * 0.5f;
		dyD = dyD + borderWidth * 0.5f;
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
			if (dxpA < this.clippingPosStart.x()) {
				dxpA = this.clippingPosStart.x();
			}
			if (dxpB > this.clippingPosStop.x()) {
				dxpB = this.clippingPosStop.x();
			}
			if (dypC < this.clippingPosStart.y()) {
				dypC = this.clippingPosStart.y();
			}
			if (dypD > this.clippingPosStop.y()) {
				dypD = this.clippingPosStop.y();
			}
		}
		if (dyC >= dyD || dxA >= dxB) {
			return;
		}
		// Buttom border:
		setPoint(new Vector3f(dxA, dyC, 0));
		setPoint(new Vector3f(dxB, dyC, 0));
		setPoint(new Vector3f(dxpA, dypC, 0));

		setPoint(new Vector3f(dxB, dyC, 0));
		setPoint(new Vector3f(dxpA, dypC, 0));
		setPoint(new Vector3f(dxpB, dypC, 0));
		
		// Right border:
		setPoint(new Vector3f(dxpB, dypC, 0));
		setPoint(new Vector3f(dxB, dyC, 0));
		setPoint(new Vector3f(dxB, dyD, 0));

		setPoint(new Vector3f(dxpB, dypC, 0));
		setPoint(new Vector3f(dxB, dyD, 0));
		setPoint(new Vector3f(dxpB, dypD, 0));

		// Top border:
		setPoint(new Vector3f(dxpB, dypD, 0));
		setPoint(new Vector3f(dxB, dyD, 0));
		setPoint(new Vector3f(dxA, dyD, 0));

		setPoint(new Vector3f(dxpB, dypD, 0));
		setPoint(new Vector3f(dxA, dyD, 0));
		setPoint(new Vector3f(dxpA, dypD, 0));
		
		// Left border:
		setPoint(new Vector3f(dxpA, dypD, 0));
		setPoint(new Vector3f(dxA, dyD, 0));
		setPoint(new Vector3f(dxpA, dypC, 0));
		
		setPoint(new Vector3f(dxA, dyD, 0));
		setPoint(new Vector3f(dxpA, dypC, 0));
		setPoint(new Vector3f(dxA, dyC, 0));
	}
	
	/**
	 * draw a 2D rectangle to the position requested.
	 * @param dest Position the the end of the rectangle
	 */
	public void rectangleRadius(final Vector2f dest, final float radius) {
		rectangleRadius(dest.toVector3f(), radius);
	}
	
	public void rectangleRadius(final Vector3f dest, final float radius) {
		internalSetColor(this.color);
		final boolean showConstruct = false;
		
		rectangleRaw(this.position.add(new Vector3f(radius, 0, 0)), dest.less(new Vector3f(radius, 0, 0)));
		if (showConstruct) {
			internalSetColor(Color.ORANGE);
		}
		rectangleRaw(this.position.add(new Vector3f(0, radius, 0)),
				new Vector3f(this.position.x() + radius, dest.y() - radius, 0));
		if (showConstruct) {
			internalSetColor(Color.GRAY);
		}
		rectangleRaw(new Vector3f(dest.x() - radius, this.position.y() + radius, 0),
				new Vector3f(dest.x(), dest.y() - radius, 0));
		
		if (showConstruct) {
			internalSetColor(Color.AQUA_MARINE);
		}
		circleRaw(this.position.add(radius, radius, 0), radius, FMath.PI, FMath.PI * 0.5f);
		circleRaw(dest.less(radius, radius, 0), radius, 0, FMath.PI * 0.5f);
		circleRaw(new Vector3f(dest.x() - radius, this.position.y() + radius, 0), radius, FMath.PI * 1.5f,
				FMath.PI * 0.5f);
		circleRaw(new Vector3f(this.position.x() + radius, dest.y() - radius, 0), radius, FMath.PI * 0.5f,
				FMath.PI * 0.5f);
		
		if (showConstruct) {
			internalSetColor(Color.BLACK);
			rectangleRaw(this.position, this.position.add(10));
			internalSetColor(Color.RED);
			rectangleRaw(dest.less(10), dest);
		}
	}

	public void rectangleRadius(final Vector2f dest, final Insets thickness, final BorderRadius radius) {
		internalSetColor(this.color);
		final boolean showConstruct = false;

		if (showConstruct) {
			internalSetColor(Color.DARK_KHAKI);
		}
		// buttom-left
		final Vector3f centerBottomLeft = this.position.add(radius.bottomLeft(), radius.bottomLeft(), 0);
		circleRaw(centerBottomLeft, radius.bottomLeft(), FMath.PI, FMath.PI * 0.5f);
		if (showConstruct) {
			internalSetColor(Color.DARK_CYAN);
		}
		// Top right
		final Vector3f centerTopRight = dest.toVector3f().less(radius.topRight(), radius.topRight(), 0);
		circleRaw(centerTopRight, radius.topRight(), 0, FMath.PI * 0.5f);
		if (showConstruct) {
			internalSetColor(Color.DARK_OLIVEGREEN);
		}
		// bottom right
		final Vector3f centerBottomRight = new Vector3f(dest.x() - radius.bottomRight(),
				this.position.y() + radius.bottomRight(), 0);
		circleRaw(centerBottomRight, radius.bottomRight(), FMath.PI * 1.5f, FMath.PI * 0.5f);
		if (showConstruct) {
			internalSetColor(Color.DARK_ORANGE);
		}
		// top-left
		final Vector3f centerTopLeft = new Vector3f(this.position.x() + radius.topLeft(), dest.y() - radius.topLeft(),
				0);
		circleRaw(centerTopLeft, radius.topLeft(), FMath.PI * 0.5f, FMath.PI * 0.5f);
		
		// center area:
		if (showConstruct) {
			internalSetColor(Color.DARK_GOLDENROD);
		}
		drawQuad(centerBottomLeft, centerTopLeft, centerTopRight, centerBottomRight);
		
		// buttom area:
		if (showConstruct) {
			internalSetColor(Color.DARK_BLUE);
		}
		drawQuad(centerBottomLeft.less(0, radius.bottomLeft(), 0), centerBottomLeft, centerBottomRight,
				centerBottomRight.less(0, radius.bottomRight(), 0));
		// top area:
		if (showConstruct) {
			internalSetColor(Color.DARK_GRAY);
		}
		drawQuad(centerTopLeft.add(0, radius.topLeft(), 0), centerTopLeft, centerTopRight,
				centerTopRight.add(0, radius.topRight(), 0));
		
		// right area:
		if (showConstruct) {
			internalSetColor(Color.DARK_SLATE_GRAY);
		}
		drawQuad(centerBottomRight.add(radius.bottomRight(), 0, 0), centerBottomRight, centerTopRight,
				centerTopRight.add(radius.topRight(), 0, 0));
		// left area:
		if (showConstruct) {
			internalSetColor(Color.DARK_SLATE_BLUE);
		}
		drawQuad(centerBottomLeft.less(radius.bottomLeft(), 0, 0), centerBottomLeft, centerTopLeft,
				centerTopLeft.less(radius.topLeft(), 0, 0));
		
		if (showConstruct) {
			internalSetColor(Color.BLACK);
			rectangleRaw(this.position, this.position.add(10));
			internalSetColor(Color.RED);
			rectangleRaw(dest.toVector3f().less(10), dest.toVector3f());
		}
	}
	
	public void rectangleBorderRadius(
			//final Vector2f pos,
			final Vector2f dest,
			final Insets thickness,
			final BorderRadius radius) {
		internalSetColor(this.colorBg);
		final boolean showConstruct = false;
		
		if (showConstruct) {
			internalSetColor(Color.KHAKI);
		}
		// buttom-left
		final Vector3f centerBottomLeft = this.position.add(radius.bottomLeft(), radius.bottomLeft(), 0);
		circleBorderRaw(centerBottomLeft, radius.bottomLeft(), thickness.left(), thickness.bottom(), FMath.PI,
				FMath.PI * 0.5f);
		
		if (showConstruct) {
			internalSetColor(Color.CYAN);
		}
		// Top right
		final Vector3f centerTopRight = dest.toVector3f().less(radius.topRight(), radius.topRight(), 0);
		circleBorderRaw(centerTopRight, radius.topRight(), thickness.right(), thickness.top(), 0, FMath.PI * 0.5f);
		if (showConstruct) {
			internalSetColor(Color.OLIVE);
		}
		// bottom right
		final Vector3f centerBottomRight = new Vector3f(dest.x() - radius.bottomRight(),
				this.position.y() + radius.bottomRight(), 0);
		circleBorderRaw(centerBottomRight, radius.bottomRight(), thickness.bottom(), thickness.right(), FMath.PI * 1.5f,
				FMath.PI * 0.5f);
		if (showConstruct) {
			internalSetColor(Color.ORANGE);
		}
		// top-left
		final Vector3f centerTopLeft = new Vector3f(this.position.x() + radius.topLeft(), dest.y() - radius.topLeft(),
				0);
		circleBorderRaw(centerTopLeft, radius.topLeft(), thickness.top(), thickness.left(), FMath.PI * 0.5f,
				FMath.PI * 0.5f);
		
		// buttom area:
		if (showConstruct) {
			internalSetColor(Color.BLUE);
		}
		drawQuad(centerBottomLeft.less(0, radius.bottomLeft(), 0), //
				centerBottomLeft.less(0, radius.bottomLeft() - thickness.bottom(), 0), //
				centerBottomRight.less(0, radius.bottomRight() - thickness.bottom(), 0), //
				centerBottomRight.less(0, radius.bottomRight(), 0));
		// top area:
		if (showConstruct) {
			internalSetColor(Color.GRAY);
		}
		drawQuad(centerTopLeft.add(0, radius.topLeft(), 0), //
				centerTopLeft.add(0, radius.topLeft() - thickness.top(), 0), //
				centerTopRight.add(0, radius.topRight() - thickness.top(), 0), //
				centerTopRight.add(0, radius.topRight(), 0));
		
		// right area:
		if (showConstruct) {
			internalSetColor(Color.SLATE_GRAY);
		}
		drawQuad(centerBottomRight.add(radius.bottomRight(), 0, 0), //
				centerBottomRight.add(radius.bottomRight() - thickness.right(), 0, 0), //
				centerTopRight.add(radius.topRight() - thickness.right(), 0, 0), //
				centerTopRight.add(radius.topRight(), 0, 0));
		// left area:
		if (showConstruct) {
			internalSetColor(Color.SLATE_BLUE);
		}
		drawQuad(centerBottomLeft.less(radius.bottomLeft(), 0, 0), //
				centerBottomLeft.less(radius.bottomLeft() - thickness.left(), 0, 0), //
				centerTopLeft.less(radius.topLeft() - thickness.left(), 0, 0), //
				centerTopLeft.less(radius.topLeft(), 0, 0));

		if (showConstruct) {
			internalSetColor(Color.BLACK);
			rectangleRaw(this.position, this.position.add(10));
			internalSetColor(Color.RED);
			rectangleRaw(dest.toVector3f().less(10), dest.toVector3f());
		}
	}

	public void rectangleBorderRadius(final Vector2f dest, final float thickness, final float radius) {
		rectangleBorderRadius(dest.toVector3f(), thickness, radius);
	}

	public void rectangleBorderRadius(final Vector3f dest, final float thickness, final float radius) {
		internalSetColor(this.colorBg);
		final boolean showConstruct = false;
		if (showConstruct) {
			internalSetColor(Color.ANTIQUE_WHITE);
		}
		// Bottom
		rectangleRaw(new Vector3f(this.position.x() + radius, this.position.y() - thickness * 0.5f, 0),
				new Vector3f(dest.x() - radius, this.position.y() + thickness * 0.5f, 0));
		// top
		rectangleRaw(new Vector3f(this.position.x() + radius, dest.y() - thickness * 0.5f, 0),
				new Vector3f(dest.x() - radius, dest.y() + thickness * 0.5f, 0));
		// left
		rectangleRaw(new Vector3f(this.position.x() - thickness * 0.5f, this.position.y() + radius, 0),
				new Vector3f(this.position.x() + thickness * 0.5f, dest.y() - radius, 0));
		// right
		rectangleRaw(new Vector3f(dest.x() - thickness * 0.5f, this.position.y() + radius, 0),
				new Vector3f(dest.x() + thickness * 0.5f, dest.y() - radius, 0));
		
		if (showConstruct) {
			internalSetColor(Color.DARK_RED);
		}
		circleBorderRaw(this.position.add(radius, radius, 0), radius, thickness, FMath.PI, FMath.PI * 0.5f);
		circleBorderRaw(dest.less(radius, radius, 0), radius, thickness, 0, FMath.PI * 0.5f);
		circleBorderRaw(new Vector3f(dest.x() - radius, this.position.y() + radius, 0), radius, thickness,
				FMath.PI * 1.5f, FMath.PI * 0.5f);
		circleBorderRaw(new Vector3f(this.position.x() + radius, dest.y() - radius, 0), radius, thickness,
				FMath.PI * 0.5f, FMath.PI * 0.5f);
		
		if (showConstruct) {
			internalSetColor(Color.BLACK);
			rectangleRaw(this.position, this.position.add(10));
			internalSetColor(Color.RED);
			rectangleRaw(dest.less(10), dest);
		}
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
	 * Set the color used for the next drawing primitives (drawQuad, circleBorderRaw, etc.).
	 * Unlike {@link #setColor(Color)} which only stores the foreground color,
	 * this method immediately applies the color to the triangle color buffer.
	 * @param color Color to apply for next drawing operations
	 */
	public void setDrawingColor(final Color color) {
		internalSetColor(color);
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