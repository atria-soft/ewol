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
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.gale.resource.ResourceVirtualBufferObject;

public class CompositingDrawing extends Compositing {

	protected static int vboIdCoord = 0;
	protected static int vboIdColor = 1;
	private Vector3f position = new Vector3f(0, 0, 0); // !< The current position to draw
	private Vector3f clippingPosStart = new Vector3f(0, 0, 0); // !< Clipping start position
	private Vector3f clippingPosStop = new Vector3f(0, 0, 0); // !< Clipping stop position
	private boolean clippingEnable = false; // !< true if the clipping must be activated
	private Color color = Color.BLACK; // !< The text foreground color
	private Color colorBg = Color.NONE; // !< The text background color
	private ResourceProgram GLprogram; // !< pointer on the opengl display program
	private int GLPosition = -1; // !< openGL id on the element (vertex buffer)
	private int GLMatrix = -1; // !< openGL id on the element (transformation matrix)
	private int GLMatrixPosition = -1; // !< position matrix
	private int GLColor = -1; // !< openGL id on the element (color buffer)
	protected ResourceVirtualBufferObject VBO;

	private float thickness = 0; // !< when drawing line and other things

	private int triElement = 0; // !< special counter of the single dot generated

	private final Vector3f[] triangle = new Vector3f[3]; // !< Register every system with a combinaison of tiangle

	private final Color[] tricolor = new Color[3]; // !< Register every the associated color foreground
	private final List<Vector3f> outTriangles = new ArrayList<>();
	private final List<Color> outColors = new ArrayList<>();

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
		this.VBO = ResourceVirtualBufferObject.create(4);
		// TO facilitate some debugs we add a name of the VBO:
		this.VBO.setName("[VBO] of ewol::compositing::Area");
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
	 * @param _radius Distence to the dorder
	 * @param _angleStart start angle of this circle ([0..2PI] otherwithe == >
	 *            disable)
	 * @param _angleStop stop angle of this circle ([0..2PI] otherwithe == >
	 *            disable)
	 */
	public void circle(final float _radius) {
		circle(_radius, 0);
	}

	public void circle(final float _radius, final float _angleStart) {
		circle(_radius, _angleStart, 2.0f * FMath.PI);
	};

	public void circle(float _radius, final float _angleStart, float _angleStop) {
		resetCount();

		if (_radius < 0) {
			_radius *= -1;
		}
		_angleStop = _angleStop - _angleStart;

		int nbOcurence = (int) _radius;
		if (nbOcurence < 10) {
			nbOcurence = 10;
		}

		// display background :
		if (this.colorBg.a() != 0) {
			internalSetColor(this.colorBg);
			for (int iii = 0; iii < nbOcurence; iii++) {
				setPoint(new Vector3f(this.position.x(), this.position.y(), 0));

				float angleOne = _angleStart + (_angleStop * iii / nbOcurence);
				float offsety = FMath.sin(angleOne) * _radius;
				float offsetx = FMath.cos(angleOne) * _radius;

				setPoint(new Vector3f(this.position.x() + offsetx, this.position.y() + offsety, 0));

				float angleTwo = _angleStart + (_angleStop * (iii + 1) / nbOcurence);
				offsety = FMath.sin(angleTwo) * _radius;
				offsetx = FMath.cos(angleTwo) * _radius;

				setPoint(new Vector3f(this.position.x() + offsetx, this.position.y() + offsety, 0));
			}
		}

		// show if we have a border :
		if (this.thickness == 0 || this.color.a() == 0) {
			return;
		}
		internalSetColor(this.color);
		for (int iii = 0; iii < nbOcurence; iii++) {

			float angleOne = _angleStart + (_angleStop * iii / nbOcurence);
			float offsetExty = FMath.sin(angleOne) * (_radius + this.thickness / 2);
			float offsetExtx = FMath.cos(angleOne) * (_radius + this.thickness / 2);
			float offsetInty = FMath.sin(angleOne) * (_radius - this.thickness / 2);
			float offsetIntx = FMath.cos(angleOne) * (_radius - this.thickness / 2);

			float angleTwo = _angleStart + (_angleStop * (iii + 1) / nbOcurence);
			float offsetExt2y = FMath.sin(angleTwo) * (_radius + this.thickness / 2);
			float offsetExt2x = FMath.cos(angleTwo) * (_radius + this.thickness / 2);
			float offsetInt2y = FMath.sin(angleTwo) * (_radius - this.thickness / 2);
			float offsetInt2x = FMath.cos(angleTwo) * (_radius - this.thickness / 2);

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
		this.VBO.clear();
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
	public void draw(final boolean _disableDepthTest) {

		// push data on the VBO
		// TODO optimize this with single push when needed
		this.VBO.setVboData(CompositingDrawing.vboIdCoord, this.outTriangles.toArray(Vector3f[]::new));
		this.VBO.setVboData(CompositingDrawing.vboIdColor, this.outColors.toArray(Color[]::new));
		this.VBO.flush();

		if (this.GLprogram == null) {
			Log.error("No shader ...");
			return;
		}
		// set Matrix : translation/positionMatrix
		Matrix4f tmpMatrix = OpenGL.getMatrix().multiply(this.matrixApply);
		this.GLprogram.use();
		this.GLprogram.uniformMatrix(this.GLMatrix, tmpMatrix);
		this.GLprogram.uniformMatrix(this.GLMatrixPosition, Matrix4f.IDENTITY);
		// position:
		this.GLprogram.sendAttributePointer(this.GLPosition, this.VBO, CompositingDrawing.vboIdCoord);
		// color:
		this.GLprogram.sendAttributePointer(this.GLColor, this.VBO, CompositingDrawing.vboIdColor);
		// Request the draw od the elements :
		OpenGL.drawArrays(OpenGL.RenderMode.triangle, 0, this.VBO.bufferSize(CompositingDrawing.vboIdCoord));
		this.GLprogram.unUse();
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
	};

	/**
	 * get the current display position (sometime needed in the gui control)
	 * @return the current position.
	 */
	public Vector3f getPos() {
		return this.position;
	};

	/**
	 * set the Color of the current triangle drawing
	 * @param _color Color to current dots generated
	 */
	private void internalSetColor(final Color _color) {
		if (this.triElement < 1) {
			this.tricolor[0] = _color;
		}
		if (this.triElement < 2) {
			this.tricolor[1] = _color;
		}
		if (this.triElement < 3) {
			this.tricolor[2] = _color;
		}
	};

	public void lineRel(final Vector2f _vect) {
		lineRel(new Vector3f(_vect.x(), _vect.y(), 0));
	};

	/**
	 * Relative drawing a line (spacial vector)
	 * @param _vect Vector of the curent line.
	 */
	public void lineRel(final Vector3f _vect) {
		lineTo(this.position.add(_vect));
	};

	public void lineTo(final Vector2f _dest) {
		lineTo(new Vector3f(_dest.x(), _dest.y(), 0));
	};

	/**
	 * draw a line to a specific position
	 * @param _dest Position of the end of the line.
	 */
	public void lineTo(final Vector3f _dest) {
		resetCount();
		internalSetColor(this.color);
		// Log.verbose("DrawLine : " + this.position + " to " + _dest);
		if (this.position.x() == _dest.x() && this.position.y() == _dest.y() && this.position.z() == _dest.z()) {
			// Log.warning("Try to draw a line width 0");
			return;
		}
		// teta = tan-1(oposer/adjacent)
		float teta = 0;
		if (this.position.x() <= _dest.x()) {
			teta = FMath.atan((_dest.y() - this.position.y()) / (_dest.x() - this.position.x()));
		} else {
			teta = FMath.PI + FMath.atan((_dest.y() - this.position.y())) / (_dest.x() - this.position.x());
		}
		if (teta < 0) {
			teta += 2 * FMath.PI;
		} else if (teta > 2 * FMath.PI) {
			teta -= 2 * FMath.PI;
		}
		// Log.debug("teta = " + (teta*180/(FMath.PI)) + " deg." );
		float offsety = FMath.sin(teta - FMath.PI / 2) * (this.thickness / 2);
		float offsetx = FMath.cos(teta - FMath.PI / 2) * (this.thickness / 2);
		setPoint(new Vector3f(this.position.x() - offsetx, this.position.y() - offsety, this.position.z()));
		setPoint(new Vector3f(this.position.x() + offsetx, this.position.y() + offsety, this.position.z()));
		setPoint(new Vector3f(_dest.x() + offsetx, _dest.y() + offsety, this.position.z()));

		setPoint(new Vector3f(_dest.x() + offsetx, _dest.y() + offsety, _dest.z()));
		setPoint(new Vector3f(_dest.x() - offsetx, _dest.y() - offsety, _dest.z()));
		setPoint(new Vector3f(this.position.x() - offsetx, this.position.y() - offsety, _dest.z()));
		// update the system position :
		this.position = _dest;
	};

	/**
	 * load the openGL program and get all the ID needed
	 */
	private void loadProgram() {
		// remove previous loading ... in case
		unLoadProgram();
		// oad the new ...
		this.GLprogram = ResourceProgram.create(new Uri("DATA", "color3.vert", "ewol"),
				new Uri("DATA", "color3.frag", "ewol"));
		// get the shader resource :
		if (this.GLprogram != null) {
			this.GLPosition = this.GLprogram.getAttribute("EW_coord3d");
			this.GLColor = this.GLprogram.getAttribute("EW_color");
			this.GLMatrix = this.GLprogram.getUniform("EW_MatrixTransformation");
			this.GLMatrixPosition = this.GLprogram.getUniform("EW_MatrixPosition");
		}
	};

	public void rectangle(final Vector2f _dest) {
		rectangle(new Vector3f(_dest.x(), _dest.y(), 0));
	};

	/**
	 * draw a 2D rectangle to the position requested.
	 * @param _dest Position the the end of the rectangle
	 */
	public void rectangle(final Vector3f _dest) {
		resetCount();
		internalSetColor(this.color);
		/*
		 * Bitmap position xA xB yC *------* | | | | yD *------*
		 */
		float dxA = this.position.x();
		float dxB = _dest.x();
		if (dxA > dxB) {
			// inverse order :
			float tmp = dxA;
			dxA = dxB;
			dxB = tmp;
		}
		float dyC = this.position.y();
		float dyD = _dest.y();
		if (dyC > dyD) {
			// inverse order :
			float tmp = dyC;
			dyC = dyD;
			dyD = tmp;
		}
		if (true == this.clippingEnable) {
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
	};

	public void rectangleWidth(final Vector2f _size) {
		rectangleWidth(new Vector3f(_size.x(), _size.y(), 0));
	};

	/**
	 * draw a 2D rectangle to the requested size.
	 * @param _size size of the rectangle
	 */
	public void rectangleWidth(final Vector3f _size) {
		rectangle(this.position.add(_size));
	}

	/**
	 * in case of some error the count can be reset
	 */
	private void resetCount() {
		this.triElement = 0;
	};

	public void setClipping(final Vector2f _pos, final Vector2f _posEnd) {
		setClipping(new Vector3f(_pos.x(), _pos.y(), -1), new Vector3f(_posEnd.x(), _posEnd.y(), 1));
	};

	/**
	 * Request a clipping area for the text (next draw only)
	 * @param _pos Start position of the clipping
	 * @param _posEnd End position of the clipping
	 */
	public void setClipping(final Vector3f _pos, final Vector3f _posEnd) {
		// note the internal system all time request to have a bounding all time in the
		// same order
		this.clippingPosStop = Vector3f.max(_pos, _posEnd);
		this.clippingPosStart = Vector3f.min(_pos, _posEnd);
		this.clippingEnable = true;
	}

	/**
	 * enable/Disable the clipping (without lose the current clipping
	 *        position)
	 * _newMode The new status of the clipping
	 */
	public void setClippingMode(final boolean _newMode) {
		this.clippingEnable = _newMode;
	}

	public void setClippingWidth(final Vector2f _pos, final Vector2f _width) {
		setClippingWidth(new Vector3f(_pos.x(), _pos.y(), -1), new Vector3f(_width.x(), _width.y(), 2));
	}

	/**
	 * Request a clipping area for the text (next draw only)
	 * @param_ pos Start position of the clipping
	 * @param _width Width size of the clipping
	 */
	public void setClippingWidth(final Vector3f _pos, final Vector3f _width) {
		setClipping(_pos, _pos.add(_width));
	};

	/**
	 * set the Color of the current foreground font
	 * @param _color Color to set on foreground (for next print)
	 */
	public void setColor(final Color _color) {
		this.color = _color;
	};

	/**
	 * set the background color of the font (for selected Text (not the
	 *        global BG))
	 * @param _color Color to set on background (for next print)
	 */
	public void setColorBg(final Color _color) {
		this.colorBg = _color;
	};

	/**
	 * internal add of the specific point
	 * @param _point The requeste dpoint to add
	 */
	private void setPoint(final Vector3f point) {
		this.triangle[this.triElement] = point;
		this.triElement++;
		if (this.triElement >= 3) {
			generateTriangle();
		}
		this.VBO.flush();
	}

	public void setPos(final Vector2f _pos) {
		setPos(new Vector3f(_pos.x(), _pos.y(), 0));
	};

	/**
	 * set position for the next text writen
	 * @param _pos Position of the text (in 3D)
	 */
	public void setPos(final Vector3f _pos) {
		this.position = _pos;
	};

	public void setRelPos(final Vector2f _pos) {
		setRelPos(new Vector3f(_pos.x(), _pos.y(), 0));
	}

	/**
	 * set relative position for the next text writen
	 * @param _pos ofset apply of the text (in 3D)
	 */
	public void setRelPos(final Vector3f _pos) {
		this.position = this.position.add(_pos);
	}

	/**
	 * Specify the line thickness for the next elements
	 * @param _thickness The thickness disired for the next print
	 */
	public void setThickness(final float _thickness) {
		this.thickness = _thickness;
		// thickness must be positive
		if (this.thickness < 0) {
			this.thickness *= -1;
		}
	}

	/**
	 * Un-Load the openGL program and get all the ID needed
	 */
	private void unLoadProgram() {
		this.GLprogram = null;
	}

}