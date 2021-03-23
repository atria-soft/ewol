package org.atriasoft.ewol.widget;

import java.lang.ref.WeakReference;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;

import org.atriasoft.ewol.internal.Log;
import org.atriasoft.exml.model.XmlElement;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public class WidgetManager {
	// ---------------------------------------------
	// --  Focus area
	// ---------------------------------------------
	private WeakReference<Widget> focusWidgetDefault; //!< default focus when no current focus is set
	private WeakReference<Widget> focusWidgetCurrent; //!< Current focus selected
	// ---------------------------------------------
	// --  Factory area
	// ---------------------------------------------
	private final Map<String, Class<?>> creatorList = new HashMap<>(); //!< List of factory of a widget
	
	// ---------------------------------------------
	// --  Something change area (TODO: maybe set it in the windows)
	// ---------------------------------------------
	private boolean haveRedraw = true; //!< something request a redraw
	
	private Runnable funcRedrawNeeded = null;
	
	/**
	 * Create a widget with his name.
	 * @param _name Name of the widget to create.
	 * @param _node Reference on the XML node.
	 * @return The widget created (null if it does not exist).
	 */
	/*
	public Widget create( final String _name,  exml::Element _node){
		final String nameLower = _name.toLowerCase();
		final Class<?> it = this.creatorList.get(nameLower);
		if (it != null) {
			try {
				return it.getConstructor().newInstance(_node);
			} catch (InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException | NoSuchMethodException | SecurityException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return null;
			}
		}
		Log.warning("try to create an UnExistant widget : " + nameLower);
		return null;
	}
	*/
	
	public WidgetManager() {
		/*
		this.creatorList.put("Button", Button.class);
		this.creatorList.put("ButtonColor", ButtonColor.class);
		this.creatorList.put("Spacer", Spacer.class);
		this.creatorList.put("Slider", Slider.class);
		this.creatorList.put("Sizer", Sizer.class);
		this.creatorList.put("ProgressBar", ProgressBar.class);
		this.creatorList.put("Layer", Layer.class);
		this.creatorList.put("Label", Label.class);
		this.creatorList.put("Image", Image.class);
		this.creatorList.put("Gird", Gird.class);
		this.creatorList.put("Entry", Entry.class);
		this.creatorList.put("Menu", Menu.class);
		this.creatorList.put("CheckBox", CheckBox.class);
		this.creatorList.put("Scroll", Scroll.class);
		this.creatorList.put("ContextMenu", ContextMenu.class);
		this.creatorList.put("PopUp", PopUp.class);
		this.creatorList.put("WSlider", WSlider.class);
		this.creatorList.put("ListFileSystem", ListFileSystem.class);
		this.creatorList.put("Composer", Composer.class);
		this.creatorList.put("Select", Select.class);
		this.creatorList.put("Spin", Spin.class);
		 */
	}
	
	/**
	 * @throws Exception 
	 * add a factory of a specific widget.
	 * @param _name Name of the widget that is associated of the factory.
	 * @param _class class interface
	 */
	public void addWidgetCreator(final String _name, final Class<?> _class) throws Exception {
		if (_class == null) {
			throw new Exception("Can not add widget creator without specified class.");
		}
		//Keep name in lower case :
		final String nameLower = _name.toLowerCase();
		final Class<?> it = this.creatorList.get(nameLower);
		if (it != null) {
			Log.warning("Replace Creator of a specify widget : " + nameLower);
			return;
		}
		this.creatorList.put(nameLower, _class);
		// TODO check constructors ...
	}
	
	/**
	 * Create a widget with his name.
	 * @param _name Name of the widget to create.
	 * @return The widget created (null if it does not exist).
	 */
	public Widget create(final String _name) {
		final String nameLower = _name.toLowerCase();
		final Class<?> it = this.creatorList.get(nameLower);
		if (it != null) {
			try {
				return (Widget) it.getConstructor().newInstance();
			} catch (InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException | NoSuchMethodException | SecurityException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return null;
			}
		}
		Log.warning("try to create an UnExistant widget : " + nameLower);
		return null;
	}
	public Widget create(final String _name, XmlElement node) {
		final String nameLower = _name.toLowerCase();
		final Class<?> it = this.creatorList.get(nameLower);
		if (it != null) {
			try {
				Widget tmp = (Widget) it.getConstructor().newInstance();
				tmp.loadXML(node);
			} catch (InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException | NoSuchMethodException | SecurityException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return null;
			}
		}
		Log.warning("try to create an UnExistant widget : " + nameLower);
		return null;
	}
	
	/**
	 * Check if an Widget exist
	 * @param _name Name of the widget to check.
	 * @return true The Widget exist.
	 * @return false The Widget Does NOT exist.
	 */
	public boolean exist(final String _name) {
		return this.creatorList.get(_name.toLowerCase()) != null;
	}
	
	/**
	 * Get the current Focused widget.
	 * @return The pointer on the current focused element.
	 */
	public Widget focusGet() {
		return this.focusWidgetCurrent.get();
	}
	
	/**
	 * Request a focus on a specify widget.
	 * @param _newWidget Widget that might get the focus.
	 */
	public void focusKeep(final Widget _newWidget) {
		if (_newWidget == null) {
			// nothing to do ...
			return;
		}
		Log.debug("focusKeep=" + _newWidget.getId());
		//elog::displayBacktrace();
		Widget focusWidgetCurrent = this.focusWidgetCurrent.get();
		if (_newWidget == focusWidgetCurrent) {
			// nothing to do ... 
			return;
		}
		if (focusWidgetCurrent != null) {
			Log.debug("Rm focus on WidgetID=" + focusWidgetCurrent.getId());
			focusWidgetCurrent.rmFocus();
			focusWidgetCurrent = null;
		}
		if (!_newWidget.propertyCanFocus) {
			Log.debug("Widget can not have focus, id=" + _newWidget.getId());
			return;
		}
		this.focusWidgetCurrent = new WeakReference<>(_newWidget);
	}
	
	/**
	 * Release the current focus (back on default if possible).
	 */
	public void focusRelease() {
		final Widget focusWidgetDefault = this.focusWidgetDefault.get();
		Widget focusWidgetCurrent = this.focusWidgetCurrent.get();
		if (focusWidgetDefault == focusWidgetCurrent) {
			// nothink to do ...
			return;
		}
		if (focusWidgetCurrent != null) {
			Log.debug("Rm focus on WidgetID=" + focusWidgetCurrent.getId());
			focusWidgetCurrent.rmFocus();
		}
		this.focusWidgetCurrent = this.focusWidgetDefault;
		focusWidgetCurrent = this.focusWidgetCurrent.get();
		if (focusWidgetCurrent != null) {
			Log.debug("Set focus on WidgetID=" + focusWidgetCurrent.getId());
			focusWidgetCurrent.setFocus();
		}
	}
	
	/**
	 * Set the default focus when none selected.
	 * @param _newWidget Widget that might get the focus (when nothing else).
	 */
	public void focusSetDefault(final Widget _newWidget) {
		if ((_newWidget != null) && (!_newWidget.propertyCanFocus)) {
			Log.verbose("Widget can not have focus, id=" + _newWidget.getId());
			return;
		}
		final Widget focusWidgetDefault = this.focusWidgetDefault.get();
		final Widget focusWidgetCurrent = this.focusWidgetCurrent.get();
		if (focusWidgetDefault == focusWidgetCurrent) {
			if (focusWidgetCurrent != null) {
				Log.debug("Rm focus on WidgetID=" + focusWidgetCurrent.getId());
				focusWidgetCurrent.rmFocus();
			}
			this.focusWidgetCurrent = new WeakReference<>(_newWidget);
			if (_newWidget != null) {
				Log.debug("Set focus on WidgetID=" + _newWidget.getId());
				_newWidget.setFocus();
			}
		}
		this.focusWidgetDefault = new WeakReference<>(_newWidget);
	}
	
	/**
	 * Check if a redraw has been requested (set the local value back at false)
	 * @return true if something to be redraw
	 */
	public boolean isDrawingNeeded() {
		final boolean tmp = this.haveRedraw;
		this.haveRedraw = false;
		return tmp;
	}
	
	/**
	 * Get the list of all Widget that can be created.
	 * @return Separate with ',' string list.
	 */
	public String list() {
		return this.creatorList.keySet().toString();
	}
	
	/**
	 * Mark the display to redraw
	 */
	public void markDrawingIsNeeded() {
		if (this.haveRedraw) {
			return;
		}
		this.haveRedraw = true;
		if (this.funcRedrawNeeded != null) {
			this.funcRedrawNeeded.run();
		}
	}
	
	/**
	 * Set a callback when we need redraw the display (need by MacOs)
	 * @param _func function to call
	 */
	public void setCallbackonRedrawNeeded(final Runnable _func) {
		this.funcRedrawNeeded = _func;
	}
	
}
