/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.attribute.PosixFileAttributes;
import java.nio.file.attribute.PosixFilePermission;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.Set;
import java.util.function.Consumer;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.compositing.CompositingText;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * File system browser as a tree view.
 * Wraps TreeView with TreeNode&lt;Path&gt; nodes.
 * Lazily loads directory contents on expand.
 */
public class TreeFileSystem extends TreeView {
	private static final Logger LOGGER =
			LoggerFactory.getLogger(TreeFileSystem.class);

	private static final Color FOLDER_ICON_COLOR = new Color(0xE8, 0xB8, 0x30, 0xFF);
	private static final float ICON_SIZE = 12.0f;
	private static final float ICON_MARGIN = 3.0f;

	// Signals (file-specific)
	public final Signal<Path> signalFileSelect = new Signal<>();
	public final Signal<Path> signalFolderSelect = new Signal<>();

	// Properties
	private Path rootPath;
	private boolean showFiles = true;
	private boolean showFolders = true;
	private boolean showHidden = false;
	private String filter = "^.*$";

	// Connection storage
	private final List<Connection> internalConnections = new ArrayList<>();

	public TreeFileSystem() {
		// Column 0: Name (tree column) — custom renderer with folder icon
		addColumn(new TreeColumn("Name", 300.0f,
				TreeFileSystem::renderNameCell));

		// Column 1: Size
		addColumn(new TreeColumn("Size", 100.0f,
				TreeFileSystem::renderSizeCell));

		// Column 2: Modified
		addColumn(new TreeColumn("Modified", 150.0f,
				TreeFileSystem::renderModifiedCell));

		// Column 3: Type
		addColumn(new TreeColumn("Type", 80.0f,
				TreeFileSystem::renderTypeCell));

		// Column 4: Permissions (rwxrwxrwx)
		addColumn(new TreeColumn("Permissions", 110.0f,
				TreeFileSystem::renderPermissionsCell));

		// Column 5: Owner
		addColumn(new TreeColumn("Owner", 90.0f,
				TreeFileSystem::renderOwnerCell));

		// Column 6: Group
		addColumn(new TreeColumn("Group", 90.0f,
				TreeFileSystem::renderGroupCell));

		setShowRoot(true);
		setShowHeaders(true);

		// Connect to expand signal for lazy loading
		this.internalConnections.add(
				this.signalExpand.connect(
						(final TreeNode<?> node) -> onNodeExpanded(node)));

		// Connect to select signal for file/folder distinction
		this.internalConnections.add(
				this.signalSelect.connect(
						(final TreeNode<?> node) -> onNodeSelected(node)));
	}

	// ========================================================================
	// Lazy loading
	// ========================================================================

	@SuppressWarnings("unchecked")
	private void onNodeExpanded(final TreeNode<?> rawNode) {
		final TreeNode<Path> node = (TreeNode<Path>) rawNode;
		// Only load if children are the placeholder (single child with null data)
		if (node.getChildren().size() == 1
				&& node.getChildren().get(0).getData() == null) {
			loadChildren(node);
		}
	}

	private void loadChildren(final TreeNode<Path> parentNode) {
		final Path dir = parentNode.getData();
		if (dir == null || !Files.isDirectory(dir)) {
			return;
		}

		final List<TreeNode<Path>> childNodes = new ArrayList<>();
		try (DirectoryStream<Path> stream = Files.newDirectoryStream(dir)) {
			for (final Path path : stream) {
				if (!this.showHidden && isHidden(path)) {
					continue;
				}
				if (Files.isDirectory(path)) {
					if (!this.showFolders) {
						continue;
					}
					final TreeNode<Path> dirNode = new TreeNode<>(path,
							path.getFileName().toString(), false);
					// Placeholder child to show the expand chevron
					dirNode.addChild(new TreeNode<>(null, "Loading...",
							true));
					childNodes.add(dirNode);
				} else if (this.showFiles) {
					final String fileName = path.getFileName().toString();
					if (fileName.matches(this.filter)) {
						final TreeNode<Path> fileNode = new TreeNode<>(path,
								fileName, true);
						childNodes.add(fileNode);
					}
				}
			}
		} catch (final IOException ex) {
			LOGGER.error("Failed to list directory: {}", dir, ex);
		}

		// Sort: directories first, then alphabetically
		childNodes.sort(Comparator
				.<TreeNode<Path>, Boolean>comparing(
						n -> !Files.isDirectory(n.getData()))
				.thenComparing(TreeNode::getLabel,
						String.CASE_INSENSITIVE_ORDER));

		// Clear placeholder and add real children
		parentNode.removeAllChildren();
		for (final TreeNode<Path> child : childNodes) {
			parentNode.addChild(child);
		}

		markToRedraw();
	}

	private static boolean isHidden(final Path path) {
		try {
			return Files.isHidden(path);
		} catch (final IOException ex) {
			return false;
		}
	}

	// ========================================================================
	// Selection signals
	// ========================================================================

	@SuppressWarnings("unchecked")
	private void onNodeSelected(final TreeNode<?> rawNode) {
		final TreeNode<Path> node = (TreeNode<Path>) rawNode;
		final Path path = node.getData();
		if (path == null) {
			return;
		}
		if (Files.isDirectory(path)) {
			this.signalFolderSelect.emit(path);
		} else {
			this.signalFileSelect.emit(path);
		}
	}

	// ========================================================================
	// Static cell renderers
	// ========================================================================

	/**
	 * Name column renderer with yellow folder icon for directories.
	 */
	private static void renderNameCell(final CompositingDrawing gc,
			final CompositingText text, final TreeNode<?> node,
			final Vector2f pos, final Vector2f size,
			final boolean selected) {
		@SuppressWarnings("unchecked")
		final TreeNode<Path> pathNode = (TreeNode<Path>) node;
		final Path path = pathNode.getData();

		float textOffsetX = 0;
		if (path != null && Files.isDirectory(path)) {
			// Draw yellow folder icon
			drawFolderIcon(gc, pos.x(), pos.y(), size.y());
			textOffsetX = ICON_SIZE + ICON_MARGIN * 2;
		}

		text.setPos(new Vector2f(pos.x() + textOffsetX, pos.y()));
		text.print(node.getLabel());
	}

	/**
	 * Draw a simple folder icon (rectangle with tab) in yellow.
	 */
	private static void drawFolderIcon(final CompositingDrawing gc,
			final float x, final float y, final float cellHeight) {
		gc.setColor(FOLDER_ICON_COLOR);

		final float iconX = x + ICON_MARGIN;
		final float centerY = y + cellHeight / 2.0f;
		final float halfH = ICON_SIZE * 0.4f;
		final float iconW = ICON_SIZE;

		// Main folder body
		final float bodyBottom = centerY - halfH;
		final float bodyTop = centerY + halfH * 0.7f;
		gc.setPos(new Vector2f(iconX, bodyBottom));
		gc.rectangleWidth(new Vector2f(iconW, bodyTop - bodyBottom));

		// Folder tab (small rectangle on top-left)
		final float tabWidth = iconW * 0.4f;
		final float tabHeight = halfH * 0.35f;
		gc.setPos(new Vector2f(iconX, bodyTop));
		gc.rectangleWidth(new Vector2f(tabWidth, tabHeight));
	}

	private static void renderSizeCell(final CompositingDrawing gc,
			final CompositingText text, final TreeNode<?> node,
			final Vector2f pos, final Vector2f size,
			final boolean selected) {
		@SuppressWarnings("unchecked")
		final TreeNode<Path> pathNode = (TreeNode<Path>) node;
		final Path path = pathNode.getData();
		if (path == null || Files.isDirectory(path)) {
			return;
		}
		try {
			final long fileSize = Files.size(path);
			text.setPos(pos);
			text.print(formatFileSize(fileSize));
		} catch (final IOException ex) {
			// silently ignore
		}
	}

	private static void renderModifiedCell(final CompositingDrawing gc,
			final CompositingText text, final TreeNode<?> node,
			final Vector2f pos, final Vector2f size,
			final boolean selected) {
		@SuppressWarnings("unchecked")
		final TreeNode<Path> pathNode = (TreeNode<Path>) node;
		final Path path = pathNode.getData();
		if (path == null) {
			return;
		}
		try {
			final long lastModified = Files.getLastModifiedTime(path)
					.toMillis();
			final SimpleDateFormat sdf = new SimpleDateFormat(
					"yyyy-MM-dd HH:mm");
			text.setPos(pos);
			text.print(sdf.format(new Date(lastModified)));
		} catch (final IOException ex) {
			// silently ignore
		}
	}

	private static void renderTypeCell(final CompositingDrawing gc,
			final CompositingText text, final TreeNode<?> node,
			final Vector2f pos, final Vector2f size,
			final boolean selected) {
		@SuppressWarnings("unchecked")
		final TreeNode<Path> pathNode = (TreeNode<Path>) node;
		final Path path = pathNode.getData();
		if (path == null) {
			return;
		}
		text.setPos(pos);
		if (Files.isDirectory(path)) {
			text.print("Folder");
		} else {
			final String name = path.getFileName().toString();
			final int dotIndex = name.lastIndexOf('.');
			if (dotIndex >= 0 && dotIndex < name.length() - 1) {
				text.print(name.substring(dotIndex + 1).toUpperCase());
			} else {
				text.print("File");
			}
		}
	}

	private static void renderPermissionsCell(final CompositingDrawing gc,
			final CompositingText text, final TreeNode<?> node,
			final Vector2f pos, final Vector2f size,
			final boolean selected) {
		@SuppressWarnings("unchecked")
		final TreeNode<Path> pathNode = (TreeNode<Path>) node;
		final Path path = pathNode.getData();
		if (path == null) {
			return;
		}
		try {
			final PosixFileAttributes attrs =
					Files.readAttributes(path, PosixFileAttributes.class);
			final Set<PosixFilePermission> perms = attrs.permissions();
			text.setPos(pos);
			text.print(formatPermissions(perms));
		} catch (final IOException | UnsupportedOperationException ex) {
			// Not a POSIX filesystem or cannot read — silently ignore
		}
	}

	private static void renderOwnerCell(final CompositingDrawing gc,
			final CompositingText text, final TreeNode<?> node,
			final Vector2f pos, final Vector2f size,
			final boolean selected) {
		@SuppressWarnings("unchecked")
		final TreeNode<Path> pathNode = (TreeNode<Path>) node;
		final Path path = pathNode.getData();
		if (path == null) {
			return;
		}
		try {
			final PosixFileAttributes attrs =
					Files.readAttributes(path, PosixFileAttributes.class);
			text.setPos(pos);
			text.print(attrs.owner().getName());
		} catch (final IOException | UnsupportedOperationException ex) {
			// silently ignore
		}
	}

	private static void renderGroupCell(final CompositingDrawing gc,
			final CompositingText text, final TreeNode<?> node,
			final Vector2f pos, final Vector2f size,
			final boolean selected) {
		@SuppressWarnings("unchecked")
		final TreeNode<Path> pathNode = (TreeNode<Path>) node;
		final Path path = pathNode.getData();
		if (path == null) {
			return;
		}
		try {
			final PosixFileAttributes attrs =
					Files.readAttributes(path, PosixFileAttributes.class);
			text.setPos(pos);
			text.print(attrs.group().getName());
		} catch (final IOException | UnsupportedOperationException ex) {
			// silently ignore
		}
	}

	private static String formatPermissions(
			final Set<PosixFilePermission> perms) {
		final StringBuilder sb = new StringBuilder(9);
		sb.append(perms.contains(PosixFilePermission.OWNER_READ) ? 'r' : '-');
		sb.append(perms.contains(PosixFilePermission.OWNER_WRITE) ? 'w' : '-');
		sb.append(perms.contains(PosixFilePermission.OWNER_EXECUTE)
				? 'x' : '-');
		sb.append(perms.contains(PosixFilePermission.GROUP_READ) ? 'r' : '-');
		sb.append(perms.contains(PosixFilePermission.GROUP_WRITE) ? 'w' : '-');
		sb.append(perms.contains(PosixFilePermission.GROUP_EXECUTE)
				? 'x' : '-');
		sb.append(perms.contains(PosixFilePermission.OTHERS_READ)
				? 'r' : '-');
		sb.append(perms.contains(PosixFilePermission.OTHERS_WRITE)
				? 'w' : '-');
		sb.append(perms.contains(PosixFilePermission.OTHERS_EXECUTE)
				? 'x' : '-');
		return sb.toString();
	}

	private static String formatFileSize(final long bytes) {
		if (bytes < 1024) {
			return bytes + " B";
		} else if (bytes < 1024 * 1024) {
			return String.format("%.1f KB", bytes / 1024.0);
		} else if (bytes < 1024L * 1024 * 1024) {
			return String.format("%.1f MB", bytes / (1024.0 * 1024.0));
		} else {
			return String.format("%.1f GB",
					bytes / (1024.0 * 1024.0 * 1024.0));
		}
	}

	// ========================================================================
	// Public API
	// ========================================================================

	public void setRootPath(final String pathStr) {
		this.rootPath = Paths.get(pathStr);
		final String displayName = this.rootPath.getFileName() != null
				? this.rootPath.getFileName().toString()
				: this.rootPath.toString();
		final TreeNode<Path> root = new TreeNode<>(this.rootPath,
				displayName, false);
		root.setExpanded(true);
		setRootNode(root);
		// Load first level only (subdirectories stay collapsed)
		loadChildren(root);
	}

	@JsonProperty("root-path")
	@JacksonXmlProperty(isAttribute = true, localName = "root-path")
	public String getRootPathString() {
		return this.rootPath != null ? this.rootPath.toString() : "";
	}

	public Path getRootPath() {
		return this.rootPath;
	}

	@JsonProperty("show-files")
	@JacksonXmlProperty(isAttribute = true, localName = "show-files")
	public boolean isShowFiles() {
		return this.showFiles;
	}

	public void setShowFiles(final boolean show) {
		if (this.showFiles == show) {
			return;
		}
		this.showFiles = show;
		reload();
	}

	@JsonProperty("show-folders")
	@JacksonXmlProperty(isAttribute = true, localName = "show-folders")
	public boolean isShowFolders() {
		return this.showFolders;
	}

	public void setShowFolders(final boolean show) {
		if (this.showFolders == show) {
			return;
		}
		this.showFolders = show;
		reload();
	}

	@JsonProperty("show-hidden")
	@JacksonXmlProperty(isAttribute = true, localName = "show-hidden")
	public boolean isShowHidden() {
		return this.showHidden;
	}

	public void setShowHidden(final boolean show) {
		if (this.showHidden == show) {
			return;
		}
		this.showHidden = show;
		reload();
	}

	@JsonProperty("filter")
	@JacksonXmlProperty(isAttribute = true, localName = "filter")
	public String getFilter() {
		return this.filter;
	}

	public void setFilter(final String regex) {
		if (this.filter.equals(regex)) {
			return;
		}
		this.filter = regex;
		reload();
	}

	/**
	 * Reload the tree from the current root path with current filters.
	 */
	private void reload() {
		if (this.rootPath != null) {
			setRootPath(this.rootPath.toString());
		}
	}

	// ========================================================================
	// Factory and Fluent API
	// ========================================================================

	public static TreeFileSystem create() {
		return new TreeFileSystem();
	}

	public static TreeFileSystem create(final String rootPath) {
		final TreeFileSystem tfs = new TreeFileSystem();
		tfs.setRootPath(rootPath);
		return tfs;
	}

	public TreeFileSystem rootPath(final String path) {
		setRootPath(path);
		return this;
	}

	public TreeFileSystem showFiles(final boolean show) {
		setShowFiles(show);
		return this;
	}

	public TreeFileSystem showFolders(final boolean show) {
		setShowFolders(show);
		return this;
	}

	public TreeFileSystem showHidden(final boolean show) {
		setShowHidden(show);
		return this;
	}

	public TreeFileSystem filter(final String regex) {
		setFilter(regex);
		return this;
	}

	public TreeFileSystem onFileSelect(final Consumer<Path> callback) {
		this.internalConnections.add(
				this.signalFileSelect.connect(callback));
		return this;
	}

	public TreeFileSystem onFolderSelect(final Consumer<Path> callback) {
		this.internalConnections.add(
				this.signalFolderSelect.connect(callback));
		return this;
	}
}
