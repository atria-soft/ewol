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
import org.atriasoft.etk.Dimension1f;
import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.DimensionBorderRadius;
import org.atriasoft.etk.DimensionInsets;
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
import org.atriasoft.ewol.widget.Container;
import org.atriasoft.ewol.widget.Entry;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.ScrollView;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.Spin;
import org.atriasoft.ewol.widget.Widget;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ModelWidget extends Container {
	private static final Logger LOGGER = LoggerFactory.getLogger(ModelWidget.class);
	private static final String LABEL_GRAVITY = "gravity: ";
	
	Widget testWidget;
	Sizer sizerTestAreaHori;
	Sizer sizerMenuRoot;
	Sizer sizerMenu;
	
	// Log callback provided by BasicWindows
	private java.util.function.Consumer<String> logCallback;
	
	Gravity basicGravity = Gravity.BOTTOM_LEFT;
	
	private final List<Connection> conections = new ArrayList<>();
	
	private boolean isMetaWidget = false;

	public ModelWidget(final TestWidgetInterface interfaceToTest) {
		this(interfaceToTest, null);
	}

	public ModelWidget(final TestWidgetInterface interfaceToTest,
			final java.util.function.Consumer<String> logCallback) {
		this.logCallback = logCallback;
		this.isMetaWidget = interfaceToTest.isMetaWidget();
		
		setPropertyExpand(Vector2b.TRUE);
		setPropertyFill(Vector2b.TRUE);
		final var sizerMain = new Sizer(DisplayMode.HORIZONTAL);
		sizerMain.setPropertyExpand(Vector2b.TRUE);
		sizerMain.setPropertyFill(Vector2b.TRUE);
		setSubWidget(sizerMain);
		
		this.sizerMenuRoot = new Sizer(DisplayMode.VERTICAL);
		this.sizerMenuRoot.setPropertyExpand(Vector2b.FALSE_TRUE);
		this.sizerMenuRoot.setPropertyLockExpand(Vector2b.TRUE);
		this.sizerMenuRoot.setPropertyFill(Vector2b.TRUE);
		this.sizerMenuRoot.setPropertyMinSize(new Dimension2f(new Vector2f(350, 10), Distance.PIXEL));
		this.sizerMenuRoot.setPropertyGravity(Gravity.TOP);
		sizerMain.subWidgetAdd(this.sizerMenuRoot);
		
		this.sizerMenu = new Sizer(DisplayMode.VERTICAL);
		this.sizerMenu.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.sizerMenu.setPropertyFill(Vector2b.TRUE_FALSE);
		this.sizerMenu.setPropertyMinSize(new Dimension2f(new Vector2f(350, 10), Distance.PIXEL));
		this.sizerMenu.setPropertyGravity(Gravity.TOP);

		// Wrap sizerMenu in a ScrollView for scrolling when there are many parameters
		final var scrollView = ScrollView.create().content(this.sizerMenu).showVertical(true).showHorizontal(false)
				.expand(true, true).fill(true, true);
		scrollView.setPropertyGravity(Gravity.TOP);
		this.sizerMenuRoot.subWidgetAdd(scrollView);
		
		final var sizerVertMain = new Sizer(DisplayMode.VERTICAL);
		sizerVertMain.setPropertyExpand(Vector2b.TRUE);
		sizerVertMain.setPropertyFill(Vector2b.TRUE);
		sizerMain.subWidgetAdd(sizerVertMain);
		
		{
			final var simpleSpacer = new Spacer();
			simpleSpacer.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_128, Distance.PIXEL));
			simpleSpacer.setPropertyColor(Color.ALICE_BLUE);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector2b.TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
			sizerVertMain.subWidgetAdd(simpleSpacer);
		}
		
		this.sizerTestAreaHori = new Sizer(DisplayMode.HORIZONTAL);
		this.sizerTestAreaHori.setPropertyExpand(Vector2b.TRUE_FALSE);
		this.sizerTestAreaHori.setPropertyExpandIfFree(Vector2b.TRUE);
		this.sizerTestAreaHori.setPropertyFill(Vector2b.TRUE_FALSE);
		sizerVertMain.subWidgetAdd(this.sizerTestAreaHori);
		
		{
			final var simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.DARK_GREEN);
			simpleSpacer.setPropertyExpand(Vector2b.TRUE_FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector2b.TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
			sizerVertMain.subWidgetAdd(simpleSpacer);
		}
		
		// add the default widget to test:
		setTestWidget(interfaceToTest.getWidget());
	}
	
	/**
	 * Adds a log entry via the callback.
	 */
	public void addLogEntry(final String message) {
		if (this.logCallback != null) {
			this.logCallback.accept(message);
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
			final var checkBox = new CheckBox("Y");
			checkBox.setPropertyExpand(Vector2b.TRUE_FALSE);
			checkBox.setPropertyFill(Vector2b.TRUE);
			checkBox.setPropertyValue(value);
			this.sizerMenu.subWidgetAdd(checkBox);
			final var con = checkBox.signalValue.connect(valueButton -> {
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
	
	public void addMenuDimension1f(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Dimension1f value) {
			{
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					LOGGER.warn("Receved event for button ...");
					try {
						final var oldValue = pojo.getValue(widget);
						LOGGER.warn("Receved event for button ... {}", oldValue);
						if (oldValue instanceof final Dimension1f castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withSize(valueButton));
							pojo.setExistingValue(widget, new Dimension1f(valueButton));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().x());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension2f castedValue) {
							LOGGER.warn("Set new value: {}",
									castedValue.withSize(castedValue.size().withX(valueButton)));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().y());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension2f castedValue) {
							LOGGER.warn("Set new value: {}",
									castedValue.withSize(castedValue.size().withY(valueButton)));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().x());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension3f castedValue) {
							LOGGER.warn("Set new value: {}",
									castedValue.withSize(castedValue.size().withX(valueButton)));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().y());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension3f castedValue) {
							LOGGER.warn("Set new value: {}",
									castedValue.withSize(castedValue.size().withY(valueButton)));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("Z");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().z());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Dimension3f castedValue) {
							LOGGER.warn("Set new value: {}",
									castedValue.withSize(castedValue.size().withZ(valueButton)));
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
			final var spin = new Spin();
			spin.setPropertyExpand(Vector2b.TRUE_FALSE);
			spin.setPropertyFill(Vector2b.TRUE);
			spin.setPropertyValue((int) (double) value);
			this.sizerMenu.subWidgetAdd(spin);
			final var con = spin.signalValue.connect(valueButton -> {
				try {
					LOGGER.warn("Set new value: {}", valueButton);
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
			final var spin = new Spin();
			spin.setPropertyExpand(Vector2b.TRUE_FALSE);
			spin.setPropertyFill(Vector2b.TRUE);
			spin.setPropertyValue((int) (float) value);
			this.sizerMenu.subWidgetAdd(spin);
			final var con = spin.signalValue.connect(valueButton -> {
				try {
					LOGGER.warn("Set new value: {}", valueButton);
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
			final var buttonGravity = Button.createLabelButton("Gravity");
			buttonGravity.setPropertyExpand(Vector2b.TRUE_FALSE);
			buttonGravity.setPropertyFill(Vector2b.TRUE);
			buttonGravity.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
			buttonGravity.setPropertyGravity(Gravity.CENTER);
			this.sizerMenu.subWidgetAdd(buttonGravity);
			final var gravLabel = (Label) buttonGravity.getSubWidget();
			gravLabel.setPropertyValue(LABEL_GRAVITY + Gravity.BOTTOM_LEFT);
			
			final var con = buttonGravity.signalClick.connect(() -> {
				try {
					final var oldValue = pojo.getValue(widget);
					if (oldValue instanceof final Gravity castedValue) {
						var state = castedValue;
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
						LOGGER.warn("Set new value: {}", state);
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
			final var spin = new Spin();
			spin.setPropertyExpand(Vector2b.TRUE_FALSE);
			spin.setPropertyFill(Vector2b.TRUE);
			spin.setPropertyValue(value);
			this.sizerMenu.subWidgetAdd(spin);
			final var con = spin.signalValue.connect(valueButton -> {
				try {
					LOGGER.warn("Set new value: {}", valueButton);
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
			final var spin = new Spin();
			spin.setPropertyExpand(Vector2b.TRUE_FALSE);
			spin.setPropertyFill(Vector2b.TRUE);
			spin.setPropertyValue(value);
			this.sizerMenu.subWidgetAdd(spin);
			final var con = spin.signalValue.connect(valueButton -> {
				try {
					LOGGER.warn("Set new value: {}", valueButton);
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
			final var element = new Entry();
			element.setPropertyExpand(Vector2b.TRUE_FALSE);
			element.setPropertyFill(Vector2b.TRUE);
			element.setPropertyValue(value);
			this.sizerMenu.subWidgetAdd(element);
			final var con = element.signalModify.connect(valueButton -> {
				try {
					LOGGER.warn("Set new value: {}", valueButton);
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
			final var element = new Entry();
			element.setPropertyExpand(Vector2b.TRUE_FALSE);
			element.setPropertyFill(Vector2b.TRUE);
			element.setPropertyValue(value.toString());
			this.sizerMenu.subWidgetAdd(element);
			final var con = element.signalModify.connect(valueButton -> {
				try {
					LOGGER.warn("Set new value: {}", valueButton);
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
		final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Vector2b value) {
			lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
			lineSizer.setPropertyFill(Vector2b.TRUE);
			this.sizerMenu.subWidgetAdd(lineSizer);
			{
				final var checkBox = new CheckBox("X");
				checkBox.setPropertyExpand(Vector2b.TRUE_FALSE);
				checkBox.setPropertyFill(Vector2b.TRUE);
				checkBox.setPropertyValue(value.x());
				lineSizer.subWidgetAdd(checkBox);
				final var con = checkBox.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2b castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withX(valueButton));
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
				final var checkBox = new CheckBox("Y");
				checkBox.setPropertyExpand(Vector2b.TRUE_FALSE);
				checkBox.setPropertyFill(Vector2b.TRUE);
				checkBox.setPropertyValue(value.y());
				lineSizer.subWidgetAdd(checkBox);
				final var con = checkBox.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.x());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2f castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withX(valueButton));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.y());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2f castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withY(valueButton));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue(value.x());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2i castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withX((int) (long) valueButton));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue(value.y());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector2i castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withY((int) (long) valueButton));
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
		final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final Vector3b value) {
			lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
			lineSizer.setPropertyFill(Vector2b.TRUE);
			this.sizerMenu.subWidgetAdd(lineSizer);
			{
				final var checkBox = new CheckBox("X");
				checkBox.setPropertyExpand(Vector2b.TRUE_FALSE);
				checkBox.setPropertyFill(Vector2b.TRUE);
				checkBox.setPropertyValue(value.x());
				lineSizer.subWidgetAdd(checkBox);
				final var con = checkBox.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3b castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withX(valueButton));
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
				final var checkBox = new CheckBox("Y");
				checkBox.setPropertyExpand(Vector2b.TRUE_FALSE);
				checkBox.setPropertyFill(Vector2b.TRUE);
				checkBox.setPropertyValue(value.y());
				lineSizer.subWidgetAdd(checkBox);
				final var con = checkBox.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
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
				final var checkBox = new CheckBox("Z");
				checkBox.setPropertyExpand(Vector2b.TRUE_FALSE);
				checkBox.setPropertyFill(Vector2b.TRUE);
				checkBox.setPropertyValue(value.z());
				lineSizer.subWidgetAdd(checkBox);
				final var con = checkBox.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.x());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3f castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withX(valueButton));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.y());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3f castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withY(valueButton));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("Z");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.z());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3f castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withZ(valueButton));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("X");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue(value.x());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3i castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withX((int) (long) valueButton));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("Y");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue(value.y());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3i castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withY((int) (long) valueButton));
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
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("Z");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue(value.z());
				lineSizer.subWidgetAdd(spin);
				final var con = spin.signalValue.connect(valueButton -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof final Vector3i castedValue) {
							LOGGER.warn("Set new value: {}", castedValue.withZ((int) (long) valueButton));
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
		LOGGER.trace("Connect all signal(s) on '{}'", widget.getName());
		final var signals = modelPojo.getSignals();
		for (final IntrospectionProperty pojo : signals) {
			LOGGER.trace("    - '{}' otherNames={}", pojo.getBeanName(), Arrays.toString(pojo.getNames()));
			LOGGER.trace("        ==> description='{}'", pojo.getDescription());
			LOGGER.trace("        ==> type='{}'", pojo.getType());
			LOGGER.trace("        ==> sub-type='{}'", pojo.getSubType());
			
			final var eventName = pojo.getNames() != null && pojo.getNames().length != 0 ? pojo.getNames()[0]
					: pojo.getBeanName();
			
			if (pojo.getSubType() != null && pojo.getType() == Signal.class) {
				LOGGER.trace("        ** Signal<{}>", pojo.getSubType());
				final var signalObject = pojo.getValue(widget);
				if (signalObject == null) {
					LOGGER.error("Signal is not accessible !!!!!!! ");
				} else {
					final var valueNameOfSignal = eventName;
					@SuppressWarnings("unchecked")
					final var tmp = (Signal<Object>) signalObject;
					tmp.connectAuto(this, (final ModelWidget self, final Object object) -> {
						final String logMessage = "<b>" + valueNameOfSignal + "</b>: " + object;
						LOGGER.info("Get event from '{}' value='{}'", valueNameOfSignal, object);
						self.addLogEntry(logMessage);
					});
				}
			}
			if (pojo.getSubType() == null && pojo.getType() == SignalEmpty.class) {
				LOGGER.trace("        ** SignalEmpty");
				final var signalObject = pojo.getValue(widget);
				if (signalObject == null) {
					LOGGER.error("Signal is not accessible !!!!!!! ");
				} else {
					final var valueNameOfSignal = eventName;
					final var tmp = (SignalEmpty) signalObject;
					tmp.connectAuto(this, (final ModelWidget self) -> {
						final String logMessage = "<b>" + valueNameOfSignal + "</b>";
						LOGGER.info("Get event from '{}'", valueNameOfSignal);
						self.addLogEntry(logMessage);
					});
				}
				
			}
		}
	}
	
	public void addMenuDimensionBorderRadius(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final DimensionBorderRadius value) {
			{
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("bottom-left");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(new Vector2f(100, 0), Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().bottomLeft());
				lineSizer.subWidgetAdd(spin);
				final var spacer = new Spacer();
				spacer.setPropertyMinSize(new Dimension2f(new Vector2f(5, 0), Distance.PIXEL));
				lineSizer.subWidgetAdd(spacer);
				final var con = spin.signalValue.connect(newValue -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof DimensionBorderRadius castedValue) {
							castedValue = castedValue.withSize(castedValue.size().withBottomLeft(newValue));
							LOGGER.warn("Set new value: {}", castedValue);
							pojo.setExistingValue(widget, castedValue);
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("bottom-right");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(new Vector2f(100, 0), Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().bottomRight());
				lineSizer.subWidgetAdd(spin);
				final var spacer = new Spacer();
				spacer.setPropertyMinSize(new Dimension2f(new Vector2f(5, 0), Distance.PIXEL));
				lineSizer.subWidgetAdd(spacer);
				final var con = spin.signalValue.connect(newValue -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof DimensionBorderRadius castedValue) {
							castedValue = castedValue.withSize(castedValue.size().withBottomRight(newValue));
							LOGGER.warn("Set new value: {}", castedValue);
							pojo.setExistingValue(widget, castedValue);
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("top-right");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(new Vector2f(100, 0), Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().topRight());
				lineSizer.subWidgetAdd(spin);
				final var spacer = new Spacer();
				spacer.setPropertyMinSize(new Dimension2f(new Vector2f(5, 0), Distance.PIXEL));
				lineSizer.subWidgetAdd(spacer);
				final var con = spin.signalValue.connect(newValue -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof DimensionBorderRadius castedValue) {
							castedValue = castedValue.withSize(castedValue.size().withTopRight(newValue));
							LOGGER.warn("Set new value: {}", castedValue);
							pojo.setExistingValue(widget, castedValue);
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("top-left");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(new Vector2f(100, 0), Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().topLeft());
				lineSizer.subWidgetAdd(spin);
				final var spacer = new Spacer();
				spacer.setPropertyMinSize(new Dimension2f(new Vector2f(5, 0), Distance.PIXEL));
				lineSizer.subWidgetAdd(spacer);
				final var con = spin.signalValue.connect(newValue -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof DimensionBorderRadius castedValue) {
							castedValue = castedValue.withSize(castedValue.size().withTopLeft(newValue));
							LOGGER.warn("Set new value: {}", castedValue);
							pojo.setExistingValue(widget, castedValue);
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
	
	public void addMenuDimensionInsets(final Widget widget, final IntrospectionProperty pojo) {
		Object valueRaw = null;
		try {
			valueRaw = pojo.getValue(widget);
		} catch (final AknotException e) {
			e.printStackTrace();
			return;
		}
		if (valueRaw instanceof final DimensionInsets value) {
			{
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("left");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(new Vector2f(100, 0), Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().left());
				lineSizer.subWidgetAdd(spin);
				final var spacer = new Spacer();
				spacer.setPropertyMinSize(new Dimension2f(new Vector2f(5, 0), Distance.PIXEL));
				lineSizer.subWidgetAdd(spacer);
				final var con = spin.signalValue.connect(newValue -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof DimensionInsets castedValue) {
							castedValue = castedValue.withSize(castedValue.size().withLeft(newValue));
							LOGGER.warn("Set new value: {}", castedValue);
							pojo.setExistingValue(widget, castedValue);
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("bottom");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(new Vector2f(100, 0), Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().bottom());
				lineSizer.subWidgetAdd(spin);
				final var spacer = new Spacer();
				spacer.setPropertyMinSize(new Dimension2f(new Vector2f(5, 0), Distance.PIXEL));
				lineSizer.subWidgetAdd(spacer);
				final var con = spin.signalValue.connect(newValue -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof DimensionInsets castedValue) {
							castedValue = castedValue.withSize(castedValue.size().withBottom(newValue));
							LOGGER.warn("Set new value: {}", castedValue);
							pojo.setExistingValue(widget, castedValue);
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("right");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(new Vector2f(100, 0), Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().right());
				lineSizer.subWidgetAdd(spin);
				final var spacer = new Spacer();
				spacer.setPropertyMinSize(new Dimension2f(new Vector2f(5, 0), Distance.PIXEL));
				lineSizer.subWidgetAdd(spacer);
				final var con = spin.signalValue.connect(newValue -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof DimensionInsets castedValue) {
							castedValue = castedValue.withSize(castedValue.size().withRight(newValue));
							LOGGER.warn("Set new value: {}", castedValue);
							pojo.setExistingValue(widget, castedValue);
						}
					} catch (final AknotException e) {
						e.printStackTrace();
						return;
					}
				});
				this.conections.add(con);
			}
			{
				final var lineSizer = new Sizer(DisplayMode.HORIZONTAL);
				lineSizer.setPropertyExpand(Vector2b.TRUE_FALSE);
				lineSizer.setPropertyFill(Vector2b.TRUE);
				this.sizerMenu.subWidgetAdd(lineSizer);
				
				final var simpleLabel = new Label("top");
				simpleLabel.setPropertyExpand(Vector2b.FALSE);
				simpleLabel.setPropertyFill(Vector2b.TRUE);
				simpleLabel.setPropertyMinSize(new Dimension2f(new Vector2f(100, 0), Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				lineSizer.subWidgetAdd(simpleLabel);
				
				final var spin = new Spin();
				spin.setPropertyExpand(Vector2b.TRUE_FALSE);
				spin.setPropertyFill(Vector2b.TRUE);
				spin.setPropertyValue((int) value.size().top());
				lineSizer.subWidgetAdd(spin);
				final var spacer = new Spacer();
				spacer.setPropertyMinSize(new Dimension2f(new Vector2f(5, 0), Distance.PIXEL));
				lineSizer.subWidgetAdd(spacer);
				final var con = spin.signalValue.connect(newValue -> {
					try {
						final var oldValue = pojo.getValue(widget);
						if (oldValue instanceof DimensionInsets castedValue) {
							castedValue = castedValue.withSize(castedValue.size().withTop(newValue));
							LOGGER.warn("Set new value: {}", castedValue);
							pojo.setExistingValue(widget, castedValue);
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
	
	public void displayAllPropertyWithType(final Widget widget, final IntrospectionModel modelPojo) throws Exception {
		LOGGER.warn("Connect all property(ies) on '{}'", widget.getName());
		final var atributes = modelPojo.getAttributes();
		for (final IntrospectionProperty pojo : atributes) {
			LOGGER.trace("    - '{}' otherNames={}", pojo.getBeanName(), Arrays.toString(pojo.getNames()));
			LOGGER.trace("        ==> description='{}'", pojo.getDescription());
			LOGGER.trace("        ==> type='{}'", pojo.getType());
			LOGGER.trace("        ==> sub-type='{}'", pojo.getSubType());
			final var propertyName = pojo.getNames() != null && pojo.getNames().length != 0 ? pojo.getNames()[0]
					: pojo.getBeanName();
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
			} else if (pojo.getType() == Dimension1f.class) {
				addMenuDimension1f(widget, pojo);
			} else if (pojo.getType() == DimensionBorderRadius.class) {
				addMenuDimensionBorderRadius(widget, pojo);
			} else if (pojo.getType() == DimensionInsets.class) {
				addMenuDimensionInsets(widget, pojo);
			} else if (pojo.getType() == DisplayMode.class) {
				LOGGER.error("        ==> plop");
			} else if (pojo.getType() == Uri.class) {
				addMenuURI(widget, pojo);
			} else if (pojo.getType() == Gravity.class) {
				addMenuGravity(widget, pojo);
			} else {
				LOGGER.error("        ==> property type unknown='{}'", pojo.getType());
			}
			{
				final var simpleLabel = new Label("<b>" + propertyName + ":</b>");
				simpleLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
				simpleLabel.setPropertyFill(Vector2b.FALSE);
				simpleLabel.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
				simpleLabel.setPropertyGravity(Gravity.LEFT);
				this.sizerMenu.subWidgetAdd(simpleLabel);
			}
		}
	}
	
	public void setTestWidget(final Widget widget) {
		this.sizerMenu.subWidgetRemoveAll();
		if (this.isMetaWidget) {
			// Display message for meta widgets (composites without editable properties)
			final var metaLabel = new Label("<b>Meta Widget</b><br/><br/>"
					+ "<i>This is a composite test containing multiple widgets.<br/>"
					+ "Individual widget properties are not available for editing.</i>");
			metaLabel.setPropertyExpand(Vector2b.TRUE_FALSE);
			metaLabel.setPropertyFill(Vector2b.TRUE);
			metaLabel.setPropertyGravity(Gravity.TOP_LEFT);
			this.sizerMenu.subWidgetAdd(metaLabel);
		} else {
			try {
				final var modelPojo = new IntrospectionModelComplex(widget.getClass());

				connectAllSignals(widget, modelPojo);
				displayAllPropertyWithType(widget, modelPojo);
			} catch (final Exception e) {
				e.printStackTrace();
			}
		}
		this.sizerTestAreaHori.subWidgetRemoveAll();
		{
			final var simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.CHOCOLATE);
			simpleSpacer.setPropertyExpand(Vector2b.FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector2b.TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
			this.sizerTestAreaHori.subWidgetAdd(simpleSpacer);
		}
		this.testWidget = widget;
		this.sizerTestAreaHori.subWidgetAdd(this.testWidget);
		{
			final var simpleSpacer = new Spacer();
			simpleSpacer.setPropertyColor(Color.GREEN_YELLOW);
			simpleSpacer.setPropertyExpand(Vector2b.FALSE);
			simpleSpacer.setPropertyExpandIfFree(Vector2b.TRUE);
			simpleSpacer.setPropertyFill(Vector2b.TRUE);
			simpleSpacer.setPropertyMinSize(new Dimension2f(Vector2f.VALUE_16, Distance.PIXEL));
			this.sizerTestAreaHori.subWidgetAdd(simpleSpacer);
		}
		// update properties...
		// final Vector3b stateExpand = this.testWidget.getPropertyExpand();
		// this.buttonExpandX.setPropertyValue(stateExpand.x());
		// this.buttonExpandY.setPropertyValue(stateExpand.y());
		// final Vector3b stateFill = this.testWidget.getPropertyFill();
		// this.buttonFillX.setPropertyValue(stateFill.x());
		// this.buttonFillY.setPropertyValue(stateFill.y());
		
		// final Gravity gravity = this.testWidget.getPropertyGravity();
		// final Label gravLabel = (Label) (this.buttonGravity.getSubWidgets()[0]);
		// gravLabel.setPropertyValue(LABEL_GRAVITY + gravity.toString());
	}
	
}