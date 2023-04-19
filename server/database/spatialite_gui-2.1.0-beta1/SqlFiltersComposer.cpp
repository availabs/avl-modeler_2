/*
/ SqlFiltersComposer.cpp
/ Sql Filters Composer
/
/ version 2.0, 2016 August 9
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2016  Alessandro Furieri
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

void MyFrame::SqlFiltersComposer()
{
//
// query/view composer invoked
//
  SqlFiltersDialog dlg;

  if (SqlFilters.IsValid() == false)
    return;

  wxString db_prefix = SqlFilters.GetDbPrefix();
  wxString table = SqlFilters.GetTableName();

  dlg.Create(this, SqlFilters);
  if (dlg.ShowModal() == wxID_OK)
    {
      DoSaveSqlFilters(&dlg);
      int primaryKeys[1024];
      int blobCols[1024];
      int pk = 0;
      int pb = 0;
      char **results;
      int rows;
      int columns;
      char *xsql;
      int i;
      char *errMsg = NULL;
      wxString sql;
      char *type;
      char *xname;
      char *xname2;

      for (i = 0; i < 1024; i++)
        {
          primaryKeys[i] = -1;
          blobCols[i] = -1;
        }
      primaryKeys[pk++] = 0;
      sql = wxT("PRAGMA \"");
      xname = (char *) malloc((db_prefix.Len() * 4) + 1);
      strcpy(xname, db_prefix.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\".table_info(\"");
      xname = (char *) malloc((table.Len() * 4) + 1);
      strcpy(xname, table.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\")");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(GetSqlite(), xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              type = results[(i * columns) + 2];
              if (strcasecmp(type, "BLOB") == 0)
                blobCols[pb++] = i;
              if (atoi(results[(i * columns) + 5]) == 0)
                ;
              else
                primaryKeys[pk++] = i;
            }
        }
      sqlite3_free_table(results);

      wxString dummy;
      if (SqlFilters.IsReadOnly() == true)
        SetSql(dlg.GetSqlSample(), true, false, dummy, dummy, true);
      else
        EditTable(dlg.GetSqlSample(), primaryKeys, blobCols, table);
    }
}

bool SqlFiltersDialog::Create(MyFrame * parent, CurrentSqlFilters & filters)
{
//
// creating the dialog
//
  MainFrame = parent;
  if (filters.IsValid() == false)
    return false;
  DbPrefix = filters.GetDbPrefix();
  TableName = filters.GetTableName();
  ReadOnly = filters.IsReadOnly();
  IgnoreEvent = false;
  Where1Enabled = filters.IsWhere1Enabled();
  Where2Enabled = filters.IsWhere2Enabled();
  Where3Enabled = filters.IsWhere3Enabled();
  Where1Column = filters.GetWhere1Column();
  Where2Column = filters.GetWhere2Column();
  Where3Column = filters.GetWhere3Column();
  Where1Operator = filters.GetWhere1Operator();
  Where2Operator = filters.GetWhere2Operator();
  Where3Operator = filters.GetWhere3Operator();
  Where1Value = filters.GetWhere1Value();
  Where2Value = filters.GetWhere2Value();
  Where3Value = filters.GetWhere3Value();
  AndOr12 = filters.IsAndOr12();
  AndOr23 = filters.IsAndOr23();
  Order1Enabled = filters.IsOrder1Enabled();
  Order2Enabled = filters.IsOrder2Enabled();
  Order3Enabled = filters.IsOrder3Enabled();
  Order4Enabled = filters.IsOrder4Enabled();
  Order1Column = filters.GetOrder1Column();
  Order2Column = filters.GetOrder2Column();
  Order3Column = filters.GetOrder3Column();
  Order4Column = filters.GetOrder4Column();
  Order1Desc = filters.IsOrder1Desc();
  Order2Desc = filters.IsOrder2Desc();
  Order3Desc = filters.IsOrder3Desc();
  Order4Desc = filters.IsOrder4Desc();
  FreeHand = filters.GetFreeHand();
  PopulateColumnList();
  if (wxDialog::Create(parent, wxID_ANY, wxT("apply SQL Filters")) == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  UpdateSqlSample();
  return true;
}

void SqlFiltersDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
// the SQL sample row
  wxBoxSizer *sqlBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sqlBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxStaticBox *sqlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("SQL statement"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *sqlSizer = new wxStaticBoxSizer(sqlBox, wxVERTICAL);
  sqlBoxSizer->Add(sqlSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  SqlCtrl =
    new wxTextCtrl(this, ID_SQL_FC_SQL, wxT(""), wxDefaultPosition,
                   wxSize(750, 150),
                   wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL | wxTE_RICH);
  sqlSizer->Add(SqlCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);

// the tab control
  TabCtrl =
    new wxNotebook(this, ID_SQL_FC_TAB, wxDefaultPosition, wxDefaultSize,
                   wxNB_TOP);
  boxSizer->Add(TabCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// tab - Page #1
  Page1 = new SqlFiltersMainPage();
  Page1->Create(this);
  TabCtrl->AddPage(Page1, wxT("Where Clauses"), true);
// tab - Page #2
  Page2 = new SqlFiltersOrderPage();
  Page2->Create(this);
  TabCtrl->AddPage(Page2, wxT("Order By Clauses"), true);
// tab - Page #3
  Page3 = new SqlFiltersFreeHandPage();
  Page3->Create(this);
  TabCtrl->AddPage(Page3, wxT("Freehand Clauses"), true);

  TabCtrl->ChangeSelection(0);

// appending the OK / CANCEL buttons
  wxBoxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *okBtn = new wxButton(this, wxID_OK, wxT("&Ok"));
  btnSizer->Add(okBtn, 0, wxALIGN_LEFT | wxALL, 5);
  wxButton *cancelBtn = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnSizer->Add(cancelBtn, 0, wxALIGN_LEFT | wxALL, 5);
// appending the event handlers
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SqlFiltersDialog::OnOk);
  Connect(wxID_ANY, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) & SqlFiltersDialog::OnSqlSyntaxColor);
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
          (wxObjectEventFunction) & SqlFiltersDialog::OnPageChanged);
}

void SqlFiltersDialog::ResetWizardClauses()
{
//
// resetting Wizard SQL Filters
//
  Page1->ResetWizard();
  Page2->ResetWizard();
}

void SqlFiltersDialog::ResetFreeHand()
{
//
// resetting FreeHand SQL
//
  FreeHand = wxEmptyString;
}

void SqlFiltersDialog::SqlCleanList(wxString & dirty, wxString & clean)
{
// well-formatting a string to be used as an SQL values list [IN]
  wxString token[256];
  int nextToken = 0;
  int i;
  bool quotedList;
  char org[8192];
  char dst[8192];
  char *in = org;
  char *out = dst;
  strcpy(org, dirty.ToUTF8());
  while (1)
    {
      if (*in == '\0')
        {
          *out = '\0';
          token[nextToken++] = wxString::FromUTF8(dst);
          break;
        }
      if (*in == ',')
        {
          *out = '\0';
          token[nextToken++] = wxString::FromUTF8(dst);
          in++;
          out = dst;
          continue;
        }
      *out++ = *in++;
    }
  for (i = 0; i < nextToken; i++)
    {
      wxString str = token[i].Trim(false);
      token[i] = str.Trim(true);
    }
  quotedList = false;
  for (i = 0; i < nextToken; i++)
    {
      bool isNumber = false;
      long lngTest;
      double dblTest;
      if (token[i].Len() == 0)
        continue;
      if (token[i].ToLong(&lngTest) == true)
        isNumber = true;
      if (token[i].ToDouble(&dblTest) == true)
        isNumber = true;
      if (isNumber == false)
        quotedList = true;
    }
  if (quotedList == true)
    {
      for (i = 0; i < nextToken; i++)
        {
          if (token[i].Len() == 0)
            continue;
          wxString str;
          SqlCleanString(token[i], str);
          token[i] = str;
        }
    }
  clean = wxT("");
  bool comma = false;
  for (i = 0; i < nextToken; i++)
    {
      if (token[i].Len() == 0)
        continue;
      if (comma == true)
        clean += wxT(", ");
      if (quotedList == true)
        clean += wxT("'");
      clean += token[i];
      if (quotedList == true)
        clean += wxT("'");
      comma = true;
    }
}

void SqlFiltersDialog::OnPageChanged(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changed
//
  switch (event.GetSelection())
    {
      case 0:
        Page1->UpdatePage();
        break;
      case 1:
        Page2->UpdatePage();
        break;
      case 2:
        Page3->UpdatePage();
        break;
    };
}

void SqlFiltersDialog::DoWhereClause(int index)
{
//
// composing a WHERE clause
//
  char *xname;
  char *xname2;
  wxString column;
  wxString comp;
  wxString value;
  bool enabled;

  switch (index)
    {
      case 1:
        Where1Clause = wxT("");
        column = Where1Column;
        comp = Where1Operator;
        value = Where1Value;
        enabled = Where1Enabled;
        break;
      case 2:
        Where2Clause = wxT("");
        column = Where2Column;
        comp = Where2Operator;
        value = Where2Value;
        enabled = Where2Enabled;
        break;
      default:
        Where3Clause = wxT("");
        column = Where3Column;
        comp = Where3Operator;
        value = Where3Value;
        enabled = Where3Enabled;
        break;
    };

  if (enabled == true)
    {
      if (column.Len() > 0 && comp.Len() > 0)
        {
          bool is_null = false;
          if (comp == wxT("IS NULL") || comp == wxT("IS NOT NULL"))
            is_null = true;
          if (is_null == true)
            {
              // a clause such as "column IS NULL" or "column IS NOT NULL"
              wxString clause;
              xname = (char *) malloc((column.Len() * 4) + 1);
              strcpy(xname, column.ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              clause = wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
              free(xname2);
              clause += wxT(" ") + comp;
              switch (index)
                {
                  case 1:
                    Where1Clause = clause;
                    break;
                  case 2:
                    Where2Clause = clause;
                    break;
                  default:
                    Where3Clause = clause;
                    break;
                };
          } else if (comp == wxT("IN") && value.Len() > 0)
            {
              // a clause such as "column IN (a, b, c, d)"
              wxString clause;
              xname = (char *) malloc((column.Len() * 4) + 1);
              strcpy(xname, column.ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              clause = wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
              free(xname2);
              clause += wxT(" IN (");
              wxString cleanList;
              SqlCleanList(value, cleanList);
              clause += cleanList;
              clause += wxT(")");
              switch (index)
                {
                  case 1:
                    Where1Clause = clause;
                    break;
                  case 2:
                    Where2Clause = clause;
                    break;
                  default:
                    Where3Clause = clause;
                    break;
                };
          } else if (value.Len() > 0)
            {
              // a clause such as "column = value", "column <value" and so on
              wxString clause;
              xname = (char *) malloc((column.Len() * 4) + 1);
              strcpy(xname, column.ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              clause = wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
              free(xname2);
              clause += wxT(" ");
              clause += comp;
              clause += wxT(" ");
              bool isNumber = false;
              long lngTest;
              double dblTest;
              if (value.ToLong(&lngTest) == true)
                isNumber = true;
              if (value.ToDouble(&dblTest) == true)
                isNumber = true;
              bool noQuote;
              wxString cleanSql;
              if (isNumber == false)
                noQuote = SqlCleanString(value, cleanSql);
              else
                {
                  noQuote = true;
                  cleanSql = value;
                }
              if (noQuote == false)
                clause += wxT("'");
              clause += cleanSql;
              if (noQuote == false)
                clause += wxT("'");
              switch (index)
                {
                  case 1:
                    Where1Clause = clause;
                    break;
                  case 2:
                    Where2Clause = clause;
                    break;
                  default:
                    Where3Clause = clause;
                    break;
                };
            }
        }
    }
}

void SqlFiltersDialog::DoOrderBy(int index)
{
//
// composing an ORDER BY fragment
//
  char *xname;
  char *xname2;
  wxString column;
  bool desc;
  bool enabled;
  switch (index)
    {
      case 1:
        OrderBy1 = wxT("");
        column = Order1Column;
        desc = Order1Desc;
        enabled = Order1Enabled;
        break;
      case 2:
        OrderBy2 = wxT("");
        column = Order2Column;
        desc = Order2Desc;
        enabled = Order2Enabled;
        break;
      case 3:
        OrderBy3 = wxT("");
        column = Order3Column;
        desc = Order3Desc;
        enabled = Order3Enabled;
        break;
      default:
        OrderBy4 = wxT("");
        column = Order4Column;
        desc = Order4Desc;
        enabled = Order4Enabled;
        break;
    };
  if (enabled == true)
    {
      if (column.Len() > 0)
        {
          wxString clause;
          xname = (char *) malloc((column.Len() * 4) + 1);
          strcpy(xname, column.ToUTF8());
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          clause = wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
          free(xname2);
          if (desc == true)
            clause += wxT(" DESC");
          else
            clause += wxT(" ASC");
          switch (index)
            {
              case 1:
                OrderBy1 = clause;
                break;
              case 2:
                OrderBy2 = clause;
                break;
              case 3:
                OrderBy3 = clause;
                break;
              default:
                OrderBy4 = clause;
                break;
            };
        }
    }
}

void SqlFiltersDialog::UpdateSqlSample()
{
//
// composing the SQL statement
//
  AuxColumn *pCol;
  char *xname;
  char *xname2;
  bool comma = false;
  SqlSample = wxT("SELECT");

  if (ReadOnly == false)
    {
      SqlSample += wxT(" ROWID");
      comma = true;
    }
  pCol = ColumnList.GetFirst();
  while (pCol)
    {
      if (ReadOnly == false && pCol->GetName() == wxT("ROWID"))
        {
          pCol = pCol->GetNext();
          continue;
        }
      xname = (char *) malloc((pCol->GetName().Len() * 4) + 1);
      strcpy(xname, pCol->GetName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      wxString col;
      if (comma == true)
        col = wxT(", \"") + wxString::FromUTF8(xname2) + wxT("\"");
      else
        col = wxT(" \"") + wxString::FromUTF8(xname2) + wxT("\"");
      free(xname2);
      SqlSample += col;
      comma = true;
      pCol = pCol->GetNext();
    }
  SqlSample += wxT("\nFROM \"");
  if (DbPrefix.CmpNoCase(wxT("MAIN")) != 0)
    {
      xname = (char *) malloc((DbPrefix.Len() * 4) + 1);
      strcpy(xname, DbPrefix.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      SqlSample += wxString::FromUTF8(xname2);
      free(xname2);
      SqlSample += wxT("\".\"");
    }
  xname = (char *) malloc((TableName.Len() * 4) + 1);
  strcpy(xname, TableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  SqlSample += wxString::FromUTF8(xname2);
  free(xname2);
  SqlSample += wxT("\"");
  if (FreeHand.Len() > 0)
    {
      SqlSample += wxT("\n") + FreeHand;
  } else
    {
      DoWhereClause(1);
      DoWhereClause(2);
      DoWhereClause(3);
      DoOrderBy(1);
      DoOrderBy(2);
      DoOrderBy(3);
      DoOrderBy(4);
      if (Where1Clause.Len() > 0 || Where2Clause.Len() > 0
          || Where3Clause.Len() > 0)
        {
          SqlSample += wxT("\nWHERE ");
          if (Where1Clause.Len() > 0)
            SqlSample += Where1Clause;
          if (Where1Clause.Len() > 0 && Where2Clause.Len() > 0)
            {
              if (AndOr12 == true)
                SqlSample += wxT(" AND ");
              else
                SqlSample += wxT(" OR ");
            }
          if (Where2Clause.Len() > 0)
            SqlSample += Where2Clause;
          if (Where2Clause.Len() > 0 && Where3Clause.Len() > 0)
            {
              if (AndOr23 == true)
                SqlSample += wxT(" AND ");
              else
                SqlSample += wxT(" OR ");
            }
          if (Where2Clause.Len() > 0 && Where3Clause.Len() > 0)
            SqlSample += Where3Clause;
        }
      if (OrderBy1.Len() > 0 || OrderBy2.Len() > 0 || OrderBy3.Len() > 0
          || OrderBy4.Len() > 0)
        {
          bool comma = false;
          SqlSample += wxT("\nORDER BY ");
          if (OrderBy1.Len() > 0)
            {
              SqlSample += OrderBy1;
              comma = true;
            }
          if (OrderBy2.Len() > 0)
            {
              if (comma == true)
                SqlSample += wxT(", ");
              SqlSample += OrderBy2;
              comma = true;
            }
          if (OrderBy3.Len() > 0)
            {
              if (comma == true)
                SqlSample += wxT(", ");
              SqlSample += OrderBy3;
              comma = true;
            }
          if (OrderBy4.Len() > 0)
            {
              if (comma == true)
                SqlSample += wxT(", ");
              SqlSample += OrderBy4;
              comma = true;
            }
        }
    }

  SqlCtrl->SetValue(SqlSample);
  DoSqlSyntaxColor();
}

bool SqlFiltersDialog::SqlCleanString(wxString & dirty, wxString & clean)
{
// well-formatting a string to be used as an SQL string
  char org[8192];
  char dst[8192];
  int len;
  char *in = org;
  char *out = dst;
  bool ret = false;
  strcpy(org, dirty.ToUTF8());
  len = strlen(org);
  if (len > 2 && *org == '\'' && *(org + (len - 1)) == '\'')
    {
      strcpy(dst, org);
      ret = true;
  } else
    {
      while (*in != '\0')
        {
          if (*in == '\'')
            *out++ = '\'';
          *out++ = *in++;
        }
      *out = '\0';
    }
  clean = wxString::FromUTF8(dst);
  return ret;
}

void SqlFiltersDialog::OnSqlSyntaxColor(wxCommandEvent & event)
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
}

void SqlFiltersDialog::DoSqlSyntaxColor()
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
      } else if (MyQueryView::IsSqliteExtra(token))
        {
          // setting the SQL keyword style
          SqlCtrl->SetStyle(from, to, sql_style);
      } else if (MyQueryView::IsSqlString(token) == true)
        {
          // setting the SQL string constant style
          SqlCtrl->SetStyle(from, to, const_style);
      } else if (MyQueryView::IsSqlNumber(token) == true)
        {
          // setting the SQL numeric constant style
          SqlCtrl->SetStyle(from, to, const_style);
      } else if (MyQueryView::IsSqlFunction(token, next_c) == true)
        {
          // setting the SQL function style
          SqlCtrl->SetStyle(from, to, fnct_style);
      } else if (MyQueryView::IsSqlGeoFunction(token, next_c) == true)
        {
          // setting the SQL geo-function style
          SqlCtrl->SetStyle(from, to, fnct_style);
      } else if (MyQueryView::IsSqlRasterFunction(token, next_c) == true)
        {
          // setting the SQL raster-function style
          SqlCtrl->SetStyle(from, to, fnct_style);
        }
      delete[]word;
    }
  SqlCtrl->Show();
  SqlCtrl->SetFocus();
  IgnoreEvent = false;
}

void SqlFiltersDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool SqlFiltersMainPage::Create(SqlFiltersDialog * parent)
{
//
// creating the dialog
//
  Parent = parent;
  if (wxPanel::Create(Parent->GetTabCtrl()) == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void SqlFiltersMainPage::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxSize size;
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);

// the WHERE clauses row
  wxBoxSizer *whereSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(whereSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);

// the WHERE clause #1 pane
  wxBoxSizer *where1BoxSizer = new wxBoxSizer(wxVERTICAL);
  whereSizer->Add(where1BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *where1Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Filter #1"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *where1Sizer = new wxStaticBoxSizer(where1Box, wxVERTICAL);
  where1BoxSizer->Add(where1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where1EnabledCtrl =
    new wxCheckBox(this, ID_SQL_FC_WHERE_1_ENABLE, wxT("Enable"));
  Where1EnabledCtrl->SetValue(Parent->IsWhere1Enabled());
  where1Sizer->Add(Where1EnabledCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxStaticBox *cols1Box = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Column to be filtered"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *cols1Sizer = new wxStaticBoxSizer(cols1Box, wxVERTICAL);
  where1Sizer->Add(cols1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where1ColumnCtrl =
    new wxComboBox(this, ID_SQL_FC_WHERE_1_COLUMN, Parent->GetWhere1Column(),
                   wxDefaultPosition, wxSize(200, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  Where1ColumnCtrl->Enable(Parent->IsWhere1Enabled());
  InitializeColumns(Where1ColumnCtrl);
  Where1ColumnCtrl->SetValue(Parent->GetWhere1Column());
  cols1Sizer->Add(Where1ColumnCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *oper1Box = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Comparison operator"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *oper1Sizer = new wxStaticBoxSizer(oper1Box, wxVERTICAL);
  where1Sizer->Add(oper1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where1OperatorCtrl =
    new wxComboBox(this, ID_SQL_FC_WHERE_1_OPERATOR,
                   Parent->GetWhere1Operator(), wxDefaultPosition, wxSize(200,
                                                                          21),
                   0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  Where1OperatorCtrl->Enable(Parent->IsWhere1Enabled());
  InitializeOperators(Where1OperatorCtrl);
  wxString compar;
  GetWhereOperator(Parent->GetWhere1Operator(), compar);
  Where1OperatorCtrl->SetValue(compar);
  oper1Sizer->Add(Where1OperatorCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  size = Where1OperatorCtrl->GetSize();
  wxStaticBox *value1Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Value"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *value1Sizer = new wxStaticBoxSizer(value1Box, wxVERTICAL);
  where1Sizer->Add(value1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where1ValueCtrl =
    new wxTextCtrl(this, ID_SQL_FC_WHERE_1_VALUE, Parent->GetWhere1Value(),
                   wxDefaultPosition, size);
  bool enabled = Parent->IsWhere1Enabled();
  if (Parent->GetWhere1Value() == wxT("IS NULL"))
    enabled = false;
  if (Parent->GetWhere1Value() == wxT("IS NOT NULL"))
    enabled = false;
  Where1ValueCtrl->Enable(enabled);
  value1Sizer->Add(Where1ValueCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);

// the CONNECTOR 1-2 pane
  wxString bool12[2];
  bool12[0] = wxT("&AND");
  bool12[1] = wxT("&OR");
  Connector12Ctrl = new wxRadioBox(this, ID_SQL_FC_CONNECTOR_12,
                                   wxT(""),
                                   wxDefaultPosition, wxDefaultSize, 2,
                                   bool12, 1, wxRA_SPECIFY_COLS);
  if (Parent->IsAndOr12() == true)
    Connector12Ctrl->SetSelection(0);
  else
    Connector12Ctrl->SetSelection(1);
  Connector12Ctrl->Enable(Parent->IsWhere1Enabled()
                          && Parent->IsWhere2Enabled());
  whereSizer->Add(Connector12Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

// the WHERE clause #2 pane
  wxBoxSizer *where2BoxSizer = new wxBoxSizer(wxVERTICAL);
  whereSizer->Add(where2BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *where2Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Filter #2"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *where2Sizer = new wxStaticBoxSizer(where2Box, wxVERTICAL);
  where2BoxSizer->Add(where2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where2EnabledCtrl =
    new wxCheckBox(this, ID_SQL_FC_WHERE_2_ENABLE, wxT("Enable"));
  Where2EnabledCtrl->SetValue(Parent->IsWhere2Enabled());
  where2Sizer->Add(Where2EnabledCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxStaticBox *cols2Box = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Column to be filtered"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *cols2Sizer = new wxStaticBoxSizer(cols2Box, wxVERTICAL);
  where2Sizer->Add(cols2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where2ColumnCtrl =
    new wxComboBox(this, ID_SQL_FC_WHERE_2_COLUMN, Parent->GetWhere2Column(),
                   wxDefaultPosition, wxSize(200, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  Where2ColumnCtrl->Enable(Parent->IsWhere2Enabled());
  InitializeColumns(Where2ColumnCtrl);
  Where2ColumnCtrl->SetValue(Parent->GetWhere2Column());
  cols2Sizer->Add(Where2ColumnCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *oper2Box = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Comparison operator"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *oper2Sizer = new wxStaticBoxSizer(oper2Box, wxVERTICAL);
  where2Sizer->Add(oper2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where2OperatorCtrl =
    new wxComboBox(this, ID_SQL_FC_WHERE_2_OPERATOR,
                   Parent->GetWhere2Operator(), wxDefaultPosition, wxSize(200,
                                                                          21),
                   0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  Where2OperatorCtrl->Enable(Parent->IsWhere2Enabled());
  InitializeOperators(Where2OperatorCtrl);
  GetWhereOperator(Parent->GetWhere2Operator(), compar);
  Where2OperatorCtrl->SetValue(compar);
  oper2Sizer->Add(Where2OperatorCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  size = Where2OperatorCtrl->GetSize();
  wxStaticBox *value2Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Value"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *value2Sizer = new wxStaticBoxSizer(value2Box, wxVERTICAL);
  where2Sizer->Add(value2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where2ValueCtrl =
    new wxTextCtrl(this, ID_SQL_FC_WHERE_2_VALUE, Parent->GetWhere2Value(),
                   wxDefaultPosition, size);
  enabled = Parent->IsWhere1Enabled();
  if (Parent->GetWhere2Value() == wxT("IS NULL"))
    enabled = false;
  if (Parent->GetWhere2Value() == wxT("IS NOT NULL"))
    enabled = false;
  Where2ValueCtrl->Enable(enabled);
  value2Sizer->Add(Where2ValueCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);

// the CONNECTOR 2-3 pane
  wxString bool23[2];
  bool23[0] = wxT("&AND");
  bool23[1] = wxT("&OR");
  Connector23Ctrl = new wxRadioBox(this, ID_SQL_FC_CONNECTOR_23,
                                   wxT(""),
                                   wxDefaultPosition, wxDefaultSize, 2,
                                   bool12, 1, wxRA_SPECIFY_COLS);
  if (Parent->IsAndOr23() == true)
    Connector23Ctrl->SetSelection(0);
  else
    Connector23Ctrl->SetSelection(1);
  Connector23Ctrl->Enable(Parent->IsWhere2Enabled()
                          && Parent->IsWhere3Enabled());
  whereSizer->Add(Connector23Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

// the WHERE clause #3 pane
  wxBoxSizer *where3BoxSizer = new wxBoxSizer(wxVERTICAL);
  whereSizer->Add(where3BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *where3Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Filter #3"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *where3Sizer = new wxStaticBoxSizer(where3Box, wxVERTICAL);
  where3BoxSizer->Add(where3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where3EnabledCtrl =
    new wxCheckBox(this, ID_SQL_FC_WHERE_3_ENABLE, wxT("Enable"));
  Where3EnabledCtrl->SetValue(Parent->IsWhere3Enabled());
  where3Sizer->Add(Where3EnabledCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxStaticBox *cols3Box = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Column to be filtered"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *cols3Sizer = new wxStaticBoxSizer(cols3Box, wxVERTICAL);
  where3Sizer->Add(cols3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where3ColumnCtrl =
    new wxComboBox(this, ID_SQL_FC_WHERE_3_COLUMN, Parent->GetWhere3Column(),
                   wxDefaultPosition, wxSize(200, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  Where3ColumnCtrl->Enable(Parent->IsWhere3Enabled());
  InitializeColumns(Where3ColumnCtrl);
  Where3ColumnCtrl->SetValue(Parent->GetWhere3Column());
  cols3Sizer->Add(Where3ColumnCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *oper3Box = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Comparison operator"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *oper3Sizer = new wxStaticBoxSizer(oper3Box, wxVERTICAL);
  where3Sizer->Add(oper3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where3OperatorCtrl =
    new wxComboBox(this, ID_SQL_FC_WHERE_3_OPERATOR,
                   Parent->GetWhere3Operator(), wxDefaultPosition, wxSize(200,
                                                                          21),
                   0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  Where3OperatorCtrl->Enable(Parent->IsWhere3Enabled());
  InitializeOperators(Where3OperatorCtrl);
  GetWhereOperator(Parent->GetWhere3Operator(), compar);
  Where3OperatorCtrl->SetValue(compar);
  oper3Sizer->Add(Where3OperatorCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  size = Where3OperatorCtrl->GetSize();
  wxStaticBox *value3Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Value"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *value3Sizer = new wxStaticBoxSizer(value3Box, wxVERTICAL);
  where3Sizer->Add(value3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Where3ValueCtrl =
    new wxTextCtrl(this, ID_SQL_FC_WHERE_3_VALUE, Parent->GetWhere3Value(),
                   wxDefaultPosition, size);
  enabled = Parent->IsWhere1Enabled();
  if (Parent->GetWhere3Value() == wxT("IS NULL"))
    enabled = false;
  if (Parent->GetWhere3Value() == wxT("IS NOT NULL"))
    enabled = false;
  Where3ValueCtrl->Enable(enabled);
  value3Sizer->Add(Where3ValueCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);

// adding the event handlers
  Connect(ID_SQL_FC_WHERE_1_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnWhere1Enabled);
  Connect(ID_SQL_FC_WHERE_2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnWhere2Enabled);
  Connect(ID_SQL_FC_WHERE_3_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnWhere3Enabled);
  Connect(ID_SQL_FC_CONNECTOR_12, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnConnector12Changed);
  Connect(ID_SQL_FC_CONNECTOR_23, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnConnector23Changed);
  Connect(ID_SQL_FC_WHERE_1_COLUMN, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnWhere1ColumnSelected);
  Connect(ID_SQL_FC_WHERE_2_COLUMN, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnWhere2ColumnSelected);
  Connect(ID_SQL_FC_WHERE_3_COLUMN, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnWhere3ColumnSelected);
  Connect(ID_SQL_FC_WHERE_1_OPERATOR, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) &
          SqlFiltersMainPage::OnWhere1OperatorSelected);
  Connect(ID_SQL_FC_WHERE_2_OPERATOR, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) &
          SqlFiltersMainPage::OnWhere2OperatorSelected);
  Connect(ID_SQL_FC_WHERE_3_OPERATOR, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) &
          SqlFiltersMainPage::OnWhere3OperatorSelected);
  Connect(ID_SQL_FC_WHERE_1_VALUE, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnWhere1ValueChanged);
  Connect(ID_SQL_FC_WHERE_2_VALUE, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnWhere2ValueChanged);
  Connect(ID_SQL_FC_WHERE_3_VALUE, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) & SqlFiltersMainPage::OnWhere3ValueChanged);
}

void SqlFiltersMainPage::ResetWizard()
{
//
// resetting the wizard page
//
  Parent->SetWhere1Enabled(false);
  Parent->SetWhere2Enabled(false);
  Parent->SetWhere3Enabled(false);
  Where1EnabledCtrl->Enable(true);
  Where2EnabledCtrl->Enable(true);
  Where2EnabledCtrl->Enable(true);
  Where1ColumnCtrl->Enable(false);
  Where2ColumnCtrl->Enable(false);
  Where3ColumnCtrl->Enable(false);
  Where1ValueCtrl->Enable(false);
  Where2ValueCtrl->Enable(false);
  Where3ValueCtrl->Enable(false);
  Where1OperatorCtrl->Enable(false);
  Where2OperatorCtrl->Enable(false);
  Where3OperatorCtrl->Enable(false);
  Connector12Ctrl->Enable(false);
  Connector23Ctrl->Enable(false);
}

void SqlFiltersMainPage::UpdatePage()
{
//
// updating the Filters page
//
  Where1EnabledCtrl->SetValue(Parent->IsWhere1Enabled());
  Where2EnabledCtrl->SetValue(Parent->IsWhere2Enabled());
  Where3EnabledCtrl->SetValue(Parent->IsWhere3Enabled());
}

void SqlFiltersMainPage::InitializeOperators(wxComboBox * ctrl)
{
// initializing a list of boolean operators ComboBox
  ctrl->Clear();
  ctrl->Append(wxT("= {equal to}"));
  ctrl->Append(wxT("<> {not equal}"));
  ctrl->Append(wxT("< {lesser than}"));
  ctrl->Append(wxT("<= {lesser / equal}"));
  ctrl->Append(wxT("> {greather than}"));
  ctrl->Append(wxT(">= {greather / equal}"));
  ctrl->Append(wxT("LIKE {text search}"));
  ctrl->Append(wxT("IN {val1, val2, ...}"));
  ctrl->Append(wxT("IS NULL"));
  ctrl->Append(wxT("IS NOT NULL"));
}

void SqlFiltersMainPage::GetWhereOperator(wxString & value, wxString & compar)
{
// retriving the Operator full name
  if (value == wxT("="))
    {
      compar = wxT("= {equal to}");
      return;
    }
  if (value == wxT("<>"))
    {
      compar = wxT("<> {not equal}");
      return;
    }
  if (value == wxT("<"))
    {
      compar = wxT("< {lesser than}");
      return;
    }
  if (value == wxT("<="))
    {
      compar = wxT("<= {lesser / equal}");
      return;
    }
  if (value == wxT(">"))
    {
      compar = wxT("> {greather than}");
      return;
    }
  if (value == wxT(">="))
    {
      compar = wxT(">= {greather / equal}");
      return;
    }
  if (value == wxT("LIKE"))
    {
      compar = wxT("LIKE {text search}");
      return;
    }
  if (value == wxT("IN"))
    {
      compar = wxT("IN {val1, val2, ...}");
      return;
    }
  compar = value;

}

void SqlFiltersMainPage::InitializeColumns(wxComboBox * ctrl)
{
// initializing a columns list ComboBox
  ctrl->Clear();
  AuxColumn *pColumn = Parent->GetColumnList()->GetFirst();
  while (pColumn)
    {
      wxString col = pColumn->GetName();
      ctrl->Append(col);
      pColumn = pColumn->GetNext();
    }
}

void SqlFiltersMainPage::OnWhere1Enabled(wxCommandEvent & WXUNUSED(event))
{
//
// Where #1 clause enabled/disabled
//
  Parent->ResetFreeHand();
  Parent->SetWhere1Enabled(Where1EnabledCtrl->GetValue());
  if (Parent->IsWhere1Enabled() == true)
    {
      Where1ColumnCtrl->Enable(true);
      Where1OperatorCtrl->Enable(true);
      Where1ValueCtrl->Enable(true);
      if (Parent->IsWhere2Enabled() == true)
        Connector12Ctrl->Enable(true);
  } else
    {
      Where1ColumnCtrl->Enable(false);
      Where1OperatorCtrl->Enable(false);
      Where1ValueCtrl->Enable(false);
      Connector12Ctrl->Enable(false);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::OnWhere2Enabled(wxCommandEvent & WXUNUSED(event))
{
//
// Where #2 clause enabled/disabled
//
  Parent->ResetFreeHand();
  Parent->SetWhere2Enabled(Where2EnabledCtrl->GetValue());
  if (Parent->IsWhere2Enabled() == true)
    {
      Where2ColumnCtrl->Enable(true);
      Where2OperatorCtrl->Enable(true);
      Where2ValueCtrl->Enable(true);
      if (Parent->IsWhere1Enabled() == true)
        Connector12Ctrl->Enable(true);
      if (Parent->IsWhere3Enabled() == true)
        Connector23Ctrl->Enable(true);
  } else
    {
      Where2ColumnCtrl->Enable(false);
      Where2OperatorCtrl->Enable(false);
      Where2ValueCtrl->Enable(false);
      Parent->SetAndOr12(true);
      Connector12Ctrl->SetSelection(0);
      Connector12Ctrl->Enable(false);
      Connector12Ctrl->Enable(false);
      Connector23Ctrl->Enable(false);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::OnWhere3Enabled(wxCommandEvent & WXUNUSED(event))
{
//
// Where #3 clause enabled/disabled
//
  Parent->ResetFreeHand();
  Parent->SetWhere3Enabled(Where3EnabledCtrl->GetValue());
  if (Parent->IsWhere3Enabled() == true)
    {
      Where3ColumnCtrl->Enable(true);
      Where3OperatorCtrl->Enable(true);
      Where3ValueCtrl->Enable(true);
      if (Parent->IsWhere2Enabled() == true)
        Connector23Ctrl->Enable(true);
  } else
    {
      Where3ColumnCtrl->Enable(false);
      Where3OperatorCtrl->Enable(false);
      Where3ValueCtrl->Enable(false);
      Connector23Ctrl->Enable(false);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::OnConnector12Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Connector 1-2 changed
//
  Parent->ResetFreeHand();
  if (Connector12Ctrl->GetSelection() == 0)
    Parent->SetAndOr12(true);
  else
    Parent->SetAndOr12(false);
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::OnConnector23Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Connector 2-3 changed
//
  Parent->ResetFreeHand();
  if (Connector23Ctrl->GetSelection() == 0)
    Parent->SetAndOr23(true);
  else
    Parent->SetAndOr23(false);
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::
OnWhere1ColumnSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Where #1 column changed
//
  Parent->SetWhere1Column(Where1ColumnCtrl->GetValue());
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::
OnWhere2ColumnSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Where #2 column changed
//
  Parent->ResetFreeHand();
  Parent->SetWhere2Column(Where2ColumnCtrl->GetValue());
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::
OnWhere3ColumnSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Where #3 column changed
//
  Parent->ResetFreeHand();
  Parent->SetWhere3Column(Where3ColumnCtrl->GetValue());
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::
OnWhere1OperatorSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Where #1 operator changed
//
  Parent->ResetFreeHand();
  wxString str = Where1OperatorCtrl->GetValue();
  wxString clean = wxT("");
  if (str == wxT("= {equal to}"))
    clean = wxT("=");
  if (str == wxT("<> {not equal}"))
    clean = wxT("<>");
  if (str == wxT("< {lesser than}"))
    clean = wxT("<");
  if (str == wxT("<= {lesser / equal}"))
    clean = wxT("<=");
  if (str == wxT("> {greather than}"))
    clean = wxT(">");
  if (str == wxT(">= {greather / equal}"))
    clean = wxT(">=");
  if (str == wxT("LIKE {text search}"))
    clean = wxT("LIKE");
  if (str == wxT("IN {val1, val2, ...}"))
    clean = wxT("IN");
  if (str == wxT("IS NULL"))
    clean = wxT("IS NULL");
  if (str == wxT("IS NOT NULL"))
    clean = wxT("IS NOT NULL");
  Parent->SetWhere1Operator(clean);
  if (Parent->GetWhere1Operator() == wxT("IS NULL")
      || Parent->GetWhere1Operator() == wxT("IS NOT NULL"))
    {
      Where1ValueCtrl->SetValue(wxT(""));
      Parent->SetWhere1Value(wxT(""));
      Where1ValueCtrl->Enable(false);
  } else
    {
      Where1ValueCtrl->SetValue(Parent->GetWhere1Value());
      Where1ValueCtrl->Enable(true);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::
OnWhere2OperatorSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Where #2 operator changed
//
  Parent->ResetFreeHand();
  wxString str = Where2OperatorCtrl->GetValue();
  wxString clean = wxT("");
  if (str == wxT("= {equal to}"))
    clean = wxT("=");
  if (str == wxT("<> {not equal}"))
    clean = wxT("<>");
  if (str == wxT("< {lesser than}"))
    clean = wxT("<");
  if (str == wxT("<= {lesser / equal}"))
    clean = wxT("<=");
  if (str == wxT("> {greather than}"))
    clean = wxT(">");
  if (str == wxT(">= {greather / equal}"))
    clean = wxT(">=");
  if (str == wxT("LIKE {text search}"))
    clean = wxT("LIKE");
  if (str == wxT("IN {val1, val2, ...}"))
    clean = wxT("IN");
  if (str == wxT("IS NULL"))
    clean = wxT("IS NULL");
  if (str == wxT("IS NOT NULL"))
    clean = wxT("IS NOT NULL");
  Parent->SetWhere2Operator(clean);
  if (Parent->GetWhere2Operator() == wxT("IS NULL")
      || Parent->GetWhere2Operator() == wxT("IS NOT NULL"))
    {
      Where2ValueCtrl->SetValue(wxT(""));
      Parent->SetWhere2Value(wxT(""));
      Where2ValueCtrl->Enable(false);
  } else
    {
      Where2ValueCtrl->SetValue(Parent->GetWhere2Value());
      Where2ValueCtrl->Enable(true);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::
OnWhere3OperatorSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Where #3 operator changed
//
  Parent->ResetFreeHand();
  wxString str = Where3OperatorCtrl->GetValue();
  wxString clean = wxT("");
  if (str == wxT("= {equal to}"))
    clean = wxT("=");
  if (str == wxT("<> {not equal}"))
    clean = wxT("<>");
  if (str == wxT("< {lesser than}"))
    clean = wxT("<");
  if (str == wxT("<= {lesser / equal}"))
    clean = wxT("<=");
  if (str == wxT("> {greather than}"))
    clean = wxT(">");
  if (str == wxT(">= {greather / equal}"))
    clean = wxT(">=");
  if (str == wxT("LIKE {text search}"))
    clean = wxT("LIKE");
  if (str == wxT("IN {val1, val2, ...}"))
    clean = wxT("IN");
  if (str == wxT("IS NULL"))
    clean = wxT("IS NULL");
  if (str == wxT("IS NOT NULL"))
    clean = wxT("IS NOT NULL");
  Parent->SetWhere3Operator(clean);
  if (Parent->GetWhere3Operator() == wxT("IS NULL")
      || Parent->GetWhere3Operator() == wxT("IS NOT NULL"))
    {
      Where3ValueCtrl->SetValue(wxT(""));
      Parent->SetWhere3Value(wxT(""));
      Where3ValueCtrl->Enable(false);
  } else
    {
      Where3ValueCtrl->SetValue(Parent->GetWhere3Value());
      Where3ValueCtrl->Enable(true);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersMainPage::OnWhere1ValueChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Where #1 value changed
//
  Parent->ResetFreeHand();
  Parent->SetWhere1Value(Where1ValueCtrl->GetValue());
  Parent->UpdateSqlSample();
  Where1ValueCtrl->SetFocus();
}

void SqlFiltersMainPage::OnWhere2ValueChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Where #2 value changed
//
  Parent->ResetFreeHand();
  Parent->SetWhere2Value(Where2ValueCtrl->GetValue());
  Parent->UpdateSqlSample();
  Where2ValueCtrl->SetFocus();
}

void SqlFiltersMainPage::OnWhere3ValueChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Where #3 value changed
//
  Parent->ResetFreeHand();
  Parent->SetWhere3Value(Where3ValueCtrl->GetValue());
  Parent->UpdateSqlSample();
  Where3ValueCtrl->SetFocus();
}

bool SqlFiltersOrderPage::Create(SqlFiltersDialog * parent)
{
//
// creating the dialog
//
  Parent = parent;
  if (wxPanel::Create(Parent->GetTabCtrl()) == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void SqlFiltersOrderPage::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxSize size;
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);

// the ORDER BY clauses row
  wxBoxSizer *OrderSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(OrderSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);

// the Order clause #1 pane
  wxBoxSizer *Order1BoxSizer = new wxBoxSizer(wxVERTICAL);
  OrderSizer->Add(Order1BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *Order1Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Order by #1"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *Order1Sizer = new wxStaticBoxSizer(Order1Box, wxVERTICAL);
  Order1BoxSizer->Add(Order1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Order1EnabledCtrl =
    new wxCheckBox(this, ID_SQL_FC_ORDER_1_ENABLE, wxT("Enable"));
  Order1EnabledCtrl->SetValue(Parent->IsOrder1Enabled());
  Order1Sizer->Add(Order1EnabledCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  Order1ColumnCtrl =
    new wxComboBox(this, ID_SQL_FC_ORDER_1_COLUMN, Parent->GetOrder1Column(),
                   wxDefaultPosition, wxSize(175, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  Order1ColumnCtrl->Enable(Parent->IsOrder1Enabled());
  InitializeColumns(Order1ColumnCtrl);
  Order1ColumnCtrl->SetValue(Parent->GetOrder1Column());
  Order1Sizer->Add(Order1ColumnCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxString desc1[2];
  desc1[0] = wxT("&Descending [Z-A]");
  desc1[1] = wxT("&Ascending [A-Z]");
  Order1DescCtrl = new wxRadioBox(this, ID_SQL_FC_ORDER_1_DESC,
                                  wxT("Direction"),
                                  wxDefaultPosition, wxDefaultSize, 2,
                                  desc1, 1, wxRA_SPECIFY_COLS);
  if (Parent->IsOrder1Desc() == true)
    Order1DescCtrl->SetSelection(0);
  else
    Order1DescCtrl->SetSelection(1);
  Order1DescCtrl->Enable(Parent->IsOrder1Enabled());
  Order1Sizer->Add(Order1DescCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);

// the Order clause #2 pane
  wxBoxSizer *Order2BoxSizer = new wxBoxSizer(wxVERTICAL);
  OrderSizer->Add(Order2BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *Order2Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Order by #2"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *Order2Sizer = new wxStaticBoxSizer(Order2Box, wxVERTICAL);
  Order2BoxSizer->Add(Order2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Order2EnabledCtrl =
    new wxCheckBox(this, ID_SQL_FC_ORDER_2_ENABLE, wxT("Enable"));
  Order2EnabledCtrl->SetValue(Parent->IsOrder2Enabled());
  Order2Sizer->Add(Order2EnabledCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  Order2ColumnCtrl =
    new wxComboBox(this, ID_SQL_FC_ORDER_2_COLUMN, Parent->GetOrder2Column(),
                   wxDefaultPosition, wxSize(175, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  Order2ColumnCtrl->Enable(Parent->IsOrder2Enabled());
  InitializeColumns(Order2ColumnCtrl);
  Order2ColumnCtrl->SetValue(Parent->GetOrder2Column());
  Order2Sizer->Add(Order2ColumnCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxString desc2[2];
  desc2[0] = wxT("&Descending [Z-A]");
  desc2[1] = wxT("&Ascending [A-Z]");
  Order2DescCtrl = new wxRadioBox(this, ID_SQL_FC_ORDER_2_DESC,
                                  wxT("Direction"),
                                  wxDefaultPosition, wxDefaultSize, 2,
                                  desc2, 1, wxRA_SPECIFY_COLS);
  if (Parent->IsOrder2Desc() == true)
    Order2DescCtrl->SetSelection(0);
  else
    Order2DescCtrl->SetSelection(1);
  Order2DescCtrl->Enable(Parent->IsOrder2Enabled());
  Order2Sizer->Add(Order2DescCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);

// the Order clause #3 pane
  wxBoxSizer *Order3BoxSizer = new wxBoxSizer(wxVERTICAL);
  OrderSizer->Add(Order3BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *Order3Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Order by #3"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *Order3Sizer = new wxStaticBoxSizer(Order3Box, wxVERTICAL);
  Order3BoxSizer->Add(Order3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Order3EnabledCtrl =
    new wxCheckBox(this, ID_SQL_FC_ORDER_3_ENABLE, wxT("Enable"));
  Order3EnabledCtrl->SetValue(Parent->IsOrder3Enabled());
  Order3Sizer->Add(Order3EnabledCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  Order3ColumnCtrl =
    new wxComboBox(this, ID_SQL_FC_ORDER_3_COLUMN, Parent->GetOrder3Column(),
                   wxDefaultPosition, wxSize(175, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  Order3ColumnCtrl->Enable(Parent->IsOrder3Enabled());
  InitializeColumns(Order3ColumnCtrl);
  Order3ColumnCtrl->SetValue(Parent->GetOrder3Column());
  Order3Sizer->Add(Order3ColumnCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxString desc3[2];
  desc3[0] = wxT("&Descending [Z-A]");
  desc3[1] = wxT("&Ascending [A-Z]");
  Order3DescCtrl = new wxRadioBox(this, ID_SQL_FC_ORDER_3_DESC,
                                  wxT("Direction"),
                                  wxDefaultPosition, wxDefaultSize, 2,
                                  desc3, 1, wxRA_SPECIFY_COLS);
  if (Parent->IsOrder3Desc() == true)
    Order3DescCtrl->SetSelection(0);
  else
    Order3DescCtrl->SetSelection(1);
  Order3DescCtrl->Enable(Parent->IsOrder3Enabled());
  Order3Sizer->Add(Order3DescCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);

// the Order clause #4 pane
  wxBoxSizer *Order4BoxSizer = new wxBoxSizer(wxVERTICAL);
  OrderSizer->Add(Order4BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *Order4Box = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Order by #4"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *Order4Sizer = new wxStaticBoxSizer(Order4Box, wxVERTICAL);
  Order4BoxSizer->Add(Order4Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);
  Order4EnabledCtrl =
    new wxCheckBox(this, ID_SQL_FC_ORDER_4_ENABLE, wxT("Enable"));
  Order4EnabledCtrl->SetValue(Parent->IsOrder4Enabled());
  Order4Sizer->Add(Order4EnabledCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  Order4ColumnCtrl =
    new wxComboBox(this, ID_SQL_FC_ORDER_4_COLUMN, Parent->GetOrder4Column(),
                   wxDefaultPosition, wxSize(175, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  Order4ColumnCtrl->Enable(Parent->IsOrder4Enabled());
  InitializeColumns(Order4ColumnCtrl);
  Order4ColumnCtrl->SetValue(Parent->GetOrder4Column());
  Order4Sizer->Add(Order4ColumnCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxString desc4[2];
  desc4[0] = wxT("&Descending [Z-A]");
  desc4[1] = wxT("&Ascending [A-Z]");
  Order4DescCtrl = new wxRadioBox(this, ID_SQL_FC_ORDER_4_DESC,
                                  wxT("Direction"),
                                  wxDefaultPosition, wxDefaultSize, 2,
                                  desc4, 1, wxRA_SPECIFY_COLS);
  if (Parent->IsOrder4Desc() == true)
    Order4DescCtrl->SetSelection(0);
  else
    Order4DescCtrl->SetSelection(1);
  Order4DescCtrl->Enable(Parent->IsOrder4Enabled());
  Order4Sizer->Add(Order4DescCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 1);

// adding the event handlers
  Connect(ID_SQL_FC_ORDER_1_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & SqlFiltersOrderPage::OnOrder1Enabled);
  Connect(ID_SQL_FC_ORDER_2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & SqlFiltersOrderPage::OnOrder2Enabled);
  Connect(ID_SQL_FC_ORDER_3_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & SqlFiltersOrderPage::OnOrder3Enabled);
  Connect(ID_SQL_FC_ORDER_4_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & SqlFiltersOrderPage::OnOrder4Enabled);
  Connect(ID_SQL_FC_ORDER_1_COLUMN, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) &
          SqlFiltersOrderPage::OnOrder1ColumnSelected);
  Connect(ID_SQL_FC_ORDER_2_COLUMN, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) &
          SqlFiltersOrderPage::OnOrder2ColumnSelected);
  Connect(ID_SQL_FC_ORDER_3_COLUMN, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) &
          SqlFiltersOrderPage::OnOrder3ColumnSelected);
  Connect(ID_SQL_FC_ORDER_4_COLUMN, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) &
          SqlFiltersOrderPage::OnOrder4ColumnSelected);
  Connect(ID_SQL_FC_ORDER_1_DESC, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & SqlFiltersOrderPage::OnOrder1DescChanged);
  Connect(ID_SQL_FC_ORDER_2_DESC, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & SqlFiltersOrderPage::OnOrder2DescChanged);
  Connect(ID_SQL_FC_ORDER_3_DESC, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & SqlFiltersOrderPage::OnOrder3DescChanged);
  Connect(ID_SQL_FC_ORDER_4_DESC, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & SqlFiltersOrderPage::OnOrder4DescChanged);
}

void SqlFiltersOrderPage::ResetWizard()
{
//
// resetting the wizard page
//
  Parent->SetOrder1Enabled(false);
  Parent->SetOrder2Enabled(false);
  Parent->SetOrder3Enabled(false);
  Parent->SetOrder4Enabled(false);
  Order1EnabledCtrl->Enable(true);
  Order2EnabledCtrl->Enable(true);
  Order3EnabledCtrl->Enable(true);
  Order4EnabledCtrl->Enable(true);
  Order1ColumnCtrl->Enable(false);
  Order2ColumnCtrl->Enable(false);
  Order3ColumnCtrl->Enable(false);
  Order4ColumnCtrl->Enable(false);
  Order1DescCtrl->Enable(false);
  Order2DescCtrl->Enable(false);
  Order3DescCtrl->Enable(false);
  Order4DescCtrl->Enable(false);
}

void SqlFiltersOrderPage::UpdatePage()
{
//
// updating the Order page
//
  Order1EnabledCtrl->SetValue(Parent->IsOrder1Enabled());
  Order2EnabledCtrl->SetValue(Parent->IsOrder2Enabled());
  Order3EnabledCtrl->SetValue(Parent->IsOrder3Enabled());
  Order4EnabledCtrl->SetValue(Parent->IsOrder4Enabled());
}

void SqlFiltersOrderPage::OnOrder1Enabled(wxCommandEvent & WXUNUSED(event))
{
//
// Order #1 clause enabled/disabled
//
  Parent->ResetFreeHand();
  Parent->SetOrder1Enabled(Order1EnabledCtrl->GetValue());
  if (Parent->IsOrder1Enabled() == true)
    {
      Order1ColumnCtrl->Enable(true);
      Order1DescCtrl->Enable(true);
  } else
    {
      Order1ColumnCtrl->Enable(false);
      Order1DescCtrl->Enable(false);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::OnOrder2Enabled(wxCommandEvent & WXUNUSED(event))
{
//
// Order #2 clause enabled/disabled
//
  Parent->ResetFreeHand();
  Parent->SetOrder2Enabled(Order2EnabledCtrl->GetValue());
  if (Parent->IsOrder2Enabled() == true)
    {
      Order2ColumnCtrl->Enable(true);
      Order2DescCtrl->Enable(true);
  } else
    {
      Order2ColumnCtrl->Enable(false);
      Order2DescCtrl->Enable(false);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::OnOrder3Enabled(wxCommandEvent & WXUNUSED(event))
{
//
// Order #3 clause enabled/disabled
//
  Parent->ResetFreeHand();
  Parent->SetOrder3Enabled(Order3EnabledCtrl->GetValue());
  if (Parent->IsOrder3Enabled() == true)
    {
      Order3ColumnCtrl->Enable(true);
      Order3DescCtrl->Enable(true);
  } else
    {
      Order3ColumnCtrl->Enable(false);
      Order3DescCtrl->Enable(false);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::OnOrder4Enabled(wxCommandEvent & WXUNUSED(event))
{
//
// Order #4 clause enabled/disabled
//
  Parent->ResetFreeHand();
  Parent->SetOrder4Enabled(Order4EnabledCtrl->GetValue());
  if (Parent->IsOrder4Enabled() == true)
    {
      Order4ColumnCtrl->Enable(true);
      Order4DescCtrl->Enable(true);
  } else
    {
      Order4ColumnCtrl->Enable(false);
      Order4DescCtrl->Enable(false);
    }
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::InitializeColumns(wxComboBox * ctrl)
{
// initializing a columns list ComboBox
  ctrl->Clear();
  AuxColumn *pColumn = Parent->GetColumnList()->GetFirst();
  while (pColumn)
    {
      wxString col = pColumn->GetName();
      ctrl->Append(col);
      pColumn = pColumn->GetNext();
    }
}

void SqlFiltersOrderPage::
OnOrder1ColumnSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Order #1 column changed
//
  Parent->ResetFreeHand();
  Parent->SetOrder1Column(Order1ColumnCtrl->GetValue());
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::
OnOrder2ColumnSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Order #2 column changed
//
  Parent->ResetFreeHand();
  Parent->SetOrder2Column(Order2ColumnCtrl->GetValue());
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::
OnOrder3ColumnSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Order #3 column changed
//
  Parent->ResetFreeHand();
  Parent->SetOrder3Column(Order3ColumnCtrl->GetValue());
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::
OnOrder4ColumnSelected(wxCommandEvent & WXUNUSED(event))
{
//
// Order #4 column changed
//
  Parent->ResetFreeHand();
  Parent->SetOrder4Column(Order4ColumnCtrl->GetValue());
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::OnOrder1DescChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Order #1 Asc/Desc changed
//
  Parent->ResetFreeHand();
  if (Order1DescCtrl->GetSelection() == 0)
    Parent->SetOrder1Desc(true);
  else
    Parent->SetOrder1Desc(false);
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::OnOrder2DescChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Order #2 Asc/Desc changed
//
  Parent->ResetFreeHand();
  if (Order2DescCtrl->GetSelection() == 0)
    Parent->SetOrder2Desc(true);
  else
    Parent->SetOrder2Desc(false);
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::OnOrder3DescChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Order #3 Asc/Desc changed
//
  Parent->ResetFreeHand();
  if (Order3DescCtrl->GetSelection() == 0)
    Parent->SetOrder3Desc(true);
  else
    Parent->SetOrder3Desc(false);
  Parent->UpdateSqlSample();
}

void SqlFiltersOrderPage::OnOrder4DescChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Order #4 Asc/Desc changed
//
  Parent->ResetFreeHand();
  if (Order4DescCtrl->GetSelection() == 0)
    Parent->SetOrder4Desc(true);
  else
    Parent->SetOrder4Desc(false);
  Parent->UpdateSqlSample();
}

bool SqlFiltersFreeHandPage::Create(SqlFiltersDialog * parent)
{
//
// creating the dialog
//
  Parent = parent;
  if (wxPanel::Create(Parent->GetTabCtrl()) == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void SqlFiltersFreeHandPage::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxSize size;
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);

// the ORDER BY clauses row
  wxBoxSizer *OrderSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(OrderSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);

// the SQL FreeHand Clauses
  wxBoxSizer *sqlBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sqlBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxStaticBox *sqlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT
                                        ("FreeHand SELECT (WHERE and/or ORDER BY clauses)"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *sqlSizer = new wxStaticBoxSizer(sqlBox, wxVERTICAL);
  sqlBoxSizer->Add(sqlSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  FreeHandCtrl =
    new wxTextCtrl(this, ID_SQL_FC_FREE_HAND, Parent->GetFreeHand(),
                   wxDefaultPosition, wxSize(750, 150),
                   wxTE_MULTILINE | wxHSCROLL | wxTE_RICH);
  FreeHandCtrl->SetValue(Parent->GetFreeHand());
  sqlSizer->Add(FreeHandCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);

// adding the event handlers
  Connect(ID_SQL_FC_FREE_HAND, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) & SqlFiltersFreeHandPage::OnFreeHandChanged);
}

void SqlFiltersFreeHandPage::OnFreeHandChanged(wxCommandEvent & WXUNUSED(event))
{
//
// FreeHand SQL clauses changed
//
  Parent->ResetWizardClauses();
  Parent->SetFreeHand(FreeHandCtrl->GetValue());
  Parent->UpdateSqlSample();
  FreeHandCtrl->SetFocus();
}

void SqlFiltersFreeHandPage::UpdatePage()
{
//
// updating the FreeHand SQL page
//
  FreeHandCtrl->SetValue(Parent->GetFreeHand());
}

void CurrentSqlFilters::Initialize(wxString & db_prefix, wxString & table,
                                   bool read_only, wxString & geom_column)
{
//
// initializing
//
  Reset();
  Valid = true;
  DbPrefix = db_prefix;
  if (DbPrefix.Len() == 0)
    DbPrefix = wxT("MAIN");
  TableName = table;
  GeometryColumn = geom_column;
  ReadOnly = read_only;
}

void CurrentSqlFilters::Reset()
{
//
// resetting current SQL Filters
//
  Valid = false;
  DbPrefix = wxEmptyString;
  TableName = wxEmptyString;
  GeometryColumn = wxEmptyString;
  ReadOnly = true;
  Where1Enabled = false;
  Where2Enabled = false;
  Where3Enabled = false;
  AndOr12 = true;
  AndOr23 = true;
  Where1Column = wxEmptyString;
  Where2Column = wxEmptyString;
  Where3Column = wxEmptyString;
  Where1Operator = wxEmptyString;
  Where2Operator = wxEmptyString;
  Where3Operator = wxEmptyString;
  Where1Value = wxEmptyString;
  Where2Value = wxEmptyString;
  Where3Value = wxEmptyString;
  Order1Enabled = false;
  Order2Enabled = false;
  Order3Enabled = false;
  Order4Enabled = false;
  Order1Column = wxEmptyString;
  Order2Column = wxEmptyString;
  Order3Column = wxEmptyString;
  Order4Column = wxEmptyString;
  Order1Desc = false;
  Order2Desc = false;
  Order3Desc = false;
  Order4Desc = false;
  FreeHand = wxEmptyString;
}

void CurrentSqlFilters::Save(class SqlFiltersDialog * dlg)
{
//
// saving current SQL Filters from the Dialog
//
  Where1Enabled = dlg->IsWhere1Enabled();
  Where2Enabled = dlg->IsWhere2Enabled();
  Where3Enabled = dlg->IsWhere3Enabled();
  AndOr12 = dlg->IsAndOr12();
  AndOr23 = dlg->IsAndOr23();
  Where1Column = dlg->GetWhere1Column();
  Where2Column = dlg->GetWhere2Column();
  Where3Column = dlg->GetWhere3Column();
  Where1Operator = dlg->GetWhere1Operator();
  Where2Operator = dlg->GetWhere2Operator();
  Where3Operator = dlg->GetWhere3Operator();
  Where1Value = dlg->GetWhere1Value();
  Where2Value = dlg->GetWhere2Value();
  Where3Value = dlg->GetWhere3Value();
  Order1Enabled = dlg->IsOrder1Enabled();
  Order2Enabled = dlg->IsOrder2Enabled();
  Order3Enabled = dlg->IsOrder3Enabled();
  Order4Enabled = dlg->IsOrder4Enabled();
  Order1Column = dlg->GetOrder1Column();
  Order2Column = dlg->GetOrder2Column();
  Order3Column = dlg->GetOrder3Column();
  Order4Column = dlg->GetOrder4Column();
  Order1Desc = dlg->IsOrder1Desc();
  Order2Desc = dlg->IsOrder2Desc();
  Order3Desc = dlg->IsOrder3Desc();
  Order4Desc = dlg->IsOrder4Desc();
  FreeHand = dlg->GetFreeHand();
}
