package org.atriasoft.ewol.widget.meta;

import java.util.function.Supplier;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Connection;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.event.InputSystem;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.ewol.widget.Container;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.gale.key.KeyStatus;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * A transparent wrapper widget that shows a {@link Popover} when the user
 * hovers over the child widget for a configurable delay.
 *
 * <p>PopoverTrigger extends {@link Container} so it is invisible in the
 * widget tree — the child widget is laid out exactly as if PopoverTrigger
 * were not there. Events pass through to the child normally.</p>
 *
 * <p>Usage (programmatic):</p>
 * <pre>
 * PopoverTrigger.create(new Label("Hover me"))
 *     .popoverContent(new Label("Tooltip text"))
 *     .hoverDelay(0.2f)
 *     .position(PopoverPosition.TOP);
 * </pre>
 *
 * <p>Usage (XML):</p>
 * <pre>{@code
 * <PopoverTrigger hover-delay="0.2" position="AUTO">
 *     <Label>Hover me</Label>
 * </PopoverTrigger>
 * }</pre>
 */
public class PopoverTrigger extends Container {
	private static final Logger LOGGER = LoggerFactory.getLogger(PopoverTrigger.class);

	// Configuration — factory creates fresh content for each popover cycle
	private Supplier<Widget> popoverContentFactory;
	private PopoverPosition preferredPosition = PopoverPosition.AUTO;
	private float hoverDelaySeconds = 0.2f;
	private Color popoverColor;
	private Color popoverBorderColor;
	private boolean followMouseEnabled = false;

	// Hover state
	private boolean mouseHover = false;
	private float hoverElapsed = 0.0f;
	private Vector2f lastMousePos = Vector2f.ZERO;

	// Timer
	private Connection periodicConnection;

	// Active popover
	private Popover currentPopover;
	private Connection dismissConnection;

	public PopoverTrigger() {
	}

	public PopoverTrigger(final Widget child) {
		setSubWidget(child);
	}

	// ========================================================================
	// Properties (XML-bindable)
	// ========================================================================

	@JsonProperty("hover-delay")
	@JacksonXmlProperty(isAttribute = true, localName = "hover-delay")
	public float getHoverDelaySeconds() {
		return this.hoverDelaySeconds;
	}

	public void setHoverDelaySeconds(final float hoverDelaySeconds) {
		this.hoverDelaySeconds = hoverDelaySeconds;
	}

	@JsonProperty("position")
	@JacksonXmlProperty(isAttribute = true, localName = "position")
	public PopoverPosition getPreferredPosition() {
		return this.preferredPosition;
	}

	public void setPreferredPosition(final PopoverPosition position) {
		this.preferredPosition = position;
	}

	// ========================================================================
	// Event handling — observe hover, don't consume
	// ========================================================================

	/**
	 * Intercept events as they bubble up from the child widget.
	 * We observe hover state but never consume the event.
	 */
	@Override
	public boolean systemEventInput(final InputSystem event) {
		observeHoverEvent(event.event());
		return super.systemEventInput(event);
	}

	/**
	 * Observe mouse movement to detect hover enter/leave.
	 * Follows the same pattern as {@link org.atriasoft.ewol.widget.Button}.
	 */
	private void observeHoverEvent(final EventInput event) {
		final Vector2f relPos = relativePosition(event.pos());
		final boolean over = isInsideTrigger(relPos);

		// Handle cursor leave from a child widget.
		// The InputManager sends 'leave' when the mouse exits the bounds of the
		// *target* widget (a child Label/Button), not the trigger itself.
		// Only close if the mouse is truly outside both the trigger AND the popover.
		if (event.status() == KeyStatus.leave) {
			if (this.mouseHover) {
				// Still inside the trigger area? Keep the popover open.
				if (over) {
					return;
				}
				// Moving into the popover content area? Keep the popover open.
				if (this.currentPopover != null && isInsidePopoverHitBox(event.pos())) {
					LOGGER.debug("PopoverTrigger[{}]: mouse LEAVE trigger but inside popover", getId());
					return;
				}
				LOGGER.debug("PopoverTrigger[{}]: mouse LEAVE", getId());
				this.mouseHover = false;
				cancelHover();
			}
			return;
		}

		// Mouse movement (inputId == 0 means cursor movement without button)
		if (event.inputId() == 0) {
			this.lastMousePos = event.pos();
			if (over && !this.mouseHover) {
				LOGGER.info("PopoverTrigger[{}]: mouse ENTER, starting {}s timer", getId(), this.hoverDelaySeconds);
				this.mouseHover = true;
				startHoverTimer();
			} else if (!over && this.mouseHover) {
				LOGGER.debug("PopoverTrigger[{}]: mouse EXIT (no longer inside)", getId());
				this.mouseHover = false;
				cancelHover();
			} else if (over && this.followMouseEnabled && this.currentPopover != null) {
				// Follow-mouse mode: fast relocation without full relayout
				this.currentPopover.relocateToAnchor(event.pos());
			}
		}
	}

	/**
	 * Check if the position is inside the current popover's hit box.
	 */
	private boolean isInsidePopoverHitBox(final Vector2f pos) {
		if (this.currentPopover == null) {
			return false;
		}
		return this.currentPopover.isInsideHitBox(pos);
	}

	private boolean isInsideTrigger(final Vector2f relPos) {
		return relPos.x() >= 0 && relPos.y() >= 0
				&& relPos.x() <= this.size.x() && relPos.y() <= this.size.y();
	}

	// ========================================================================
	// Timer — periodic call for hover delay
	// ========================================================================

	private void startHoverTimer() {
		this.hoverElapsed = 0.0f;
		if (this.periodicConnection == null || !this.periodicConnection.isConnected()) {
			this.periodicConnection = EwolObject.getObjectManager().periodicCall
					.connect(this::onPeriodicCall);
		}
	}

	private void onPeriodicCall(final EventTime event) {
		this.hoverElapsed += event.getTimeDeltaCallSecond();
		if (this.hoverElapsed >= this.hoverDelaySeconds) {
			stopPeriodicCall();
			showPopover();
		}
	}

	private void stopPeriodicCall() {
		if (this.periodicConnection != null) {
			this.periodicConnection.close();
			this.periodicConnection = null;
		}
	}

	private void cancelHover() {
		stopPeriodicCall();
		closePopover();
	}

	// ========================================================================
	// Popover lifecycle
	// ========================================================================

	private void showPopover() {
		if (this.popoverContentFactory == null) {
			return;
		}
		if (this.currentPopover != null) {
			return; // already showing
		}

		final Popover popover = Popover.create()
				.content(this.popoverContentFactory.get())
				.position(this.preferredPosition)
				.closeOnOutside(false)
				.closeOnHoverLeave(false)
				.managed(true);

		// Anchor to mouse position or widget position
		if (this.followMouseEnabled) {
			popover.anchor(this.lastMousePos);
		} else {
			popover.anchor(getOrigin(), getSize());
		}

		if (this.popoverColor != null) {
			popover.color(this.popoverColor);
		}
		if (this.popoverBorderColor != null) {
			popover.borderColor(this.popoverBorderColor);
		}

		this.currentPopover = popover;
		this.dismissConnection = popover.signalDismissed.connect(this::onPopoverDismissed);
		popover.show();
		LOGGER.debug("Popover shown for trigger {}", getId());
	}

	private void closePopover() {
		if (this.currentPopover != null) {
			this.currentPopover.closePopup();
			// onPopoverDismissed will be called via signal
		}
	}

	private void onPopoverDismissed() {
		this.currentPopover = null;
		this.mouseHover = false;
		if (this.dismissConnection != null) {
			this.dismissConnection.close();
			this.dismissConnection = null;
		}
	}

	// ========================================================================
	// Factory methods and Fluent API
	// ========================================================================

	public static PopoverTrigger create() {
		return new PopoverTrigger();
	}

	public static PopoverTrigger create(final Widget child) {
		return new PopoverTrigger(child);
	}

	/**
	 * Set a factory that creates fresh popover content each time the popover is shown.
	 * A new widget is created for each show cycle to avoid stale internal state.
	 */
	public PopoverTrigger popoverContent(final Supplier<Widget> factory) {
		this.popoverContentFactory = factory;
		return this;
	}

	public PopoverTrigger hoverDelay(final float seconds) {
		this.hoverDelaySeconds = seconds;
		return this;
	}

	public PopoverTrigger position(final PopoverPosition position) {
		this.preferredPosition = position;
		return this;
	}

	public PopoverTrigger popoverColor(final Color color) {
		this.popoverColor = color;
		return this;
	}

	public PopoverTrigger popoverBorderColor(final Color color) {
		this.popoverBorderColor = color;
		return this;
	}

	/**
	 * Enable follow-mouse mode: the popover tracks the cursor position
	 * instead of anchoring on the widget.
	 */
	public PopoverTrigger followMouse(final boolean follow) {
		this.followMouseEnabled = follow;
		return this;
	}
}
