package org.atriasoft.ewol.widget.meta;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.resource.ResourceConfigFile;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Composer;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.model.SpinPosition;

/**
 * @ingroup ewolWidgetGroup
 */
public class SpinBase extends Sizer {
	// properties list:
	private Uri propertyShape; //!< Shape of the widget
	private SpinPosition propertySpinMode = SpinPosition.RIGHT_RIGHT; //!< How to display the spin base
	protected ResourceConfigFile config;
	protected int confIdEntryShaper = -1;
	protected int confIdUpShaper = -1;
	protected int confIdDownShaper = -1;
	protected int confIdUpData = -1;
	protected int confIdDownData = -1;
	
	protected Entry widgetEntry = null;
	
	protected Button widgetButtonDown = null;
	
	protected Button widgetButtonUp = null;
	
	/**
	 * Constructor
	 */
	protected SpinBase(final Uri shape) {
		setPropertyShape(shape);
		/*
		propertySpinMode.add(ewol::widget::spinPosition_noneNone, "none-none");
		propertySpinMode.add(ewol::widget::spinPosition_noneRight, "none-right");
		propertySpinMode.add(ewol::widget::spinPosition_leftNone, "left-none");
		propertySpinMode.add(ewol::widget::spinPosition_leftRight, "left-right");
		propertySpinMode.add(ewol::widget::spinPosition_leftLeft, "left-left");
		propertySpinMode.add(ewol::widget::spinPosition_RightRight, "right-right");
		*/
		setPropertyGravity(Gravity.CENTER);
		updateGui();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "shape")
	@AknotDescription(value = "shape for the display")
	public Uri getPropertyShape() {
		return this.propertyShape;
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "spin-mode")
	@AknotDescription(value = "The display spin mode")
	public SpinPosition getPropertySpinMode() {
		return this.propertySpinMode;
	}
	
	protected void onChangePropertyShape() {
		this.config = ResourceConfigFile.create(this.propertyShape);
		if (this.config != null) {
			this.confIdEntryShaper = this.config.request("entry-shaper");
			this.confIdUpShaper = this.config.request("up-shaper");
			this.confIdDownShaper = this.config.request("down-shaper");
			this.confIdUpData = this.config.request("up-data");
			this.confIdDownData = this.config.request("down-data");
		}
		markToRedraw();
	}
	
	protected void onChangePropertySpinMode() {
		updateGui();
	}
	
	public void setPropertyShape(final Uri propertyShape) {
		if (this.propertyShape != null && this.propertyShape.equals(propertyShape)) {
			return;
		}
		this.propertyShape = propertyShape;
		onChangePropertyShape();
	}
	
	public void setPropertySpinMode(final SpinPosition propertySpinMode) {
		if (this.propertySpinMode == propertySpinMode) {
			return;
		}
		this.propertySpinMode = propertySpinMode;
		onChangePropertySpinMode();
	}
	
	protected void updateGui() {
		subWidgetRemoveAll();
		markToRedraw();
		requestUpdateSize();
		if (this.widgetEntry == null) {
			this.widgetEntry = new Entry();
			if (this.config != null) {
				final String shaper = this.config.getString(this.confIdEntryShaper);
				Log.verbose("shaper entry : " + shaper);
				if (!shaper.isEmpty()) {
					this.widgetEntry.setPropertyConfig(Uri.valueOf(shaper));
				}
			}
			this.widgetEntry.setPropertyExpand(new Vector3b(true, false, false));
			this.widgetEntry.setPropertyFill(Vector3b.TRUE);
		}
		if (this.widgetButtonDown == null) {
			this.widgetButtonDown = new Button();
			if (this.config != null) {
				final String shaper = this.config.getString(this.confIdDownShaper);
				Log.verbose("shaper button DOWN : " + shaper);
				if (!shaper.isEmpty()) {
					this.widgetButtonDown.setPropertyConfig(Uri.valueOf(shaper));
				}
			}
			this.widgetButtonDown.setPropertyExpand(new Vector3b(false, false, false));
			this.widgetButtonDown.setPropertyFill(Vector3b.TRUE);
			final String data = this.config.getString(this.confIdDownData);
			final Widget widget = Composer.composerGenerateString(data);
			this.widgetButtonDown.setSubWidget(widget, 0);
		}
		if (this.widgetButtonUp == null) {
			this.widgetButtonUp = new Button();
			if (this.config != null) {
				final String shaper = this.config.getString(this.confIdUpShaper);
				Log.verbose("shaper button UP : " + shaper);
				if (!shaper.isEmpty()) {
					this.widgetButtonUp.setPropertyConfig(Uri.valueOf(shaper));
				}
			}
			this.widgetButtonUp.setPropertyExpand(new Vector3b(false, false, false));
			this.widgetButtonUp.setPropertyFill(Vector3b.TRUE);
			final String data = this.config.getString(this.confIdUpData);
			final Widget widget = Composer.composerGenerateString(data);
			this.widgetButtonUp.setSubWidget(widget);
		}
		switch (this.propertySpinMode) {
			case NONE_NONE:
				subWidgetAdd(this.widgetEntry);
				break;
			case NONE_RIGHT:
				subWidgetAdd(this.widgetEntry);
				subWidgetAdd(this.widgetButtonUp);
				break;
			case LEFT_NONE:
				subWidgetAdd(this.widgetButtonDown);
				subWidgetAdd(this.widgetEntry);
				break;
			case LEFT_RIGHT:
				subWidgetAdd(this.widgetButtonDown);
				subWidgetAdd(this.widgetEntry);
				subWidgetAdd(this.widgetButtonUp);
				break;
			case LEFT_LEFT:
				subWidgetAdd(this.widgetButtonDown);
				subWidgetAdd(this.widgetButtonUp);
				subWidgetAdd(this.widgetEntry);
				break;
			case RIGHT_RIGHT:
				subWidgetAdd(this.widgetEntry);
				subWidgetAdd(this.widgetButtonDown);
				subWidgetAdd(this.widgetButtonUp);
				break;
			default:
				break;
		}
	}
	
}
