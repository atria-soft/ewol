
/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.gale.resource.ResourceVirtualBufferObject;

class Drawing extends Compositing {
	
	private Vector3f position = new Vector3f(0, 0, 0); //!< The current position to draw
	private final Vector3f clippingPosStart = new Vector3f(0, 0, 0); //!< Clipping start position
	private final Vector3f clippingPosStop = new Vector3f(0, 0, 0); //!< Clipping stop position
	private boolean clippingEnable = false; //!< true if the clipping must be activated
	private Color color = Color.BLACK.clone(); //!< The text foreground color
	private Color colorBg = Color.NONE.clone(); //!< The text background color
	private ResourceProgram GLprogram; //!< pointer on the opengl display program
	private final int GLPosition = -1; //!< openGL id on the element (vertex buffer)
	private final int GLMatrix = -1; //!< openGL id on the element (transformation matrix)
	private final int GLMatrixPosition = -1; //!< position matrix
	private final int GLColor = -1; //!< openGL id on the element (color buffer)
	protected static int vboIdCoord = 0;
	protected static int vboIdColor = 1;
	protected ResourceVirtualBufferObject VBO;
	
	/**
	 * @brief Basic ructor
	 */
	public Drawing() {
		loadProgram();
		for (int iii = 0; iii < 3; iii++) {
			this.triangle[iii] = this.position;
			this.tricolor[iii] = this.color;
		}
		// Create the VBO:
		this.VBO = ResourceVirtualBufferObject.create(4);
		if (this.VBO == null) {
			Log.error("can not instanciate VBO ...");
			return;
		}
		// TO facilitate some debugs we add a name of the VBO:
		this.VBO.setName("[VBO] of ewol::compositing::Area");
	}
	
	/**
				 * @brief load the openGL program and get all the ID needed
				 */
	private void loadProgram();
	
	/**
				 * @brief Un-Load the openGL program and get all the ID needed
				 */
	private void unLoadProgram();
	
	private final float thickness = 0; //!< when drawing line and other things
	private final int triElement = 0; //!< special counter of the single dot generated
	private final Vector3f[] triangle = new Vector3f[3]; //!< Register every system with a combinaison of tiangle
	private final Color[] tricolor = new Color[3]; //!< Register every the associated color foreground
	// internal API for the generation abstraction of triangles
	
	/**
				 * @brief Lunch the generation of triangle
				 */
	private void generateTriangle() {
		this.triElement = 0;
		
		this.VBO.pushOnBuffer(this.vboIdCoord, this.triangle[0]);
		this.VBO.pushOnBuffer(this.vboIdColor, this.tricolor[0]);
		this.VBO.pushOnBuffer(this.vboIdCoord, this.triangle[1]);
		this.VBO.pushOnBuffer(this.vboIdColor, this.tricolor[1]);
		this.VBO.pushOnBuffer(this.vboIdCoord, this.triangle[2]);
		this.VBO.pushOnBuffer(this.vboIdColor, this.tricolor[2]);
	};
	
	/**
				 * @brief in case of some error the count can be reset
				 */
	private void resetCount();
	
	/**
				 * @brief set the Color of the current triangle drawing
				 * @param[in] _color Color to current dots generated
				 */
	private void internalSetColor(Color _color);
	
	/**
				 * @brief internal add of the specific point
				 * @param[in] _point The requeste dpoint to add
				 */
	private void setPoint(Vector3f point);
	
	/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
		@Override
		public void draw(final boolean _disableDepthTest=true);
	
	/**
	 * @brief clear alll tre registered element in the current element
	 */
	@Override
	public void clear();
	
	/**
	 * @brief get the current display position (sometime needed in the gui control)
	 * @return the current position.
	 */
	public Vector3f getPos() {
		return this.position;
	};
	
	/**
	 * @brief set position for the next text writen
	 * @param[in] _pos Position of the text (in 3D)
	 */
	public void setPos(final Vector3f _pos) {
		this.position = _pos;
	};
	
	public void setPos(final Vector2f _pos) {
		setPos(Vector3f(_pos.x(), _pos.y(), 0));
	};
	
	/**
	 * @brief set relative position for the next text writen
	 * @param[in] _pos ofset apply of the text (in 3D)
	 */
	public void setRelPos(final Vector3f _pos) {
		this.position += _pos;
	};
	
	public void setRelPos(final Vector2f _pos) {
		setRelPos(Vector3f(_pos.x(), _pos.y(), 0));
	};
	
	/**
				 * @brief set the Color of the current foreground font
				 * @param[in] _color Color to set on foreground (for next print)
				 */
	public void setColor(final Color _color) {
		this.color = _color;
	};
	
	/**
	 * @brief Get the foreground color of the font.
	 * @return Foreground color.
	 */
	public Color getColor() {
		return this.color;
	};
	
	/**
				 * @brief set the background color of the font (for selected Text (not the global BG))
				 * @param[in] _color Color to set on background (for next print)
				 */
	public void setColorBg(final Color _color) {
		this.colorBg = _color;
	};
	
	/**
	 * @brief Get the background color of the font.
	 * @return Background color.
	 */
	public Color getColorBg() {
		return this.colorBg;
	};
	
	/**
	 * @brief Request a clipping area for the text (next draw only)
	 * @param[in]_ pos Start position of the clipping
	 * @param[in] _width Width size of the clipping
	 */
	public void setClippingWidth(final Vector3f _pos, final Vector3f _width) {
		setClipping(_pos, _pos + _width);
	};
	
	public void setClippingWidth(final Vector2f _pos, final Vector2f _width) {
		setClippingWidth(Vector3f(_pos.x(), _pos.y(), -1), Vector3f(_width.x(), _width.y(), 2));
	};
	
	/**
	 * @brief Request a clipping area for the text (next draw only)
	 * @param[in] _pos Start position of the clipping
	 * @param[in] _posEnd End position of the clipping
	 */
	public void setClipping(Vector3f _pos, Vector3f _posEnd);
	
	public void setClipping(final Vector2f _pos, final Vector2f _posEnd) {
		setClipping(Vector3f(_pos.x(), _pos.y(), -1), Vector3f(_posEnd.x(), _posEnd.y(), 1));
	};
	
	/**
	 * @brief enable/Disable the clipping (without lose the current clipping position)
	 * @brief _newMode The new status of the clipping
	 */
	public void setClippingMode(final boolean _newMode) {
		this.clippingEnable = _newMode;
	};
	
	/**
	 * @brief Specify the line thickness for the next elements
	 * @param[in] _thickness The thickness disired for the next print
	 */
	public void setThickness(float _thickness);
	
	/**
	 * @brief add a point reference at the current position (this is a vertex reference at the current position
	 */
	public void addVertex();
	
	/**
	 * @brief draw a line to a specific position
	 * @param[in] _dest Position of the end of the line.
	 */
	public void lineTo(Vector3f _dest);
	
	public void lineTo(final Vector2f _dest) {
		lineTo(new Vector3f(_dest.x, _dest.y, 0));
	};
	
	/**
	 * @brief Relative drawing a line (spacial vector)
	 * @param[in] _vect Vector of the curent line.
	 */
	public void lineRel(final Vector3f _vect) {
		lineTo(this.position.addNew(_vect));
	};
	
	public void lineRel(final Vector2f _vect) {
		lineRel(new Vector3f(_vect.x, _vect.y, 0));
	};
	
	/**
	 * @brief draw a 2D rectangle to the position requested.
	 * @param[in] _dest Position the the end of the rectangle
	 */
	public void rectangle(Vector3f _dest);
	
	public void rectangle(final Vector2f _dest) {
		rectangle(Vector3f(_dest.x(), _dest.y(), 0));
	};
	
	/**
	 * @brief draw a 2D rectangle to the requested size.
	 * @param[in] _size size of the rectangle
	 */
	public void rectangleWidth(final Vector3f _size) {
		rectangle(this.position + _size);
	};
	
	public void rectangleWidth(final Vector2f _size) {
		rectangleWidth(Vector3f(_size.x(), _size.y(), 0));
	};
	
	/**
	 * @brief draw a 3D rectangle to the position requested.
	 * @param[in] _dest Position the the end of the rectangle
	 */
	public void cube(Vector3f _dest);
	
	/**
				 * @brief draw a 2D circle with the specify rafdius parameter.
				 * @param[in] _radius Distence to the dorder
				 * @param[in] _angleStart start angle of this circle ([0..2PI] otherwithe  == > disable)
				 * @param[in] _angleStop stop angle of this circle ([0..2PI] otherwithe  == > disable)
				 */
	public void circle(final float _radius, final float _angleStart = 0, float _angleStop = 2*M_PI);
		}
