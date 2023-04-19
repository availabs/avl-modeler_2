#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(spatialite_gui, 2.1.0-beta1, a.furieri@lqt.it)
AC_LANG(C)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE
AM_CONFIG_HEADER(config.h)

# supporting SPATIALITE_AMALGAMATION
AH_TEMPLATE([SPATIALITE_AMALGAMATION],
	    [must be defined when using libspatialite-amalgamation])
AH_TEMPLATE([OMIT_FREEXL],
            [Should be defined in order to disable FREEXL support.])
AH_TEMPLATE([ENABLE_LIBXML2],
            [Should be defined in order to enable LIBXML2 support.])
AH_TEMPLATE([ENABLE_MINIZIP],
            [Should be defined in order to enable MiniZIP support.])
AH_TEMPLATE([ENABLE_XLSXWRITER],
            [Should be defined in order to enable XlsxWriter support.])
AH_TEMPLATE([OMIT_LZ4],
            [Should be defined in order to disable LZ4 support.])
AH_TEMPLATE([OMIT_ZSTD],
            [Should be defined in order to disable ZSTD support.])
AH_TEMPLATE([OMIT_WEBP],
            [Should be defined in order to disable WebP support.])
AH_TEMPLATE([OMIT_OPENJPEG],
            [Should be defined in order to disable OpenJpeg support.])
AH_TEMPLATE([PROJ_NEW],
            [Should be defined in order to enable PROJ.6 support.])
AH_TEMPLATE([CURL_URL_GET],
            [Should be defined in order to enable full Curl support.])
AH_TEMPLATE([LIBPQ_DEFERRED],
            [Should be defined in order to enable LibPQ late binding.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_LOOKASIDE_USED],
            [depending on SQLite library version.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_LOOKASIDE_HIT],
            [depending on SQLite library version.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE],
            [depending on SQLite library version.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL],
            [depending on SQLite library version.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_CACHE_USED],
            [depending on SQLite library version.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_CACHE_HIT],
            [depending on SQLite library version.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_CACHE_MISS],
            [depending on SQLite library version.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_CACHE_WRITE],
            [depending on SQLite library version.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_SCHEMA_USED],
            [depending on SQLite library version.])
AH_TEMPLATE([HAVE_DECL_SQLITE_DBSTATUS_STMT_USED],
            [depending on SQLite library version.])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

AC_ARG_WITH([wxconfig],
      [AS_HELP_STRING([--with-wxconfig=FILE], [specify an alternative wx-config file])],
	[WXCONFIG="$withval"], [WXCONFIG=""])
if test "x$WXCONFIG" = "x"; then
      # WXCONFIG was not specified, so search within the current path
      AC_PATH_PROG([WXCONFIG], [wx-config])	
      # If we couldn't find wx-config, display an error
      if test "x$WXCONFIG" = "x"; then
              AC_MSG_ERROR([could not find wx-config within the current path. You may need to try re-running configure with a --with-wxconfig parameter.])
      fi
else
      # WXCONFIG was specified; display a message to the user
      if test "x$WXCONFIG" = "xyes"; then
              AC_MSG_ERROR([you must specify a parameter to --with-wxconfig, e.g. --with-wxconfig=/path/to/wx-config])
      else
              if test -f $WXCONFIG; then
                      AC_MSG_RESULT([Using user-specified wx-config file: $WXCONFIG])
              else
                      AC_MSG_ERROR([the user-specified wx-config file $WXCONFIG does not exist])
              fi     
      fi
fi
CXXFLAGS=`$WXCONFIG --cxxflags`
AM_CXXFLAGS=`$WXCONFIG --cxxflags`
WX_LIBS=`$WXCONFIG --libs std,aui`
AC_SUBST(WX_LIBS)

# Checks for header files.
AC_CHECK_HEADERS(stdlib.h,, [AC_MSG_ERROR([cannot find stdlib.h, bailing out])])
AC_CHECK_HEADERS(stdio.h,, [AC_MSG_ERROR([cannot find stdio.h, bailing out])])
AC_CHECK_HEADERS(string.h,, [AC_MSG_ERROR([cannot find string.h, bailing out])])
AC_CHECK_HEADERS(memory.h,, [AC_MSG_ERROR([cannot find memory.h, bailing out])])
AC_CHECK_HEADERS(math.h,, [AC_MSG_ERROR([cannot find math.h, bailing out])])
AC_CHECK_HEADERS(float.h,, [AC_MSG_ERROR([cannot find float.h, bailing out])])
AC_CHECK_HEADERS(fcntl.h,, [AC_MSG_ERROR([cannot find fcntl.h, bailing out])])
AC_CHECK_HEADERS(inttypes.h,, [AC_MSG_ERROR([cannot find inttypes.h, bailing out])])
AC_CHECK_HEADERS(stddef.h,, [AC_MSG_ERROR([cannot find stddef.h, bailing out])])
AC_CHECK_HEADERS(stdint.h,, [AC_MSG_ERROR([cannot find stdint.h, bailing out])])
AC_CHECK_HEADERS(sys/time.h,, [AC_MSG_ERROR([cannot find sys/time.h, bailing out])])
AC_CHECK_HEADERS(unistd.h,, [AC_MSG_ERROR([cannot find unistd.h, bailing out])])
AC_CHECK_HEADERS(sqlite3.h,, [AC_MSG_ERROR([cannot find sqlite3.h, bailing out])])
AC_CHECK_HEADERS(sqlite3ext.h,, [AC_MSG_ERROR([cannot find sqlite3ext.h, bailing out])])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MEMCMP
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([memset sqrt strcasecmp strerror strncasecmp strstr fdatasync ftruncate getcwd gettimeofday localtime_r memmove strerror])
 
# Checks for installed libraries
AC_CHECK_LIB(sqlite3,sqlite3_prepare_v2,,AC_MSG_ERROR(['libsqlite3' is required but it doesn't seem to be installed on this system.]),-lm)

AC_CHECK_HEADERS(proj.h, [proj_new_incl=1], 
   [AC_CHECK_HEADERS(proj_api.h, [proj_new_incl=0], [AC_MSG_ERROR([cannot find proj_api.h, bailing out])])])
AC_SEARCH_LIBS(proj_normalize_for_visualization, proj, [proj_new_lib=1],
   [AC_SEARCH_LIBS(pj_init_plus, proj, [proj_new_lib=0], [AC_MSG_ERROR(['libproj' is required but it doesn't seem to be installed on this system.])], [-lm -lpthread -lsqlite3])])
if test $proj_new_incl -eq 1 && test $proj_new_lib -eq 1; then
   AC_DEFINE(PROJ_NEW)
fi

#-----------------------------------------------------------------------
#   --with-geosconfig
#
AC_ARG_WITH([geosconfig],
      [AS_HELP_STRING([--with-geosconfig=FILE], [specify an alternative geos-config file])],
	[GEOSCONFIG="$withval"], [GEOSCONFIG=""])
if test "x$GEOSCONFIG" = "x"; then
      # GEOSCONFIG was not specified, so search within the current path
      AC_PATH_PROG([GEOSCONFIG], [geos-config])	
      # If we couldn't find geos-config, display an error
      if test "x$GEOSCONFIG" = "x"; then
              AC_MSG_ERROR([could not find geos-config within the current path. You may need to try re-running configure with a --with-geosconfig parameter.])
      fi
else
      # GEOSCONFIG was specified; display a message to the user
      if test "x$GEOSCONFIG" = "xyes"; then
              AC_MSG_ERROR([you must specify a parameter to --with-geosconfig, e.g. --with-geosconfig=/path/to/geos-config])
      else
              if test -f $GEOSCONFIG; then
                      AC_MSG_RESULT([Using user-specified geos-config file: $GEOSCONFIG])
              else
                      AC_MSG_ERROR([the user-specified geos-config file $GEOSCONFIG does not exist])
              fi     
      fi
fi
# Extract the linker and include flags
GEOS_LDFLAGS=`$GEOSCONFIG --ldflags`
GEOS_CPPFLAGS=-I`$GEOSCONFIG --includes`
AC_SUBST([GEOS_LDFLAGS])
AC_SUBST([GEOS_CPPFLAGS])	
# Ensure that we can parse geos_c.h
CPPFLAGS_SAVE="$CPPFLAGS"
CPPFLAGS="$GEOS_CPPFLAGS"
AC_CHECK_HEADERS([geos_c.h],, [AC_MSG_ERROR([could not find geos_c.h - you may need to specify the directory of a geos-config file using --with-geosconfig])])
CPPFLAGS="$CPPFLAGS_SAVE"	
# Ensure we can link against libgeos_c
LIBS_SAVE="$LIBS"
LIBS="$GEOS_LDFLAGS"
AC_SEARCH_LIBS(GEOSTopologyPreserveSimplify,geos_c,,AC_MSG_ERROR([could not find libgeos_c - you may need to specify the directory of a geos-config file using --with-geosconfig]))
LIBS="$LIBS_SAVE"
LIBS=$LIBS$GEOS_LDFLAGS' -lgeos_c'

#-----------------------------------------------------------------------
#   --enable-freexl
#
AC_ARG_ENABLE(freexl, [AS_HELP_STRING(
  [--enable-freexl], [enables FreeXL inclusion [default=yes]])],
  [], [enable_freexl=yes])
if test x"$enable_freexl" != "xno"; then
  PKG_CHECK_MODULES([LIBFREEXL], [freexl], , AC_MSG_ERROR(['libfreexl' is required but it doesn't seem to be installed on this system.]))
  AC_SUBST(LIBFREEXL_CFLAGS)
  AC_SUBST(LIBFREEXL_LIBS)
else
  AC_DEFINE(OMIT_FREEXL)
fi

#-----------------------------------------------------------------------
#   --enable-libxml2
#
AC_ARG_ENABLE(libxml2, [AS_HELP_STRING(
  [--enable-libxml2], [enables libxml2 inclusion [default=yes]])],
  [], [enable_libxml2=yes])
if test x"$enable_libxml2" != "xno"; then
  PKG_CHECK_MODULES([LIBXML2], [libxml-2.0], , AC_MSG_ERROR(['libxml2' is required but it doesn't seem to be installed on this system.]))
  AC_SUBST(LIBXML2_CFLAGS)
  AC_SUBST(LIBXML2_LIBS)
  AC_DEFINE(ENABLE_LIBXML2)
fi

#-----------------------------------------------------------------------
#   --enable-minizip
#
AC_ARG_ENABLE(minizip, [AS_HELP_STRING(
  [--enable-minizip], [enables MiniZIP inclusion [default=yes]])],
  [], [enable_minizip=yes])
if test x"$enable_minizip" != "xno"; then
  AC_CHECK_HEADERS(minizip/unzip.h,, [AC_MSG_ERROR([cannot find minizip/unzip.h, bailing out])])
  AC_SEARCH_LIBS(unzLocateFile, minizip,,AC_MSG_ERROR(['libminizip' is required but it doesn't seem to be installed on this system.]))
  AC_DEFINE(ENABLE_MINIZIP)
fi

#-----------------------------------------------------------------------
#   --enable-xlsxwriter
#
AC_ARG_ENABLE(xlsxwriter, [AS_HELP_STRING(
  [--enable-xlsxwriter], [enables XlsxWriter inclusion [default=yes]])],
  [], [enable_xlsxwriter=yes])
if test x"$enable_xlsxwriter" != "xno"; then
	AC_CHECK_HEADERS(xlsxwriter.h,, [AC_MSG_ERROR([cannot find xlsxwriter.h, bailing out])])
	AC_SEARCH_LIBS(worksheet_write_string, xlsxwriter,,AC_MSG_ERROR(['libxlsxwriter' is required but it doesn't seem to be installed on this system.]))
  AC_DEFINE(ENABLE_XLSXWRITER)
fi

#-----------------------------------------------------------------------
#   --enable-lz4
#
AC_ARG_ENABLE(lz4, [AS_HELP_STRING(
  [--enable-lz4], [enables LZ4 inclusion [default=yes]])],
  [], [enable_lz4=yes])
if test x"$enable_lz4" != "xno"; then
    PKG_CHECK_MODULES([LIBLZ4], [liblz4], , AC_MSG_ERROR(['liblz4' is required but it doesn't seems to be installed on this system.]))
    AC_SUBST(LIBLZ4_CFLAGS)
    AC_SUBST(LIBLZ4_LIBS)
else
  AC_DEFINE(OMIT_LZ4)
fi

#-----------------------------------------------------------------------
#   --enable-zstd
#
AC_ARG_ENABLE(zstd, [AS_HELP_STRING(
  [--enable-zstd], [enables ZSTD inclusion [default=yes]])],
  [], [enable_zstd=yes])
if test x"$enable_zstd" != "xno"; then
    PKG_CHECK_MODULES([LIBZSTD], [libzstd], , AC_MSG_ERROR(['libzstd' is required but it doesn't seems to be installed on this system.]))
    AC_SUBST(LIBZSTD_CFLAGS)
    AC_SUBST(LIBZSTD_LIBS)
else
  AC_DEFINE(OMIT_ZSTD)
fi
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#   --enable-openjpeg
#
AC_ARG_ENABLE(openjpeg, [AS_HELP_STRING(
  [--enable-openjpeg], [enables OpenJpeg inclusion [default=yes]])],
  [], [enable_openjpeg=yes])
if test x"$enable_openjpeg" != "xno"; then
  PKG_CHECK_MODULES([LIBOPENJP2], [libopenjp2], , AC_MSG_ERROR(['libopenjp2' (v2.1 or later) is required but it doesn't seem to be installed on this system.]))
  AC_SUBST(LIBOPENJP2_CFLAGS)
  AC_SUBST(LIBOPENJP2_LIBS)
else
  AC_DEFINE(OMIT_OPENJPEG)
fi

#-----------------------------------------------------------------------
#   --enable-webp
#
AC_ARG_ENABLE(webp, [AS_HELP_STRING(
  [--enable-webp], [enables WebP inclusion [default=yes]])],
  [], [enable_webp=yes])
if test x"$enable_webp" != "xno"; then
    PKG_CHECK_MODULES([LIBWEBP], [libwebp], , AC_MSG_ERROR(['libwebp' is required but it doesn't seems to be installed on this system.]))
    AC_SUBST(LIBWEBP_CFLAGS)
    AC_SUBST(LIBWEBP_LIBS)
else
  AC_DEFINE(OMIT_WEBP)
fi
#-----------------------------------------------------------------------

PKG_CHECK_MODULES([LIBLZMA], [liblzma], , AC_MSG_ERROR(['liblzma' is required but it doesn't seems to be installed on this system.]))
AC_SUBST(LIBLZMA_CFLAGS)
AC_SUBST(LIBLZMA_LIBS)
    
PKG_CHECK_MODULES([LIBSPATIALITE], [spatialite], , AC_MSG_ERROR(['libspatialite' is required but it doesn't seem to be installed on this system.]))
AC_SUBST(LIBSPATIALITE_CFLAGS)
# testing for libspatialite-amalgamation
if test "x$(pkg-config --cflags spatialite|grep "DSPATIALITE_AMALGAMATION=1")" != "x"; then
  AC_DEFINE(SPATIALITE_AMALGAMATION, 1)
fi
AC_SUBST(LIBSPATIALITE_LIBS)
AC_CHECK_LIB(spatialite,gaia_sql_proc_execute,,AC_MSG_ERROR(['libspatialite >= 5.0.0' is required but it doesn't seems to be installed on this system.]),-lm)
AC_CHECK_LIB(spatialite,gaiaTopoGeo_FromGeoTableNoFaceExtended,,AC_MSG_ERROR(['libspatialite (with Topology support enabled)' is required but it doesn't seems to be installed on this system.]),-lm)
AC_CHECK_LIB(spatialite,gaiaCreateControlPoints,,AC_MSG_ERROR(['libspatialite (with GCP support enebled)' is required but it doesn't seems to be installed on this system.]),-lm)

PKG_CHECK_MODULES([LIBRASTERLITE2], [rasterlite2], , AC_MSG_ERROR(['librasterlite2' is required but it doesn't seem to be installed on this system.]))
AC_SUBST(LIBRASTERLITE2_CFLAGS)
AC_SUBST(LIBRASTERLITE2_LIBS)
AC_CHECK_LIB(rasterlite2,rl2_drape_geometries,,AC_MSG_ERROR(['librasterlite2 >= 1.1.0' is required but it doesn't seems to be installed on this system.]),-lm)

PKG_CHECK_MODULES([LIBVIRTUALPG], [virtualpg], , AC_MSG_ERROR(['libvirtualpg' is required but it doesn't seem to be installed on this system.]))
AC_SUBST(LIBVIRTUALPG_CFLAGS)
AC_SUBST(LIBVIRTUALPG_LIBS)

PKG_CHECK_MODULES([LIBCURL], [libcurl], , AC_MSG_ERROR(['libcurl' is required but it doesn't seem to be installed on this system.]))
AC_SUBST(LIBCURL_CFLAGS)
AC_SUBST(LIBCURL_LIBS)
AC_SEARCH_LIBS(curl_url_get, curl, [curl_new_lib=1],
   [AC_SEARCH_LIBS(curl_easy_init, curl, [curl_new_lib=0], [AC_MSG_ERROR(['libcurlj' is required but it doesn't seem to be installed on this system.])], [-lm])])
if test $curl_new_lib -eq 1; then
   AC_DEFINE(CURL_URL_GET)
fi

#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#   --with-pgconfig
#
AC_ARG_WITH([pgconfig],
      [AS_HELP_STRING([--with-pgconfig=FILE], [specify an alternative pg_config file])],
         [PGCONFIG="$withval"], [PGCONFIG=""])
if test "x$PGCONFIG" = "x"; then
      # PGCONFIG was not specified, so search within the current path
      AC_PATH_PROG([PGCONFIG], [pg_config])	
      # If we couldn't find pg_config, display an error
      if test "x$PGCONFIG" = "x"; then
              AC_MSG_ERROR([could not find pg_config within the current path. You may need to try re-running configure with a --with-pgconfig parameter.])
      fi
else
      # PGCONFIG was specified; display a message to the user
      if test "x$PGSCONFIG" = "xyes"; then
              AC_MSG_ERROR([you must specify a parameter to --with-pgconfig, e.g. --with-pgconfig=/path/to/pg_config])
      else
              if test -f $PGCONFIG; then
                      AC_MSG_RESULT([Using user-specified pg_config file: $PGCONFIG])
              else
                      AC_MSG_ERROR([the user-specified pg_config file $PGCONFIG does not exist])
              fi     
      fi
fi
PG_CFLAGS=-I`$PGCONFIG --includedir`
PG_LDFLAGS=-L`$PGCONFIG --libdir`
AC_SUBST(PG_CFLAGS)
AC_SUBST(PG_LDFLAGS)
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#   --with-libpq_deferred
#
AC_ARG_WITH(libpq_deferred, [AS_HELP_STRING(
	[--with-libpq_deferred], [enables libpq late binding [default=no]])],
	[], [with_libpq_deferred=no])
if test x"$with_libpq_deferred" != "xno"; then
	AC_DEFINE(LIBPQ_DEFERRED)
	libpq_deferred=1
fi
#-----------------------------------------------------------------------

# Ensure that we can parse libpq-fe.h
CFLAGS_SAVE="$CFLAGS"
CFLAGS="$PG_CFLAGS"
AC_CHECK_HEADERS(libpq-fe.h,, [AC_MSG_ERROR([cannot find libpq-fe.h, bailing out])])
CFLAGS="$CFLAGS_SAVE"	

if test x"$libpq_deferred" != "x1"; then
	# Ensure we can link against libpq
	LDFLAGS_SAVE="$LDFLAGS"
	LDFLAGS="$PG_LDFLAGS"
	AC_CHECK_LIB(pq,PQconnectdb,PG_LIB=-lpq,AC_MSG_ERROR(['libpq' is required but it doesn't seem to be installed on this system.]))
	LDFLAGS="$LDFLAGS_SAVE"
	AC_SUBST(PG_LIB)
fi

AC_CONFIG_FILES([Makefile \
	icons/Makefile \
	win_resource/Makefile \
	gnome_resource/Makefile \
	mac_resource/Makefile])

#-----------------------------------------------------------------------
#   --enable-sqlite_stmtstatus_autoindex
#
AC_ARG_ENABLE(sqlite_stmtstatus_autoindex, [AS_HELP_STRING(
  [--enable-sqlite_stmtstatus_autoindex], [enables SQLITE_STMTSTATUS_AUTOINDEX [default=yes]])],
  [], [sqlite_stmtstatus_autoindex=yes])
if test x"$enable_sqlite_stmtstatus_autoindex" != "xno"; then
  OMIT_SQLITE_STMTSTATUS_AUTOINDEX_FLAGS=
else
  OMIT_SQLITE_STMTSTATUS_AUTOINDEX_FLAGS=-DOMIT_SQLITE_STMTSTATUS_AUTOINXED
fi
AC_SUBST(OMIT_SQLITE_STMTSTATUS_AUTOINDEX_FLAGS)

# checks for SQLite version-depending constants
AC_CHECK_DECL([SQLITE_DBSTATUS_LOOKASIDE_USED],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_LOOKASIDE_USED)],[],[[#include <sqlite3.h>]])
AC_CHECK_DECL([SQLITE_DBSTATUS_LOOKASIDE_HIT],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_LOOKASIDE_HIT)],[],[[#include <sqlite3.h>]])
AC_CHECK_DECL([SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE)],[],[[#include <sqlite3.h>]])
AC_CHECK_DECL([SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL)],[],[[#include <sqlite3.h>]])
AC_CHECK_DECL([SQLITE_DBSTATUS_CACHE_USED],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_CACHE_USED)],[],[[#include <sqlite3.h>]])
AC_CHECK_DECL([SQLITE_DBSTATUS_CACHE_HIT],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_CACHE_HIT)],[],[[#include <sqlite3.h>]])
AC_CHECK_DECL([SQLITE_DBSTATUS_CACHE_MISS],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_CACHE_MISS)],[],[[#include <sqlite3.h>]])
AC_CHECK_DECL([SQLITE_DBSTATUS_CACHE_WRITE],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_CACHE_WRITE)],[],[[#include <sqlite3.h>]])
AC_CHECK_DECL([SQLITE_DBSTATUS_SCHEMA_USED],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_SCHEMA_USED)],[],[[#include <sqlite3.h>]])
AC_CHECK_DECL([SQLITE_DBSTATUS_STMT_USED],
  [AC_DEFINE(HAVE_DECL_SQLITE_DBSTATUS_STMT_USED)],[],[[#include <sqlite3.h>]])

AC_OUTPUT
