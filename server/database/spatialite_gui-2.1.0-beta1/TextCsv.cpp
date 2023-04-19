/*
/ TextCsv.cpp
/ methods related to CSV/TXT loading 
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

#if defined(_WIN32) && !defined(__MINGW32__)
#define strcasecmp	_stricmp
#endif

static void text_clean_integer(char *value)
{
// cleaning an integer value
  char last;
  char buffer[35536];
  int len = strlen(value);
  last = value[len - 1];
  if (last == '-' || last == '+')
    {
      // trailing sign; transforming into a leading sign
      *buffer = last;
      strcpy(buffer + 1, value);
      buffer[len - 1] = '\0';
      strcpy(value, buffer);
    }
}

static void text_clean_double(char *value)
{
// cleaning an integer value
  char *p;
  char last;
  char buffer[35536];
  int len = strlen(value);
  last = value[len - 1];
  if (last == '-' || last == '+')
    {
      // trailing sign; transforming into a leading sign
      *buffer = last;
      strcpy(buffer + 1, value);
      buffer[len - 1] = '\0';
      strcpy(value, buffer);
    }
  p = value;
  while (*p != '\0')
    {
      // transforming COMMAs into POINTs
      if (*p == ',')
        *p = '.';
      p++;
    }
}

void
  MyFrame::LoadText(wxString & path, wxString & table, wxString & charset,
                    bool first_titles, const char decimal_separator,
                    const char separator, const char text_separator)
{
//
// loading a CSV/TXT as a new DB table
//
  gaiaTextReaderPtr text = NULL;
  int seed;
  int dup;
  int idup;
  char **col_name = NULL;
  int i;
  char *sql;
  char *prevsql;
  int len;
  int ret;
  int rows = 0;
  char *errMsg = NULL;
  bool sqlError = false;
  char *xname;
  char *xname2;
  int current_row;
  wxString msg;
  char buf[4096];
  char dummy[4096];
  int type;
  const char *value;
//
// performing some checks before starting
//
  if (TableAlreadyExists(table) == true)
    {
      wxMessageBox(wxT("a table named '") + table + wxT("' already exists"),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      return;
    }
  text = gaiaTextReaderAlloc(path.ToUTF8(), separator,
                             text_separator, decimal_separator,
                             first_titles, charset.ToUTF8());
  if (text)
    {
      if (gaiaTextReaderParse(text) == 0)
        {
          gaiaTextReaderDestroy(text);
          text = NULL;
        }
    }
  if (!text)
    return;
  ::wxBeginBusyCursor();
//
// checking for duplicate / illegal column names and antialising them 
//
  col_name = (char **) malloc(sizeof(char *) * text->max_fields);
  seed = 0;
  for (i = 0; i < text->max_fields; i++)
    {
      xname = (char *) sqlite3_malloc(strlen(text->columns[i].name) + 1);
      strcpy(xname, text->columns[i].name);
      dup = 0;
      for (idup = 0; idup < i; idup++)
        {
          if (strcasecmp(xname, *(col_name + idup)) == 0)
            dup = 1;
        }
      if (strcasecmp(xname, "PK_UID") == 0)
        dup = 1;
      if (dup)
        {
          sqlite3_free(xname);
          xname = sqlite3_mprintf("DUPCOL_%d", seed++);
        }
      len = strlen(xname);
      *(col_name + i) = (char *) malloc(len + 1);
      strcpy(*(col_name + i), xname);
      sqlite3_free(xname);
    }
//
// starting a transaction
//
  ret = sqlite3_exec(SqliteHandle, "BEGIN", NULL, 0, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("load CSV/TXT error:") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      sqlError = true;
      goto clean_up;
    }
//
// creating the Table 
//
  xname = (char *) malloc((table.Len() * 4) + 1);
  strcpy(xname, table.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql = sqlite3_mprintf("CREATE TABLE \"%s\"", xname2);
  free(xname2);
  prevsql = sql;
  sql =
    sqlite3_mprintf("%s (\nPK_UID INTEGER PRIMARY KEY AUTOINCREMENT", prevsql);
  sqlite3_free(prevsql);
  prevsql = sql;
  for (i = 0; i < text->max_fields; i++)
    {
      sql = sqlite3_mprintf("%s,\n", prevsql);
      sqlite3_free(prevsql);
      prevsql = sql;
      xname = (char *) malloc(strlen(*(col_name + i)) + 1);
      strcpy(xname, *(col_name + i));
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql = sqlite3_mprintf("%s\"%s\"", prevsql, xname2);
      free(xname2);
      sqlite3_free(prevsql);
      prevsql = sql;
      if (text->columns[i].type == VRTTXT_INTEGER)
        sql = sqlite3_mprintf("%s INTEGER", prevsql);
      else if (text->columns[i].type == VRTTXT_DOUBLE)
        sql = sqlite3_mprintf("%s DOUBLE", prevsql);
      else
        sql = sqlite3_mprintf("%s TEXT", prevsql);
      sqlite3_free(prevsql);
      prevsql = sql;
    }
  sql = sqlite3_mprintf("%s)", prevsql);
  sqlite3_free(prevsql);
  ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("load text error:") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      sqlError = true;
      goto clean_up;
    }
  current_row = 0;
  while (current_row < text->num_rows)
    {
      //
      // inserting rows from CSV/TXT
      //
      if (!gaiaTextReaderGetRow(text, current_row))
        break;
      xname = (char *) malloc((table.Len() * 4) + 1);
      strcpy(xname, table.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql = sqlite3_mprintf("INSERT INTO \"%s\" (\nPK_UID", xname2);
      free(xname2);
      prevsql = sql;
      for (i = 0; i < text->max_fields; i++)
        {
          // columns corresponding to some CSV/TXT column
          sql = sqlite3_mprintf("%s,", prevsql);
          sqlite3_free(prevsql);
          prevsql = sql;
          xname = (char *) malloc(strlen(*(col_name + i)) + 1);
          strcpy(xname, *(col_name + i));
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          sql = sqlite3_mprintf("%s\"%s\"", prevsql, xname2);
          free(xname2);
          sqlite3_free(prevsql);
          prevsql = sql;
        }
      sql = sqlite3_mprintf("%s)\nVALUES (%d", prevsql, current_row);
      sqlite3_free(prevsql);
      prevsql = sql;
      for (i = 0; i < text->max_fields; i++)
        {
          // column values
          sql = sqlite3_mprintf("%s,", prevsql);
          sqlite3_free(prevsql);
          prevsql = sql;
          if (!gaiaTextReaderFetchField(text, i, &type, &value))
            {
              sql = sqlite3_mprintf("%sNULL", prevsql);
              sqlite3_free(prevsql);
              prevsql = sql;
          } else
            {
              if (type == VRTTXT_INTEGER)
                {
                  strcpy(buf, value);
                  text_clean_integer(buf);
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime has non-standard functions for 64 bits
                  sprintf(dummy, "%I64d", _atoi64(buf));
#else
                  sprintf(dummy, "%lld", atoll(buf));
#endif
                  sql = sqlite3_mprintf("%s%s", prevsql, dummy);
              } else if (type == VRTTXT_DOUBLE)
                {
                  strcpy(buf, value);
                  text_clean_double(buf);
                  sprintf(dummy, "%1.6f", atof(buf));
                  sql = sqlite3_mprintf("%s%s", prevsql, dummy);
              } else if (type == VRTTXT_TEXT)
                {
                  sql = sqlite3_mprintf("%s%Q", prevsql, value);
                  free((void *) value);
              } else
                sql = sqlite3_mprintf("%sNULL", prevsql);
              sqlite3_free(prevsql);
              prevsql = sql;
            }
        }
      sql = sqlite3_mprintf("%s)", prevsql);
      sqlite3_free(prevsql);
      ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
      sqlite3_free(sql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("load text error:") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          sqlError = true;
          goto clean_up;
        }
      rows++;
      current_row++;
    }
clean_up:
  if (col_name)
    {
      // releasing memory allocation for column names 
      for (i = 0; i < text->max_fields; i++)
        free(*(col_name + i));
      free(col_name);
    }
  gaiaTextReaderDestroy(text);
  if (sqlError == true)
    {
      // some error occurred - ROLLBACK 
      ret = sqlite3_exec(SqliteHandle, "ROLLBACK", NULL, 0, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("load text error:") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
        }
      ::wxEndBusyCursor();
      msg =
        wxT("CSV/TXT not loaded\n\n\na ROLLBACK was automatically performed");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  } else
    {
      // ok - confirming pending transaction - COMMIT 
      ret = sqlite3_exec(SqliteHandle, "COMMIT", NULL, 0, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("load text error:") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      ::wxEndBusyCursor();
      sprintf(dummy, "CSV/TXT loaded\n\n%d inserted rows", rows);
      msg = wxString::FromUTF8(dummy);
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      InitTableTree();
    }
}
