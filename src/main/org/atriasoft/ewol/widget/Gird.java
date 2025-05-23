/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * @ingroup ewolWidgetGroup
 */
class Gird extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(Gird.class);

	protected class GirdProperties {
		public Widget widget;
		public int row;
		public int col;
	}
	
	protected int sizeRow = 0; //!< size of all lines (row) (if set (otherwise 0))  == > we have a only one size ==> multiple size will have no use ...
	protected int uniformSizeRow = 0;
	protected List<Integer> sizeCol = new ArrayList<>(); //!< size of all colomn (if set (otherwise 0))
	protected List<GirdProperties> subWidget = new ArrayList<>(); //!< all sub widget are contained in this element
	protected Widget tmpWidget = null; //!< use when replace a widget ...
	protected boolean gavityButtom = true;
	
	protected Vector3f propertyBorderSize = Vector3f.ZERO; //!< Border size needed for all the display
	
	/**
	 * Constructor
	 */
	public Gird() {
		
	}
	
	@Override
	public void calculateMinMaxSize() {
		for (int iii = 0; iii < this.sizeCol.size(); iii++) {
			if (this.sizeCol.get(iii) <= 0) {
				this.sizeCol.set(iii, 0);
			}
		}
		//LOGGER.debug("Update minimum size");
		this.minSize = this.propertyMinSize.getPixel();
		this.maxSize = this.propertyMaxSize.getPixel();
		this.uniformSizeRow = 0;
		this.minSize = this.minSize.add(this.propertyBorderSize.multiply(2));
		int lastLineID = 0;
		for (int iii = 0; iii < this.subWidget.size(); iii++) {
			if (this.subWidget.get(iii).row > lastLineID) {
				// change of line :
				lastLineID = this.subWidget.get(iii).row;
			}
			if (this.subWidget.get(iii).widget != null) {
				this.subWidget.get(iii).widget.calculateMinMaxSize();
				final Vector3f tmpSize = this.subWidget.get(iii).widget.getCalculateMinSize();
				LOGGER.debug("     [" + iii + "] subWidgetMinSize=" + tmpSize);
				// for all we get the max size :
				this.uniformSizeRow = Math.max((int) tmpSize.y(), this.uniformSizeRow);
				// for the colomn size : We set the autamatic value in negative :
				if (this.sizeCol.get(this.subWidget.get(iii).col) <= 0) {
					this.sizeCol.set(this.subWidget.get(iii).col,
							Math.min(this.sizeCol.get(this.subWidget.get(iii).col), (int) -tmpSize.x()));
				}
			}
		}
		
		if (this.sizeRow > 0) {
			this.uniformSizeRow = this.sizeRow;
		}
		int tmpSizeWidth = 0;
		for (final Integer element : this.sizeCol) {
			tmpSizeWidth += Math.abs(element);
		}
		LOGGER.debug("     tmpSizeWidth=" + tmpSizeWidth);
		LOGGER.debug("     this.uniformSizeRow=" + this.uniformSizeRow);
		this.minSize = this.minSize.add(tmpSizeWidth, (lastLineID + 1) * this.uniformSizeRow, 0);
		
		LOGGER.debug("Calculate min size : " + this.minSize);
		
		//LOGGER.debug("Vert Result : expand="+ this.userExpand + "  minSize="+ this.minSize);
	}
	
	/**
	 * get the current border size of the current element:
	 * @return the border size (0 if not used)
	 */
	public Vector3f getBorderSize() {
		return this.propertyBorderSize;
	}
	
	/**
	 * get the size view of a colomn.
	 * @param colId Id of the colomn [0..x].
	 * @return The size of the colomn.
	 */
	public int getColSize(final int colId) {
		if ((long) this.sizeCol.size() > colId) {
			if (this.sizeCol.get(colId) <= 0) {
				return 0;
			}
			return this.sizeCol.get(colId);
		}
		LOGGER.error("Can not get the Colomn size : " + colId + 1 + "  we have " + this.sizeCol.size() + " colomn");
		return 0;
	}
	
	public Vector3f getPropertyBorderSize() {
		return this.propertyBorderSize;
	}
	
	/**
	 * get the size view of the lines.
	 * @return The size of the lines.
	 */
	public int getRowSize() {
		return this.sizeRow;
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector3f pos) {
		if (this.propertyHide) {
			return null;
		}
		// for all element in the sizer ...
		for (final GirdProperties it : this.subWidget) {
			if (it.widget == null) {
				continue;
			}
			final Vector3f tmpSize = it.widget.getSize();
			final Vector3f tmpOrigin = it.widget.getOrigin();
			if ((tmpOrigin.x() <= pos.x() && tmpOrigin.x() + tmpSize.x() >= pos.x())
					&& (tmpOrigin.y() <= pos.y() && tmpOrigin.y() + tmpSize.y() >= pos.y())) {
				final Widget tmpWidget = it.widget.getWidgetAtPos(pos);
				if (tmpWidget != null) {
					return tmpWidget;
				}
				// stop searching
				break;
			}
		}
		return null;
	}
	
	@Override
	public void onChangeSize() {
		//LOGGER.debug("Update size");
		this.size = this.size.less(this.propertyBorderSize.x() * 2, this.propertyBorderSize.y() * 2,
				this.propertyBorderSize.y() * 2);
		
		for (int iii = 0; iii < this.subWidget.size(); iii++) {
			if (this.subWidget.get(iii).widget != null) {
				//calculate the origin :
				Vector3f tmpOrigin = this.origin.add(this.propertyBorderSize);
				if (!this.gavityButtom) {
					tmpOrigin = tmpOrigin.add(0, this.size.y() - this.propertyBorderSize.y(), 0);
				}
				
				int tmpSizeWidth = 0;
				for (int jjj = 0; jjj < this.subWidget.get(iii).col; jjj++) {
					tmpSizeWidth += Math.abs(this.sizeCol.get(jjj));
				}
				// adding Y origin :
				int addingPos = 0;
				if (this.gavityButtom) {
					addingPos = (this.subWidget.get(iii).row) * this.uniformSizeRow;
				} else {
					addingPos = -(this.subWidget.get(iii).row + 1) * this.uniformSizeRow;
				}
				tmpOrigin = tmpOrigin.add(tmpSizeWidth, addingPos, 0);
				
				LOGGER.debug("     [{}] set subwidget origin={} size={}", iii, tmpOrigin,
						new Vector3f(Math.abs(this.sizeCol.get(this.subWidget.get(iii).col)), this.uniformSizeRow, 0));
				// set the origin :
				this.subWidget.get(iii).widget.setOrigin(tmpOrigin.clipInteger());
				// all time set all the space .
				this.subWidget.get(iii).widget.setSize(
						(new Vector3f(Math.abs(this.sizeCol.get(this.subWidget.get(iii).col)), this.uniformSizeRow, 0))
								.clipInteger());
				this.subWidget.get(iii).widget.onChangeSize();
			}
		}
		this.size = this.size.add(this.propertyBorderSize.multiply(0.5f));
		LOGGER.debug("Calculate size : " + this.size);
		markToRedraw();
	}
	
	@Override
	public void onRegenerateDisplay() {
		for (final GirdProperties it : this.subWidget) {
			if (it.widget != null) {
				it.widget.onRegenerateDisplay();
			}
		}
	}
	
	/**
	 * set the current border size of the current element:
	 * @param newBorderSize The border size to set (0 if not used)
	 */
	public void setBorderSize(final Vector3f newBorderSize) {
		this.propertyBorderSize = newBorderSize;
	}
	
	/**
	 * set the number of colomn
	 * @param colNumber Nuber of colomn
	 */
	public void setColNumber(final int colNumber) {
		if ((long) this.sizeCol.size() > colNumber) {
			int errorControl = this.subWidget.size();
			// remove subWidget :
			for (int iii = this.subWidget.size(); iii >= 0; iii--) {
				if (this.subWidget.get(iii).col > (colNumber - 1)) {
					// out of bounds : must remove it ...
					if (this.subWidget.get(iii).widget != null) {
						this.subWidget.get(iii).widget = null;
						// no remove, this element is removed with the function onObjectRemove  == > it does not exist anymore ...
						if (errorControl == this.subWidget.size()) {
							LOGGER.error("[" + getId()
									+ "] The number of element might have been reduced ...  == > it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
							System.exit(-1);
						}
					} else {
						LOGGER.warn("[" + getId() + "] Must not have null pointer on the subWidget list ...");
						this.subWidget.remove(iii);
					}
					errorControl = this.subWidget.size();
				}
			}
			// just add the col size:
			this.sizeCol.remove(this.sizeCol.size() - 1);
		} else {
			// just add the col size:
			for (int iii = this.sizeCol.size() - 1; iii < colNumber - 1; iii++) {
				this.sizeCol.add(0);
			}
		}
	}
	
	/**
	 * change a size view of a colomn.
	 * @param colId Id of the colomn [0..x].
	 * @param size size of the colomn.
	 */
	public void setColSize(final int colId, final int size) {
		if ((long) this.sizeCol.size() > colId) {
			this.sizeCol.set(colId, size);
		} else {
			LOGGER.error("Can not set the Colomn size : " + colId + 1 + " at " + size + "px  we have "
					+ this.sizeCol.size() + " colomn");
		}
	}
	
	/**
	 * set the gravity of the widget on the Button (index 0 is on buttom)
	 */
	public void setGravityButtom() {
		this.gavityButtom = true;
		markToRedraw();
	}
	
	/**
	 * set the gravity of the widget on the Top (index 0 is on top)
	 */
	public void setGravityTop() {
		this.gavityButtom = false;
		markToRedraw();
	}
	
	public void setPropertyBorderSize(final Vector3f propertyBorderSize) {
		this.propertyBorderSize = propertyBorderSize;
		if (this.propertyBorderSize.x() < 0) {
			LOGGER.error("Try to set a border size <0 on x : " + this.propertyBorderSize.x() + "  == > restore to 0");
			this.propertyBorderSize = this.propertyBorderSize.withX(0);
		}
		if (this.propertyBorderSize.y() < 0) {
			LOGGER.error("Try to set a border size <0 on y : " + this.propertyBorderSize.y() + "  == > restore to 0");
			this.propertyBorderSize = this.propertyBorderSize.withY(0);
		}
		markToRedraw();
		requestUpdateSize();
	}
	
	/**
	 * change a size view of a line.
	 * @param size size of the line.
	 */
	public void setRowSize(final int size) {
		this.sizeRow = size;
	}
	
	/**
	 * add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
	 * @param colId Id of the colomn [0..x].
	 * @param rowId Id of the row [0..y].
	 * @param newWidget the element pointer
	 */
	public void subWidgetAdd(final int colId, final int rowId, final Widget newWidget) {
		if (newWidget == null) {
			return;
		}
		final GirdProperties prop = new GirdProperties();
		prop.row = rowId;
		prop.col = colId;
		prop.widget = newWidget;
		
		// need to find the correct position :
		for (int iii = 0; iii < this.subWidget.size(); iii++) {
			if (this.subWidget.get(iii).row < prop.row) {
				continue;
			} else if (this.subWidget.get(iii).row > prop.row) {
				// find a new position;
				this.subWidget.add(iii, prop);
				return;
			} else if (this.subWidget.get(iii).col < prop.col) {
				continue;
			} else if (this.subWidget.get(iii).col > prop.col) {
				// find a new position;
				this.subWidget.add(iii, prop);
				return;
			} else {
				// The element already exist  == > replace it ...
				this.tmpWidget = this.subWidget.get(iii).widget;
				this.subWidget.get(iii).widget = newWidget;
				this.tmpWidget = null;
			}
		}
		// not find  == > just adding it ...
		this.subWidget.add(prop);
	}
	
	/**
	 * remove definitly a widget from the system and this Gird.
	 * @param colId Id of the colomn [0..x].
	 * @param rowId Id of the row [0..y].
	 */
	public void subWidgetRemove(final int colId, final int rowId) {
		if (colId < 0 || rowId < 0) {
			LOGGER.warn("[" + getId() + "] try to remove widget with id < 0 col=" + colId + " row=" + rowId);
			return;
		}
		final int errorControl = this.subWidget.size();
		// try to find it ...
		for (int iii = 0; iii < this.subWidget.size(); iii++) {
			if (this.subWidget.get(iii).row == rowId && this.subWidget.get(iii).col == colId) {
				this.subWidget.remove(iii);
				return;
			}
		}
		LOGGER.warn("[" + getId() + "] Can not remove unExistant widget");
	}
	
	/**
	 * remove definitly a widget from the system and this Gird.
	 * @param newWidget the element pointer.
	 */
	public void subWidgetRemove(final Widget newWidget) {
		for (int iii = 0; iii < this.subWidget.size(); iii++) {
			if (newWidget == this.subWidget.get(iii).widget) {
				this.subWidget.remove(iii);
				return;
			}
		}
		LOGGER.warn("[" + getId() + "] Can not remove unExistant widget");
	}
	
	/**
	 * remove all sub element from the widget.
	 */
	public void subWidgetRemoveAll() {
		final int errorControl = this.subWidget.size();
		this.subWidget.clear();
	}
	
	/**
	 * Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
	 * @param colId Id of the colomn [0..x].
	 * @param rowId Id of the row [0..y].
	 */
	public void subWidgetUnLink(final int colId, final int rowId) {
		if (colId < 0 || rowId < 0) {
			LOGGER.warn("[" + getId() + "] try to Unlink widget with id < 0 col=" + colId + " row=" + rowId);
			return;
		}
		// try to find it ...
		for (int iii = 0; iii < this.subWidget.size(); iii++) {
			if (this.subWidget.get(iii).row == rowId && this.subWidget.get(iii).col == colId) {
				this.subWidget.remove(iii);
				return;
			}
		}
		LOGGER.warn("[" + getId() + "] Can not unLink unExistant widget");
	}
	
	/**
	 * Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
	 * @param newWidget the element pointer.
	 */
	public void subWidgetUnLink(final Widget newWidget) {
		if (newWidget == null) {
			return;
		}
		for (int iii = 0; iii < this.subWidget.size(); iii++) {
			if (newWidget == this.subWidget.get(iii).widget) {
				this.subWidget.remove(iii);
				return;
			}
		}
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		super.systemDraw(displayProp);
		for (final GirdProperties it : this.subWidget) {
			if (it.widget != null) {
				it.widget.systemDraw(displayProp);
			}
		}
	}
}
