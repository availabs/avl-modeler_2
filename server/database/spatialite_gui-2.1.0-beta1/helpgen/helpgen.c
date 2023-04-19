/* 
/ helpgen
/
/ a tool autogenerating C++ code for SpatiaLite_GUI
/ [HTML doc]
/
/ version 1.0, 2017 August 4
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2017  Alessandro Furieri
/
/    This program is free software: you can redistribute it and/or modify
/    it under the terms of the GNU General Public License as published by
/    the Free Software Foundation, either version 3 of the License, or
/    (at your option) any later version.
/
/    This program is distributed in the hope that it will be useful,
/    but WITHOUT ANY WARRANTY; without even the implied warranty of
/    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/    GNU General Public License for more details.
/
/    You should have received a copy of the GNU General Public License
/    along with this program.  If not, see <http://www.gnu.org/licenses/>.
/
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <zlib.h>

/*
static void
do_debug (const char *html, int len)
{
    fwrite (html, 1, len, stdout);
}
*/

static int
load_splite (FILE * in, char **buf, int *buf_len)
{
/* loading in memory "./spatialite-sql-latest.html" */
    char *mem = NULL;
    int size;
    if (fseek (in, 0, SEEK_END) < 0)
      {
	  fprintf (stderr,
		   "ERROR: fseek failure on \"./spatialite-sql-latest.html/\"\n");
	  goto stop;
      }
    size = ftell (in);
    rewind (in);
    mem = malloc (size);
    if (mem == NULL)
      {
	  fprintf (stderr, "ERROR: insufficient memory\n");
	  goto stop;
      }
    if (fread (mem, 1, size, in) != (unsigned int) size)
      {
	  fprintf (stderr,
		   "ERROR: fread failure on \"./spatialite-sql-latest.html/\"\n");
	  goto stop;
      }
    *buf = mem;
    *buf_len = size;
    return 1;

  stop:
    *buf = NULL;
    *buf_len = 0;
    return 0;
}

static int
load_rl2 (FILE * in, char **buf, int *buf_len)
{
/* loading in memory "./rl2-sql-latest.html" */
    char *mem = NULL;
    int size;
    if (fseek (in, 0, SEEK_END) < 0)
      {
	  fprintf (stderr,
		   "ERROR: fseek failure on \"./rl2-sql-latest.html/\"\n");
	  goto stop;
      }
    size = ftell (in);
    rewind (in);
    mem = malloc (size);
    if (mem == NULL)
      {
	  fprintf (stderr, "ERROR: insufficient memory\n");
	  goto stop;
      }
    if (fread (mem, 1, size, in) != (unsigned int) size)
      {
	  fprintf (stderr,
		   "ERROR: fread failure on \"./rl2-sql-latest.html/\"\n");
	  goto stop;
      }
    *buf = mem;
    *buf_len = size;
    return 1;

  stop:
    *buf = NULL;
    *buf_len = 0;
    return 0;
}

static int
check_back (const char *buf)
{
    const char *p = buf;
    while (1)
      {
	  if (*p == ' ' || *p == '\t')
	    {
		/* skipping leading blanks */
		p++;
		continue;
	    }
	  break;
      }
    if (strcmp
	(p,
	 "<a href=\"https://www.gaia-gis.it/fossil/libspatialite\">back</a>") ==
	0)
	return 1;
    return 0;
}

static int
check_end (const char *buf)
{
    const char *p = buf;
    while (1)
      {
	  if (*p == ' ' || *p == '\t')
	    {
		/* skipping leading blanks */
		p++;
		continue;
	    }
	  break;
      }
    if (strncmp (p, "</body>", 7) == 0)
	return 1;
    return 0;
}

static void
check_toc (char *buf)
{
    int i;
    char title[256];
    const char *str;
    char *p = buf;
    while (1)
      {
	  if (*p == ' ' || *p == '\t')
	    {
		/* skipping leading blanks */
		p++;
		continue;
	    }
	  break;
      }
    if (strncmp (p, "<h2>SpatiaLite ", 15) != 0)
	return;

    strcpy (title, p + 4);
    *(title + strlen (title) - 5) = '\0';
    p = buf;

    str = "<h1>Table of Contents</h1>\n<ul>\n";
    for (i = 0; i < (int) strlen (str); i++)
	*p++ = str[i];

    str = "<li><a href=\"#spatialite_sql\">SpatiaLite SQL functions</a></li>\n";
    for (i = 0; i < (int) strlen (str); i++)
	*p++ = str[i];

    str =
	"<li><a href=\"#rasterlite2_sql\">RasterLite2 SQL functions</a></li>\n";
    for (i = 0; i < (int) strlen (str); i++)
	*p++ = str[i];

    str = "</ul><br><hr>\n<h2><a name=\"spatialite_sql\">";
    for (i = 0; i < (int) strlen (str); i++)
	*p++ = str[i];

    str = title;
    for (i = 0; i < (int) strlen (str); i++)
	*p++ = str[i];

    str = "</a></h2>";
    for (i = 0; i < (int) strlen (str); i++)
	*p++ = str[i];

    *p = '\0';
}

static void
clean_splite (char **buf, int *buf_len)
{
/* cleaning "./spatialite-sql-latest.html" */
    const char *p_in = *buf;
    int in_len = *buf_len;
    char *out = malloc (in_len + 1024);
    char *p_out = out;
    char line[2048];
    char *p_line = line;
    int i;

    for (i = 0; i < in_len; i++)
      {
	  if (*p_in == '\r')
	    {
		/* skipping RETURN chars */
		p_in++;
		continue;
	    }
	  if (*p_in == '\n')
	    {
		/* found a NEWLINE char */
		int skip = 0;
		*p_line = '\0';
		check_toc (line);
		if (check_back (line))
		    skip = 1;
		if (check_end (line))
		    skip = 1;
		if (!skip)
		  {
		      /* copying the current line into the output buffer */
		      unsigned int x;
		      for (x = 0; x < strlen (line); x++)
			  *p_out++ = line[x];
		      *p_out++ = '\n';
		  }
		p_line = line;
		p_in++;
		continue;
	    }
	  *p_line++ = *p_in++;
      }
    free (*buf);
    *buf = out;
    *buf_len = p_out - out;
}

static int
check_start_rl2 (char *buf)
{
    int i;
    char title[256];
    const char *str;
    char *p = buf;
    while (1)
      {
	  if (*p == ' ' || *p == '\t')
	    {
		/* skipping leading blanks */
		p++;
		continue;
	    }
	  break;
      }
    if (strcmp (p, "<h1>RasterLite2 SQL functions - reference list</h1>") != 0)
	return 0;

    strcpy (title, p + 4);
    *(title + strlen (title) - 5) = '\0';
    p = buf;

    str = title;
    for (i = 0; i < (int) strlen (str); i++)
	*p++ = str[i];

    str = "</a></h1>";
    for (i = 0; i < (int) strlen (str); i++)
	*p++ = str[i];

    *p = '\0';

    return 1;
}

static int
check_end_rl2 (const char *buf)
{
    const char *p = buf;
    while (1)
      {
	  if (*p == ' ' || *p == '\t')
	    {
		/* skipping leading blanks */
		p++;
		continue;
	    }
	  break;
      }
    if (strcmp
	(p,
	 "Back to <a href=\"https://www.gaia-gis.it/fossil/librasterlite2/wiki?name=rasterlite2-doc\">RasterLite2 doc index</a>")
	== 0)
	return 1;
    return 0;
}

static void
sanitize_tags (char *line)
{
    char *p;
    while (1)
      {
	  p = strstr (line, "<br />");
	  if (p != NULL)
	      memcpy (p, "<br>  ", 6);
	  else
	      break;
      }
    while (1)
      {
	  p = strstr (line, "<hr />");
	  if (p != NULL)
	      memcpy (p, "<hr>  ", 6);
	  else
	      break;
      }
}

static void
clean_rl2 (char **buf, int *buf_len)
{
/* cleaning "./rl2-sql-latest.html" */
    const char *p_in = *buf;
    int in_len = *buf_len;
    char *out = malloc (in_len + 1024);
    char *p_out = out;
    char line[2048];
    char *p_line = line;
    int i;
    int start = 0;
    int end = 0;
    const char *header =
	"<br><br><br><br><br><br>\n<h1><a name=\"rasterlite2_sql\">";
    const char *footer = "</body></html>\n";

    for (i = 0; i < in_len; i++)
      {
	  if (*p_in == '\r')
	    {
		/* skipping RETURN chars */
		p_in++;
		continue;
	    }
	  if (*p_in == '\n')
	    {
		/* found a NEWLINE char */
		*p_line = '\0';
		if (check_start_rl2 (line))
		  {
		      int x;
		      start = 1;
		      for (x = 0; x < (int) strlen (header); x++)
			  *p_out++ = header[x];
		  }
		if (check_end_rl2 (line))
		    end = 1;
		sanitize_tags (line);
		if (start && !end)
		  {
		      /* copying the current line into the output buffer */
		      unsigned int x;
		      for (x = 0; x < strlen (line); x++)
			  *p_out++ = line[x];
		      *p_out++ = '\n';
		  }
		p_line = line;
		p_in++;
		continue;
	    }
	  *p_line++ = *p_in++;
      }
    free (*buf);
    for (i = 0; i < (int) strlen (footer); i++)
	*p_out++ = footer[i];
    *buf = out;
    *buf_len = p_out - out;
}

static int
zip_compress (const char *html, int len_html, unsigned char **zip, int *len_zip)
{
/* compressing the HTML page (DEFLATE - ZIP) */
    int ret;
    uLong zLen = len_html - 1;
    unsigned char *zip_buf = malloc (zLen);
    if (zip_buf == NULL)
      {
	  fprintf (stderr, "ERROR: insufficient memory\n");
	  goto error;
      }
    ret = compress (zip_buf, &zLen, (const Bytef *) html, (uLong) len_html);
    if (ret == Z_OK)
      {
	  *zip = zip_buf;
	  *len_zip = zLen;
	  return 1;
      }
    else if (ret == Z_BUF_ERROR)
	fprintf (stderr, "ERROR: ZIP compression causes inflation\n");
    else
	fprintf (stderr, "ERROR: internal ZIP codec failure (compression)\n");

  error:
    *zip = NULL;
    *len_zip = 0;
    return 0;
}

static void
print_source (FILE * out, int len_html, unsigned char *zip, int len_zip)
{
/* printing the C++ source file */
    int i;
    int j;
    int lim;

    fprintf (out, "/*\n/ HtmlHelp.cpp\n");
    fprintf (out, "/ spatialite_gui HTML Help generator\n/\n");
    fprintf (out, "/ This code was autogenerated by \"helpgen\" on xxxxx\n/\n");
    fprintf (out, "/ Author: Sandro Furieri a.furieri@lqt.it\n/\n");
    fprintf (out, "/ Copyright (C) 2017  Alessandro Furieri\n/\n");
    fprintf (out,
	     "/    This program is free software: you can redistribute it and/or modify\n");
    fprintf (out,
	     "/    it under the terms of the GNU General Public License as published by\n");
    fprintf (out,
	     "/    the Free Software Foundation, either version 3 of the License, or\n");
    fprintf (out, "/    (at your option) any later version.\n/\n");
    fprintf (out,
	     "/    This program is distributed in the hope that it will be useful,\n");
    fprintf (out,
	     "/    but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    fprintf (out,
	     "/    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
    fprintf (out, "/    GNU General Public License for more details.\n/\n");
    fprintf (out,
	     "/    You should have received a copy of the GNU General Public License\n");
    fprintf (out,
	     "/    along with this program.  If not, see <http://www.gnu.org/licenses/>.\n/\n*/\n\n");
    fprintf (out, "#include \"Classdef.h\"\n\n");
    fprintf (out, "#include <zlib.h>\n\n");
    fprintf (out, "void MyFrame::GetHelp(wxString & html_text)\n{\n");
    fprintf (out,
	     "//\n// return the HTML Help Page from the ZIP compressed data\n//\n");
    fprintf (out, "\tunsigned char *zip_buf = NULL;\n");
    fprintf (out, "\tint uncompressed_len = %d;\n", len_html);
    fprintf (out, "\tint compressed_len = %d;\n", len_zip);
    fprintf (out, "\tuLong inLen = compressed_len;\n");
    fprintf (out, "\tuLong outLen = uncompressed_len;\n");
    fprintf (out, "\tconst Bytef *in;\n");
    fprintf (out, "\tBytef *out;\n");
    fprintf (out, "\tchar *html = NULL;\n\n");

    fprintf (out, "// allocating the ZIP compressed buffer\n");
    fprintf (out, "\tzip_buf = (unsigned char *)malloc(uncompressed_len);\n");
    fprintf (out, "\tif (zip_buf == NULL)\n\t\tgoto error;\n\n");

    fprintf (out, "// feeding the ZIP compressed buffer\n");
    for (i = 0; i < len_zip; i += 25)
      {
	  fprintf (out, "\tFeedZipHtml (zip_buf, %d, \"", i);
	  lim = 25;
	  if (i + lim > len_zip)
	      lim = len_zip - i;
	  for (j = 0; j < lim; j++)
	      fprintf (out, "%02x", *(zip + i + j));
	  fprintf (out, "\");\n");
      }
    fprintf (out, "\n// uncompressing the ZIP payload\n");
    fprintf (out, "\tin = zip_buf;\n");
    fprintf (out, "\thtml = (char *)malloc (uncompressed_len + 1);\n");
    fprintf (out, "\tif (html == NULL)\n\t\tgoto error;\n");
    fprintf (out, "\tout = (Bytef *)html;\n");
    fprintf (out, "\tif (uncompress (out, &outLen, in, inLen) != Z_OK)\n");
    fprintf (out, "\t\tgoto error;\n");
    fprintf (out, "\t*(html + uncompressed_len) = '\\0';\n");
    fprintf (out, "\tfree(zip_buf);\n");
    fprintf (out, "\thtml_text = wxString::FromUTF8(html);\n");
    fprintf (out, "\tfree(html);\n");
    fprintf (out, "\treturn;\n\nerror:\n");
    fprintf (out, "\tif (zip_buf != NULL)\n\t\tfree(zip_buf);\n");
    fprintf (out, "\tif (html != NULL)\n\t\tfree(html);\n");
    fprintf (out, "\thtml_text = wxT(\"<html><body><h1>Sorry, HTML Help "
	     "is currently unavailable.</h1></body></html>\");\n");
    fprintf (out, "}\n");
}

int
main (void)
{
/*
/
/ Please note: no args are supported !!!
/
/ we'll expect to find two input files respectively named:
/ ./spatialite-sql-latest.html
/ ./rl2-sql-latest.html
/
/ the C++ code will be generated into: ./HtmlHelp.cpp
/
*/
    FILE *fl_splite = NULL;
    FILE *fl_rl2 = NULL;
    FILE *fl_out = NULL;
    char *p_splite = NULL;
    int len_splite = 0;
    char *p_rl2 = NULL;
    int len_rl2 = 0;
    char *html = NULL;
    int len_html = 0;
    unsigned char *zip = NULL;
    int len_zip = 0;

/* opening input and output files */
    fl_splite = fopen ("./spatialite-sql-latest.html", "rb");
    if (fl_splite == NULL)
      {
	  fprintf (stderr,
		   "ERROR: unable to open the \"./spatialite-sql-latest.html\" input file\n");
	  goto stop;
      }
    fl_rl2 = fopen ("./rl2-sql-latest.html", "rb");
    if (fl_rl2 == NULL)
      {
	  fprintf (stderr,
		   "ERROR: unable to open the \"./rl2-sql-latest.html\" input file\n");
	  goto stop;
      }
    fl_out = fopen ("./HtmlHelp.cpp", "wb");
    if (fl_out == NULL)
      {
	  fprintf (stderr,
		   "ERROR: unable to open/create the \"./HtmlHelp.cpp\" output file\n");
	  goto stop;
      }

/* loading and arranging input files */
    if (!load_splite (fl_splite, &p_splite, &len_splite))
      {
	  fprintf (stderr,
		   "ERROR: unable to load the \"./spatialite-sql-latest.html\" input file\n");
	  goto stop;
      }
    if (!load_rl2 (fl_rl2, &p_rl2, &len_rl2))
      {
	  fprintf (stderr,
		   "ERROR: unable to load the \"./rl2-sql-latest.html\" input file\n");
	  goto stop;
      }
    clean_splite (&p_splite, &len_splite);
    clean_rl2 (&p_rl2, &len_rl2);

/* creating in memory the uncompressed HTML page */
    len_html = len_splite + len_rl2;
    html = malloc (len_html);
    if (html == NULL)
      {
	  fprintf (stderr, "ERROR: insufficient memory\n");
	  goto stop;
      }
    memcpy (html, p_splite, len_splite);
    memcpy (html + len_splite, p_rl2, len_rl2);
    free (p_splite);
    p_splite = NULL;
    free (p_rl2);
    p_rl2 = NULL;

/* compressing the HTML page */
    if (!zip_compress (html, len_html, &zip, &len_zip))
      {
	  fprintf (stderr, "ERROR: unable to DEFLATE\n");
	  goto stop;
      }

/* autogenerating the C++ source */
    print_source (fl_out, len_html, zip, len_zip);

/* end - cleanup */
  stop:
    if (zip != NULL)
	free (zip);
    if (html != NULL)
	free (html);
    if (p_splite != NULL)
	free (p_splite);
    if (p_rl2 != NULL)
	free (p_rl2);
    if (fl_splite)
	fclose (fl_splite);
    if (fl_rl2)
	fclose (fl_rl2);
    if (fl_out)
	fclose (fl_out);
    return 0;
}
