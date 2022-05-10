package sample.atriasoft.ewol;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.atriasoft.aknot.exception.AknotException;
import org.atriasoft.aknot.model.IntrospectionModel;
import org.atriasoft.aknot.pojo.IntrospectionModelComplex;
import org.atriasoft.aknot.pojo.IntrospectionProperty;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.esignal.SignalEmpty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etk.math.Vector3i;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.GravityDepth;
import org.atriasoft.ewol.GravityHorizontal;
import org.atriasoft.ewol.GravityVertical;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.CheckBox;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.Spin;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.Windows;

public class BasicWindows extends Windows {
	private static final String LABEL_GRAVITY = "gravity: ";
	
	Widget testWidget;
	Sizer sizerTestAreaHori;
	Sizer sizerMenu;
	Gravity basicGravity = Gravity.BOTTOM_LEFT;
	
	private final List<Connection> conections = new ArrayList<>();
	
	public BasicWindows() {
		//! [ewol_sample_HW_windows_title]
		setPropertyTitle("No title set !!! for this test");
		
		final Sizer sizerMain = new Sizer(DisplayMode.HORIZONTAL);
		sizerMain.setPropertyExpand(Vector3b.TRUE);
		sizerMain.setPropertyFill(Vector3b.TRUE);
		setSubWidget(sizerMain);
		
		this.sizerMenu = new Sizer(DisplayMode.VERTICAL);
		this.sizerMenu.setPropertyExpand(Vector3b.FALSE_TRUE_FALSE);
		this.sizerMenu.setPropertyLockExpand(Vector3b.TRUE);
		this.sizerMenu.setPropertyFill(Vector3b.TRUE);
		this.sizerMenu.setPropertyMinSize(new Dimension3f(new Vector3f(350, 10, 10), Distance.PIXEL));
		this.sizerMenu.setPropertyGravity(Gravity.TOP);
		sizerMain.subWidgetAdd(this.sizerMenu);
		
		final Sizer sizerVertMain = new Sizer(DisplayMode.VERTICAL);
		sizerVertMain.setPropertyExpand(Vector3b.TRUE);
		sizerVertMain.setPropertyFill(Vector3b.TRUE);
		sizerMain.subWidgetAdd(sizerVertMain);
		
		{
			final Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_128, Distance.PIXEL));
			simpleSpacer.setPropertyColor(Color.ALICE_BLUE);
			simpleSpacer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			sizerVertMain.subWidgetAdd(simpleSpacer);
		}
		
		this.sizerTestAreaHori = new Sizer(DisplayMode.HORIZONTAL);
		this.sizerTestAreaHori.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
		this.sizerTestAreaHori.setPropertyExpandIfFree(Vector3b.TRUE);
		this.sizerTestAreaHori.setPropertyFill(Vector3b.TRUE_FALSE_FALSE);
		sizerVertMain.subWidgetAdd(this.sizerTestAreaHori);
		
		{
			final Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.DARK_GREEN);
			simpleSpacer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			sizerVertMain.subWidgetAdd(simpleSpacer);
		}
	}
	
	public void addButton(final Widget widget) {
		this.sizerMenu.subWidgetAdd(widget);
	}
	
	public void addMenuBoolean(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Boolean value) {
			final CheckBox checkBox = new CheckBox("Y");
			checkBox.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			checkBox.setPropertyFill(Vector3b.TRUE);
			checkBox.setPropertyValue(value);
			this.sizerMenu.subWidgetAdd(checkBox);
			final Connection con = checkBox.signalValue.connect((valueButton) -> {
				try {
					pojo.setExistingValue(widget, valueButton);
				} catch (final AknotException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					return;
				}
			});
			this.conections.add(con);
		}
	}
	
	public void addMenuDimension2f(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Dimension2f value) {
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.size().x());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension2f castedValue) {
							Log.warning("Set new value: {}", castedValue.withSize(castedValue.size().withX(valueButton)));
							pojo.setExistingValue(widget, castedValue.withSize(castedValue.size().withX(valueButton)));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.size().y());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension2f castedValue) {
							Log.warning("Set new value: {}", castedValue.withSize(castedValue.size().withY(valueButton)));
							pojo.setExistingValue(widget, castedValue.withSize(castedValue.size().withY(valueButton)));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
		}
	}
	
	public void addMenuDimension3f(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Dimension3f value) {
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.size().x());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension3f castedValue) {
							Log.warning("Set new value: {}", castedValue.withSize(castedValue.size().withX(valueButton)));
							pojo.setExistingValue(widget, castedValue.withSize(castedValue.size().withX(valueButton)));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.size().y());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension3f castedValue) {
							Log.warning("Set new value: {}", castedValue.withSize(castedValue.size().withY(valueButton)));
							pojo.setExistingValue(widget, castedValue.withSize(castedValue.size().withY(valueButton)));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("Z");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.size().z());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension3f castedValue) {
							Log.warning("Set new value: {}", castedValue.withSize(castedValue.size().withZ(valueButton)));
							pojo.setExistingValue(widget, castedValue.withSize(castedValue.size().withZ(valueButton)));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
		}
	}
	
	public void addMenuDouble(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Double value) {
			final Spin spin = new Spin();
			spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			spin.setPropertyFill(Vector3b.TRUE);
			spin.setPropertyValue((int) (double) value);
			this.sizerMenu.subWidgetAdd(spin);
			final Connection con = spin.signalValue.connect((valueButton) -> {
				try {
					Log.warning("Set new value: {}", valueButton);
					pojo.setExistingValue(widget, (double) valueButton);
				} catch (final AknotException e) {
					e.printStackTrace();
					return;
				}
			});
			this.conections.add(con);
		}
	}
	
	public void addMenuFloat(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Float value) {
			final Spin spin = new Spin();
			spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			spin.setPropertyFill(Vector3b.TRUE);
			spin.setPropertyValue((int) (float) value);
			this.sizerMenu.subWidgetAdd(spin);
			final Connection con = spin.signalValue.connect((valueButton) -> {
				try {
					Log.warning("Set new value: {}", valueButton);
					pojo.setExistingValue(widget, (float) valueButton);
				} catch (final AknotException e) {
					e.printStackTrace();
					return;
				}
			});
			this.conections.add(con);
		}
	}
	
	public void addMenuGravity(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Gravity value) {
			final Button buttonGravity = Button.createLabelButton("Gravity");
			buttonGravity.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			buttonGravity.setPropertyFill(Vector3b.TRUE);
			buttonGravity.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			buttonGravity.setPropertyGravity(Gravity.CENTER);
			this.sizerMenu.subWidgetAdd(buttonGravity);
			final Label gravLabel = (Label) (buttonGravity.getSubWidgets()[0]);
			gravLabel.setPropertyValue(LABEL_GRAVITY + Gravity.BOTTOM_LEFT);
			
			final Connection con = buttonGravity.signalClick.connect(() -> {
				try {
					final Object oldValue = pojo.getValue(widget);
					if (oldValue instanceof final Gravity castedValue) {
						Gravity state = castedValue;
						// TODO: I change the gravity model to integrate the 3rd rank...
						if (state.x() == GravityHorizontal.LEFT && state.y() == GravityVertical.BOTTOM) {
							state = new Gravity(GravityHorizontal.CENTER, GravityVertical.BOTTOM, GravityDepth.CENTER);
						} else if (state.x() == GravityHorizontal.CENTER && state.y() == GravityVertical.BOTTOM) {
							state = new Gravity(GravityHorizontal.RIGHT, GravityVertical.BOTTOM, GravityDepth.CENTER);
						} else if (state.x() == GravityHorizontal.RIGHT && state.y() == GravityVertical.BOTTOM) {
							state = new Gravity(GravityHorizontal.LEFT, GravityVertical.CENTER, GravityDepth.CENTER);
							
						} else if (state.x() == GravityHorizontal.LEFT && state.y() == GravityVertical.CENTER) {
							state = new Gravity(GravityHorizontal.CENTER, GravityVertical.CENTER, GravityDepth.CENTER);
						} else if (state.x() == GravityHorizontal.CENTER && state.y() == GravityVertical.CENTER) {
							state = new Gravity(GravityHorizontal.RIGHT, GravityVertical.CENTER, GravityDepth.CENTER);
						} else if (state.x() == GravityHorizontal.RIGHT && state.y() == GravityVertical.CENTER) {
							state = new Gravity(GravityHorizontal.LEFT, GravityVertical.TOP, GravityDepth.CENTER);
							
						} else if (state.x() == GravityHorizontal.LEFT && state.y() == GravityVertical.TOP) {
							state = new Gravity(GravityHorizontal.CENTER, GravityVertical.TOP, GravityDepth.CENTER);
						} else if (state.x() == GravityHorizontal.CENTER && state.y() == GravityVertical.TOP) {
							state = new Gravity(GravityHorizontal.RIGHT, GravityVertical.TOP, GravityDepth.CENTER);
						} else if (state.x() == GravityHorizontal.RIGHT && state.y() == GravityVertical.TOP) {
							state = new Gravity(GravityHorizontal.LEFT, GravityVertical.BOTTOM, GravityDepth.CENTER);
						}
						gravLabel.setPropertyValue(LABEL_GRAVITY + state.toString());
						Log.warning("Set new value: {}", state);
						pojo.setExistingValue(widget, state);
					}
				} catch (final AknotException e) {
					e.printStackTrace();
					return;
				}
			});
			this.conections.add(con);
		}
	}
	
	public void addMenuInt(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Integer value) {
			final Spin spin = new Spin();
			spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			spin.setPropertyFill(Vector3b.TRUE);
			spin.setPropertyValue(value);
			this.sizerMenu.subWidgetAdd(spin);
			final Connection con = spin.signalValue.connect((valueButton) -> {
				try {
					Log.warning("Set new value: {}", valueButton);
					pojo.setExistingValue(widget, (int) (long) valueButton);
					
				} catch (final AknotException e) {
					e.printStackTrace();
					return;
				}
			});
			this.conections.add(con);
		}
	}
	
	public void addMenuLong(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Long value) {
			final Spin spin = new Spin();
			spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			spin.setPropertyFill(Vector3b.TRUE);
			spin.setPropertyValue(value);
			this.sizerMenu.subWidgetAdd(spin);
			final Connection con = spin.signalValue.connect((valueButton) -> {
				try {
					Log.warning("Set new value: {}", valueButton);
					pojo.setExistingValue(widget, valueButton);
				} catch (final AknotException e) {
					e.printStackTrace();
					return;
				}
			});
			this.conections.add(con);
		}
	}
	
	public void addMenuString(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final String value) {
			final Entry element = new Entry();
			element.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			element.setPropertyFill(Vector3b.TRUE);
			element.setPropertyValue(value);
			this.sizerMenu.subWidgetAdd(element);
			final Connection con = element.signalModify.connect((valueButton) -> {
				try {
					Log.warning("Set new value: {}", valueButton);
					pojo.setExistingValue(widget, valueButton);
				} catch (final AknotException e) {
					e.printStackTrace();
					return;
				}
			});
			this.conections.add(con);
		}
	}
	
	public void addMenuURI(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Uri value) {
			final Entry element = new Entry();
			element.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			element.setPropertyFill(Vector3b.TRUE);
			element.setPropertyValue(value.toString());
			this.sizerMenu.subWidgetAdd(element);
			final Connection con = element.signalModify.connect((valueButton) -> {
				try {
					Log.warning("Set new value: {}", valueButton);
					pojo.setExistingValue(widget, Uri.valueOf(valueButton));
				} catch (final AknotException e) {
					e.printStackTrace();
					return;
				}
			});
			this.conections.add(con);
		}
	}
	
	public void addMenuVector2b(final Widget widget, final IntrospectionProperty pojo) {
		final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Vector3b value) {
			lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			lineSizer.setPropertyFill(Vector3b.TRUE);
			this.sizerMenu.subWidgetAdd(lineSizer);
			{
				final CheckBox checkBox = new CheckBox("X");
				checkBox.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				checkBox.setPropertyFill(Vector3b.TRUE);
				checkBox.setPropertyValue(value.x());
				lineSizer.subWidgetAdd(checkBox);
				final Connection con = checkBox.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2b castedValue) {
							Log.warning("Set new value: {}", castedValue.withX(valueButton));
							pojo.setExistingValue(widget, castedValue.withX(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final CheckBox checkBox = new CheckBox("Y");
				checkBox.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				checkBox.setPropertyFill(Vector3b.TRUE);
				checkBox.setPropertyValue(value.y());
				lineSizer.subWidgetAdd(checkBox);
				final Connection con = checkBox.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2b castedValue) {
							pojo.setExistingValue(widget, castedValue.withY(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
		}
		
	}
	
	public void addMenuVector2f(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Vector2f value) {
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.x());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2f castedValue) {
							Log.warning("Set new value: {}", castedValue.withX(valueButton));
							pojo.setExistingValue(widget, castedValue.withX(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.y());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2f castedValue) {
							Log.warning("Set new value: {}", castedValue.withY(valueButton));
							pojo.setExistingValue(widget, castedValue.withY(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
		}
	}
	
	public void addMenuVector2i(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Vector2i value) {
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue(value.x());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2i castedValue) {
							Log.warning("Set new value: {}", castedValue.withX((int) (long) valueButton));
							pojo.setExistingValue(widget, castedValue.withX((int) (long) valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue(value.y());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2i castedValue) {
							Log.warning("Set new value: {}", castedValue.withY((int) (long) valueButton));
							pojo.setExistingValue(widget, castedValue.withY((int) (long) valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
		}
	}
	
	public void addMenuVector3b(final Widget widget, final IntrospectionProperty pojo) {
		final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Vector3b value) {
			lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
			lineSizer.setPropertyFill(Vector3b.TRUE);
			this.sizerMenu.subWidgetAdd(lineSizer);
			{
				final CheckBox checkBox = new CheckBox("X");
				checkBox.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				checkBox.setPropertyFill(Vector3b.TRUE);
				checkBox.setPropertyValue(value.x());
				lineSizer.subWidgetAdd(checkBox);
				final Connection con = checkBox.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3b castedValue) {
							Log.warning("Set new value: {}", castedValue.withX(valueButton));
							pojo.setExistingValue(widget, castedValue.withX(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final CheckBox checkBox = new CheckBox("Y");
				checkBox.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				checkBox.setPropertyFill(Vector3b.TRUE);
				checkBox.setPropertyValue(value.y());
				lineSizer.subWidgetAdd(checkBox);
				final Connection con = checkBox.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3b castedValue) {
							pojo.setExistingValue(widget, castedValue.withY(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final CheckBox checkBox = new CheckBox("Z");
				checkBox.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				checkBox.setPropertyFill(Vector3b.TRUE);
				checkBox.setPropertyValue(value.z());
				lineSizer.subWidgetAdd(checkBox);
				final Connection con = checkBox.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3b castedValue) {
							pojo.setExistingValue(widget, castedValue.withZ(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
		}
		
	}
	
	public void addMenuVector3f(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Vector3f value) {
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.x());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3f castedValue) {
							Log.warning("Set new value: {}", castedValue.withX(valueButton));
							pojo.setExistingValue(widget, castedValue.withX(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.y());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3f castedValue) {
							Log.warning("Set new value: {}", castedValue.withY(valueButton));
							pojo.setExistingValue(widget, castedValue.withY(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("Z");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue((int) value.z());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3f castedValue) {
							Log.warning("Set new value: {}", castedValue.withZ(valueButton));
							pojo.setExistingValue(widget, castedValue.withZ(valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
		}
	}
	
	public void addMenuVector3i(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Vector3i value) {
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue(value.x());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3i castedValue) {
							Log.warning("Set new value: {}", castedValue.withX((int) (long) valueButton));
							pojo.setExistingValue(widget, castedValue.withX((int) (long) valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue(value.y());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3i castedValue) {
							Log.warning("Set new value: {}", castedValue.withY((int) (long) valueButton));
							pojo.setExistingValue(widget, castedValue.withY((int) (long) valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final Sizer lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				lineSizer.setPropertyFill(Vector3b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final Label simpleLabel = new Label("Z");
				simpleLabel.setPropertyExpand(Vector3b.FALSE);
				simpleLabel.setPropertyFill(Vector3b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final Spin spin = new Spin();
				spin.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				spin.setPropertyFill(Vector3b.TRUE);
				spin.setPropertyValue(value.z());
				lineSizer.subWidgetAdd(spin);
				final Connection con = spin.signalValue.connect((valueButton) -> {
					try {
						final Object oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3i castedValue) {
							Log.warning("Set new value: {}", castedValue.withZ((int) (long) valueButton));
							pojo.setExistingValue(widget, castedValue.withZ((int) (long) valueButton));
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
		}
	}
	
	public void connectAllSignals(final Widget widget, final IntrospectionModelComplex modelPojo) throws Exception {
		Log.warning("Connect all signal(s) on '{}'", widget.getName());
		final List<IntrospectionProperty> signals = modelPojo.getSignals();
		for (final IntrospectionProperty pojo : signals) {
			Log.warning("    - '{}' otherNames={}", pojo.getBeanName(), Arrays.toString(pojo.getNames()));
			Log.warning("        ==> description='{}'", pojo.getDescription());
			Log.warning("        ==> type='{}'", pojo.getType());
			Log.warning("        ==> sub-type='{}'", pojo.getSubType());
			
			final String eventName = pojo.getNames() != null && pojo.getNames().length != 0 ? pojo.getNames()[0] : pojo.getBeanName();
			
			if (pojo.getSubType() != null && pojo.getType() == Signal.class) {
				Log.warning("        ** Signal<{}>", pojo.getSubType());
				final Object signalObject = pojo.getValue(widget);
				if (signalObject == null) {
					Log.error("Signal is not accessible !!!!!!! ");
				} else {
					final String valueNameOfSignal = eventName;
					@SuppressWarnings("unchecked")
					final Signal<Object> tmp = (Signal<Object>) signalObject;
					tmp.connect((object) -> {
						Log.print("Get event from '{}' value='{}'", valueNameOfSignal, object);
					});
				}
			}
			if (pojo.getSubType() == null && pojo.getType() == SignalEmpty.class) {
				Log.warning("        ** SignalEmpty");
				final Object signalObject = pojo.getValue(widget);
				if (signalObject == null) {
					Log.error("Signal is not accessible !!!!!!! ");
				} else {
					final String valueNameOfSignal = eventName;
					final SignalEmpty tmp = (SignalEmpty) signalObject;
					tmp.connect(() -> {
						Log.print("Get event from '{}'", valueNameOfSignal);
					});
				}
				
			}
		}
	}
	
	public void displayAllPropertyWithType(final Widget widget, final IntrospectionModel modelPojo) throws Exception {
		Log.warning("Connect all property(ies) on '{}'", widget.getName());
		final List<IntrospectionProperty> atributes = modelPojo.getAttributes();
		for (final IntrospectionProperty pojo : atributes) {
			Log.warning("    - '{}' otherNames={}", pojo.getBeanName(), Arrays.toString(pojo.getNames()));
			Log.warning("        ==> description='{}'", pojo.getDescription());
			Log.warning("        ==> type='{}'", pojo.getType());
			Log.warning("        ==> sub-type='{}'", pojo.getSubType());
			final String propertyName = pojo.getNames() != null && pojo.getNames().length != 0 ? pojo.getNames()[0] : pojo.getBeanName();
			if (pojo.getType() == int.class || pojo.getType() == Integer.class) {
				addMenuInt(widget, pojo);
			} else if (pojo.getType() == long.class || pojo.getType() == Long.class) {
				addMenuLong(widget, pojo);
			} else if (pojo.getType() == boolean.class || pojo.getType() == Boolean.class) {
				addMenuBoolean(widget, pojo);
			} else if (pojo.getType() == float.class || pojo.getType() == Float.class) {
				addMenuFloat(widget, pojo);
			} else if (pojo.getType() == double.class || pojo.getType() == Double.class) {
				addMenuDouble(widget, pojo);
			} else if (pojo.getType() == String.class) {
				addMenuString(widget, pojo);
			} else if (pojo.getType() == Vector3f.class) {
				addMenuVector3f(widget, pojo);
			} else if (pojo.getType() == Vector2f.class) {
				addMenuVector2f(widget, pojo);
			} else if (pojo.getType() == Vector3b.class) {
				addMenuVector3b(widget, pojo);
			} else if (pojo.getType() == Vector2b.class) {
				addMenuVector2b(widget, pojo);
			} else if (pojo.getType() == Vector3i.class) {
				addMenuVector3i(widget, pojo);
			} else if (pojo.getType() == Vector2i.class) {
				addMenuVector2i(widget, pojo);
			} else if (pojo.getType() == Dimension3f.class) {
				addMenuDimension3f(widget, pojo);
			} else if (pojo.getType() == Dimension2f.class) {
				addMenuDimension2f(widget, pojo);
			} else if (pojo.getType() == DisplayMode.class) {
				Log.error("        ==> plop");
			} else if (pojo.getType() == Uri.class) {
				addMenuURI(widget, pojo);
			} else if (pojo.getType() == Gravity.class) {
				addMenuGravity(widget, pojo);
			} else {
				Log.error("        ==> property type unknown='{}'", pojo.getType());
			}
			{
				final Label simpleLabel = new Label("<b>" + propertyName + ":</b>");
				simpleLabel.setPropertyExpand(Vector3b.TRUE_FALSE_FALSE);
				simpleLabel.setPropertyFill(Vector3b.FALSE);
				simpleLabel.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				this.sizerMenu.subWidgetAdd(simpleLabel);
			}
		}
	}
	
	public void setTestWidget(final Widget widget) {
		try {
			final IntrospectionModelComplex modelPojo = new IntrospectionModelComplex(widget.getClass());
			
			connectAllSignals(widget, modelPojo);
			displayAllPropertyWithType(widget, modelPojo);
		} catch (final Exception e) {
			e.printStackTrace();
		}
		this.sizerTestAreaHori.subWidgetRemoveAll();
		{
			final Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.CHOCOLATE);
			simpleSpacer.setPropertyExpand(Vector3b.FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerTestAreaHori.subWidgetAdd(simpleSpacer);
		}
		this.testWidget = widget;
		this.sizerTestAreaHori.subWidgetAdd(this.testWidget);
		{
			final Spacer simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.GREEN_YELLOW);
			simpleSpacer.setPropertyExpand(Vector3b.FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector3b.TRUE);
			simpleSpacer.setPropertyFill(Vector3b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension3f(Vector3f.VALUE_16, Distance.PIXEL));
			this.sizerTestAreaHori.subWidgetAdd(simpleSpacer);
		}
		// update properties...
		//final Vector3b stateExpand = this.testWidget.getPropertyExpand();
		//this.buttonExpandX.setPropertyValue(stateExpand.x());
		//this.buttonExpandY.setPropertyValue(stateExpand.y());
		//final Vector3b stateFill = this.testWidget.getPropertyFill();
		//this.buttonFillX.setPropertyValue(stateFill.x());
		//this.buttonFillY.setPropertyValue(stateFill.y());
		
		//final Gravity gravity = this.testWidget.getPropertyGravity();
		//final Label gravLabel = (Label) (this.buttonGravity.getSubWidgets()[0]);
		//gravLabel.setPropertyValue(LABEL_GRAVITY + gravity.toString());
	}
	
}