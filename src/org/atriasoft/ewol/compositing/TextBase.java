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
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etk.util.Dynamic;
import org.atriasoft.ewol.compositing.tools.TextDecoration;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.font.FontMode;
import org.atriasoft.ewol.resource.font.GlyphProperty;
import org.atriasoft.exml.Exml;
import org.atriasoft.exml.exception.ExmlAttributeDoesNotExist;
import org.atriasoft.exml.exception.ExmlBuilderException;
import org.atriasoft.exml.exception.ExmlNodeDoesNotExist;
import org.atriasoft.exml.exception.ExmlParserErrorMulti;
import org.atriasoft.exml.model.XmlElement;
import org.atriasoft.exml.model.XmlNode;
import org.atriasoft.gale.resource.ResourceProgram;
import org.atriasoft.gale.resource.ResourceVirtualBufferObject;

public abstract class TextBase extends Compositing {
	private static final int NBVBO = 4;
	protected static int vboIdColor = 2;
	// Text
	protected static int vboIdCoord = 0;
	protected static int vboIdCoordText = 1;
	protected static int vboIdGlyphLevel = 3;
	// previously this line and the center is perform with this one)
	protected AlignMode alignment = AlignMode.alignDisable; // !< Current Alignment mode (justify/left/right ...)
	protected boolean clippingEnable = false; // !< true if the clipping must be activated
	protected Vector3f clippingPosStart = Vector3f.ZERO; // !< Clipping start position
	protected Vector3f clippingPosStop = Vector3f.ZERO; // !< Clipping stop position
	protected Color color = Color.BLACK; // !< The text foreground color
	protected Color colorBg = Color.NONE; // !< The text background color
	protected Color colorCursor = Color.BLACK; // !< The text cursor color
	protected Color colorSelection = Color.OLIVE; // !< The text Selection color
	// selection)
	protected int cursorPos = -100; // !< Cursor position (default no cursor == > -100)
	protected Color defaultColorBg = Color.NONE; // !< The text background color
	protected Color defaultColorFg = Color.BLACK; // !< The text foreground color
	// this section is reserved for HTML parsing and display:
	public String htmlCurrentLine = ""; // !< current line for HTML display
	
	public List<TextDecoration> htmlDecoration = new ArrayList<>(); // !< current decoration for the HTML display
	public TextDecoration htmlDecoTmp = new TextDecoration(); // !< current decoration
	protected boolean kerning = true; // !< Kerning enable or disable on the next elements displayed
	protected FontMode mode = FontMode.Regular; // !< font display property : Regular/Bold/Italic/BoldItalic
	protected int nbCharDisplayed; // !< prevent some error in calculation size.
	protected boolean needDisplay; // !< This just need the display and not the size rendering.
	protected int oGLColor = -1; // !< openGL id on the element (color buffer)
	protected int oGLMatrix = -1; // !< openGL id on the element (transformation matrix)
	protected int oGLPosition = -1; // !< openGL id on the element (vertex buffer)
	protected ResourceProgram oGLprogram; // !< pointer on the opengl display program
	protected int oGLtexID = -1; // !< openGL id on the element (texture ID)
	protected int oGLtextHeight = -1; // !< openGL Id on the texture height
	protected int oGLtexture = -1; // !< openGL id on the element (Texture position)
	protected int oGLtextWidth = -1; // !< openGL Id on the texture width
	protected Vector3f position = Vector3f.ZERO; // !< The current position to draw
	protected Character previousCharcode; // !< we remember the previous charcode to perform the kerning. @ref Kerning
	protected int selectionStartPos = -100; // !< start position of the Selection (if == this.cursorPos ==> no
	protected Vector3f sizeDisplayStart = Vector3f.ZERO; // !< The start windows of the display.
	protected Vector3f sizeDisplayStop = Vector3f.ZERO; // !< The end windows of the display.
	protected float startTextPos = 0; // !< start position of the Alignment (when \n the text return at this
	// position)
	protected float stopTextPos = 0; // !< end of the alignment (when a string is too height it cut at the word
	protected ResourceVirtualBufferObject vbo;
	protected CompositingDrawing vectorialDraw = new CompositingDrawing();
	
	/**
	 * generic constructor
	 */
	public TextBase() {
		this(new Uri("DATA", "text.vert", "ewol"), new Uri("DATA", "text.frag", "ewol"));
	}
	
	public TextBase(final Uri vertexShader, final Uri fragmentShader) {
		this(vertexShader, fragmentShader, true);
	}
	
	public TextBase(final Uri vertexShader, final Uri fragmentShader, final boolean loadProgram) {
		if (loadProgram) {
			loadProgram(vertexShader, fragmentShader);
		}
		// Create the VBO:
		this.vbo = ResourceVirtualBufferObject.create(TextBase.NBVBO);
		// TO facilitate some debugs we add a name of the VBO:
		this.vbo.setName("[VBO] of super.TextBase");
	}
	
	/**
	 * calculate a theoric charcode size
	 * @param charcode The Unicode value to calculate dimention.
	 * @return The theoric size used.
	 */
	public Vector3f calculateSize(final Character charcode) {
		return calculateSizeChar(charcode);
	}
	
	/**
	 * calculate a theoric text size
	 * @param text The string to calculate dimention.
	 * @return The theoric size used.
	 */
	public Vector3f calculateSize(final String text) {
		Vector3f outputSize = Vector3f.ZERO;
		for (int iii = 0; iii < text.length(); iii++) {
			final Vector3f tmpp = calculateSize(text.charAt(iii));
			if (outputSize.y() == 0) {
				outputSize = outputSize.withY(tmpp.y());
			}
			outputSize = outputSize.withX(outputSize.x() + tmpp.x());
		}
		return outputSize;
	}
	
	// ! @previous
	public abstract Vector3f calculateSizeChar(Character charcode);
	
	/**
	 * calculate a theoric text size
	 * @param text The string to calculate dimention.
	 * @return The theoric size used.
	 */
	public Vector3f calculateSizeDecorated(final String text) {
		if (text.length() == 0) {
			return Vector3f.ZERO;
		}
		
		String tmpData = "<html><body>\n";
		tmpData += text;
		tmpData += "\n</body></html>\n";
		return calculateSizeHTML(tmpData);
	}
	
	/**
	 * calculate a theoric text size
	 * @param text The string to calculate dimention.
	 * @return The theoric size used.
	 */
	public Vector3f calculateSizeHTML(final String text) {
		// remove intermediate result
		reset();
		// Log.debug(" 0 size for=\n" + text);
		// disable display system
		this.needDisplay = false;
		
		setPos(Vector3f.ZERO);
		// same as print without the end display ...
		printHTML(text);
		// Log.debug(" 1 Start pos=" + this.sizeDisplayStart);
		// Log.debug(" 1 Stop pos=" + this.sizeDisplayStop);
		
		// get the last elements
		this.sizeDisplayStop = Vector3f.max(this.position, this.sizeDisplayStop);
		this.sizeDisplayStart = Vector3f.min(this.position, this.sizeDisplayStart);
		
		// Log.debug(" 2 Start pos=" + this.sizeDisplayStart);
		// Log.debug(" 2 Stop pos=" + this.sizeDisplayStop);
		// set back the display system
		this.needDisplay = true;
		
		return new Vector3f(this.sizeDisplayStop.x() - this.sizeDisplayStart.x(), this.sizeDisplayStop.y() - this.sizeDisplayStart.y(), this.sizeDisplayStop.z() - this.sizeDisplayStart.z());
	}
	
	/**
	 * clear all the registered element in the current element
	 */
	@Override
	public void clear() {
		// call upper class
		super.clear();
		// remove sub draw system
		this.vectorialDraw.clear();
		// reset Buffer:
		this.vbo.clear();
		// reset temporal variables:
		reset();
	}
	
	/**
	 * disable the alignement system
	 */
	public void disableAlignement() {
		this.alignment = AlignMode.alignDisable;
	}
	
	/**
	 * remove the cursor display
	 */
	public void disableCursor() {
		this.selectionStartPos = -100;
		this.cursorPos = -100;
	}
	
	/**
	 * draw All the registered text in the current element on openGL
	 */
	@Override
	public void draw(final boolean disableDepthTest) {
		drawD(disableDepthTest);
	}
	
	// ! @previous
	public void draw(final Matrix4f transformationMatrix, final boolean enableDepthTest) {
		drawMT(transformationMatrix, enableDepthTest);
	}
	
	/**
	 * draw All the refistered text in the current element on openGL
	 */
	public abstract void drawD(final boolean disableDepthTest);
	
	// ! @previous
	public abstract void drawMT(final Matrix4f transformationMatrix, final boolean enableDepthTest);
	
	/**
	 * calculate the element number that is the first out the alignment
	 *        range (start at the specify ID, and use start pos with current one)
	 * @param text The string that might be parsed.
	 * @param start The first element that might be used to calculate.
	 * @param stop The last Id available in the current string.
	 * @param space Number of space in the string.
	 * @param freeSpace This represent the number of pixel present in the
	 *             right white space.
	 * @return true if the right has free space that can be use for justify.
	 * false if we find '\n'
	 */
	public boolean extrapolateLastId(final String text, final int start, final Dynamic<Integer> stop, final Dynamic<Integer> space, final Dynamic<Integer> freeSpace) {
		// store previous :
		final Character storePrevious = this.previousCharcode;
		
		stop.value = text.length();
		space.value = 0;
		
		int lastSpacePosition = start;
		int lastSpacefreeSize = 0;
		
		float endPos = this.position.x();
		boolean endOfLine = false;
		
		float stopPosition = this.stopTextPos;
		if (!this.needDisplay || this.stopTextPos == this.startTextPos) {
			stopPosition = this.startTextPos + 3999999999.0f;
		}
		
		for (int iii = start; iii < text.length(); iii++) {
			final Vector3f tmpSize = calculateSize(text.charAt(iii));
			// check overflow :
			if (endPos + tmpSize.x() > stopPosition) {
				stop.value = iii;
				break;
			}
			// save number of space :
			if (text.charAt(iii) == Character.SPACE_SEPARATOR) {
				space.value++;
				lastSpacePosition = iii;
				lastSpacefreeSize = (int) (stopPosition - endPos);
			} else if (text.charAt(iii) == Character.LINE_SEPARATOR) {
				stop.value = iii;
				endOfLine = true;
				break;
			}
			// update local size :
			endPos += tmpSize.x();
		}
		freeSpace.value = (int) (stopPosition - endPos);
		// restore previous :
		this.previousCharcode = storePrevious;
		// need to align left or right ...
		if (stop.value == (long) text.length()) {
			return true;
		}
		if (endOfLine) {
			return true;
		}
		if (space.value == 0) {
			return true;
		}
		stop.value = lastSpacePosition;
		freeSpace.value = lastSpacefreeSize;
		return false;
	}
	
	@Override
	public void flush() {
		this.vectorialDraw.flush();
	}
	
	/**
	 * This generate the line return == > it return to the alignment
	 *        position start and at the correct line position ==> it might be use to
	 *        not know the line height
	 */
	public void forceLineReturn() {
		// reset position :
		setPos(new Vector3f(this.startTextPos, this.position.y() - getHeight(), 0));
	}
	
	/**
	 * get the current alignment property
	 * @return the current alignment type
	 */
	public AlignMode getAlignment() {
		return this.alignment;
	}
	
	// This is used to draw background selection and other things ...
	public CompositingDrawing getDrawing() {
		return this.vectorialDraw;
	}
	
	/**
	 * get the current font mode
	 * @return The font mode applied
	 */
	public FontMode getFontMode() {
		return this.mode;
	}
	
	public abstract GlyphProperty getGlyphPointer(Character charcode);
	
	public abstract float getHeight();
	
	/**
	 * get the current display position (sometime needed in the gui control)
	 * @return the current position.
	 */
	public Vector3f getPos() {
		return this.position;
	}
	
	public abstract float getSize();
	
	/**
	 * add a line with the current this.htmlDecoTmp decoration
	 * @param data The cuurent data to add.
	 */
	public void htmlAddData(final String data) {
		if (this.htmlCurrentLine.length() > 0 && this.htmlCurrentLine.charAt(this.htmlCurrentLine.length() - 1) != Character.SPACE_SEPARATOR) {
			this.htmlCurrentLine += Character.SPACE_SEPARATOR;
			if (this.htmlDecoration.size() > 0) {
				final TextDecoration tmp = this.htmlDecoration.get(this.htmlDecoration.size() - 1);
				this.htmlDecoration.add(tmp);
			} else {
				this.htmlDecoration.add(this.htmlDecoTmp);
			}
		}
		this.htmlCurrentLine += data;
		for (int iii = 0; iii < data.length(); iii++) {
			this.htmlDecoration.add(this.htmlDecoTmp);
		}
	}
	
	/**
	 * draw the current line
	 */
	public void htmlFlush() {
		if (this.htmlCurrentLine.length() > 0) {
			print(this.htmlCurrentLine, this.htmlDecoration);
		}
		this.htmlCurrentLine = "";
		this.htmlDecoration.clear();
	}
	
	/**
	 * load the openGL program and get all the ID needed
	 */
	public void loadProgram(final Uri vertexShader, final Uri fragmentShader) {
		ResourceProgram old = this.oGLprogram;
		this.oGLprogram = ResourceProgram.create(vertexShader, fragmentShader);
		if (this.oGLprogram != null) {
			this.oGLPosition = this.oGLprogram.getAttribute("EWcoord3d");
			this.oGLColor = this.oGLprogram.getAttribute("EWcolor");
			this.oGLtexture = this.oGLprogram.getAttribute("EWtexture2d");
			this.oGLMatrix = this.oGLprogram.getUniform("EWMatrixTransformation");
			this.oGLtexID = this.oGLprogram.getUniform("EWtexID");
			this.oGLtextWidth = this.oGLprogram.getUniform("EWtexWidth");
			this.oGLtextHeight = this.oGLprogram.getUniform("EWtexHeight");
		} else {
			Log.error("Can not load the program => create previous one...");
			this.oGLprogram = old;
			old = null;
		}
	}
	
	/**
	 * This parse a tinyXML node (void pointer to permit to hide tiny XML in
	 *        include).
	 * @param element the exml element.
	 */
	public void parseHtmlNode(final XmlElement element) {
		for (final XmlNode it : element.getNodes()) {
			if (it.isComment()) {
				// nothing to do ...
				continue;
			}
			if (it.isText()) {
				htmlAddData(it.getValue());
				Log.verbose("XML add : " + it.getValue());
				continue;
			}
			if (!it.isElement()) {
				Log.error("node not suported type : " + it.getType() + " val='" + it.getValue() + "'");
				continue;
			}
			final XmlElement elem = (XmlElement) it;
			final String lowercaseValue = elem.getValue().toLowerCase();
			if (lowercaseValue.contentEquals("br")) {
				htmlFlush();
				Log.verbose("XML flush  newLine");
				forceLineReturn();
			} else if (lowercaseValue.contentEquals("font")) {
				Log.verbose("XML Font ...");
				final TextDecoration tmpDeco = this.htmlDecoTmp;
				if (elem.existAttribute("color")) {
					try {
						final String colorValue = elem.getAttribute("color");
						if (colorValue.length() != 0) {
							this.htmlDecoTmp = this.htmlDecoTmp.withFG(Color.valueOf(colorValue));
						}
					} catch (final ExmlAttributeDoesNotExist e) {
						Log.error("Can not get attribute 'color' in XML:" + e.getMessage());
						e.printStackTrace();
					} catch (final Exception e) {
						Log.error("Can not parse attribute 'color' in XML:" + e.getMessage());
						e.printStackTrace();
					}
				}
				if (elem.existAttribute("colorBg")) {
					try {
						final String colorValue = elem.getAttribute("colorBg");
						if (colorValue.length() != 0) {
							this.htmlDecoTmp = this.htmlDecoTmp.withBG(Color.valueOf(colorValue));
						}
					} catch (final ExmlAttributeDoesNotExist e) {
						Log.error("Can not get attribute 'colorBg' in XML:" + e.getMessage());
						e.printStackTrace();
					} catch (final Exception e) {
						Log.error("Can not parse attribute 'colorBg' in XML:" + e.getMessage());
						e.printStackTrace();
					}
				}
				parseHtmlNode(elem);
				this.htmlDecoTmp = tmpDeco;
			} else if (lowercaseValue.contentEquals("b") || lowercaseValue.contentEquals("bold")) {
				Log.verbose("XML bold ...");
				final TextDecoration tmpDeco = this.htmlDecoTmp;
				if (this.htmlDecoTmp.mode() == FontMode.Regular) {
					this.htmlDecoTmp = this.htmlDecoTmp.withMode(FontMode.Bold);
				} else if (this.htmlDecoTmp.mode() == FontMode.Italic) {
					this.htmlDecoTmp = this.htmlDecoTmp.withMode(FontMode.BoldItalic);
				}
				parseHtmlNode(elem);
				this.htmlDecoTmp = tmpDeco;
			} else if (lowercaseValue.contentEquals("i") || lowercaseValue.contentEquals("italic")) {
				Log.verbose("XML italic ...");
				final TextDecoration tmpDeco = this.htmlDecoTmp;
				if (this.htmlDecoTmp.mode() == FontMode.Regular) {
					this.htmlDecoTmp = this.htmlDecoTmp.withMode(FontMode.Italic);
				} else if (this.htmlDecoTmp.mode() == FontMode.Bold) {
					this.htmlDecoTmp = this.htmlDecoTmp.withMode(FontMode.BoldItalic);
				}
				parseHtmlNode(elem);
				this.htmlDecoTmp = tmpDeco;
			} else if (lowercaseValue.contentEquals("u") || lowercaseValue.contentEquals("underline")) {
				Log.verbose("XML underline ...");
				parseHtmlNode(elem);
			} else if (lowercaseValue.contentEquals("p") || lowercaseValue.contentEquals("paragraph")) {
				Log.verbose("XML paragraph ...");
				htmlFlush();
				this.alignment = AlignMode.alignLeft;
				forceLineReturn();
				parseHtmlNode(elem);
				forceLineReturn();
			} else if (lowercaseValue.contentEquals("center")) {
				Log.verbose("XML center ...");
				htmlFlush();
				this.alignment = AlignMode.alignCenter;
				parseHtmlNode(elem);
			} else if (lowercaseValue.contentEquals("left")) {
				Log.verbose("XML left ...");
				htmlFlush();
				this.alignment = AlignMode.alignLeft;
				parseHtmlNode(elem);
			} else if (lowercaseValue.contentEquals("right")) {
				Log.verbose("XML right ...");
				htmlFlush();
				this.alignment = AlignMode.alignRight;
				parseHtmlNode(elem);
			} else if (lowercaseValue.contentEquals("justify")) {
				Log.verbose("XML justify ...");
				htmlFlush();
				this.alignment = AlignMode.alignJustify;
				parseHtmlNode(elem);
			} else {
				Log.error("node not suported type: " + elem.getType() + " val='" + elem.getValue() + "'");
			}
		}
	}
	
	/**
	 * display a compleat string in the current element.
	 * @param text The string to display.
	 */
	public void print(final String text) {
		final List<TextDecoration> decorationEmpty = new ArrayList<>();
		print(text, decorationEmpty);
	}
	
	/**
	 * display a compleat string in the current element whith specific
	 *        decorations (advence mode).
	 * @param text The string to display.
	 * @param decoration The text decoration for the text that might be display
	 *            (if the vector is smaller, the last 0,·;2p!arameter is get)
	 */
	public void print(final String text, final List<TextDecoration> decoration) {
		Color tmpFg = this.color;
		Color tmpBg = this.colorBg;
		if (this.alignment == AlignMode.alignDisable) {
			// Log.debug(" 1 print in not alligned mode : start=" + this.sizeDisplayStart +
			// " stop=" + this.sizeDisplayStop + " pos=" + this.position);
			// display the cursor if needed (if it is at the start position...)
			if (this.needDisplay) {
				if (0 == this.cursorPos) {
					this.vectorialDraw.setPos(this.position);
					setColorBg(this.colorCursor);
					printCursor(false);
				}
			}
			// note this is faster when nothing is requested ...
			for (int iii = 0; iii < text.length(); iii++) {
				// check if ve have decoration
				if (iii < decoration.size()) {
					tmpFg = decoration.get(iii).colorFG();
					tmpBg = decoration.get(iii).colorBG();
					setFontMode(decoration.get(iii).mode());
				}
				// if real display : ( not display is for size calculation)
				if (this.needDisplay) {
					if ((this.selectionStartPos - 1 < (long) iii && (long) iii <= this.cursorPos - 1) || (this.selectionStartPos - 1 >= (long) iii && (long) iii > this.cursorPos - 1)) {
						setColor(Color.BLACK);
						setColorBg(this.colorSelection);
					} else {
						setColor(tmpFg);
						setColorBg(tmpBg);
					}
				}
				if (this.needDisplay && this.colorBg.a() != 0) {
					final Vector3f pos = this.position;
					this.vectorialDraw.setPos(pos);
					printChar(text.charAt(iii));
					final float fontHeigh = getHeight();
					this.vectorialDraw.rectangleWidth(new Vector3f(this.position.x() - pos.x(), fontHeigh, 0.0f));
					this.nbCharDisplayed++;
				} else {
					printChar(text.charAt(iii));
					this.nbCharDisplayed++;
				}
				// display the cursor if needed (if it is at the other position...)
				if (this.needDisplay) {
					if ((long) iii == this.cursorPos - 1) {
						this.vectorialDraw.setPos(this.position);
						setColorBg(this.colorCursor);
						printCursor(false);
					}
				}
			}
			// Log.debug(" 2 print in not alligned mode : start=" + this.sizeDisplayStart +
			// " stop=" + this.sizeDisplayStop + " pos=" + this.position);
		} else {
			// Log.debug(" 3 print in not alligned mode : start=" + this.sizeDisplayStart +
			// " stop=" + this.sizeDisplayStop + " pos=" + this.position);
			// special start case at the right of the endpoint :
			if (this.stopTextPos < this.position.x()) {
				forceLineReturn();
			}
			final float basicSpaceWidth = calculateSize(' ').x();
			int currentId = 0;
			final Dynamic<Integer> stop = new Dynamic<Integer>(0);
			final Dynamic<Integer> space = new Dynamic<Integer>(0);
			final Dynamic<Integer> freeSpace = new Dynamic<Integer>(0);
			while (currentId < (long) text.length()) {
				final boolean needNoJustify = extrapolateLastId(text, currentId, stop, space, freeSpace);
				float interpolation = basicSpaceWidth;
				switch (this.alignment) {
					case alignJustify:
						if (!needNoJustify) {
							interpolation += (float) freeSpace.value / (float) (space.value - 1);
						}
						break;
					case alignDisable: // must not came from here ...
					case alignLeft:
						// nothing to do ...
						break;
					case alignRight:
						if (this.needDisplay) {
							// Move the first char at the right :
							setPos(new Vector3f(this.position.x() + freeSpace.value, this.position.y(), this.position.z()));
						}
						break;
					case alignCenter:
						if (this.needDisplay) {
							// Move the first char at the right :
							setPos(new Vector3f(this.position.x() + freeSpace.value / 2, this.position.y(), this.position.z()));
						}
						break;
					default:
						break;
				}
				// display all the elements
				if (this.needDisplay && this.cursorPos == 0) {
					this.vectorialDraw.setPos(this.position);
					setColorBg(this.colorCursor);
					printCursor(false);
				}
				for (int iii = currentId; (long) iii < stop.value && iii < text.length(); iii++) {
					final float fontHeigh = getHeight();
					// get specific decoration if provided
					if (iii < decoration.size()) {
						tmpFg = decoration.get(iii).colorFG();
						tmpBg = decoration.get(iii).colorBG();
						setFontMode(decoration.get(iii).mode());
					}
					if (this.needDisplay) {
						if ((this.selectionStartPos - 1 < (long) iii && (long) iii <= this.cursorPos - 1) || (this.selectionStartPos - 1 >= (long) iii && (long) iii > this.cursorPos - 1)) {
							setColor(Color.BLACK);
							setColorBg(this.colorSelection);
						} else {
							setColor(tmpFg);
							setColorBg(tmpBg);
						}
					}
					// special for the justify mode
					if (text.charAt(iii) == Character.SPACE_SEPARATOR) {
						// Log.debug(" generateString : \" \"");
						if (this.needDisplay && this.colorBg.a() != 0) {
							this.vectorialDraw.setPos(this.position);
						}
						// Must generate a dynamic space :
						setPos(new Vector3f(this.position.x() + interpolation, this.position.y(), this.position.z()));
						if (this.needDisplay && this.colorBg.a() != 0) {
							this.vectorialDraw.rectangleWidth(new Vector3f(interpolation, fontHeigh, 0.0f));
						}
					} else // Log.debug(" generateString : \"" + (char)text[iii] + "\"");
					if (this.needDisplay && this.colorBg.a() != 0) {
						final Vector3f pos = this.position;
						this.vectorialDraw.setPos(pos);
						printChar(text.charAt(iii));
						this.vectorialDraw.rectangleWidth(new Vector3f(this.position.x() - pos.x(), fontHeigh, 0.0f));
						this.nbCharDisplayed++;
					} else {
						printChar(text.charAt(iii));
						this.nbCharDisplayed++;
					}
					if (this.needDisplay) {
						if ((long) iii == this.cursorPos - 1) {
							this.vectorialDraw.setPos(this.position);
							setColorBg(this.colorCursor);
							printCursor(false);
						}
					}
				}
				if (stop.value >= text.length()) {
					currentId = stop.value;
					continue;
				}
				if (currentId == stop.value) {
					currentId++;
				} else if (text.charAt(stop.value) == Character.SPACE_SEPARATOR) {
					currentId = stop.value + 1;
					// reset position :
					setPos(new Vector3f(this.startTextPos, this.position.y() - getHeight(), this.position.z()));
					this.nbCharDisplayed++;
				} else if (text.charAt(stop.value) == Character.LINE_SEPARATOR) {
					currentId = stop.value + 1;
					// reset position :
					setPos(new Vector3f(this.startTextPos, this.position.y() - getHeight(), this.position.z()));
					this.nbCharDisplayed++;
				} else {
					currentId = stop.value;
				}
			}
			Log.debug(" 4 print in not alligned mode : start=" + this.sizeDisplayStart + " stop=" + this.sizeDisplayStop + " pos=" + this.position);
		}
	}
	
	/**
	 * display the current char in the current element (note that the kerning
	 *        is availlable if the position is not changed)
	 * @param charcode Char that might be dispalyed
	 */
	public abstract void printChar(Character charcode);
	
	/**
	 * draw a cursor at the specify position
	 * @param isInsertMode True if the insert mode is activated
	 */
	public void printCursor(final boolean isInsertMode) {
		printCursor(isInsertMode, 20.0f);
	}
	
	public void printCursor(final boolean isInsertMode, final float cursorSize) {
		final int fontHeigh = (int) getHeight();
		if (isInsertMode) {
			this.vectorialDraw.rectangleWidth(new Vector3f(cursorSize, fontHeigh, 0));
		} else {
			this.vectorialDraw.setThickness(2);
			this.vectorialDraw.lineRel(new Vector3f(0, fontHeigh, 0));
			this.vectorialDraw.setThickness(0);
		}
	}
	
	/**
	 * display a compleat string in the current element with the generic
	 *        decoration specification. (basic html data)
	 * 
	 *        [code style=xml] <br/>
	 *        <br/>
	 *        <br/>
	 *        <br/>
	 *        <center> text exemple <b>in bold</b> other text <b>bold part
	 *        <i>boldItalic part</i></b> an other thext
	 *        <font color="#FF0000">colored text <b>bold color text</b> <i>bold
	 *        italic text</i> normal color text</font> the end of the string<br/>
	 *        an an other thext </center> <br/>
	 *        <br/>
	 *        <br/>
	 *        <left> plop 1 </left> <br/>
	 *        <br/>
	 *        <br/>
	 *        <right> plop 2 </right> <br/>
	 *        <br/>
	 *        <br/>
	 *        <justify> Un exemple de text </justify> [/code]
	 * 
	 * @note This is parsed with tiny xml, then be carfull that the XML is correct,
	 *       and all balises are closed ... otherwite the display can not be done
	 * @param text The string to display.
	 * @TODO : implementation not done ....
	 */
	public void printDecorated(final String text) {
		String tmpData = "<html>\n<body>\n";
		tmpData += text;
		tmpData += "\n</body>\n</html>\n";
		// Log.debug("plop : " + tmpData);
		printHTML(tmpData);
	}
	
	/**
	 * display a compleat string in the current element with the generic
	 *        decoration specification. (basic html data)
	 * 
	 *        [code style=xml] <html> <body> <br/>
	 *        <br/>
	 *        <br/>
	 *        <br/>
	 *        <center> text exemple <b>in bold</b> other text <b>bold part
	 *        <i>boldItalic part</i></b> an other thext
	 *        <font color="#FF0000">colored text <b>bold color text</b> <i>bold
	 *        italic text</i> normal color text</font> the end of the string<br/>
	 *        an an other thext </center> <br/>
	 *        <br/>
	 *        <br/>
	 *        <left> plop 1 </left> <br/>
	 *        <br/>
	 *        <br/>
	 *        <right> plop 2 </right> <br/>
	 *        <br/>
	 *        <br/>
	 *        <justify> Un exemple de text </justify> </body> </html> [/code]
	 * 
	 * @note This is parsed with tiny xml, then be carfull that the XML is correct,
	 *       and all balises are closed ... otherwite the display can not be done
	 * @param text The string to display.
	 * @TODO : implementation not done ....
	 */
	public void printHTML(final String text) {
		// reset parameter :
		this.htmlDecoTmp = new TextDecoration(this.defaultColorFg, this.defaultColorBg, FontMode.Regular);
		try {
			final XmlElement doc = Exml.parse(text);
			if (!doc.existNode("html")) {
				Log.error("can not load XML: main node not find: 'html'");
				Exml.display(doc);
				return;
			}
			final XmlElement root = (XmlElement) doc.getNode("html");
			
			if (!root.existNode("body")) {
				Log.error("can not load XML: main node not find: 'body'");
				return;
			}
			final XmlElement bodyNode = (XmlElement) root.getNode("body");
			parseHtmlNode(bodyNode);
			htmlFlush();
		} catch (final ExmlParserErrorMulti e) {
			Log.error("Can not parse XML data in printHTML:" + e.getMessage());
			e.printStackTrace();
		} catch (final ExmlBuilderException e) {
			Log.error("Can not generate XML data in printHTML:" + e.getMessage());
			e.printStackTrace();
		} catch (final ExmlNodeDoesNotExist e) {
			Log.error("Error in finding node from XML data in printHTML:" + e.getMessage());
			e.printStackTrace();
		}
	}
	
	/**
	 * clear all the intermediate result detween 2 prints
	 */
	public void reset() {
		this.position = Vector3f.ZERO;
		this.clippingPosStart = Vector3f.ZERO;
		this.clippingPosStop = Vector3f.ZERO;
		this.sizeDisplayStart = this.position;
		this.sizeDisplayStop = this.position;
		this.nbCharDisplayed = 0;
		this.clippingEnable = false;
		this.color = this.defaultColorFg;
		this.colorBg = this.defaultColorBg;
		this.mode = FontMode.Regular;
		this.previousCharcode = 0;
		this.startTextPos = 0;
		this.stopTextPos = 0;
		this.alignment = AlignMode.alignDisable;
		this.htmlCurrentLine = "";
		this.selectionStartPos = -100;
		this.cursorPos = -100;
		this.htmlDecoration.clear();
		this.needDisplay = true;
		this.nbCharDisplayed = 0;
	}
	
	@Override
	public void rotate(final Vector3f vect, final float angle) {
		super.rotate(vect, angle);
		this.vectorialDraw.rotate(vect, angle);
	}
	
	@Override
	public void scale(final Vector3f vect) {
		super.scale(vect);
		this.vectorialDraw.scale(vect);
	}
	
	// ! @previous
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
		this.vectorialDraw.setClipping(this.clippingPosStart, this.clippingPosStop);
	}
	
	/**
	 * enable/Disable the clipping (without lose the current clipping
	 *        position)
	 * newMode The new status of the clipping
	 */
	// TODO : Rename setClippingActivity
	public void setClippingMode(final boolean newMode) {
		this.clippingEnable = newMode;
		this.vectorialDraw.setClippingMode(this.clippingEnable);
	}
	
	// ! @previous
	public void setClippingWidth(final Vector2f pos, final Vector2f width) {
		setClipping(pos, pos.add(width));
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
		this.vectorialDraw.setColor(color);
	}
	
	/**
	 * change the cursor color
	 * @param color New color for the Selection
	 */
	public void setCursorColor(final Color color) {
		this.colorCursor = color;
	}
	
	/**
	 * set a cursor at a specific position:
	 * @param cursorPos id of the cursor position
	 */
	public void setCursorPos(final int cursorPos) {
		this.selectionStartPos = cursorPos;
		this.cursorPos = cursorPos;
	}
	
	/**
	 * set a cursor at a specific position with his associated selection:
	 * @param cursorPos id of the cursor position
	 * @param selectionStartPos id of the starting of the selection
	 */
	public void setCursorSelection(final int cursorPos, final int selectionStartPos) {
		this.selectionStartPos = selectionStartPos;
		this.cursorPos = cursorPos;
	}
	
	/**
	 * set the default background color of the font (when reset, set this
	 *        value ...)
	 * @param color Color to set on background
	 */
	public void setDefaultColorBg(final Color color) {
		this.defaultColorBg = color;
	}
	
	/**
	 * set the default Foreground color of the font (when reset, set this
	 *        value ...)
	 * @param color Color to set on foreground
	 */
	public void setDefaultColorFg(final Color color) {
		this.defaultColorFg = color;
	}
	
	/**
	 * Specify the font property (this reset the internal element of the
	 *        current text (system requirement)
	 * @param fontName Current name of the selected font
	 * @param fontSize New font size
	 */
	public abstract void setFont(final String fontName, final int fontSize);
	
	/**
	 * enable or disable the bold mode
	 * @param status The new status for this display property
	 */
	public void setFontBold(final boolean status) {
		if (status) {
			// enable
			if (this.mode == FontMode.Regular) {
				setFontMode(FontMode.Bold);
			} else if (this.mode == FontMode.Italic) {
				setFontMode(FontMode.BoldItalic);
			}
		} else // disable
		if (this.mode == FontMode.Bold) {
			setFontMode(FontMode.Regular);
		} else if (this.mode == FontMode.BoldItalic) {
			setFontMode(FontMode.Italic);
		}
	}
	
	/**
	 * enable or disable the italic mode
	 * @param status The new status for this display property
	 */
	public void setFontItalic(final boolean status) {
		if (status) {
			// enable
			if (this.mode == FontMode.Regular) {
				setFontMode(FontMode.Italic);
			} else if (this.mode == FontMode.Bold) {
				setFontMode(FontMode.BoldItalic);
			}
		} else // disable
		if (this.mode == FontMode.Italic) {
			setFontMode(FontMode.Regular);
		} else if (this.mode == FontMode.BoldItalic) {
			setFontMode(FontMode.Bold);
		}
	}
	
	/**
	 * Specify the font mode for the next @ref print
	 * @param mode The font mode requested
	 */
	public abstract void setFontMode(FontMode mode);
	
	/**
	 * Specify the font name (this reset the internal element of the current
	 *        text (system requirement)
	 * @param fontName Current name of the selected font
	 */
	public abstract void setFontName(final String fontName);
	
	/**
	 * Specify the font size (this reset the internal element of the current
	 *        text (system requirement)
	 * @param fontSize New font size
	 */
	public abstract void setFontSize(final int fontSize);
	
	/**
	 * set the activation of the Kerning for the display (if it existed)
	 * @param newMode enable/Diasable the kerning on this font.
	 */
	public void setKerningMode(final boolean newMode) {
		this.kerning = newMode;
	}
	
	// ! @previous
	public void setPos(final Vector2f pos) {
		setPos(new Vector3f(pos.x(), pos.y(), 0));
	}
	
	/**
	 * set position for the next text writen
	 * @param pos Position of the text (in 3D)
	 */
	public void setPos(final Vector3f pos) {
		// check min max for display area
		if (this.nbCharDisplayed != 0) {
			Log.verbose("update size 1 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
			this.sizeDisplayStop = Vector3f.max(this.position, this.sizeDisplayStop);
			this.sizeDisplayStart = Vector3f.min(this.position, this.sizeDisplayStart);
		}
		// update position
		this.position = pos;
		this.previousCharcode = 0;
		this.vectorialDraw.setPos(this.position);
		// update min max of the display area:
		if (this.nbCharDisplayed == 0) {
			this.sizeDisplayStart = this.position;
			this.sizeDisplayStop = this.position.withY(this.sizeDisplayStop.y() + getHeight());
			Log.verbose("update size 0 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
		} else {
			Log.verbose("update size 3 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
			this.sizeDisplayStop = Vector3f.max(this.position, this.sizeDisplayStop);
			this.sizeDisplayStart = Vector3f.min(this.position, this.sizeDisplayStart);
			Log.verbose("update size 4 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
		}
	}
	
	// ! @previous
	public void setRelPos(final Vector2f pos) {
		setRelPos(new Vector3f(pos.x(), pos.y(), 0));
	}
	
	/**
	 * set relative position for the next text written
	 * @param pos offset apply of the text (in 3D)
	 */
	public void setRelPos(final Vector3f pos) {
		this.position = this.position.add(pos);
		this.previousCharcode = 0;
		this.vectorialDraw.setPos(this.position);
	}
	
	/**
	 * change the selection color
	 * @param color New color for the Selection
	 */
	public void setSelectionColor(final Color color) {
		this.colorSelection = color;
	}
	
	/**
	 * This generate the possibility to generate the big text property
	 * @param startTextPos The x text start position of the display.
	 * @param stopTextPos The x text stop position of the display.
	 * @note The text align in center change of line every display done (even if it
	 *       was just a char)
	 */
	public void setTextAlignment(final float startTextPos, final float stopTextPos) {
		setTextAlignment(startTextPos, stopTextPos, AlignMode.alignDisable);
	}
	
	public void setTextAlignment(final float startTextPos, final float stopTextPos, final AlignMode alignement) {
		this.startTextPos = startTextPos;
		this.stopTextPos = stopTextPos + 1;
		this.alignment = alignement;
		if (this.startTextPos >= this.stopTextPos) {
			// TODO understand why this flush ...
			Log.verbose("Request alignment with Borne position error : " + startTextPos + " => " + stopTextPos);
		}
	}
	
	@Override
	public void translate(final Vector3f vect) {
		super.translate(vect);
		this.vectorialDraw.translate(vect);
	}
	
}