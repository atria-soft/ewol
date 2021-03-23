/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/ewol.hpp>
#include <ewol/widget/Gird.hpp>
#include <ewol/widget/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Gird);

ewol::widget::Gird::Gird() :
  this.sizeRow(0),
  this.tmpWidget(null),
  this.gavityButtom(true),
  this.borderSize(0,0) {
	addObjectType("ewol::widget::Gird");
	requestUpdateSize();
}

ewol::widget::Gird::~Gird() {
	Log.debug("[" + getId() + "]={" + getObjectType() + "} Gird : destroy");
	subWidgetRemoveAll();
}

void ewol::widget::Gird::setBorderSize( Vector2i _newBorderSize) {
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

void ewol::widget::Gird::onChangeSize() {
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

void ewol::widget::Gird::calculateMinMaxSize() {
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

void ewol::widget::Gird::setColNumber(int _colNumber) {
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

void ewol::widget::Gird::setColSize(int _colId, int _size) {
	if ((long)this.sizeCol.size() > _colId) {
		this.sizeCol[_colId] = _size;
	} else {
		Log.error("Can not set the Colomn size : " + _colId+1
		           + " at " + _size + "px  we have "
		           + this.sizeCol.size() + " colomn");
	}
}

void ewol::widget::Gird::setRowSize(int _size) {
	this.sizeRow = _size;
}

int ewol::widget::Gird::getColSize(int _colId) {
	if ((long)this.sizeCol.size() > _colId) {
		if (this.sizeCol[_colId] <= 0) {
			return 0;
		}
		return this.sizeCol[_colId];
	}
	Log.error("Can not get the Colomn size : " + _colId+1 + "  we have "+ this.sizeCol.size() + " colomn");
	return 0;
}

int ewol::widget::Gird::getRowSize() {
	return this.sizeRow;
}

void ewol::widget::Gird::subWidgetRemoveAll() {
	int errorControl = this.subWidget.size();
	this.subWidget.clear();
}


void ewol::widget::Gird::subWidgetAdd(int _colId, int _rowId, Widget _newWidget) {
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

void ewol::widget::Gird::subWidgetRemove(Widget _newWidget) {
	for (int iii=0; iii<this.subWidget.size(); iii++) {
		if (_newWidget == this.subWidget[iii].widget) {
			this.subWidget.erase(this.subWidget.begin()+iii);
			return;
		}
	}
	Log.warning("[" + getId() + "] Can not remove unExistant widget");
}

void ewol::widget::Gird::subWidgetRemove(int _colId, int _rowId) {
	if (    _colId < 0
	     || _rowId < 0) {
		Log.warning("[" + getId() + "] try to remove widget with id < 0 col=" + _colId + " row=" + _rowId);
		return;
	}
	int errorControl = this.subWidget.size();
	// try to find it ...
	for (int iii=0; iii<this.subWidget.size(); iii++) {
		if(    this.subWidget[iii].row == _rowId
		    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[iii].col == _colId) {
			this.subWidget.erase(this.subWidget.begin()+iii);
			return;
		}
	}
	Log.warning("[" + getId() + "] Can not remove unExistant widget");
}

void ewol::widget::Gird::subWidgetUnLink(Widget _newWidget) {
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

void ewol::widget::Gird::subWidgetUnLink(int _colId, int _rowId) {
	if (    _colId < 0
	     || _rowId < 0) {
		Log.warning("[" + getId() + "] try to Unlink widget with id < 0 col=" + _colId + " row=" + _rowId);
		return;
	}
	// try to find it ...
	for (int iii=0; iii<this.subWidget.size(); iii++) {
		if(    this.subWidget[iii].row == _rowId
		    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.subWidget[iii].col == _colId) {
			this.subWidget.erase(this.subWidget.begin()+iii);
			return;
		}
	}
	Log.warning("[" + getId() + "] Can not unLink unExistant widget");
}

void ewol::widget::Gird::systemDraw( ewol::DrawProperty _displayProp) {
	Widget::systemDraw(_displayProp);
	for (auto it : this.subWidget) {
		if (it.widget != null) {
			it.widget.systemDraw(_displayProp);
		}
	}
}

void ewol::widget::Gird::onRegenerateDisplay() {
	for (auto it : this.subWidget) {
		if (it.widget != null) {
			it.widget.onRegenerateDisplay();
		}
	}
}

Widget ewol::widget::Gird::getWidgetAtPos( Vector2f _pos) {
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
		if(    (tmpOrigin.x() <= _pos.x() LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM tmpOrigin.x() + tmpSize.x() >= _pos.x())
		    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (tmpOrigin.y() <= _pos.y() LOMLOMLOMLOMLOM tmpOrigin.y() + tmpSize.y() >= _pos.y()) ) {
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

