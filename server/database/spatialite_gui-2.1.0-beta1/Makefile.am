ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = spatialite_gui

AM_CPPFLAGS = @CFLAGS@ @CPPFLAGS@ \
	@PG_CFLAGS@ @LIBCURL_CFLAGS@
	
AM_CPPFLAGS += -I$(top_srcdir)

spatialite_gui_SOURCES = Classdef.h BlobExplorer.cpp \
	Dialogs.cpp DialogsGraph.cpp WmsDialog.cpp Exif.cpp \
	Main.cpp MalformedGeoms.cpp \
	Objects.cpp QueryView.cpp QueryViewComposer.cpp \
	ResultSetView.cpp Shapefiles.cpp TableTree.cpp \
	TextCsv.cpp Wfs.cpp Raster.cpp Styles.cpp \
	RasterSymbolizers.cpp VectorSymbolizers1.cpp \
	VectorSymbolizers2.cpp SqlFiltersComposer.cpp \
	MapPanel.cpp MapView.cpp LayerTree.cpp \
	QuickStylesVector.cpp QuickStylesTopology.cpp \
	QuickStylesRaster.cpp HtmlHelp.cpp Postgres.cpp \
	AuxCurl.h AuxCurl.cpp PrinterPlotter.cpp \
	MapConfig.cpp ExportXLSX.cpp

LDADD = @WX_LIBS@ @LIBSPATIALITE_LIBS@ \
	@LIBRASTERLITE2_LIBS@ @LIBFREEXL_LIBS@ \
	@LIBVIRTUALPG_LIBS@ @LIBXML2_LIBS@ \
	@LIBCURL_LIBS@ @PG_LDFLAGS@ @PG_LIB@ -lgeos_c -lz

EXTRA_DIST = Makefile-static-mingw32 \
	Makefile-static-mingw64 \
    helpgen/helpgen.c \
    helpgen/READ_ME.txt \
    indent_me

AUTOMAKE_OPTIONS = dist-zip foreign

SUBDIRS = icons win_resource mac_resource gnome_resource

