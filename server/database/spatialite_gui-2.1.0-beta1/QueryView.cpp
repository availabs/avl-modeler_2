/*
/ QueryView.cpp
/ a panel to set SQL queries
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

//
// ICONs in XPM format [universally portable]
//
#include "icons/sql_go.xpm"
#include "icons/filter.xpm"
#include "icons/sql_erase.xpm"
#include "icons/sql_abort.xpm"
#include "icons/sql_abort_no.xpm"
#include "icons/hs_back.xpm"
#include "icons/hs_back_no.xpm"
#include "icons/hs_forward.xpm"
#include "icons/hs_forward_no.xpm"

MyQueryView::MyQueryView(MyFrame * parent, wxWindowID id):
wxPanel(parent, id, wxDefaultPosition, wxSize(440, 120), wxBORDER_SUNKEN)
{
//
// constructor: a frame for SQL Queries
//
  MainFrame = parent;
  BracketStart = -1;
  BracketEnd = -1;
  IgnoreEvent = false;
  MainFrame->DoResetSqlFilters();
// SQL statement
  SqlCtrl =
    new MySqlControl(this, ID_SQL, wxT(""), wxPoint(40, 5),
                     wxSize(20, 20),
                     wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB |
                     wxHSCROLL | wxTE_RICH);
  BtnSqlGo =
    new wxBitmapButton(this, ID_SQL_GO, wxBitmap(sql_go_xpm), wxPoint(340, 5),
                       wxSize(32, 32));
  BtnSqlGo->SetToolTip(wxT("Execute SQL statement"));
  BtnSqlFilter =
    new wxBitmapButton(this, ID_SQL_FILTER, wxBitmap(filter_xpm),
                       wxPoint(340, 38), wxSize(32, 32));
  BtnSqlFilter->SetToolTip(wxT("Apply/Remove SQL filters"));
  BtnSqlFilter->Enable(false);
  BtnSqlErase =
    new wxBitmapButton(this, ID_SQL_ERASE, wxBitmap(sql_erase_xpm),
                       wxPoint(340, 70), wxSize(32, 32));
  BtnSqlErase->SetToolTip(wxT("Clear SQL query"));
  BtnSqlErase->Enable(true);
  BtnSqlAbort =
    new wxBitmapButton(this, ID_SQL_ABORT, wxBitmap(sql_abort_xpm),
                       wxPoint(340, 102), wxSize(32, 32));
  BtnSqlAbort->SetBitmapDisabled(wxBitmap(sql_abort_no_xpm));
  BtnSqlAbort->SetToolTip(wxT("Abort SQL query"));
  BtnHistoryBack =
    new wxBitmapButton(this, ID_HISTORY_BACK, wxBitmap(hs_back_xpm),
                       wxPoint(5, 5), wxSize(32, 32));
  BtnHistoryBack->SetBitmapDisabled(wxBitmap(hs_back_no_xpm));
  BtnHistoryBack->SetToolTip(wxT("History: previous SQL statement"));
  BtnHistoryForward =
    new wxBitmapButton(this, ID_HISTORY_FORWARD, wxBitmap(hs_forward_xpm),
                       wxPoint(5, 40), wxSize(32, 32));
  BtnHistoryForward->SetBitmapDisabled(wxBitmap(hs_forward_no_xpm));
  BtnHistoryForward->SetToolTip(wxT("History: next SQL statement"));
  SetHistoryStates();
  BtnSqlAbort->Enable(false);

// setting up Keyboard Shortcuts
  wxAcceleratorEntry entries[3];
  entries[0].Set(wxACCEL_NORMAL, WXK_F5, ID_SQL_GO);
  entries[1].Set(wxACCEL_NORMAL, WXK_PAGEUP, ID_HISTORY_BACK);
  entries[2].Set(wxACCEL_NORMAL, WXK_PAGEDOWN, ID_HISTORY_FORWARD);
  wxAcceleratorTable accel(3, entries);
  SetAcceleratorTable(accel);

// setting up event handlers
  Connect(ID_SQL_GO, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyQueryView::OnSqlGo);
  Connect(ID_SQL_FILTER, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyQueryView::OnSqlFilter);
  Connect(ID_SQL_ERASE, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyQueryView::OnSqlErase);
  Connect(ID_SQL_ABORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyQueryView::OnSqlAbort);
  Connect(ID_HISTORY_BACK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyQueryView::OnHistoryBack);
  Connect(ID_HISTORY_FORWARD, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyQueryView::OnHistoryForward);
  Connect(wxID_ANY, wxEVT_SIZE, (wxObjectEventFunction) & MyQueryView::OnSize);
  Connect(wxID_ANY, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) & MyQueryView::OnSqlSyntaxColor);
}

void MyQueryView::ShowControls()
{
//
// making all SQL controls to be visible
//
  SqlCtrl->Show(true);
  BtnSqlGo->Show(true);
  BtnSqlFilter->Show(true);
  BtnSqlErase->Show(true);
  BtnSqlAbort->Show(true);
  BtnHistoryBack->Show(true);
  BtnHistoryForward->Show(true);
  SetHistoryStates();
}

void MyQueryView::HideControls()
{
//
// making all controls to be invisible
//
  SqlCtrl->Show(false);
  BtnSqlGo->Show(false);
  BtnSqlFilter->Show(false);
  BtnSqlErase->Show(false);
  BtnSqlAbort->Show(false);
  BtnHistoryBack->Show(false);
  BtnHistoryForward->Show(false);

}

void MyQueryView::AddToHistory(wxString & sql)
{
//
// adds an SQL statement to history
//
  History.Add(sql);
  SetHistoryStates();
}

void MyQueryView::SetHistoryStates()
{
//
// updates the history buttons state
//
  BtnHistoryForward->Enable(History.TestNext());
  BtnHistoryBack->Enable(History.TestPrev());
}

void MyQueryView::SetSql(wxString & sql, bool execute)
{
  bool coverage = false;
  wxString tile_data_table;
  wxString tile_data_db_prefix;
  SetSql(sql, execute, coverage, tile_data_db_prefix, tile_data_table, true);
}

void MyQueryView::SetSql(wxString & sql, bool execute, bool coverage,
                         wxString & tile_data_db_prefix,
                         wxString & tile_data_table, bool reset)
{
//
// sets an SQL statement [and maybe executes it]
//
  int metaDataType = MainFrame->GetMetaDataType();
  SqlCtrl->SetValue(sql);
  if (execute == true)
    {
      if (metaDataType == METADATA_CURRENT)
        {
          // current metadata style >= v.4.0.0
          MainFrame->InsertIntoLog(sql);
        }
      if (MainFrame->GetRsView()->ExecuteSqlPre(sql, 0, true, coverage,
                                                tile_data_db_prefix,
                                                tile_data_table,
                                                reset) == false)
        {
          if (metaDataType == METADATA_CURRENT)
            {
              // current metadata style >= v.4.0.0
              MainFrame->UpdateLog(MainFrame->GetRsView()->GetSqlErrorMsg());
            }
          wxMessageBox(MainFrame->GetRsView()->GetSqlErrorMsg(),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, MainFrame);
      } else
        {
          if (metaDataType == METADATA_CURRENT)
            {
              // current metadata style >= v.4.0.0
              MainFrame->UpdateLog();
            }
        }
    }
}

void MyQueryView::OnSize(wxSizeEvent & WXUNUSED(event))
{
//
// this window has changed its size
//
  int vert;
  int vertBack;
  wxSize sz = GetClientSize();
// setting the SQL statement pane size
  SqlCtrl->SetSize(sz.GetWidth() - 113, sz.GetHeight() - 10);
// setting the SQL GO button position
  BtnSqlGo->Move(sz.GetWidth() - 35, 5);
// setting the SQL GO button size
  vert = sz.GetHeight() - 10;
  if (vert < 32)
    vert = 32;
  BtnSqlGo->SetSize(32, vert);
  vert = (sz.GetHeight() - 18) / 3;
// setting the SQL FILTER button position
  BtnSqlFilter->Move(sz.GetWidth() - 70, 5);
// setting the SQL FILTER button size
  BtnSqlFilter->SetSize(32, vert);
// setting the SQL ERASE button position
  BtnSqlErase->Move(sz.GetWidth() - 70, 10 + vert);
// setting the SQL ERASE button size
  BtnSqlErase->SetSize(32, vert);
// setting the SQL ABORT button position
  BtnSqlAbort->Move(sz.GetWidth() - 70, 15 + (vert * 2));
// setting the SQL ABORT button size
  BtnSqlAbort->SetSize(32, vert);
// setting the HISTORY BACK button position
  BtnHistoryBack->Move(5, 5);
// setting the HISTORY BACK button size
  vert = (sz.GetHeight() - 15) / 2;
  if (vert < 32)
    vert = 32;
  BtnHistoryBack->SetSize(32, vert);
  vertBack = 10 + vert;
// setting the HISTORY FORWARD button position
  BtnHistoryForward->Move(5, vertBack);
// setting the HISTORY FORWARD button size
  BtnHistoryForward->SetSize(32, vert);
}

void MyQueryView::OnSqlGo(wxCommandEvent & WXUNUSED(event))
{
//
// executing an SQL statement
//
  wxString tile_data_db_prefix;
  wxString tile_data_name;
  int metaDataType = MainFrame->GetMetaDataType();
  wxString sql = SqlCtrl->GetValue();
  if (metaDataType == METADATA_CURRENT)
    {
      // current metadata style >= v.4.0.0
      MainFrame->InsertIntoLog(sql);
    }
  if (MainFrame->
      GetRsView()->ExecuteSqlPre(sql, 0, true, false, tile_data_db_prefix,
                                 tile_data_name, true) == false)
    {
      if (metaDataType == METADATA_CURRENT)
        {
          // current metadata style >= v.4.0.0
          MainFrame->UpdateLog(MainFrame->GetRsView()->GetSqlErrorMsg());
        }
      wxMessageBox(MainFrame->GetRsView()->GetSqlErrorMsg(),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, MainFrame);
  } else
    {
      if (metaDataType == METADATA_CURRENT)
        {
          // current metadata style >= v.4.0.0
          MainFrame->UpdateLog();
        }
    }
  MainFrame->GetQueryView()->DisableFilterButton();
  MainFrame->DoResetSqlFilters();
}

void MyQueryView::OnSqlFilter(wxCommandEvent & WXUNUSED(event))
{
//
// managing SQL filters
//
  MainFrame->SqlFiltersComposer();
}

void MyQueryView::OnSqlErase(wxCommandEvent & WXUNUSED(event))
{
//
// cleaning the current SQL query
//
  wxString empty;
  MainFrame->GetRsView()->ResetEmpty();
  SqlCtrl->Clear();
}

void MyQueryView::OnSqlAbort(wxCommandEvent & WXUNUSED(event))
{
//
// aborting the current SQL query
//
  MainFrame->GetRsView()->AbortRequested();
  if (MainFrame->GetMetaDataType() == METADATA_CURRENT)
    {
      // current metadata style >= v.4.0.0
      MainFrame->UpdateAbortedLog();
    }
}

void MyQueryView::OnHistoryBack(wxCommandEvent & WXUNUSED(event))
{
//
// going backward into the SQL Queries History
//
  MySqlQuery *sql = History.GetPrev();
  if (sql)
    {
      MainFrame->GetRsView()->ResetEmpty();
      SetSql(sql->GetSql(), false);
      SetHistoryStates();
    }
}

void MyQueryView::OnHistoryForward(wxCommandEvent & WXUNUSED(event))
{
//
// going forward into the SQL Queries History
//
  MySqlQuery *sql = History.GetNext();
  if (sql)
    {
      MainFrame->GetRsView()->ResetEmpty();
      SetSql(sql->GetSql(), false);
      SetHistoryStates();
    }
}

bool MyQueryView::IsSqlString(wxString & str)
{
// checks if this one is an SQL string constant
  char *word = new char[str.Len() * 4];
  strcpy(word, str.ToUTF8());
  int len = strlen(word);
  if (len < 2)
    goto no;
  if (word[0] == '\'' && word[len - 1] == '\'')
    goto yes;
  if (word[0] == '"' && word[len - 1] == '"')
    goto yes;
no:
  delete[]word;
  return false;
yes:
  delete[]word;
  return true;
}

bool MyQueryView::IsSqlNumber(wxString & str)
{
// checks if this one is an SQL numeric constant
  double dbl;
  return str.ToDouble(&dbl);
}

bool MyQueryView::IsSqliteExtra(wxString & str)
{
// checks if this one is an extra SQLite keyword
  if (str.CmpNoCase(wxT("add")) == 0)
    return true;
  if (str.CmpNoCase(wxT("asc")) == 0)
    return true;
  if (str.CmpNoCase(wxT("attach")) == 0)
    return true;
  if (str.CmpNoCase(wxT("column")) == 0)
    return true;
  if (str.CmpNoCase(wxT("database")) == 0)
    return true;
  if (str.CmpNoCase(wxT("detach")) == 0)
    return true;
  if (str.CmpNoCase(wxT("desc")) == 0)
    return true;
  if (str.CmpNoCase(wxT("null")) == 0)
    return true;
  if (str.CmpNoCase(wxT("trigger")) == 0)
    return true;
  if (str.CmpNoCase(wxT("for")) == 0)
    return true;
  if (str.CmpNoCase(wxT("each")) == 0)
    return true;
  if (str.CmpNoCase(wxT("row")) == 0)
    return true;
  if (str.CmpNoCase(wxT("begin")) == 0)
    return true;
  if (str.CmpNoCase(wxT("end")) == 0)
    return true;
  if (str.CmpNoCase(wxT("before")) == 0)
    return true;
  if (str.CmpNoCase(wxT("after")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rename")) == 0)
    return true;
  if (str.CmpNoCase(wxT("virtual")) == 0)
    return true;
  if (str.CmpNoCase(wxT("with")) == 0)
    return true;
  if (str.CmpNoCase(wxT("recursive")) == 0)
    return true;
  if (str.CmpNoCase(wxT("vacuum")) == 0)
    return true;
  if (str.CmpNoCase(wxT("analize")) == 0)
    return true;
  return false;
}

bool MyQueryView::IsSqlFunction(wxString & str, char next_c)
{
// checks if this one is an SQL function
  if (next_c != '(')
    return false;
  if (str.CmpNoCase(wxT("raise")) == 0)
    return true;
  if (str.CmpNoCase(wxT("avg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("count")) == 0)
    return true;
  if (str.CmpNoCase(wxT("group_concat")) == 0)
    return true;
  if (str.CmpNoCase(wxT("max")) == 0)
    return true;
  if (str.CmpNoCase(wxT("min")) == 0)
    return true;
  if (str.CmpNoCase(wxT("sum")) == 0)
    return true;
  if (str.CmpNoCase(wxT("total")) == 0)
    return true;
  if (str.CmpNoCase(wxT("abs")) == 0)
    return true;
  if (str.CmpNoCase(wxT("changes")) == 0)
    return true;
  if (str.CmpNoCase(wxT("char")) == 0)
    return true;
  if (str.CmpNoCase(wxT("coalesce")) == 0)
    return true;
  if (str.CmpNoCase(wxT("glob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ifnull")) == 0)
    return true;
  if (str.CmpNoCase(wxT("instr")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("last_insert_rowid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("length")) == 0)
    return true;
  if (str.CmpNoCase(wxT("load_extension")) == 0)
    return true;
  if (str.CmpNoCase(wxT("lower")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ltrim")) == 0)
    return true;
  if (str.CmpNoCase(wxT("nullif")) == 0)
    return true;
  if (str.CmpNoCase(wxT("quote")) == 0)
    return true;
  if (str.CmpNoCase(wxT("random")) == 0)
    return true;
  if (str.CmpNoCase(wxT("randomblob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("replace")) == 0)
    return true;
  if (str.CmpNoCase(wxT("round")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rtrim")) == 0)
    return true;
  if (str.CmpNoCase(wxT("soundex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("sqlite_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("substr")) == 0)
    return true;
  if (str.CmpNoCase(wxT("trim")) == 0)
    return true;
  if (str.CmpNoCase(wxT("typeof")) == 0)
    return true;
  if (str.CmpNoCase(wxT("unicode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("upper")) == 0)
    return true;
  if (str.CmpNoCase(wxT("zeroblob")) == 0)
    return true;
  return false;
}

bool MyQueryView::IsSqlGeoFunction(wxString & str, char next_c)
{
// checks if this one is an SQL geo-function
  if (next_c != '(')
    return false;
  if (str.CmpNoCase(wxT("spatialite_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("spatialite_target_cpu")) == 0)
    return true;
  if (str.CmpNoCase(wxT("check_strict_sql_quoting")) == 0)
    return true;
  if (str.CmpNoCase(wxT("freexl_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("geos_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("proj4_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("proj_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rttopo_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("libxml2_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasIconv")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasMathSql")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasGeoCallbacks")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasGeos")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasProj")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasProj6")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasProjGeodesic")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasGeosAdvanced")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasGeosTrunk")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasGeosReentrant")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasGeosOnlyReentrant")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasMiniZip")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasRtTopo")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasEpsg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasFreeXL")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasLibXML2")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasGeoPackage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasGGP")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasGroundControlPoints")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasTopology")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasKNN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("hasRouting")) == 0)
    return true;

  if (str.CmpNoCase(wxT("EnableGpkgAmphibiousMode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DisableGpkgAmphibiousMode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetGpkgAmphibiousMode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("EnableGpkgMode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DisableGpkgMode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetGpkgMode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("EnableTinyPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DisableTInyPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsTinyPointEnabled")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetDecimalPrecision")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetDecimalPrecision")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetVirtualShapeExtent")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetVirtualGeoJsonExtent")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetVirtualTableExtent")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BufferOptions_Reset")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BufferOptions_SetEndCapStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BufferOptions_GetEndCapStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BufferOptions_SetJoinStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BufferOptions_GetJoinStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BufferOptions_SetMitreLimit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BufferOptions_GetMitreLimit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BufferOptions_SetQuadrantSegments")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BufferOptions_GetQuadrantSegments")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsLowASCII")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_CreateTables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_RegisterGetCapabilities")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_UnRegisterGetCapabilities")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_SetGetCapabilitiesInfos")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_RegisterGetMap")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_UnRegisterGetMap")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_SetGetMapInfos")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_SetGetMapCopyright")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_SetGetMapOptions")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_RegisterSetting")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_UnRegisterSetting")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_DefaultSetting")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_RegisterRefSys")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_UnRegisterRefSys")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_DefaultRefSys")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_GetMapRequestURL")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WMS_GetFeatureInfoRequestURL")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RegisterDataLicense")) == 0)
    return true;
  if (str.CmpNoCase(wxT("UnRegisterDataLicense")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RenameDataLicense")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetDataLicenseUrl")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeometryConstraints")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CheckSpatialMetaData")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AutoFDOStart")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AutoFDOStop")) == 0)
    return true;
  if (str.CmpNoCase(wxT("InitFDOSpatialMetaData")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AddFDOGeometryColumn")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RecoverFDOGeometryColumn")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DiscardFDOGeometryColumn")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetDbObjectScope")) == 0)
    return true;
  if (str.CmpNoCase(wxT("EnablePause")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DisablePause")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsPauseEnabled")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Pause")) == 0)
    return true;
  if (str.CmpNoCase(wxT("InitSpatialMetaData")) == 0)
    return true;
  if (str.CmpNoCase(wxT("InitSpatialMetaDataFull")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AddGeometryColumn")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RecoverGeometryColumn")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DiscardGeometryColumn")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AddTemporaryGeometryColumn")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RegisterVirtualGeometry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DropVirtualGeometry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("InvalidateLayerStatistics")) == 0)
    return true;
  if (str.CmpNoCase(wxT("UpdateLayerStatistics")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetLayerExtent")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateSpatialIndex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateTemporarySpatialIndex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateMbrCache")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DisableSpatialIndex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RebuildGeometryTriggers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("UpgradeGeometryTriggers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CheckSpatialIndex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RecoverSpatialIndex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetSpatialIndexExtent")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CheckShadowedRowid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CheckWithoutRowid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateMetaCatalogTables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("UpdateMetaCatalogStatistics")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateStylingTables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ReCreateStylingTriggers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterVectorCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterSpatialViewCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterVirtualShapeCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterVirtualGeoJsonCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterVirtualTableCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterTopoGeoCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterTopoNetCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterVectorCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_SetVectorCoverageCopyright")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_SetVectorCoverageInfos")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterVectorCoverageSrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterVectorCoverageSrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterVectorCoverageKeyword")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterVectorCoverageKeyword")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UpdateVectorCoverageExtent")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_AutoRegisterStandardBrushes")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterExternalGraphic")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterExternalGraphic")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterVectorStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterVectorStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_ReloadVectorStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterVectorStyledLayer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterVectorStyledLayer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterRasterStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterRasterStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_ReloadRasterStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterRasterStyledLayer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterRasterStyledLayer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterRasterCoverageSrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterRasterCoverageSrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_RegisterRasterCoverageKeyword")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UnregisterRasterCoverageKeyword")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SE_UpdateRasterCoverageExtent")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_RegisterMapConfiguration")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_UnregisterMapConfiguration")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_ReloadMapConfiguration")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateIsoMetadataTables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetIsoMetadataId")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RegisterIsoMetadata")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_LoadXML")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_StoreXML")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CountUnsafeTriggers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsInteger")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsDecimalNumber")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsNumber")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToInteger")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToDouble")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ForceAsNull")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateUUID")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MD5Checksum")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MD5TotalChecksum")) == 0)
    return true;
  if (str.CmpNoCase(wxT("EncodeURL")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DecodeURL")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DirNameFromPath")) == 0)
    return true;
  if (str.CmpNoCase(wxT("FullFileNameFromPath")) == 0)
    return true;
  if (str.CmpNoCase(wxT("FileNameFromPath")) == 0)
    return true;
  if (str.CmpNoCase(wxT("FileExtFromPath")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RemoveExtraSpaces")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeStringList")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_Create")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_CreateTranslate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_CreateScale")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_CreateRotate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_CreateXRoll")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_CreateYRoll")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_CreateZRoll")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_Translate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_Scale")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_Rotate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_XRoll")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_YRoll")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_ZRoll")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_Multiply")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_Transform")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_IsValid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_AsText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_Determinant")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_IsInvertible")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ATM_Invert")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GCP_Compute")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GCP_Transform")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GCP_IsValid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GCP_AsText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GCP2ATM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Cutter")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetCutterMessage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateRasterCoveragesTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ReCreateRasterCoveragesTriggers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateVectorCoveragesTables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ReCreateVectorCoveragesTriggers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CloneTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateClonedTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CheckDuplicateRows")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RemoveDuplicateRows")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ElementaryGeometries")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DropGeoTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DropTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RenameTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RenameColumn")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportSHP")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportZipSHP")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Zipfile_NumSHP")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Zipfile_ShpN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExportSHP")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportDBF")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportZipDBF")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Zipfile_NumDBF")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Zipfile_DbfN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExportDBF")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExportKML")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExportGeoJSON")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExportGeoJSON2")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportGeoJSON")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportXLS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportWFS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportDXF")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportDXFfromDir")) == 0)
    return true;

  if (str.CmpNoCase(wxT("SqlProc_GetLastError")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_SetLogfile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_GetLogfile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_FromFile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_FromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_IsValid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_NumVariables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_VariableN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_AllVariables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_RawSQL")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_VarValue")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_IsValidVarValue")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_CookedSQL")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_Execute")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_ExecuteLoop")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SqlProc_Return")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredProc_CreateTables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredProc_Register")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredProc_Get")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredProc_Delete")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredProc_UpdateTitle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredProc_UpdateSqlBody")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredProc_Execute")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredProc_ExecuteLoop")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredProc_Return")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredVar_Register")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredVar_Get")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredVar_GetValue")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredVar_Delete")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredVar_UpdateTitle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StoredVar_UpdateValue")) == 0)
    return true;

  if (str.CmpNoCase(wxT("CreateMissingSystemTables")) == 0)
    return true;

  if (str.CmpNoCase(wxT("PostgreSql_ResetLastError")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PostgreSql_SetLastError")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PostgreSql_GetLastError")) == 0)
    return true;

  if (str.CmpNoCase(wxT("CreateRoutingNodes")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateRouting")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateRouting_GetLastError")) == 0)
    return true;

  if (str.CmpNoCase(wxT("sequence_nextval")) == 0)
    return true;
  if (str.CmpNoCase(wxT("sequence_currval")) == 0)
    return true;
  if (str.CmpNoCase(wxT("sequence_lastval")) == 0)
    return true;
  if (str.CmpNoCase(wxT("sequence_setval")) == 0)
    return true;

  if (str.CmpNoCase(wxT("InsertEpsgSrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Abs")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Acos")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Asin")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Atan")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Atan2")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Ceil")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Ceiling")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Cos")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Cot")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Degrees")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Exp")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Floor")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Ln")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Log")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Log2")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Log10")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PI")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Pow")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Power")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Radians")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Round")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Sign")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Sin")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Sqrt")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Stddev_pop")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Stddev_samp")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Var_pop")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Var_samp")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Tan")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsGeometryBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsCompressedGeometryBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsTinyPointBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsZipBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsPdfBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsGifBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsPngBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsTiffBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsWaveletBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsJpegBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsJp2Blob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsExifBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsExifGpsBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsWebpBlob")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetMimeType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BlobFromFile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BlobToFile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExportDXF")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TinyPointEncode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeometryPointEncode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakePoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Point")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakePointZ")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PointZ")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakePointM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PointM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakePointZM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PointZM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeLine")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeCircle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeEllipse")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeArc")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeEllipticArc")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeCircularSector")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeCircularStripe")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeEllipticSector")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BuildMbr")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BuildCircleMbr")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Extent")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrMinX")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrMinY")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrMaxX")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrMaxY")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MbrMinX")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MbrMinY")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MbrMaxX")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MbrMaxY")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MinX")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MinY")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MaxX")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MaxY")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MinZ")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MinM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MaxZ")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MaxM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GeomFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_WKTToSQL")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeometryFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GeometryFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PointFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PointFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LineFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LineFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LineStringFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LineStringFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PolyFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PolyFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PolygonFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PolygonFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MPointFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MPointFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MultiPointFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MultiPointFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MLineFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MLineFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MultiLineStringFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MultiLineStringFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MPolyFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MPolyFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MultiPolygonFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MultiPolygonFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomCollFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GeomCollFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeometryCollectionFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BdPolyFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_BdPolyFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BdMPolyFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_BdMPolyFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeometryCollectionFromText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GeomFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_WKBToSQL")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PointFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PointFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LineFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LineFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LineStringFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LineStringFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PolyFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PolyFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PolygonFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PolygonFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MPointFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MPointFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MultiPointFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MultiPointFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MLineFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MLineFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MultiLineStringFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MultiLineStringFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MPolyFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MPolyFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MultiPolygonFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MultiPolygonFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomCollFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GeomCollFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeometryCollectionFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GeometryCollectionFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BdPolyFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_BdPolyFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BdMPolyFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_BdMPolyFromWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AsText")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsWKT")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsSVG")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsKML")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromKML")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsGML")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromGML")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsGeoJSON")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromGeoJSON")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsFGF")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsBinary")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AsBinary")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromFGF")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsEWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromEWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsTWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromTWKB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AsEncodedPolyline")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LineFromEncodedPolyline")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsEWKT")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromEWKT")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CompressGeometry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("UncompressGeometry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SanitizeGeometry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("EnsureClosedRings")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RemoveRepeatedPoints")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToLinestring")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToPolygon")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToMultiPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToMultiLinestring")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToMultiPolygon")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToGeometryCollection")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToMulti")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Multi")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToSingle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToXY")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToXYZ")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToXYM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastToXYZM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Reverse")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ForceLHR")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ForcePolygonCW")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ForcePolygonCCW")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsPolygonCW")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsPolygonCCW")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Dimension")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Dimension")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CoordDimension")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NDims")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Is3D")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsMeasured")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeometryType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeometryAliasType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GeometryType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SRID")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SRID")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetSRID")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ToGARS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GARSMbr")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsEmpty")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsEmpty")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsSimple")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsSimple")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsValid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsValid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsValidReason")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsValidReason")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsValidDetail")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsValidDetail")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Boundary")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Boundary")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Envelope")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Envelope")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Expand")) == 0)
    return true;
  if (str.CmpNoCase(wxT("X")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_X")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Y")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Y")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Z")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Z")) == 0)
    return true;
  if (str.CmpNoCase(wxT("M")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_M")) == 0)
    return true;
  if (str.CmpNoCase(wxT("StartPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_StartPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("EndPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_EndPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Length")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_3dLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Perimeter")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Perimeter")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LinestringMinSegmentLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LinestringMinSegmentLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LinestringMaxSegmentLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LinestringMaxSegmentLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LinestringAvgSegmentLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LinestringAvgSegmentLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsClosed")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsClosed")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsRing")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsRing")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Simplify")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Simplify")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Generalize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SimplifyPreserveTopology")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SimplifyPreserveTopology")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeodesicLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GreatCircleLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeodesicArcLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeodesicChordLength")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeodesicCentralAngle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeodesicArcArea")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeodesicArcHeigth")) == 0)
    return true;
  if (str.CmpNoCase(wxT("NumPoints")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NumPoints")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PointN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PointN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Centroid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Centroid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PointOnSurface")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_PointOnSurface")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Area")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Area")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Circularity")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExteriorRing")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ExteriorRing")) == 0)
    return true;
  if (str.CmpNoCase(wxT("NumInteriorRing")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NumInteriorRing")) == 0)
    return true;
  if (str.CmpNoCase(wxT("NumInteriorRings")) == 0)
    return true;
  if (str.CmpNoCase(wxT("InteriorRingN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_InteriorRingN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("NumGeometries")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NumGeometries")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NPoints")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NRings")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeometryN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GeometryN")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AddPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AddPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RemovePoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_RemovePoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SetPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetStartPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SetStartPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetEndPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SetEndPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrEqual")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrDisjoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrTouches")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrWithin")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrOverlaps")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrIntersects")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_EnvIntersects")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_EnvelopesIntersects")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MbrContains")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Equals")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Equals")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Disjoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Disjoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Touches")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Touches")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Within")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Within")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Overlaps")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Overlaps")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Crosses")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Crosses")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Intersects")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Intersects")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Contains")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Contains")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Covers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Covers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CoveredBy")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_CoveredBy")) == 0)
    return true;
  if (str.CmpNoCase(wxT("OffsetCurve")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_OffsetCurve")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SingleSidedBuffer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SingleSidedBuffer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SharedPaths")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SharedPaths")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Relate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Relate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RelateMatch")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_RelateMatch")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Distance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Distance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("HausdorffDistance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_HausdorffDistance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("FrechetDistance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_FrechetDistance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PtDistWithin")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Intersection")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Intersection")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Difference")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Difference")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GUnion")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Union")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SymDifference")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SymDifference")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Buffer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Buffer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ConvexHull")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ConvexHull")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Transform")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Transform")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TransformXY")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_TransformXY")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TransformXYZ")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_TransformXYZ")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Shift_Longitude")) == 0)
    return true;
  if (str.CmpNoCase(wxT("NormalizeLonLat")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Line_Interpolate_Point")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Line_Interpolate_Point")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Line_Interpolate_Equidistant_Points")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Line_Interpolate_Equidistant_Points")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Line_Locate_Point")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Line_Locate_Point")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Line_Substring")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Line_Substring")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ClosestPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ClosestPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ShortestLine")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ShortestLine")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Snap")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Snap")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Collect")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Collect")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LineMerge")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LineMerge")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BuildArea")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_BuildArea")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Polygonize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Polygonize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("UnaryUnion")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_UnaryUnion")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_DrapeLine")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_DrapeLineExceptions")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DissolveSegments")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_DissolveSegments")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DissolvePoints")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_DissolvePoints")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LinesFromRings")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LinesFromRings")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RingsCutAtNodes")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_RingsCutAtNodes")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LinesCutAtNodes")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LinesCutAtNodes")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CollectionExtract")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_CollectionExtract")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExtractMultiPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExtractMultiLinestring")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExtractMultiPolygon")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AddMeasure")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_InterpolatePoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Locate_Along_Measure")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LocateAlong")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Locate_Between_Measures")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LocateBetween")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CurvosityIndex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_CurvosityIndex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("UpHillHeight")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_UpHillHeight")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DownHillHeight")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_DownHillHeight")) == 0)
    return true;
  if (str.CmpNoCase(wxT("UpDownHeight")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_UpDownHeight")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_IsValidTrajectory")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_TrajectoryInterpolatePoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SquareGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SquareGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TriangularGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_TriangularGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("HexagonalGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_HexagonalGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DelaunayTriangulation")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_DelaunayTriangulation")) == 0)
    return true;
  if (str.CmpNoCase(wxT("VoronojDiagram")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_VoronojDiagram")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ConcaveHull")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ConcaveHull")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeValid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MakeValid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakeValidDiscarded")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MakeValidDiscarded")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Segmentize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Segmentize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SnapAndSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SnapAndSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Split")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Split")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SplitLeft")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SplitLeft")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SplitRight")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SplitRight")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Azimuth")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Azimuth")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Project")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Project")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeoHash")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GeoHash")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsX3D")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AsX3D")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_3DDistance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_3DMaxDistance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MaxDistance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MaxDistance")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SnapToGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SnapToGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Node")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Subdivide")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SelfIntersections")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SelfIntersections")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridFromAuthCRS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridIsGeographic")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridIsProjected")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridHasFlippedAxes")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetSpheroid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetEllipsoid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetPrimeMeridian")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetDatum")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetUnit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetProjection")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetAxis_1_Name")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetAxis_1_Orientation")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetAxis_2_Name")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SridGetAxis_2_Orientation")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ShiftCoords")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ShiftCoordinates")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_Translate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ScaleCoords")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ScaleCoordinates")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RotateCoords")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RotateCoordinates")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ReflectCoords")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ReflectCoordinates")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SwapCoords")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SwapCoordinates")) == 0)
    return true;
  if (str.CmpNoCase(wxT("FilterMbrWithin")) == 0)
    return true;
  if (str.CmpNoCase(wxT("FilterMbrContains")) == 0)
    return true;
  if (str.CmpNoCase(wxT("FilterMbrIntersects")) == 0)
    return true;
  if (str.CmpNoCase(wxT("BuildMbrFilter")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RTreeWithin")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RTreeContains")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RTreeIntersects")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RTreeDistWithin")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_Create")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetPayload")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetDocument")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsValid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_SchemaValidate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsCompressed")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsIsoMetadata")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsSldSeVectorStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsSldSeRasterStyle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsSvg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsGpx")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsMapConfig")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_Compress")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_Uncompress")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsSchemaValidated")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetSchemaURI")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetInternalSchemaURI")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetFileId")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_SetFileId")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_AddFileId")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetParentId")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_SetParentId")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_AddParentId")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetName")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetTitle")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetAbstract")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetGeometry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetDocumentSize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetEncoding")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_MLineFromGPX")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetLastParseError")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_IsValidXPathExpression")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetLastValidateError")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_GetLastXPathError")) == 0)
    return true;
  if (str.CmpNoCase(wxT("XB_CacheFlush")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PROJ_GetDatabasePath")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PROJ_SetDatabasePath")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PROJ_AsProjString")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PROJ_AsWKT")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PROJ_GuessSridFromWKT")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PROJ_GuessSridFromSHP")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PROJ_GuessSridFromZipSHP")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PROJ_GetLastErrorMsg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GEOS_GetLastWarningMsg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GEOS_GetLastErrorMsg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GEOS_GetLastAuxErrorMsg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GEOS_GetCriticalPointFromMsg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RTTOPO_GetLastWarningMsg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RTTOPO_GetLastErrorMsg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("MakePolygon")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MakePolygon")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LongLatToDMS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LongitudeFromDMS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LatitudeFromDMS")) == 0)
    return true;

  if (str.CmpNoCase(wxT("CreateTopoTables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ReCreateTopoTriggers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateTopology")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_InitTopoGeo")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DropTopology")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetLastTopologyException")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AddIsoNode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MoveIsoNode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_RemIsoNode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AddIsoEdge")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_RemIsoEdge")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ModEdgeSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NewEdgesSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ModEdgeHeal")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NewEdgeHeal")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AddEdgeModFace")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AddEdgeNewFaces")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_RemEdgeModFace")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_RemEdgeNewFace")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ChangeEdgeGeom")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GetFaceGeometry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_GetFaceEdges")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ValidateTopoGeo")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_CreateTopoGeo")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetNodeByPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetEdgeByPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetFaceByPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_AddPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_AddLineString")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_AddLineStringNoFace")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_AddPolygon")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_TopoSnap")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_SnappedGeoTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_SubdivideLines")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_FromGeoTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_FromGeoTableNoFace")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_FromGeoTableExt")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_FromGeoTableNoFaceExt")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_Polygonize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_RemoveSmallFaces")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_RemoveDanglingEdges")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_RemoveDanglingNodes")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_NewEdgeHeal")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_ModEdgeHeal")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_NewEdgesSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_ModEdgesSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_Clone")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_GetEdgeSeed")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_GetFaceSeed")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_DisambiguateSegmentEdges")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_UpdateSeeds")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_SnapPointToSeed")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_SnapLineToSeed")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_ToGeoTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_ToGeoTableGeneralize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_CreateTopoLayer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_InitTopoLayer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_RemoveTopoLayer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_ExportTopoLayer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_InsertFeatureFromTopoLayer")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_PolyFacesList")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoGeo_LineEdgesList")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateNetwork")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_InitTopoNet")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DropNetwork")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetLastNetworkException")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AddIsoNetNode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_MoveIsoNetNode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_RemIsoNetNode")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_AddLink")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ChangeLinkGeom")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_RemoveLink")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NewLogLinkSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ModLogLinkSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NewGeoLinkSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ModGeoLinkSplit")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_NewLinkHeal")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ModLinkHeal")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_LogiNetFromTGeo")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SpatNetFromTGeo")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_SpatNetFromGeom")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ValidLogicalNet")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ST_ValidSpatialNet")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetNetNodeByPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetLinkByPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoNet_FromGeoTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoNet_Clone")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoNet_GetLinkSeed")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoNet_DisambiguateSegmentLinks")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoNet_UpdateSeeds")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoNet_ToGeoTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoNet_ToGeoTableGeneralize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("TopoNet_LineLinksList")) == 0)
    return true;
  return false;

}

bool MyQueryView::IsSqlRasterFunction(wxString & str, char next_c)
{
// checks if this one is an SQL raster-function
  if (next_c != '(')
    return false;
  if (str.CmpNoCase(wxT("rl2_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_target_cpu")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_none")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_deflate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_deflate_no")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_jpeg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_png")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_fax4")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_lzma")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_lzma_no")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_lz4")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_lz4_no")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_zstd")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_zstd_no")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_webp")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_ll_webp")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_jp2")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_has_codec_ll_jp2")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_cairo_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_curl_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_zlib_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_lzma_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_lz4_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_zstd_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_png_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_jpeg_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_tiff_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_geotiff_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_webp_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("rl2_openJpeg_version")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetMaxThreads")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_SetMaxThreads")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsAntiLabelCollisionEnabled")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_EnableAntiLabelCollision")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_DisableAntiLabelCollision")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsPolygonLabelsMultilineEnabled")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_EnablePolygonLabelsMultiline")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_DisablePolygonLabelsMultiline")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsPolygonLabelsAutorotateEnabled")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_EnablePolygonLabelsAutorotate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_DisablePolygonLabelsAutorotate")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsValidPixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsValidPixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsPixelNone")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsPixelNone")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsValidRasterPalette")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsValidRasterPalette")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsValidRasterStatistics")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsValidRasterStatistics")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsValidRasterTile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsValidRasterTile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreateCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_CreateRasterCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CopyRasterCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_CopyRasterCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DeleteSection")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_DeleteSection")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DropRasterCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_DropRasterCoverage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetRasterCoverageInfos")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_SetRasterCoverageInfos")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetRasterCoverageCopyright")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_SetRasterCoverageCopyright")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetRasterCoverageDefaultBands")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_SetRasterCoverageDefaultBands")) == 0)
    return true;
  if (str.CmpNoCase(wxT("EnableRasterCoverageAutoNDVI")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_EnableRasterCoverageAutoNDVI")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsRasterCoverageAutoNdviEnabled")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsRasterCoverageAutoNdviEnabled")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetPaletteNumEntries")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetPaletteNumEntries")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetPaletteColorEntry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetPaletteColorEntry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetPaletteColorEntry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_SetPaletteColorEntry")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PaletteEquals")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_PaletteEquals")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreatePixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_CreatePixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CreatePixelNone")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_CreatePixelNone")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetPixelType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetPixelType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetPixelSampleType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetPixelSampleType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetPixelNumBands")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetPixelNumBands")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetPixelValue")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetPixelValue")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetPixelValue")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_SetPixelValue")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsTransparentPixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsTransparentPixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsOpaquePixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsOpaquePixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetTransparentPixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_SetTransparentPixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("SetOpaquePixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_SetOpaquePixel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PixelEquals")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_PixelEquals")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetRasterStatistics_NoDataPixelsCount")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetRasterStatistics_NoDataPixelsCount")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetRasterStatistics_ValidPixelsCount")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetRasterStatistics_ValidPixelsCount")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetRasterStatistics_SampleType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetRasterStatistics_SampleType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetRasterStatistics_BandsCount")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetRasterStatistics_BandsCount")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetBandStatistics_Min")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetBandStatistics_Min")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetBandStatistics_Max")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetBandStatistics_Max")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetBandStatistics_Avg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetBandStatistics_Avg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetBandStatistics_Var")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetBandStatistics_Var")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetBandStatistics_StdDev")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetBandStatistics_StdDev")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetBandStatistics_Histogram")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetBandStatistics_Histogram")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetBandHistogramFromImage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetBandHistogramFromImage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("Pyramidize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_Pyramidize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DePyramidize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_DePyramidize")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetPixelFromRasterByPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetPixelFromRasterByPoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetMapImageFromRaster")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetMapImageFromRaster")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetStyledMapImageFromRaster")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetStyledMapImageFromRaster")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetMapImageFromVector")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetMapImageFromVector")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetStyledMapImageFromVector")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetStyledMapImageFromVector")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetMapImageFromWMS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetMapImageFromWMS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("InitializeMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_InitializeMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("FinalizeMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_FinalizeMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PaintRasterOnMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_PaintRasterOnMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PaintStyledRasterOnMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_PaintStyledRasterOnMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PaintVectorOnMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_PaintVectorOnMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("PaintStyledVectorOnMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_PaintStyledVectorOnMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetImageFromMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetImageFromMapCanvas")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetTileImage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetTileImage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetTripleBandTileImage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetTripleBandTileImage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetMonoBandTileImage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetMonoBandTileImage")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExportRawPixels")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_ExportRawPixels")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExportSectionRawPixels")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_ExportSectionRawPixels")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ImportSectionRawPixels")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_ImportSectionRawPixels")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetDrapingLastError")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetDrapingLastError")) == 0)
    return true;
  if (str.CmpNoCase(wxT("DrapeGeometries")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_DrapeGeometries")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LoadFontFromFile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_LoadFontFromFile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("ExportFontToFile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_ExportFontToFile")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsValidFont")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsValidFont")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetFontFamily")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetFontFamily")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GetFontFacename")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_GetFontFacename")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsFontBold")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsFontBold")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsFontItalic")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_IsFontItalic")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LoadRaster")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_LoadRaster")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LoadRastersFromDir")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_LoadRastersFromDir")) == 0)
    return true;
  if (str.CmpNoCase(wxT("LoadRasterFromWMS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_LoadRasterFromWMS")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteJpegJgw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteJpegJgw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteJpeg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteJpeg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionJpegJgw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionJpegJgw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionJpeg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionJpeg")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteTripleBandGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteTripleBandGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteMonoBandGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteMonoBandGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteTripleBandTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteTripleBandTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteMonoBandTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteMonoBandTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteTripleBandTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteTripleBandTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteMonoBandTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteMonoBandTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionTripleBandGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionTripleBandGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionMonoBandGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionMonoBandGeoTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionTripleBandTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionTripleBandTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionMonoBandTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionMonoBandTiffTfw")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionTripleBandTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionTripleBandTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionMonoBandTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionMonoBandTiff")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteAsciiGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteAsciiGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionAsciiGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionAsciiGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteNdviAsciiGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteNdviAsciiGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("WriteSectionNdviAsciiGrid")) == 0)
    return true;
  if (str.CmpNoCase(wxT("RL2_WriteSectionNdviAsciiGrid")) == 0)
    return true;

  if (str.CmpNoCase(wxT("AutoGPKGStart")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AutoGPKGStop")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CheckGeoPackageMetaData")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgCreateBaseTables")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgInsertEpsgSRID")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgCreateTilesTable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgCreateTilesZoomLevel")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgAddTileTriggers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgGetNormalZoom")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgGetNormalRow")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgGetImageType")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgAddGeometryColumn")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgAddGeometryTriggers")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgAddSpatialIndex")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgMakePoint")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgMakePointZ")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgMakePointM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("gpkgMakePointZM")) == 0)
    return true;
  if (str.CmpNoCase(wxT("AsGPB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GeomFromGPB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("IsValidGPB")) == 0)
    return true;
  if (str.CmpNoCase(wxT("GPKG_IsAssignable")) == 0)
    return true;
  if (str.CmpNoCase(wxT("CastAutomagic")) == 0)
    return true;
  return false;
}

void MyQueryView::DoSqlSyntaxColor()
{
//
// evidencing a nice colored SQL syntax 
//
  IgnoreEvent = true;
  SqlCtrl->Hide();
  wxTextAttr normal_style(wxColour(128, 128, 128), wxColour(255, 255, 255),
                          wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                                 wxFONTWEIGHT_NORMAL));
  wxTextAttr sql_style(wxColour(0, 0, 255), wxColour(255, 255, 255),
                       wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                              wxFONTWEIGHT_BOLD));
  wxTextAttr const_style(wxColour(255, 0, 255), wxColour(255, 255, 255),
                         wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_NORMAL));
  wxTextAttr fnct_style(wxColour(192, 128, 0), wxColour(255, 255, 255),
                        wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                               wxFONTWEIGHT_BOLD));
  wxTextAttr bracket_style(wxColour(255, 0, 0), wxColour(192, 192, 192),
                           wxFont(12, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL,
                                  wxFONTWEIGHT_BOLD));
  wxString sql = SqlCtrl->GetValue();
// setting the base style
  SqlCtrl->SetStyle(0, sql.Len(), normal_style);
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
      char *word = new char[token.Len() * 4];
      strcpy(word, token.ToUTF8());
      if (gaiaIsReservedSqliteName(word))
        {
          // setting the SQL keyword style
          SqlCtrl->SetStyle(from, to, sql_style);
      } else if (IsSqliteExtra(token))
        {
          // setting the SQL keyword style
          SqlCtrl->SetStyle(from, to, sql_style);
      } else if (IsSqlString(token) == true)
        {
          // setting the SQL string constant style
          SqlCtrl->SetStyle(from, to, const_style);
      } else if (IsSqlNumber(token) == true)
        {
          // setting the SQL numeric constant style
          SqlCtrl->SetStyle(from, to, const_style);
      } else if (IsSqlFunction(token, next_c) == true)
        {
          // setting the SQL function style
          SqlCtrl->SetStyle(from, to, fnct_style);
      } else if (IsSqlGeoFunction(token, next_c) == true)
        {
          // setting the SQL geo-function style
          SqlCtrl->SetStyle(from, to, fnct_style);
      } else if (IsSqlRasterFunction(token, next_c) == true)
        {
          // setting the SQL raster-function style
          SqlCtrl->SetStyle(from, to, fnct_style);
        }
      delete[]word;
    }
  if (BracketStart >= 0)
    {
      // evidencing an opening bracket
      SqlCtrl->SetStyle(BracketStart, BracketStart + 1, bracket_style);
    }
  if (BracketEnd >= 0)
    {
      // evidencing a closing bracket
      SqlCtrl->SetStyle(BracketEnd, BracketEnd + 1, bracket_style);
    }
  SqlCtrl->Show();
  SqlCtrl->SetFocus();
  IgnoreEvent = false;
}

void MyQueryView::OnSqlSyntaxColor(wxCommandEvent & event)
{
//
// EVENT: updating the SQL syntax 
//
  if (IgnoreEvent == true)
    {
      // processing is still in progress; ignoring any internally generated call
      return;
    }
  event.Skip();
  EventBrackets();
}

void MyQueryView::EvidBrackets(int on, int off)
{
// evidencing corresponding brackets [open/close]
  BracketStart = -1;
  BracketEnd = -1;
  if (on >= 0)
    BracketStart = on;
  if (off >= 0)
    BracketEnd = off;
  DoSqlSyntaxColor();
}

void MyQueryView::EventBrackets()
{
//
// evidencing brackets [balancing open-close pairs] 
//
  if (IgnoreEvent == true)
    {
      // processing is still in progress; ignoring any internally generated call
      return;
    }
  int pos = SqlCtrl->GetInsertionPoint();
  int on;
  int off;
  wxString sql = SqlCtrl->GetValue();
  char pre = '\0';
  char post = '\0';
  if (pos > 0)
    pre = sql.GetChar(pos - 1);
  if (pos < (int) sql.Len())
    post = sql.GetChar(pos);
  if (post == '(')
    {
      // positioned before an opening bracket
      if (CheckBrackets(pos, false, &on, &off) == true)
        EvidBrackets(on, off);
      else
        EvidBrackets(pos, -1);
      return;
    }
  if (pre == ')')
    {
      // positioned after a closing bracket
      if (CheckBrackets(pos - 1, true, &on, &off) == true)
        EvidBrackets(on, off);
      else
        EvidBrackets(-1, pos - 1);
      return;
    }
  EvidBrackets(-1, -1);
}

bool MyQueryView::CheckBrackets(int pos, bool reverse_direction, int *on,
                                int *off)
{
// trying to balance a brackets pair [opening/closing]
  int i;
  int len;
  int level = 0;
  char c;
  int single_quoted = 0;
  int double_quoted = 0;
  wxString sql = SqlCtrl->GetValue();
  if (reverse_direction == true)
    {
      // going backward from CLOSE to OPEN
      for (i = pos - 1; i >= 0; i--)
        {
          c = sql.GetChar(i);
          if (c == '\'' && !double_quoted)
            {
              // single quoting start-stop
              if (single_quoted)
                single_quoted = 0;
              else
                single_quoted = 1;
            }
          if (c == '"' && !single_quoted)
            {
              // double quoting start-stop
              if (double_quoted)
                double_quoted = 0;
              else
                double_quoted = 1;
            }
          if (single_quoted || double_quoted)
            continue;
          if (c == ')')
            level++;
          if (c == '(')
            {
              if (level == 0)
                {
                  *on = i;
                  *off = pos;
                  return true;
                }
              level--;
            }
        }
  } else
    {
      // going forward from OPEN to CLOSE
      len = sql.Len();
      for (i = pos + 1; i < len; i++)
        {
          c = sql.GetChar(i);
          if (c == '\'' && !double_quoted)
            {
              // single quoting start-stop
              if (single_quoted)
                single_quoted = 0;
              else
                single_quoted = 1;
            }
          if (c == '"' && !single_quoted)
            {
              // double quoting start-stop
              if (double_quoted)
                double_quoted = 0;
              else
                double_quoted = 1;
            }
          if (single_quoted || double_quoted)
            continue;
          if (c == '(')
            level++;
          if (c == ')')
            {
              if (level == 0)
                {
                  *on = pos;
                  *off = i;
                  return true;
                }
              level--;
            }
        }
    }
  return false;
}

MySqlControl::MySqlControl(MyQueryView * parent, wxWindowID id,
                           const wxString & value, const wxPoint & pos,
                           const wxSize & size, long style):wxTextCtrl(parent,
                                                                       id,
                                                                       value,
                                                                       pos,
                                                                       size,
                                                                       style)
{
//
// constructor: SQL text control
//
  Parent = parent;
  Connect(wxID_ANY, wxEVT_LEFT_DOWN,
          (wxObjectEventFunction) & MySqlControl::OnSqlMousePosition);
  Connect(wxID_ANY, wxEVT_KEY_UP,
          (wxObjectEventFunction) & MySqlControl::OnSqlArrowPosition);
}

void MySqlControl::OnSqlMousePosition(wxMouseEvent & event)
{
//
// intercepting mouse clicks
//
  if (Parent->IsIgnoreEvent() == true)
    return;
  event.Skip();
  Parent->EventBrackets();
}

void MySqlControl::OnSqlArrowPosition(wxKeyEvent & event)
{
//
// intercepting arrow keys
//
  if (Parent->IsIgnoreEvent() == true)
    return;
  event.Skip();
  int key_code = event.GetKeyCode();
  switch (key_code)
    {
      case WXK_DELETE:
      case WXK_HOME:
      case WXK_LEFT:
      case WXK_UP:
      case WXK_RIGHT:
      case WXK_DOWN:
      case WXK_PAGEUP:
      case WXK_PAGEDOWN:
      case WXK_NUMPAD_DELETE:
      case WXK_NUMPAD_HOME:
      case WXK_NUMPAD_LEFT:
      case WXK_NUMPAD_UP:
      case WXK_NUMPAD_RIGHT:
      case WXK_NUMPAD_DOWN:
      case WXK_NUMPAD_PAGEUP:
      case WXK_NUMPAD_PAGEDOWN:
        Parent->EventBrackets();
        break;
      default:
        break;
    };
}

SqlTokenizer::SqlTokenizer(wxString & sql)
{
// breaking tokens from an SQL expression
  Block = 1024;
  Max = Block;
  int i;
  char c;
  int single_quoted = 0;
  int double_quoted = 0;
  int white_space = 0;
  int start = -1;
  int len;
// initial allocation for the token list
  TokenList = new wxString *[Max];
  for (i = 0; i < Max; i++)
    TokenList[i] = NULL;
  Index = 0;
  for (i = 0; i < (int) sql.Len(); i++)
    {
      // scanning the SQL statement
      c = sql.GetChar(i);
      if (c == '\'' && !double_quoted)
        {
          if (single_quoted)
            {
              single_quoted = 0;
              len = i - start;
              len++;
              wxString *token = new wxString(sql.Mid(start, len));
              Insert(token);
              start = -1;
          } else
            {
              single_quoted = 1;
              start = i;
            }
          continue;
        }
      if (c == '"' && !single_quoted)
        {
          if (double_quoted)
            {
              double_quoted = 0;
              len = i - start;
              len++;
              wxString *token = new wxString(sql.Mid(start, len));
              Insert(token);
              start = -1;
          } else
            {
              double_quoted = 1;
              start = i;
            }
          continue;
        }
      if (single_quoted || double_quoted)
        continue;
      if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '('
          || c == ')' || c == ';' || c == ',')
        {
          if (white_space)
            continue;
          if (start >= 0)
            {
              // ok, we have a valid SQL token
              len = i - start;
              wxString *token = new wxString(sql.Mid(start, len));
              Insert(token);
            }
          start = -1;
          white_space = 1;
          continue;
        }
      white_space = 0;
      if (start < 0)
        start = i;
    }
  if (start >= 0)
    {
      // fetching the last token
      i = sql.Len();
      len = i - start;
      wxString *token = new wxString(sql.Mid(start, len));
      Insert(token);
    }
  Index = 0;
}

SqlTokenizer::~SqlTokenizer()
{
// destructor
  wxString *token;
  Index = 0;
  while (1)
    {
      token = TokenList[Index];
      if (token == NULL)
        break;
      delete token;
      Index++;
    }
  delete[]TokenList;
}

void SqlTokenizer::Expand()
{
// expanding the token list
  int newSize = Max + Block;
  int i;
  wxString **newList = new wxString *[newSize];
  for (i = 0; i < newSize; i++)
    newList[i] = NULL;
  for (i = 0; i < Max; i++)
    newList[i] = TokenList[i];
  delete[]TokenList;
  TokenList = newList;
  Max = newSize;
}

void SqlTokenizer::Insert(wxString * token)
{
// inserting a new token
  if (Index == (Max - 1))
    Expand();
  TokenList[Index++] = token;
}

bool SqlTokenizer::HasMoreTokens()
{
  wxString *token = TokenList[Index];
  if (token == NULL)
    return false;
  return true;
}

wxString & SqlTokenizer::GetNextToken()
{
// return the next token
  wxString *token = TokenList[Index];
  Index++;
  CurrentToken = *token;
  return CurrentToken;
}
