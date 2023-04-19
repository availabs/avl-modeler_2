/*
/ ResultSetView.cpp
/ a panel to show SQL query results
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

#include "wx/clipbrd.h"
#include "wx/filename.h"

#if defined(_WIN32) && !defined(__MINGW32__)
#include <windows.h>
#include <process.h>
#include <io.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif
#ifdef __MINGW32__
#include <unistd.h>
#include <pthread.h>
#endif

//
// ICONs in XPM format [universally portable]
//
#include "icons/rs_first.xpm"
#include "icons/rs_last.xpm"
#include "icons/rs_next.xpm"
#include "icons/rs_previous.xpm"
#include "icons/refresh.xpm"
#include "icons/filter.xpm"
#include "icons/map_preview.xpm"
#include "icons/map_magnify.xpm"

#if defined(_WIN32) || defined (__MINGW32__)
#define FORMAT_64	"%I64d"
#else
#define FORMAT_64	"%lld"
#endif

MyResultSetView::MyResultSetView(MyFrame * parent, wxWindowID id):
wxPanel(parent, id, wxDefaultPosition, wxSize(440, 80), wxBORDER_SUNKEN)
{
//
// constructor: a frame for SQL Result Sets
//
  ProgressTimer = NULL;
  RsBlock = 500;                // the ResultSet block size
  RowIds = new sqlite3_int64[RsBlock];
  ReadOnly = true;
  CoverageTiles = false;
  InsertRow = NULL;
  InsertPending = false;
  MainFrame = parent;
  BtnRsFirst =
    new wxBitmapButton(this, ID_RS_FIRST, wxBitmap(rs_first_xpm),
                       wxPoint(5, 80), wxSize(32, 32));
  BtnRsFirst->SetToolTip(wxT("ResultSet: go to first row"));
  BtnRsPrevious =
    new wxBitmapButton(this, ID_RS_PREVIOUS, wxBitmap(rs_previous_xpm),
                       wxPoint(55, 80), wxSize(32, 32));
  BtnRsPrevious->SetToolTip(wxT("ResultSet: go to previous block"));
  BtnRefresh =
    new wxBitmapButton(this, ID_REFRESH, wxBitmap(refresh_xpm),
                       wxPoint(55, 80), wxSize(32, 32));
  BtnRefresh->SetToolTip(wxT("ResultSet: refresh"));
  BtnRsNext =
    new wxBitmapButton(this, ID_RS_NEXT, wxBitmap(rs_next_xpm),
                       wxPoint(105, 80), wxSize(32, 32));
  BtnRsNext->SetToolTip(wxT("ResultSet: go to next block"));
  BtnRsLast =
    new wxBitmapButton(this, ID_RS_LAST, wxBitmap(rs_last_xpm),
                       wxPoint(155, 80), wxSize(32, 32));
  BtnRsLast->SetToolTip(wxT("ResultSet: go to last row"));
  RsCurrentBlock =
    new wxStaticText(this, ID_RS_BLOCK, wxT(""), wxPoint(210, 400),
                     wxSize(200, 18));
  BtnRsMapShow =
    new wxBitmapButton(this, ID_RS_MAP_SHOW, wxBitmap(map_preview_xpm),
                       wxPoint(305, 80), wxSize(32, 32));
  BtnRsMapShow->SetToolTip(wxT("Map Panel: Show Feature"));
  BtnRsMapZoom =
    new wxBitmapButton(this, ID_RS_MAP_ZOOM, wxBitmap(map_magnify_xpm),
                       wxPoint(355, 80), wxSize(32, 32));
  BtnRsMapZoom->SetToolTip(wxT("Map Panel: Zoom to Feature"));
  BtnRsMapShow->Enable(false);
  BtnRsMapZoom->Enable(false);
  TableView = NULL;
  TableBlobs = NULL;
  TableValues = NULL;
  CurrentBlob = NULL;
// setting up event handlers
  Connect(wxID_ANY, wxEVT_GRID_LABEL_LEFT_CLICK,
          (wxObjectEventFunction) & MyResultSetView::OnMapFeatureSelect);
  Connect(ID_RS_FIRST, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyResultSetView::OnRsFirst);
  Connect(ID_RS_PREVIOUS, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyResultSetView::OnRsPrevious);
  Connect(ID_RS_NEXT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyResultSetView::OnRsNext);
  Connect(ID_RS_LAST, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyResultSetView::OnRsLast);
  Connect(ID_RS_MAP_SHOW, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyResultSetView::OnRsMapShow);
  Connect(ID_RS_MAP_ZOOM, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyResultSetView::OnRsMapZoom);
  Connect(ID_REFRESH, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyResultSetView::OnRefresh);
  Connect(ID_RS_THREAD_FINISHED, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyResultSetView::OnThreadFinished);
  Connect(ID_RS_STATS_UPDATE, wxEVT_TIMER,
          wxTimerEventHandler(MyResultSetView::OnTimerStatistics), NULL, this);
  Connect(wxID_ANY, wxEVT_SIZE,
          (wxObjectEventFunction) & MyResultSetView::OnSize);
  Connect(wxID_ANY, wxEVT_GRID_SELECT_CELL,
          (wxObjectEventFunction) & MyResultSetView::OnCellSelected);
  Connect(wxID_ANY, wxEVT_GRID_CELL_RIGHT_CLICK,
          (wxObjectEventFunction) & MyResultSetView::OnRightClick);
  Connect(wxID_ANY, wxEVT_GRID_CELL_CHANGE,
          (wxObjectEventFunction) & MyResultSetView::OnCellChanged);
  Connect(Grid_Delete, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdDelete);
  Connect(Grid_TilePreview, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdTilePreview);
  Connect(Grid_Insert, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdInsert);
  Connect(Grid_Abort, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdAbort);
  Connect(Grid_Clear, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdClearSelection);
  Connect(Grid_All, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdSelectAll);
  Connect(Grid_Row, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdSelectRow);
  Connect(Grid_Column, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdSelectColumn);
  Connect(Grid_Copy, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdCopy);
  Connect(Grid_Blob, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdBlob);
  Connect(Grid_BlobIn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdBlobIn);
  Connect(Grid_BlobOut, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdBlobOut);
  Connect(Grid_BlobNull, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdBlobNull);
  Connect(Grid_XmlBlobIn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdXmlBlobIn);
  Connect(Grid_XmlBlobOut, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdXmlBlobOut);
  Connect(Grid_XmlBlobOutIndented, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdXmlBlobOutIndented);
  Connect(Grid_ExpTxtTab, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdExpTxtTab);
  Connect(Grid_ExpCsv, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdExpCsv);
  Connect(Grid_ExpHtml, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdExpHtml);
  Connect(Grid_ExpShp, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdExpShp);
  Connect(Grid_ExpDif, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdExpDif);
  Connect(Grid_ExpSylk, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdExpSylk);
  Connect(Grid_ExpDbf, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdExpDbf);
  Connect(Grid_ExpXlsx, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdExpXlsx);
  Connect(Grid_Filter, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnCmdFilter);
  Connect(Grid_MapShow, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnRsMapShow);
  Connect(Grid_MapZoom, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyResultSetView::OnRsMapZoom);
}

MyResultSetView::~MyResultSetView()
{
// destructor
  if (ProgressTimer)
    {
      ProgressTimer->Stop();
      delete ProgressTimer;
    }
  if (RowIds)
    delete[]RowIds;
  if (InsertRow)
    delete InsertRow;
  if (TableBlobs)
    delete TableBlobs;
  if (TableValues)
    delete TableValues;
}

void MyResultSetView::ShowControls()
{
//
// making all ResultSet controls to be visible
//
  BtnRsFirst->Show(true);
  BtnRsPrevious->Show(true);
  BtnRsNext->Show(true);
  BtnRsLast->Show(true);
  BtnRefresh->Show(true);
  if (MainFrame->IsMapLinkedResultSet() == true)
    {
      BtnRsMapShow->Show(true);
      BtnRsMapZoom->Show(true);
  } else
    {
      BtnRsMapShow->Show(false);
      BtnRsMapZoom->Show(false);
    }
}

void MyResultSetView::HideControls()
{
//
// making all controls to be invisible
//
  BtnRsFirst->Show(false);
  BtnRsPrevious->Show(false);
  BtnRsNext->Show(false);
  BtnRsLast->Show(false);
  BtnRefresh->Show(false);
  BtnRsMapShow->Show(false);
  BtnRsMapZoom->Show(false);
}

void MyResultSetView::EditTable(wxString & sql, int *primaryKeys, int *blobCols,
                                wxString & table)
{
//
// starting the edit table sequence
//
  int i;
  for (i = 0; i < 1024; i++)
    {
      PrimaryKeys[i] = *(primaryKeys + i);
      BlobColumns[i] = *(blobCols + i);
    }
  ReadOnly = false;
  TableName = table;
  MainFrame->GetQueryView()->EnableFilterButton();
  MainFrame->GetQueryView()->GetSqlCtrl()->SetValue(sql);
  if (ExecuteSqlPre
      (sql, 0, ReadOnly, false, TileDataDbPrefix, TileDataTable, true) == false)
    wxMessageBox(SqlErrorMsg, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                 MainFrame);
}

void MyResultSetView::FormatElapsedTime(double seconds, char *elapsed,
                                        bool simple)
{
// well formatting elapsed time
  int int_time = (int) seconds;
  int millis = (int) ((seconds - (double) int_time) * 1000.0);
  int secs = int_time % 60;
  int_time /= 60;
  int mins = int_time % 60;
  int_time /= 60;
  int hh = int_time;
  if (simple == true)
    {
      if (hh == 0 && mins == 0)
        sprintf(elapsed, "%d.%03d", secs, millis);
      else if (hh == 0)
        sprintf(elapsed, "%d:%02d.%03d", mins, secs, millis);
      else
        sprintf(elapsed, "%d:%02d:%02d.%03d", hh, mins, secs, millis);
  } else
    sprintf(elapsed, "%02d:%02d:%02d.%03d", hh, mins, secs, millis);
}

void SqlThreadParams::Reset(void)
{
//
// thread params: resetting to empty state
//
  Mother = NULL;
  Sql = wxT("");
  Stmt = NULL;
  FromRow = 0;
  EndRow = 0;
  MaxRow = 0;
  List.Reset();
  Sqlite = NULL;
  Start = 0;
  Error = false;
  FetchedRows = 0;
  StatFullscanStep = 0;
  StatSort = 0;
  StatAutoindex = 0;
  ElapsedTime = 0.0;
  AbortRequested = false;
  Valid = false;
}

void SqlThreadParams::Initialize(MyResultSetView * mother, wxString & sql,
                                 sqlite3_stmt * stmt, int from,
                                 sqlite3 * sqlite, clock_t start)
{
//
// thread params: initializing
//
  Reset();
  Mother = mother;
  Sql = sql;
  Stmt = stmt;
  FromRow = from;
  EndRow = 0;
  MaxRow = 0;
  Sqlite = sqlite;
  Start = start;
  Error = false;
  FetchedRows = 0;
  StatFullscanStep = 0;
  StatSort = 0;
  StatAutoindex = 0;
  ElapsedTime = 0.0;
  AbortRequested = false;
  Valid = true;
}

void SqlThreadParams::UpdateStats(int fullscan, int sort, int autoindex,
                                  clock_t now)
{
//
// updading Stats (SQL query progress handler callback)
//
  StatFullscanStep = fullscan;
  StatSort = sort;
  StatAutoindex = autoindex;
  ElapsedTime = (double) (now - Start) / (double) CLOCKS_PER_SEC;
}

int SqlProgressCallback(void *arg)
{
//
// SQL query progress handler callback function
//
  int fullscan;
  int sort;
  int autoindex;
  clock_t clock_end;
  wxString currentBlock;
  SqlThreadParams *params = (SqlThreadParams *) arg;

  if (params->IsValid() == false)
    return 0;

  if (params->IsAbortRequested() == true)
    {
      // aborting the SQL query
      sqlite3_interrupt(params->GetSqlite());
      return 0;
    }
// updating query stats
  if (params->GetStmt() != NULL)
    {
      fullscan =
        sqlite3_stmt_status(params->GetStmt(), SQLITE_STMTSTATUS_FULLSCAN_STEP,
                            0);
      sort = sqlite3_stmt_status(params->GetStmt(), SQLITE_STMTSTATUS_SORT, 0);
#ifdef OMIT_SQLITE_STMTSTATUS_AUTOINXED
      autoindex = -1;
#else
      autoindex =
        sqlite3_stmt_status(params->GetStmt(), SQLITE_STMTSTATUS_AUTOINDEX, 0);
#endif
    }
  clock_end = clock();
  params->UpdateStats(fullscan, sort, autoindex, clock_end);
  return 0;
}

#if defined(_WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteSqlThread(void *arg)
#else
void *DoExecuteSqlThread(void *arg)
#endif
{
//
// threaded function: processing an SQL query
//
  int ret;
  int i_col;
  int i_row = 0;
  int end_row = 0;
  int columns;
  char err_msg[2048];

#ifdef __MINGW32__
  wxMilliSleep(250);
#endif

#ifndef _WIN32
  pthread_t th_id = pthread_self();
  int policy;
  struct sched_param sched;
  if (pthread_getschedparam(th_id, &policy, &sched) == 0)
    {
      if (policy == SCHED_OTHER && sched.sched_priority == 0)
        {
          // setting a lower priority
          nice(10);
        }
    }
#endif

// setting up the sqlite's progress handler callback
  SqlThreadParams *params = (SqlThreadParams *) arg;
  sqlite3_progress_handler(params->GetSqlite(), 1000, SqlProgressCallback, arg);

  while (1)
    {
      //
      // fetching the result set rows 
      //
      if (params->IsValid() == false)
        goto error;
      ret = sqlite3_step(params->GetStmt());
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          //
          // fetching a row
          //
          if (params->IsAbortRequested() == true)
            {
              // aborting the SQL query
              sqlite3_interrupt(params->GetSqlite());
              continue;
            }

          params->FetchedRow();
          if ((params->GetFetchedRows() % 1000) == 0)
            SqlProgressCallback(params);
          if (i_row < params->GetFromRow())
            {
              i_row++;
              continue;
            }
          if ((i_row - params->GetFromRow()) >=
              params->GetMother()->GetRsBlock())
            break;
          end_row = i_row;
          columns = sqlite3_column_count(params->GetStmt());
          MyRowVariant *rowVariant = params->GetList()->Add(columns);
          for (i_col = 0; i_col < columns; i_col++)
            {
              sqlite3_int64 int_value;
              double dbl_value;
              const unsigned char *txt_value;
              const void *blob_value;
              int blobSize;
              params->GetList()->SetColumnName(i_col,
                                               sqlite3_column_name
                                               (params->GetStmt(), i_col));
              switch (sqlite3_column_type(params->GetStmt(), i_col))
                {
                  case SQLITE_INTEGER:
                    int_value = sqlite3_column_int64(params->GetStmt(), i_col);
                    rowVariant->Set(i_col, int_value);
                    break;
                  case SQLITE_FLOAT:
                    dbl_value = sqlite3_column_double(params->GetStmt(), i_col);
                    rowVariant->Set(i_col, dbl_value);
                    break;
                  case SQLITE_TEXT:
                    txt_value = sqlite3_column_text(params->GetStmt(), i_col);
                    rowVariant->Set(i_col, txt_value);
                    break;
                  case SQLITE_BLOB:
                    blob_value = sqlite3_column_blob(params->GetStmt(), i_col);
                    blobSize = sqlite3_column_bytes(params->GetStmt(), i_col);
                    rowVariant->Set(i_col, blob_value, blobSize);
                    break;
                  case SQLITE_NULL:
                  default:
                    break;
                };
            }
          i_row++;
      } else
        {
          sprintf(err_msg, "SQL error: %s",
                  sqlite3_errmsg(params->GetSqlite()));
          wxString msg = wxString::FromUTF8(err_msg);
          params->GetMother()->SetSqlErrorMsg(msg);
          goto error;
        }
    }

  params->Finalize();
  params->SetEndRow(end_row);
  params->SetMaxRow(i_row);
  goto ok;
error:
  params->Finalize();
  params->SetError();
ok:
  wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_RS_THREAD_FINISHED);
  params->GetMother()->GetEventHandler()->AddPendingEvent(event);
#if defined(_WIN32) && !defined(__MINGW32__)
  return 0;
#else
  pthread_exit(NULL);
  return NULL;
#endif
}

void MyResultSetView::AbortRequested()
{
//
// attempting to abort the current SQL query
//
  if (ThreadParams.IsValid() == true)
    ThreadParams.Abort();
}

const char *MyResultSetView::CleanSqlTail(const char *dirty)
{
// strips any leading white-space
  const char *p = dirty;
  while (*p != '\0')
    {
      if (*p == ' ')
        {
          p++;
          continue;
        }
      if (*p == '\t')
        {
          p++;
          continue;
        }
      if (*p == '\n')
        {
          p++;
          continue;
        }
      if (*p == '\r')
        {
          p++;
          continue;
        }
      break;
    }
  return p;
}

void MyResultSetView::UpdateMaxRow(wxString & sql)
{
//
// updating the max row value
//
  char *xSql = NULL;
  char err_msg[2048];
  sqlite3 *sqlite = MainFrame->GetSqlite();
  sqlite3_stmt *stmt;
  int i_row = 0;
  int ret;

  if (IsMaxAlreadySet)
    return;
  xSql = new char[(sql.Len() * 4) + 1];
  strcpy(xSql, sql.ToUTF8());
  const char *pSql = xSql;

  ::wxBeginBusyCursor();
  while (1)
    {
      const char *sql_tail;
      ret = sqlite3_prepare_v2(sqlite, pSql, strlen(pSql), &stmt, &sql_tail);
      if (ret != SQLITE_OK)
        {
          sprintf(err_msg, "SQL error: %s", sqlite3_errmsg(sqlite));
          SqlErrorMsg = wxString::FromUTF8(err_msg);
          ::wxEndBusyCursor();
          return;
        }
      pSql = CleanSqlTail(sql_tail);
      if (strlen(pSql) == 0)
        {
          // this is the latest SQL statement in a (possibly) multiple
          // request; enabling GUI processing of the resultset
          break;
        }
      // silently consuming an intermediate SQL statement
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE || ret == SQLITE_ROW)
        ;
      else
        {
          sprintf(err_msg, "SQL error: %s", sqlite3_errmsg(sqlite));
          SqlErrorMsg = wxString::FromUTF8(err_msg);
          ::wxEndBusyCursor();
          sqlite3_finalize(stmt);
          return;
        }
      sqlite3_finalize(stmt);
    }
  delete[]xSql;

  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        i_row++;
    }
  sqlite3_finalize(stmt);
  RsMaxRow = i_row;
  ::wxEndBusyCursor();
  IsMaxAlreadySet = true;
}

char *MyResultSetView::ConsumeSqlEmptyLine(char *sql)
{
//
// consuming an eventual SQL empty line
//
  bool empty = true;
  char *p = sql;
  while (1)
    {
      // checking all bytes until the end of the line
      if (*p == '\0')
        break;
      if (*p == '\n')
        {
          p++;
          break;
        }
      if (*p == ' ' || *p == '\t' || *p == '\r')
        ;
      else
        {
          empty = false;
          break;
        }
      p++;
    }
  if (empty == false)
    return NULL;

  for (char *c = sql; c < p; c++)
    *c = ' ';
  return p;
}

char *MyResultSetView::ConsumeSqlComment(char *sql, bool mode)
{
//
// consuming an eventual SQL comment
//
  int len = strlen(sql);
  if (len > 0)
    {
      char *blank = ConsumeSqlEmptyLine(sql);
      if (blank != NULL)
        return blank;
    }

  char *p = sql;
  while (1)
    {
      // consuming all whitespaces
      if (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
        {
          p++;
          continue;
        }
      break;
    }
  if (len < 2)
    return NULL;
  len = strlen(p);
  if (*(p + 0) == '-' && *(p + 1) == '-')
    ;                           // SQL comment marker found
  else
    return NULL;

  while (1)
    {
      // consuming all bytes until the end of the line
      if (*p == '\0')
        break;
      if (*p == '\n')
        {
          p++;
          break;
        }
      if (mode == true)
        *p = ' ';
      p++;
    }
  return p;
}

bool MyResultSetView::ExecuteSqlPre(wxString & sql, int from, bool read_only,
                                    bool coverage_tile_data,
                                    wxString & tile_data_db_prefix,
                                    wxString & tile_data_table, bool reset)
{
//
// executing some SQL statement
//
  ReadOnly = read_only;
  CoverageTiles = false;
  if (coverage_tile_data == true)
    {
      ReadOnly = true;
      CoverageTiles = true;
      TileDataDbPrefix = tile_data_db_prefix;
      TileDataTable = tile_data_table;
    }
  char *xSql = NULL;
  char err_msg[2048];
  int i_row;
  clock_t clock_start;
  sqlite3_stmt *stmt = NULL;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  SqlErrorMsg = wxT("");
  if (TableView)
    TableView->Destroy();
  TableView = NULL;
  if (TableBlobs)
    delete TableBlobs;
  if (TableValues)
    delete TableValues;
  TableBlobs = NULL;
  TableValues = NULL;
  CurrentBlob = NULL;
  HideControls();
  RsBeginRow = 0;
  RsEndRow = 0;
  if (reset == true)
    {
      RsMaxRow = 0;
      IsMaxAlreadySet = false;
    }
#if defined(_WIN32) && !defined(__MINGW32__)
  HANDLE thread_handle;
  DWORD dwThreadId;
#else
  pthread_t thread_id;
#endif
  for (i_row = 0; i_row < RsBlock; i_row++)
    RowIds[i_row] = -1;
  i_row = 0;
  xSql = new char[(sql.Len() * 4) + 1];
  strcpy(xSql, sql.ToUTF8());
  char *pSql = xSql;

  clock_start = clock();
  ::wxBeginBusyCursor();
  while (1)
    {
      const char *sql_tail;
      int ret;

      while (1)
        {
          // consuming SQL comments BEFORE 
          char *ptr = ConsumeSqlComment(pSql, false);
          if (ptr == NULL)
            break;
          pSql = ptr;
        }
      if (strlen(pSql) == 0)
        {
          SqlErrorMsg = wxString::FromUTF8("Empty SQL Statement");
          ::wxEndBusyCursor();
          delete[]xSql;
          return false;
        }
      ret = sqlite3_prepare_v2(sqlite, pSql, strlen(pSql), &stmt, &sql_tail);
      if (ret != SQLITE_OK)
        {
          sprintf(err_msg, "SQL error: %s", sqlite3_errmsg(sqlite));
          SqlErrorMsg = wxString::FromUTF8(err_msg);
          ::wxEndBusyCursor();
          delete[]xSql;
          return false;
        }
      char *p2 = (char *) sql_tail;
      while (1)
        {
          // consuming SQL comments AFTER
          char *ptr = ConsumeSqlComment(p2, true);
          if (ptr == NULL)
            break;
          p2 = ptr;
        }
      pSql = (char *) CleanSqlTail(sql_tail);
      if (strlen(pSql) == 0)
        {
          // this is the latest SQL statement in a (possibly) multiple
          // request; enabling GUI processing of the resultset
          break;
        }
      while (1)
        {
          // silently consuming an intermediate SQL statement
          ret = sqlite3_step(stmt);
          if (ret == SQLITE_DONE)
            break;
          else if (ret == SQLITE_ROW)
            ;
          else
            {
              sprintf(err_msg, "SQL error: %s", sqlite3_errmsg(sqlite));
              SqlErrorMsg = wxString::FromUTF8(err_msg);
              ::wxEndBusyCursor();
              sqlite3_finalize(stmt);
              delete[]xSql;
              return false;
            }
        }
      sqlite3_finalize(stmt);
      stmt = NULL;
    }
  delete[]xSql;
  CreateStatsGrid();
  MainFrame->DisableAllTools();

  ThreadParams.Initialize(this, sql, stmt, from, sqlite, clock_start);
  if (!ProgressTimer)
    ProgressTimer = new wxTimer(this, ID_RS_STATS_UPDATE);
  ProgressTimer->Start(125, wxTIMER_ONE_SHOT);
#if defined(_WIN32) && !defined(__MINGW32__)
  thread_handle =
    CreateThread(NULL, 0, DoExecuteSqlThread, &ThreadParams, 0, &dwThreadId);
  SetThreadPriority(thread_handle, THREAD_PRIORITY_IDLE);
#else
  int ok_prior = 0;
  int policy;
  int min_prio;
  pthread_attr_t attr;
  struct sched_param sp;
  pthread_attr_init(&attr);
  if (pthread_attr_setschedpolicy(&attr, SCHED_RR) == 0)
    {
      // attempting to set the lowest priority  
      if (pthread_attr_getschedpolicy(&attr, &policy) == 0)
        {
          min_prio = sched_get_priority_min(policy);
          sp.sched_priority = min_prio;
          if (pthread_attr_setschedparam(&attr, &sp) == 0)
            {
              // ok, setting the lowest priority  
              ok_prior = 1;
              pthread_create(&thread_id, &attr, DoExecuteSqlThread,
                             &ThreadParams);
              pthread_detach(thread_id);
            }
        }
    }
#ifdef __MINGW32__
  if (!ok_prior)
    {
      // attempting to set the lowest priority on Windows
      if (pthread_attr_getschedpolicy(&attr, &policy) == 0)
        {
          if (policy == SCHED_OTHER)
            {
              min_prio = sched_get_priority_min(policy);
              sp.sched_priority = min_prio;
              if (pthread_attr_setschedparam(&attr, &sp) == 0)
                {
                  // ok, setting the lowest priority  
                  ok_prior = 1;
                  pthread_create(&thread_id, &attr, DoExecuteSqlThread,
                                 &ThreadParams);
                  pthread_detach(thread_id);
                }
            }
        }
    }
#endif
  if (!ok_prior)
    {
      // failure: using standard priority
      pthread_create(&thread_id, NULL, DoExecuteSqlThread, &ThreadParams);
      pthread_detach(thread_id);
    }
#endif
  MainFrame->GetQueryView()->EnableAbortButton();
  return true;
}

void MyResultSetView::ResetEmpty()
{
  if (TableView)
    TableView->Destroy();
  TableView = NULL;
  if (TableBlobs)
    delete TableBlobs;
  if (TableValues)
    delete TableValues;
  TableBlobs = NULL;
  TableValues = NULL;
  CurrentBlob = NULL;
  MainFrame->GetQueryView()->DisableFilterButton();
  MainFrame->DoResetSqlFilters();
}

bool MyResultSetView::ExecuteSqlPost()
{
  char dummy[1024];
  int i_col;
  int i_row;
  double seconds;
  clock_t clock_end;
  char elapsed[64];
  MyRowVariant *row;
  MyVariant *value;
  wxString blobType;
  int type;
  wxString cellValue;
  wxString currentBlock;
  MyVariantList *list;
  int decimal_precision = MainFrame->GetDecimalPrecision();
  bool isView = MainFrame->IsView(TableName);

  MainFrame->GetQueryView()->DisableAbortButton();
  if (TableView)
    TableView->Destroy();
  TableView = NULL;
  if (TableBlobs)
    delete TableBlobs;
  if (TableValues)
    delete TableValues;
  TableBlobs = NULL;
  TableValues = NULL;
  CurrentBlob = NULL;

  if (ThreadParams.IsValid() == false)
    {
      if (SqlErrorMsg.Len() > 0)
        {
          wxMessageBox(SqlErrorMsg, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                       MainFrame);
          ::wxEndBusyCursor();
          ThreadParams.Reset();
          return true;
        }
      goto error;
    }
  if (ThreadParams.IsError() == true)
    {
      if (SqlErrorMsg.Len() > 0)
        {
          wxMessageBox(SqlErrorMsg, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                       MainFrame);
          ::wxEndBusyCursor();
          ThreadParams.Reset();
          return true;
        }
      goto error;
    }

  list = ThreadParams.GetList();
  clock_end = clock();
  seconds =
    (double) (clock_end - ThreadParams.GetStart()) / (double) CLOCKS_PER_SEC;
  RsBeginRow = ThreadParams.GetFromRow();
  RsEndRow = ThreadParams.GetEndRow();
  if (ThreadParams.GetMaxRow() > RsMaxRow)
    RsMaxRow = ThreadParams.GetMaxRow();
  if (list->GetRows() == 0)
    {
      //
      // this one is an EMPTY Result Set
      //
      if (ReadOnly == false)
        {
          // preparing the insert row
          int numCols = 0;
          wxString *colNames = MainFrame->GetColumnNames(TableName, &numCols);
          CreateGrid(0, numCols + 1);
          if (isView == false)
            TableView->SetColLabelValue(0, wxT("ROWID"));
          for (i_col = 0; i_col < numCols; i_col++)
            TableView->SetColLabelValue(i_col + 1, *(colNames + i_col));
          TableView->EnableEditing(true);
          delete[]colNames;
      } else
        {
          // simply showing a warning message
          CreateGrid(1, 1);
          TableView->SetColLabelValue(0, wxT("Message"));
          TableView->SetRowLabelValue(0, wxT("Message"));
          TableView->SetCellValue(0, 0,
                                  wxT
                                  ("SQL query returned an empty ResultSet\n\nThis is not an error"));
        }
  } else
    {
      //
      // preparing the Grid to show the result set
      //
      CreateGrid(list->GetRows(), list->GetColumns());
      if (ReadOnly == true)
        TableView->EnableEditing(false);
      else
        TableView->EnableEditing(true);
      for (i_col = 0; i_col < list->GetColumns(); i_col++)
        TableView->SetColLabelValue(i_col, list->GetColumnName(i_col));
      if (ReadOnly == false && isView == false)
        TableView->SetColLabelValue(0, wxT("ROWID"));
      i_row = 0;
      row = list->GetFirst();
      while (row)
        {
          sprintf(dummy, "%d", i_row + RsBeginRow + 1);
          cellValue = wxString::FromUTF8(dummy);
          TableView->SetRowLabelValue(i_row, cellValue);
          if (ReadOnly == false)
            {
              // storing the ROWID value into the RowIds array
              value = row->GetColumn(0);
              if (value->GetType() == MY_INT_VARIANT)
                RowIds[i_row] = value->GetIntValue();
            }
          for (i_col = 0; i_col < row->GetNumCols(); i_col++)
            {
              value = row->GetColumn(i_col);
              if (value)
                {
                  switch (value->GetType())
                    {
                      case MY_INT_VARIANT:
                        sprintf(dummy, FORMAT_64, value->GetIntValue());
                        cellValue = wxString::FromUTF8(dummy);
                        TableView->SetCellValue(i_row, i_col, cellValue);
                        if (ReadOnly == false)
                          TableValues->SetValue(i_row, i_col,
                                                value->GetIntValue());
                        TableView->SetCellAlignment(i_row, i_col, wxALIGN_RIGHT,
                                                    wxALIGN_TOP);
                        break;
                      case MY_DBL_VARIANT:
                        if (decimal_precision >= 0)
                          {
                            char fmt[64];
                            double min = pow(10, -decimal_precision);
                            if (value->GetDblValue() != 0.0
                                && fabs(value->GetDblValue()) < min)
                              sprintf(fmt, "%%1.%de", decimal_precision);
                            else
                              sprintf(fmt, "%%1.%df", decimal_precision);
                            sprintf(dummy, fmt, value->GetDblValue());
                        } else
                          {
                            if (value->GetDblValue() != 0.0
                                && fabs(value->GetDblValue()) < 0.000001)
                              sprintf(dummy, "%1.6e", value->GetDblValue());
                            else
                              sprintf(dummy, "%1.6f", value->GetDblValue());
                          }
                        cellValue = wxString::FromUTF8(dummy);
                        TableView->SetCellValue(i_row, i_col, cellValue);
                        if (ReadOnly == false)
                          TableValues->SetValue(i_row, i_col,
                                                value->GetDblValue());
                        TableView->SetCellAlignment(i_row, i_col, wxALIGN_RIGHT,
                                                    wxALIGN_TOP);
                        break;
                      case MY_TXT_VARIANT:
                        TableView->SetCellValue(i_row, i_col,
                                                value->GetTxtValue());
                        if (ReadOnly == false)
                          TableValues->SetValue(i_row, i_col,
                                                value->GetTxtValue());
                        break;
                      case MY_BLOB_VARIANT:
                        blobType = wxT("UNKNOWN type");
                        type =
                          gaiaGuessBlobType(value->GetBlob(),
                                            value->GetBlobSize());
                        switch (type)
                          {
                            case GAIA_GEOMETRY_BLOB:
                              blobType = wxT("GEOMETRY");
                              break;
                            case GAIA_COMPRESSED_GEOMETRY_BLOB:
                              blobType = wxT("CompressedGEOMETRY");
                              break;
                            case GAIA_TINYPOINT_BLOB:
                              blobType = wxT("TinyPoint");
                              break;
                            case GAIA_GPB_BLOB:
                              blobType = wxT("GeoPackageGEOMETRY");
                              break;
                            case GAIA_XML_BLOB:
                              blobType = wxT("XmlBLOB");
                              break;
                            case GAIA_JPEG_BLOB:
                              blobType = wxT("JPEG image");
                              break;
                            case GAIA_EXIF_BLOB:
                              blobType = wxT("JPEG-EXIF image");
                              break;
                            case GAIA_EXIF_GPS_BLOB:
                              blobType = wxT("JPEG-EXIF-GPS image");
                              break;
                            case GAIA_PNG_BLOB:
                              blobType = wxT("PNG image");
                              break;
                            case GAIA_GIF_BLOB:
                              blobType = wxT("GIF image");
                              break;
                            case GAIA_TIFF_BLOB:
                              blobType = wxT("TIFF image");
                              break;
                            case GAIA_WEBP_BLOB:
                              blobType = wxT("WEBP image");
                              break;
                            case GAIA_JP2_BLOB:
                              blobType = wxT("JP2 image (Jpeg2000)");
                              break;
                            case GAIA_PDF_BLOB:
                              blobType = wxT("PDF document");
                              break;
                            case GAIA_ZIP_BLOB:
                              blobType = wxT("ZIP archive");
                              break;
                            default:
                              // testing for an eventual Text Font
                              int ret =
                                rl2_is_valid_encoded_font(value->GetBlob(),
                                                          value->GetBlobSize());
                              if (ret == RL2_OK)
                                blobType = wxT("TrueType Font");
                              else
                                {
                                  // testing for an encoded Raster Tile
                                  unsigned int tile_width;
                                  unsigned int tile_height;
                                  unsigned char sample_type;
                                  unsigned char pixel_type;
                                  unsigned char num_bands;
                                  unsigned char compression;
                                  int is_odd_tile;
                                  int has_mask;
                                  if (rl2_query_dbms_raster_tile
                                      (value->GetBlob(), value->GetBlobSize(),
                                       &tile_width, &tile_height, &sample_type,
                                       &pixel_type, &num_bands, &compression,
                                       &is_odd_tile, &has_mask) == RL2_OK)
                                    {
                                      char tile_sz[1024];
                                      sprintf(tile_sz, "(%ux%u)", tile_width,
                                              tile_height);
                                      blobType = wxT("RL2TILE ");
                                      if (is_odd_tile)
                                        blobType += wxT("ODD ");
                                      else
                                        blobType += wxT("EVEN ");
                                      blobType + wxString::FromUTF8(tile_sz);
                                      switch (pixel_type)
                                        {
                                          case RL2_PIXEL_MONOCHROME:
                                            blobType += wxT(", MonoChrome");
                                            break;
                                          case RL2_PIXEL_PALETTE:
                                            blobType += wxT(", Palette");
                                            break;
                                          case RL2_PIXEL_GRAYSCALE:
                                            blobType += wxT(", GrayScale");
                                            break;
                                          case RL2_PIXEL_RGB:
                                            blobType += wxT(", RGB");
                                            break;
                                          case RL2_PIXEL_MULTIBAND:
                                            blobType += wxT(", MultiBand");
                                            break;
                                          case RL2_PIXEL_DATAGRID:
                                            blobType += wxT(", DataGrid");
                                            break;
                                          default:
                                            blobType += wxT(", UnknownPixel");
                                            break;
                                        };
                                      switch (sample_type)
                                        {
                                          case RL2_SAMPLE_1_BIT:
                                            blobType += wxT(", 1-bit");
                                            break;
                                          case RL2_SAMPLE_2_BIT:
                                            blobType += wxT(", 2-bit");
                                            break;
                                          case RL2_SAMPLE_4_BIT:
                                            blobType += wxT(", 4-bit");
                                            break;
                                          case RL2_SAMPLE_INT8:
                                            blobType += wxT(", int8");
                                            break;
                                          case RL2_SAMPLE_UINT8:
                                            blobType += wxT(", uint8");
                                            break;
                                          case RL2_SAMPLE_INT16:
                                            blobType += wxT(", int16");
                                            break;
                                          case RL2_SAMPLE_UINT16:
                                            blobType += wxT(", uint16");
                                            break;
                                          case RL2_SAMPLE_INT32:
                                            blobType += wxT(", int32");
                                            break;
                                          case RL2_SAMPLE_UINT32:
                                            blobType += wxT(", uint32");
                                            break;
                                          case RL2_SAMPLE_FLOAT:
                                            blobType += wxT(", float");
                                            break;
                                          case RL2_SAMPLE_DOUBLE:
                                            blobType += wxT(", double");
                                            break;
                                          default:
                                            blobType += wxT(", UnknownSample");
                                            break;
                                        };
                                      char bands[256];
                                      if (num_bands == 1)
                                        sprintf(bands, ", %u-Band", num_bands);
                                      else
                                        sprintf(bands, ", %u-Bands", num_bands);
                                      blobType += wxString::FromUTF8(bands);
                                      switch (compression)
                                        {
                                          case RL2_COMPRESSION_NONE:
                                            blobType += wxT(", NotCompressed");
                                            break;
                                          case RL2_COMPRESSION_DEFLATE:
                                            blobType += wxT(", Deflate");
                                            break;
                                          case RL2_COMPRESSION_DEFLATE_NO:
                                            blobType += wxT(", DeflateNoDelta");
                                            break;
                                          case RL2_COMPRESSION_LZMA:
                                            blobType += wxT(", LZMA");
                                            break;
                                          case RL2_COMPRESSION_LZMA_NO:
                                            blobType += wxT(", LZMAnoDelta");
                                            break;
                                          case RL2_COMPRESSION_LZ4:
                                            blobType += wxT(", LZ4");
                                            break;
                                          case RL2_COMPRESSION_LZ4_NO:
                                            blobType += wxT(", LZ4noDelta");
                                            break;
                                          case RL2_COMPRESSION_ZSTD:
                                            blobType += wxT(", ZSTD");
                                            break;
                                          case RL2_COMPRESSION_ZSTD_NO:
                                            blobType += wxT(", ZSTDnoDelta");
                                            break;
                                          case RL2_COMPRESSION_PNG:
                                            blobType += wxT(", PNG");
                                            break;
                                          case RL2_COMPRESSION_JPEG:
                                            blobType += wxT(", JPEG");
                                            break;
                                          case RL2_COMPRESSION_LOSSY_WEBP:
                                            blobType += wxT(", WebP (lossy)");
                                            break;
                                          case RL2_COMPRESSION_LOSSLESS_WEBP:
                                            blobType +=
                                              wxT(", WebP (lossless)");
                                            break;
                                          case RL2_COMPRESSION_CCITTFAX4:
                                            blobType += wxT(", CCITT FAX4");
                                            break;
                                          case RL2_COMPRESSION_LOSSY_JP2:
                                            blobType +=
                                              wxT(", Jpeg2000 (lossy)");
                                            break;
                                          case RL2_COMPRESSION_LOSSLESS_JP2:
                                            blobType +=
                                              wxT(", Jpeg2000 (lossless)");
                                            break;
                                          default:
                                            blobType +=
                                              wxT(", UnknownCompression");
                                            break;
                                        };
                                      if (has_mask)
                                        blobType +=
                                          wxT(", WithTransparencyMask");
                                    }
                                }
                              break;
                          };
                        if (type == GAIA_XML_BLOB)
                          {
#ifdef ENABLE_LIBXML2           // only if LIBXML2 is enabled

                            int doc_size =
                              gaiaXmlBlobGetDocumentSize(value->GetBlob(),
                                                         value->GetBlobSize());
                            if (gaiaIsSvgXmlBlob
                                (value->GetBlob(), value->GetBlobSize()))
                              sprintf(dummy, "XmlBLOB-SVG sz=%d (XMLsz=%d) ",
                                      value->GetBlobSize(), doc_size);
                            else if (gaiaIsIsoMetadataXmlBlob
                                     (value->GetBlob(), value->GetBlobSize()))
                              sprintf(dummy,
                                      "XmlBLOB-ISOmetadata sz=%d (XMLsz=%d) ",
                                      value->GetBlobSize(), doc_size);
                            else
                              if (gaiaIsSldSeVectorStyleXmlBlob
                                  (value->GetBlob(), value->GetBlobSize()))
                              sprintf(dummy,
                                      "XmlBLOB-VectorStyle sz=%d (XMLsz=%d) ",
                                      value->GetBlobSize(), doc_size);
                            else
                              if (gaiaIsSldStyleXmlBlob
                                  (value->GetBlob(), value->GetBlobSize()))
                              sprintf(dummy,
                                      "XmlBLOB-SldStyle sz=%d (XMLsz=%d) ",
                                      value->GetBlobSize(), doc_size);
                            else
                              if (gaiaIsSldSeRasterStyleXmlBlob
                                  (value->GetBlob(), value->GetBlobSize()))
                              sprintf(dummy,
                                      "XmlBLOB-RasterStyle sz=%d (XMLsz=%d) ",
                                      value->GetBlobSize(), doc_size);
                            else
                              if (gaiaIsGpxXmlBlob
                                  (value->GetBlob(), value->GetBlobSize()))
                              sprintf(dummy,
                                      "XmlBLOB-GPX sz=%d (XMLsz=%d) ",
                                      value->GetBlobSize(), doc_size);
                            else
                              if (gaiaIsMapConfigXmlBlob
                                  (value->GetBlob(), value->GetBlobSize()))
                              sprintf(dummy,
                                      "XmlBLOB-MapConfig sz=%d (XMLsz=%d) ",
                                      value->GetBlobSize(), doc_size);
                            else
                              sprintf(dummy, "XmlBLOB sz=%d (XMLsz=%d) ",
                                      value->GetBlobSize(), doc_size);
                            cellValue = wxString::FromUTF8(dummy);
                            if (gaiaIsSchemaValidatedXmlBlob
                                (value->GetBlob(), value->GetBlobSize()))
                              cellValue += wxT(" SchemaValidated");

#endif // end LIBXML2 conditionals
                        } else
                          {
                            sprintf(dummy, "BLOB sz=%d ", value->GetBlobSize());
                            cellValue = wxString::FromUTF8(dummy);
                            cellValue += blobType;
                          }
                        TableView->SetCellValue(i_row, i_col, cellValue);
                        TableView->SetReadOnly(i_row, i_col);
                        TableBlobs->SetBlob(i_row, i_col, value);
                        break;
                      case MY_NULL_VARIANT:
                      default:
                        TableView->SetCellValue(i_row, i_col, wxT("NULL"));
                        break;
                    };
              } else
                TableView->SetCellValue(i_row, i_col, wxT("NULL"));
              if (ReadOnly == false)
                {
                  if (IsPrimaryKey(i_col) == true)
                    TableView->SetReadOnly(i_row, i_col);
                  if (IsBlobColumn(i_col) == true)
                    TableView->SetReadOnly(i_row, i_col);
                }
            }
          i_row++;
          row = row->GetNext();
        }
    }
  if (ReadOnly == false)
    {
      // prepearing the insert row
      TableView->SetRowLabelValue(TableView->GetNumberRows() - 1,
                                  wxT("Insert row"));
      InsertPending = false;
      for (i_col = 0; i_col < TableView->GetNumberCols(); i_col++)
        {
          TableView->SetCellValue(TableView->GetNumberRows() - 1, i_col,
                                  wxT(""));
          TableView->SetCellBackgroundColour(TableView->GetNumberRows() - 1,
                                             i_col, wxColour(0, 0, 0));
          TableView->SetReadOnly(TableView->GetNumberRows() - 1, i_col);
        }
    }
  TableView->SetRowLabelSize(wxGRID_AUTOSIZE);
  TableView->AutoSize();
  ResizeView();
  FormatElapsedTime(seconds, elapsed);
  sprintf(dummy, "current block: %d / %d [%d rows]    [fetched in %s]",
          RsBeginRow + 1, RsEndRow + 1, RsMaxRow, elapsed);
  currentBlock = wxString::FromUTF8(dummy);
  RsCurrentBlock->SetLabel(currentBlock);
  ShowControls();
  MainFrame->GetQueryView()->AddToHistory(ThreadParams.GetSql());
  ThreadParams.Reset();
  ::wxEndBusyCursor();
  return true;
error:
  ::wxEndBusyCursor();
  ThreadParams.Reset();
  return false;
}

bool MyResultSetView::IsPrimaryKey(int column)
{
//
// checks if this column is a Primary Key one
//
  int i;
  for (i = 0; i < 1024; i++)
    {
      if (PrimaryKeys[i] == column)
        return true;
    }
  return false;
}

bool MyResultSetView::IsBlobColumn(int column)
{
//
// checks if this column is a BLOB-type column
//
  int i;
  for (i = 0; i < 1024; i++)
    {
      if (BlobColumns[i] == column)
        return true;
    }
  return false;
}

void MyResultSetView::CreateGrid(int rows, int cols)
{
//
// creating a new Grid to show the result set
//
  int extra = 0;
  if (ReadOnly == false)
    extra = 1;
  TableView = new wxGrid(this, wxID_ANY, wxPoint(5, 5), wxSize(200, 200));
  TableView->Show(false);
  TableView->CreateGrid(rows + extra, cols);
  TableBlobs = new MyBlobs(rows, cols);
  if (ReadOnly == false)
    TableValues = new MyValues(rows, cols);
}

void MyResultSetView::CreateStatsGrid()
{
//
// creating a new Grid to show the SQL query stats (progress handler)
//
  wxSize sz = GetClientSize();
  TableView = new wxGrid(this, wxID_ANY, wxPoint(5, 5), wxSize(200, 200));
  TableView->Show(false);
  TableView->CreateGrid(5, 2);
  TableView->EnableEditing(false);
  TableView->SetColLabelValue(0, wxT("Progress Counter"));
  TableView->SetColLabelValue(1, wxT("Current Value"));
  TableView->SetCellValue(0, 0, wxT("FullscanStep"));
  TableView->SetCellValue(1, 0, wxT("Sort"));
  TableView->SetCellValue(2, 0, wxT("Autoindex"));
  TableView->SetCellValue(3, 0, wxT("FetchedRows"));
  TableView->SetCellValue(4, 0, wxT("ElapsedTime"));
  TableView->SetCellBackgroundColour(4, 0, wxColour(128, 255, 128));
  TableView->SetCellValue(0, 1, wxT("0"));
  TableView->SetCellAlignment(0, 1, wxALIGN_RIGHT, wxALIGN_CENTRE);
  TableView->SetCellValue(1, 1, wxT("0"));
  TableView->SetCellAlignment(1, 1, wxALIGN_RIGHT, wxALIGN_CENTRE);
  TableView->SetCellValue(2, 1, wxT("0"));
  TableView->SetCellAlignment(2, 1, wxALIGN_RIGHT, wxALIGN_CENTRE);
  TableView->SetCellValue(3, 1, wxT("0"));
  TableView->SetCellAlignment(3, 1, wxALIGN_RIGHT, wxALIGN_CENTRE);
  TableView->SetCellValue(4, 1, wxT("0"));
  TableView->SetCellAlignment(4, 1, wxALIGN_RIGHT, wxALIGN_CENTRE);
  TableView->SetRowLabelSize(wxGRID_AUTOSIZE);
  TableView->AutoSize();
  TableView->SetSize(sz.GetWidth() - 10, sz.GetHeight() - 45);
  TableView->Show(true);
  TableView->Disable();
  wxString msg = wxT("... wait please: SQL query in progress ...");
  RsCurrentBlock->SetLabel(msg);
}

void MyResultSetView::ResizeView()
{
//
// resizing the Grid to show the result set
//
  wxSize sz = GetClientSize();
  if (TableView)
    {
      TableView->SetSize(sz.GetWidth() - 10, sz.GetHeight() - 45);
      TableView->Show(true);
    }
}

void MyResultSetView::OnSize(wxSizeEvent & WXUNUSED(event))
{
//
// this window has changed its size
//
  wxSize sz = GetClientSize();
  if (TableView)
    TableView->SetSize(sz.GetWidth() - 10, sz.GetHeight() - 45);
  BtnRsFirst->Move(5, sz.GetHeight() - 35);
  BtnRsPrevious->Move(40, sz.GetHeight() - 35);
  BtnRefresh->Move(75, sz.GetHeight() - 35);
  BtnRsNext->Move(110, sz.GetHeight() - 35);
  BtnRsLast->Move(145, sz.GetHeight() - 35);
  RsCurrentBlock->Move(180, sz.GetHeight() - 25);
  BtnRsMapShow->Move(sz.GetWidth() - 80, sz.GetHeight() - 35);
  BtnRsMapZoom->Move(sz.GetWidth() - 45, sz.GetHeight() - 35);
}

void MyResultSetView::OnMapFeatureSelect(wxCommandEvent & event)
{
//
// a Grid's row has been selected
//  
  if (MainFrame->IsMapLinkedResultSet() == true)
    {
      BtnRsMapShow->Enable(true);
      BtnRsMapZoom->Enable(true);
    }
  event.Skip();
}

void MyResultSetView::OnRsFirst(wxCommandEvent & WXUNUSED(event))
{
//
// scrolling to the result set beginning
//
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  if (ExecuteSqlPre
      (sql, 0, ReadOnly, CoverageTiles, TileDataDbPrefix, TileDataTable,
       false) == false)
    wxMessageBox(SqlErrorMsg, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                 MainFrame);
}

void MyResultSetView::OnRsPrevious(wxCommandEvent & WXUNUSED(event))
{
//
// scrolling to the result set previous block
//
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  int start = RsBeginRow - RsBlock;
  if (start < 0)
    start = 0;
  if (ExecuteSqlPre
      (sql, start, ReadOnly, CoverageTiles, TileDataDbPrefix, TileDataTable,
       false) == false)
    wxMessageBox(SqlErrorMsg, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                 MainFrame);
}

void MyResultSetView::OnRsNext(wxCommandEvent & WXUNUSED(event))
{
//
// scrolling to the result set next block
//
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  int start = RsEndRow + 1;
  if (ExecuteSqlPre
      (sql, start, ReadOnly, CoverageTiles, TileDataDbPrefix, TileDataTable,
       false) == false)
    wxMessageBox(SqlErrorMsg, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                 MainFrame);
}

void MyResultSetView::OnRsLast(wxCommandEvent & WXUNUSED(event))
{
//
// scrolling to the result set ending
//
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  UpdateMaxRow(sql);
  int start = RsMaxRow - RsBlock;
  if (start < 0)
    start = 0;
  if (ExecuteSqlPre
      (sql, start, ReadOnly, CoverageTiles, TileDataDbPrefix, TileDataTable,
       false) == false)
    wxMessageBox(SqlErrorMsg, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                 MainFrame);
}

int MyResultSetView::FindGeomColumnIndex(wxString & column)
{
//
// searching the column corresponding the Geometry
//
  int col;
  for (col = 0; col < TableView->GetNumberCols(); col++)
    {
      if (TableView->GetColLabelValue(col).CmpNoCase(column) == 0)
        return col;
    }
  return -1;
}

gaiaGeomCollPtr MyResultSetView::DoTransformMapGeometry(unsigned char *blob,
                                                        int blob_size, int srid)
{
//
// transformint a Selected Geometry into the Map Panel own SRID
//
  gaiaGeomCollPtr geom = NULL;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT ST_Transform(?, ?)";
  int ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return NULL;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_blob(stmt, 1, blob, blob_size, NULL);
  sqlite3_bind_int(stmt, 2, srid);
  while (1)
    {
      // scrolling the result set rows
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          blob = (unsigned char *) sqlite3_column_blob(stmt, 0);
          blob_size = sqlite3_column_bytes(stmt, 0);
          geom = gaiaFromSpatiaLiteBlobWkb(blob, blob_size);
        }
    }
  sqlite3_finalize(stmt);
  return geom;
}

void MyResultSetView::OnRsMapShow(wxCommandEvent & WXUNUSED(event))
{
//
// showing the selected Feature on the Map Panel
//
  if (MainFrame->IsMapLinkedResultSet() != true)
    return;
  if (TableView->IsSelection() == false)
    return;

  int r;
  int col = FindGeomColumnIndex(MainFrame->GetGeometryColumnFromSqlFilter());
  if (col < 0)
    return;

  MapFeaturesList *list = new MapFeaturesList(MainFrame->GetMapSRID());
  for (r = 0; r < TableView->GetNumberRows(); r++)
    {
      if (TableView->IsInSelection(r, 0) == true)
        {
          /*
           * Appending a selected Map Feature's Geometry to be 
           * hilighted on the Map Panel
           */
          MyVariant *blobVar = TableBlobs->GetBlob(r, col);
          if (blobVar->GetType() == MY_BLOB_VARIANT)
            {
              if (blobVar->GetBlob() != NULL && blobVar->GetBlobSize() > 0)
                {
                  gaiaGeomCollPtr geom =
                    DoTransformMapGeometry(blobVar->GetBlob(),
                                           blobVar->GetBlobSize(),
                                           MainFrame->GetMapSRID());
                  if (geom != NULL)
                    list->Add(geom);
                }
            }
        }
    }
  if (list->GetFirst() == NULL)
    {
      delete list;
      return;
    }
  MainFrame->MapViewShowSelected(list, false);
}

void MyResultSetView::OnRsMapZoom(wxCommandEvent & WXUNUSED(event))
{
//
// zooming and hilighting the selected Feature on the Map Panel
//
  if (MainFrame->IsMapLinkedResultSet() != true)
    return;
  if (TableView->IsSelection() == false)
    return;

  int r;
  int col = FindGeomColumnIndex(MainFrame->GetGeometryColumnFromSqlFilter());
  if (col < 0)
    return;

  MapFeaturesList *list = new MapFeaturesList(MainFrame->GetMapSRID());
  for (r = 0; r < TableView->GetNumberRows(); r++)
    {
      if (TableView->IsInSelection(r, 0) == true)
        {
          /*
           * Appending a selected Map Feature's Geometry to be 
           * hilighted on the Map Panel
           */
          MyVariant *blobVar = TableBlobs->GetBlob(r, col);
          if (blobVar->GetType() == MY_BLOB_VARIANT)
            {
              if (blobVar->GetBlob() != NULL && blobVar->GetBlobSize() > 0)
                {
                  gaiaGeomCollPtr geom =
                    DoTransformMapGeometry(blobVar->GetBlob(),
                                           blobVar->GetBlobSize(),
                                           MainFrame->GetMapSRID());
                  if (geom != NULL)
                    list->Add(geom);
                }
            }
        }
    }
  if (list->GetFirst() == NULL)
    {
      delete list;
      return;
    }
  MainFrame->MapViewShowSelected(list, true);
}

void MyResultSetView::OnRefresh(wxCommandEvent & WXUNUSED(event))
{
//
// refreshing the result set
//
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  int start = RsBeginRow;
  if (ExecuteSqlPre
      (sql, start, ReadOnly, CoverageTiles, TileDataDbPrefix, TileDataTable,
       false) == false)
    wxMessageBox(SqlErrorMsg, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                 MainFrame);
}

void MyResultSetView::OnThreadFinished(wxCommandEvent & WXUNUSED(event))
{
//
// the SQL thread signals termination
//
  if (ProgressTimer)
    {
      ProgressTimer->Stop();
      delete ProgressTimer;
      ProgressTimer = NULL;
    }
  MainFrame->EnableAllTools();
  if (ExecuteSqlPost() == false)
    wxMessageBox(wxT("An error occurred while showing the ResultSet"),
                 wxT("spatialite_gui"), wxOK | wxICON_ERROR, MainFrame);
}

void MyResultSetView::OnTimerStatistics(wxTimerEvent & WXUNUSED(event))
{
//
// updating SQL stats (progress handler)
//
  char elapsed[64];
  char dummy[1024];
  wxString cellValue;
  if (ThreadParams.IsValid() == false)
    return;
  FormatElapsedTime(ThreadParams.GetElapsedTime(), elapsed, true);
  sprintf(dummy, "%d", ThreadParams.GetStatFullscanStep());
  cellValue = wxString::FromUTF8(dummy);
  TableView->SetCellValue(0, 1, cellValue);
  sprintf(dummy, "%d", ThreadParams.GetStatSort());
  cellValue = wxString::FromUTF8(dummy);
  TableView->SetCellValue(1, 1, cellValue);
  sprintf(dummy, "%d", ThreadParams.GetStatAutoindex());
  cellValue = wxString::FromUTF8(dummy);
  TableView->SetCellValue(2, 1, cellValue);
  sprintf(dummy, "%d", ThreadParams.GetFetchedRows());
  cellValue = wxString::FromUTF8(dummy);
  TableView->SetCellValue(3, 1, cellValue);
  sprintf(dummy, "%s", elapsed);
  cellValue = wxString::FromUTF8(dummy);
  TableView->SetCellValue(4, 1, cellValue);
  TableView->ForceRefresh();
  ProgressTimer->Start(125, wxTIMER_ONE_SHOT);
}

void MyResultSetView::OnRightClick(wxGridEvent & event)
{
//
// right click on some cell [mouse action]
//
  int blob_type;
  MyVariant *blobVar;
  wxMenu menu;
  wxMenuItem *menuItem;
  wxPoint pt = event.GetPosition();
  if (ReadOnly == false && event.GetRow() == TableView->GetNumberRows() - 1)
    {
      // this is the INSERT ROW
      if (InsertPending == true)
        {
          menuItem =
            new wxMenuItem(&menu, Grid_Insert, wxT("&Confirm insertion"));
          menu.Append(menuItem);
          menuItem = new wxMenuItem(&menu, Grid_Abort, wxT("&Abort insertion"));
          menu.Append(menuItem);
      } else
        {
          menuItem = new wxMenuItem(&menu, Grid_Insert, wxT("&Insert new row"));
          menu.Append(menuItem);
        }
      TableView->PopupMenu(&menu, pt);
      return;
    }
  if (CoverageTiles == true)
    {
      // supporting Tile Preview
      wxString value = TableView->GetCellValue(event.GetRow(), 0);
      long tile_id;
      value.ToLong(&tile_id);
      CurrentTileId = tile_id;
      menuItem =
        new wxMenuItem(&menu, Grid_TilePreview, wxT("Raster &Tile Preview"));
      menu.Append(menuItem);
      goto done;
    }
  CurrentEvtRow = event.GetRow();
  CurrentEvtColumn = event.GetCol();
  blobVar = TableBlobs->GetBlob(CurrentEvtRow, CurrentEvtColumn);
  if (blobVar)
    {
      // this one is a BLOB cell
      if (ReadOnly == false)
        {
          MyRowVariant *varRow = TableValues->GetRow(CurrentEvtRow);
          if (varRow->IsDeleted() == false)
            {
              menuItem = new wxMenuItem(&menu, Grid_Delete, wxT("&Delete row"));
              menu.Append(menuItem);
              menuItem =
                new wxMenuItem(&menu, Grid_Insert, wxT("&Insert new row"));
              menu.Append(menuItem);
              menu.AppendSeparator();
            }
        }
      menuItem = new wxMenuItem(&menu, Grid_Blob, wxT("BLOB &explore"));
      menu.Append(menuItem);
      blob_type = gaiaGuessBlobType(blobVar->GetBlob(), blobVar->GetBlobSize());
      if (blob_type == GAIA_GEOMETRY_BLOB
          || blob_type == GAIA_COMPRESSED_GEOMETRY_BLOB
          || blob_type == GAIA_TINYPOINT_BLOB || blob_type == GAIA_GPB_BLOB)
        ;
      else if (blob_type == GAIA_XML_BLOB)
        {
          menu.AppendSeparator();
          menuItem =
            new wxMenuItem(&menu, Grid_XmlBlobIn, wxT("XmlBLOB &import"));
          menu.Append(menuItem);
          menuItem =
            new wxMenuItem(&menu, Grid_XmlBlobOut,
                           wxT("XmlBLOB &export (not indented)"));
          menu.Append(menuItem);
          menuItem =
            new wxMenuItem(&menu, Grid_XmlBlobOutIndented,
                           wxT("XmlBLOB export (i&ndented)"));
          menu.Append(menuItem);
          menuItem =
            new wxMenuItem(&menu, Grid_BlobNull, wxT("Set BLOB as &NULL"));
          menu.Append(menuItem);
      } else
        {
          menu.AppendSeparator();
          menuItem = new wxMenuItem(&menu, Grid_BlobIn, wxT("BLOB &import"));
          menu.Append(menuItem);
          menuItem = new wxMenuItem(&menu, Grid_BlobOut, wxT("BLOB &export"));
          menu.Append(menuItem);
          menuItem =
            new wxMenuItem(&menu, Grid_BlobNull, wxT("Set BLOB as &NULL"));
          menu.Append(menuItem);
        }
      CurrentBlob = blobVar;
  } else
    {
      // this one is an ordinary cell
      CurrentBlob = NULL;
      if (ReadOnly == false)
        {
          MyRowVariant *varRow = TableValues->GetRow(CurrentEvtRow);
          if (varRow->IsDeleted() == false)
            {
              menuItem = new wxMenuItem(&menu, Grid_Delete, wxT("&Delete row"));
              menu.Append(menuItem);
              menuItem =
                new wxMenuItem(&menu, Grid_Insert, wxT("&Insert new row"));
              menu.Append(menuItem);
              if (IsBlobColumn(CurrentEvtColumn) == true)
                {
                  menu.AppendSeparator();
                  menuItem =
                    new wxMenuItem(&menu, Grid_BlobIn, wxT("BLOB &import"));
                  menu.Append(menuItem);
                }
              menu.AppendSeparator();
            }
        }
      menuItem = new wxMenuItem(&menu, Grid_Clear, wxT("&Clear selection"));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Grid_All, wxT("Select &all"));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Grid_Row, wxT("Select &row"));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Grid_Column, wxT("&Select column"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem = new wxMenuItem(&menu, Grid_Copy, wxT("&Copy"));
      menu.Append(menuItem);
      if (TableView->IsSelection() == false)
        menuItem->Enable(false);
    }
done:
  menu.AppendSeparator();
  wxMenu *exportMenu = new wxMenu();
  menuItem = new wxMenuItem(exportMenu, Grid_ExpTxtTab, wxT("as &Txt/Tab"));
  exportMenu->Append(menuItem);
  menuItem = new wxMenuItem(exportMenu, Grid_ExpCsv, wxT("as &CSV"));
  exportMenu->Append(menuItem);
  menuItem = new wxMenuItem(exportMenu, Grid_ExpHtml, wxT("as &HTML"));
  exportMenu->Append(menuItem);
  menuItem = new wxMenuItem(exportMenu, Grid_ExpShp, wxT("as &Shapefile"));
  exportMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(exportMenu, Grid_ExpDif, wxT("as &DIF spreadsheet"));
  exportMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(exportMenu, Grid_ExpSylk, wxT("as &SYLK spreadsheet"));
  exportMenu->Append(menuItem);
  menuItem = new wxMenuItem(exportMenu, Grid_ExpDbf, wxT("as &DBF archive"));
  exportMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(exportMenu, Grid_ExpXlsx,
                   wxT("as MS &Excell Spreadsheet (XLSX)"));
  exportMenu->Append(menuItem);
  menu.AppendSubMenu(exportMenu, wxT("&Export ResultSet"));
  menu.AppendSeparator();
  if (MainFrame->IsValidSqlFilter() == true)
    {
      menu.AppendSeparator();
      if (MainFrame->IsMapLinkedResultSet() == true)
        {
          menuItem =
            new wxMenuItem(&menu, Grid_MapShow,
                           wxT("Map Panel: &Show Feature"));
          menuItem->SetBitmap(wxBitmap(map_preview_xpm));
          menu.Append(menuItem);
          menuItem =
            new wxMenuItem(&menu, Grid_MapZoom,
                           wxT("Map Panel: &Zoom to Feature"));
          menuItem->SetBitmap(wxBitmap(map_magnify_xpm));
          menu.Append(menuItem);
        }
      menuItem = new wxMenuItem(&menu, Grid_Filter, wxT("Add &Filter(s)"));
      menuItem->SetBitmap(wxBitmap(filter_xpm));
      menu.Append(menuItem);
    }
  if (MainFrame->IsMapLinkedResultSet() == true)
    {
      BtnRsMapShow->Enable(true);
      BtnRsMapZoom->Enable(true);
  } else
    {
      BtnRsMapShow->Enable(false);
      BtnRsMapZoom->Enable(false);
    }
  PopupMenu(&menu, pt);
}

void MyResultSetView::OnCellSelected(wxGridEvent & event)
{
//
// cell selection changed
//
  if (InsertPending == true)
    {
      // an INSERT row is still pending
      if (event.GetRow() != TableView->GetNumberRows() - 1)
        DoInsert(false);
    }
  event.Skip();
}

void MyResultSetView::OnCellChanged(wxGridEvent & event)
{
//
// user changed value in some cell
//
  MyVariant *oldValue;
  MyVariant *insValue;
  wxString value;
  wxString numValue;
  wxString newValue = wxT("NULL");
  wxString sql;
  char *xsql;
  wxString rowid;
  char *errMsg = NULL;
  bool error = false;
  sqlite3_int64 int64_value;
  long long_value;
  bool okIntValue = false;
  double dbl_value;
  bool okDblValue = false;
  int ret;
  char dummy[256];
  int row = event.GetRow();
  int column = event.GetCol();
  char *xname;
  char *xname2;
  int decimal_precision = MainFrame->GetDecimalPrecision();
  value = TableView->GetCellValue(row, column);
  if (InsertPending == true)
    {
      // an INSERT row is still pending
      insValue = InsertRow->GetColumn(column);
      numValue = value;
      numValue.Replace(wxT(","), wxT("."));
      okIntValue = numValue.ToLong(&long_value);
      okDblValue = numValue.ToDouble(&dbl_value);
      if (okIntValue == true)
        {
          int64_value = long_value;
          insValue->Set(int64_value);
      } else if (okDblValue == true)
        insValue->Set(dbl_value);
      else
        insValue->Set(value);
      if (row != TableView->GetNumberRows() - 1)
        DoInsert(false);
      return;
    }
  if (value.Len() > 0)
    {
      numValue = value;
      numValue.Replace(wxT(","), wxT("."));
      okIntValue = numValue.ToLong(&long_value);
      okDblValue = numValue.ToDouble(&dbl_value);
      if (okIntValue == true)
        {
          int64_value = long_value;
          sprintf(dummy, FORMAT_64, int64_value);
          newValue = wxString::FromUTF8(dummy);
      } else if (okDblValue == true)
        {
          if (decimal_precision >= 0)
            {
              char fmt[64];
              sprintf(fmt, "%%1.%df", decimal_precision);
              sprintf(dummy, fmt, dbl_value);
          } else
            sprintf(dummy, "%1.6f", dbl_value);
          newValue = wxString::FromUTF8(dummy);
      } else
        {
          value.Replace(wxT("'"), wxT("''"));
          newValue = wxT("'") + value + wxT("'");
        }
    }
  if (MainFrame->IsWritableView(TableName) == true)
    {
      // Writable View
      char *xpk_name;
      char *xpk_name2;
      int pk_index = -1;
      int col;
      wxString pkValue;
      char *xpkValue;
      char *xxpkValue;
      wxString pk_name;
      MainFrame->GetWritableViewPK(TableName, pk_name);
      if (pk_name.Len() == 0)
        {
          wxMessageBox(wxT("Unable to retrieve a Primary Key"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          error = true;
          goto done;
        }
      for (col = 0; col < TableView->GetNumberCols(); col++)
        {
          if (pk_name.CmpNoCase(TableView->GetColLabelValue(col)) == 0)
            pk_index = col;
        }
      if (pk_index < 0)
        {
          wxMessageBox(wxT("Unable to retrieve a Primary Key"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          error = true;
          goto done;
        }
      pkValue = TableView->GetCellValue(row, pk_index);
      xpkValue = (char *) malloc((pkValue.Len() * 4) + 1);
      strcpy(xpkValue, pkValue.ToUTF8());
      xxpkValue = sqlite3_mprintf("%Q", xpkValue);
      free(xpkValue);
      pkValue = wxString::FromUTF8(xxpkValue);
      sqlite3_free(xxpkValue);
      sql = wxT("UPDATE ");
      xname = (char *) malloc((TableName.Len() * 4) + 1);
      strcpy(xname, TableName.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT(" SET ");
      xname =
        (char *) malloc((TableView->GetColLabelValue(column).Len() * 4) + 1);
      strcpy(xname, TableView->GetColLabelValue(column).ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT(" = ") + newValue + wxT(" WHERE ");
      xpk_name = (char *) malloc((pk_name.Len() * 4) + 1);
      strcpy(xpk_name, pk_name.ToUTF8());
      xpk_name2 = gaiaDoubleQuotedSql(xpk_name);
      free(xpk_name);
      sql += wxString::FromUTF8(xpk_name2);
      free(xpk_name2);
      sql += wxT(" = ") + pkValue;
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
  } else
    {
      // ordinary Table
      oldValue = TableValues->GetValue(row, 0);
      sprintf(dummy, FORMAT_64, oldValue->GetIntValue());
      rowid = wxString::FromUTF8(dummy);
      sql = wxT("UPDATE ");
      xname = (char *) malloc((TableName.Len() * 4) + 1);
      strcpy(xname, TableName.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT(" SET ");
      xname =
        (char *) malloc((TableView->GetColLabelValue(column).Len() * 4) + 1);
      strcpy(xname, TableView->GetColLabelValue(column).ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT(" = ") + newValue + wxT(" WHERE ROWID = ") + rowid;
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
    }
  ret = sqlite3_exec(MainFrame->GetSqlite(), xsql, NULL, NULL, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      error = true;
    }

done:
  oldValue = TableValues->GetValue(row, column);
  if (error == true)
    {
      // update failed; restorig old cell value
      value = wxT("NULL");
      if (oldValue)
        {
          if (oldValue->GetType() == MY_INT_VARIANT)
            {
              sprintf(dummy, FORMAT_64, oldValue->GetIntValue());
              value = wxString::FromUTF8(dummy);
            }
          if (oldValue->GetType() == MY_DBL_VARIANT)
            {
              if (decimal_precision >= 0)
                {
                  char fmt[64];
                  sprintf(fmt, "%%1.%df", decimal_precision);
                  sprintf(dummy, fmt, oldValue->GetDblValue());
              } else
                sprintf(dummy, "%1.6f", oldValue->GetDblValue());
              value = wxString::FromUTF8(dummy);
            }
          if (oldValue->GetType() == MY_TXT_VARIANT)
            value = oldValue->GetTxtValue();
        }
      TableView->SetCellValue(row, column, value);
      TableView->ForceRefresh();
  } else
    {
      // marking cell as modified
      TableView->SetCellTextColour(row, column, wxColour(0, 0, 192));
      TableView->SetCellBackgroundColour(row, column, wxColour(255, 255, 204));
      TableView->ForceRefresh();
    }
}

void MyResultSetView::DoInsert(bool confirmed)
{
//
// performing actual row INSERT
//
  int i;
  int ret;
  MyVariant *var;
  wxString value;
  wxString strValue;
  wxString sql;
  char *xsql;
  char *dummy;
  char *errMsg = NULL;
  char *xname;
  char *xname2;
  int decimal_precision = MainFrame->GetDecimalPrecision();
  if (confirmed == false)
    {
      ret =
        wxMessageBox(wxT("A new row is ready for insertion\n\nConfirm ?"),
                     wxT("spatialite_gui"), wxYES_NO | wxICON_QUESTION, this);
      if (ret != wxYES)
        goto close_insert;
    }
  sql = wxT("INSERT INTO ");
  xname = (char *) malloc((TableName.Len() * 4) + 1);
  strcpy(xname, TableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT(" (");
  for (i = 1; i < TableView->GetNumberCols(); i++)
    {
      if (i > 1)
        sql += wxT(", ");
      xname = (char *) malloc((TableView->GetColLabelValue(i).Len() * 4) + 1);
      strcpy(xname, TableView->GetColLabelValue(i).ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
    }
  sql += wxT(") VALUES (");
  for (i = 1; i < InsertRow->GetNumCols(); i++)
    {
      if (i > 1)
        sql += wxT(", ");
      var = InsertRow->GetColumn(i);
      value = wxT("NULL");
      if (var->GetType() == MY_INT_VARIANT)
        {
          dummy = (char *) malloc(1024);
          sprintf(dummy, FORMAT_64, var->GetIntValue());
          value = wxString::FromUTF8(dummy);
          free(dummy);
        }
      if (var->GetType() == MY_DBL_VARIANT)
        {
          dummy = (char *) malloc(1024);
          if (decimal_precision >= 0)
            {
              char fmt[64];
              sprintf(fmt, "%%1.%df", decimal_precision);
              sprintf(dummy, fmt, var->GetDblValue());
          } else
            sprintf(dummy, "%1.6f", var->GetDblValue());
          value = wxString::FromUTF8(dummy);
          free(dummy);
        }
      if (var->GetType() == MY_TXT_VARIANT)
        {
          strValue = var->GetTxtValue();
          strValue.Replace(wxT("'"), wxT("''"));
          value = wxT("'") + strValue + wxT("'");
        }
      sql += value;
    }
  sql += wxT(")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(MainFrame->GetSqlite(), xsql, NULL, NULL, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
    }
close_insert:
//
// closing insert row
//
  InsertPending = false;
  delete InsertRow;
  InsertRow = NULL;
  for (i = 0; i < TableView->GetNumberCols(); i++)
    {
      TableView->SetCellValue(TableView->GetNumberRows() - 1, i, wxT(""));
      TableView->SetCellBackgroundColour(TableView->GetNumberRows() - 1, i,
                                         wxColour(0, 0, 0));
      TableView->SetReadOnly(TableView->GetNumberRows() - 1, i);
    }
}

void MyResultSetView::OnCmdDelete(wxCommandEvent & WXUNUSED(event))
{
//
// user required row deletion
//
  char *errMsg = NULL;
  bool error = false;
  int ret;
  int i;
  wxString sql;
  char *xsql;
  wxString rowid;
  MyVariant *value;
  char dummy[1024];
  char *xname;
  char *xname2;
  MyRowVariant *varRow = TableValues->GetRow(CurrentEvtRow);
  if (varRow->IsDeleted() == true)
    return;
  xname = (char *) malloc((TableName.Len() * 4) + 1);
  strcpy(xname, TableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  if (MainFrame->IsWritableView(TableName) == true)
    {
      // Writable View
      char *xpk_name;
      char *xpk_name2;
      int pk_index = -1;
      int col;
      wxString pkValue;
      char *xpkValue;
      char *xxpkValue;
      wxString pk_name;
      MainFrame->GetWritableViewPK(TableName, pk_name);
      if (pk_name.Len() == 0)
        {
          wxMessageBox(wxT("Unable to retrieve a Primary Key"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          error = true;
          goto done;
        }
      for (col = 0; col < TableView->GetNumberCols(); col++)
        {
          if (pk_name.CmpNoCase(TableView->GetColLabelValue(col)) == 0)
            pk_index = col;
        }
      if (pk_index < 0)
        {
          wxMessageBox(wxT("Unable to retrieve a Primary Key"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          error = true;
          goto done;
        }
      pkValue = TableView->GetCellValue(CurrentEvtRow, pk_index);
      wxString msg = wxT("Requested deletion for row identified by ");
      msg += pk_name + wxT(" = ") + pkValue + wxT("\n\nConfirm ?");
      ret =
        wxMessageBox(msg, wxT("spatialite_gui"),
                     wxYES_NO | wxICON_QUESTION, this);
      if (ret != wxYES)
        return;
      xpkValue = (char *) malloc((pkValue.Len() * 4) + 1);
      strcpy(xpkValue, pkValue.ToUTF8());
      xxpkValue = sqlite3_mprintf("%Q", xpkValue);
      free(xpkValue);
      pkValue = wxString::FromUTF8(xxpkValue);
      sqlite3_free(xxpkValue);
      sql =
        wxT("DELETE FROM \"") + wxString::FromUTF8(xname2) + wxT("\" WHERE ");
      free(xname2);
      xpk_name = (char *) malloc((pk_name.Len() * 4) + 1);
      strcpy(xpk_name, pk_name.ToUTF8());
      xpk_name2 = gaiaDoubleQuotedSql(xpk_name);
      free(xpk_name);
      sql += wxString::FromUTF8(xpk_name2);
      free(xpk_name2);
      sql += wxT(" = ") + pkValue;
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
  } else
    {
      // ordinary table
      value = TableValues->GetValue(CurrentEvtRow, 0);
      sprintf(dummy, FORMAT_64, value->GetIntValue());
      rowid = wxString::FromUTF8(dummy);
      ret =
        wxMessageBox(wxT("Requested deletion for row identified by RowId = ")
                     + rowid + wxT("\n\nConfirm ?"), wxT("spatialite_gui"),
                     wxYES_NO | wxICON_QUESTION, this);
      if (ret != wxYES)
        return;
      sql =
        wxT("DELETE FROM \"") + wxString::FromUTF8(xname2) +
        wxT("\" WHERE ROWID = ") + rowid;
      free(xname2);
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
    }
  ret = sqlite3_exec(MainFrame->GetSqlite(), xsql, NULL, NULL, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      error = true;
    }
done:
  if (error == false)
    {
      // marking row as deleted
      varRow->SetDeleted();
      for (i = 0; i < TableView->GetNumberCols(); i++)
        {
          TableView->SetCellTextColour(CurrentEvtRow, i, wxColour(0, 0, 0));
          TableView->SetCellBackgroundColour(CurrentEvtRow, i,
                                             wxColour(128, 128, 128));
          TableView->SetReadOnly(CurrentEvtRow, i);
        }
      TableView->ForceRefresh();
    }
}

void MyResultSetView::OnCmdInsert(wxCommandEvent & WXUNUSED(event))
{
//
// user required row insertion
//
  int i;
  if (ReadOnly == true)
    return;
  if (InsertPending == true)
    {
      // an INSERT is still pending, and the user required actual insertion
      DoInsert(true);
      return;
    }
  InsertRow = new MyRowVariant(TableView->GetNumberCols());
  TableView->MakeCellVisible(TableView->GetNumberRows() - 1, 1);
  InsertPending = true;
  for (i = 1; i < TableView->GetNumberCols(); i++)
    {
      TableView->SetCellValue(TableView->GetNumberRows() - 1, i, wxT(""));
      TableView->SetCellBackgroundColour(TableView->GetNumberRows() - 1, i,
                                         wxColour(255, 255, 255));
      TableView->SetReadOnly(TableView->GetNumberRows() - 1, i, false);
    }
}

void MyResultSetView::OnCmdAbort(wxCommandEvent & WXUNUSED(event))
{
//
// user cancelled current row insertion
//
  int i;
  if (InsertPending)
    {
      InsertPending = false;
      delete InsertRow;
      InsertRow = NULL;
      for (i = 0; i < TableView->GetNumberCols(); i++)
        {
          TableView->SetCellValue(TableView->GetNumberRows() - 1, i, wxT(""));
          TableView->SetCellBackgroundColour(TableView->GetNumberRows() - 1, i,
                                             wxColour(0, 0, 0));
          TableView->SetReadOnly(TableView->GetNumberRows() - 1, i);
        }
    }
}

void MyResultSetView::OnCmdFilter(wxCommandEvent & WXUNUSED(event))
{
//
// setting Edit filters
//
  MainFrame->SqlFiltersComposer();
}

void MyResultSetView::OnCmdClearSelection(wxCommandEvent & WXUNUSED(event))
{
//
// clearing current selection
//
  TableView->ClearSelection();
  BtnRsMapShow->Enable(false);
  BtnRsMapZoom->Enable(false);
}

void MyResultSetView::OnCmdSelectAll(wxCommandEvent & WXUNUSED(event))
{
//
// selecting all
//
  TableView->SelectAll();
  BtnRsMapShow->Enable(false);
  BtnRsMapZoom->Enable(false);
}

void MyResultSetView::OnCmdSelectRow(wxCommandEvent & WXUNUSED(event))
{
//
// selecting the current row
//
  TableView->SelectRow(CurrentEvtRow);
}

void MyResultSetView::OnCmdSelectColumn(wxCommandEvent & WXUNUSED(event))
{
//
// selecting column
//
  TableView->SelectCol(CurrentEvtColumn);
}

void MyResultSetView::OnCmdCopy(wxCommandEvent & WXUNUSED(event))
{
//
// copying the selection into the clipboard
//
  wxString copyData;
  int row;
  int col;
  bool newRow;
  bool firstRow = true;
  for (row = 0; row < TableView->GetNumberRows(); row++)
    {
      newRow = true;
      for (col = 0; col < TableView->GetNumberCols(); col++)
        {
          if (TableView->IsInSelection(row, col) == true)
            {
              // ok, this cell is included into the selection to copy
              if (firstRow == true)
                {
                  newRow = false;
                  firstRow = false;
              } else if (newRow == true)
                {
                  newRow = false;
                  copyData += wxT("\n");
              } else
                copyData += wxT("\t");
              copyData += TableView->GetCellValue(row, col);
            }
        }
    }
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(copyData));
      wxTheClipboard->Close();
    }
}

void MyResultSetView::OnCmdBlob(wxCommandEvent & WXUNUSED(event))
{
//
// exploring some BLOB value
//
  if (!CurrentBlob)
    return;
  BlobExplorerDialog dlg;
  dlg.Create(MainFrame, CurrentBlob->GetBlobSize(), CurrentBlob->GetBlob());
  dlg.ShowModal();
}

void MyResultSetView::OnCmdTilePreview(wxCommandEvent & WXUNUSED(event))
{
//
// showing a Raster Tile Preview
//
  unsigned char *blob;
  int blobSize;
  if (MainFrame->GetTilePreview
      (TileDataDbPrefix, TileDataTable, CurrentTileId, &blob,
       &blobSize) != true)
    return;
  TilePreviewDialog dlg;
  dlg.Create(MainFrame, TileDataDbPrefix, TileDataTable, CurrentTileId,
             blobSize, blob);
  dlg.ShowModal();
}

void MyResultSetView::OnCmdBlobIn(wxCommandEvent & WXUNUSED(event))
{
// importing an external file into a BLOB-value
  FILE *in = NULL;
  char path[2048];
  int rd;
  int maxSize = 1024 * 1024;    // limit BLOB size to 1MB
  wxString fileList;
  wxString rowid;
  wxString sql;
  char *xsql;
  wxString blobValue;
  wxString hex;
  MyVariant *value;
  char dummy[1024];
  bool error = false;
  unsigned char *buffer = NULL;
  int ret;
  char *errMsg = NULL;
  wxString lastDir;
  char *xname;
  char *xname2;
  fileList =
    wxT
    ("BLOB Document (*.jpg;*.jpeg;*.jp2;*.png;*.gif;*.tif;*.pdf;*.zip)|*.jpg;*.jpeg;*.jp2;*.png;*.gif;*.tif;*.pdf;*.zip|");
  fileList +=
    wxT
    ("Image (*.jpg;*.jpeg;*.jp2;*.png;*.gif;*.tif;*.webp)|*.jpg;*.jpeg;*.jp2;*.png;*.gif;*.tif;*.webp|");
  fileList +=
    wxT
    ("JPEG Image (*.jpg;*.jpeg)|*.jpg;*.jpeg|Jpeg2000 Image (*.jp2)|*.jp2|PNG Image (*.png)|*.png|GIF Image (*.gif)|*.gif");
  fileList += wxT("|TIFF Image (*.tif)|*.tif|WEBP Image (*.webp)|*.webp|");
  fileList +=
    wxT("PDF Document (*.pdf)|*.pdf|ZIP Archive|(*.zip)|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("loading a BLOB value"),
                          wxT(""), wxT(""), fileList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      strcpy(path, fileDialog.GetPath().ToUTF8());
      in = fopen(path, "rb");
      if (!in)
        {
          wxMessageBox(wxT("Cannot open '") + fileDialog.GetPath() +
                       wxT("' for reading"), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
          return;
        }
      wxFileName file(fileDialog.GetPath());
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      ::wxBeginBusyCursor();
      buffer = new unsigned char[maxSize];
      rd = fread(buffer, 1, maxSize, in);
      if (rd == maxSize && !(feof(in)))
        {
          // exceding 1MB; it's too big for a BLOB
          wxMessageBox(wxT
                       ("Selected file excedes 1MB; cowardly refusing to load it as a BLOB value ..."),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          goto end;
        }
      if (ferror(in))
        {
          // I/O error
          wxMessageBox(wxT("an I/O error occurred"), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
          goto end;
        }
      //
      // preparing  the SQL UPDATE statement
      //
      value = TableValues->GetValue(CurrentEvtRow, 0);
      sprintf(dummy, FORMAT_64, value->GetIntValue());
      rowid = wxString::FromUTF8(dummy);
      HexBlobValue(buffer, rd, hex);
      sql = wxT("UPDATE ");
      xname = (char *) malloc((TableName.Len() * 4) + 1);
      strcpy(xname, TableName.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT(" SET ");
      xname =
        (char *)
        malloc((TableView->GetColLabelValue(CurrentEvtColumn).Len() * 4) + 1);
      strcpy(xname, TableView->GetColLabelValue(CurrentEvtColumn).ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT(" = ") + hex + wxT(" WHERE ROWID = ") + rowid;
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(MainFrame->GetSqlite(), xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          error = true;
        }
      value = TableValues->GetValue(CurrentEvtRow, CurrentEvtColumn);
      if (error == false)
        {
          // updating the Grid cell
          sprintf(dummy, "BLOB sz=%d ", rd);
          blobValue = wxString::FromUTF8(dummy);
          switch (gaiaGuessBlobType(buffer, rd))
            {
              case GAIA_JPEG_BLOB:
                blobValue += wxT("JPEG image");
                break;
              case GAIA_EXIF_BLOB:
                blobValue += wxT("JPEG-EXIF image");
                break;
              case GAIA_EXIF_GPS_BLOB:
                blobValue += wxT("JPEG-EXIF-GPS image");
                break;
              case GAIA_PNG_BLOB:
                blobValue += wxT("PNG image");
                break;
              case GAIA_GIF_BLOB:
                blobValue += wxT("GIF image");
                break;
              case GAIA_TIFF_BLOB:
                blobValue += wxT("TIFF image");
                break;
              case GAIA_WEBP_BLOB:
                blobValue += wxT("WEBP image");
                break;
              case GAIA_JP2_BLOB:
                blobValue += wxT("JP2 image (Jpeg2000)");
                break;
              case GAIA_PDF_BLOB:
                blobValue += wxT("PDF document");
                break;
              case GAIA_ZIP_BLOB:
                blobValue += wxT("ZIP archive");
                break;
              default:
                // testing for an eventual Text Font
                int ret = rl2_is_valid_encoded_font(buffer, rd);
                if (ret == RL2_OK)
                  blobValue += wxT("TrueType Font");
                else
                  blobValue += wxT("UNKNOWN type");
                break;
            };
          TableView->SetCellValue(CurrentEvtRow, CurrentEvtColumn, blobValue);
          TableView->SetCellTextColour(CurrentEvtRow, CurrentEvtColumn,
                                       wxColour(0, 0, 192));
          TableView->SetCellBackgroundColour(CurrentEvtRow, CurrentEvtColumn,
                                             wxColour(255, 255, 204));
          TableView->ForceRefresh();
        }
    }
end:
// clean-up
  ::wxEndBusyCursor();
  if (in)
    fclose(in);
  if (buffer)
    delete[]buffer;
}

void MyResultSetView::HexBlobValue(unsigned char *blob, int size,
                                   wxString & hex)
{
//
// builds the HEX BLOB as X'01234567890abcdef'
//
  int i;
  char digit[16];
  hex = wxT("X'");
  for (i = 0; i < size; i++)
    {
      sprintf(digit, "%02x", *(blob + i));
      hex += wxString::FromUTF8(digit);
    }
  hex += wxT("'");
}

void MyResultSetView::OnCmdBlobOut(wxCommandEvent & WXUNUSED(event))
{
// exporting to external file a BLOB-value
  int blobType;
  wxString fileName;
  wxString fileType;
  int ret;
  wxString path;
  FILE *out = NULL;
  char xpath[2048];
  int wr;
  wxString lastDir;
  if (!CurrentBlob)
    return;
  blobType =
    gaiaGuessBlobType(CurrentBlob->GetBlob(), CurrentBlob->GetBlobSize());
  switch (blobType)
    {
      case GAIA_JPEG_BLOB:
      case GAIA_EXIF_BLOB:
      case GAIA_EXIF_GPS_BLOB:
        fileName = wxT("image.jpg");
        fileType = wxT("File JPEG (*.jpg;*.jpeg)|*.jpg");
        break;
      case GAIA_JP2_BLOB:
        fileName = wxT("image.jp2");
        fileType = wxT("File Jpeg2000 (*.jp2)|*.jp2");
        break;
      case GAIA_PNG_BLOB:
        fileName = wxT("image.png");
        fileType = wxT("File PNG (*.png)|*.png");
        break;
      case GAIA_GIF_BLOB:
        fileName = wxT("image.gif");
        fileType = wxT("File GIF (*.gif)|*.gif");
        break;
      case GAIA_TIFF_BLOB:
        fileName = wxT("image.tiff");
        fileType = wxT("File TIFF (*.tif)|*.tif");
        break;
      case GAIA_PDF_BLOB:
        fileName = wxT("document.pdf");
        fileType = wxT("PDF document (*.pdf)|*.pdf");
        break;
      case GAIA_ZIP_BLOB:
        fileName = wxT("archive.zip");
        fileType = wxT("ZIP Archive (*.zip)|*.zip");
        break;
      default:
        fileName = wxT("file");
    };
  fileType += wxT("|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("exporting a BLOB value to file"),
                          wxT(""), fileName, fileType,
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      switch (blobType)
        {
          case GAIA_JPEG_BLOB:
          case GAIA_EXIF_BLOB:
          case GAIA_EXIF_GPS_BLOB:
            path += wxT(".jpg");
            break;
          case GAIA_JP2_BLOB:
            path += wxT(".jp2");
            break;
          case GAIA_PNG_BLOB:
            path += wxT(".png");
            break;
          case GAIA_GIF_BLOB:
            path += wxT(".gif");
            break;
          case GAIA_TIFF_BLOB:
            path += wxT(".tif");
            break;
          case GAIA_PDF_BLOB:
            path += wxT(".pdf");
            break;
          case GAIA_ZIP_BLOB:
            path += wxT(".zip");
            break;
          default:
            path += file.GetExt();
        };
      strcpy(xpath, path.ToUTF8());
      out = fopen(xpath, "wb");
      if (!out)
        {
          wxMessageBox(wxT("Cannot open '") + path + wxT("' for writing"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          return;
        }
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      ::wxBeginBusyCursor();
      wr = fwrite(CurrentBlob->GetBlob(), 1, CurrentBlob->GetBlobSize(), out);
      if (wr != CurrentBlob->GetBlobSize())
        {
          wxMessageBox(wxT("an I/O error occurred"), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
        }
      fclose(out);
      ::wxEndBusyCursor();
    }
}

void MyResultSetView::OnCmdXmlBlobIn(wxCommandEvent & WXUNUSED(event))
{
// importing an external XMLDocument into an XmlBLOB-value
#ifdef ENABLE_LIBXML2           // only if LIBXML2 is enabled
  FILE *in = NULL;
  char path[2048];
  int rd;
  int maxSize = 1024 * 1024;    // limit XmlBLOB size to 1MB
  wxString fileList;
  wxString rowid;
  wxString sql;
  char *xsql;
  wxString blobValue;
  wxString hex;
  MyVariant *value;
  char dummy[1024];
  bool error = false;
  unsigned char *buffer = NULL;
  int ret;
  char *errMsg = NULL;
  wxString lastDir;
  char *xname;
  char *xname2;
  unsigned char *xml = NULL;
  int xml_size;
  int compressed;
  char *schemaURI;
  char xschema[8192];
  fileList = wxT("XML Document (*.xml)|*.xml|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("loading an XmlBLOB value"),
                          wxT(""), wxT(""), fileList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      strcpy(path, fileDialog.GetPath().ToUTF8());
      in = fopen(path, "rb");
      if (!in)
        {
          wxMessageBox(wxT("Cannot open '") + fileDialog.GetPath() +
                       wxT("' for reading"), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
          return;
        }
      wxFileName file(fileDialog.GetPath());
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      ::wxBeginBusyCursor();
      buffer = new unsigned char[maxSize];
      rd = fread(buffer, 1, maxSize, in);
      if (rd == maxSize && !(feof(in)))
        {
          // exceding 1MB; it's too big for a BLOB
          wxMessageBox(wxT
                       ("Selected file excedes 1MB; cowardly refusing to load it as an XmlBLOB value ..."),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          goto end;
        }
      if (ferror(in))
        {
          // I/O error
          wxMessageBox(wxT("an I/O error occurred"), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
          goto end;
        }

      LoadXmlDialog dlg;
      wxString inPath = fileDialog.GetPath();
      dlg.Create(MainFrame, inPath);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          if (dlg.IsCompressed() == true)
            compressed = 1;
          else
            compressed = 0;
          wxString schema = dlg.GetSchemaURI();
          if (schema.Len() == 0)
            schemaURI = NULL;
          else
            {
              strcpy(xschema, schema.ToUTF8());
              schemaURI = xschema;
            }
      } else
        goto end;

// attempting to parse (and possibly validate) the XML
      gaiaXmlToBlob(MainFrame->GetSpliteInternalCache(), buffer, rd, compressed,
                    schemaURI, &xml, &xml_size, NULL, NULL);
      if (xml == NULL)
        {
          wxMessageBox(wxT("Invalid XML ... unable to parse or validate"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          goto end;
        }
//
// preparing  the SQL UPDATE statement
//
      value = TableValues->GetValue(CurrentEvtRow, 0);
      sprintf(dummy, FORMAT_64, value->GetIntValue());
      rowid = wxString::FromUTF8(dummy);
      HexBlobValue(xml, xml_size, hex);
      sql = wxT("UPDATE ");
      xname = (char *) malloc((TableName.Len() * 4) + 1);
      strcpy(xname, TableName.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT(" SET ");
      xname =
        (char *)
        malloc((TableView->GetColLabelValue(CurrentEvtColumn).Len() * 4) + 1);
      strcpy(xname, TableView->GetColLabelValue(CurrentEvtColumn).ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT(" = ") + hex + wxT(" WHERE ROWID = ") + rowid;
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(MainFrame->GetSqlite(), xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          error = true;
        }
      value = TableValues->GetValue(CurrentEvtRow, CurrentEvtColumn);
      if (error == false)
        {
          // updating the Grid cell
          int doc_size = gaiaXmlBlobGetDocumentSize(xml, xml_size);
          sprintf(dummy, "XmlBLOB sz=%d (XMLsz=%d) ", xml_size, doc_size);
          blobValue = wxString::FromUTF8(dummy);
          if (gaiaIsSchemaValidatedXmlBlob(xml, xml_size))
            blobValue += wxT(" SchemaValidated");
          TableView->SetCellValue(CurrentEvtRow, CurrentEvtColumn, blobValue);
          TableView->SetCellTextColour(CurrentEvtRow, CurrentEvtColumn,
                                       wxColour(0, 0, 192));
          TableView->SetCellBackgroundColour(CurrentEvtRow, CurrentEvtColumn,
                                             wxColour(255, 255, 204));
          TableView->ForceRefresh();
        }
    }
end:
// clean-up
  ::wxEndBusyCursor();
  if (in)
    fclose(in);
  if (buffer)
    delete[]buffer;
  if (xml)
    free(xml);

#else

  wxMessageBox(wxT
               ("Sorry, spatialite_gui was built disabling LIBXML2\n\nUnsupported operation"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);

#endif // end LIBXML2 conditionals
}

void MyResultSetView::XmlBlobOut(bool indented)
{
// exporting to external file an XmlBLOB-value
#ifdef ENABLE_LIBXML2           // only if LIBXML2 is enabled
  wxString fileName;
  wxString fileType;
  int ret;
  wxString path;
  FILE *out = NULL;
  char xpath[2048];
  int wr;
  wxString lastDir;
  unsigned char *xml;
  int xml_size;
  wxString title = wxT("exporting an XmlBLOB value to file ");
  if (!CurrentBlob)
    return;
  if (indented)
    {
      title += wxT("(indented)");
      gaiaXmlFromBlob(CurrentBlob->GetBlob(), CurrentBlob->GetBlobSize(), 1,
                      &xml, &xml_size);
  } else
    {
      title += wxT("(not indented)");
      gaiaXmlFromBlob(CurrentBlob->GetBlob(), CurrentBlob->GetBlobSize(), 0,
                      &xml, &xml_size);
    }
  if (xml == NULL)
    return;
  fileName = wxT("document.xml");
  fileType = wxT("XML Document (*.xml)|*.xml");
  fileType += wxT("|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, title,
                          wxT(""), fileName, fileType,
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".xml");
      strcpy(xpath, path.ToUTF8());
      out = fopen(xpath, "wb");
      if (!out)
        {
          wxMessageBox(wxT("Cannot open '") + path + wxT("' for writing"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          free(xml);
          return;
        }
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      ::wxBeginBusyCursor();
      wr = fwrite(xml, 1, xml_size, out);
      if (wr != xml_size)
        {
          wxMessageBox(wxT("an I/O error occurred"), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
        }
      fclose(out);
      ::wxEndBusyCursor();
    }
  free(xml);

#else

  wxMessageBox(wxT
               ("Sorry, spatialite_gui was built disabling LIBXML2\n\nUnsupported operation"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);

#endif // end LIBXML2 conditionals
}

void MyResultSetView::OnCmdXmlBlobOut(wxCommandEvent & WXUNUSED(event))
{
// exporting to external file an XmlBLOB-value (not indented)
  XmlBlobOut(false);
}

void MyResultSetView::OnCmdXmlBlobOutIndented(wxCommandEvent & WXUNUSED(event))
{
// exporting to external file an XmlBLOB-value (indented)
  XmlBlobOut(true);
}

void MyResultSetView::OnCmdBlobNull(wxCommandEvent & WXUNUSED(event))
{
// setting to NULL a BLOB-value
  wxString rowid;
  wxString sql;
  char *xsql;
  int ret;
  char *errMsg = NULL;
  bool error = false;
  MyVariant *value;
  char dummy[256];
  char *xname;
  char *xname2;
  value = TableValues->GetValue(CurrentEvtRow, 0);
  sprintf(dummy, FORMAT_64, value->GetIntValue());
  rowid = wxString::FromUTF8(dummy);
  sql = wxT("UPDATE ");
  xname = (char *) malloc((TableName.Len() * 4) + 1);
  strcpy(xname, TableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT(" SET ");
  xname =
    (char *) malloc((TableView->GetColLabelValue(CurrentEvtColumn).Len() * 4) +
                    1);
  strcpy(xname, TableView->GetColLabelValue(CurrentEvtColumn).ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT(" = NULL WHERE ROWID = ") + rowid;
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(MainFrame->GetSqlite(), xsql, NULL, NULL, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      error = true;
    }
  if (error == false)
    {
      // updating the Grid cell
      TableView->SetCellValue(CurrentEvtRow, CurrentEvtColumn, wxT("NULL"));
      TableView->SetCellTextColour(CurrentEvtRow, CurrentEvtColumn,
                                   wxColour(0, 0, 192));
      TableView->SetCellBackgroundColour(CurrentEvtRow, CurrentEvtColumn,
                                         wxColour(255, 255, 204));
      TableView->ForceRefresh();
    }
}

void MyResultSetView::OnCmdExpTxtTab(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the ResultSet as TXT-TAB
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  wxFileDialog fileDialog(this, wxT("Exporting the ResultSet as Txt/Tab file"),
                          wxT(""), wxT("result_set.txt"),
                          wxT("Txt/Tab file (*.txt)|*.txt|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".txt");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->ExportResultSetAsTxtTab(path, sql,
                                             MainFrame->GetDefaultCharset());
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("TXT / TAB");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->ExportResultSetAsTxtTab(path, sql, dlg.GetCharset());
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyResultSetView::OnCmdExpCsv(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the ResultSet as CSV
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  wxFileDialog fileDialog(this, wxT("Exporting the ResultSet as CSV"),
                          wxT(""), wxT("result_set.csv"),
                          wxT("CSV file (*.csv)|*.csv|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".csv");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->ExportResultSetAsCsv(path, sql,
                                          MainFrame->GetDefaultCharset());
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("CSV");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->ExportResultSetAsCsv(path, sql, dlg.GetCharset());
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyResultSetView::OnCmdExpHtml(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the ResultSet as HTML
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  wxFileDialog fileDialog(this, wxT("Exporting the ResultSet as HTML"),
                          wxT(""), wxT("result_set.html"),
                          wxT
                          ("HTML web page (*.html)|*.html|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".html");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->ExportResultSetAsHtml(path, sql,
                                           MainFrame->GetSqlitePath(),
                                           MainFrame->GetDefaultCharset());
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("HTML");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->ExportResultSetAsHtml(path, sql,
                                               MainFrame->GetSqlitePath(),
                                               dlg.GetCharset());
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyResultSetView::OnCmdExpShp(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the ResultSet as Shapefile
//
  int ret;
  const void *proj_ctx = NULL;
  wxString path;
  wxString lastDir;
  wxString target;
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  wxFileDialog fileDialog(this, wxT("Exporting the ResultSet as Shapefile"),
                          wxT(""), wxT("shapefile.shp"),
                          wxT
                          ("Shapefile (*.shp)|*.shp|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {

      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      lastDir = file.GetPath();

#ifdef PROJ_NEW                 // only when using new PROJ.6
      // attempting to retrieve the current PROJ.6 context
      proj_ctx = gaiaGetCurrentProjContext(MainFrame->GetSpliteInternalCache());
#endif

      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->ExportResultSetAsShp(proj_ctx, path, sql,
                                          MainFrame->GetDefaultCharset());
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("Shapefile");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->ExportResultSetAsShp(proj_ctx, path, sql,
                                              dlg.GetCharset());
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyResultSetView::OnCmdExpDif(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the ResultSet as DIF spreadsheet
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
// asking Decimal Point / Date-Times params
  DumpSpreadsheetDialog sheet_dlg;
  char decimal_point;
  bool date_times;
  sheet_dlg.Create(MainFrame);
  ret = sheet_dlg.ShowModal();
  if (ret == wxID_OK)
    {
      decimal_point = sheet_dlg.GetDecimalPoint();
      date_times = sheet_dlg.IsDateTimes();
  } else
    return;
  wxFileDialog fileDialog(this,
                          wxT("Exporting the ResultSet as DIF spreadsheet"),
                          wxT(""), wxT("spreadsheet.dif"),
                          wxT
                          ("DIF spreadsheet document (*.dif)|*.dif|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {

      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".dif");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->ExportResultSetAsDif(path, sql,
                                          MainFrame->GetDefaultCharset(),
                                          decimal_point, date_times);
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("DIF spreadsheet");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->ExportResultSetAsDif(path, sql, dlg.GetCharset(),
                                              decimal_point, date_times);
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyResultSetView::OnCmdExpSylk(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the ResultSet as SYLK spreadsheet
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
// asking Decimal Point / Date-Times params
  DumpSpreadsheetDialog sheet_dlg;
  bool date_times;
  sheet_dlg.Create(MainFrame);
  ret = sheet_dlg.ShowModal();
  if (ret == wxID_OK)
    date_times = sheet_dlg.IsDateTimes();
  else
    return;
  wxFileDialog fileDialog(this,
                          wxT("Exporting the ResultSet as SYLK spreadsheet"),
                          wxT(""), wxT("spreadsheet.slk"),
                          wxT
                          ("SYLK spreadsheet document (*.slk)|*.slk|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {

      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".slk");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->ExportResultSetAsSylk(path, sql,
                                           MainFrame->GetDefaultCharset(),
                                           date_times);
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("SYLK spreadsheet");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->ExportResultSetAsSylk(path, sql, dlg.GetCharset(),
                                               date_times);
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyResultSetView::OnCmdExpDbf(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the ResultSet as DBF archive
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  wxFileDialog fileDialog(this, wxT("Exporting the ResultSet as DBF archive"),
                          wxT(""), wxT("archive.dbf"),
                          wxT
                          ("DBF archive (*.dbf)|*.dbf|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {

      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".dbf");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->ExportResultSetAsDbf(path, sql,
                                          MainFrame->GetDefaultCharset());
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("DBF archive");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->ExportResultSetAsDbf(path, sql, dlg.GetCharset());
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyResultSetView::OnCmdExpXlsx(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the ResultSet as MS Excell shreadsheet (xlsx)
//
#ifdef ENABLE_XLSXWRITER	// only if XlsxWriter is supported
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  wxString sql = MainFrame->GetQueryView()->GetSqlCtrl()->GetValue();
  wxFileDialog fileDialog(this,
                          wxT("Exporting the ResultSet as MS Excell XLSX"),
                          wxT(""), wxT("archive.xlsx"),
                          wxT
                          ("MS Excell spreadsheet (*.xlsx)|*.xlsx|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".xlsx");
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      ::wxBeginBusyCursor();
      MainFrame->DoExportXLSX(path, sql);
      ::wxEndBusyCursor();
    }
#else
    wxMessageBox(wxT("This copy of SpatialiteGUI has been buil by disabling libXlsxWriter support\n... Sorry, cowardly quitting"), 
    wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                 MainFrame);
#endif
}
