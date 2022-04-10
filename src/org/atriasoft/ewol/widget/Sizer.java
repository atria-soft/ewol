/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etk.math.Vector3i;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.annotation.EwolObjectProperty;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.exml.annotation.XmlAttribute;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;

public class Sizer extends ContainerN {
	public enum DisplayMode {
		modeHori, //!< Vertical mode
		modeVert; //!< Horizontal mode
	}
	
	protected Dimension3f propertyBorderSize = Dimension3f.ZERO; //!< Border size needed for all the display
	
	protected DisplayMode propertyMode = DisplayMode.modeHori; //!< Methode to display the widget list (vert/hory ...)
	
	/**
	 * Constructor
	 */
	public Sizer() {
		
	}
	
	/**
	 * Constructor
	 * @param mode The mode to display the elements
	 */
	public Sizer(final DisplayMode mode) {
		this.propertyMode = mode;
	}
	
	@Override
	public void calculateMinMaxSize() {
		Log.verbose("[" + getId() + "] update minimum size");
		this.subExpend = Vector3b.FALSE;
		this.minSize = this.propertyMinSize.getPixel();
		final Vector3f tmpBorderSize = this.propertyBorderSize.getPixel();
		Log.verbose("[" + getId() + "] {" + getClass().getCanonicalName() + "} set min size : " + this.minSize);
		for (final Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			it.calculateMinMaxSize();
			if (it.canExpand().x()) {
				this.subExpend = this.subExpend.withX(true);
			}
			if (it.canExpand().y()) {
				this.subExpend = this.subExpend.withY(true);
			}
			final Vector3f tmpSize = it.getCalculateMinSize();
			Log.verbose("[" + getId() + "] NewMinSize=" + tmpSize);
			Log.verbose("[" + getId() + "] {" + getClass().getCanonicalName() + "}     Get minSize=" + tmpSize);
			if (this.propertyMode == DisplayMode.modeVert) {
				this.minSize = this.minSize.withY(this.minSize.y() + tmpSize.y());
				if (tmpSize.x() > this.minSize.x()) {
					this.minSize = this.minSize.withX(tmpSize.x());
				}
			} else {
				this.minSize = this.minSize.withX(this.minSize.x() + tmpSize.x());
				if (tmpSize.y() > this.minSize.y()) {
					this.minSize = this.minSize.withY(tmpSize.y());
				}
			}
		}
		this.minSize = this.minSize.add(tmpBorderSize.multiply(2));
		Log.verbose("[{}] Result min size : {}", getId(), this.minSize);
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName("border")
	@EwolObjectProperty
	@EwolDescription("The sizer border size")
	public Dimension3f getPropertyBorderSize() {
		return this.propertyBorderSize;
	}
	
	@XmlManaged
	@XmlAttribute
	@XmlName("mode")
	@EwolObjectProperty
	@EwolDescription("The display mode")
	public DisplayMode getPropertyMode() {
		return this.propertyMode;
	}
	
	@Override
	public void onChangeSize() {
		super.onChangeSize();
		final Vector3f tmpBorderSize = this.propertyBorderSize.getPixel();
		Log.verbose("[" + getId() + "] update size : " + this.size + " nbElement : " + this.subWidget.size() + " borderSize=" + tmpBorderSize + " from border=" + this.propertyBorderSize);
		final Vector3f localWidgetSize = this.size.less(tmpBorderSize.multiply(2.0f));
		// -1- calculate min-size and expand requested:
		Vector3f minSize = Vector3f.ZERO;
		Vector3i nbWidgetExpand = Vector3i.ZERO;
		for (final Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			final Vector3f tmpSize = it.getCalculateMinSize();
			if (this.propertyMode == DisplayMode.modeVert) {
				minSize = new Vector3f(Math.max(minSize.x(), tmpSize.x()), minSize.y() + tmpSize.y(), Math.max(minSize.z(), tmpSize.z()));
			} else {
				minSize = new Vector3f(minSize.x() + tmpSize.x(), Math.max(minSize.y(), tmpSize.y()), Math.max(minSize.z(), tmpSize.z()));
			}
			final Vector3b expand = it.canExpand();
			nbWidgetExpand = nbWidgetExpand.add(expand.x() ? 1 : 0, expand.y() ? 1 : 0, 0);
		}
		// -2- Calculate the size to add at every elements...
		float deltaExpandSize = 0.0f;
		if (!nbWidgetExpand.isEqual(Vector3i.ZERO)) {
			if (this.propertyMode == DisplayMode.modeVert) {
				deltaExpandSize = (localWidgetSize.y() - minSize.y()) / (nbWidgetExpand.y());
			} else {
				deltaExpandSize = (localWidgetSize.x() - minSize.x()) / (nbWidgetExpand.x());
			}
			if (deltaExpandSize < 0.0) {
				deltaExpandSize = 0;
			}
		}
		// -3- Configure all at the min size ...
		for (final Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			it.setSize(it.getCalculateMinSize());
		}
		// -4- For each element we apply the minmax range and update if needed
		while (deltaExpandSize > 0.0001f) {
			float residualNext = 0.0f;
			// get the number of element that need to devide...
			int countCalculation = nbWidgetExpand.x();
			if (this.propertyMode == DisplayMode.modeVert) {
				countCalculation = nbWidgetExpand.y();
			}
			// -4.1- Update every subWidget size
			Widget lastWidget = null;
			if (!this.subWidget.isEmpty()) {
				lastWidget = this.subWidget.get(this.subWidget.size() - 1);
			}
			for (final Widget it : this.subWidget) {
				if (it == null) {
					continue;
				}
				Vector3f tmpSizeMin = it.getSize();
				final Vector3f tmpSizeMax = it.getCalculateMaxSize();
				// Now update his size  his size in X and the current sizer size in Y:
				if (this.propertyMode == DisplayMode.modeVert) {
					if (it.canExpand().y() || (it == lastWidget && it.canExpandIfFree().y())) {
						float sizeExpand = tmpSizeMin.y() + deltaExpandSize;
						if (sizeExpand > tmpSizeMax.y()) {
							residualNext += (sizeExpand - tmpSizeMax.y());
							sizeExpand = tmpSizeMax.y();
							countCalculation--;
						}
						tmpSizeMin = tmpSizeMin.withY(sizeExpand);
					}
					it.setSize(tmpSizeMin);
				} else {
					if (it.canExpand().x() || (it == lastWidget && it.canExpandIfFree().x())) {
						float sizeExpand = tmpSizeMin.x() + deltaExpandSize;
						if (sizeExpand > tmpSizeMax.x()) {
							residualNext += (sizeExpand - tmpSizeMax.x());
							sizeExpand = tmpSizeMax.x();
							countCalculation--;
						}
						tmpSizeMin = tmpSizeMin.withX(sizeExpand);
					}
					it.setSize(tmpSizeMin);
				}
			}
			// Reset size add ...
			deltaExpandSize = 0.0f;
			if (residualNext < 0.0001f) {
				break;
			}
			if (countCalculation <= 0) {
				break;
			}
			if (this.propertyMode == DisplayMode.modeVert) {
				deltaExpandSize = residualNext / (countCalculation);
			} else {
				deltaExpandSize = residualNext / (countCalculation);
			}
			if (deltaExpandSize < 0.0f) {
				deltaExpandSize = 0.0f;
				break;
			}
		}
		// -5- Update the expand in the second size if vert ==> X and if hori ==> Y
		for (final Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			// Now update his size, his size in X and the current sizer size in Y:
			if (this.propertyMode == DisplayMode.modeVert) {
				if (!it.canExpand().x() && !it.canExpandIfFree().x()) {
					continue;
				}
				Vector3f tmpSizeMin = it.getSize();
				tmpSizeMin = tmpSizeMin.withX(FMath.avg(tmpSizeMin.x(), localWidgetSize.x(), it.getCalculateMaxSize().x()));
				it.setSize(tmpSizeMin);
			} else {
				if (!it.canExpand().y() && !it.canExpandIfFree().y()) {
					continue;
				}
				Vector3f tmpSizeMin = it.getSize();
				tmpSizeMin = tmpSizeMin.withY(FMath.avg(tmpSizeMin.y(), localWidgetSize.y(), it.getCalculateMaxSize().y()));
				it.setSize(tmpSizeMin);
			}
		}
		// -6- Force size at the entire number:
		for (final Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			it.setSize(Vector3f.clipInt(it.getSize()));
		}
		// -7- get under Size
		Vector3f underSize = Vector3f.ZERO;
		for (final Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			final Vector3f size = it.getSize();
			if (this.propertyMode == DisplayMode.modeVert) {
				underSize = new Vector3f(Math.max(underSize.x(), size.x()), underSize.y() + size.y(), Math.max(underSize.z(), size.z()));
			} else {
				underSize = new Vector3f(underSize.x() + size.x(), Math.max(underSize.y(), size.y()), Math.max(underSize.z(), size.z()));
			}
		}
		final Vector3f deltas = localWidgetSize.less(underSize);
		
		// -8- Calculate the local origin, depending of the gravity:
		Vector3f tmpOrigin = this.origin.add(tmpBorderSize).add(this.propertyGravity.gravityGenerateDelta(deltas));
		// -9- Set sub widget origin:
		for (final Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			Vector3f origin;
			final Vector3f size = it.getSize();
			if (this.propertyMode == DisplayMode.modeVert) {
				origin = Vector3f.clipInt(tmpOrigin.add(this.offset).add(this.propertyGravity.gravityGenerateDelta(new Vector3f(underSize.x() - size.x(), 0.0f, 0.0f))));
			} else {
				origin = Vector3f.clipInt(tmpOrigin.add(this.offset).add(this.propertyGravity.gravityGenerateDelta(new Vector3f(0.0f, underSize.y() - size.y(), 0.0f))));
			}
			it.setOrigin(origin);
			if (this.propertyMode == DisplayMode.modeVert) {
				tmpOrigin = tmpOrigin.withY(tmpOrigin.y() + size.y());
			} else {
				tmpOrigin = tmpOrigin.withX(tmpOrigin.x() + size.x());
			}
		}
		// -10- Update all subSize at every element:
		for (final Widget it : this.subWidget) {
			if (it == null) {
				continue;
			}
			it.onChangeSize();
		}
		markToRedraw();
	}
	
	public void setPropertyBorderSize(final Dimension3f propertyBorderSize) {
		if (this.propertyBorderSize.equals(propertyBorderSize)) {
			return;
		}
		this.propertyBorderSize = propertyBorderSize;
	}
	
	public void setPropertyMode(final DisplayMode propertyMode) {
		if (this.propertyMode.equals(propertyMode)) {
			return;
		}
		this.propertyMode = propertyMode;
	}
}
