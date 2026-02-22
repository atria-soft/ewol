package org.atriasoft.ewol.widget.meta;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.model.SpinPosition;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * @ingroup ewolWidgetGroup
 */
public class NumberInputBase extends Sizer {
	private static final Logger LOGGER = LoggerFactory.getLogger(NumberInputBase.class);

	private SpinPosition propertySpinMode = SpinPosition.RIGHT_RIGHT;

	protected Entry widgetEntry = null;
	protected Button widgetButtonDown = null;
	protected Button widgetButtonUp = null;

	/**
	 * Constructor
	 */
	protected NumberInputBase() {
		setPropertyGravity(Gravity.CENTER);
		updateGui();
	}

	@JsonProperty("spin-mode")
	@JacksonXmlProperty(isAttribute = true, localName = "spin-mode")
	public SpinPosition getPropertySpinMode() {
		return this.propertySpinMode;
	}

	public void setPropertySpinMode(final SpinPosition propertySpinMode) {
		if (this.propertySpinMode == propertySpinMode) {
			return;
		}
		this.propertySpinMode = propertySpinMode;
		updateGui();
	}

	protected void updateGui() {
		subWidgetRemoveAll();
		markToRedraw();
		requestUpdateSize();
		if (this.widgetEntry == null) {
			this.widgetEntry = new Entry();
			this.widgetEntry.setPropertyExpand(Vector2b.TRUE_FALSE);
			this.widgetEntry.setPropertyFill(Vector2b.TRUE);
			this.widgetEntry.setPropertyPadding(new DimensionInsets(6f, 4f, 6f, 4f));
			this.widgetEntry.setPropertyBorderWidth(new DimensionInsets(2f, 1f, 2f, 2f));
			this.widgetEntry.setPropertyBorderRadius(new DimensionBorderRadius(8, 0, 0, 8));
		}
		if (this.widgetButtonDown == null) {
			this.widgetButtonDown = new Button();
			this.widgetButtonDown.setPropertyBorderWidth(new DimensionInsets(2f, 1f, 2f, 1f));
			this.widgetButtonDown.setPropertyExpand(Vector2b.FALSE);
			this.widgetButtonDown.setPropertyFill(Vector2b.TRUE);
			this.widgetButtonDown.setSubWidget(new Label("-"));
		}
		if (this.widgetButtonUp == null) {
			this.widgetButtonUp = new Button();
			this.widgetButtonUp.setPropertyBorderWidth(new DimensionInsets(2, 2, 2, 1));
			this.widgetButtonUp.setPropertyBorderRadius(new DimensionBorderRadius(0, 8, 8, 0));
			this.widgetButtonUp.setPropertyExpand(Vector2b.FALSE);
			this.widgetButtonUp.setPropertyFill(Vector2b.TRUE);
			this.widgetButtonUp.setSubWidget(new Label("+"));
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
