/*
/ ExportXLSX.cpp
/ exporting MS Excell spradsheets (xlsx)
/
/ version 2.1, 2020 May 9
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2020-2020  Alessandro Furieri
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

#ifdef ENABLE_XLSXWRITER        // only if XlsxWriter is supported

#include <xlsxwriter.h>

void MyFrame::DoExportXLSX(wxString & path, wxString & sql)
{
//
// attenting to export a ResultSet as a MS Excell Spreadsheet (xlsx)
//
  int ret;
  sqlite3_stmt *stmt = NULL;
  int error = 0;
  int i_col;
  int xlsx_row;
  lxw_workbook *workbook = NULL;
  lxw_worksheet *worksheet = NULL;
  lxw_format *fmt_title;
  lxw_format *fmt_text;
  lxw_format *fmt_integer;
  lxw_format *fmt_double;
  lxw_format *fmt_null;
  char *x_path;
  char *sql_stmt = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(sql_stmt, sql.ToUTF8());

// preparing the SQL statement
  ret =
    sqlite3_prepare_v2(SqliteHandle, sql_stmt, strlen(sql_stmt), &stmt, NULL);
  free(sql_stmt);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(SqliteHandle);
      wxMessageBox(wxT("XLSX SQL ERRORs: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }


  // preparing the XLSX objects
  x_path = (char *) malloc((path.Len() * 4) + 1);
  strcpy(x_path, path.ToUTF8());
  workbook = workbook_new(x_path);
  free(x_path);
  fmt_title = workbook_add_format(workbook);
  format_set_align(fmt_title, LXW_ALIGN_LEFT);
  format_set_bg_color(fmt_title, 0xFFFFDF);
  format_set_bold(fmt_title);
  format_set_border(fmt_title, LXW_BORDER_THIN);
  fmt_text = workbook_add_format(workbook);
  format_set_align(fmt_text, LXW_ALIGN_LEFT);
  format_set_border(fmt_text, LXW_BORDER_THIN);
  fmt_integer = workbook_add_format(workbook);
  format_set_num_format(fmt_integer, "#,##0");
  format_set_align(fmt_integer, LXW_ALIGN_RIGHT);
  format_set_border(fmt_integer, LXW_BORDER_THIN);
  fmt_double = workbook_add_format(workbook);
  format_set_num_format(fmt_double, "#,##0.00");
  format_set_align(fmt_double, LXW_ALIGN_RIGHT);
  format_set_border(fmt_double, LXW_BORDER_THIN);
  fmt_null = workbook_add_format(workbook);
  format_set_border(fmt_null, LXW_BORDER_THIN);
  worksheet = workbook_add_worksheet(workbook, "ResultSet");

  for (i_col = 0; i_col < sqlite3_column_count(stmt); i_col++)
    {
      // exporting column headers into the XLSX spreadsheet
      const char *col_name = sqlite3_column_name(stmt, i_col);
      worksheet_write_string(worksheet, 0, i_col, col_name, fmt_title);
    }
  xlsx_row = 1;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          // a new row starts here
          for (i_col = 0; i_col < sqlite3_column_count(stmt); i_col++)
            {
              // exporting column values into the XLSX spreadsheet
              const char *txt_value;
              int int_value;
              double dbl_value;
              int bytes;
              char blob[128];
              switch (sqlite3_column_type(stmt, i_col))
                {
                  case SQLITE_TEXT:
                    txt_value = (const char *) sqlite3_column_text(stmt, i_col);
                    worksheet_write_string(worksheet, xlsx_row, i_col,
                                           txt_value, fmt_text);
                    break;
                  case SQLITE_INTEGER:
                    int_value = sqlite3_column_int(stmt, i_col);
                    worksheet_write_number(worksheet, xlsx_row, i_col,
                                           int_value, fmt_integer);
                    break;
                  case SQLITE_FLOAT:
                    dbl_value = sqlite3_column_double(stmt, i_col);
                    worksheet_write_number(worksheet, xlsx_row, i_col,
                                           dbl_value, fmt_double);
                    break;
                  case SQLITE_BLOB:
                    bytes = sqlite3_column_bytes(stmt, i_col);
                    sprintf(blob, "BLOB [%d bytes]", bytes);
                    worksheet_write_string(worksheet, xlsx_row, i_col, blob,
                                           fmt_text);
                    break;
                  case SQLITE_NULL:
                  default:
                    worksheet_write_string(worksheet, xlsx_row, i_col, "",
                                           fmt_null);
                    break;
                };
            }
          xlsx_row++;
      } else
        {
          error = 1;
          goto stop;
        }
    }
stop:
  if (error)
    {
      wxString msg = wxT("Some unexpected error occurred\n\n"
                         "The output spreadsheet may easily be corrupted\n");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  } else
    {
      char *xmsg =
        sqlite3_mprintf("Exported %d rows into Spreadsheet", xlsx_row);
      wxString msg = wxString::FromUTF8(xmsg);
      sqlite3_free(xmsg);
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
    }
  workbook_close(workbook);
}

#endif // end XlsxWriter
