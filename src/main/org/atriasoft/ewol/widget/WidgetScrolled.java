package org.atriasoft.ewol.widget;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.HighSpeedMode;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Widget to integrate a scrool bar in a widget. This is not a stadalone widget.
 */
public class WidgetScrolled extends Widget {
	private static final Logger LOGGER = LoggerFactory.getLogger(WidgetScrolled.class);
	
	public enum ScrollingMode {
		scroolModeNormal, //!< No Zoom , can UP and down, left and right
		scroolModeCenter, //!< Zoom enable, no move left and right
		scroolModeGame, //!< Zoom enable, no move left and right
	}

	public static final int CALCULATE_SIMULTANEOUS_FINGER = 5;
	protected Uri propertyShapeVert = new Uri("THEME", "shape/WidgetScrolled.json", "ewol");
	protected Uri propertyShapeHori = new Uri("THEME", "shape/WidgetScrolled.json", "ewol");

	protected CompositingDrawing compositingScrollbar = new CompositingGC();
	protected static final float SCROLLBAR_WIDTH = 8.0f;
	protected static final Color SCROLLBAR_BG_COLOR = new Color(0x40, 0x40, 0x40, 0x80);
	protected static final Color SCROLLBAR_FG_COLOR = new Color(0x80, 0x80, 0x80, 0xC0);
	protected Vector2f originScrooled = Vector2f.ZERO;
	protected Vector2f maxSize = Vector2f.ZERO;
	protected Vector2f limitScrolling = Vector2f.ZERO;
	private ScrollingMode scroollingMode = ScrollingMode.scroolModeNormal;
	private float pixelScrolling = 20;
	private Vector2f highSpeedStartPos;
	private HighSpeedMode highSpeedMode = HighSpeedMode.speedModeDisable;
	private int highSpeedButton = -1;
	private KeyType highSpeedType = KeyType.unknow;
	// finger section:
	private boolean singleFingerMode = true;
	private final boolean[] fingerPresent = { false, false, false, false, false };
	private boolean fingerScoolActivated = false;
	private final Vector2f[] fingerMoveStartPos = new Vector2f[CALCULATE_SIMULTANEOUS_FINGER];

	/**
	 * Scroll Widget main constructor to be inherited from an other widget (this is not a stand-alone widget)
	 * @param _shaperName Shaper name if the scrolled widget.
	 */
	public WidgetScrolled() {
		onChangePropertyShapeVert();
		onChangePropertyShapeHori();
	}

	@JsonProperty("shape-hori")
	@JacksonXmlProperty(isAttribute = true, localName = "shape-hori")
	public Uri getPropertyShapeHori() {
		return this.propertyShapeHori;
	}

	@JsonProperty("shape-vert")
	@JacksonXmlProperty(isAttribute = true, localName = "shape-vert")
	public Uri getPropertyShapeVert() {
		return this.propertyShapeVert;
	}

	/**
	 * Get the single finger capabilities
	 * @return true The single finger mode is active
	 * @return false The To finger mode is active
	 */
	public boolean getSingleFinger() {
		return this.singleFingerMode;
	}

	protected void onChangePropertyShapeHori() {
		//		if (this.shaperH == null) {
		//			this.shaperH = new GuiShape(this.propertyShapeHori);
		//		} else {
		//			this.shaperH.setSource(this.propertyShapeHori);
		//		}
		markToRedraw();
	}

	protected void onChangePropertyShapeVert() {
		//		if (this.shaperV == null) {
		//			this.shaperV = new GuiShape(this.propertyShapeVert);
		//		} else {
		//			this.shaperV.setSource(this.propertyShapeVert);
		//		}
		markToRedraw();
	}

	@Override
	protected void onDraw() {
		this.compositingScrollbar.draw();
	}

	@Override
	public boolean onEventInput(final EventInput event) {
		LOGGER.trace("event: {}", event);
		Vector2f relativePos = relativePosition(new Vector2f(event.pos().x(), event.pos().y()));
		// Correction due to the open Gl insertion ...
		relativePos = relativePos.withY(this.size.y() - relativePos.y());
		final Padding paddingV = new Padding(SCROLLBAR_WIDTH, SCROLLBAR_WIDTH, SCROLLBAR_WIDTH, SCROLLBAR_WIDTH);
		final Padding paddingH = new Padding(SCROLLBAR_WIDTH, SCROLLBAR_WIDTH, SCROLLBAR_WIDTH, SCROLLBAR_WIDTH);
		if (this.scroollingMode == ScrollingMode.scroolModeNormal) {
			if (event.type() == KeyType.mouse
					&& (this.highSpeedType == KeyType.unknow || this.highSpeedType == KeyType.mouse)) {
				if (event.inputId() == 1 && event.status() == KeyStatus.down) {
					// check if selected the scrolling position with the scrolling bar ...
					if (relativePos.x() >= (this.size.x() - paddingV.x())) {
						if (this.size.y() < this.maxSize.y() || this.originScrooled.y() != 0) {
							this.highSpeedMode = HighSpeedMode.speedModeEnableVertical;
							this.highSpeedType = KeyType.mouse;
							this.highSpeedStartPos = new Vector2f(
									relativePos.x(),
									this.originScrooled.y() / this.maxSize.y() * (this.size.y() - paddingV.y()));
							this.highSpeedButton = 1;
							// force direct scrolling in this case
							this.originScrooled = this.originScrooled.withY((int) (this.maxSize.y()
									* (relativePos.y() - paddingV.bottom()) / (this.size.y() - paddingV.bottom() * 2)));
							this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(),
									(this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
							markToRedraw();
							return true;
						}
					} else if (relativePos.y() >= (this.size.y() - paddingH.y())) {
						if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0) {
							this.highSpeedMode = HighSpeedMode.speedModeEnableHorizontal;
							this.highSpeedType = KeyType.mouse;
							this.highSpeedStartPos = new Vector2f(
									this.originScrooled.x() / this.maxSize.x() * (this.size.x() - paddingH.x()),
									relativePos.y());
							this.highSpeedButton = 1;
							// force direct scrolling in this case
							this.originScrooled = this.originScrooled.withX((int) (this.maxSize.x()
									* (relativePos.x() - paddingH.left()) / (this.size.x() - paddingH.left() * 2)));
							this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(),
									(this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
							markToRedraw();
							return true;
						}
					}
					return false;
				} else if (event.inputId() == 5 && event.status() == KeyStatus.up) {
					if (event.specialKey().getCtrl()) {
						changeZoom(1);
					} else if (event.specialKey().getShift()) {
						// Shift + scroll up = horizontal scroll left
						if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0
								|| this.size.x() * this.limitScrolling.x() < this.maxSize.x()) {
							this.originScrooled = this.originScrooled.withX(this.originScrooled.x() - this.pixelScrolling);
							this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(),
									(this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
							markToRedraw();
							return true;
						}
					} else if (this.size.y() < this.maxSize.y() || this.originScrooled.y() != 0
							|| this.size.y() * this.limitScrolling.y() < this.maxSize.y()) {
						this.originScrooled = this.originScrooled.withY(this.originScrooled.y() - this.pixelScrolling);
						this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(),
								(this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
						markToRedraw();
						return true;
					}
				} else if (event.inputId() == 4 && event.status() == KeyStatus.up) {
					if (event.specialKey().getCtrl()) {
						changeZoom(-1);
					} else if (event.specialKey().getShift()) {
						// Shift + scroll down = horizontal scroll right
						if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0
								|| this.size.x() * this.limitScrolling.x() < this.maxSize.x()) {
							this.originScrooled = this.originScrooled.withX(this.originScrooled.x() + this.pixelScrolling);
							this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(),
									(this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
							markToRedraw();
							return true;
						}
					} else if (this.size.y() < this.maxSize.y() || this.originScrooled.y() != 0
							|| this.size.y() * this.limitScrolling.y() < this.maxSize.y()) {
						this.originScrooled = this.originScrooled.withY(this.originScrooled.y() + this.pixelScrolling);
						this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(),
								(this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
						markToRedraw();
						return true;
					}
				} else if (event.inputId() == 11 && event.status() == KeyStatus.up) {
					// Scrool Left
					if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0
							|| this.size.x() * this.limitScrolling.x() < this.maxSize.x()) {
						this.originScrooled = this.originScrooled.withX(this.originScrooled.x() - this.pixelScrolling);
						this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(),
								(this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
						markToRedraw();
						return true;
					}
				} else if (event.inputId() == 10 && event.status() == KeyStatus.up) {
					// Scrool Right
					if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0
							|| this.size.x() * this.limitScrolling.x() < this.maxSize.x()) {
						this.originScrooled = this.originScrooled.withX(this.originScrooled.x() + this.pixelScrolling);
						this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(),
								(this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
						markToRedraw();
						return true;
					}
				} else if (event.inputId() == 2) {
					/*
					if (true == ewol::isSetCtrl()) {
						if (KeyStatus.down == typeEvent) {
							float zoom = 1.0;
							setZoom(zoom);
						}
					} else */ {
						if (event.status() == KeyStatus.down) {
							this.highSpeedMode = HighSpeedMode.speedModeInit;
							this.highSpeedType = KeyType.mouse;
							this.highSpeedStartPos = new Vector2f(relativePos.x(), relativePos.y());
							this.highSpeedButton = 2;
							return true;
						}
					}
				} else if (this.highSpeedMode != HighSpeedMode.speedModeDisable && event.status() == KeyStatus.leave) {
					this.highSpeedMode = HighSpeedMode.speedModeDisable;
					this.highSpeedType = KeyType.unknow;
					markToRedraw();
					return true;
				}
				if (event.inputId() == this.highSpeedButton && this.highSpeedMode != HighSpeedMode.speedModeDisable) {
					if (event.status() == KeyStatus.upAfter) {
						this.highSpeedMode = HighSpeedMode.speedModeDisable;
						this.highSpeedType = KeyType.unknow;
						return false;
					} else if (this.highSpeedMode == HighSpeedMode.speedModeGrepEndEvent) {
						if (event.status() == KeyStatus.pressSingle) {
							this.highSpeedMode = HighSpeedMode.speedModeDisable;
							this.highSpeedType = KeyType.unknow;
							this.highSpeedButton = -1;
							markToRedraw();
						}
						return true;
					} else if (event.status() == KeyStatus.up) {
						return true;
					} else if (this.highSpeedMode == HighSpeedMode.speedModeInit && event.status() == KeyStatus.move) {
						// wait that the cursor move more than 10 px to enable it :
						if (FMath.abs(relativePos.x() - this.highSpeedStartPos.x()) > 10
								|| FMath.abs(relativePos.y() - this.highSpeedStartPos.y()) > 10) {
							// the scrolling can start :
							// select the direction :
							if (relativePos.x() == this.highSpeedStartPos.x()) {
								this.highSpeedMode = HighSpeedMode.speedModeEnableVertical;
							} else if (relativePos.y() == this.highSpeedStartPos.y()) {
								this.highSpeedMode = HighSpeedMode.speedModeEnableHorizontal;
							} else {
								final float coef = (relativePos.y() - this.highSpeedStartPos.y())
										/ (relativePos.x() - this.highSpeedStartPos.x());
								if (FMath.abs(coef) <= 1) {
									this.highSpeedMode = HighSpeedMode.speedModeEnableHorizontal;
								} else {
									this.highSpeedMode = HighSpeedMode.speedModeEnableVertical;
								}
							}
							if (this.highSpeedMode == HighSpeedMode.speedModeEnableHorizontal) {
								this.highSpeedStartPos = this.highSpeedStartPos.withX(
										this.originScrooled.x() / this.maxSize.x() * (this.size.x() - paddingV.x()));
							} else {
								this.highSpeedStartPos = this.highSpeedStartPos.withY(
										this.originScrooled.y() / this.maxSize.y() * (this.size.y() - paddingV.y()));
							}
							markToRedraw();
						}
						this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(),
								(this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
						return true;
					}
					if (this.highSpeedMode == HighSpeedMode.speedModeEnableHorizontal
							&& event.status() == KeyStatus.move) {
						this.originScrooled = this.originScrooled.withX((int) (this.maxSize.x()
								* (relativePos.x() - paddingH.left()) / (this.size.x() - paddingH.x())));
						this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(),
								(this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
						markToRedraw();
						return true;
					}
					if (this.highSpeedMode == HighSpeedMode.speedModeEnableVertical
							&& event.status() == KeyStatus.move) {
						this.originScrooled = this.originScrooled.withY((int) (this.maxSize.y()
								* (relativePos.y() - paddingV.bottom()) / (this.size.y() - paddingV.y())));
						this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(),
								(this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
						markToRedraw();
						return true;
					}
				}
			} else if (event.type() == KeyType.finger
					&& (this.highSpeedType == KeyType.unknow || this.highSpeedType == KeyType.finger)) {
				if (!this.singleFingerMode) {
					// ***********************
					// ** Two finger mode : **
					// ***********************
					if (event.inputId() >= 3) {
						return false;
					}
					final int idTable = event.inputId() - 1;
					if (event.status() == KeyStatus.down) {
						this.fingerPresent[idTable] = true;
					} else if (event.status() == KeyStatus.upAfter) {
						this.fingerPresent[idTable] = false;
					}
					if (!this.fingerScoolActivated) {
						this.fingerMoveStartPos[idTable] = new Vector2f(relativePos.x(), relativePos.y());
					}
					if (this.fingerPresent[0] && this.fingerPresent[1] && !this.fingerScoolActivated) {
						this.fingerScoolActivated = true;
						LOGGER.trace("SCROOL  == > START pos={}", this.fingerMoveStartPos);
					}
					if (this.fingerScoolActivated) {
						// 1: HighSpeedMode...
						// 2: remove all unneeded sub event ... ==> maybe a better methode ...
						if (event.status() == KeyStatus.move) {
							this.originScrooled = this.originScrooled.withX(this.originScrooled.x()
									- (relativePos.x() - this.fingerMoveStartPos[idTable].x()) * 0.5f);
							this.originScrooled = this.originScrooled.withY(this.originScrooled.y()
									- (relativePos.y() - this.fingerMoveStartPos[idTable].y()) * 0.5f);
							this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(),
									(this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
							this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(),
									(this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
							this.fingerMoveStartPos[idTable] = new Vector2f(relativePos.x(), relativePos.y());
							LOGGER.trace("SCROOL  == > MOVE this.originScrooled={} {} {}", this.originScrooled,
									relativePos, this.highSpeedStartPos);
							markToRedraw();
						}
						if (!this.fingerPresent[0] && !this.fingerPresent[1]) {
							if (event.status() == KeyStatus.upAfter) {
								// TODO : Reset event ...
								this.fingerScoolActivated = false;
								// WTF ??? _event.reset();
							}
						}
						return true;
					}
				} else // **************************
				// ** Single finger mode : **
				// **************************
				if (event.inputId() == 1) {
					LOGGER.trace("event 1: {}", event);
					if (event.status() == KeyStatus.down) {
						this.highSpeedMode = HighSpeedMode.speedModeInit;
						this.highSpeedType = KeyType.finger;
						this.highSpeedStartPos = new Vector2f(relativePos.x(), relativePos.y());
						LOGGER.trace("SCROOL  == > INIT");
						return true;
					} else if (event.status() == KeyStatus.upAfter) {
						this.highSpeedMode = HighSpeedMode.speedModeDisable;
						this.highSpeedType = KeyType.unknow;
						LOGGER.trace("SCROOL  == > DISABLE");
						markToRedraw();
						return true;
					} else if (this.highSpeedMode == HighSpeedMode.speedModeInit && event.status() == KeyStatus.move) {
						// wait that the cursor move more than 10 px to enable it :
						if (FMath.abs(relativePos.x() - this.highSpeedStartPos.x()) > 10
								|| FMath.abs(relativePos.y() - this.highSpeedStartPos.y()) > 10) {
							// the scrooling can start :
							// select the direction :
							this.highSpeedMode = HighSpeedMode.speedModeEnableFinger;
							LOGGER.debug("SCROOL  == > ENABLE");
							markToRedraw();
						}
						return true;
					} else if (this.highSpeedMode == HighSpeedMode.speedModeEnableFinger
							&& event.status() == KeyStatus.pressSingle) {
						// Keep all event in the range of moving
						return true;
					} else if (this.highSpeedMode == HighSpeedMode.speedModeEnableFinger
							&& event.status() == KeyStatus.pressDouble) {
						// Keep all event in the range of moving
						return true;
					}
					if (this.highSpeedMode == HighSpeedMode.speedModeEnableFinger && event.status() == KeyStatus.move) {
						//this.originScrooled.x = (int)(this.maxSize.x * x / this.size.x);
						this.originScrooled = this.originScrooled
								.withX(this.originScrooled.x() - (relativePos.x() - this.highSpeedStartPos.x()));
						this.originScrooled = this.originScrooled
								.withY(this.originScrooled.y() - (relativePos.y() - this.highSpeedStartPos.y()));
						this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(),
								(this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
						this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(),
								(this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
						this.highSpeedStartPos = new Vector2f(relativePos.x(), relativePos.y());
						LOGGER.trace("SCROOL  == > MOVE this.originScrooled={} {} {}", this.originScrooled, relativePos,
								this.highSpeedStartPos);
						markToRedraw();
						return true;
					}
				} else if (this.highSpeedMode == HighSpeedMode.speedModeDisable && event.status() == KeyStatus.leave) {
					this.highSpeedMode = HighSpeedMode.speedModeDisable;
					this.highSpeedType = KeyType.unknow;
					LOGGER.trace("SCROOL  == > DISABLE");
					markToRedraw();
					return true;
				}
			}
		} else if (this.scroollingMode == ScrollingMode.scroolModeCenter) {
			if (event.type() == KeyType.mouse) {
				float tmp1 = this.size.x() / this.maxSize.y();
				final float tmp2 = this.size.y() / this.maxSize.x();
				//LOGGER.info(" elements Zoom : " + tmp1 + " " + tmp2);
				tmp1 = FMath.min(tmp1, tmp2);
				if (event.inputId() == 4 && event.status() == KeyStatus.up) {
					this.zoom -= 0.1;
					if (tmp1 < 1.0) {
						this.zoom = FMath.max(tmp1, this.zoom);
					} else {
						this.zoom = FMath.max(1.0f, this.zoom);
					}
					markToRedraw();
					return true;
				} else if (event.inputId() == 5 && event.status() == KeyStatus.up) {
					this.zoom += 0.1;
					if (tmp1 > 1.0) {
						this.zoom = FMath.min(tmp1, this.zoom);
					} else {
						this.zoom = FMath.min(1.0f, this.zoom);
					}
					markToRedraw();
					return true;
				}
			}
		} else if (this.scroollingMode == ScrollingMode.scroolModeGame) {
			
		} else {
			LOGGER.error("Scrolling mode unknown: {}", this.scroollingMode);
		}
		return false;
	}

	@Override
	public void onRegenerateDisplay() {
		this.compositingScrollbar.clear();
		if (this.scroollingMode == ScrollingMode.scroolModeGame) {
			// nothing to do ...
			return;
		}
		if (this.maxSize.equals(Vector2f.ZERO)) {
			return;
		}
		// Draw vertical scrollbar if needed
		if (this.size.y() < this.maxSize.y() || this.originScrooled.y() != 0) {
			float lenScrollBar = this.size.y() * this.size.y() / this.maxSize.y();
			lenScrollBar = FMath.avg(20.0f, lenScrollBar, this.size.y());
			final float maxScroll = this.maxSize.y() - this.size.y() * this.limitScrolling.y();
			float originScrollBar = 0;
			if (maxScroll > 0) {
				originScrollBar = this.originScrooled.y() / maxScroll;
			}
			originScrollBar = FMath.avg(0.0f, originScrollBar, 1.0f);
			originScrollBar *= (this.size.y() - lenScrollBar);
			
			// Draw scrollbar background
			this.compositingScrollbar.setColor(SCROLLBAR_BG_COLOR);
			this.compositingScrollbar.setPos(new Vector2f(this.size.x() - SCROLLBAR_WIDTH, 0));
			this.compositingScrollbar.rectangleWidth(new Vector2f(SCROLLBAR_WIDTH, this.size.y()));
			
			// Draw scrollbar thumb
			this.compositingScrollbar.setColor(SCROLLBAR_FG_COLOR);
			this.compositingScrollbar.setPos(
					new Vector2f(this.size.x() - SCROLLBAR_WIDTH, this.size.y() - originScrollBar - lenScrollBar));
			this.compositingScrollbar.rectangleWidth(new Vector2f(SCROLLBAR_WIDTH, lenScrollBar));
		}
		// Draw horizontal scrollbar if needed (at bottom, Y=0 in OpenGL)
		if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0) {
			float lenScrollBar = this.size.x() * this.size.x() / this.maxSize.x();
			lenScrollBar = FMath.avg(20.0f, lenScrollBar, this.size.x() - SCROLLBAR_WIDTH);
			final float maxScroll = this.maxSize.x() - this.size.x() * this.limitScrolling.x();
			float originScrollBar = 0;
			if (maxScroll > 0) {
				originScrollBar = this.originScrooled.x() / maxScroll;
			}
			originScrollBar = FMath.avg(0.0f, originScrollBar, 1.0f);
			originScrollBar *= (this.size.x() - SCROLLBAR_WIDTH - lenScrollBar);

			// Draw scrollbar background
			this.compositingScrollbar.setColor(SCROLLBAR_BG_COLOR);
			this.compositingScrollbar.setPos(new Vector2f(0, 0));
			this.compositingScrollbar.rectangleWidth(new Vector2f(this.size.x() - SCROLLBAR_WIDTH, SCROLLBAR_WIDTH));

			// Draw scrollbar thumb
			this.compositingScrollbar.setColor(SCROLLBAR_FG_COLOR);
			this.compositingScrollbar.setPos(new Vector2f(originScrollBar, 0));
			this.compositingScrollbar.rectangleWidth(new Vector2f(lenScrollBar, SCROLLBAR_WIDTH));
		}
		this.compositingScrollbar.flush();
	}

	/**
	 * Reset the scoll of the subWidget
	 */
	public void resetScrollOrigin() {
		this.originScrooled = new Vector2f(0, 0);
	}

	/**
	 * Specify the mode of scrolling for this windows
	 * @param newMode the selected mode for the scrolling...
	 */
	protected void scroolingMode(final ScrollingMode newMode) {
		this.scroollingMode = newMode;
		if (this.scroollingMode == ScrollingMode.scroolModeGame) {
			// set the scene maximum size :
			this.maxSize = new Vector2f(FMath.max(this.size.x(), this.size.y()), this.maxSize.x());
			this.zoom = 1;
		}
	}

	/**
	 * set the scrolling limit when arriving at he end of the widget
	 * @param poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end ...
	 */
	protected void setLimitScrolling(float poucentageLimit) {
		poucentageLimit = FMath.avg(0.1f, poucentageLimit, 1.0f);
		this.limitScrolling = new Vector2f(poucentageLimit, poucentageLimit);
	}

	/**
	 * set the scrolling limit when arriving at he end of the widget
	 * @param poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end for axis specific...
	 */
	protected void setLimitScrolling(final Vector2f poucentageLimit) {
		this.limitScrolling = new Vector2f(FMath.avg(0.1f, poucentageLimit.x(), 1.0f),
				FMath.avg(0.1f, poucentageLimit.y(), 1.0f));
	}

	/**
	 * set the specific mawimum size of the widget
	 * @param localSize new Maximum size
	 */
	protected void setMaxSize(final Vector2f localSize) {
		this.maxSize = localSize;
	}

	public void setPropertyShapeHori(final Uri value) {
		if (this.propertyShapeHori.equals(value)) {
			return;
		}
		this.propertyShapeHori = value;
		onChangePropertyShapeHori();
	}

	public void setPropertyShapeVert(final Uri value) {
		if (this.propertyShapeVert.equals(value)) {
			return;
		}
		this.propertyShapeVert = value;
		onChangePropertyShapeVert();
	}

	/**
		 * Request a specific position for the scrolling of the current windows.
		 * @param borderWidth size of the border that requested the element might not to be
		 * @param currentPosition Position that is requested to view
		 * @param _center True if the position might be at the center of the widget
		 */
	protected void setScrollingPositionDynamic(final Vector2f borderWidth, final Vector2f currentPosition) {
		setScrollingPositionDynamic(borderWidth, currentPosition, false);
	}

	protected void setScrollingPositionDynamic(
			Vector2f borderWidth,
			final Vector2f currentPosition,
			final boolean center) {
		if (center) {
			borderWidth = new Vector2f(this.size.x() / 2 - borderWidth.x(), this.size.y() / 2 - borderWidth.y());
		}
		// check scrolling in X
		if (currentPosition.x() < (this.originScrooled.x() + borderWidth.x())) {
			this.originScrooled = this.originScrooled.withX(currentPosition.x() - borderWidth.x());
			this.originScrooled = this.originScrooled.withX(FMath.max(0.0f, this.originScrooled.x()));
		} else if (currentPosition.x() > (this.originScrooled.x() + this.size.x() - 2 * borderWidth.x())) {
			this.originScrooled = this.originScrooled.withX(currentPosition.x() - this.size.x() + 2 * borderWidth.x());
			this.originScrooled = this.originScrooled.withX(FMath.max(0.0f, this.originScrooled.x()));
		}
		// check scrolling in Y
		if (currentPosition.y() < (this.originScrooled.y() + borderWidth.y())) {
			this.originScrooled = this.originScrooled.withY(currentPosition.y() - borderWidth.y());
			this.originScrooled = this.originScrooled.withY(FMath.max(0.0f, this.originScrooled.y()));
		} else if (currentPosition.y() > (this.originScrooled.y() + this.size.y() - 2 * borderWidth.y())) {
			this.originScrooled = this.originScrooled.withY(currentPosition.y() - this.size.y() + 2 * borderWidth.y());
			this.originScrooled = this.originScrooled.withY(FMath.max(0.0f, this.originScrooled.y()));
		}
	}

	/**
	 * For mouse event when we have a scrolling UP and dows, specify the number of pixel that we scrooled
	 * @param nbPixel number of pixel scrolling
	 */
	protected void setScrollingSize(final float nbPixel) {
		this.pixelScrolling = nbPixel;
	}

	/**
	 * Set the single finger capabilities/
	 * @param status True if single inger mode, two otherwise/
	 */
	public void setSingleFinger(final boolean status) {
		if (this.singleFingerMode == status) {
			return;
		}
		this.singleFingerMode = status;
	}

	@Override
	public void systemDraw(final DrawProperty displayProp) {
		OpenGL.push();
		if (this.scroollingMode == ScrollingMode.scroolModeCenter) {
			// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
			OpenGL.setViewPort(this.origin, this.size);
			final Matrix4f tmpProjection = Matrix4f.createMatrixOrtho(-this.size.x() / 2, this.size.x() / 2,
					-this.size.y() / 2, this.size.y() / 2, -1, 1);
			final Matrix4f tmpScale = Matrix4f.createMatrixScale(new Vector2f(this.zoom, this.zoom));
			final Matrix4f tmpTranslate = Matrix4f
					.createMatrixTranslate(new Vector2f(-this.maxSize.x() / 2, -this.maxSize.y() / 2));
			final Matrix4f tmpMat = tmpProjection.multiply(tmpScale).multiply(tmpTranslate);
			// set internal matrix system :
			OpenGL.setMatrix(tmpMat);
			// Call the widget drawing methode
			onDraw();
		}
		if (this.scroollingMode == ScrollingMode.scroolModeGame) {
			// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
			OpenGL.setViewPort(this.origin, this.size);
			final Matrix4f tmpProjection = Matrix4f.createMatrixOrtho(-this.size.x() / 2, this.size.x() / 2,
					-this.size.y() / 2, this.size.y() / 2, -1, 1);
			final Matrix4f tmpTranslate = Matrix4f
					.createMatrixTranslate(new Vector2f(-this.maxSize.x() / 2, -this.maxSize.y() / 2));
			final Matrix4f tmpMat = tmpProjection.multiply(tmpTranslate);
			// set internal matrix system :
			OpenGL.setMatrix(tmpMat);
			// Call the widget drawing methode
			onDraw();
		} else {
			super.systemDraw(displayProp);
		}
		OpenGL.pop();
	}
}
