import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.exml.model.XmlElement;
import org.atriasoft.exml.model.XmlNode;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
/*
 * @ingroup ewolWidgetGroup
 * the Cotainer widget is a widget that have an only one subWidget
 */
class Container extends Widget {
	protected Widget subWidget = null;
	/**
	 * Constructor
	 */
	public Container() {
		super();
	}
	/**
	 * get the main node widget
	 * @return the requested pointer on the node
	 */
	public Widget getSubWidget(){
		return this.subWidget;
	}
	/**
	 * set the subWidget node widget.
	 * @param _newWidget The widget to add.
	 */
	public void setSubWidget(Widget _newWidget){
		if (_newWidget == null) {
			return;
		}
		subWidgetRemove();
		this.subWidget = _newWidget;
		if (this.subWidget != null) {
			this.subWidget.setParent(this);
		}
		markToRedraw();
		requestUpdateSize();
	}
	/**
	 * Replace a old subwidget with a new one.
	 * @param _oldWidget The widget to replace.
	 * @param _newWidget The widget to set.
	 */
	public void subWidgetReplace( Widget _oldWidget,
								   Widget _newWidget){
		if (this.subWidget != _oldWidget) {
			Log.warning("Request replace with a wrong old widget");
			return;
		}
		this.subWidget.removeParent();
		this.subWidget = _newWidget;
		if (this.subWidget != null) {
			this.subWidget.setParent(this);
		}
		markToRedraw();
		requestUpdateSize();
	}
	/**
	 * remove the subWidget node (async).
	 */
	public void subWidgetRemove() {
		if (this.subWidget != null) {
			this.subWidget.removeParent();
			this.subWidget = null;
			markToRedraw();
			requestUpdateSize();
		}
	}
	/**
	 * Unlink the subwidget Node.
	 */
	public void subWidgetUnLink(){
		if (this.subWidget != null) {
			this.subWidget.removeParent();
		}
		this.subWidget = null;
	}
	public void systemDraw( DrawProperty _displayProp){
		if (propertyHide){
			// widget is hidden ...
			return;
		}
		super.systemDraw(_displayProp);
		if (this.subWidget != null) {
			DrawProperty prop = _displayProp.withLimit(this.origin, this.size);
			//Log.info("Draw : [" + propertyName + "] t=" + getObjectType() + " o=" + this.origin + "  s=" + this.size);
			this.subWidget.systemDraw(prop);
		} else {
			Log.info("[" + getId() + "]       ++++++ : [null]");
		}
	}
	public void onRegenerateDisplay() {
		if (this.subWidget != null) {
			this.subWidget.onRegenerateDisplay();
		}
	}
	public void onChangeSize() {
		super.onChangeSize();
		if (propertyHide) {
			return;
		}
		if (this.subWidget == null) {
			return;
		}
		Vector2f origin = this.origin.add(this.offset);
		Vector2f minSize = this.subWidget.getCalculateMinSize();
		Vector2b expand = this.subWidget.getPropertyExpand();
		origin = origin.add(Gravity.gravityGenerateDelta(propertyGravity, minSize.less(this.size)));
		this.subWidget.setOrigin(origin);
		this.subWidget.setSize(this.size);
		this.subWidget.onChangeSize();
	}
	public void calculateMinMaxSize(){
		// call main class
		super.calculateMinMaxSize();
		// call sub classes
		if (this.subWidget != null) {
			this.subWidget.calculateMinMaxSize();
			Vector2f min = this.subWidget.getCalculateMinSize();
			this.minSize = Vector2f.max(this.minSize, min);
		}
		//Log.error("[" + getId() + "] Result min size : " +  this.minSize);
	}
	public Widget getWidgetAtPos( Vector2f _pos) {
		if (!propertyHide) {
			if (this.subWidget != null) {
				return this.subWidget.getWidgetAtPos(_pos);
			}
		}
		return null;
	};
	public EwolObject getSubObjectNamed( String _objectName) {
		EwolObject tmpObject = super.getSubObjectNamed(_objectName);
		if (tmpObject != null) {
			return tmpObject;
		}
		if (this.subWidget != null) {
			return this.subWidget.getSubObjectNamed(_objectName);
		}
		return null;
	}
	
	public boolean loadXML( XmlElement _node)  {
		if (_node == null) {
			return false;
		}
		// parse generic properties:
		super.loadXML(_node);
		// remove previous element:
		subWidgetRemove();
		// parse all the elements:
		for (XmlNode it : _node.getNodes()) {
			if (!it.isElement()) {
				// trash here all that is not element
				continue;
			}
			XmlElement pNode = it.toElement();
			String widgetName = pNode.getValue();
			Log.verbose("[" + getId() + "] t=" + getClass().getCanonicalName() + " Load node name : '" + widgetName + "'");
			if (!getWidgetManager().exist(widgetName)) {
				Log.error("Unknown basic node='" + widgetName + "' not in : [" + getWidgetManager().list() + "]" );
				continue;
			}
			if (getSubWidget() != null) {
				Log.error("Can only have one subWidget ??? node='" + widgetName + "'" );
				continue;
			}
			Log.debug("try to create subwidget : '" + widgetName + "'");
			Widget tmpWidget = getWidgetManager().create(widgetName, pNode);
			if (tmpWidget == null) {
				Log.error ("Can not create the widget : '" + widgetName + "'");
				continue;
			}
			// add widget :
			setSubWidget(tmpWidget);
			if (!tmpWidget.loadXML(pNode)) {
				Log.error ("can not load widget properties : '" + widgetName + "'");
				return false;
			}
		}
		if (_node.getNodes().size() != 0 && this.subWidget == null) {
			Log.warning("Load container with no data inside");
		}
		return true;
	}
	public void setOffset( Vector2f _newVal) {
		if (this.offset.equals(_newVal)) {
			return;
		}
		super.setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		onChangeSize();
	
	}
	public void requestDestroyFromChild( EwolObject _child){
		if (this.subWidget != _child) {
			return;
		}
		if (this.subWidget == null) {
			return;
		}
		this.subWidget.removeParent();
		this.subWidget = null;
		markToRedraw();
	}
	public void drawWidgetTree(int _level)  {
		super.drawWidgetTree(_level);
		_level++;
		if (this.subWidget != null) {
			this.subWidget.drawWidgetTree(_level);
		}
	}
};