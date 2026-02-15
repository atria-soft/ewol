package org.atriasoft.ewol.widget.debug;

import java.io.IOException;
import java.lang.ref.WeakReference;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.stream.Stream;

import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.compositing.CompositingGC;
import org.atriasoft.ewol.compositing.CompositingText;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.event.InputSystem;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.ewol.widget.WidgetManager;
import org.atriasoft.ewol.widget.Windows;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Widget Inspector overlay — press F12 to toggle.
 *
 * <p>When enabled, the inspector intercepts all input events (via
 * {@link Windows#getWidgetAtPos}) and draws a highlight rectangle
 * around the hovered widget with a tooltip showing its class name
 * and source file location. Clicking opens the source in VS Code.</p>
 *
 * <p>Layer order: drawn ABOVE everything (after toasts) in
 * {@link Windows#systemDraw}.</p>
 */
public class WidgetInspector {
	private static final Logger LOGGER = LoggerFactory.getLogger(WidgetInspector.class);

	private static final Color HIGHLIGHT_BORDER = new Color(0xFF, 0x00, 0x00, 0xFF);
	private static final Color TOOLTIP_BG = new Color(0x20, 0x20, 0x20, 0xE0);
	private static final Color TOOLTIP_TEXT_COLOR = Color.WHITE;
	private static final float HIGHLIGHT_BORDER_WIDTH = 2.0f;
	private static final float TOOLTIP_PADDING = 6.0f;
	private static final int TOOLTIP_FONT_SIZE = 14;

	private boolean enabled = false;
	private WeakReference<Widget> hoveredWidget = null;
	private Vector2f windowSize = Vector2f.ZERO;

	private final CompositingGC highlightDraw = new CompositingGC();
	private final CompositingText textDraw = new CompositingText();

	private boolean needRedraw = false;

	/** Cache: class name -> resolved source file path */
	private final Map<String, Path> sourceFileCache = new ConcurrentHashMap<>();

	// -- State management --

	/**
	 * Toggle the inspector on/off.
	 */
	public void toggle() {
		this.enabled = !this.enabled;
		if (!this.enabled) {
			this.hoveredWidget = null;
		}
		this.needRedraw = true;
		markGlobalRedraw();
		LOGGER.info("Widget Inspector {}", this.enabled ? "ENABLED (F12 to disable)" : "DISABLED");
	}

	public boolean isEnabled() {
		return this.enabled;
	}

	public void onWindowChangeSize(final Vector2f size) {
		this.windowSize = size;
		this.needRedraw = true;
	}

	// -- Event handling (called from a proxy Widget created by Windows) --

	/**
	 * Handle an input event while the inspector is active.
	 * @param event the input event
	 * @param windows the current windows (for widget-at-pos lookup)
	 * @return true if the event was consumed
	 */
	public boolean onEventInput(final EventInput event, final Windows windows) {
		if (!this.enabled) {
			return false;
		}
		// Hover: find the real widget under the cursor
		if (event.inputId() == 0) {
			if (event.status() == KeyStatus.move || event.status() == KeyStatus.enter) {
				final Widget realWidget = windows.getWidgetAtPosForInspection(event.pos());
				updateHoveredWidget(realWidget);
			}
			return true;
		}
		// Click: open source in IDE
		if (event.inputId() == 1 && event.status() == KeyStatus.pressSingle) {
			final Widget target = this.hoveredWidget != null ? this.hoveredWidget.get() : null;
			if (target != null) {
				openInIde(target);
			}
			return true;
		}
		// Consume all other mouse events to prevent interaction with widgets below
		return true;
	}

	private void updateHoveredWidget(final Widget widget) {
		final Widget current = this.hoveredWidget != null ? this.hoveredWidget.get() : null;
		if (current != widget) {
			this.hoveredWidget = widget != null ? new WeakReference<>(widget) : null;
			this.needRedraw = true;
			markGlobalRedraw();
		}
	}

	// -- Drawing --

	public void onRegenerateDisplay() {
		if (!this.enabled || !this.needRedraw) {
			return;
		}
		this.needRedraw = false;

		this.highlightDraw.clear();
		this.textDraw.clear();

		final Widget target = this.hoveredWidget != null ? this.hoveredWidget.get() : null;
		if (target == null) {
			this.highlightDraw.flush();
			this.textDraw.flush();
			return;
		}

		final Vector2f widgetOrigin = target.getOrigin();
		final Vector2f widgetSize = target.getSize();

		// Draw highlight border
		this.highlightDraw.setPaintFillColor(HIGHLIGHT_BORDER);
		// Top border
		this.highlightDraw.addRectangle(widgetOrigin,
				new Vector2f(widgetSize.x(), HIGHLIGHT_BORDER_WIDTH));
		// Bottom border
		this.highlightDraw.addRectangle(
				new Vector2f(widgetOrigin.x(), widgetOrigin.y() + widgetSize.y() - HIGHLIGHT_BORDER_WIDTH),
				new Vector2f(widgetSize.x(), HIGHLIGHT_BORDER_WIDTH));
		// Left border
		this.highlightDraw.addRectangle(widgetOrigin,
				new Vector2f(HIGHLIGHT_BORDER_WIDTH, widgetSize.y()));
		// Right border
		this.highlightDraw.addRectangle(
				new Vector2f(widgetOrigin.x() + widgetSize.x() - HIGHLIGHT_BORDER_WIDTH, widgetOrigin.y()),
				new Vector2f(HIGHLIGHT_BORDER_WIDTH, widgetSize.y()));

		this.highlightDraw.flush();

		// Build tooltip text
		final String className = target.getClass().getSimpleName();
		final StackTraceElement source = target.getSourceLocation();
		final String sourceInfo;
		if (source != null) {
			sourceInfo = source.getFileName() + ":" + source.getLineNumber();
		} else {
			sourceInfo = "(framework)";
		}
		final String tooltipText = className + "\n" + sourceInfo;

		// Draw tooltip
		this.textDraw.reset();
		this.textDraw.setFontSize(TOOLTIP_FONT_SIZE);
		this.textDraw.setColor(TOOLTIP_TEXT_COLOR);
		this.textDraw.setDefaultColorFg(TOOLTIP_TEXT_COLOR);

		// Estimate tooltip dimensions
		final float tooltipHeight = TOOLTIP_FONT_SIZE * 2.5f + TOOLTIP_PADDING * 2;
		final float tooltipWidth = Math.max(className.length(), sourceInfo.length()) * TOOLTIP_FONT_SIZE * 0.6f + TOOLTIP_PADDING * 2;

		// Position tooltip: try above widget, then below, then clamp to window
		float tooltipY;
		if (widgetOrigin.y() + widgetSize.y() + tooltipHeight + 4 < this.windowSize.y()) {
			// Above widget (ewol Y goes up)
			tooltipY = widgetOrigin.y() + widgetSize.y() + 4;
		} else if (widgetOrigin.y() - tooltipHeight - 4 >= 0) {
			// Below widget
			tooltipY = widgetOrigin.y() - tooltipHeight - 4;
		} else {
			// Clamp to bottom of window
			tooltipY = 4;
		}

		// Clamp X so tooltip stays within window bounds
		float tooltipX = widgetOrigin.x();
		if (tooltipX + tooltipWidth > this.windowSize.x()) {
			tooltipX = Math.max(0, this.windowSize.x() - tooltipWidth);
		}

		this.textDraw.setPos(new Vector2f(tooltipX + TOOLTIP_PADDING, tooltipY + TOOLTIP_PADDING));
		this.textDraw.print(tooltipText);
		this.textDraw.flush();
	}

	/**
	 * Draw the inspector overlay. Called from {@link Windows#systemDraw}
	 * after all other layers.
	 */
	public void onDraw(final DrawProperty displayProp) {
		if (!this.enabled) {
			return;
		}
		// Set up full-window viewport (same as Windows does for itself)
		final Vector2i winSize = new Vector2i((int) this.windowSize.x(), (int) this.windowSize.y());
		OpenGL.setViewPort(Vector2f.ZERO, this.windowSize);
		OpenGL.push();
		final Matrix4f projection = Matrix4f.createMatrixOrtho(
				-this.windowSize.x() / 2, this.windowSize.x() / 2,
				-this.windowSize.y() / 2, this.windowSize.y() / 2,
				-500, 500);
		final Matrix4f translate = Matrix4f.createMatrixTranslate(
				new Vector3f(-this.windowSize.x() / 2, -this.windowSize.y() / 2, -1.0f));
		OpenGL.setMatrix(projection);
		OpenGL.setCameraMatrix(translate);

		this.highlightDraw.draw();
		this.textDraw.draw(false);

		OpenGL.pop();
	}

	// -- IDE integration --

	private void openInIde(final Widget widget) {
		final StackTraceElement source = widget.getSourceLocation();
		if (source == null) {
			LOGGER.warn("No source location for widget: {} ({})",
					widget.getClass().getSimpleName(), widget.getClass().getCanonicalName());
			return;
		}
		final Path sourceFile = resolveSourceFile(source);
		if (sourceFile == null) {
			LOGGER.warn("Could not find source file for: {}.{} ({}:{})",
					source.getClassName(), source.getMethodName(),
					source.getFileName(), source.getLineNumber());
			return;
		}
		final String gotoArg = sourceFile.toAbsolutePath() + ":" + source.getLineNumber();
		LOGGER.info("Opening in VS Code: {}", gotoArg);
		try {
			new ProcessBuilder("code", "--goto", gotoArg)
					.inheritIO()
					.start();
		} catch (final IOException e) {
			LOGGER.error("Failed to launch VS Code: {}", e.getMessage());
		}
	}

	private Path resolveSourceFile(final StackTraceElement source) {
		final String className = source.getClassName();
		final Path cached = this.sourceFileCache.get(className);
		if (cached != null) {
			return cached;
		}
		// Convert class name to relative path: org.example.Foo -> org/example/Foo.java
		final String relativePath = className.replace('.', '/') + ".java";
		// Handle inner classes: org/example/Foo$Bar.java -> org/example/Foo.java
		final String outerPath;
		final int dollarIdx = relativePath.indexOf('$');
		if (dollarIdx >= 0) {
			outerPath = relativePath.substring(0, dollarIdx) + ".java";
		} else {
			outerPath = relativePath;
		}

		// Search from user.dir and parent directories
		Path searchRoot = Paths.get(System.getProperty("user.dir"));
		for (int depth = 0; depth < 3 && searchRoot != null; depth++) {
			final Path found = findFileInTree(searchRoot, outerPath);
			if (found != null) {
				this.sourceFileCache.put(className, found);
				return found;
			}
			searchRoot = searchRoot.getParent();
		}
		return null;
	}

	private Path findFileInTree(final Path root, final String relativePath) {
		try (final Stream<Path> dirs = Files.walk(root, 6)) {
			return dirs
					.filter(Files::isDirectory)
					.filter(dir -> dir.getFileName().toString().equals("src")
							|| dir.getFileName().toString().equals("main")
							|| dir.getFileName().toString().equals("java"))
					.map(dir -> dir.resolve(relativePath))
					.filter(Files::isRegularFile)
					.findFirst()
					.orElse(null);
		} catch (final IOException e) {
			return null;
		}
	}

	private void markGlobalRedraw() {
		final WidgetManager wm = Ewol.getContext().getWidgetManager();
		if (wm != null) {
			wm.markDrawingIsNeeded();
		}
	}
}
