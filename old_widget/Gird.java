/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.io.File;
import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etk.math.Vector3i;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolSignal;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.ewol.widget.model.ListRole;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.gale.key.KeyStatus;

/**
 * @ingroup ewolWidgetGroup
 */
class Gird extends Widget {
	protected class GirdProperties {
			public Widget widget;
			public int row;
			public int col;
		};
		protected int sizeRow = 0; //!< size of all lines (row) (if set (otherwise 0))  == > we have a only one size ==> multiple size will have no use ...
		protected int uniformSizeRow = 0;
		protected List<int> sizeCol = new ArrayList<>(); //!< size of all colomn (if set (otherwise 0))
		protected List<GirdProperties> subWidget = new ArrayList<>(); //!< all sub widget are contained in this element
		protected Widget tmpWidget = null; //!< use when replace a widget ...
		protected boolean gavityButtom = true;
		/**
		 * Constructor
		 */
		public Gird() {
			this.borderSize = _newBorderSize;
			if (this.borderSize.x() < 0) {
				Log.error("Try to set a border size <0 on x : " + this.borderSize.x() + "  == > restore to 0");
				this.borderSize.setX(0);
			}
			if (this.borderSize.y() < 0) {
				Log.error("Try to set a border size <0 on y : " + this.borderSize.y() + "  == > restore to 0");
				this.borderSize.setY(0);
			}
			markToRedraw();
			requestUpdateSize();
		}
		/**
		 * set the number of colomn
		 * @param colNumber Nuber of colomn
		 */
		public void setColNumber(int _colNumber) {
			if ((long)this.sizeCol.size() > _colNumber) {
				int errorControl = this.subWidget.size();
				// remove subWidget :
				for (long iii=this.subWidget.size(); iii >= 0; iii--) {
					if (this.subWidget[iii].col > (_colNumber-1)) {
						// out of bounds : must remove it ...
						if (this.subWidget[iii].widget != null) {
							this.subWidget[iii].widget.reset();
							// no remove, this element is removed with the function onObjectRemove  == > it does not exist anymore ...
							if (errorControl == this.subWidget.size()) {
								Log.critical("[" + getId() + "] The number of element might have been reduced ...  == > it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
							}
						} else {
							Log.warning("[" + getId() + "] Must not have null pointer on the subWidget list ...");
							this.subWidget.erase(this.subWidget.begin()+iii);
						}
						errorControl = this.subWidget.size();
					}
				}
				// just add the col size:
				this.sizeCol.erase(this.sizeCol.end());
			} else {
				// just add the col size:
				for (int iii=this.sizeCol.size()-1; iii<_colNumber-1 ; iii++) {
					this.sizeCol.pushBack(0);
				}
			}
		}
		/**
		 * change a size view of a colomn.
		 * @param colId Id of the colomn [0..x].
		 * @param size size of the colomn.
		 */
		public void setColSize(int _colId, int _size) {
			if ((long)this.sizeCol.size() > _colId) {
				this.sizeCol[_colId] = _size;
			} else {
				Log.error("Can not set the Colomn size : " + _colId+1
				           + " at " + _size + "px  we have "
				           + this.sizeCol.size() + " colomn");
			}
		}

		/**
		 * change a size view of a line.
		 * @param size size of the line.
		 */
		public void setRowSize(int _size){
			this.sizeRow = _size;
		}
		/**
		 * get the size view of a colomn.
		 * @param colId Id of the colomn [0..x].
		 * @return The size of the colomn.
		 */
		public int getColSize(int _colId) {
			if ((long)this.sizeCol.size() > _colId) {
				if (this.sizeCol[_colId] <= 0) {
					return 0;
				}
				return this.sizeCol[_colId];
			}
			Log.error("Can not get the Colomn size : " + _colId+1 + "  we have "+ this.sizeCol.size() + " colomn");
			return 0;
		}
		/**
		 * get the size view of the lines.
		 * @return The size of the lines.
		 */
		public int getRowSize(){
			return this.sizeRow;
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
		/**
		 * remove all sub element from the widget.
		 */
		public void subWidgetRemoveAll() {
			int errorControl = this.subWidget.size();
			this.subWidget.clear();
		}
		/**
		 * add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
		 * @param _colId Id of the colomn [0..x].
		 * @param _rowId Id of the row [0..y].
		 * @param _newWidget the element pointer
		 */
	public void subWidgetAdd(int _colId, int _rowId, Widget _newWidget){
		if (_newWidget == null) {
			return;
		}
		GirdProperties prop;
		prop.row = _rowId;
		prop.col = _colId;
		prop.widget = _newWidget;
		
		// need to find the correct position : 
		for (int iii=0; iii<this.subWidget.size(); iii++) {
			if (this.subWidget[iii].row < prop.row) {
				continue;
			} else if (this.subWidget[iii].row > prop.row) {
				// find a new position;
				this.subWidget.insert(this.subWidget.begin()+iii, prop);
				return;
			} else {
				if (this.subWidget[iii].col < prop.col) {
					continue;
				} else if (this.subWidget[iii].col > prop.col) {
					// find a new position;
					this.subWidget.insert(this.subWidget.begin()+iii, prop);
					return;
				} else {
					// The element already exist  == > replace it ...
					this.tmpWidget = this.subWidget[iii].widget;
					this.subWidget[iii].widget = _newWidget;
					if (this.tmpWidget != null) {
						this.tmpWidget.reset();
						if (this.tmpWidget != null) {
							Log.critical("[" + getId() + "] Error while replacing a widget ...  == > never call when free");
							this.tmpWidget = null;
						}
					}
				}
			}
		}
		// not find  == > just adding it ...
		this.subWidget.pushBack(prop);
	}
		/**
		 * remove definitly a widget from the system and this Gird.
		 * @param _newWidget the element pointer.
		 */
		 public void subWidgetRemove(Widget _newWidget){
				for (int iii=0; iii<this.subWidget.size(); iii++) {
					if (_newWidget == this.subWidget[iii].widget) {
						this.subWidget.erase(this.subWidget.begin()+iii);
						return;
					}
				}
				Log.warning("[" + getId() + "] Can not remove unExistant widget");
			}
		/**
		 * remove definitly a widget from the system and this Gird.
		 * @param _colId Id of the colomn [0..x].
		 * @param _rowId Id of the row [0..y].
		 */
		 public void subWidgetRemove(int _colId, int _rowId){
				if (    _colId < 0
					     || _rowId < 0) {
						Log.warning("[" + getId() + "] try to remove widget with id < 0 col=" + _colId + " row=" + _rowId);
						return;
					}
					int errorControl = this.subWidget.size();
					// try to find it ...
					for (int iii=0; iii<this.subWidget.size(); iii++) {
						if(    this.subWidget[iii].row == _rowId
						    && this.subWidget[iii].col == _colId) {
							this.subWidget.erase(this.subWidget.begin()+iii);
							return;
						}
					}
					Log.warning("[" + getId() + "] Can not remove unExistant widget");
				}
		/**
		 * Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
		 * @param _newWidget the element pointer.
		 */
		 public void subWidgetUnLink(Widget _newWidget) {
				if (_newWidget == null) {
					return;
				}
				for (int iii=0; iii<this.subWidget.size(); iii++) {
					if (_newWidget == this.subWidget[iii].widget) {
						this.subWidget.erase(this.subWidget.begin()+iii);
						return;
					}
				}
			}
		/**
		 * Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...).
		 * @param _colId Id of the colomn [0..x].
		 * @param _rowId Id of the row [0..y].
		 */
		 public void subWidgetUnLink(int _colId, int _rowId){
				if (    _colId < 0
					     || _rowId < 0) {
						Log.warning("[" + getId() + "] try to Unlink widget with id < 0 col=" + _colId + " row=" + _rowId);
						return;
					}
					// try to find it ...
					for (int iii=0; iii<this.subWidget.size(); iii++) {
						if(    this.subWidget[iii].row == _rowId
						    && this.subWidget[iii].col == _colId) {
							this.subWidget.erase(this.subWidget.begin()+iii);
							return;
						}
					}
					Log.warning("[" + getId() + "] Can not unLink unExistant widget");
				}

		protected Vector2i borderSize; //!< Border size needed for all the display
		/**
		 * set the current border size of the current element:
		 * @param _newBorderSize The border size to set (0 if not used)
		 */
		public void setBorderSize( Vector2i _newBorderSize) {
			borderSize = _newBorderSize;
		}
		/**
		 * get the current border size of the current element:
		 * @return the border size (0 if not used)
		 */
	public Vector2i getBorderSize() {
			return this.borderSize;
		};
	public void systemDraw( ewol::DrawProperty _displayProp) {
		Widget::systemDraw(_displayProp);
		for (auto it : this.subWidget) {
			if (it.widget != null) {
				it.widget.systemDraw(_displayProp);
			}
		}
	}
	public void onRegenerateDisplay() {
		for (auto it : this.subWidget) {
			if (it.widget != null) {
				it.widget.onRegenerateDisplay();
			}
		}
	}
	public Widget getWidgetAtPos( Vector2f pos) {
		if (*propertyHide == true) {
			return null;
		}
		// for all element in the sizer ...
		for (auto it : this.subWidget) {
			if (it.widget == null) {
				continue;
			}
			Vector2f tmpSize = it.widget.getSize();
			Vector2f tmpOrigin = it.widget.getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) ) {
				Widget tmpWidget = it.widget.getWidgetAtPos(_pos);
				if (tmpWidget != null) {
					return tmpWidget;
				}
				// stop searching
				break;
			}
		}
		return null;
	}
	public void onChangeSize() {
		//Log.debug("Update size");
		this.size -= this.borderSize*2;
		
		for (int iii=0; iii<this.subWidget.size(); iii++) {
			if (this.subWidget[iii].widget != null) {
				//calculate the origin :
				Vector2f tmpOrigin = this.origin + this.borderSize;
				if (false == this.gavityButtom) {
					tmpOrigin += Vector2f(0, this.size.y()-this.borderSize.y());
				}
				
				int tmpSizeWidth = 0;
				for (int jjj=0; jjj<this.subWidget[iii].col; jjj++ ){
					tmpSizeWidth += abs(this.sizeCol[jjj]);
				}
				// adding Y origin : 
				int addingPos = 0;
				if (true == this.gavityButtom) {
					addingPos = (this.subWidget[iii].row)*this.uniformSizeRow;
				} else {
					addingPos = -(this.subWidget[iii].row+1)*this.uniformSizeRow;
				}
				tmpOrigin += Vector2f(tmpSizeWidth, addingPos);
				
				Log.debug("     [" + iii + "] set subwidget origin=" +tmpOrigin + " size=" + Vector2i(abs(this.sizeCol[this.subWidget[iii].col]), this.uniformSizeRow) );
				// set the origin :
				this.subWidget[iii].widget.setOrigin(Vector2fClipInt32(tmpOrigin));
				// all time set oll the space .
				this.subWidget[iii].widget.setSize(Vector2fClipInt32(Vector2f(abs(this.sizeCol[this.subWidget[iii].col]), this.uniformSizeRow)));
				this.subWidget[iii].widget.onChangeSize();
			}
		}
		this.size += this.borderSize*2;
		Log.debug("Calculate size : " + this.size);
		markToRedraw();
	}
	public void calculateMinMaxSize() {
		for (int iii=0; iii<this.sizeCol.size(); iii++ ){
			if (this.sizeCol[iii] <= 0) {
				this.sizeCol[iii] = 0;
			}
		}
		//Log.debug("Update minimum size");
		this.minSize = propertyMinSize.getPixel();
		this.maxSize = propertyMaxSize.getPixel();
		this.uniformSizeRow = 0;
		this.minSize += this.borderSize*2;
		int lastLineID = 0;
		for (int iii=0; iii<this.subWidget.size(); iii++) {
			if (this.subWidget[iii].row > lastLineID) {
				// change of line : 
				lastLineID = this.subWidget[iii].row;
			}
			if (this.subWidget[iii].widget != null) {
				this.subWidget[iii].widget.calculateMinMaxSize();
				Vector2f tmpSize = this.subWidget[iii].widget.getCalculateMinSize();
				Log.debug("     [" + iii + "] subWidgetMinSize=" + tmpSize);
				// for all we get the max size :
				this.uniformSizeRow = etk::max((int)tmpSize.y(), this.uniformSizeRow);
				// for the colomn size : We set the autamatic value in negative : 
				if (this.sizeCol[this.subWidget[iii].col] <= 0) {
					this.sizeCol[this.subWidget[iii].col] = etk::min(this.sizeCol[this.subWidget[iii].col], (int)-tmpSize.x() );
				}
			}
		}
		
		if (this.sizeRow > 0) {
			this.uniformSizeRow = this.sizeRow;
		}
		int tmpSizeWidth = 0;
		for (int iii=0; iii<this.sizeCol.size(); iii++ ){
			tmpSizeWidth += abs(this.sizeCol[iii]);
		}
		Log.debug("     tmpSizeWidth=" + tmpSizeWidth);
		Log.debug("     this.uniformSizeRow=" + this.uniformSizeRow);
		this.minSize += Vector2i(tmpSizeWidth, (lastLineID+1)*this.uniformSizeRow);
		
		Log.debug("Calculate min size : " + this.minSize);
		
		//Log.debug("Vert Result : expand="+ this.userExpand + "  minSize="+ this.minSize);
	}
}
