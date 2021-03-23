import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.annotation.EwolDescription;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.exml.annotation.XmlManaged;
import org.atriasoft.exml.annotation.XmlName;
import org.atriasoft.exml.annotation.XmlProperty;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
class ProgressBar extends Widget {
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "value")
	@EwolDescription(value = "Value of the progress bar [0..1]")
	protected float propertyValue = 0;
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "color-bg")
	@EwolDescription(value = "ackground color")
	protected Color propertyTextColorFg = Color.BLACK;
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "color-on")
	@EwolDescription(value = "Color of the true value")
	protected Color propertyTextColorBgOn = Color.GREEN;
	@XmlManaged()
	@XmlProperty()
	@XmlName(value = "color-off")
	@EwolDescription(value = "Color of the false value")
	protected Color propertyTextColorBgOff = Color.NONE;
	static private final int DOT_RADIUS = 6;
	public ProgressBar() {
		super();
		setPropertyCanFocus(true);
	}
	
	private CompositingDrawing draw = new CompositingDrawing(); // basic drawing element
	
	protected void onDraw() {
		this.draw.draw();
	}
	
	public void onRegenerateDisplay() {
		if (!needRedraw()) {
			return;
		}
		// clean the object list ...
		this.draw.clear();
		
		this.draw.setColor(propertyTextColorFg);
		
		int tmpSizeX = (int) (this.size.x() - 10);
		int tmpSizeY = (int) (this.size.y() - 10);
		int tmpOriginX = 5;
		int tmpOriginY = 5;
		this.draw.setColor(propertyTextColorBgOn);
		this.draw.setPos(new Vector3f(tmpOriginX, tmpOriginY, 0));
		this.draw.rectangleWidth(new Vector3f(tmpSizeX * propertyValue, tmpSizeY, 0));
		this.draw.setColor(propertyTextColorBgOff);
		this.draw.setPos(new Vector3f(tmpOriginX + tmpSizeX * propertyValue, tmpOriginY, 0));
		this.draw.rectangleWidth(new Vector3f(tmpSizeX * (1.0f - propertyValue), tmpSizeY, 0));
		
		// TODO : Create a better progress Bar ...
		//this.draw.setColor(propertyTextColorFg);
		//this.draw.rectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, 1);
	}
	
	public void calculateMinMaxSize() {
		Vector2f tmpMin = propertyMinSize.getPixel();
		this.minSize = new Vector2f(Math.max(tmpMin.x(), 40.0f),Math.max(tmpMin.y(), DOT_RADIUS * 2.0f) );
		markToRedraw();
	}
	
	public float getPropertyValue() {
		return propertyValue;
	}
	
	public void setPropertyValue(float propertyValue) {
		if (propertyValue == this.propertyValue) {
			return;
		}
		this.propertyValue = propertyValue;
		markToRedraw();
	}
	
	public Color getPropertyTextColorFg() {
		return propertyTextColorFg;
	}
	
	public void setPropertyTextColorFg(Color propertyTextColorFg) {
		if (propertyTextColorFg.equals(this.propertyTextColorFg)) {
			return;
		}
		this.propertyTextColorFg = propertyTextColorFg;
		markToRedraw();
	}
	
	public Color getPropertyTextColorBgOn() {
		return propertyTextColorBgOn;
	}
	
	public void setPropertyTextColorBgOn(Color propertyTextColorBgOn) {
		if (propertyTextColorBgOn.equals(this.propertyTextColorBgOn)) {
			return;
		}
		this.propertyTextColorBgOn = propertyTextColorBgOn;
		markToRedraw();
	}
	
	public Color getPropertyTextColorBgOff() {
		return propertyTextColorBgOff;
	}
	
	public void setPropertyTextColorBgOff(Color propertyTextColorBgOff) {
		if (propertyTextColorBgOff.equals(this.propertyTextColorBgOff)) {
			return;
		}
		this.propertyTextColorBgOff = propertyTextColorBgOff;
		markToRedraw();
	}
}
