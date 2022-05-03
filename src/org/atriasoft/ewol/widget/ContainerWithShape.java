/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.GuiShape;
import org.atriasoft.ewol.compositing.ShapeBox;
import org.atriasoft.ewol.internal.Log;

/**
 * Simple Container that have a Shape (not directly instantiate!!!!)
 */
public class ContainerWithShape extends Container {
	// properties
	public Uri propertyShape = null; //!< Compositing theme.
	
	protected GuiShape shape; //!< Compositing theme.
	protected ShapeBox shapeProperty = ShapeBox.ZERO;
	
	/**
	 * Constructor
	 * @param propertyShape shape file properties
	 */
	public ContainerWithShape(final Uri propertyShape) {
		this.propertyShape = propertyShape;
		onChangePropertyShape();
	}
	
	@Override
	public void calculateMinMaxSize() {
		// call main class
		calculateMinMaxSizeWidget();
		// call sub classes
		if (this.subWidget != null) {
			this.subWidget.calculateMinMaxSize();
			final Vector3f min = this.subWidget.getCalculateMinSize();
			final Padding padding = this.shape.getPadding();
			this.minSize = Vector3f.max(this.minSize, min.add(padding.x(), padding.y(), padding.z()));
		}
		Log.warning("[{}] Result min size : {}", getId(), this.minSize);
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "shape")
	@AknotDescription(value = "The uri one the shape for the Pop-up")
	public Uri getPropertyShape() {
		return this.propertyShape;
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector3f pos) {
		final Widget val = super.getWidgetAtPos(pos);
		if (val != null) {
			return val;
		}
		return this;
	}
	
	protected void onChangePropertyShape() {
		if (this.shape == null) {
			this.shape = new GuiShape(this.propertyShape);
		} else {
			this.shape.setSource(this.propertyShape);
		}
		markToRedraw();
		requestUpdateSize();
	}
	
	@Override
	public void onChangeSize() {
		markToRedraw();
		if (this.subWidget == null) {
			return;
		}
		final Padding padding = this.shape.getPadding();
		Vector3f subWidgetSize = this.subWidget.getCalculateMinSize();
		if (this.subWidget.canExpand().x() && this.propertyFill.x()) {
			subWidgetSize = subWidgetSize.withX(this.size.x());
		} else {
			subWidgetSize = subWidgetSize.withX(this.minSize.x());
		}
		if (this.subWidget.canExpand().y() && this.propertyFill.y()) {
			subWidgetSize = subWidgetSize.withY(this.size.y());
		} else {
			subWidgetSize = subWidgetSize.withY(this.minSize.y());
		}
		if (this.subWidget.canExpand().z() && this.propertyFill.z()) {
			subWidgetSize = subWidgetSize.withZ(this.size.z());
		} else {
			subWidgetSize = subWidgetSize.withZ(this.minSize.z());
		}
		subWidgetSize = subWidgetSize.less(padding.x(), padding.y(), padding.z());
		subWidgetSize = subWidgetSize.clipInteger();
		
		// set config to the Sub-widget
		Vector3f subWidgetOrigin = this.origin.add(this.size.less(subWidgetSize).multiply(0.5f));
		subWidgetOrigin = subWidgetOrigin.clipInteger();
		
		this.subWidget.setOrigin(subWidgetOrigin);
		this.subWidget.setSize(subWidgetSize);
		this.subWidget.onChangeSize();
	}
	
	@Override
	protected void onDraw() {
		this.shape.draw();
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (needRedraw()) {
			this.shape.clear();
			final Padding padding = this.shape.getPadding();
			final Vector3f tmpSize = Vector3f.ZERO;
			Vector3f tmpSizeShaper = this.minSize;
			Vector3f tmpOriginShaper = this.propertyGravity.gravityGenerateDelta(this.size.less(this.minSize));
			if (this.propertyFill.x()) {
				tmpSizeShaper = tmpSizeShaper.withX(this.size.x());
				tmpOriginShaper = tmpOriginShaper.withX(0.0f);
			}
			if (this.propertyFill.y()) {
				tmpSizeShaper = tmpSizeShaper.withY(this.size.y());
				tmpOriginShaper = tmpOriginShaper.withY(0.0f);
			}
			if (this.propertyFill.z()) {
				tmpSizeShaper = tmpSizeShaper.withZ(this.size.y());
				tmpOriginShaper = tmpOriginShaper.withZ(0.0f);
			}
			// not sure this is needed...
			tmpSizeShaper = tmpSizeShaper.clipInteger();
			tmpOriginShaper = tmpOriginShaper.clipInteger();
			
			this.shapeProperty = new ShapeBox(tmpOriginShaper, tmpSizeShaper, padding);
			this.shape.setShape(tmpOriginShaper, tmpSizeShaper);
		}
		// SubWidget generation ...
		if (this.subWidget != null) {
			this.subWidget.onRegenerateDisplay();
		}
	}
	
	public void setPropertyShape(final Uri propertyShape) {
		if (this.propertyShape.equals(propertyShape)) {
			return;
		}
		this.propertyShape = propertyShape;
		onChangePropertyShape();
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			// widget is hidden ...
			return;
		}
		systemDrawWidget(displayProp);
		if (this.subWidget == null) {
			return;
		}
		if (true) { //this.shape.getNextDisplayedStatus() == GuiShapeMode.NONE && this.shape.getTransitionStatus() >= 1.0) {
			final DrawProperty prop = displayProp.withLimit(this.origin, this.size);
			this.subWidget.systemDraw(prop);
		}
	}
	
}
