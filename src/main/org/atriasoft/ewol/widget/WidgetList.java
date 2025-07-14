/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.compositing.Compositing;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.widget.model.ListRole;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

class WidgetList extends WidgetScrolled {
	private static final Logger LOGGER = LoggerFactory.getLogger(WidgetList.class);
	// drawing capabilities ....
	protected List<Compositing> listOObject = new ArrayList<>(); //!< generic element to display...
	
	protected List<Integer> listSizeX = new ArrayList<>(); //!< size of every colons
	
	protected List<Integer> listSizeY = new ArrayList<>(); //!< size of every rows
	protected Map<String, Compositing> compositingElements = new HashMap<>();
	// list properties ...
	protected int paddingSizeX = 0;
	protected int paddingSizeY = 0;
	
	protected int displayStartRaw = 0; //!< Current starting displayed raw
	
	protected int displayCurrentNbLine = 0; //!< Number of line in the display
	
	protected int nbVisibleRaw = 0; // set the number of visible raw (calculate don display)
	// function call to display the list :
	
	public WidgetList() {
		this.paddingSizeX = 2;
		this.paddingSizeY = 2;
		this.nbVisibleRaw = 0;
		this.propertyCanFocus = true;
		this.limitScrolling = new Vector2f(1.0f, 0.5f);
		addComposeElemnent("drawing", new CompositingGC());
		addComposeElemnent("text", new CompositingText());
	}
	
	protected void addComposeElemnent(final String name, final Compositing element) {
		this.compositingElements.put(name, element);
		//this.listOObject.add(element);
	}
	
	/**
	 * Calculate an element size to estimate the render size.
	 * @note Does not generate the with the same size.
	 * @param pos Position of column and Raw of the element.
	 * @return The estimate size of the element.
	 */
	protected Vector2f calculateElementSize(final Vector2i pos) {
		if (getComposeElemnent("text") instanceof final CompositingText tmpText) {
			if (getData(ListRole.Text, pos) instanceof final String myTextToWrite) {
				final Vector2f textSize = tmpText.calculateSize(myTextToWrite);
				//final Vector2i count = getMatrixSize();
				return new Vector2f(textSize.x(), textSize.y() + this.paddingSizeY * 3);
			}
		}
		return Vector2f.ZERO;
	}
	
	@Override
	public void calculateMinMaxSize() {
		/*int fontId = getDefaultFontId();
		int minWidth = ewol::getWidth(fontId, this.label);
		int minHeight = ewol::getHeight(fontId);
		this.minSize.x = 3+minWidth;
		this.minSize.y = 3+minHeight;
		*/
		this.minSize = new Vector2f(200, 150);
	}
	
	protected void clearComposeElemnent() {
		for (final Entry<String, Compositing> it : this.compositingElements.entrySet()) {
			//it.setValue(null);
			it.getValue().clear();
		}
	}
	
	public void clearOObjectList() {
		//this.listOObject.clear();
	}
	
	/**
	 * Draw the background
	 */
	protected void drawBackground() {
		if (getComposeElemnent("drawing") instanceof final CompositingDrawing BGOObjects) {
			final Color basicBG = getBasicBG();
			BGOObjects.setColor(basicBG);
			BGOObjects.setPos(Vector2f.ZERO);
			BGOObjects.rectangleWidth(new Vector2f(this.size.x(), this.size.y()));
		}
	}
	
	/**
	 * Draw an element in the specific size and position.
	 * @param pos Position of colomn and Raw of the element.
	 * @param start Start display position.
	 * @param size Render raw size
	 * @return The estimate size of the element.
	 */
	protected void drawElement(final Vector2i pos, final Vector2f start, final Vector2f size) {
		if (getData(ListRole.Text, pos) instanceof final String myTextToWrite) {
			if (getData(ListRole.BgColor, pos) instanceof final Color bg) {
				if (getComposeElemnent("drawing") instanceof final CompositingDrawing BGOObjects) {
					BGOObjects.setColor(bg);
					BGOObjects.setPos(new Vector2f(start.x(), start.y()));
					BGOObjects.rectangleWidth(size);
				}
			}
			if (!myTextToWrite.isEmpty()) {
				if (getData(ListRole.FgColor, pos) instanceof final Color fg) {
					if (getComposeElemnent("text") instanceof final CompositingText tmpText) {
						final int displayPositionY = (int) (start.y() + this.paddingSizeY);
						tmpText.setColor(fg);
						tmpText.setPos(new Vector2f(start.x() + this.paddingSizeX, displayPositionY));
						tmpText.print(myTextToWrite);
					}
				}
			}
		}
	}
	
	protected void flushElements() {
		for (final Entry<String, Compositing> it : this.compositingElements.entrySet()) {
			it.getValue().flush();
		}
		for (final Compositing elem : this.listOObject) {
			if (elem != null) {
				elem.flush();
			}
		}
	}
	
	protected Color getBasicBG() {
		return new Color(0xFF, 0xFF, 0xFF, 0xFF);
	}
	
	protected Compositing getComposeElemnent(final String name) {
		return this.compositingElements.get(name);
	}
	
	protected Object getData(final ListRole role, final Vector2i pos) {
		switch (role) {
			case Text:
				return "";
			case FgColor:
				return new Color(0x00, 0x00, 0x00, 0xFF);
			case BgColor:
				if (pos.y() % 2 == 0) {
					return new Color(0xFF, 0xFF, 0xFF, 0xFF);
				}
				return new Color(0x7F, 0x7F, 0x7F, 0xFF);
			default:
				break;
		}
		return null;
	}
	
	/**
	 * Get the number of colomn and row availlable in the list
	 * @return Number of colomn and row
	 */
	protected Vector2i getMatrixSize() {
		return new Vector2i(1, 0);
	}
	
	@Override
	protected void onDraw() {
		for (final Entry<String, Compositing> it : this.compositingElements.entrySet()) {
			it.getValue().draw();
		}
		for (final Compositing elem : this.listOObject) {
			if (elem != null) {
				elem.draw();
			}
		}
		super.onDraw();
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		Vector2f relativePos = relativePosition(new Vector2f(event.pos().x(), event.pos().y()));
		if (super.onEventInput(event)) {
			keepFocus();
			// nothing to do ... done on upper widget ...
			return true;
		}
		if (this.listSizeY.size() == 0) {
			return false;
		}
		relativePos = new Vector2f(relativePos.x() + this.originScrooled.x(),
				this.size.y() - relativePos.y() + this.originScrooled.y());
		// Find the colomn and the row
		Vector2i pos = Vector2i.ZERO;
		float offsetY = 0;
		for (int iii = 0; iii < this.listSizeY.size() - 1; iii++) {
			final int previous = (int) offsetY;
			offsetY += this.listSizeY.get(iii);
			if (relativePos.y() < offsetY && relativePos.y() >= previous) {
				pos = pos.withY(iii);
				offsetY = previous;
				break;
			}
			if (iii == this.listSizeY.size() - 2 && relativePos.y() >= offsetY) {
				pos = pos.withY(iii + 1);
				break;
			}
		}
		float offsetX = 0;
		for (int iii = 0; iii < this.listSizeX.size() - 1; iii++) {
			final int previous = (int) offsetX;
			offsetX += this.listSizeX.get(iii);
			if (relativePos.x() < offsetX && relativePos.x() >= previous) {
				pos = pos.withX(iii);
				offsetX = previous;
				break;
			}
			if (iii == this.listSizeX.size() - 2 && relativePos.x() >= offsetX) {
				pos = pos.withX(iii + 1);
				break;
			}
		}
		final Vector2f posInternalMouse = relativePos.less(offsetX, offsetY);
		final boolean isUsed = onItemEvent(event, pos, posInternalMouse);
		if (isUsed) {
			// TODO : this generate bugs ... I did not understand why ..
			//WidgetManager::focusKeep(this);
		}
		return isUsed;
	}
	
	/**
	 * set a raw visible in the main display
	 * @param _id Id of the raw that might be visible.
	 */
	//void setRawVisible(int _id);
	@Override
	protected void onGetFocus() {
		LOGGER.debug("WidgetList get focus");
	}
	
	protected boolean onItemEvent(final EventInput event, final Vector2i pos, final Vector2f mousePosition) {
		return false;
	}
	
	@Override
	protected void onLostFocus() {
		LOGGER.debug("WidgetList Lost focus");
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		// clean the object list ...
		clearComposeElemnent();
		// -------------------------------------------------------
		// -- Calculate the size of each element
		// -------------------------------------------------------
		final Vector2i matrixSize = getMatrixSize();
		// set the correct number of element in the List
		this.listSizeX.clear();
		for (int iii = 0; iii < matrixSize.x(); iii++) {
			this.listSizeX.add(0);
		}
		// set the correct number of element in the List
		this.listSizeY.clear();
		for (int iii = 0; iii < matrixSize.y(); iii++) {
			this.listSizeY.add(0);
		}
		// set real values
		for (int yyy = 0; yyy < matrixSize.y(); ++yyy) {
			for (int xxx = 0; xxx < matrixSize.x(); ++xxx) {
				final Vector2i pos = new Vector2i(xxx, yyy);
				final Vector2f elementSize = calculateElementSize(pos);
				if (elementSize.x() > this.listSizeX.get(xxx)) {
					this.listSizeX.set(xxx, (int) elementSize.x());
				}
				if (elementSize.y() > this.listSizeY.get(yyy)) {
					this.listSizeY.set(yyy, (int) elementSize.y());
				}
			}
		}
		// -------------------------------------------------------
		// -- Fill property appliance
		// -------------------------------------------------------
		if (this.propertyFill.x()) {
			int fullSize = 0;
			for (final int size : this.listSizeX) {
				fullSize += size;
			}
			if (fullSize < this.size.x()) {
				// need to expand all elements:
				final int residualAdd = (int) ((this.size.x() - fullSize) / matrixSize.x());
				if (residualAdd != 0) {
					for (int iii = 0; iii < this.listSizeX.size(); iii++) {
						this.listSizeX.set(iii, this.listSizeX.get(iii) + residualAdd);
					}
				}
			}
		}
		/*
		if (propertyFill.y() == true) {
			int fullSize = 0;
			for (auto size: this.listSizeY) {
				fullSize += size;
			}
			if (fullSize < this.size.y() ) {
				// need to expand all elements:
				int residualAdd = (this.size.y() - fullSize) / matrixSize.y();
				if (residualAdd != 0) {
					for (auto size: this.listSizeY) {
						size += residualAdd;
					}
				}
			}
		}
		*/
		// -------------------------------------------------------
		// -- Calculate the start position size of each element
		// -------------------------------------------------------
		final List<Integer> listStartPosX = new ArrayList<>();
		final List<Integer> listStartPosY = new ArrayList<>();
		int lastPositionX = 0;
		for (final Integer size : this.listSizeX) {
			listStartPosX.add(lastPositionX);
			lastPositionX += size;
		}
		int lastPositionY = 0;
		for (final Integer size : this.listSizeY) {
			lastPositionY += size;
			listStartPosY.add(lastPositionY);
		}
		// -------------------------------------------------------
		// -- Update the scroolBar
		// -------------------------------------------------------
		this.maxSize = new Vector2f(lastPositionX, lastPositionY);
		// -------------------------------------------------------
		// -- Clean the background
		// -------------------------------------------------------
		drawBackground();
		// -------------------------------------------------------
		// -- Draw each element
		// -------------------------------------------------------
		for (int yyy = 0; yyy < matrixSize.y(); ++yyy) {
			final float startYposition = this.size.y() + this.originScrooled.y() - listStartPosY.get(yyy);
			if (startYposition + this.listSizeY.get(yyy) < 0) {
				// ==> element out of range ==> nothing to display
				break;
			}
			if (startYposition > this.size.y()) {
				// ==> element out of range ==> nothing to display
				continue;
			}
			for (int xxx = 0; xxx < matrixSize.x(); ++xxx) {
				final float startXposition = -this.originScrooled.x() + listStartPosX.get(xxx);
				//LOGGER.error("display start: " + startXposition);
				if (startXposition + this.listSizeX.get(xxx) < 0) {
					// ==> element out of range ==> nothing to display
					continue;
				}
				if (startXposition > this.size.x()) {
					// ==> element out of range ==> nothing to display
					break;
				}
				drawElement(new Vector2i(xxx, yyy), new Vector2f(startXposition, startYposition),
						new Vector2f(this.listSizeX.get(xxx), this.listSizeY.get(yyy)));
			}
		}
		// -------------------------------------------------------
		// -- Draw Scrolling widget
		// -------------------------------------------------------
		super.onRegenerateDisplay();
		// flush all compositing drawing
		flushElements();
	}
	
	protected void removeComposeElemnent() {
		this.compositingElements.clear();
	}
	
}
