
EXTRA_DIST = spatialite-gui.png spatialite-gui.desktop icons

desktopdir = $(datadir)/applications
desktop_DATA = spatialite-gui.desktop

#
# all gnome icons have been kindly contributed by
# Bas Couwenberg <sebastic@debian.org>
#

ICON_SIZES = 8 16 22 24 32 36 40 42 48 64 72 80 96 128 192 256 512

install-data-local:
	@for size in $(ICON_SIZES); do \
		mkdir -p $(DESTDIR)$(datadir)/icons/hicolor/$${size}x$${size}/apps; \
		$(INSTALL_DATA) icons/hicolor/$${size}x$${size}/apps/spatialite-gui.png $(DESTDIR)$(datadir)/icons/hicolor/$${size}x$${size}/apps/spatialite-gui.png; \
	done
