package org.atriasoft.ewol.internal;

import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.FileSystem;
import java.nio.file.FileSystemNotFoundException;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Collections;
import java.util.stream.Stream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class LoadPackageStream {
	private static final Logger LOGGER = LoggerFactory.getLogger(LoadPackageStream.class);
	
	public static byte[] getAllData(final String resourceName) {
		LOGGER.trace("Load resource: '/resources{}'", resourceName);
		final InputStream out = LoadPackageStream.class.getResourceAsStream("/resources" + resourceName);
		if (out == null) {
			LOGGER.error("Can not load resource: '{}'", resourceName);
			for (final Path elem : LoadPackageStream.getResources(LoadPackageStream.class.getResource("/resources"))
					.toArray(Path[]::new)) {
				LOGGER.warn("  - '{}'", elem);
			}
			return null;
		}
		byte[] data = null;
		try {
			data = out.readAllBytes();
		} catch (final IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return data;
	}

	public static Stream<Path> getResources(final URL element) {
		try {
			final URI uri = element.toURI();
			FileSystem fs;
			Path path;
			if (uri.getScheme().contentEquals("jar")) {
				try {
					fs = FileSystems.getFileSystem(uri);
				} catch (final FileSystemNotFoundException e) {
					fs = FileSystems.newFileSystem(uri, Collections.<String, String> emptyMap());
				}
				String pathInJar = "/";
				final String tmpPath = element.getPath();
				final int idSeparate = tmpPath.indexOf('!');
				if (idSeparate != -1) {
					pathInJar = tmpPath.substring(idSeparate + 1);
					while (pathInJar.startsWith("/")) {
						pathInJar = pathInJar.substring(1);
					}
				}
				path = fs.getPath(pathInJar);
			} else {
				fs = FileSystems.getDefault();
				path = Paths.get(uri);
			}
			return Files.walk(path, 1);
		} catch (URISyntaxException | IOException e) {
			e.printStackTrace();
			return Stream.of();
		}
	}

	public static InputStream getStream(final String resourceName) {
		LOGGER.trace("Load resource: '/resources{}'", resourceName);
		final InputStream out = LoadPackageStream.class.getResourceAsStream("/resources" + resourceName);
		if (out == null) {
			LOGGER.error("Can not load resource: '{}'", resourceName);
			for (final Path elem : LoadPackageStream.getResources(LoadPackageStream.class.getResource("/resources"))
					.toArray(Path[]::new)) {
				LOGGER.warn("  - '{}'", elem);
			}
		}
		return out;
	}

	private LoadPackageStream() {}

}
