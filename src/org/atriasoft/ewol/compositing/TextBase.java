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

enum AligneMode {
	alignDisable, alignRight, alignLeft, alignCenter, alignJustify
};

public abstract class TextBase extends Compositing {
	private static final int NB_VBO = 4;
	// Text
	protected static int vboIdCoord = 0;
	protected static int vboIdCoordText = 1;
	protected static int vboIdColor = 2;
	protected static int vboIdGlyphLevel = 3;
	// TODO set it back later ... protected CompositingDrawing vectorialDraw; //!<
	// This is used to draw background selection and other things ...
	/*
	 * public CompositingDrawing getDrawing() { return //TODO this.vectorialDraw; };
	 */
	protected int nbCharDisplayed; // !< prevent some error in calculation size.
	protected Vector3f sizeDisplayStart = Vector3f.ZERO; // !< The start windows of the display.
	protected Vector3f sizeDisplayStop = Vector3f.ZERO; // !< The end windows of the display.
	protected boolean needDisplay; // !< This just need the display and not the size rendering.
	protected Vector3f position = Vector3f.ZERO; // !< The current position to draw
	protected Vector3f clippingPosStart = Vector3f.ZERO; // !< Clipping start position
	protected Vector3f clippingPosStop = Vector3f.ZERO; // !< Clipping stop position
	protected boolean clippingEnable = false; // !< true if the clipping must be activated
	protected Color defaultColorFg = Color.BLACK; // !< The text foreground color
	protected Color defaultColorBg = Color.NONE; // !< The text background color

	protected Color color = Color.BLACK; // !< The text foreground color
	protected Color colorBg = Color.NONE; // !< The text background color
	protected Color colorCursor = Color.BLACK; // !< The text cursor color
	protected Color colorSelection = Color.OLIVE; // !< The text Selection color
	protected FontMode mode = FontMode.Regular; // !< font display property : Regular/Bold/Italic/BoldItalic
	protected boolean kerning = true; // !< Kerning enable or disable on the next elements displayed
	protected Character previousCharcode; // !< we remember the previous charcode to perform the kerning. @ref Kerning
	protected float startTextpos = 0; // !< start position of the Alignement (when \n the text return at this
	// position)
	protected float stopTextPos = 0; // !< end of the alignement (when a string is too hight it cut at the word
	// previously this line and the center is perform with this one)
	protected AligneMode alignement = AligneMode.alignDisable; // !< Current Alignement mode (justify/left/right ...)
	protected ResourceProgram GLprogram; // !< pointer on the opengl display program
	protected int GLPosition = -1; // !< openGL id on the element (vertex buffer)
	protected int GLMatrix = -1; // !< openGL id on the element (transformation matrix)
	protected int GLColor = -1; // !< openGL id on the element (color buffer)
	protected int GLtexture = -1; // !< openGL id on the element (Texture position)
	protected int GLtexID = -1; // !< openGL id on the element (texture ID)
	protected int GLtextWidth = -1; // !< openGL Id on the texture width
	protected int GLtextHeight = -1; // !< openGL Id on the texture height
	protected int selectionStartPos = -100; // !< start position of the Selection (if == this.cursorPos ==> no
	// selection)
	protected int cursorPos = -100; // !< Cursor position (default no cursor == > -100)
	protected ResourceVirtualBufferObject VBO;
	// this section is reserved for HTML parsing and display:
	public String htmlCurrrentLine = ""; // !< current line for HTML display
	public List<TextDecoration> htmlDecoration = new ArrayList<>(); // !< current decoration for the HTML display
	public TextDecoration htmlDecoTmp = new TextDecoration(); // !< current decoration

	/**
	 * generic constructor
	 */
	public TextBase() {
		this(new Uri("DATA", "text.vert", "ewol"), new Uri("DATA", "text.frag", "ewol"));
	}

	public TextBase(final Uri _vertexShader, final Uri _fragmentShader) {
		this(_vertexShader, _fragmentShader, true);
	}

	public TextBase(final Uri _vertexShader, final Uri _fragmentShader, final boolean _loadProgram) {
		if (_loadProgram == true) {
			loadProgram(_vertexShader, _fragmentShader);
		}
		// Create the VBO:
		this.VBO = ResourceVirtualBufferObject.create(NB_VBO);
		if (this.VBO == null) {
			Log.error("can not instanciate VBO ...");
			return;
		}
		// TO facilitate some debugs we add a name of the VBO:
		this.VBO.setName("[VBO] of super.TextBase");
	}

	/**
	 * calculate a theoric charcode size
	 * @param _charcode The Unicode value to calculate dimention.
	 * @return The theoric size used.
	 */
	public Vector3f calculateSize(final Character _charcode) {
		return calculateSizeChar(_charcode);
	}

	/**
	 * calculate a theoric text size
	 * @param _text The string to calculate dimention.
	 * @return The theoric size used.
	 */
	public Vector3f calculateSize(final String _text) {
		Vector3f outputSize = Vector3f.ZERO;
		for (int iii = 0; iii < _text.length(); iii++) {
			Vector3f tmpp = calculateSize(_text.charAt(iii));
			if (outputSize.y() == 0) {
				outputSize = outputSize.withY(tmpp.y());
			}
			outputSize = outputSize.withX(outputSize.x() + tmpp.x());
		}
		return outputSize;
	}

	// ! @previous
	public abstract Vector3f calculateSizeChar(Character _charcode);

	/**
	 * calculate a theoric text size
	 * @param _text The string to calculate dimention.
	 * @return The theoric size used.
	 */
	public Vector3f calculateSizeDecorated(final String _text) {
		if (_text.length() == 0) {
			return Vector3f.ZERO;
		}

		String tmpData = "<html><body>\n";
		tmpData += _text;
		tmpData += "\n</body></html>\n";
		Vector3f tmpVal = calculateSizeHTML(tmpData);
		return tmpVal;
	}

	/**
	 * calculate a theoric text size
	 * @param _text The string to calculate dimention.
	 * @return The theoric size used.
	 */
	public Vector3f calculateSizeHTML(final String _text) {
		// remove intermediate result
		reset();
		// Log.debug(" 0 size for=\n" + text);
		// disable display system
		this.needDisplay = false;

		setPos(Vector3f.ZERO);
		// same as print without the end display ...
		printHTML(_text);
		// Log.debug(" 1 Start pos=" + this.sizeDisplayStart);
		// Log.debug(" 1 Stop pos=" + this.sizeDisplayStop);

		// get the last elements
		this.sizeDisplayStop = Vector3f.max(this.position, this.sizeDisplayStop);
		this.sizeDisplayStart = Vector3f.min(this.position, this.sizeDisplayStop);

		// Log.debug(" 2 Start pos=" + this.sizeDisplayStart);
		// Log.debug(" 2 Stop pos=" + this.sizeDisplayStop);
		// set back the display system
		this.needDisplay = true;

		return new Vector3f(this.sizeDisplayStop.x() - this.sizeDisplayStart.x(),
				this.sizeDisplayStop.y() - this.sizeDisplayStart.y(),
				this.sizeDisplayStop.z() - this.sizeDisplayStart.z());
	}

	/**
	 * clear all the registered element in the current element
	 */
	@Override
	public void clear() {
		// call upper class
		super.clear();
		// remove sub draw system
		// TODO this.vectorialDraw.clear();
		// reset Buffer:
		this.VBO.clear();
		// reset temporal variables:
		reset();
	}

	/**
	 * disable the alignement system
	 */
	public void disableAlignement() {
		this.alignement = AligneMode.alignDisable;
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
	public void draw(final boolean _disableDepthTest) {
		drawD(_disableDepthTest);
	}

	// ! @previous
	public void draw(final Matrix4f _transformationMatrix, final boolean _enableDepthTest) {
		drawMT(_transformationMatrix, _enableDepthTest);
	}

	/**
	 * draw All the refistered text in the current element on openGL
	 */
	public abstract void drawD(final boolean _disableDepthTest);;

	// ! @previous
	public abstract void drawMT(final Matrix4f _transformationMatrix, final boolean _enableDepthTest);

	/**
	 * calculate the element number that is the first out the alignement
	 *        range (start at the specify ID, and use start pos with current one)
	 * @param _text The string that might be parsed.
	 * @param _start The first elemnt that might be used to calculate.
	 * @param _stop The last Id availlable in the current string.
	 * @param _space Number of space in the string.
	 * @param _freespace This represent the number of pixel present in the
	 *             right white space.
	 * @return true if the rifht has free space that can be use for jystify.
	 * @return false if we find '\n'
	 */
	public boolean extrapolateLastId(final String _text, final int _start, int _stop, int _space, int _freeSpace) {
		// store previous :
		Character storePrevious = this.previousCharcode;

		_stop = _text.length();
		_space = 0;

		int lastSpacePosition = _start;
		int lastSpacefreeSize = 0;

		float endPos = this.position.x();
		boolean endOfLine = false;

		float stopPosition = this.stopTextPos;
		if (this.needDisplay == false || this.stopTextPos == this.startTextpos) {
			stopPosition = this.startTextpos + 3999999999.0f;
		}

		for (int iii = _start; iii < _text.length(); iii++) {
			Vector3f tmpSize = calculateSize(_text.charAt(iii));
			// check oveflow :
			if (endPos + tmpSize.x() > stopPosition) {
				_stop = iii;
				break;
			}
			// save number of space :
			if (_text.charAt(iii) == Character.SPACE_SEPARATOR) {
				_space++;
				lastSpacePosition = iii;
				lastSpacefreeSize = (int) (stopPosition - endPos);
			} else if (_text.charAt(iii) == Character.LINE_SEPARATOR) {
				_stop = iii;
				endOfLine = true;
				break;
			}
			// update local size :
			endPos += tmpSize.x();
		}
		_freeSpace = (int) (stopPosition - endPos);
		// retore previous :
		this.previousCharcode = storePrevious;
		// need to align left or right ...
		if (_stop == (long) _text.length()) {
			return true;
		} else {
			if (endOfLine) {
				return true;
			} else {
				if (_space == 0) {
					return true;
				}
				_stop = lastSpacePosition;
				_freeSpace = lastSpacefreeSize;
				return false;
			}
		}
	};

	/**
	 * This generate the line return == > it return to the alignement
	 *        position start and at the correct line position ==> it might be use to
	 *        not know the line height
	 */
	public void forceLineReturn() {
		// reset position :
		setPos(new Vector3f(this.startTextpos, this.position.y() - getHeight(), 0));
	}

	/**
	 * get the current alignement property
	 * @return the curent alignement type
	 */
	public AligneMode getAlignement() {
		return this.alignement;
	};

	/**
	 * get the current font mode
	 * @return The font mode applied
	 */
	public FontMode getFontMode() {
		return this.mode;
	}

	public abstract GlyphProperty getGlyphPointer(Character _charcode);

	public abstract float getHeight();;

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
	 * @param _data The cuurent data to add.
	 */
	public void htmlAddData(final String _data) {
		if (this.htmlCurrrentLine.length() > 0
				&& this.htmlCurrrentLine.charAt(this.htmlCurrrentLine.length() - 1) != Character.SPACE_SEPARATOR) {
			this.htmlCurrrentLine += Character.SPACE_SEPARATOR;
			if (this.htmlDecoration.size() > 0) {
				TextDecoration tmp = this.htmlDecoration.get(this.htmlDecoration.size() - 1);
				this.htmlDecoration.add(tmp);
			} else {
				this.htmlDecoration.add(this.htmlDecoTmp);
			}
		}
		this.htmlCurrrentLine += _data;
		for (int iii = 0; iii < _data.length(); iii++) {
			this.htmlDecoration.add(this.htmlDecoTmp);
		}
	};

	/**
	 * draw the current line
	 */
	public void htmlFlush() {
		if (this.htmlCurrrentLine.length() > 0) {
			print(this.htmlCurrrentLine, this.htmlDecoration);
		}
		this.htmlCurrrentLine = "";
		this.htmlDecoration.clear();
	}

	/**
	 * load the openGL program and get all the ID needed
	 */
	public void loadProgram(final Uri _vertexShader, final Uri _fragmentShader) {
		ResourceProgram old = this.GLprogram;
		this.GLprogram = ResourceProgram.create(_vertexShader, _fragmentShader);
		if (this.GLprogram != null) {
			this.GLPosition = this.GLprogram.getAttribute("EW_coord3d");
			this.GLColor = this.GLprogram.getAttribute("EW_color");
			this.GLtexture = this.GLprogram.getAttribute("EW_texture2d");
			this.GLMatrix = this.GLprogram.getUniform("EW_MatrixTransformation");
			this.GLtexID = this.GLprogram.getUniform("EW_texID");
			this.GLtextWidth = this.GLprogram.getUniform("EW_texWidth");
			this.GLtextHeight = this.GLprogram.getUniform("EW_texHeight");
		} else {
			Log.error("Can not load the program => create previous one...");
			this.GLprogram = old;
			old = null;
		}
	};

	/**
	 * This parse a tinyXML node (void pointer to permit to hide tiny XML in
	 *        include).
	 * @param _element the exml element.
	 */
	public void parseHtmlNode(final XmlElement _element) {
		for (XmlNode it : _element.getNodes()) {
			if (it.isComment() == true) {
				// nothing to do ...
				continue;
			} else if (it.isText() == true) {
				htmlAddData(it.getValue());
				Log.verbose("XML add : " + it.getValue());
				continue;
			} else if (it.isElement() == false) {
				Log.error("node not suported type : " + it.getType() + " val='" + it.getValue() + "'");
				continue;
			}
			XmlElement elem = (XmlElement) it;
			String lowercaseValue = elem.getValue().toLowerCase();
			if (lowercaseValue.contentEquals("br") == true) {
				htmlFlush();
				Log.verbose("XML flush  newLine");
				forceLineReturn();
			} else if (lowercaseValue.contentEquals("font") == true) {
				Log.verbose("XML Font ...");
				TextDecoration tmpDeco = this.htmlDecoTmp;
				if (elem.existAttribute("color")) {
					try {
						String colorValue = elem.getAttribute("color");
						if (colorValue.length() != 0) {
							this.htmlDecoTmp = this.htmlDecoTmp.withFG(Color.valueOf(colorValue));
						}
					} catch (ExmlAttributeDoesNotExist e) {
						Log.error("Can not get attribute 'color' in XML:" + e.getMessage());
						e.printStackTrace();
					} catch (Exception e) {
						Log.error("Can not parse attribute 'color' in XML:" + e.getMessage());
						e.printStackTrace();
					}
				}
				if (elem.existAttribute("colorBg")) {
					try {
						String colorValue = elem.getAttribute("colorBg");
						if (colorValue.length() != 0) {
							this.htmlDecoTmp = this.htmlDecoTmp.withBG(Color.valueOf(colorValue));
						}
					} catch (ExmlAttributeDoesNotExist e) {
						Log.error("Can not get attribute 'colorBg' in XML:" + e.getMessage());
						e.printStackTrace();
					} catch (Exception e) {
						Log.error("Can not parse attribute 'colorBg' in XML:" + e.getMessage());
						e.printStackTrace();
					}
				}
				parseHtmlNode(elem);
				this.htmlDecoTmp = tmpDeco;
			} else if (lowercaseValue.contentEquals("b") == true || lowercaseValue.contentEquals("bold") == true) {
				Log.verbose("XML bold ...");
				TextDecoration tmpDeco = this.htmlDecoTmp;
				if (this.htmlDecoTmp.mode() == FontMode.Regular) {
					this.htmlDecoTmp = this.htmlDecoTmp.withMode(FontMode.Bold);
				} else if (this.htmlDecoTmp.mode() == FontMode.Italic) {
					this.htmlDecoTmp = this.htmlDecoTmp.withMode(FontMode.BoldItalic);
				}
				parseHtmlNode(elem);
				this.htmlDecoTmp = tmpDeco;
			} else if (lowercaseValue.contentEquals("i") == true || lowercaseValue.contentEquals("italic") == true) {
				Log.verbose("XML italic ...");
				TextDecoration tmpDeco = this.htmlDecoTmp;
				if (this.htmlDecoTmp.mode() == FontMode.Regular) {
					this.htmlDecoTmp = this.htmlDecoTmp.withMode(FontMode.Italic);
				} else if (this.htmlDecoTmp.mode() == FontMode.Bold) {
					this.htmlDecoTmp = this.htmlDecoTmp.withMode(FontMode.BoldItalic);
				}
				parseHtmlNode(elem);
				this.htmlDecoTmp = tmpDeco;
			} else if (lowercaseValue.contentEquals("u") == true || lowercaseValue.contentEquals("underline") == true) {
				Log.verbose("XML underline ...");
				parseHtmlNode(elem);
			} else if (lowercaseValue.contentEquals("p") == true || lowercaseValue.contentEquals("paragraph") == true) {
				Log.verbose("XML paragraph ...");
				htmlFlush();
				this.alignement = AligneMode.alignLeft;
				forceLineReturn();
				parseHtmlNode(elem);
				forceLineReturn();
			} else if (lowercaseValue.contentEquals("center") == true) {
				Log.verbose("XML center ...");
				htmlFlush();
				this.alignement = AligneMode.alignCenter;
				parseHtmlNode(elem);
			} else if (lowercaseValue.contentEquals("left") == true) {
				Log.verbose("XML left ...");
				htmlFlush();
				this.alignement = AligneMode.alignLeft;
				parseHtmlNode(elem);
			} else if (lowercaseValue.contentEquals("right") == true) {
				Log.verbose("XML right ...");
				htmlFlush();
				this.alignement = AligneMode.alignRight;
				parseHtmlNode(elem);
			} else if (lowercaseValue.contentEquals("justify") == true) {
				Log.verbose("XML justify ...");
				htmlFlush();
				this.alignement = AligneMode.alignJustify;
				parseHtmlNode(elem);
			} else {
				Log.error("node not suported type: " + elem.getType() + " val='" + elem.getValue() + "'");
			}
		}
	}

	/**
	 * display a compleat string in the current element.
	 * @param _text The string to display.
	 */
	public void print(final String _text) {
		List<TextDecoration> decorationEmpty = new ArrayList<>();
		print(_text, decorationEmpty);
	}

	/**
	 * display a compleat string in the current element whith specific
	 *        decorations (advence mode).
	 * @param _text The string to display.
	 * @param _decoration The text decoration for the text that might be display
	 *            (if the vector is smaller, the last parameter is get)
	 */
	public void print(final String _text, final List<TextDecoration> _decoration) {
		Color tmpFg = this.color;
		Color tmpBg = this.colorBg;
		if (this.alignement == AligneMode.alignDisable) {
			// Log.debug(" 1 print in not alligned mode : start=" + this.sizeDisplayStart +
			// " stop=" + this.sizeDisplayStop + " pos=" + this.position);
			// display the cursor if needed (if it is at the start position...)
			if (this.needDisplay == true) {
				if (0 == this.cursorPos) {
					// TODO this.vectorialDraw.setPos(this.position);
					setColorBg(this.colorCursor);
					printCursor(false);
				}
			}
			// note this is faster when nothing is requested ...
			for (int iii = 0; iii < _text.length(); iii++) {
				// check if ve have decoration
				if (iii < _decoration.size()) {
					tmpFg = _decoration.get(iii).colorFG();
					tmpBg = _decoration.get(iii).colorBG();
					setFontMode(_decoration.get(iii).mode());
				}
				// if real display : ( not display is for size calculation)
				if (this.needDisplay == true) {
					if ((this.selectionStartPos - 1 < (long) iii && (long) iii <= this.cursorPos - 1)
							|| (this.selectionStartPos - 1 >= (long) iii && (long) iii > this.cursorPos - 1)) {
						setColor(Color.BLACK);
						setColorBg(this.colorSelection);
					} else {
						setColor(tmpFg);
						setColorBg(tmpBg);
					}
				}
				if (this.needDisplay == true && this.colorBg.a() != 0) {
					Vector3f pos = this.position;
					// TODO this.vectorialDraw.setPos(pos);
					printChar(_text.charAt(iii));
					float fontHeigh = getHeight();
					// TODO
					// this.vectorialDraw.rectangleWidth(Vector3f(this.position.x()-pos.x(),fontHeigh,0.0f)
					// );
					this.nbCharDisplayed++;
				} else {
					printChar(_text.charAt(iii));
					this.nbCharDisplayed++;
				}
				// display the cursor if needed (if it is at the other position...)
				if (this.needDisplay == true) {
					if ((long) iii == this.cursorPos - 1) {
						// TODO this.vectorialDraw.setPos(this.position);
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
			float basicSpaceWidth = calculateSize(' ').x();
			int currentId = 0;
			int stop = 0;
			int space = 0;
			int freeSpace = 0;
			while (currentId < (long) _text.length()) {
				boolean needNoJustify = extrapolateLastId(_text, currentId, stop, space, freeSpace);
				float interpolation = basicSpaceWidth;
				switch (this.alignement) {
				case alignJustify:
					if (needNoJustify == false) {
						interpolation += (float) freeSpace / (float) (space - 1);
					}
					break;
				case alignDisable: // must not came from here ...
				case alignLeft:
					// nothing to do ...
					break;
				case alignRight:
					if (this.needDisplay == true) {
						// Move the first char at the right :
						setPos(new Vector3f(this.position.x() + freeSpace, this.position.y(), this.position.z()));
					}
					break;
				case alignCenter:
					if (this.needDisplay == true) {
						// Move the first char at the right :
						setPos(new Vector3f(this.position.x() + freeSpace / 2, this.position.y(), this.position.z()));
					}
					break;
				}
				// display all the elements
				if (this.needDisplay == true && this.cursorPos == 0) {
					// TODO this.vectorialDraw.setPos(this.position);
					setColorBg(this.colorCursor);
					printCursor(false);
				}
				for (int iii = currentId; (long) iii < stop && iii < _text.length(); iii++) {
					float fontHeigh = getHeight();
					// get specific decoration if provided
					if (iii < _decoration.size()) {
						tmpFg = _decoration.get(iii).colorFG();
						tmpBg = _decoration.get(iii).colorBG();
						setFontMode(_decoration.get(iii).mode());
					}
					if (this.needDisplay == true) {
						if ((this.selectionStartPos - 1 < (long) iii && (long) iii <= this.cursorPos - 1)
								|| (this.selectionStartPos - 1 >= (long) iii && (long) iii > this.cursorPos - 1)) {
							setColor(Color.BLACK);
							setColorBg(this.colorSelection);
						} else {
							setColor(tmpFg);
							setColorBg(tmpBg);
						}
					}
					// special for the justify mode
					if (_text.charAt(iii) == Character.SPACE_SEPARATOR) {
						// Log.debug(" generateString : \" \"");
						if (this.needDisplay == true && this.colorBg.a() != 0) {
							// TODO this.vectorialDraw.setPos(this.position);
						}
						// Must generate a dynamic space :
						setPos(new Vector3f(this.position.x() + interpolation, this.position.y(), this.position.z()));
						if (this.needDisplay == true && this.colorBg.a() != 0) {
							// TODO this.vectorialDraw.rectangleWidth(Vector3f(interpolation,fontHeigh,0.0f)
							// );
						}
					} else {
						// Log.debug(" generateString : \"" + (char)text[iii] + "\"");
						if (this.needDisplay == true && this.colorBg.a() != 0) {
							Vector3f pos = this.position;
							// TODO this.vectorialDraw.setPos(pos);
							printChar(_text.charAt(iii));
							// TODO
							// this.vectorialDraw.rectangleWidth(Vector3f(this.position.x()-pos.x(),fontHeigh,0.0f)
							// );
							this.nbCharDisplayed++;
						} else {
							printChar(_text.charAt(iii));
							this.nbCharDisplayed++;
						}
					}
					if (this.needDisplay == true) {
						if ((long) iii == this.cursorPos - 1) {
							// TODO this.vectorialDraw.setPos(this.position);
							setColorBg(this.colorCursor);
							printCursor(false);
						}
					}
				}
				if (currentId == stop) {
					currentId++;
				} else if (_text.charAt(stop) == Character.SPACE_SEPARATOR) {
					currentId = stop + 1;
					// reset position :
					setPos(new Vector3f(this.startTextpos, this.position.y() - getHeight(), this.position.z()));
					this.nbCharDisplayed++;
				} else if (_text.charAt(stop) == Character.LINE_SEPARATOR) {
					currentId = stop + 1;
					// reset position :
					setPos(new Vector3f(this.startTextpos, this.position.y() - getHeight(), this.position.z()));
					this.nbCharDisplayed++;
				} else {
					currentId = stop;
				}
			}
			// Log.debug(" 4 print in not alligned mode : start=" + this.sizeDisplayStart +
			// " stop=" + this.sizeDisplayStop + " pos=" + this.position);
		}
	}

	/**
	 * display the current char in the current element (note that the kerning
	 *        is availlable if the position is not changed)
	 * @param _charcode Char that might be dispalyed
	 */
	public abstract void printChar(Character _charcode);

	/**
	 * draw a cursor at the specify position
	 * @param _isInsertMode True if the insert mode is activated
	 * @param _cursorSize The sizae of the cursor that might be set when insert
	 *            mode is set [default 20]
	 */
	public void printCursor(final boolean _isInsertMode) {
		printCursor(_isInsertMode, 20.0f);
	}

	public void printCursor(final boolean _isInsertMode, final float _cursorSize) {
		int fontHeigh = (int) getHeight();
		if (true == _isInsertMode) {
			// TODO this.vectorialDraw.rectangleWidth(Vector3f(_cursorSize, fontHeigh, 0) );
		} else {
			// TODO this.vectorialDraw.setThickness(2);
			// TODO this.vectorialDraw.lineRel( Vector3f(0, fontHeigh, 0) );
			// TODO this.vectorialDraw.setThickness(0);
		}
	};

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
	 * @param _text The string to display.
	 * @TODO : implementation not done ....
	 */
	public void printDecorated(final String _text) {
		String tmpData = "<html>\n<body>\n";
		tmpData += _text;
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
	 * @param _text The string to display.
	 * @TODO : implementation not done ....
	 */
	public void printHTML(final String _text) {
		// reset parameter :
		this.htmlDecoTmp = new TextDecoration(this.defaultColorFg, this.defaultColorBg, FontMode.Regular);
		try {
			XmlElement doc = Exml.parse(_text);
			if (doc.existNode("html") == false) {
				Log.error("can not load XML: main node not find: 'html'");
				Exml.display(doc);
				return;
			}
			XmlElement root = (XmlElement) doc.getNode("html");

			if (root.existNode("body") == false) {
				Log.error("can not load XML: main node not find: 'body'");
				return;
			}
			XmlElement bodyNode = (XmlElement) root.getNode("body");
			parseHtmlNode(bodyNode);
			htmlFlush();
		} catch (ExmlParserErrorMulti e) {
			Log.error("Can not parse XML data in printHTML:" + e.getMessage());
			e.printStackTrace();
		} catch (ExmlBuilderException e) {
			Log.error("Can not generate XML data in printHTML:" + e.getMessage());
			e.printStackTrace();
		} catch (ExmlNodeDoesNotExist e) {
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
		this.startTextpos = 0;
		this.stopTextPos = 0;
		this.alignement = AligneMode.alignDisable;
		this.htmlCurrrentLine = "";
		this.selectionStartPos = -100;
		this.cursorPos = -100;
		this.htmlDecoration.clear();
		this.needDisplay = true;
		this.nbCharDisplayed = 0;
	}

	@Override
	public void rotate(final Vector3f _vect, final float _angle) {
		super.rotate(_vect, _angle);
		// TODO this.vectorialDraw.rotate(_vect,_angle);
	}

	@Override
	public void scale(final Vector3f _vect) {
		super.scale(_vect);
		// TODO this.vectorialDraw.scale(_vect);
	}

	// ! @previous
	public void setClipping(final Vector2f _pos, final Vector2f _posEnd) {
		setClipping(new Vector3f(_pos.x(), _pos.y(), -1), new Vector3f(_posEnd.x(), _posEnd.y(), 1));
	}

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
		// //TODO this.vectorialDraw.setClipping(this.clippingPosStart,
		// this.clippingPosStop);
	}

	/**
	 * enable/Disable the clipping (without lose the current clipping
	 *        position)
	 * _newMode The new status of the clipping
	 */
	// TODO : Rename setClippingActivity
	public void setClippingMode(final boolean _newMode) {
		this.clippingEnable = _newMode;
		// //TODO this.vectorialDraw.setClippingMode(this.clippingEnable);
	}

	// ! @previous
	public void setClippingWidth(final Vector2f _pos, final Vector2f _width) {
		setClipping(_pos, _pos.add(_width));
	}

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
	}

	/**
	 * set the background color of the font (for selected Text (not the
	 *        global BG))
	 * @param _color Color to set on background (for next print)
	 */
	public void setColorBg(final Color _color) {
		this.colorBg = _color;
		// TODO this.vectorialDraw.setColor(_color);
	}

	/**
	 * change the cursor color
	 * @param _color New color for the Selection
	 */
	public void setCursorColor(final Color _color) {
		this.colorCursor = _color;
	}

	/**
	 * set a cursor at a specific position:
	 * @param _cursorPos id of the cursor position
	 */
	public void setCursorPos(final int _cursorPos) {
		this.selectionStartPos = _cursorPos;
		this.cursorPos = _cursorPos;
	}

	/**
	 * set a cursor at a specific position with his associated selection:
	 * @param _cursorPos id of the cursor position
	 * @param _selectionStartPos id of the starting of the selection
	 */
	public void setCursorSelection(final int _cursorPos, final int _selectionStartPos) {
		this.selectionStartPos = _selectionStartPos;
		this.cursorPos = _cursorPos;
	}

	/**
	 * set the default background color of the font (when reset, set this
	 *        value ...)
	 * @param _color Color to set on background
	 */
	public void setDefaultColorBg(final Color _color) {
		this.defaultColorBg = _color;
	}

	/**
	 * set the default Foreground color of the font (when reset, set this
	 *        value ...)
	 * @param _color Color to set on foreground
	 */
	public void setDefaultColorFg(final Color _color) {
		this.defaultColorFg = _color;
	}

	/**
	 * Specify the font property (this reset the internal element of the
	 *        current text (system requirement)
	 * @param fontName Current name of the selected font
	 * @param fontSize New font size
	 */
	public abstract void setFont(final String _fontName, final int _fontSize);

	/**
	 * enable or disable the bold mode
	 * @param _status The new status for this display property
	 */
	public void setFontBold(final boolean _status) {
		if (_status == true) {
			// enable
			if (this.mode == FontMode.Regular) {
				setFontMode(FontMode.Bold);
			} else if (this.mode == FontMode.Italic) {
				setFontMode(FontMode.BoldItalic);
			}
		} else {
			// disable
			if (this.mode == FontMode.Bold) {
				setFontMode(FontMode.Regular);
			} else if (this.mode == FontMode.BoldItalic) {
				setFontMode(FontMode.Italic);
			}
		}
	}

	/**
	 * enable or disable the italic mode
	 * @param _status The new status for this display property
	 */
	public void setFontItalic(final boolean _status) {
		if (_status == true) {
			// enable
			if (this.mode == FontMode.Regular) {
				setFontMode(FontMode.Italic);
			} else if (this.mode == FontMode.Bold) {
				setFontMode(FontMode.BoldItalic);
			}
		} else {
			// disable
			if (this.mode == FontMode.Italic) {
				setFontMode(FontMode.Regular);
			} else if (this.mode == FontMode.BoldItalic) {
				setFontMode(FontMode.Bold);
			}
		}
	}

	/**
	 * Specify the font mode for the next @ref print
	 * @param mode The font mode requested
	 */
	public abstract void setFontMode(FontMode _mode);;

	/**
	 * Specify the font name (this reset the internal element of the current
	 *        text (system requirement)
	 * @param _fontName Current name of the selected font
	 */
	public abstract void setFontName(final String _fontName);

	/**
	 * Specify the font size (this reset the internal element of the current
	 *        text (system requirement)
	 * @param _fontSize New font size
	 */
	public abstract void setFontSize(final int _fontSize);

	/**
	 * set the activation of the Kerning for the display (if it existed)
	 * @param _newMode enable/Diasable the kerning on this font.
	 */
	public void setKerningMode(final boolean _newMode) {
		this.kerning = _newMode;
	}

	// ! @previous
	public void setPos(final Vector2f _pos) {
		setPos(new Vector3f(_pos.x(), _pos.y(), 0));
	}

	/**
	 * set position for the next text writen
	 * @param _pos Position of the text (in 3D)
	 */
	public void setPos(final Vector3f _pos) {
		// check min max for display area
		if (this.nbCharDisplayed != 0) {
			Log.verbose("update size 1 " + this.sizeDisplayStart + " " + this.sizeDisplayStop);
			this.sizeDisplayStop = Vector3f.max(this.position, this.sizeDisplayStop);
			this.sizeDisplayStart = Vector3f.min(this.position, this.sizeDisplayStart);
		}
		// update position
		this.position = _pos;
		this.previousCharcode = 0;// TODO this.vectorialDraw.setPos(this.position);
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
	public void setRelPos(final Vector2f _pos) {
		setRelPos(new Vector3f(_pos.x(), _pos.y(), 0));
	}

	/**
	 * set relative position for the next text written
	 * @param _pos offset apply of the text (in 3D)
	 */
	public void setRelPos(final Vector3f _pos) {
		this.position = this.position.add(_pos);
		this.previousCharcode = 0;
		// TODO this.vectorialDraw.setPos(this.position);
	}

	/**
	 * change the selection color
	 * @param _color New color for the Selection
	 */
	public void setSelectionColor(final Color _color) {
		this.colorSelection = _color;
	}

	/**
	 * This generate the possibility to generate the big text property
	 * @param _startTextpos The x text start position of the display.
	 * @param _stopTextPos The x text stop position of the display.
	 * @param _alignement mode of alignement for the Text.
	 * @note The text align in center change of line every display done (even if it
	 *       was just a char)
	 */
	public void setTextAlignement(final float _startTextpos, final float _stopTextPos) {
		setTextAlignement(_startTextpos, _stopTextPos, AligneMode.alignDisable);
	}

	public void setTextAlignement(final float _startTextpos, final float _stopTextPos, final AligneMode _alignement) {
		this.startTextpos = _startTextpos;
		this.stopTextPos = _stopTextPos + 1;
		this.alignement = _alignement;
		if (this.startTextpos >= this.stopTextPos) {
			// TODO: understand why this flush ...
			Log.verbose("Request allignement with Borne position error : " + _startTextpos + " => " + _stopTextPos);
		}
	}

	@Override
	public void translate(final Vector3f _vect) {
		super.translate(_vect);
		// TODO this.vectorialDraw.translate(_vect);
	}

}