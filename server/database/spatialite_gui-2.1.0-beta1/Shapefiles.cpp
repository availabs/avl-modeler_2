/*
/ Shapefiles.cpp
/ methods related to Shapefile loading and saving
/
/ version 1.7, 2013 May 8
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2008-2013  Alessandro Furieri
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

#include "Classdef.h"

#include <iconv.h>

#ifdef PROJ_NEW                 /* supporting PROJ.6 */
#include <proj.h>
#endif

#if defined(_WIN32) && !defined(__MINGW32__)
#define strcasecmp	_stricmp
#endif

void MyFrame::CleanTxtTab(char *buf)
{
// well-formatting a string to be used as a Txt/Tab string
  char *tmp;
  char *in;
  char *out = buf;
  int len = strlen(buf);
  tmp = (char *) malloc(len + 1);
  strcpy(tmp, buf);
  in = tmp;
  while (*in != '\0')
    {
      if (*in == '\t' || *in == '\r' || *in == '\n')
        {
          in++;
          *out++ = ' ';
      } else
        *out++ = *in++;
    }
  *out = '\0';
  free(tmp);
}

char *MyFrame::CleanCsv(const char *buf)
{
// well-formatting a string to be used as a Csv string
  char *tmp;
  char *result;
  const char *in;
  char *out;
  bool special = false;
  int extra = 0;
  int len = strlen(buf);
  in = buf;
  while (*in != '\0')
    {
      // counting how many extra-chars are needed
      if (*in == '"')
        extra++;
      in++;
    }
  tmp = (char *) sqlite3_malloc(len + extra + 1);
  in = buf;
  out = tmp;
  while (*in != '\0')
    {
      // cleaning
      if (*in == ',' || *in == '\r' || *in == '\n')
        special = true;
      if (*in == '"')
        *out++ = '"';
      *out++ = *in++;
    }
  *out = '\0';
  if (special == true)
    {
      result = (char *) sqlite3_malloc(len + extra + 3);
      sprintf(result, "\"%s\"", tmp);
      sqlite3_free(tmp);
      return result;
  } else
    return tmp;
}

char *MyFrame::CleanHtml(const char *buf)
{
// well-formatting a string to be used as an Html string
  char *tmp;
  int extra = 0;
  int len = strlen(buf);
  const char *in;
  char *out;
  in = buf;
  while (*in != '\0')
    {
      // counting how many extra-chars are needed
      if (*in == '"')
        if (*in == '<')
          extra += 4;
      if (*in == '>')
        extra += 4;
      if (*in == ' ')
        extra += 6;
      if (*in == '"')
        extra += 6;
      in++;
    }
  tmp = (char *) sqlite3_malloc(len + extra + 64);
  in = buf;
  out = tmp;
  while (*in != '\0')
    {
      // cleaning
      if (*in == '<')
        {
          *out++ = '&';
          *out++ = 'l';
          *out++ = 't';
          *out++ = ';';
          in++;
          continue;
        }
      if (*in == '>')
        {
          *out++ = '&';
          *out++ = 'g';
          *out++ = 't';
          *out++ = ';';
          in++;
          continue;
        }
      if (*in == ' ')
        {
          *out++ = '&';
          *out++ = 'n';
          *out++ = 'b';
          *out++ = 's';
          *out++ = 'p';
          *out++ = ';';
          in++;
          continue;
        }
      if (*in == '"')
        {
          *out++ = '&';
          *out++ = 'q';
          *out++ = 'u';
          *out++ = 'o';
          *out++ = 't';
          *out++ = ';';
          in++;
          continue;
        }
      if (*in == '&')
        {
          *out++ = '&';
          *out++ = 'a';
          *out++ = 'm';
          *out++ = 'p';
          *out++ = ';';
          in++;
          continue;
        }
      *out++ = *in++;
    }
  *out = '\0';
  return tmp;
}

bool MyFrame::TableAlreadyExists(wxString & name)
{
//
// checks if a table of this name already exists 
//
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  bool already_exists = false;
  char *xname;
  char *xname2;
  char *xsql;
  wxString sql =
    wxT
    ("SELECT name FROM sqlite_master WHERE type = 'table' AND Lower(name) = Lower('");
  xname = (char *) malloc((name.Len() * 4) + 1);
  strcpy(xname, name.ToUTF8());
  xname2 = gaiaSingleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("')");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        already_exists = true;
    }
  sqlite3_free_table(results);
  return already_exists;
}

bool MyFrame::CoverageAlreadyExists(wxString & name)
{
//
// checks if a Raster Coverage of this name already exists 
//
  wxString table = name + wxT("_levels");
  if (TableAlreadyExists(table) == true)
    return true;
  table = name + wxT("_sections");
  if (TableAlreadyExists(table) == true)
    return true;
  table = name + wxT("_tile_data");
  if (TableAlreadyExists(table) == true)
    return true;
  table = name + wxT("_tiles");
  if (TableAlreadyExists(table) == true)
    return true;
  table = name + wxT("_section_levels");
  if (TableAlreadyExists(table) == true)
    return true;
  return false;
}

bool MyFrame::SRIDnotExists(int srid)
{
//
// checks if a SRID value is a valid one
//
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  bool constrained = false;
  bool not_exists = true;
  wxString RefSysName;
  char *xsql;
  wxString sql =
    wxT
    ("SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'spatial_ref_sys'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        constrained = true;
    }
  sqlite3_free_table(results);
  if (constrained == false)
    return false;
  xsql =
    sqlite3_mprintf("SELECT ref_sys_name FROM spatial_ref_sys WHERE srid = %d",
                    srid);
  ret =
    sqlite3_get_table(SqliteHandle, xsql, &results, &rows, &columns, &errMsg);
  sqlite3_free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        not_exists = false;
    }
  sqlite3_free_table(results);
  return not_exists;
}

bool MyFrame::CheckMetadata()
{
//
// checking if METADATA tables are defined
//
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  bool constrained = false;
  if (IsConnected() == false)
    return false;
  if (SpatiaLiteMetadata == false)
    return false;
  char *xsql;
  wxString sql =
    wxT
    ("SELECT name FROM main.sqlite_master WHERE type = 'table' AND name = 'geometry_columns'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        constrained = true;
    }
  sqlite3_free_table(results);
  return constrained;
}

bool MyFrame::CheckMetadata(wxString & dbAlias)
{
//
// checking if METADATA tables are defined [Attached DB]
//
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  bool constrained = false;
  if (IsConnected() == false)
    return false;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  char *xsql;
  wxString sql = wxT("SELECT name FROM \"") + dbAlias2;
  sql +=
    wxT("\".sqlite_master WHERE type = 'table' AND name = 'geometry_columns'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        constrained = true;
    }
  sqlite3_free_table(results);
  return constrained;
}

gaiaDbfFieldPtr MyFrame::GetDbfField(gaiaDbfListPtr list, int index)
{
//
// find a DBF attribute by name 
//
  int count = 0;
  gaiaDbfFieldPtr fld = list->First;
  while (fld)
    {
      if (count == index)
        return fld;
      count++;
      fld = fld->Next;
    }
  return NULL;
}

bool MyFrame::OutputPrjFileProjNew(const void *proj_ctx, wxString & path,
                                   int srid)
{
//
// exporting [if possible] a .PRJ file based on PROJ.6 ESRI WKT
//
#ifdef PROJ_NEW                 /* only id PROJ.6 is supported */
  PJ_CONTEXT *ctx = (PJ_CONTEXT *) proj_ctx;
  PJ *crs_def;
  FILE *out;
  const char *projPath;
  const char *auth_name = "EPSG";
  char dummy[8192];
  const char *wkt;
  char *options[4];
  options[0] = (char *) "MULTILINE=NO";
  options[1] = (char *) "INDENTATION_WIDTH=4";
  options[2] = (char *) "OUTPUT_AXIS=AUTO";
  options[3] = NULL;

  if (ctx == NULL)
    goto error;
  projPath = proj_context_get_database_path(ctx);
  if (projPath == NULL)
    goto error;                 // PROJ's own SQLite DB is undefined


// attemping to get an ESRI WKT from PROJ.6
  sprintf(dummy, "%d", srid);
  crs_def =
    proj_create_from_database(ctx, auth_name, dummy, PJ_CATEGORY_CRS, 0, NULL);
  if (crs_def != NULL)
    {
      wkt =
        proj_as_wkt(ctx, crs_def, PJ_WKT1_ESRI, (const char *const *) options);
      if (wkt != NULL)
        {
          // generating the .PRJ file
          strcpy(dummy, path.ToUTF8());
          strcat(dummy, ".prj");
          out = fopen(dummy, "wb");
          if (out)
            {
              fprintf(out, "%s\r\n", wkt);
              fclose(out);
            }
        }
      proj_destroy(crs_def);
    }
  return true;

error:
#endif
  if (path.Len() == 0 && srid < 0 && proj_ctx == NULL)
    srid = 0;                   // silencing stupid compiler warnings about unused args
  return false;
}

void MyFrame::OutputPrjFile(const void *proj_ctx, wxString & path, int srid)
{
//
// exporting [if possible] a .PRJ file
//
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  char sql[1024];
  int ret;
  bool rs_srid = false;
  bool rs_srs_wkt = false;
  bool rs_srtext = false;
  bool is_srs = false;
  const char *name;
  wxString srsWkt;
  char dummy[8192];

#ifdef PROJ_NEW
/* if new PROJ.6 is available we'll attempt first to export a genuine ESRI WKT */
  if (proj_ctx != NULL)
    {
      if (OutputPrjFileProjNew(proj_ctx, path, srid))
        return;
    }
#else
/* suppressing stupid compiler warnings about unused args */
  if (proj_ctx == NULL)
    proj_ctx = NULL;
#endif

// step I: checking if the SRS_WKT column actually exists
  ret =
    sqlite3_get_table(SqliteHandle, "PRAGMA table_info(spatial_ref_sys)",
                      &results, &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("dump shapefile error:") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 1];
          if (strcasecmp(name, "srid") == 0)
            rs_srid = true;
          if (strcasecmp(name, "srs_wkt") == 0)
            rs_srs_wkt = true;
          if (strcasecmp(name, "srtext") == 0)
            rs_srtext = true;
        }
    }
  sqlite3_free_table(results);
  if (rs_srs_wkt == true || rs_srtext == true)
    is_srs = true;
  if (rs_srid == false || is_srs == false)
    return;

// step II: fetching WKT SRS
  if (rs_srtext == true)
    sprintf(sql,
            "SELECT srtext FROM spatial_ref_sys WHERE srid = %d AND srtext IS NOT NULL",
            srid);
  else
    sprintf(sql,
            "SELECT srs_wkt FROM spatial_ref_sys WHERE srid = %d AND srs_wkt IS NOT NULL",
            srid);
  ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("dump shapefile error:") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          srsWkt = wxString::FromUTF8(name);
        }
    }
  sqlite3_free_table(results);
  if (srsWkt.Len() == 0)
    return;

// step IV: generating the .PRJ file
  strcpy(dummy, path.ToUTF8());
  strcat(dummy, ".prj");
  FILE *out = fopen(dummy, "wb");
  if (!out)
    goto no_file;
  strcpy(dummy, srsWkt.ToUTF8());
  fprintf(out, "%s\r\n", dummy);
  fclose(out);
no_file:
  return;
}

void MyFrame::DumpTxtTab(wxString & path, wxString & table, wxString & charset)
{
//
// dumping a  table as Txt/Tab
//
  wxString sql;
  sqlite3_stmt *stmt = NULL;
  int ret;
  int rows = 0;
  int i;
  int n_cols;
  char xpath[1024];
  char *dummy;
  char outCs[128];
  char *xname;
  char *xname2;
  wxString msg;
  int decimal_precision = GetDecimalPrecision();
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  FILE *out = fopen(xpath, "w");
  if (!out)
    goto no_file;
//
// preparing SQL statement 
//
  sql = wxT("SELECT * FROM \"");
  xname = (char *) malloc((table.Len() * 4) + 1);
  strcpy(xname, table.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\"");
//
// compiling SQL prepared statement 
//
  xname = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xname, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, xname, strlen(xname), &stmt, NULL);
  free(xname);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (rows == 0)
            {
              // outputting the column titles
              for (i = 0; i < n_cols; i++)
                {
                  const char *text =
                    (const char *) sqlite3_column_name(stmt, i);
                  int len = strlen(text);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  CleanTxtTab(dummy);
                  if (!gaiaConvertCharset
                      (&dummy, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  if (i == 0)
                    fprintf(out, "%s", dummy);
                  else
                    fprintf(out, "\t%s", dummy);
                  sqlite3_free(dummy);
                }
              fprintf(out, "\n");
            }
          rows++;
          for (i = 0; i < n_cols; i++)
            {
              if (i > 0)
                fprintf(out, "\t");
              if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                  fprintf(out, "%I64d", sqlite3_column_int64(stmt, i));
#else
                  fprintf(out, "%lld", sqlite3_column_int64(stmt, i));
#endif
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  if (decimal_precision >= 0)
                    {
                      char fmt[64];
                      sprintf(fmt, "%%1.%df", decimal_precision);
                      fprintf(out, fmt, sqlite3_column_double(stmt, i));
                  } else
                    fprintf(out, "%1.6f", sqlite3_column_double(stmt, i));
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  const char *text =
                    (const char *) sqlite3_column_text(stmt, i);
                  int len = sqlite3_column_bytes(stmt, i);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  CleanTxtTab(dummy);
                  if (!gaiaConvertCharset(&dummy, "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "%s", dummy);
                  sqlite3_free(dummy);
                }
            }
          fprintf(out, "\n");
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  fclose(out);
  sprintf(xpath, "Exported %d rows into Txt/Tab file", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump Txt/Tab error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump Txt/Tab: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

void MyFrame::ExportResultSetAsTxtTab(wxString & path, wxString & sql,
                                      wxString & charset)
{
//
// exporting a ResultSet as Txt/Tab
//
  sqlite3_stmt *stmt = NULL;
  int ret;
  int rows = 0;
  int i;
  int n_cols;
  char xpath[1024];
  char *dummy;
  char outCs[128];
  wxString msg;
  int decimal_precision = GetDecimalPrecision();
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  FILE *out = fopen(xpath, "w");
  if (!out)
    goto no_file;
//
// compiling SQL prepared statement 
//
  dummy = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(dummy, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, dummy, strlen(dummy), &stmt, NULL);
  free(dummy);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (rows == 0)
            {
              // outputting the column titles
              for (i = 0; i < n_cols; i++)
                {
                  const char *text =
                    (const char *) sqlite3_column_name(stmt, i);
                  int len = strlen(text);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  CleanTxtTab(dummy);
                  if (!gaiaConvertCharset
                      (&dummy, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  if (i == 0)
                    fprintf(out, "%s", dummy);
                  else
                    fprintf(out, "\t%s", dummy);
                  sqlite3_free(dummy);
                }
              fprintf(out, "\n");
            }
          rows++;
          for (i = 0; i < n_cols; i++)
            {
              if (i > 0)
                fprintf(out, "\t");
              if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                  fprintf(out, "%I64d", sqlite3_column_int64(stmt, i));
#else
                  fprintf(out, "%lld", sqlite3_column_int64(stmt, i));
#endif
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  if (decimal_precision >= 0)
                    {
                      char fmt[64];
                      sprintf(fmt, "%%1.%df", decimal_precision);
                      fprintf(out, fmt, sqlite3_column_double(stmt, i));
                  } else
                    fprintf(out, "%1.6f", sqlite3_column_double(stmt, i));
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  const char *text =
                    (const char *) sqlite3_column_text(stmt, i);
                  int len = sqlite3_column_bytes(stmt, i);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  CleanTxtTab(dummy);
                  if (!gaiaConvertCharset(&dummy, "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "%s", dummy);
                  sqlite3_free(dummy);
                }
            }
          fprintf(out, "\n");
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  fclose(out);
  sprintf(xpath, "Exported %d rows into Txt/Tab file", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump Txt/Tab error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump Txt/Tab: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

void MyFrame::DumpCsv(wxString & path, wxString & table, wxString & charset)
{
//
// dumping a  table as CSV
//
  wxString sql;
  sqlite3_stmt *stmt = NULL;
  int ret;
  int rows = 0;
  int i;
  int n_cols;
  char xpath[1024];
  char *dummy;
  char outCs[128];
  wxString msg;
  char *xname;
  char *xname2;
  int decimal_precision = GetDecimalPrecision();
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  FILE *out = fopen(xpath, "w");
  if (!out)
    goto no_file;
//
// preparing SQL statement 
//
  sql = wxT("SELECT * FROM \"");
  xname = (char *) malloc((table.Len() * 4) + 1);
  strcpy(xname, table.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\"");
//
// compiling SQL prepared statement 
//
  xname = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xname, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, xname, strlen(xname), &stmt, NULL);
  free(xname);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (rows == 0)
            {
              // outputting the column titles
              for (i = 0; i < n_cols; i++)
                {
                  char *cleaned;
                  const char *text =
                    (const char *) sqlite3_column_name(stmt, i);
                  int len = strlen(text);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  cleaned = CleanCsv(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&cleaned, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  if (i == 0)
                    fprintf(out, "%s", cleaned);
                  else
                    fprintf(out, ",%s", cleaned);
                  sqlite3_free(cleaned);
                }
              fprintf(out, "\n");
            }
          rows++;
          for (i = 0; i < n_cols; i++)
            {
              if (i > 0)
                fprintf(out, ",");
              if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                  fprintf(out, "%I64d", sqlite3_column_int64(stmt, i));
#else
                  fprintf(out, "%lld", sqlite3_column_int64(stmt, i));
#endif
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  if (decimal_precision >= 0)
                    {
                      char fmt[64];
                      sprintf(fmt, "%%1.%df", decimal_precision);
                      fprintf(out, fmt, sqlite3_column_double(stmt, i));
                  } else
                    fprintf(out, "%1.6f", sqlite3_column_double(stmt, i));
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  char *cleaned;
                  const char *text =
                    (const char *) sqlite3_column_text(stmt, i);
                  int len = sqlite3_column_bytes(stmt, i);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  cleaned = CleanCsv(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&cleaned, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "%s", cleaned);
                  sqlite3_free(cleaned);
                }
            }
          fprintf(out, "\n");
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  fclose(out);
  sprintf(xpath, "Exported %d rows into CSV file", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump CSV error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump CSV: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

void MyFrame::ExportResultSetAsCsv(wxString & path, wxString & sql,
                                   wxString & charset)
{
//
// exporting a ResultSet as CSV
//
  sqlite3_stmt *stmt = NULL;
  int ret;
  int rows = 0;
  int i;
  int n_cols;
  char xpath[1024];
  char *dummy;
  char outCs[128];
  wxString msg;
  int decimal_precision = GetDecimalPrecision();
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  FILE *out = fopen(xpath, "w");
  if (!out)
    goto no_file;
//
// compiling SQL prepared statement 
//
  dummy = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(dummy, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, dummy, strlen(dummy), &stmt, NULL);
  free(dummy);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (rows == 0)
            {
              // outputting the column titles
              for (i = 0; i < n_cols; i++)
                {
                  char *cleaned;
                  const char *text =
                    (const char *) sqlite3_column_name(stmt, i);
                  int len = strlen(text);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  cleaned = CleanCsv(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&cleaned, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  if (i == 0)
                    fprintf(out, "%s", cleaned);
                  else
                    fprintf(out, ",%s", cleaned);
                  sqlite3_free(cleaned);
                }
              fprintf(out, "\n");
            }
          rows++;
          for (i = 0; i < n_cols; i++)
            {
              if (i > 0)
                fprintf(out, ",");
              if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                  fprintf(out, "%I64d", sqlite3_column_int64(stmt, i));
#else
                  fprintf(out, "%lld", sqlite3_column_int64(stmt, i));
#endif
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  if (decimal_precision >= 0)
                    {
                      char fmt[64];
                      sprintf(fmt, "%%1.%df", decimal_precision);
                      fprintf(out, fmt, sqlite3_column_double(stmt, i));
                  } else
                    fprintf(out, "%1.6f", sqlite3_column_double(stmt, i));
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  char *cleaned;
                  const char *text =
                    (const char *) sqlite3_column_text(stmt, i);
                  int len = sqlite3_column_bytes(stmt, i);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  cleaned = CleanCsv(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&cleaned, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "%s", cleaned);
                  sqlite3_free(cleaned);
                }
            }
          fprintf(out, "\n");
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  fclose(out);
  sprintf(xpath, "Exported %d rows into CSV file", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump CSV error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump CSV: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

void MyFrame::DumpHtml(wxString & path, wxString & table,
                       wxString & sqlite_path, wxString & charset)
{
//
// dumping a  table as HTML
//
  wxString sql;
  sqlite3_stmt *stmt = NULL;
  int ret;
  int rows = 0;
  int i;
  int n_cols;
  char xpath[1024];
  char sqlpath[1024];
  char xtable[1024];
  char *dummy;
  char outCs[128];
  wxString msg;
  char *xname;
  char *xname2;
  int decimal_precision = GetDecimalPrecision();
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  strcpy(sqlpath, sqlite_path.ToUTF8());
  strcpy(xtable, table.ToUTF8());
  FILE *out = fopen(xpath, "w");
  if (!out)
    goto no_file;
  fprintf(out,
          "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
  fprintf(out, "<html>\n\t<head>\n");
  fprintf(out,
          "\t\t<meta http-equiv=\"content-type\" content=\"text/html; charset=%s\">\n",
          outCs);
  fprintf(out, "\t\t<title>\nTable '%s': from SQLite/SpatiaLite DB '%s'\n",
          xtable, sqlpath);
  fprintf(out, "\t\t</title>\n");
  fprintf(out, "\t\t<style type=\"text/css\">\n");
  fprintf(out, "table { border: 1px; }\n");
  fprintf(out, "tr.t0 th { background-color: #dfc9c9; }\n");
  fprintf(out, "tr.d0 td { background-color: #e0efe0; }\n");
  fprintf(out, "tr.d1 td { background-color: #d0d0df; }\n");
  fprintf(out, "\t\t</style>\n\t</head>\n\t<body>\n\t\t<table>\n");
//
// preparing SQL statement 
//
  sql = wxT("SELECT * FROM \"");
  xname = (char *) malloc((table.Len() * 4) + 1);
  strcpy(xname, table.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\"");
//
// compiling SQL prepared statement 
//
  xname = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xname, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, xname, strlen(xname), &stmt, NULL);
  free(xname);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if ((rows % 20) == 0)
            {
              // outputting the column titles
              fprintf(out, "\t\t\t<tr class=\"t0\">\n");
              for (i = 0; i < n_cols; i++)
                {
                  char *cleaned;
                  const char *text =
                    (const char *) sqlite3_column_name(stmt, i);
                  int len = strlen(text);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  cleaned = CleanHtml(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&cleaned, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "\t\t\t\t<th>%s</th>\n", cleaned);
                  sqlite3_free(cleaned);
                }
              fprintf(out, "\t\t\t</tr>\n");
            }
          rows++;
          fprintf(out, "\t\t\t<tr class=\"%s\">\n", (rows % 2) ? "d0" : "d1");
          for (i = 0; i < n_cols; i++)
            {
              if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                  fprintf(out, "\t\t\t\t<td align=\"right\">%I64d</td>\n",
                          sqlite3_column_int64(stmt, i));
#else
                  fprintf(out, "\t\t\t\t<td align=\"right\">%lld</td>\n",
                          sqlite3_column_int64(stmt, i));
#endif
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  if (decimal_precision >= 0)
                    {
                      char fmt[64];
                      sprintf(fmt, "\t\t\t\t<td align=\"right\">%%1.%df</td>\n",
                              decimal_precision);
                      fprintf(out, fmt, sqlite3_column_double(stmt, i));
                  } else
                    fprintf(out, "\t\t\t\t<td align=\"right\">%1.6f</td>\n",
                            sqlite3_column_double(stmt, i));
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  char *cleaned;
                  const char *text =
                    (const char *) sqlite3_column_text(stmt, i);
                  int len = sqlite3_column_bytes(stmt, i);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  cleaned = CleanHtml(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&cleaned, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "\t\t\t\t<td>%s</td>\n", cleaned);
                  sqlite3_free(cleaned);
                }
            }
          fprintf(out, "\t\t\t</tr>\n");
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  fprintf(out, "\t\t</table>\n\t</body>\n</html>\n");
  fclose(out);
  sprintf(xpath, "Exported %d rows into HTML file", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump HTML error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump HTML: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

char *MyFrame::DifQuote(const char *buf)
{
// well-formatting a string to be used as a DIF string
  char *tmp;
  const char *in = buf;
  char *out;
  int len = 2;
  while (*in != '\0')
    {
      // compunting how many bytes are there
      if (*in == '"')
        len++;
      len++;
      in++;
    }
  tmp = (char *) sqlite3_malloc(len + 1);
  out = tmp;
  in = buf;
  *out++ = '"';
  while (*in != '\0')
    {
      if (*in == '"')
        *out++ = '"';
      *out++ = *in++;
    }
  *out++ = '"';
  *out = '\0';
  return tmp;
}

void MyFrame::DecimalNumber(double num, char *str, char decimal_point)
{
// well-formatting a decimal number
  char dummy[128];
  char *out = str;
  int i;
  sprintf(dummy, "%1.18f", num);
  for (i = strlen(dummy) - 1; i >= 0; i--)
    {
      if (dummy[i] == '0')
        dummy[i] = '\0';
      else
        break;
    }
  if (dummy[strlen(dummy) - 1] == '.')
    strcat(dummy, "0");
  for (i = 0; i < (int) strlen(dummy); i++)
    {
      if (dummy[i] == '.')
        {
          // replacing LOCALE decimal point
          *out++ = decimal_point;
          continue;
        }
      *out++ = dummy[i];
    }
  *out = '\0';
}

bool MyFrame::TestDateValue(char *date)
{
// testing for DATE
  char buf[8];
  int dd;
  int mm;
  int yy;
  if (strlen(date) != 10)
    return false;
  if (date[4] == '-' && date[7] == '-')
    ;
  else
    return false;
  if (!isdigit(date[0]))
    return false;
  buf[0] = date[0];
  if (!isdigit(date[1]))
    return false;
  buf[1] = date[1];
  if (!isdigit(date[2]))
    return false;
  buf[2] = date[2];
  if (!isdigit(date[3]))
    return false;
  buf[3] = date[3];
  buf[4] = '\0';
  yy = atoi(buf);
  if (yy < 1901 || yy > 2099)
    return false;
  if (!isdigit(date[5]))
    return false;
  buf[0] = date[5];
  if (!isdigit(date[6]))
    return false;
  buf[1] = date[6];
  buf[2] = '\0';
  mm = atoi(buf);
  if (!isdigit(date[8]))
    return false;
  buf[0] = date[8];
  if (!isdigit(date[9]))
    return false;
  buf[1] = date[9];
  buf[2] = '\0';
  dd = atoi(buf);
  if (mm < 1 || mm > 12)
    return false;
  if (dd < 1)
    return false;
  switch (mm)
    {
      case 2:
        if ((yy % 4) == 0)
          {
            // leap year, according to M$ :-)
            if (dd > 29)
              return false;
        } else
          {
            if (dd > 28)
              return false;
          }
        break;
      case 4:
      case 6:
      case 9:
      case 11:
        if (dd > 30)
          return false;
        break;
      default:
        if (dd > 31)
          return false;
        break;
    }
  return true;
}

bool MyFrame::TestDateTimeValue(char *datetime)
{
// testing for DATETIME
  char buf[8];
  int dd;
  int mm;
  int yy;
  int hh;
  int min;
  int ss = 0;
  if (strlen(datetime) < 16)
    return false;
  if (datetime[4] == '-' && datetime[7] == '-' && datetime[13] == ':')
    ;
  else
    return false;
  if (!isdigit(datetime[0]))
    return false;
  buf[0] = datetime[0];
  if (!isdigit(datetime[1]))
    return false;
  buf[1] = datetime[1];
  if (!isdigit(datetime[2]))
    return false;
  buf[2] = datetime[2];
  if (!isdigit(datetime[3]))
    return false;
  buf[3] = datetime[3];
  buf[4] = '\0';
  yy = atoi(buf);
  if (yy < 1901 || yy > 2099)
    return false;
  if (!isdigit(datetime[5]))
    return false;
  buf[0] = datetime[5];
  if (!isdigit(datetime[6]))
    return false;
  buf[1] = datetime[6];
  buf[2] = '\0';
  mm = atoi(buf);
  if (!isdigit(datetime[8]))
    return false;
  buf[0] = datetime[8];
  if (!isdigit(datetime[9]))
    return false;
  buf[1] = datetime[9];
  buf[2] = '\0';
  dd = atoi(buf);
  if (mm < 1 || mm > 12)
    return false;
  if (dd < 1)
    return false;
  switch (mm)
    {
      case 2:
        if ((yy % 4) == 0)
          {
            // leap year, according to M$ :-)
            if (dd > 29)
              return false;
        } else
          {
            if (dd > 28)
              return false;
          }
        break;
      case 4:
      case 6:
      case 9:
      case 11:
        if (dd > 30)
          return false;
        break;
      default:
        if (dd > 31)
          return false;
        break;
    }
  if (!isdigit(datetime[11]))
    return false;
  buf[0] = datetime[11];
  if (!isdigit(datetime[12]))
    return false;
  buf[1] = datetime[12];
  buf[2] = '\0';
  hh = atoi(buf);
  if (hh < 0 || hh > 23)
    return false;
  if (!isdigit(datetime[14]))
    return false;
  buf[0] = datetime[14];
  if (!isdigit(datetime[15]))
    return false;
  buf[1] = datetime[15];
  buf[2] = '\0';
  min = atoi(buf);
  if (min < 0 || hh > 59)
    return false;
  if (strlen(datetime) >= 19)
    {
      if (datetime[16] != ':')
        return false;
      if (!isdigit(datetime[17]))
        return false;
      buf[0] = datetime[17];
      if (!isdigit(datetime[18]))
        return false;
      buf[1] = datetime[18];
      buf[2] = '\0';
      ss = atoi(buf);
      if (ss < 0 || ss > 59)
        return false;
    }
  return true;
}

bool MyFrame::TestTimeValue(char *time)
{
// testing for TIME
  char buf[8];
  int hh;
  int min;
  int ss = 0;
  if (strlen(time) < 5)
    return false;
  if (time[2] == ':')
    ;
  else
    return false;
  if (!isdigit(time[0]))
    return false;
  buf[0] = time[0];
  if (!isdigit(time[1]))
    return false;
  buf[1] = time[1];
  buf[2] = '\0';
  hh = atoi(buf);
  if (hh < 0 || hh > 23)
    return false;
  if (!isdigit(time[3]))
    return false;
  buf[0] = time[3];
  if (!isdigit(time[4]))
    return false;
  buf[1] = time[4];
  buf[2] = '\0';
  min = atoi(buf);
  if (min < 0 || hh > 59)
    return false;
  if (strlen(time) >= 8)
    {
      if (time[5] != ':')
        return false;
      if (!isdigit(time[6]))
        return false;
      buf[0] = time[6];
      if (!isdigit(time[7]))
        return false;
      buf[1] = time[7];
      buf[2] = '\0';
      ss = atoi(buf);
      if (ss < 0 || ss > 59)
        return false;
    }
  return true;
}

int MyFrame::ComputeSpreadsheetDate(int yy, int mm, int dd)
{
// computing a DATE value (Spreadsheet style)
  int y = 1900;
  int m = 1;
  int d = 1;
  int day_count = 1;
  while (1)
    {
      // 
      // computing how many days intercurs between this date and 
      // 1901-01-01 (reference date: day #1)
      //
      if (y == yy && m == mm && d == dd)
        break;
      day_count++;
      switch (m)
        {
          case 2:
            if ((y % 4) == 0)
              {
                // leap year, according to M$ :-)
                if (d == 29)
                  {
                    d = 1;
                    m++;
                } else
                  d++;
            } else
              {
                if (d == 28)
                  {
                    d = 1;
                    m++;
                } else
                  d++;
              }
            break;
          case 4:
          case 6:
          case 9:
          case 11:
            if (d == 30)
              {
                d = 1;
                m++;
            } else
              d++;
            break;
          default:
            if (d == 31)
              {
                if (m == 12)
                  {
                    d = 1;
                    m = 1;
                    y++;
                } else
                  {
                    d = 1;
                    m++;
                  }
            } else
              d++;
            break;
        };
    }
  return day_count;
}

double MyFrame::ComputeSpreadsheetTime(int hh, int mm, int ss)
{
// computing a TIME value (Spreadsheet style)
  double now = (hh * 3600) + (mm * 60) + ss;  // seconds since midnight
  double day = 86400.0;         // seconds in a day
  return now / day;
}

int MyFrame::GetDateValue(char *date)
{
// computing a DATE value
  char buf[8];
  int dd;
  int mm;
  int yy;
  buf[0] = date[0];
  buf[1] = date[1];
  buf[2] = date[2];
  buf[3] = date[3];
  buf[4] = '\0';
  yy = atoi(buf);
  buf[0] = date[5];
  buf[1] = date[6];
  buf[2] = '\0';
  mm = atoi(buf);
  buf[0] = date[8];
  buf[1] = date[9];
  buf[2] = '\0';
  dd = atoi(buf);
  return ComputeSpreadsheetDate(yy, mm, dd);
}

double MyFrame::GetDateTimeValue(char *datetime)
{
// computing a DATETIME value
  char buf[8];
  int dd;
  int mm;
  int yy;
  int hh;
  int min;
  int ss = 0;
  int date;
  buf[0] = datetime[0];
  buf[1] = datetime[1];
  buf[2] = datetime[2];
  buf[3] = datetime[3];
  buf[4] = '\0';
  yy = atoi(buf);
  buf[0] = datetime[5];
  buf[1] = datetime[6];
  buf[2] = '\0';
  mm = atoi(buf);
  buf[0] = datetime[8];
  buf[1] = datetime[9];
  buf[2] = '\0';
  dd = atoi(buf);
  date = ComputeSpreadsheetDate(yy, mm, dd);
  buf[0] = datetime[11];
  buf[1] = datetime[12];
  buf[2] = '\0';
  hh = atoi(buf);
  buf[0] = datetime[14];
  buf[1] = datetime[15];
  buf[2] = '\0';
  min = atoi(buf);
  if (strlen(datetime) >= 19)
    {
      buf[0] = datetime[17];
      buf[1] = datetime[18];
      buf[2] = '\0';
      ss = atoi(buf);
    }
  return (double) date + ComputeSpreadsheetTime(hh, min, ss);
}

double MyFrame::GetTimeValue(char *time)
{
// computing a TIME value
  char buf[8];
  int hh;
  int min;
  int ss = 0;
  buf[0] = time[0];
  buf[1] = time[1];
  buf[2] = '\0';
  hh = atoi(buf);
  buf[0] = time[3];
  buf[1] = time[4];
  buf[2] = '\0';
  min = atoi(buf);
  if (strlen(time) >= 8)
    {
      buf[0] = time[6];
      buf[1] = time[7];
      buf[2] = '\0';
      ss = atoi(buf);
    }
  return ComputeSpreadsheetTime(hh, min, ss);
}

void MyFrame::DumpDif(wxString & path, wxString & table, wxString & charset,
                      char decimal_point, bool date_times)
{
//
// dumping a  table as DIF
//
  wxString sql;
  sqlite3_stmt *stmt = NULL;
  int ret;
  int rows = 0;
  int i;
  int n_cols;
  int tot_cols = 0;
  int tot_rows = 0;
  char xpath[1024];
  char xtable[1024];
  char *dummy;
  char *dummy2;
  char outCs[128];
  wxString msg;
  char *xname;
  char *xname2;
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  strcpy(xtable, table.ToUTF8());
  FILE *out = fopen(xpath, "wb");
  if (!out)
    goto no_file;
//
// preparing SQL statement 
//
  sql = wxT("SELECT * FROM \"");
  xname = (char *) malloc((table.Len() * 4) + 1);
  strcpy(xname, table.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\"");
//
// compiling SQL prepared statement 
//
  xname = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xname, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, xname, strlen(xname), &stmt, NULL);
  free(xname);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  while (1)
    {
      // first pass: we must compute #cols and #rows first of all
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (n_cols > tot_cols)
            tot_cols = n_cols;
          tot_rows++;
      } else
        goto sql_error;
    }
// DIF header
  fprintf(out, "TABLE\r\n0,1\r\n\"Sheet1\"\r\n");
  fprintf(out, "VECTORS\r\n0,%d\r\n\"\"\r\n", tot_cols);
  fprintf(out, "TUPLES\r\n0,%d\r\n\"\"\r\n", tot_rows);
  fprintf(out, "DATA\r\n0,0\r\n\"\"\r\n-1,0\r\n");
  sqlite3_reset(stmt);
  while (1)
    {
      // final pass: real output
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (rows == 0)
            {
              // column titles
              fprintf(out, "BOT\r\n");
              for (i = 0; i < n_cols; i++)
                {
                  const char *str = sqlite3_column_name(stmt, i);
                  dummy = (char *) sqlite3_malloc(strlen(str) + 1);
                  strcpy(dummy, str);
                  dummy2 = DifQuote(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&dummy2, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "1,0\r\n%s\r\n", dummy2);
                  sqlite3_free(dummy2);
                }
              // DIF row footer
              fprintf(out, "-1,0\r\n");
            }
          // DIF row header
          fprintf(out, "BOT\r\n");
          for (i = 0; i < n_cols; i++)
            {
              if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                  fprintf(out, "\t\t\t\t<td align=\"right\">%I64d</td>\n",
                          sqlite3_column_int64(stmt, i));
#else
                  fprintf(out, "\t\t\t\t<td align=\"right\">%lld</td>\n",
                          sqlite3_column_int64(stmt, i));
#endif
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  char num[128];
                  DecimalNumber(sqlite3_column_double(stmt, i), num,
                                decimal_point);
                  fprintf(out, "0,%s\r\nV\r\n", num);
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  const char *str = (const char *) sqlite3_column_text(stmt, i);
                  dummy = (char *) sqlite3_malloc(strlen(str) + 1);
                  strcpy(dummy, str);
                  if (date_times && TestDateValue(dummy) == true)
                    {
                      fprintf(out, "0,%d\r\nV\r\n", GetDateValue(dummy));
                      sqlite3_free(dummy);
                  } else if (date_times && TestDateTimeValue(dummy) == true)
                    {
                      char num[128];
                      DecimalNumber(GetDateTimeValue(dummy), num,
                                    decimal_point);
                      fprintf(out, "0,%s\r\nV\r\n", num);
                      sqlite3_free(dummy);
                  } else if (date_times && TestTimeValue(dummy) == true)
                    {
                      char num[128];
                      DecimalNumber(GetTimeValue(dummy), num, decimal_point);
                      fprintf(out, "0,%s\r\nV\r\n", num);
                      sqlite3_free(dummy);
                  } else
                    {
                      // plain text string
                      dummy2 = DifQuote(dummy);
                      sqlite3_free(dummy);
                      if (!gaiaConvertCharset
                          (&dummy2, (const char *) "UTF-8", outCs))
                        goto encoding_error;
                      fprintf(out, "1,0\r\n%s\r\n", dummy2);
                      sqlite3_free(dummy2);
                    }
              } else
                fprintf(out, "1,0\r\n\"\"\r\n");
            }
          // DIF row footer
          fprintf(out, "-1,0\r\n");
          rows++;
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
// DIF footer
  fprintf(out, "EOD\r\n");
  fclose(out);
  sprintf(xpath, "Exported %d rows into DIF spreadsheet", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump DIF error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump DIF: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

void MyFrame::ExportResultSetAsDif(wxString & path, wxString & sql,
                                   wxString & charset, char decimal_point,
                                   bool date_times)
{
//
// exporting a ResultSet as DIF
//
  sqlite3_stmt *stmt = NULL;
  int ret;
  int rows = 0;
  int i;
  int n_cols;
  int tot_cols = 0;
  int tot_rows = 0;
  char xpath[1024];
  char *dummy;
  char *dummy2;
  char outCs[128];
  wxString msg;
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  FILE *out = fopen(xpath, "wb");
  if (!out)
    goto no_file;
//
// compiling SQL prepared statement 
//
  dummy = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(dummy, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, dummy, strlen(dummy), &stmt, NULL);
  free(dummy);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  rows = 0;
  while (1)
    {
      // first pass: first of all we must compute #cols and #rows
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (n_cols > tot_cols)
            tot_cols = n_cols;
          tot_rows++;
      } else
        goto sql_error;
    }
// DIF header
  fprintf(out, "TABLE\r\n0,1\r\n\"Sheet1\"\r\n");
  fprintf(out, "VECTORS\r\n0,%d\r\n\"\"\r\n", tot_cols);
  fprintf(out, "TUPLES\r\n0,%d\r\n\"\"\r\n", tot_rows);
  fprintf(out, "DATA\r\n0,0\r\n\"\"\r\n-1,0\r\n");
  sqlite3_reset(stmt);
  while (1)
    {
      // final pass: real output
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (rows == 0)
            {
              // column titles
              fprintf(out, "BOT\r\n");
              for (i = 0; i < n_cols; i++)
                {
                  const char *str = sqlite3_column_name(stmt, i);
                  dummy = (char *) sqlite3_malloc(strlen(str) + 1);
                  strcpy(dummy, str);
                  dummy2 = DifQuote(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&dummy2, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "1,0\r\n%s\r\n", dummy2);
                  sqlite3_free(dummy2);
                }
              // DIF row footer
              fprintf(out, "-1,0\r\n");
            }
          // DIF row header
          fprintf(out, "BOT\r\n");
          for (i = 0; i < n_cols; i++)
            {
              if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                  fprintf(out, "0,%I64d\r\nV\r\n",
                          sqlite3_column_int64(stmt, i));
#else
                  fprintf(out, "0,%lld\r\nV\r\n",
                          sqlite3_column_int64(stmt, i));
#endif
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  char num[128];
                  DecimalNumber(sqlite3_column_double(stmt, i), num,
                                decimal_point);
                  fprintf(out, "0,%s\r\nV\r\n", num);
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  const char *str = (const char *) sqlite3_column_text(stmt, i);
                  dummy = (char *) sqlite3_malloc(strlen(str) + 1);
                  strcpy(dummy, str);
                  if (date_times && TestDateValue(dummy) == true)
                    {
                      fprintf(out, "0,%d\r\nV\r\n", GetDateValue(dummy));
                      sqlite3_free(dummy);
                  } else if (date_times && TestDateTimeValue(dummy) == true)
                    {
                      char num[128];
                      DecimalNumber(GetDateTimeValue(dummy), num,
                                    decimal_point);
                      fprintf(out, "0,%s\r\nV\r\n", num);
                      sqlite3_free(dummy);
                  } else if (date_times && TestTimeValue(dummy) == true)
                    {
                      char num[128];
                      DecimalNumber(GetTimeValue(dummy), num, decimal_point);
                      fprintf(out, "0,%s\r\nV\r\n", num);
                      sqlite3_free(dummy);
                  } else
                    {
                      // plain text string
                      dummy2 = DifQuote(dummy);
                      sqlite3_free(dummy);
                      if (!gaiaConvertCharset
                          (&dummy2, (const char *) "UTF-8", outCs))
                        goto encoding_error;
                      fprintf(out, "1,0\r\n%s\r\n", dummy2);
                      sqlite3_free(dummy2);
                    }
              } else
                fprintf(out, "1,0\r\n\"\"\r\n");
            }
          // DIF row footer
          fprintf(out, "-1,0\r\n");
          rows++;
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
// DIF footer
  fprintf(out, "EOD\r\n");
  fclose(out);
  sprintf(xpath, "Exported %d rows into DIF spreadsheet", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump DIF error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump DIF: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

char *MyFrame::SylkQuote(const char *buf)
{
// well-formatting a string to be used as a SYLK string
  char *tmp;
  const char *in = buf;
  char *out;
  tmp = (char *) sqlite3_malloc(strlen(buf) + 3);
  out = tmp;
  *out++ = '"';
  while (*in != '\0')
    *out++ = *in++;
  *out++ = '"';
  *out = '\0';
  return tmp;
}

void MyFrame::DumpSylk(wxString & path, wxString & table, wxString & charset,
                       bool date_times)
{
//
// dumping a  table as SYLK
//
  wxString sql;
  sqlite3_stmt *stmt = NULL;
  int ret;
  int i;
  int n_cols;
  int eff_row = 1;
  char format[16];
  char xpath[1024];
  char xtable[1024];
  char *dummy;
  char *dummy2;
  char outCs[128];
  wxString msg;
  char *xname;
  char *xname2;
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  strcpy(xtable, table.ToUTF8());
  FILE *out = fopen(xpath, "wb");
  if (!out)
    goto no_file;
//
// preparing SQL statement 
//
  sql = wxT("SELECT * FROM \"");
  xname = (char *) malloc((table.Len() * 4) + 1);
  strcpy(xname, table.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\"");
//
// compiling SQL prepared statement 
//
  xname = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xname, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, xname, strlen(xname), &stmt, NULL);
  free(xname);
  if (ret != SQLITE_OK)
    goto sql_error;
// SYLK header
  fprintf(out, "ID;PCALC001\r\n");
  fprintf(out, "P;PGeneral\r\n");
  fprintf(out, "P;P0.00\r\n");
  fprintf(out, "P;Pdd/mm/yyyy\r\n");
  fprintf(out, "P;Ph:mm:ss\r\n");
  fprintf(out, "P;Pdd/mm/yyyy\\ h:mm\r\n");
  while (1)
    {
      // exporting data
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (eff_row == 1)
            {
              // column titles
              for (i = 0; i < n_cols; i++)
                {
                  const char *str = sqlite3_column_name(stmt, i);
                  dummy = (char *) sqlite3_malloc(strlen(str) + 1);
                  strcpy(dummy, str);
                  dummy2 = SylkQuote(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&dummy2, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "C;X%d;Y%d;K%s\r\n", i + 1, eff_row, dummy2);
                  fprintf(out, "F;X%d;Y%d;FG0L\r\n", i + 1, eff_row);
                  sqlite3_free(dummy2);
                }
              eff_row++;
            }

          for (i = 0; i < n_cols; i++)
            {
              if (sqlite3_column_type(stmt, i) == SQLITE_NULL)
                continue;
              else if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
                  sprintf(xpath, "%d", sqlite3_column_int(stmt, i));
                  dummy = sqlite3_mprintf("%s", xpath);
                  strcpy(format, "FI0G");
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  sprintf(xpath, "%1.15f", sqlite3_column_double(stmt, i));
                  dummy = sqlite3_mprintf("%s", xpath);
                  strcpy(format, "P1;FF2G");
              } else if (sqlite3_column_type(stmt, i) == SQLITE_BLOB)
                {
                  dummy =
                    sqlite3_mprintf("BLOB sz=%d",
                                    sqlite3_column_bytes(stmt, i));
                  strcpy(format, "FG0G");
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  const char *str = (const char *) sqlite3_column_text(stmt, i);
                  dummy = (char *) sqlite3_malloc(strlen(str) + 1);
                  strcpy(dummy, str);
                  if (date_times && TestDateValue(dummy) == true)
                    {
                      sprintf(xpath, "%d", GetDateValue(dummy));
                      sqlite3_free(dummy);
                      dummy = sqlite3_mprintf("%s", xpath);
                      strcpy(format, "P2;FG0R");
                  } else if (date_times && TestDateTimeValue(dummy) == true)
                    {
                      sprintf(xpath, "%1.15f", GetDateTimeValue(dummy));
                      sqlite3_free(dummy);
                      dummy = sqlite3_mprintf("%s", xpath);
                      strcpy(format, "P4;FG0R");
                  } else if (date_times && TestTimeValue(dummy) == true)
                    {
                      sprintf(xpath, "%1.15f", GetTimeValue(dummy));
                      sqlite3_free(dummy);
                      dummy = sqlite3_mprintf("%s", xpath);
                      strcpy(format, "P3;FG0R");
                  } else
                    {
                      // plain text string
                      dummy2 = SylkQuote(dummy);
                      sqlite3_free(dummy);
                      if (!gaiaConvertCharset
                          (&dummy2, (const char *) "UTF-8", outCs))
                        goto encoding_error;
                      dummy = sqlite3_mprintf("%s", dummy2);
                      sqlite3_free(dummy2);
                      strcpy(format, "FG0G");
                    }
                }
              fprintf(out, "C;X%d;Y%d;K%s\r\n", i + 1, eff_row, dummy);
              sqlite3_free(dummy);
              fprintf(out, "F;X%d;Y%d;%s\r\n", i + 1, eff_row, format);
            }
          eff_row++;
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  fclose(out);
  sprintf(xpath, "Exported %d rows into SYLK spreadsheet", eff_row - 1);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump SYLK error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump SYLK: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

void MyFrame::ExportResultSetAsSylk(wxString & path, wxString & sql,
                                    wxString & charset, bool date_times)
{
//
// exporting a ResultSet as SYLK
//
  sqlite3_stmt *stmt = NULL;
  int ret;
  int i;
  int n_cols;
  int eff_row = 1;
  char format[16];
  char xpath[1024];
  char *dummy;
  char *dummy2;
  char outCs[128];
  wxString msg;
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  FILE *out = fopen(xpath, "wb");
  if (!out)
    goto no_file;
//
// compiling SQL prepared statement 
//
  dummy = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(dummy, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, dummy, strlen(dummy), &stmt, NULL);
  free(dummy);
  if (ret != SQLITE_OK)
    goto sql_error;
// SYLK header
  fprintf(out, "ID;PCALC001\r\n");
  fprintf(out, "P;PGeneral\r\n");
  fprintf(out, "P;P0.00\r\n");
  fprintf(out, "P;Pdd/mm/yyyy\r\n");
  fprintf(out, "P;Ph:mm:ss\r\n");
  fprintf(out, "P;Pdd/mm/yyyy\\ h:mm\r\n");
  while (1)
    {
      // exporting data
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (eff_row == 1)
            {
              // column titles
              for (i = 0; i < n_cols; i++)
                {
                  const char *str = sqlite3_column_name(stmt, i);
                  dummy = (char *) sqlite3_malloc(strlen(str) + 1);
                  strcpy(dummy, str);
                  dummy2 = SylkQuote(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&dummy2, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "C;X%d;Y%d;K%s\r\n", i + 1, eff_row, dummy2);
                  fprintf(out, "F;X%d;Y%d;FG0L\r\n", i + 1, eff_row);
                  sqlite3_free(dummy2);
                }
              eff_row++;
            }

          for (i = 0; i < n_cols; i++)
            {
              if (sqlite3_column_type(stmt, i) == SQLITE_NULL)
                continue;
              else if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
                  sprintf(xpath, "%d", sqlite3_column_int(stmt, i));
                  dummy = sqlite3_mprintf("%s", xpath);
                  strcpy(format, "FI0G");
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  sprintf(xpath, "%1.15f", sqlite3_column_double(stmt, i));
                  dummy = sqlite3_mprintf("%s", xpath);
                  strcpy(format, "P1;FF2G");
              } else if (sqlite3_column_type(stmt, i) == SQLITE_BLOB)
                {
                  dummy =
                    sqlite3_mprintf("BLOB sz=%d",
                                    sqlite3_column_bytes(stmt, i));
                  strcpy(format, "FG0G");
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  const char *str = (const char *) sqlite3_column_text(stmt, i);
                  dummy = (char *) sqlite3_malloc(strlen(str) + 1);
                  strcpy(dummy, str);
                  if (date_times && TestDateValue(dummy) == true)
                    {
                      sprintf(xpath, "%d", GetDateValue(dummy));
                      sqlite3_free(dummy);
                      dummy = sqlite3_mprintf("%s", xpath);
                      strcpy(format, "P2;FG0R");
                  } else if (date_times && TestDateTimeValue(dummy) == true)
                    {
                      sprintf(xpath, "%1.15f", GetDateTimeValue(dummy));
                      sqlite3_free(dummy);
                      dummy = sqlite3_mprintf("%s", xpath);
                      strcpy(format, "P4;FG0R");
                  } else if (date_times && TestTimeValue(dummy) == true)
                    {
                      sprintf(xpath, "%1.15f", GetTimeValue(dummy));
                      sqlite3_free(dummy);
                      dummy = sqlite3_mprintf("%s", xpath);
                      strcpy(format, "P3;FG0R");
                  } else
                    {
                      // plain text string
                      dummy2 = SylkQuote(dummy);
                      sqlite3_free(dummy);
                      if (!gaiaConvertCharset
                          (&dummy2, (const char *) "UTF-8", outCs))
                        goto encoding_error;
                      dummy = sqlite3_mprintf("%s", dummy2);
                      strcpy(format, "FG0G");
                      sqlite3_free(dummy2);
                    }
                }
              fprintf(out, "C;X%d;Y%d;K%s\r\n", i + 1, eff_row, dummy);
              sqlite3_free(dummy);
              fprintf(out, "F;X%d;Y%d;%s\r\n", i + 1, eff_row, format);
            }
          eff_row++;
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  fclose(out);
  sprintf(xpath, "Exported %d rows into SYLK spreadsheet", eff_row - 1);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump SYLK error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump SYLK: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

bool MyFrame::ExportHtmlColorSqlSyntax(FILE * out, wxString & sql, char *outCs)
{
//
// exporting SQL color syntax as HTML
//
  wxString right = sql;
  int from;
  int to = 0;
  int i;
  char c;
  char next_c;
  SqlTokenizer tokenizer(sql);
  while (tokenizer.HasMoreTokens())
    {
      wxString token = tokenizer.GetNextToken();
      from = to + right.Find(token);
      to = from + token.Len();
      // exporting any leading delimiter
      for (i = 0; i < right.Find(token); i++)
        {
          c = right.GetChar(i);
          switch (c)
            {
              case '\r':
                break;
              case '\n':
                fprintf(out, "<br>\n");
                break;
              case ' ':
                fprintf(out, "&nbsp;");
                break;
              case '\t':
                fprintf(out, "&nbsp;&nbsp;&nbsp;&nbsp;");
                break;
              default:
                fprintf(out, "%c", c);
                break;
            };
        }
      // extracting the unparsed portion of the SQL string
      right = sql.Mid(to);
      next_c = '\0';
      for (i = 0; i < (int) right.Len(); i++)
        {
          c = right.GetChar(i);
          if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
            continue;
          next_c = c;
          break;
        }
      char *cleaned;
      char *dummy = (char *) sqlite3_malloc((token.Len() * 4) + 1);
      strcpy(dummy, token.ToUTF8());
      if (gaiaIsReservedSqliteName(dummy))
        {
          // setting the SQL keyword style
          cleaned = CleanHtml(dummy);
          sqlite3_free(dummy);
          if (!gaiaConvertCharset(&cleaned, (const char *) "UTF-8", outCs))
            {
              sqlite3_free(cleaned);
              return false;
            }
          fprintf(out, "<font class=\"sql\">%s</font>", cleaned);
          sqlite3_free(cleaned);
      } else if (MyQueryView::IsSqliteExtra(token))
        {
          // setting the SQL keyword style
          cleaned = CleanHtml(dummy);
          sqlite3_free(dummy);
          if (!gaiaConvertCharset(&cleaned, (const char *) "UTF-8", outCs))
            {
              sqlite3_free(cleaned);
              return false;
            }
          fprintf(out, "<font class=\"sql\">%s</font>", cleaned);
          sqlite3_free(cleaned);
      } else if (MyQueryView::IsSqlString(token) == true)
        {
          // setting the SQL string constant style
          cleaned = CleanHtml(dummy);
          sqlite3_free(dummy);
          if (!gaiaConvertCharset(&cleaned, (const char *) "UTF-8", outCs))
            {
              sqlite3_free(cleaned);
              return false;
            }
          fprintf(out, "<font class=\"const\">%s</font>", cleaned);
          sqlite3_free(cleaned);
      } else if (MyQueryView::IsSqlNumber(token) == true)
        {
          // setting the SQL numeric constant style
          cleaned = CleanHtml(dummy);
          sqlite3_free(dummy);
          if (!gaiaConvertCharset(&cleaned, (const char *) "UTF-8", outCs))
            {
              sqlite3_free(cleaned);
              return false;
            }
          fprintf(out, "<font class=\"const\">%s</font>", cleaned);
          sqlite3_free(cleaned);
      } else if (MyQueryView::IsSqlFunction(token, next_c) == true)
        {
          // setting the SQL function style
          cleaned = CleanHtml(dummy);
          sqlite3_free(dummy);
          if (!gaiaConvertCharset(&cleaned, (const char *) "UTF-8", outCs))
            {
              sqlite3_free(cleaned);
              return false;
            }
          fprintf(out, "<font class=\"funct\">%s</font>", cleaned);
          sqlite3_free(cleaned);
      } else if (MyQueryView::IsSqlGeoFunction(token, next_c) == true)
        {
          // setting the SQL geo-function style
          cleaned = CleanHtml(dummy);
          sqlite3_free(dummy);
          if (!gaiaConvertCharset(&cleaned, (const char *) "UTF-8", outCs))
            {
              sqlite3_free(cleaned);
              return false;
            }
          fprintf(out, "<font class=\"funct\">%s</font>", dummy);
      } else if (MyQueryView::IsSqlRasterFunction(token, next_c) == true)
        {
          // setting the SQL raster-function style
          cleaned = CleanHtml(dummy);
          sqlite3_free(dummy);
          if (!gaiaConvertCharset(&cleaned, (const char *) "UTF-8", outCs))
            {
              sqlite3_free(cleaned);
              return false;
            }
          fprintf(out, "<font class=\"funct\">%s</font>", cleaned);
          sqlite3_free(cleaned);
      } else
        {
          // setting normal style
          cleaned = CleanHtml(dummy);
          sqlite3_free(dummy);
          if (!gaiaConvertCharset(&cleaned, (const char *) "UTF-8", outCs))
            {
              sqlite3_free(cleaned);
              return false;
            }
          fprintf(out, "<font class=\"normal\">%s</font>", cleaned);
          sqlite3_free(cleaned);
        }
    }
  fprintf(out, "\n");
  return true;
}

void MyFrame::ExportResultSetAsHtml(wxString & path, wxString & sql,
                                    wxString & sqlite_path, wxString & charset)
{
//
// exporting a ResultSet as HTML
//
  sqlite3_stmt *stmt = NULL;
  int ret;
  int rows = 0;
  int i;
  int n_cols;
  char xpath[1024];
  char sqlpath[1024];
  char *dummy;
  char outCs[128];
  wxString msg;
  int decimal_precision = GetDecimalPrecision();
  strcpy(outCs, charset.ToUTF8());
  strcpy(xpath, path.ToUTF8());
  strcpy(sqlpath, sqlite_path.ToUTF8());
  FILE *out = fopen(xpath, "w");
  if (!out)
    goto no_file;
  fprintf(out,
          "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
  fprintf(out, "<html>\n\t<head>\n");
  fprintf(out,
          "\t\t<meta http-equiv=\"content-type\" content=\"text/html; charset=%s\">\n",
          outCs);
  fprintf(out, "\t\t<title>\nResultSet from SQLite/SpatiaLite DB '%s'\n",
          sqlpath);
  fprintf(out, "\t\t</title>\n");
  fprintf(out, "\t\t<style type=\"text/css\">\n");
  fprintf(out, "table { border: 1px; }\n");
  fprintf(out, "tr.t0 th { background-color: #dfc9c9; }\n");
  fprintf(out, "tr.d0 td { background-color: #e0efe0; }\n");
  fprintf(out, "tr.d0 td.void { background-color: #ffffff; }\n");
  fprintf(out, "tr.d1 td { background-color: #d0d0df; }\n");
  fprintf(out, "tr.d1 td.void { background-color: #ffffff; }\n");
  fprintf(out, "td.sql { background-color: #e8e8e8; }\n");
  fprintf(out,
          "font.normal { font-family: \"Monospace\"; font-weight: 400; color: #000000; }\n");
  fprintf(out,
          "font.sql { font-family:\"Monospace\"; font-weight: 700; color: #0000ff; }\n");
  fprintf(out,
          "font.funct { font-family:\"Monospace\"; font-weight: 700; color: #c08000; }\n");
  fprintf(out,
          "font.const { font-family:\"Monospace\"; font-weight: 400; color: #ff00ff; }\n");
  fprintf(out, "\t\t</style>\n\t</head>\n\t<body>\n");
  fprintf(out, "\t\t<h2>ResultSet returned by SQL query statement:</h2>\n");
  fprintf(out,
          "\t\t<table cellpadding=\"4\" cellspacing=\"4\">\n\t\t\t<tr><td class=\"sql\">\n");
  if (ExportHtmlColorSqlSyntax(out, sql, outCs) == false)
    goto encoding_error;
  fprintf(out,
          "\t\t\t</td></tr>\n\t\t</table>\n\t\t<br>\n\t\t<hr>\n\t\t<table>\n");
//
// compiling SQL prepared statement 
//
  dummy = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(dummy, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, dummy, strlen(dummy), &stmt, NULL);
  free(dummy);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if ((rows % 20) == 0)
            {
              // outputting the column titles
              fprintf(out, "\t\t\t<tr class=\"t0\">\n");
              for (i = 0; i < n_cols; i++)
                {
                  char *cleaned;
                  const char *text =
                    (const char *) sqlite3_column_name(stmt, i);
                  int len = strlen(text);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  cleaned = CleanHtml(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&cleaned, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "\t\t\t\t<th>%s</th>\n", cleaned);
                  sqlite3_free(cleaned);
                }
              fprintf(out, "\t\t\t</tr>\n");
            }
          rows++;
          fprintf(out, "\t\t\t<tr class=\"%s\">\n", (rows % 2) ? "d0" : "d1");
          for (i = 0; i < n_cols; i++)
            {
              if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                {
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                  fprintf(out, "\t\t\t\t<td align=\"right\">%I64d</td>\n",
                          sqlite3_column_int64(stmt, i));
#else
                  fprintf(out, "\t\t\t\t<td align=\"right\">%lld</td>\n",
                          sqlite3_column_int64(stmt, i));
#endif
              } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                {
                  if (decimal_precision >= 0)
                    {
                      char fmt[64];
                      sprintf(fmt, "\t\t\t\t<td align=\"right\">%%1.%df</td>\n",
                              decimal_precision);
                      fprintf(out, fmt, sqlite3_column_double(stmt, i));
                  } else
                    fprintf(out, "\t\t\t\t<td align=\"right\">%1.6f</td>\n",
                            sqlite3_column_double(stmt, i));
              } else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                {
                  char *cleaned;
                  const char *text =
                    (const char *) sqlite3_column_text(stmt, i);
                  int len = sqlite3_column_bytes(stmt, i);
                  dummy = (char *) sqlite3_malloc(len + 1);
                  strcpy(dummy, text);
                  cleaned = CleanHtml(dummy);
                  sqlite3_free(dummy);
                  if (!gaiaConvertCharset
                      (&cleaned, (const char *) "UTF-8", outCs))
                    goto encoding_error;
                  fprintf(out, "\t\t\t\t<td>%s</td>\n", cleaned);
                  sqlite3_free(cleaned);
              } else
                {
                  /* NULL or BLOB */
                  fprintf(out, "\t\t\t\t<td class=\"void\"></td>\n");
                }
            }
          fprintf(out, "\t\t\t</tr>\n");
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  fprintf(out, "\t\t</table>\n\t</body>\n</html>\n");
  fclose(out);
  sprintf(xpath, "Exported %d rows into HTML file", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump HTML error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
encoding_error:
//
// some CHARSET conversion occurred
//
  sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump HTML: charset conversion reported an error"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  if (out)
    fclose(out);
  return;
no_file:
//
// output file can't be created/opened
//
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

void MyFrame::ExportResultSetAsDbf(wxString & path, wxString & sql,
                                   wxString & charset)
{
//
// exporting a ResultSet as DBF
//
  int rows;
  int i;
  char xpath[1024];
  char *xsql;
  sqlite3_stmt *stmt;
  int row1 = 0;
  int n_cols = 0;
  int offset = 0;
  int type;
  gaiaDbfPtr dbf = NULL;
  gaiaDbfListPtr dbf_export_list = NULL;
  gaiaDbfListPtr dbf_list = NULL;
  gaiaDbfListPtr dbf_write;
  gaiaDbfFieldPtr dbf_field;
  int *max_length = NULL;
  int *sql_type = NULL;
  char *dummy;
  int len;
  wxString msg;
  int ret;
//
// compiling SQL prepared statement 
//
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, xsql, strlen(xsql), &stmt, NULL);
  free(xsql);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  while (1)
    {
      //
      // Pass I - scrolling the result set to compute real DBF attributes' sizes and types 
      //
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          // processing a result set row 
          row1++;
          if (n_cols == 0)
            {
              // this one is the first row, so we are going to prepare the DBF Fields list 
              n_cols = sqlite3_column_count(stmt);
              dbf_export_list = gaiaAllocDbfList();
              max_length = (int *) malloc(sizeof(int) * n_cols);
              sql_type = (int *) malloc(sizeof(int) * n_cols);
              for (i = 0; i < n_cols; i++)
                {
                  // initializes the DBF export fields
                  const char *str = sqlite3_column_name(stmt, i);
                  dummy = (char *) malloc(strlen(str) + 1);
                  strcpy(dummy, str);
                  gaiaAddDbfField(dbf_export_list, dummy, '\0', 0, 0, 0);
                  free(dummy);
                  max_length[i] = 0;
                  sql_type[i] = SQLITE_NULL;
                }
            }
          for (i = 0; i < n_cols; i++)
            {
              // update the DBF export fields analyzing fetched data
              type = sqlite3_column_type(stmt, i);
              if (type == SQLITE_NULL || type == SQLITE_BLOB)
                continue;
              if (type == SQLITE_TEXT)
                {
                  const char *string =
                    (const char *) sqlite3_column_text(stmt, i);
                  char *converted = (char *) sqlite3_malloc(strlen(string) + 1);
                  strcpy(converted, string);
                  char xcharset[128];
                  strcpy(xcharset, charset.ToUTF8());
                  if (gaiaConvertCharset(&converted, "UTF-8", xcharset))
                    {
                      len = strlen(converted);
                      if (len > 254)
                        {
                          // DBF C: max allowed length
                          len = 254;
                        }
                      if (len > max_length[i])
                        max_length[i] = len;
                      sqlite3_free(converted);
                    }
                  sql_type[i] = SQLITE_TEXT;
              } else if (type == SQLITE_FLOAT && sql_type[i] != SQLITE_TEXT)
                sql_type[i] = SQLITE_FLOAT; // promoting a numeric column to be DOUBLE
              else if (type == SQLITE_INTEGER
                       && (sql_type[i] == SQLITE_NULL
                           || sql_type[i] == SQLITE_INTEGER))
                sql_type[i] = SQLITE_INTEGER; // promoting a null column to be INTEGER
              if (type == SQLITE_INTEGER && max_length[i] < 18)
                max_length[i] = 18;
              if (type == SQLITE_FLOAT && max_length[i] < 24)
                max_length[i] = 24;
            }
      } else
        goto sql_error;
    }
  if (!row1)
    goto empty_result_set;
  i = 0;
  offset = 0;
  dbf_list = gaiaAllocDbfList();
  dbf_field = dbf_export_list->First;
  while (dbf_field)
    {
      // preparing the final DBF attribute list 
      if (sql_type[i] == SQLITE_NULL || sql_type[i] == SQLITE_BLOB)
        {
          i++;
          dbf_field = dbf_field->Next;
          continue;
        }
      if (sql_type[i] == SQLITE_TEXT)
        {
          gaiaAddDbfField(dbf_list, dbf_field->Name, 'C', offset, max_length[i],
                          0);
          offset += max_length[i];
        }
      if (sql_type[i] == SQLITE_FLOAT)
        {
          gaiaAddDbfField(dbf_list, dbf_field->Name, 'N', offset, 24, 6);
          offset += 24;
        }
      if (sql_type[i] == SQLITE_INTEGER)
        {
          gaiaAddDbfField(dbf_list, dbf_field->Name, 'N', offset, 18, 0);
          offset += 18;
        }
      i++;
      dbf_field = dbf_field->Next;
    }
  free(max_length);
  free(sql_type);
  gaiaFreeDbfList(dbf_export_list);
  dbf_export_list = NULL;
// resetting SQLite query 
  ret = sqlite3_reset(stmt);
  if (ret != SQLITE_OK)
    goto sql_error;
// trying to open the DBF file 
  dbf = gaiaAllocDbf();
// xfering export-list ownership
  dbf->Dbf = dbf_list;
  dbf_list = NULL;
  strcpy(xpath, path.ToUTF8());
  gaiaOpenDbfWrite(dbf, xpath, "UTF-8", charset.ToUTF8());
  if (!(dbf->Valid))
    goto no_file;
  while (1)
    {
      // Pass II - scrolling the result set to dump data into DBF 
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          rows++;
          dbf_write = gaiaCloneDbfEntity(dbf->Dbf);
          for (i = 0; i < n_cols; i++)
            {
              dbf_field = GetDbfField(dbf_write, i);
              if (!dbf_field)
                continue;
              if (sqlite3_column_type(stmt, i) == SQLITE_NULL
                  || sqlite3_column_type(stmt, i) == SQLITE_BLOB)
                {
                  // handling NULL values
                  gaiaSetNullValue(dbf_field);
              } else
                {
                  switch (dbf_field->Type)
                    {
                      case 'N':
                        if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                          gaiaSetIntValue(dbf_field,
                                          sqlite3_column_int64(stmt, i));
                        else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                          gaiaSetDoubleValue(dbf_field,
                                             sqlite3_column_double(stmt, i));
                        else
                          gaiaSetNullValue(dbf_field);
                        break;
                      case 'C':
                        if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                          {
                            const char *str =
                              (const char *) sqlite3_column_text(stmt, i);
                            dummy = (char *) malloc(strlen(str) + 1);
                            strcpy(dummy, str);
                            gaiaSetStrValue(dbf_field, dummy);
                            free(dummy);
                        } else if (sqlite3_column_type(stmt, i) ==
                                   SQLITE_INTEGER)
                          {
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                            sprintf(xpath, "%I64d",
                                    sqlite3_column_int64(stmt, i));
#else
                            sprintf(xpath, "%lld",
                                    sqlite3_column_int64(stmt, i));
#endif
                            gaiaSetStrValue(dbf_field, xpath);
                        } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                          {
                            sprintf(xpath, "%1.6f",
                                    sqlite3_column_double(stmt, i));
                            gaiaSetStrValue(dbf_field, xpath);
                        } else
                          gaiaSetNullValue(dbf_field);
                        break;
                    };
                }
            }
          if (!gaiaWriteDbfEntity(dbf, dbf_write))
            {
              wxMessageBox(wxT("DBF write error"), wxT("spatialite_gui"),
                           wxOK | wxICON_INFORMATION, this);
            }
          gaiaFreeDbfList(dbf_write);
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  gaiaFlushDbfHeader(dbf);
  gaiaFreeDbf(dbf);
  sprintf(xpath, "Exported %d rows into the DBF file", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  sqlite3_finalize(stmt);
  if (dbf_export_list)
    gaiaFreeDbfList(dbf_export_list);
  if (dbf_list)
    gaiaFreeDbfList(dbf_list);
  if (dbf)
    gaiaFreeDbf(dbf);
  wxMessageBox(wxT("dump DBF file error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
no_file:
//
// DBF file can't be created/opened
//
  if (dbf_export_list)
    gaiaFreeDbfList(dbf_export_list);
  if (dbf_list)
    gaiaFreeDbfList(dbf_list);
  if (dbf)
    gaiaFreeDbf(dbf);
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
empty_result_set:
//
// the result set is empty - nothing to do 
//
  sqlite3_finalize(stmt);
  if (dbf_export_list)
    gaiaFreeDbfList(dbf_export_list);
  if (dbf_list)
    gaiaFreeDbfList(dbf_list);
  if (dbf)
    gaiaFreeDbf(dbf);
  wxMessageBox(wxT
               ("The SQL SELECT returned an empty result set\n... there is nothing to export ..."),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  return;
}

void MyFrame::DumpKml(wxString & path, wxString & table, wxString & column,
                      int precision, wxString & name, bool isNameConst,
                      wxString & desc, bool isDescConst)
{
//
// dumping a  geometry table as KML
//
  char *xtable;
  char *xtable2;
  char *xcolumn;
  char *xcolumn2;
  char *xsql;
  char *xname;
  char *xname2;
  char *xdesc;
  char *xdesc2;
  char *clean;
  char *clean2;
  char xpath[1024];
  sqlite3_stmt *stmt = NULL;
  FILE *out = NULL;
  int ret;
  int rows = 0;

// opening/creating the KML file
  strcpy(xpath, path.ToUTF8());
  out = fopen(xpath, "wb");
  if (!out)
    goto no_file;

//
// preparing SQL statement 
//
  xtable = (char *) malloc((table.Len() * 4) + 1);
  strcpy(xtable, table.ToUTF8());
  xtable2 = gaiaDoubleQuotedSql(xtable);
  free(xtable);
  xcolumn = (char *) malloc((column.Len() * 4) + 1);
  strcpy(xcolumn, column.ToUTF8());
  xcolumn2 = gaiaDoubleQuotedSql(xcolumn);
  free(xcolumn);
  if (isNameConst == true)
    {
      clean = (char *) malloc((name.Len() * 4) + 1);
      strcpy(clean, name.ToUTF8());
      clean2 = gaiaSingleQuotedSql(clean);
      free(clean);
      xname = sqlite3_mprintf("%Q", clean2);
      free(clean2);
  } else
    {
      xname = (char *) malloc((name.Len() * 4) + 1);
      strcpy(xname, name.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      xname = sqlite3_mprintf("\"%s\"", xname2);
      free(xname2);
    }
  if (isDescConst == true)
    {
      clean = (char *) malloc((desc.Len() * 4) + 1);
      strcpy(clean, desc.ToUTF8());
      clean2 = gaiaSingleQuotedSql(clean);
      free(clean);
      xdesc = sqlite3_mprintf("%Q", clean2);
      free(clean2);
  } else
    {
      xdesc = (char *) malloc((desc.Len() * 4) + 1);
      strcpy(xdesc, desc.ToUTF8());
      xdesc2 = gaiaDoubleQuotedSql(xdesc);
      free(xdesc);
      xdesc = sqlite3_mprintf("\"%s\"", xdesc2);
      free(xdesc2);
    }
  xsql = sqlite3_mprintf("SELECT AsKML(%s, %s, \"%s\", %d) FROM \"%s\" "
                         "WHERE \"%s\" IS NOT NULL", xname, xdesc,
                         xcolumn2, precision, xtable2, xcolumn2);
// excluding NULL Geometries 
  sqlite3_free(xname);
  sqlite3_free(xdesc);
  free(xcolumn2);
  free(xtable2);
//
// compiling SQL prepared statement 
//
  ret = sqlite3_prepare_v2(SqliteHandle, xsql, strlen(xsql), &stmt, NULL);
  sqlite3_free(xsql);
  if (ret != SQLITE_OK)
    goto sql_error;

  while (1)
    {
      //
      // scrolling the result set
      //
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          // processing a result set row 
          if (rows == 0)
            {
              fprintf(out, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
              fprintf(out,
                      "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\r\n");
              fprintf(out, "<Document>\r\n");
            }
          rows++;
          fprintf(out, "\t%s\r\n", sqlite3_column_text(stmt, 0));
      } else
        goto sql_error;
    }
  if (!rows)
    goto empty_result_set;


  fprintf(out, "</Document>\r\n");
  fprintf(out, "</kml>\r\n");
  sqlite3_finalize(stmt);
  fclose(out);
  return;

sql_error:
//
// some SQL error occurred
//
  if (stmt)
    sqlite3_finalize(stmt);
  if (out)
    fclose(out);
  sqlite3_finalize(stmt);
  wxMessageBox(wxT("dump KML error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
no_file:
//
// KML file can't be created/opened
//
  if (stmt)
    sqlite3_finalize(stmt);
  if (out)
    fclose(out);
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
empty_result_set:
//
// the result set is empty - nothing to do 
//

  if (stmt)
    sqlite3_finalize(stmt);
  if (out)
    fclose(out);
  wxMessageBox(wxT
               ("The SQL SELECT returned an empty result set\n... there is nothing to export ..."),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
}

void MyFrame::ExportResultSetAsShp(const void *proj_ctx, wxString & path,
                                   wxString & sql, wxString & charset)
{
//
// exporting a ResultSet as Shapefile
//
  int rows;
  int i;
  int shape = -1;
  char xpath[1024];
  char *xsql;
  sqlite3_stmt *stmt;
  int row1 = 0;
  int n_cols = 0;
  int offset = 0;
  int type;
  int dbf_col;
  const void *blob_value;
  gaiaShapefilePtr shp = NULL;
  gaiaDbfListPtr dbf_list = NULL;
  gaiaDbfListPtr dbf_write;
  gaiaDbfFieldPtr dbf_field;
  gaiaGeomCollPtr geom;
  char *dummy;
  int len;
  wxString msg;
  int ret;
  ResultSetShapefileAnalyzer analyzer;
  ResultSetShapefileColumn *pGeom;
//
// compiling SQL prepared statement 
//
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, xsql, strlen(xsql), &stmt, NULL);
  free(xsql);
  if (ret != SQLITE_OK)
    goto sql_error;
  rows = 0;
  while (1)
    {
      //
      // Pass I - scrolling the result set to compute real DBF attributes' sizes and types 
      //
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          // processing a result set row 
          row1++;
          if (n_cols == 0)
            {
              // this one is the first row, so we are going to prepare the DBF Fields list 
              n_cols = sqlite3_column_count(stmt);
              analyzer.Init(n_cols);
              for (i = 0; i < n_cols; i++)
                {
                  // initializes the column names
                  analyzer.SetColumnName(i, sqlite3_column_name(stmt, i));
                }
            }
          for (i = 0; i < n_cols; i++)
            {
              // update the DBF export fields analyzing fetched data
              type = sqlite3_column_type(stmt, i);
              if (type == SQLITE_BLOB)
                {
                  // 
                  // we need to check if this one actually corresponds to some Geometry
                  //
                  blob_value = sqlite3_column_blob(stmt, i);
                  len = sqlite3_column_bytes(stmt, i);
                  geom =
                    gaiaFromSpatiaLiteBlobWkb((unsigned char *) blob_value,
                                              len);
                  if (geom)
                    {
                      analyzer.UpdateGeometry(i, geom);
                      gaiaFreeGeomColl(geom);
                  } else
                    analyzer.UpdateBlob(i);
                }
              if (type == SQLITE_NULL)
                analyzer.UpdateNull(i);
              if (type == SQLITE_TEXT)
                {
                  const char *string =
                    (const char *) sqlite3_column_text(stmt, i);
                  char *converted = (char *) sqlite3_malloc(strlen(string) + 1);
                  strcpy(converted, string);
                  char xcharset[128];
                  strcpy(xcharset, charset.ToUTF8());
                  if (gaiaConvertCharset(&converted, "UTF-8", xcharset))
                    {
                      len = strlen(converted);
                      if (len > 254)
                        {
                          // DBF C: max allowed length
                          len = 254;
                        }
                      analyzer.UpdateText(i, len);
                      sqlite3_free(converted);
                    }
                }
              if (type == SQLITE_INTEGER)
                analyzer.UpdateInteger(i);
              if (type == SQLITE_FLOAT)
                analyzer.UpdateDouble(i);
            }
      } else
        goto sql_error;
    }
  if (!row1)
    goto empty_result_set;
  if (analyzer.Validate() == false)
    goto invalid_result_set;
  offset = 0;
  dbf_list = gaiaAllocDbfList();
  for (i = 0; i < analyzer.GetColumnCount(); i++)
    {
      // preparing the final DBF attribute list
      ResultSetShapefileColumn *pCol = analyzer.GetColumn(i);
      if (pCol == NULL)
        continue;
      if (pCol->GetDbfType() == SQLITE_TEXT)
        {
          gaiaAddDbfField(dbf_list, pCol->GetName(), 'C', offset,
                          pCol->GetMaxTextLen(), 0);
          offset += pCol->GetMaxTextLen();
        }
      if (pCol->GetDbfType() == SQLITE_FLOAT)
        {
          gaiaAddDbfField(dbf_list, pCol->GetName(), 'N', offset, 24, 6);
          offset += 24;
        }
      if (pCol->GetDbfType() == SQLITE_INTEGER)
        {
          gaiaAddDbfField(dbf_list, pCol->GetName(), 'N', offset, 18, 0);
          offset += 18;
        }
    }
  pGeom = analyzer.GetColumn(analyzer.GetGeometryColumn());
  if (pGeom == NULL)
    goto invalid_geometry;
  if (pGeom->GetFirst()->GetDims() == GAIA_XY_M)
    {
      switch (pGeom->GetFirst()->GetType())
        {
          case GAIA_POINT:
            shape = GAIA_POINTM;
            break;
          case GAIA_LINESTRING:
          case GAIA_MULTILINESTRING:
            shape = GAIA_LINESTRINGM;
            break;
          case GAIA_POLYGON:
          case GAIA_MULTIPOLYGON:
            shape = GAIA_POLYGONM;
            break;
          case GAIA_MULTIPOINT:
            shape = GAIA_MULTIPOINTM;
            break;
        };
    }
  if (pGeom->GetFirst()->GetDims() == GAIA_XY_Z)
    {
      switch (pGeom->GetFirst()->GetType())
        {
          case GAIA_POINT:
            shape = GAIA_POINTZ;
            break;
          case GAIA_LINESTRING:
          case GAIA_MULTILINESTRING:
            shape = GAIA_LINESTRINGZ;
            break;
          case GAIA_POLYGON:
          case GAIA_MULTIPOLYGON:
            shape = GAIA_POLYGONZ;
            break;
          case GAIA_MULTIPOINT:
            shape = GAIA_MULTIPOINTZ;
            break;
        };
    }
  if (pGeom->GetFirst()->GetDims() == GAIA_XY_Z_M)
    {
      switch (pGeom->GetFirst()->GetType())
        {
          case GAIA_POINT:
            shape = GAIA_POINTZM;
            break;
          case GAIA_LINESTRING:
          case GAIA_MULTILINESTRING:
            shape = GAIA_LINESTRINGZM;
            break;
          case GAIA_POLYGON:
          case GAIA_MULTIPOLYGON:
            shape = GAIA_POLYGONZM;
            break;
          case GAIA_MULTIPOINT:
            shape = GAIA_MULTIPOINTZM;
            break;
        };
    }
  if (pGeom->GetFirst()->GetDims() == GAIA_XY)
    {
      switch (pGeom->GetFirst()->GetType())
        {
          case GAIA_POINT:
            shape = GAIA_POINT;
            break;
          case GAIA_LINESTRING:
          case GAIA_MULTILINESTRING:
            shape = GAIA_LINESTRING;
            break;
          case GAIA_POLYGON:
          case GAIA_MULTIPOLYGON:
            shape = GAIA_POLYGON;
            break;
          case GAIA_MULTIPOINT:
            shape = GAIA_MULTIPOINT;
            break;
        };
    }
  if (shape < 0)
    goto invalid_geometry;
// resetting SQLite query 
  ret = sqlite3_reset(stmt);
  if (ret != SQLITE_OK)
    goto sql_error;
// trying to open shapefile files 
  shp = gaiaAllocShapefile();
  strcpy(xpath, path.ToUTF8());
  gaiaOpenShpWrite(shp, xpath, shape, dbf_list, "UTF-8", charset.ToUTF8());
  if (!(shp->Valid))
    goto no_file;
// trying to export the .PRJ file
  OutputPrjFile(proj_ctx, path, pGeom->GetFirst()->GetSRID());
  while (1)
    {
      // Pass II - scrolling the result set to dump data into shapefile 
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          rows++;
          geom = NULL;
          dbf_write = gaiaCloneDbfEntity(dbf_list);
          dbf_col = -1;
          for (i = 0; i < n_cols; i++)
            {
              if (strcasecmp
                  (pGeom->GetName(),
                   (char *) sqlite3_column_name(stmt, i)) == 0)
                {
                  // this one is the internal BLOB encoded GEOMETRY to be exported 
                  if (sqlite3_column_type(stmt, i) != SQLITE_BLOB)
                    {
                      // this one is a NULL Geometry
                      dbf_write->Geometry = NULL;
                  } else
                    {
                      blob_value = sqlite3_column_blob(stmt, i);
                      len = sqlite3_column_bytes(stmt, i);
                      dbf_write->Geometry =
                        gaiaFromSpatiaLiteBlobWkb((unsigned char *) blob_value,
                                                  len);
                    }
                  continue;
                }
              dbf_col++;
              dbf_field = GetDbfField(dbf_write, dbf_col);
              if (!dbf_field)
                continue;
              if (sqlite3_column_type(stmt, i) == SQLITE_NULL)
                {
                  // handling NULL values
                  gaiaSetNullValue(dbf_field);
              } else
                {
                  switch (dbf_field->Type)
                    {
                      case 'N':
                        if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                          gaiaSetIntValue(dbf_field,
                                          sqlite3_column_int64(stmt, i));
                        else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                          gaiaSetDoubleValue(dbf_field,
                                             sqlite3_column_double(stmt, i));
                        else
                          gaiaSetNullValue(dbf_field);
                        break;
                      case 'C':
                        if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                          {
                            const char *str =
                              (const char *) sqlite3_column_text(stmt, i);
                            dummy = (char *) malloc(strlen(str) + 1);
                            strcpy(dummy, str);
                            gaiaSetStrValue(dbf_field, dummy);
                            free(dummy);
                        } else if (sqlite3_column_type(stmt, i) ==
                                   SQLITE_INTEGER)
                          {
#if defined(_WIN32) || defined(__MINGW32__)
                            // CAVEAT - M$ runtime doesn't supports %lld for 64 bits
                            sprintf(xpath, "%I64d",
                                    sqlite3_column_int64(stmt, i));
#else
                            sprintf(xpath, "%lld",
                                    sqlite3_column_int64(stmt, i));
#endif
                            gaiaSetStrValue(dbf_field, xpath);
                        } else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                          {
                            sprintf(xpath, "%1.6f",
                                    sqlite3_column_double(stmt, i));
                            gaiaSetStrValue(dbf_field, xpath);
                        } else
                          gaiaSetNullValue(dbf_field);
                        break;
                    };
                }
            }
          if (!gaiaWriteShpEntity(shp, dbf_write))
            {
              wxMessageBox(wxT("Shapefile write error"), wxT("spatialite_gui"),
                           wxOK | wxICON_INFORMATION, this);
            }
          gaiaFreeDbfList(dbf_write);
      } else
        goto sql_error;
    }
  sqlite3_finalize(stmt);
  gaiaFlushShpHeaders(shp);
  gaiaFreeShapefile(shp);
  sprintf(xpath, "Exported %d rows into Shapefile", rows);
  msg = wxString::FromUTF8(xpath);
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  return;
sql_error:
//
// some SQL error occurred
//
  sqlite3_finalize(stmt);
  if (dbf_list)
    gaiaFreeDbfList(dbf_list);
  if (shp)
    gaiaFreeShapefile(shp);
  wxMessageBox(wxT("dump shapefile error:") +
               wxString::FromUTF8(sqlite3_errmsg(SqliteHandle)),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
no_file:
//
// shapefile can't be created/opened
//
  if (dbf_list)
    gaiaFreeDbfList(dbf_list);
  if (shp)
    gaiaFreeShapefile(shp);
  wxMessageBox(wxT("ERROR: unable to open '") + path + wxT("' for writing"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
empty_result_set:
//
// the result set is empty - nothing to do 
//
  sqlite3_finalize(stmt);
  if (dbf_list)
    gaiaFreeDbfList(dbf_list);
  if (shp)
    gaiaFreeShapefile(shp);
  wxMessageBox(wxT
               ("The SQL SELECT returned an empty result set\n... there is nothing to export ..."),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  return;
invalid_result_set:
//
// the result set is invalid - nothing to do 
//
  sqlite3_finalize(stmt);
  if (dbf_list)
    gaiaFreeDbfList(dbf_list);
  if (shp)
    gaiaFreeShapefile(shp);
  wxMessageBox(wxT
               ("The SQL SELECT returned an invalid result set\n... [not corresponding to the Shapefile format] ..."),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  return;
invalid_geometry:sqlite3_finalize(stmt);
  if (dbf_list)
    gaiaFreeDbfList(dbf_list);
  if (shp)
    gaiaFreeShapefile(shp);
  wxMessageBox(wxT("Invalid GeometryType"), wxT("spatialite_gui"),
               wxOK | wxICON_ERROR, this);
  return;
}

ResultSetShapefileColumn::ResultSetShapefileColumn()
{
// constructor
  Name = NULL;
  NullCount = 0;
  TextCount = 0;
  MaxTextLen = 0;
  IntCount = 0;
  DoubleCount = 0;
  BlobCount = 0;
  DbfType = -1;
  First = NULL;
  Last = NULL;
}

ResultSetShapefileColumn::~ResultSetShapefileColumn()
{
// destructor
  ResultSetShapefileGeometry *pG;
  ResultSetShapefileGeometry *pGn;
  pG = First;
  while (pG)
    {
      pGn = pG->GetNext();
      delete pG;
      pG = pGn;
    }
  if (Name)
    delete[]Name;
}

void ResultSetShapefileColumn::SetName(const char *name)
{
// setting the Column Name
  int len = strlen(name);
  if (Name)
    delete[]Name;
  Name = new char[len + 1];
  strcpy(Name, name);
}

void ResultSetShapefileColumn::UpdateGeometry(gaiaGeomCollPtr geom)
{
// updating Geometry stats
  ResultSetShapefileGeometry *pG = First;
  while (pG)
    {
      if (pG->GetType() == geom->DeclaredType
          && pG->GetDims() == geom->DimensionModel
          && pG->GetSRID() == geom->Srid)
        {
          // updating an existing Geometry class
          pG->Update();
          return;
        }
      pG = pG->GetNext();
    }
// inserting a new Geometry class
  pG =
    new ResultSetShapefileGeometry(geom->DeclaredType, geom->DimensionModel,
                                   geom->Srid);
  if (First == NULL)
    First = pG;
  if (Last != NULL)
    Last->SetNext(pG);
  Last = pG;
}

bool ResultSetShapefileColumn::Validate()
{
// validating a possible Shapefile column
  if (First == NULL)
    {
      // for sure, not a Geometry
      if (IntCount > 0 && DoubleCount == 0 && TextCount == 0 && BlobCount == 0)
        {
          // INTEGER
          DbfType = SQLITE_INTEGER;
          return true;
        }
      if (IntCount >= 0 && DoubleCount > 0 && TextCount == 0 && BlobCount == 0)
        {
          // DOUBLE
          DbfType = SQLITE_FLOAT;
          return true;
        }
      if (TextCount > 0)
        {
          DbfType = SQLITE_TEXT;
          if (IntCount > 0 && MaxTextLen < 18)
            MaxTextLen = 18;
          if (DoubleCount > 0 && MaxTextLen < 24)
            MaxTextLen = 24;
          return true;
        }
      DbfType = SQLITE_TEXT;
      MaxTextLen = 1;
      return true;
  } else
    {
      // this one is a Geometry
      if (First == Last)
        {
          switch (First->GetType())
            {
              case GAIA_POINT:
              case GAIA_LINESTRING:
              case GAIA_POLYGON:
              case GAIA_MULTIPOINT:
              case GAIA_MULTILINESTRING:
              case GAIA_MULTIPOLYGON:
                DbfType = SQLITE_BLOB;
                return true;
              default:
                return false;
            };
        }
      return false;
    }
  return false;
}

ResultSetShapefileAnalyzer::~ResultSetShapefileAnalyzer()
{
// destructor
  if (Columns)
    delete[]Columns;
}

void ResultSetShapefileAnalyzer::Init(int count)
{
// initializing columns
  if (Columns)
    delete[]Columns;
  ColumnCount = count;
  Columns = NULL;
  GeometryColumn = -1;
  if (count > 0)
    Columns = new ResultSetShapefileColumn[count];
}

void ResultSetShapefileAnalyzer::SetColumnName(int column, const char *name)
{
// setting the name corresponding to some column
  if (column >= 0 && column < ColumnCount)
    Columns[column].SetName(name);
}

void ResultSetShapefileAnalyzer::UpdateNull(int column)
{
// updating stats for some Column
  if (column >= 0 && column < ColumnCount)
    Columns[column].UpdateNull();
}

void ResultSetShapefileAnalyzer::UpdateText(int column, int len)
{
// updating stats for some Column
  if (column >= 0 && column < ColumnCount)
    Columns[column].UpdateText(len);
}

void ResultSetShapefileAnalyzer::UpdateInteger(int column)
{
// updating stats for some Column
  if (column >= 0 && column < ColumnCount)
    Columns[column].UpdateInteger();
}

void ResultSetShapefileAnalyzer::UpdateDouble(int column)
{
// updating stats for some Column
  if (column >= 0 && column < ColumnCount)
    Columns[column].UpdateDouble();
}

void ResultSetShapefileAnalyzer::UpdateBlob(int column)
{
// updating stats for some Column
  if (column >= 0 && column < ColumnCount)
    Columns[column].UpdateBlob();
}

void ResultSetShapefileAnalyzer::UpdateGeometry(int column,
                                                gaiaGeomCollPtr geom)
{
// updating stats for some Column
  if (column >= 0 && column < ColumnCount)
    Columns[column].UpdateGeometry(geom);
}

ResultSetShapefileColumn *ResultSetShapefileAnalyzer::GetColumn(int column)
{
// retrieving a given Column by index
  if (column >= 0 && column < ColumnCount)
    return Columns + column;
  return NULL;
}

bool ResultSetShapefileAnalyzer::Validate()
{
// validating the ResultSet as Shapefile
  int i;
  int geoms = 0;
  int idx = -1;
  for (i = 0; i < ColumnCount; i++)
    {
      if (Columns[i].Validate() == false)
        return false;
    }
  for (i = 0; i < ColumnCount; i++)
    {
      if (Columns[i].GetDbfType() == SQLITE_BLOB)
        {
          // this one is a valid Geometry column
          geoms++;
          idx = i;
        }
    }
  if (geoms != 1)
    return false;
  GeometryColumn = idx;
  return true;
}
