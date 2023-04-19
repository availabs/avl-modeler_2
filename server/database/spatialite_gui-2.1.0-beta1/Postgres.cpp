/*
/ Postgres.cpp
/ helper functions for PostgreSQL connections
/
/ version 2.1, 2018 May 5
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2018  Alessandro Furieri
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

#include "wx/spinctrl.h"
#include "wx/utils.h"
#include "wx/filename.h"

void MyFrame::GetPQlibVersion(wxString & ver)
{
//
// returns the PQlib (PostgreSQL client) version
//
  int ver_num = vpgPQlibVersion();
  int major = ver_num / 10000;
  int minor = (ver_num - (major * 10000)) / 100;
  int revision = ver_num - (major * 10000) - (minor * 100);
  char version[128];
  sprintf(version, "%d.%02d.%02d", major, minor, revision);
  ver = wxString::FromUTF8(version);
}

bool MyFrame::HasPostgreSqlConnections()
{
// checks for any current PostgreSQL connection
  if (PostgresList.GetFirst() == NULL)
    return false;
  return true;
}

void MyFrame::DoLoadLibPQ(wxString & path)
{
//
// attempting to load LibPQ - dynamic late binding
//
  if (DynamicLibPQ.IsLoaded() == true)
    DynamicLibPQ.Unload();
  if (DynamicLibPQ.Load(path, wxDL_QUIET))
    {
      //  retievinig the pointers of ALL APIs
      bool valid = true;
      bool ok;
      VirtualPQapi.PQclear =
        (void (*)(PGresult *)) DynamicLibPQ.GetSymbol(wxT("PQclear"), &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQconnectdb =
        (PGconn *
         (*)(const char *conninfo)) DynamicLibPQ.GetSymbol(wxT("PQconnectdb"),
                                                           &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQerrorMessage =
        (char *(*)(const PGconn * conn))
        DynamicLibPQ.GetSymbol(wxT("PQerrorMessage"), &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQexec =
        (PGresult *
         (*)(PGconn * conn,
             const char *command)) DynamicLibPQ.GetSymbol(wxT("PQexec"), &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQfinish =
        (void (*)(PGconn * conn)) DynamicLibPQ.GetSymbol(wxT("PQfinish"), &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQgetisnull =
        (int (*)(const PGresult * res, int row_number, int column_number))
        DynamicLibPQ.GetSymbol(wxT("PQgetisnull"), &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQgetvalue =
        (char *(*)(const PGresult * res, int row_number, int column_number))
        DynamicLibPQ.GetSymbol(wxT("PQgetvalue"), &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQlibVersion =
        (int (*)(void)) DynamicLibPQ.GetSymbol(wxT("PQlibVersion"), &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQnfields =
        (int (*)(const PGresult * res)) DynamicLibPQ.GetSymbol(wxT("PQnfields"),
                                                               &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQntuples =
        (int (*)(const PGresult * res)) DynamicLibPQ.GetSymbol(wxT("PQntuples"),
                                                               &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQresultStatus =
        (ExecStatusType(*)(const PGresult * res))
        DynamicLibPQ.GetSymbol(wxT("PQresultStatus"), &ok);
      if (ok == false)
        valid = false;
      VirtualPQapi.PQstatus =
        (ConnStatusType(*)(const PGconn * conn))
        DynamicLibPQ.GetSymbol(wxT("PQstatus"), &ok);
      if (ok == false)
        valid = false;
      if (valid == true)
        {
          if (virtualpg_extension_init(SqliteHandle, &VirtualPQapi) ==
              SQLITE_OK)
            {
              // OK, anything is good
              PathLibPQ = path;
              VirtualPQapiOK = true;
              return;
            }
        }
    }
  PathLibPQ = wxT("");
}

void MyFrame::DoLocateLibPQ(wxString & extpath)
{
//
// attempting to load LibPQ - path selected by the user
//
  int ret;
  wxString path = wxDynamicLibrary::CanonicalizeName(wxT("libpq"));
  wxFileName name(path);
  wxString suffix =
    wxT("Dynamic Library (*.") + name.GetExt() + wxT(")|*.") + name.GetExt();
  wxFileDialog fileDialog(this, wxT("libPQ manual search"), wxT(""), path,
                          suffix, wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      DoLoadLibPQ(path);
      extpath = path;
    }
}

void MyFrame::DoPostgreSqlConnection()
{
//
// establishing a connection to PostgreSQL
//
  if (VirtualPQapiOK == false)
    {
      //
      // LibPQ isn't yet loaded
      //
      int ret = wxMessageBox(wxT
                             ("The PostgreSQL client library (LibPQ) isn't yet loaded.\n\n"
                              "Do you wish to attempt loading it right now ?"),
                             wxT("spatialite_gui"),
                             wxYES_NO | wxICON_QUESTION, this);
      if (ret == wxYES)
        {
          // trying to load LibPQ
          wxString path;
          DoLocateLibPQ(path);
          if (VirtualPQapiOK == true)
            {
              wxMessageBox(wxT
                           ("The PostgreSQL client library (LibPQ) has been correctly loaded\n\n"
                            "Full PostgreSQL support is now available."),
                           wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                           this);
              goto ok;
            }
          if (path.Len() > 0)
            wxMessageBox(wxT
                         ("Unable to load a valid PostgreSQL client library (LibPQ) from:\n\n")
                         + path +
                         wxT
                         ("\n\nFull PostgreSQL support still continues to be disabled."),
                         wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
        }
      return;
    }

ok:
  PostgresConnectionDialog dlg;
  dlg.Create(this);
  int ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      char *conninfo = sqlite3_mprintf(" ");
      char *prev;
      if (dlg.GetHost() != NULL)
        {
          prev = conninfo;
          conninfo = sqlite3_mprintf("%s host=%s", prev, dlg.GetHost());
          sqlite3_free(prev);
        }
      if (dlg.GetHostAddr() != NULL)
        {
          prev = conninfo;
          conninfo = sqlite3_mprintf("%s hostaddr=%s", prev, dlg.GetHostAddr());
          sqlite3_free(prev);
        }
      prev = conninfo;
      conninfo = sqlite3_mprintf("%s port=%d", prev, dlg.GetPort());
      sqlite3_free(prev);
      if (dlg.GetDbName() != NULL)
        {
          prev = conninfo;
          conninfo = sqlite3_mprintf("%s dbname=%s", prev, dlg.GetDbName());
          sqlite3_free(prev);
        }
      if (dlg.GetUser() != NULL)
        {
          prev = conninfo;
          conninfo = sqlite3_mprintf("%s user=%s", prev, dlg.GetUser());
          sqlite3_free(prev);
        }
      if (dlg.GetPassword() != NULL)
        {
          prev = conninfo;
          conninfo = sqlite3_mprintf("%s password=%s", prev, dlg.GetPassword());
          sqlite3_free(prev);
        }
      wxString host = wxString::FromUTF8(dlg.GetHost());
      wxString hostaddr = wxString::FromUTF8(dlg.GetHostAddr());
      int port = dlg.GetPort();
      wxString dbname = wxString::FromUTF8(dlg.GetDbName());
      wxString user = wxString::FromUTF8(dlg.GetUser());
      bool readOnly = dlg.IsReadOnly();
      bool textDates = dlg.IsTextDates();
      bool ret = false;
      if (DoCheckPostgres(host, hostaddr, port, dbname, user) == true)
        wxMessageBox(wxT
                     ("A PostgreSQL Connection with the same arguments alreay exists !!!"),
                     wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      else
        ret =
          DoInitPostgres(host, hostaddr, dlg.GetPort(), dbname, user, readOnly,
                         textDates, conninfo + 2);
      sqlite3_free(conninfo);
      if (ret)
        InitTableTree();
    }
}

bool MyFrame::DoCheckPostgres(wxString & host, wxString & hostaddr, int port,
                              wxString & dbname, wxString & user)
{
//
// testing for already existing PostgreSQL connections
//
  MyPostgresConn *pC = PostgresList.GetFirst();
  while (pC != NULL)
    {
      if (pC->GetHost().Cmp(host) == 0 && pC->GetHostAddr().Cmp(hostaddr) == 0
          && pC->GetPort() == port && pC->GetDbName().Cmp(dbname) == 0
          && pC->GetUser().Cmp(user) == 0)
        return true;
      pC = pC->GetNext();
    }
  return false;
}

void MyFrame::DoInitVirtualPQapi()
{
//
// initializing the Virtual LibPQ API
//
#ifdef LIBPQ_DEFERRED
/*
/ not directly linked to libpq
/ libpq will be eventually loaded only when required (late binding)
*/
  VirtualPQapi.PQclear = NULL;
  VirtualPQapi.PQconnectdb = NULL;
  VirtualPQapi.PQerrorMessage = NULL;
  VirtualPQapi.PQexec = NULL;
  VirtualPQapi.PQfinish = NULL;
  VirtualPQapi.PQgetisnull = NULL;
  VirtualPQapi.PQgetvalue = NULL;
  VirtualPQapi.PQlibVersion = NULL;
  VirtualPQapi.PQnfields = NULL;
  VirtualPQapi.PQntuples = NULL;
  VirtualPQapi.PQresultStatus = NULL;
  VirtualPQapi.PQstatus = NULL;
  VirtualPQapiOK = false;
#else
/* directly linked to libpq */
  VirtualPQapi.PQclear = PQclear;
  VirtualPQapi.PQconnectdb = PQconnectdb;
  VirtualPQapi.PQerrorMessage = PQerrorMessage;
  VirtualPQapi.PQexec = PQexec;
  VirtualPQapi.PQfinish = PQfinish;
  VirtualPQapi.PQgetisnull = PQgetisnull;
  VirtualPQapi.PQgetvalue = PQgetvalue;
  VirtualPQapi.PQlibVersion = PQlibVersion;
  VirtualPQapi.PQnfields = PQnfields;
  VirtualPQapi.PQntuples = PQntuples;
  VirtualPQapi.PQresultStatus = PQresultStatus;
  VirtualPQapi.PQstatus = PQstatus;
  VirtualPQapiOK = true;
#endif
}

void MyFrame::DoInitVirtualPG()
{
//
// attempting to initialize the VirtalPG extension 
//
  if (virtualpg_extension_init(SqliteHandle, &VirtualPQapi) == SQLITE_OK)
    VirtualPQapiOK = true;
}

bool MyFrame::DoInitPostgres(wxString & host, wxString & hostaddr, int port,
                             wxString & dbname, wxString & user, bool readOnly,
                             bool textDates, const char *conninfo)
{
//
// attempting to initialize a PostgreSQL connection
//
  PGconn *pg_conn;
  PGresult *res;
  int nRows;
  int nFields;
  const char *obj;
  const char *schema_name;
  const char *table_name;
  const char *geometry;
  const char *type;
  int srid;
  int dims;
  int r;
  MyPostgresConn *conn;
  MyPostgresSchema *pS;

  ::wxBeginBusyCursor();
  pg_conn = vpgPQconnectdb(conninfo);
  if (vpgPQstatus(pg_conn) != CONNECTION_OK)
    {
      char *msg = sqlite3_mprintf("Connection to Postgres failed: %s",
                                  vpgPQerrorMessage(pg_conn));
      wxMessageBox(wxString::FromUTF8(msg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(msg);
      vpgPQfinish(pg_conn);
      ::wxEndBusyCursor();
      return false;
    }

  conn =
    PostgresList.Insert(host, hostaddr, port, dbname, user, readOnly,
                        textDates);

// retrieving all PosgreSQL Tables and Views
  const char *sql =
    "SELECT 'table' AS obj, t.schemaname, t.tablename, g.f_geometry_column, "
    "g.type, g.srid, g.coord_dimension "
    "FROM pg_tables AS t LEFT JOIN geometry_columns AS g ON "
    "(g.f_table_schema = t.schemaname AND g.f_table_name = t.tablename) "
    "WHERE t.schemaname NOT IN ('information_schema', 'pg_catalog') "
    "UNION SELECT 'view' AS obj, schemaname, viewname, '', '', -1, -1 FROM pg_views "
    "WHERE schemaname NOT IN ('information_schema', 'pg_catalog') "
    "ORDER BY 1, 2, 3";
  res = vpgPQexec(pg_conn, sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto err;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 7)
    {
      for (r = 0; r < nRows; r++)
        {
          obj = vpgPQgetvalue(res, r, 0);
          schema_name = vpgPQgetvalue(res, r, 1);
          table_name = vpgPQgetvalue(res, r, 2);
          geometry = vpgPQgetvalue(res, r, 3);
          type = vpgPQgetvalue(res, r, 4);
          srid = atoi(vpgPQgetvalue(res, r, 5));
          dims = atoi(vpgPQgetvalue(res, r, 6));
          wxString Schema = wxString::FromUTF8(schema_name);
          wxString Table = wxString::FromUTF8(table_name);
          wxString Geometry = wxString::FromUTF8(geometry);
          wxString Type = wxString::FromUTF8(type);
          if (strcmp(obj, "view") == 0)
            conn->Add(Schema, Table);
          else
            conn->Add(Schema, Table, Geometry, Type, srid, dims);
        }
    }
  vpgPQclear(res);

  pS = conn->GetFirst();
  while (pS != NULL)
    {
      MyPostgresTable *pT = pS->GetFirstTable();
      while (pT != NULL)
        {
          InitPostgresPks(pg_conn, pS, pT);
          DoCheckGrantPermissions(pg_conn, conn->GetUser(), pS, pT);
          pT = pT->GetNext();
        }
      MyPostgresView *pV = pS->GetFirstView();
      while (pV != NULL)
        {
          DoCheckGrantPermissions(pg_conn, conn->GetUser(), pS, pV);
          pV = pV->GetNext();
        }
      pS = pS->GetNext();
    }
  vpgPQfinish(pg_conn);

  conn->SetConnectionString(conninfo);
  DoSetUniqueVirtNames();
  DoCreatePostgresTables();
  ::wxEndBusyCursor();
  return true;

err:
// something has gone the wrong way
  if (res != NULL)
    vpgPQclear(res);
  vpgPQfinish(pg_conn);
  ::wxEndBusyCursor();
  return false;
}

void MyFrame::DoSetUniqueVirtNames()
{
//
// ensuring to set Unique Names to all PostgreSQL Tables and Views
//
  wxString baseName;
  wxString uniqueName;
  MyPostgresConn *pC = PostgresList.GetFirst();
  while (pC != NULL)
    {
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          MyPostgresTable *pT = pS->GetFirstTable();
          while (pT != NULL)
            {
              if (pT->GetVirtName().Len() == 0)
                {
                  baseName = wxT("vpg_") + pT->GetName();
                  PostgresList.MakeUniqueVirtName(baseName, uniqueName);
                  pT->SetVirtName(uniqueName);
                }
              if (pT->GetFirst() != NULL)
                {
                  if (pT->GetPostGisName().Len() == 0)
                    {
                      baseName = wxT("vpg_") + pT->GetName() + wxT("_postgis");
                      PostgresList.MakeUniqueVirtName(baseName, uniqueName);
                      pT->SetPostGisName(uniqueName);
                    }
                }
              pT = pT->GetNext();
            }
          MyPostgresView *pV = pS->GetFirstView();
          while (pV != NULL)
            {
              if (pV->GetVirtName().Len() == 0)
                {
                  baseName = wxT("vpg_") + pV->GetName();
                  PostgresList.MakeUniqueVirtName(baseName, uniqueName);
                  pV->SetVirtName(uniqueName);
                }
              pV = pV->GetNext();
            }
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
}

void MyFrame::DoCreatePostgresTables()
{
//
// attempting to create any required PostgreSQL/PostGIS table
//
  char *sql;
  char *virt_name;
  char *xvirt_name;
  char *geom_name;
  char *geo_type;
  char *connection_string;
  char *schema;
  char *table;
  int ret;
  char *errMsg = NULL;
  MyPostgresConn *pC = PostgresList.GetFirst();
  while (pC != NULL)
    {
      const char *readOnly = "-";
      const char *textDates = "-";
      if (pC->IsReadOnly() == false)
        readOnly = "W";
      if (pC->IsTextDates() == false)
        textDates = "J";
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          MyPostgresTable *pT = pS->GetFirstTable();
          while (pT != NULL)
            {
              // creating some VirtualPG Table
              virt_name = (char *) malloc((pT->GetVirtName().Len() * 4) + 1);
              strcpy(virt_name, pT->GetVirtName().ToUTF8());
              xvirt_name = gaiaDoubleQuotedSql(virt_name);
              free(virt_name);
              connection_string =
                (char *) malloc((pC->GetConnectionString().Len() * 4) + 1);
              strcpy(connection_string, pC->GetConnectionString().ToUTF8());
              schema = (char *) malloc((pS->GetName().Len() * 4) + 1);
              strcpy(schema, pS->GetName().ToUTF8());
              table = (char *) malloc((pT->GetName().Len() * 4) + 1);
              strcpy(table, pT->GetName().ToUTF8());
              sql =
                sqlite3_mprintf
                ("CREATE VIRTUAL TABLE IF NOT EXISTS main.\"%s\" "
                 "USING VirtualPostgres(%Q, %Q, %Q, %Q, %Q)", xvirt_name,
                 connection_string, schema, table, readOnly, textDates);
              free(xvirt_name);
              free(connection_string);
              free(schema);
              free(table);
              ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
              sqlite3_free(sql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("PostgreSQL wrapper: ") +
                               wxString::FromUTF8(errMsg),
                               wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                               this);
                  sqlite3_free(errMsg);
                }
              if (pT->GetFirst() != NULL)
                {
                  // creating some SpatiaLite-like Spatial View based on a PostgreSQL Table
                  virt_name =
                    (char *) malloc((pT->GetPostGisName().Len() * 4) + 1);
                  strcpy(virt_name, pT->GetPostGisName().ToUTF8());
                  xvirt_name = gaiaDoubleQuotedSql(virt_name);
                  free(virt_name);
                  sql =
                    sqlite3_mprintf
                    ("CREATE VIEW IF NOT EXISTS main.\"%s\" AS\n", xvirt_name);
                  free(xvirt_name);
                  sql = DoCreatePostGisSpatialView(pT, sql);
                  ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
                  sqlite3_free(sql);
                  if (ret != SQLITE_OK)
                    {
                      wxMessageBox(wxT("PostGIS wrapper: ") +
                                   wxString::FromUTF8(errMsg),
                                   wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                                   this);
                      sqlite3_free(errMsg);
                    }
                  char a[1024];
                  char b[1024];
                  strcpy(a, pS->GetName().ToUTF8());
                  strcpy(b, pT->GetPostGisName().ToUTF8());
                  if (pC->IsReadOnly() == false && pT->HasPK() == true
                      && pT->CanInsertUpdateDelete() == true)
                    {
                      // creating all required Triggers so to get a Writable View
                      DoCreatePostGisSpatialViewTriggers(pC, pS, pT);
                    }
                  // registering all Spatial Views
                  MyPostGisGeometry *pG = pT->GetFirst();
                  while (pG != NULL)
                    {
                      // registering a Spatial View
                      virt_name =
                        (char *) malloc((pT->GetPostGisName().Len() * 4) + 1);
                      strcpy(virt_name, pT->GetPostGisName().ToUTF8());
                      geom_name =
                        (char *) malloc((pG->GetName().Len() * 4) + 1);
                      strcpy(geom_name, pG->GetName().ToUTF8());
                      geo_type =
                        (char *) malloc((pG->GetGeomType().Len() * 4) + 1);
                      strcpy(geo_type, pG->GetGeomType().ToUTF8());
                      sql =
                        sqlite3_mprintf
                        ("INSERT INTO temp.postgis_geometry_columns "
                         "(f_table_name, f_geometry_column, coord_dimension, srid, geometry_type) "
                         "VALUES (%Q, %Q, %d, %d, %Q)", virt_name, geom_name,
                         pG->GetDims(), pG->GetSrid(), geo_type);
                      free(virt_name);
                      free(geom_name);
                      free(geo_type);
                      ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
                      sqlite3_free(sql);
                      if (ret != SQLITE_OK)
                        {
                          wxMessageBox(wxT("PostgreSQL wrapper: ") +
                                       wxString::FromUTF8(errMsg),
                                       wxT("spatialite_gui"),
                                       wxOK | wxICON_ERROR, this);
                          sqlite3_free(errMsg);
                        }
                      pG = pG->GetNext();
                    }
                }
              pT = pT->GetNext();
            }
          MyPostgresView *pV = pS->GetFirstView();
          while (pV != NULL)
            {
              // creating some VirtualPG View
              virt_name = (char *) malloc((pV->GetVirtName().Len() * 4) + 1);
              strcpy(virt_name, pV->GetVirtName().ToUTF8());
              xvirt_name = gaiaDoubleQuotedSql(virt_name);
              free(virt_name);
              connection_string =
                (char *) malloc((pC->GetConnectionString().Len() * 4) + 1);
              strcpy(connection_string, pC->GetConnectionString().ToUTF8());
              schema = (char *) malloc((pS->GetName().Len() * 4) + 1);
              strcpy(schema, pS->GetName().ToUTF8());
              table = (char *) malloc((pV->GetName().Len() * 4) + 1);
              strcpy(table, pV->GetName().ToUTF8());
              sql =
                sqlite3_mprintf
                ("CREATE VIRTUAL TABLE IF NOT EXISTS main.\"%s\" "
                 "USING VirtualPostgres(%Q, %Q, %Q, '-', textDates)",
                 xvirt_name, connection_string, schema, table, textDates);
              free(xvirt_name);
              free(connection_string);
              free(schema);
              free(table);
              ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
              sqlite3_free(sql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("PostgreSQL wrapper: ") +
                               wxString::FromUTF8(errMsg),
                               wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                               this);
                  sqlite3_free(errMsg);
                }
              pV = pV->GetNext();
            }
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
}

char *MyFrame::DoCreatePostGisSpatialView(MyPostgresTable * table,
                                          char *prev_sql)
{
//
// auto-writing the CREATE VIEW for some PostGIS geometry
//
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  char *sql = prev_sql;
  char *prev;
  char *xtable = (char *) malloc((table->GetVirtName().Len() * 4) + 1);
  strcpy(xtable, table->GetVirtName().ToUTF8());
  char *sql2 = sqlite3_mprintf("PRAGMA table_info(%Q)", xtable);
  free(xtable);
  ret = sqlite3_get_table(SqliteHandle, sql2, &results, &rows, &columns, NULL);
  sqlite3_free(sql2);
  if (ret != SQLITE_OK)
    return sql;
  if (rows < 1)
    ;
  else
    {
      prev = sql;
      sql = sqlite3_mprintf("%sSELECT ROWID AS ROWID", prev);
      sqlite3_free(prev);
      for (i = 1; i <= rows; i++)
        {
          prev = sql;
          const char *name = results[(i * columns) + 1];
          char *xname = gaiaDoubleQuotedSql(name);
          wxString col = wxString::FromUTF8(name);
          MyPostGisGeometry *geom = table->Find(col);
          if (geom != NULL)
            {
              if (geom->IsMultiType() == true)
                sql =
                  sqlite3_mprintf
                  ("%s, CastToMulti(GeomFromEWKB(\"%s\")) AS \"%s\"", prev,
                   xname, xname);
              else
                sql =
                  sqlite3_mprintf("%s, GeomFromEWKB(\"%s\") AS \"%s\"",
                                  prev, xname, xname);
          } else
            {
              sql = sqlite3_mprintf("%s, \"%s\" AS \"%s\"", prev, xname, xname);
            }
          free(xname);
          sqlite3_free(prev);
        }
    }
  sqlite3_free_table(results);

  prev = sql;
  xtable = (char *) malloc((table->GetVirtName().Len() * 4) + 1);
  strcpy(xtable, table->GetVirtName().ToUTF8());
  char *xxtable = gaiaDoubleQuotedSql(xtable);
  free(xtable);
  sql = sqlite3_mprintf("%s\nFROM MAIN.\"%s\"", prev, xxtable);
  free(xxtable);
  sqlite3_free(prev);
  return sql;
}

void MyFrame::DoCheckGrantPermissions(void *conn, wxString & user,
                                      MyPostgresSchema * schema,
                                      MyPostgresTable * table)
{
//
// testing Grant permissions for some PostgreSQL Table or View
//
  PGconn *pg_conn = (PGconn *) conn;
  PGresult *res;
  int nRows;
  int nFields;
  int r;
  bool canSelect = false;
  bool canInsert = false;
  bool canUpdate = false;
  bool canDelete = false;
  bool canInsertUpdateDelete = false;
  char *cuser = (char *) malloc(user.Len() * 4 + 1);
  strcpy(cuser, user.ToUTF8());
  char *cschema = (char *) malloc(schema->GetName().Len() * 4 + 1);
  strcpy(cschema, schema->GetName().ToUTF8());
  char *ctable = (char *) malloc(table->GetName().Len() * 4 + 1);
  strcpy(ctable, table->GetName().ToUTF8());
  char *name = sqlite3_mprintf("%s.%s", cschema, ctable);
// testing for SELECT
  char *sql =
    sqlite3_mprintf("SELECT has_table_privilege(%Q, %Q, 'select')", cuser,
                    name);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto end;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *permission = vpgPQgetvalue(res, r, 0);
          if (strcmp(permission, "t") == 0)
            canSelect = true;
        }
    }
  vpgPQclear(res);
  res = NULL;
// testing for INSERT
  sql =
    sqlite3_mprintf("SELECT has_table_privilege(%Q, %Q, 'insert')", cuser,
                    name);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto end;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *permission = vpgPQgetvalue(res, r, 0);
          if (strcmp(permission, "t") == 0)
            canInsert = true;
        }
    }
  vpgPQclear(res);
  res = NULL;
// testing for UPDATE
  sql =
    sqlite3_mprintf("SELECT has_table_privilege(%Q, %Q, 'update')", cuser,
                    name);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto end;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *permission = vpgPQgetvalue(res, r, 0);
          if (strcmp(permission, "t") == 0)
            canUpdate = true;
        }
    }
  vpgPQclear(res);
  res = NULL;
// testing for DELETE
  sql =
    sqlite3_mprintf("SELECT has_table_privilege(%Q, %Q, 'delete')", cuser,
                    name);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto end;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *permission = vpgPQgetvalue(res, r, 0);
          if (strcmp(permission, "t") == 0)
            canDelete = true;
        }
    }
  vpgPQclear(res);
  res = NULL;

  if (canInsert == true && canUpdate == true && canDelete == true)
    canInsertUpdateDelete = true;
  table->SetGrants(canSelect, canInsertUpdateDelete);
end:
  if (res != NULL)
    vpgPQclear(res);
  free(cuser);
  free(cschema);
  free(ctable);
  sqlite3_free(name);
}

void MyFrame::DoCheckGrantPermissions(void *conn, wxString & user,
                                      MyPostgresSchema * schema,
                                      MyPostgresView * view)
{
//
// testing Grant permissions for some PostgreSQL Table or View
//
  PGconn *pg_conn = (PGconn *) conn;
  PGresult *res;
  int nRows;
  int nFields;
  int r;
  bool canSelect = false;
  bool canInsert = false;
  bool canUpdate = false;
  bool canDelete = false;
  bool canInsertUpdateDelete = false;
  char *cuser = (char *) malloc(user.Len() * 4 + 1);
  strcpy(cuser, user.ToUTF8());
  char *cschema = (char *) malloc(schema->GetName().Len() * 4 + 1);
  strcpy(cschema, schema->GetName().ToUTF8());
  char *ctable = (char *) malloc(view->GetName().Len() * 4 + 1);
  strcpy(ctable, view->GetName().ToUTF8());
  char *name = sqlite3_mprintf("%s.%s", cschema, ctable);
// testing for SELECT
  char *sql =
    sqlite3_mprintf("SELECT has_table_privilege(%Q, %Q, 'select')", cuser,
                    name);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto end;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *permission = vpgPQgetvalue(res, r, 0);
          if (strcmp(permission, "t") == 0)
            canSelect = true;
        }
    }
  vpgPQclear(res);
  res = NULL;
// testing for INSERT
  sql =
    sqlite3_mprintf("SELECT has_table_privilege(%Q, %Q, 'insert')", cuser,
                    name);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto end;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *permission = vpgPQgetvalue(res, r, 0);
          if (strcmp(permission, "t") == 0)
            canInsert = true;
        }
    }
  vpgPQclear(res);
  res = NULL;
// testing for UPDATE
  sql =
    sqlite3_mprintf("SELECT has_table_privilege(%Q, %Q, 'update')", cuser,
                    name);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto end;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *permission = vpgPQgetvalue(res, r, 0);
          if (strcmp(permission, "t") == 0)
            canUpdate = true;
        }
    }
  vpgPQclear(res);
  res = NULL;
// testing for DELETE
  sql =
    sqlite3_mprintf("SELECT has_table_privilege(%Q, %Q, 'delete')", cuser,
                    name);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto end;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *permission = vpgPQgetvalue(res, r, 0);
          if (strcmp(permission, "t") == 0)
            canDelete = true;
        }
    }
  vpgPQclear(res);
  res = NULL;

  if (canInsert == true && canUpdate == true && canDelete == true)
    canInsertUpdateDelete = true;
  view->SetGrants(canSelect, canInsertUpdateDelete);
end:
  if (res != NULL)
    vpgPQclear(res);
  free(cuser);
  free(cschema);
  free(ctable);
  sqlite3_free(name);
}

void MyFrame::DoCreatePostGisSpatialViewTriggers(MyPostgresConn * conn,
                                                 MyPostgresSchema * schema,
                                                 MyPostgresTable * table)
{
//
// auto-writing the CREATE TRIGGERS for some PostGIS geometry (writable view)
//
  char *sql;
  char *prev;
  char *ctrigger;
  char *xtrigger;
  char *ctable;
  char *xtable;
  char *cview;
  char *xview;
  char name[1024];
  char *xname;
  char *where;
  int first;
  MyPostgresCol *pC;
  MyPostgresColumns *cols;
  char *errMsg = NULL;
  int ret;

  InitPostgresPkColumns(conn, schema, table);
  cols = DoGetPostgresColumns(table);

// preparing the INSERT Trigger
  ctable = (char *) malloc(table->GetVirtName().Len() * 4 + 1);
  strcpy(ctable, table->GetVirtName().ToUTF8());
  xtable = gaiaDoubleQuotedSql(ctable);
  free(ctable);
  cview = (char *) malloc(table->GetPostGisName().Len() * 4 + 1);
  strcpy(cview, table->GetPostGisName().ToUTF8());
  xview = gaiaDoubleQuotedSql(cview);
  ctrigger = sqlite3_mprintf("%s_trgins", cview);
  free(cview);
  xtrigger = gaiaDoubleQuotedSql(ctrigger);
  sqlite3_free(ctrigger);
  sql = sqlite3_mprintf("CREATE TRIGGER \"%s\"\n\tINSTEAD OF "
                        "INSERT ON \"%s\"\nBEGIN\n\tINSERT OR REPLACE INTO \"%s\"\n\t\t(",
                        xtrigger, xview, xtable);
  free(xtrigger);
  free(xtable);
  free(xview);
  pC = cols->GetFirst();
  first = 1;
  while (pC != NULL)
    {
      prev = sql;
      if (first)
        {
          first = 0;
          strcpy(name, pC->GetName().ToUTF8());
          xname = gaiaDoubleQuotedSql(name);
          sql = sqlite3_mprintf("%s\"%s\"", prev, xname);
          free(xname);
      } else
        {
          strcpy(name, pC->GetName().ToUTF8());
          xname = gaiaDoubleQuotedSql(name);
          sql = sqlite3_mprintf("%s, \"%s\"", prev, xname);
          free(xname);
        }
      sqlite3_free(prev);
      pC = pC->GetNext();
    }
  prev = sql;
  sql = sqlite3_mprintf("%s)\n\tVALUES (", prev);
  sqlite3_free(prev);
  pC = cols->GetFirst();
  first = 1;
  while (pC != NULL)
    {
      strcpy(name, pC->GetName().ToUTF8());
      xname = gaiaDoubleQuotedSql(name);
      MyPostGisGeometry *geom = table->Find(pC->GetName());
      prev = sql;
      if (first)
        {
          first = 0;
          if (geom != NULL)
            sql = sqlite3_mprintf("%sAsEWKB(NEW.\"%s\")", prev, xname);
          else
            sql = sqlite3_mprintf("%sNEW.\"%s\"", prev, xname);
      } else
        {
          if (geom != NULL)
            sql = sqlite3_mprintf("%s, AsEWKB(NEW.\"%s\")", prev, xname);
          else
            sql = sqlite3_mprintf("%s, NEW.\"%s\"", prev, xname);
        }
      free(xname);
      sqlite3_free(prev);
      pC = pC->GetNext();
    }
  prev = sql;
  sql = sqlite3_mprintf("%s);\nEND", prev);
  sqlite3_free(prev);
  ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CREATE TRIGGER error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
    }
// preparing the UPDATE Trigger
  ctable = (char *) malloc(table->GetVirtName().Len() * 4 + 1);
  strcpy(ctable, table->GetVirtName().ToUTF8());
  xtable = gaiaDoubleQuotedSql(ctable);
  free(ctable);
  cview = (char *) malloc(table->GetPostGisName().Len() * 4 + 1);
  strcpy(cview, table->GetPostGisName().ToUTF8());
  xview = gaiaDoubleQuotedSql(cview);
  ctrigger = sqlite3_mprintf("%s_trgupd", cview);
  free(cview);
  xtrigger = gaiaDoubleQuotedSql(ctrigger);
  sqlite3_free(ctrigger);
  sql = sqlite3_mprintf("CREATE TRIGGER \"%s\"\n\tINSTEAD OF "
                        "UPDATE OF ", xtrigger);
  free(xtrigger);
  pC = cols->GetFirst();
  first = 1;
  while (pC != NULL)
    {
      strcpy(name, pC->GetName().ToUTF8());
      xname = gaiaDoubleQuotedSql(name);
      prev = sql;
      if (first)
        {
          first = 0;
          sql = sqlite3_mprintf("%s\"%s\"", prev, xname);
      } else
        sql = sqlite3_mprintf("%s, \"%s\"", prev, xname);
      free(xname);
      sqlite3_free(prev);
      pC = pC->GetNext();
    }
  prev = sql;
  sql =
    sqlite3_mprintf("%s\n\tON \"%s\"\nBEGIN\n\tUPDATE \"%s\" SET ", prev, xview,
                    xtable);
  sqlite3_free(prev);
  free(xtable);
  free(xview);
  pC = cols->GetFirst();
  first = 1;
  while (pC != NULL)
    {
      strcpy(name, pC->GetName().ToUTF8());
      xname = gaiaDoubleQuotedSql(name);
      MyPostGisGeometry *geom = table->Find(pC->GetName());
      prev = sql;
      if (first)
        {
          first = 0;
          if (geom != NULL)
            sql =
              sqlite3_mprintf("%s\"%s\" = AsEWKB(NEW.\"%s\")", prev, xname,
                              xname);
          else
            sql = sqlite3_mprintf("%s\"%s\" = NEW.\"%s\"", prev, xname, xname);
      } else
        {
          if (geom != NULL)
            sql =
              sqlite3_mprintf("%s, \"%s\" = AsEWKB(NEW.\"%s\")", prev, xname,
                              xname);
          else
            sql =
              sqlite3_mprintf("%s, \"%s\" = NEW.\"%s\"", prev, xname, xname);
        }
      free(xname);
      sqlite3_free(prev);
      pC = pC->GetNext();
    }
  prev = sql;
  where = cols->BuildWhere();
  sql = sqlite3_mprintf("%s\n\t%s;\nEND", prev, where);
  sqlite3_free(prev);
  sqlite3_free(where);
  ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CREATE TRIGGER error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
    }
// preparing the DELETE Trigger
  ctable = (char *) malloc(table->GetVirtName().Len() * 4 + 1);
  strcpy(ctable, table->GetVirtName().ToUTF8());
  xtable = gaiaDoubleQuotedSql(ctable);
  free(ctable);
  cview = (char *) malloc(table->GetPostGisName().Len() * 4 + 1);
  strcpy(cview, table->GetPostGisName().ToUTF8());
  xview = gaiaDoubleQuotedSql(cview);
  ctrigger = sqlite3_mprintf("%s_trgdel", cview);
  free(cview);
  xtrigger = gaiaDoubleQuotedSql(ctrigger);
  sqlite3_free(ctrigger);
  where = cols->BuildWhere();
  sql = sqlite3_mprintf("CREATE TRIGGER \"%s\"\n\tINSTEAD OF "
                        "DELETE ON \"%s\"\nBEGIN\n\tDELETE FROM \"%s\" %s;\nEND",
                        xtrigger, xview, xtable, where);
  sqlite3_free(where);
  free(xtrigger);
  free(xtable);
  free(xview);
  ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CREATE TRIGGER error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
    }

  delete cols;
}

MyPostgresColumns *MyFrame::DoGetPostgresColumns(MyPostgresTable * table)
{
//
// fetching all Columns for a given VirtualPG Table
//  
  MyPostgresColumns *cols = NULL;
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  char *xtable = (char *) malloc((table->GetVirtName().Len() * 4) + 1);
  strcpy(xtable, table->GetVirtName().ToUTF8());
  char *sql = sqlite3_mprintf("PRAGMA table_info(%Q)", xtable);
  free(xtable);
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return NULL;
  if (rows < 1)
    ;
  else
    {
      cols = new MyPostgresColumns();
      for (i = 1; i <= rows; i++)
        {
          const char *name = results[(i * columns) + 1];
          wxString sName = wxString::FromUTF8(name);
          cols->Add(sName);
        }
    }
  sqlite3_free_table(results);

  MyPostgresPK *pK = table->GetFirstPK();
  while (pK != NULL)
    {
      // marking all PK Columns
      cols->SetPK(pK->GetName());
      pK = pK->GetNext();
    }

  if (cols->GetFirst() == NULL)
    {
      // empyt list !!!
      delete cols;
      return NULL;
    }
  return cols;
}

void MyFrame::DoDropPostgresTables()
{
//
// attempting to drop any PostgreSQL/PostGIS table
//
  char *sql;
  char *virt_name;
  char *xvirt_name;
  char *geom_name;
  int ret;
  char *errMsg = NULL;
  MyPostgresConn *pC = PostgresList.GetFirst();
  while (pC != NULL)
    {
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          MyPostgresTable *pT = pS->GetFirstTable();
          while (pT != NULL)
            {
              if (pT->GetFirst() != NULL)
                {
                  // dropping some SpatiaLite-like Spatial View based on a PostgreSQL Table
                  virt_name =
                    (char *) malloc((pT->GetPostGisName().Len() * 4) + 1);
                  strcpy(virt_name, pT->GetPostGisName().ToUTF8());
                  xvirt_name = gaiaDoubleQuotedSql(virt_name);
                  free(virt_name);
                  sql =
                    sqlite3_mprintf("DROP VIEW IF EXISTS main.\"%s\"",
                                    xvirt_name);
                  free(xvirt_name);
                  ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
                  sqlite3_free(sql);
                  if (ret != SQLITE_OK)
                    {
                      wxMessageBox(wxT("PostGIS wrapper: ") +
                                   wxString::FromUTF8(errMsg),
                                   wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                                   this);
                      sqlite3_free(errMsg);
                    }
                  MyPostGisGeometry *pG = pT->GetFirst();
                  while (pG != NULL)
                    {
                      // removing from POSTGIS_GEOMETRY_COLUMNS
                      virt_name =
                        (char *) malloc((pT->GetPostGisName().Len() * 4) + 1);
                      strcpy(virt_name, pT->GetPostGisName().ToUTF8());
                      geom_name =
                        (char *) malloc((pG->GetName().Len() * 4) + 1);
                      strcpy(geom_name, pG->GetName().ToUTF8());
                      sql =
                        sqlite3_mprintf
                        ("DELETE FROM temp.postgis_geometry_columns "
                         "WHERE f_table_name = %Q AND f_geometry_column = %Q",
                         virt_name, geom_name);
                      free(virt_name);
                      free(geom_name);
                      ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
                      sqlite3_free(sql);
                      if (ret != SQLITE_OK)
                        {
                          wxMessageBox(wxT("PostGIS wrapper: ") +
                                       wxString::FromUTF8(errMsg),
                                       wxT("spatialite_gui"),
                                       wxOK | wxICON_ERROR, this);
                          sqlite3_free(errMsg);
                        }
                      pG = pG->GetNext();
                    }
                }
              // dropping some VirtualPG Table
              virt_name = (char *) malloc((pT->GetVirtName().Len() * 4) + 1);
              strcpy(virt_name, pT->GetVirtName().ToUTF8());
              xvirt_name = gaiaDoubleQuotedSql(virt_name);
              free(virt_name);
              sql =
                sqlite3_mprintf("DROP TABLE IF EXISTS main.\"%s\"", xvirt_name);
              free(xvirt_name);
              ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
              sqlite3_free(sql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("PostgreSQL wrapper: ") +
                               wxString::FromUTF8(errMsg),
                               wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                               this);
                  sqlite3_free(errMsg);
                }
              pT = pT->GetNext();
            }
          MyPostgresView *pV = pS->GetFirstView();
          while (pV != NULL)
            {
              // dropping some VirtualPG View
              virt_name = (char *) malloc((pV->GetVirtName().Len() * 4) + 1);
              strcpy(virt_name, pV->GetVirtName().ToUTF8());
              xvirt_name = gaiaDoubleQuotedSql(virt_name);
              free(virt_name);
              sql =
                sqlite3_mprintf("DROP TABLE IF EXISTS main.\"%s\"", xvirt_name);
              free(xvirt_name);
              ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
              sqlite3_free(sql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("PostgreSQL wrapper: ") +
                               wxString::FromUTF8(errMsg),
                               wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                               this);
                  sqlite3_free(errMsg);
                }
              pV = pV->GetNext();
            }
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
}

bool MyFrame::DoClosePostgreSqlConn(wxString & host, wxString & hostaddr,
                                    int port, wxString & dbname,
                                    wxString & user)
{
//
// attempting to close a single connection to PostgreSQL
//
  char *sql;
  char *virt_name;
  char *xvirt_name;
  char *geom_name;
  int ret;
  char *errMsg = NULL;
  MyPostgresConn *conn = NULL;
  MyPostgresConn *pC = PostgresList.GetFirst();
  while (pC != NULL)
    {
      if (pC->GetHost().Cmp(host) == 0 && pC->GetHostAddr().Cmp(hostaddr) == 0
          && pC->GetPort() == port && pC->GetDbName().Cmp(dbname) == 0
          && pC->GetUser().Cmp(user) == 0)
        conn = pC;
      pC = pC->GetNext();
    }
  if (conn == NULL)
    return false;

  pC = conn;
  MyPostgresSchema *pS = pC->GetFirst();
  while (pS != NULL)
    {
      MyPostgresTable *pT = pS->GetFirstTable();
      while (pT != NULL)
        {
          if (pT->GetFirst() != NULL)
            {
              // dropping some SpatiaLite-like Spatial View based on a PostgreSQL Table
              virt_name = (char *) malloc((pT->GetPostGisName().Len() * 4) + 1);
              strcpy(virt_name, pT->GetPostGisName().ToUTF8());
              xvirt_name = gaiaDoubleQuotedSql(virt_name);
              free(virt_name);
              sql =
                sqlite3_mprintf("DROP VIEW IF EXISTS main.\"%s\"", xvirt_name);
              free(xvirt_name);
              ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
              sqlite3_free(sql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("PostGIS wrapper: ") +
                               wxString::FromUTF8(errMsg),
                               wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                               this);
                  sqlite3_free(errMsg);
                }
              MyPostGisGeometry *pG = pT->GetFirst();
              while (pG != NULL)
                {
                  // removing from POSTGIS_GEOMETRY_COLUMNS
                  virt_name =
                    (char *) malloc((pT->GetPostGisName().Len() * 4) + 1);
                  strcpy(virt_name, pT->GetPostGisName().ToUTF8());
                  geom_name = (char *) malloc((pG->GetName().Len() * 4) + 1);
                  strcpy(geom_name, pG->GetName().ToUTF8());
                  sql =
                    sqlite3_mprintf("DELETE FROM temp.postgis_geometry_columns "
                                    "WHERE f_table_name = %Q AND f_geometry_column = %Q",
                                    virt_name, geom_name);
                  free(virt_name);
                  free(geom_name);
                  ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
                  sqlite3_free(sql);
                  if (ret != SQLITE_OK)
                    {
                      wxMessageBox(wxT("PostGIS wrapper: ") +
                                   wxString::FromUTF8(errMsg),
                                   wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                                   this);
                      sqlite3_free(errMsg);
                    }
                  pG = pG->GetNext();
                }
            }
          // dropping some VirtualPG Table
          virt_name = (char *) malloc((pT->GetVirtName().Len() * 4) + 1);
          strcpy(virt_name, pT->GetVirtName().ToUTF8());
          xvirt_name = gaiaDoubleQuotedSql(virt_name);
          free(virt_name);
          sql = sqlite3_mprintf("DROP TABLE IF EXISTS main.\"%s\"", xvirt_name);
          free(xvirt_name);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
          sqlite3_free(sql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("PostgreSQL wrapper: ") +
                           wxString::FromUTF8(errMsg),
                           wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
            }
          pT = pT->GetNext();
        }
      MyPostgresView *pV = pS->GetFirstView();
      while (pV != NULL)
        {
          // dropping some VirtualPG View
          virt_name = (char *) malloc((pV->GetVirtName().Len() * 4) + 1);
          strcpy(virt_name, pV->GetVirtName().ToUTF8());
          xvirt_name = gaiaDoubleQuotedSql(virt_name);
          free(virt_name);
          sql = sqlite3_mprintf("DROP TABLE IF EXISTS main.\"%s\"", xvirt_name);
          free(xvirt_name);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, &errMsg);
          sqlite3_free(sql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("PostgreSQL wrapper: ") +
                           wxString::FromUTF8(errMsg),
                           wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
            }
          pV = pV->GetNext();
        }
      pS = pS->GetNext();
    }
// removing the connection from the list
  PostgresList.Remove(conn);
  return true;
}

bool MyFrame::DoCheckPostGisGeometry(wxString & virtName, wxString & columnName)
{
//
// checking for a PostGIS Geometry column (wrapping View)
//
  MyPostgresConn *pC = PostgresList.GetFirst();
  while (pC != NULL)
    {
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          MyPostgresTable *pT = pS->GetFirstTable();
          while (pT != NULL)
            {
              if (pT->GetPostGisName().Cmp(virtName) == 0)
                {
                  MyPostGisGeometry *pG = pT->GetFirst();
                  while (pG != NULL)
                    {
                      if (pG->GetName().Cmp(columnName) == 0)
                        return true;
                      pG = pG->GetNext();
                    }
                }
              pT = pT->GetNext();
            }
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
  return false;
}

void MyFrame::InitPostgresPkColumns(MyPostgresConn * conn,
                                    MyPostgresSchema * schema,
                                    MyPostgresTable * table)
{
//
// attempting to initialize the PK columns for a PostgreSQL Table
//  
  PGconn *pg_conn;
  PGresult *res;
  int nRows;
  int nFields;
  int r;
  char *connection_string;

  if (table->IsPkAlreadyChecked() == true)
    return;                     // already initialized  

// attempting to establish a connection to Postgres
  connection_string =
    (char *) malloc((conn->GetConnectionString().Len() * 4) + 1);
  strcpy(connection_string, conn->GetConnectionString().ToUTF8());

  pg_conn = vpgPQconnectdb(connection_string);
  if (vpgPQstatus(pg_conn) != CONNECTION_OK)
    {
      char *msg = sqlite3_mprintf("Connection to Postgres failed: %s",
                                  vpgPQerrorMessage(pg_conn));
      wxMessageBox(wxString::FromUTF8(msg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(msg);
      vpgPQfinish(pg_conn);
      free(connection_string);
      return;
    }
  free(connection_string);

// querying the PK columns
  char *pg_schema = (char *) malloc(schema->GetName().Len() * 4 + 1);
  strcpy(pg_schema, schema->GetName().ToUTF8());
  char *pg_table = (char *) malloc(table->GetName().Len() * 4 + 1);
  strcpy(pg_table, table->GetName().ToUTF8());
  char *sql =
    sqlite3_mprintf
    ("SELECT a.attname FROM pg_index AS i "
     "JOIN pg_attribute AS a ON (a.attrelid = i.indrelid AND a.attnum = ANY(i.indkey)) "
     "WHERE i.indrelid = '%s.%s'::regclass AND i.indisprimary", pg_schema,
     pg_table);
  free(pg_schema);
  free(pg_table);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    goto err;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *col_name = vpgPQgetvalue(res, r, 0);
          wxString column = wxString::FromUTF8(col_name);
          table->AddPK(column);
        }
    }
  vpgPQclear(res);
  vpgPQfinish(pg_conn);
  table->SetPkChecked();
  return;

err:
// something has gone the wrong way
  if (res != NULL)
    vpgPQclear(res);
  vpgPQfinish(pg_conn);
}

void MyFrame::InitPostgresPks(void *conn,
                              MyPostgresSchema * schema,
                              MyPostgresTable * table)
{
//
// attempting to initialize the PK columns for a PostgreSQL Table
//  
  PGconn *pg_conn = (PGconn *) conn;
  PGresult *res;
  int nRows;
  int nFields;
  int r;

  if (table->IsPkAlreadyChecked() == true)
    return;                     // already initialized

// querying the PK columns
  char *pg_schema = (char *) malloc(schema->GetName().Len() * 4 + 1);
  strcpy(pg_schema, schema->GetName().ToUTF8());
  char *pg_table = (char *) malloc(table->GetName().Len() * 4 + 1);
  strcpy(pg_table, table->GetName().ToUTF8());
  char *sql =
    sqlite3_mprintf
    ("SELECT a.attname FROM pg_index AS i "
     "JOIN pg_attribute AS a ON (a.attrelid = i.indrelid AND a.attnum = ANY(i.indkey)) "
     "WHERE i.indrelid = '%s.%s'::regclass AND i.indisprimary", pg_schema,
     pg_table);
  free(pg_schema);
  free(pg_table);
  res = vpgPQexec(pg_conn, sql);
  sqlite3_free(sql);
  if (vpgPQresultStatus(res) != PGRES_TUPLES_OK)
    return;
  nRows = vpgPQntuples(res);
  nFields = vpgPQnfields(res);
  if (nRows > 0 && nFields == 1)
    {
      for (r = 0; r < nRows; r++)
        {
          const char *col_name = vpgPQgetvalue(res, r, 0);
          wxString column = wxString::FromUTF8(col_name);
          table->AddPK(column);
        }
    }
  vpgPQclear(res);
  table->SetPkChecked();
}

char *MyFrame::DropPostgreSqlOrphans()
{
//
// attempting to Drop all PostgreSql Orphans
//
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  int first = 1;
  char *sql_statement = NULL;
  char *prev;
  bool ok = false;

// fetching all candidate Tables
  const char *sql = "SELECT name FROM sqlite_master "
    "WHERE type = 'table' AND name LIKE 'vpg_%' AND sql LIKE '%Using VirtualPostgres%'";
  ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    goto error;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *name = results[(i * columns) + 0];
          wxString xName = wxString::FromUTF8(name);
          if (PostgresList.FindTable(NULL, xName) == NULL)
            {
              // found an Orphan Table - to be dropped
              char *x_name = gaiaDoubleQuotedSql(name);
              if (first)
                {
                  first = 0;
                  sql_statement =
                    sqlite3_mprintf("BEGIN;\n;DROP TABLE MAIN.\"%s\";\n",
                                    x_name);
              } else
                {
                  prev = sql_statement;
                  sql_statement =
                    sqlite3_mprintf("%sDROP TABLE MAIN.\"%s\";\n", prev,
                                    x_name);
                  sqlite3_free(prev);
                }
              free(x_name);
              ok = true;
            }
        }
    }
  sqlite3_free_table(results);


// fetching all candidate Views
  sql = "SELECT name FROM sqlite_master "
    "WHERE type = 'view' AND name LIKE 'vpg_%' AND sql LIKE '%GeomFromEWKB%'";
  ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    goto error;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *name = results[(i * columns) + 0];
          wxString xName = wxString::FromUTF8(name);
          if (PostgresList.FindPostGisView(NULL, xName) == NULL)
            {
              // found an Orphan View - to be dropped
              char *x_name = gaiaDoubleQuotedSql(name);
              if (first)
                {
                  first = 0;
                  sql_statement =
                    sqlite3_mprintf("BEGIN;\n;DROP VIEW MAIN.\"%s\";\n",
                                    x_name);
              } else
                {
                  prev = sql_statement;
                  sql_statement =
                    sqlite3_mprintf("%sDROP VIEW MAIN.\"%s\";\n", prev, x_name);
                  sqlite3_free(prev);
                }
              free(x_name);
              ok = true;
            }
        }
    }
  sqlite3_free_table(results);
  if (ok == false)
    return NULL;
// committing the transaction
  prev = sql_statement;
  sql_statement = sqlite3_mprintf("%sCOMMIT;\n", prev);
  sqlite3_free(prev);
  return sql_statement;

error:
  wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  sqlite3_free(errMsg);
  return NULL;
}

bool PostgresConnectionDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Connecting to PostgreSQL")) ==
      false)
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

void PostgresConnectionDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxString user = wxGetUserId();
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: HOST
  wxBoxSizer *hostSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(hostSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticText *hostLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Host Name"));
  hostSizer->Add(hostLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *hostCtrl = new wxTextCtrl(this, ID_POSTGRES_HOST,
                                        wxT("localhost"), wxDefaultPosition,
                                        wxSize(250, 22));
  hostSizer->Add(hostCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
// second row: HOSTADDR
  wxBoxSizer *addrSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(addrSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticText *addrLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Host Address"));
  addrSizer->Add(addrLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *addrCtrl = new wxTextCtrl(this, ID_POSTGRES_HOSTADDR,
                                        wxT("127.0.0.1"), wxDefaultPosition,
                                        wxSize(250, 22));
  addrSizer->Add(addrCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
// third row: PORT
  wxBoxSizer *portSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(portSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticText *portLabel = new wxStaticText(this, wxID_STATIC, wxT("Port:"));
  portSizer->Add(portLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *portCtrl = new wxSpinCtrl(this, ID_POSTGRES_PORT, wxT("5432"),
                                        wxDefaultPosition, wxSize(80, 20),
                                        wxSP_ARROW_KEYS,
                                        0, 65535, 5432);
  portSizer->Add(portCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fourth row: DBNAME
  wxBoxSizer *dbSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(dbSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticText *dbLabel = new wxStaticText(this, wxID_STATIC, wxT("DB Name"));
  dbSizer->Add(dbLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *dbCtrl = new wxTextCtrl(this, ID_POSTGRES_DBNAME,
                                      user, wxDefaultPosition,
                                      wxSize(250, 22));
  dbSizer->Add(dbCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
// fifth row: USER
  wxBoxSizer *userSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(userSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticText *userLabel = new wxStaticText(this, wxID_STATIC, wxT("User"));
  userSizer->Add(userLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *userCtrl = new wxTextCtrl(this, ID_POSTGRES_USER,
                                        user, wxDefaultPosition,
                                        wxSize(250, 22));
  userSizer->Add(userCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
// sixth row: PASSWORD
  wxBoxSizer *pwdSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(pwdSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticText *pwdLabel = new wxStaticText(this, wxID_STATIC, wxT("Password"));
  pwdSizer->Add(pwdLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *pwdCtrl = new wxTextCtrl(this, ID_POSTGRES_PASSWORD,
                                       wxT(""), wxDefaultPosition,
                                       wxSize(250, 22), wxTE_PASSWORD);
  pwdSizer->Add(pwdCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
// seventh row: Options
  wxBoxSizer *optSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(optSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString rdonly[2];
  rdonly[0] = wxT("Read Only");
  rdonly[1] = wxT("Read && Write");
  wxRadioBox *rdonly_sel = new wxRadioBox(this, ID_POSTGRES_RDONLY,
                                          wxT("Connection Mode"),
                                          wxDefaultPosition,
                                          wxDefaultSize, 2,
                                          rdonly, 2,
                                          wxRA_SPECIFY_ROWS);
  rdonly_sel->SetSelection(0);
  optSizer->Add(rdonly_sel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);


  wxString textdates[2];
  textdates[0] = wxT("as Julian Day numbers");
  textdates[1] = wxT("as PlainText strings");
  wxRadioBox *textdates_sel = new wxRadioBox(this, ID_POSTGRES_TEXTDATES,
                                             wxT("&DATE/TIME values"),
                                             wxDefaultPosition,
                                             wxDefaultSize, 2,
                                             textdates, 2,
                                             wxRA_SPECIFY_ROWS);
  textdates_sel->SetSelection(1);
  optSizer->Add(textdates_sel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Connect"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & PostgresConnectionDialog::OnOk);
}

void PostgresConnectionDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *hostCtrl = (wxTextCtrl *) FindWindow(ID_POSTGRES_HOST);
  wxTextCtrl *addrCtrl = (wxTextCtrl *) FindWindow(ID_POSTGRES_HOSTADDR);
  wxSpinCtrl *portCtrl = (wxSpinCtrl *) FindWindow(ID_POSTGRES_PORT);
  wxTextCtrl *dbCtrl = (wxTextCtrl *) FindWindow(ID_POSTGRES_DBNAME);
  wxTextCtrl *userCtrl = (wxTextCtrl *) FindWindow(ID_POSTGRES_USER);
  wxTextCtrl *pwdCtrl = (wxTextCtrl *) FindWindow(ID_POSTGRES_PASSWORD);
  wxRadioBox *rdonly_sel = (wxRadioBox *) FindWindow(ID_POSTGRES_RDONLY);
  wxRadioBox *textdates_sel = (wxRadioBox *) FindWindow(ID_POSTGRES_TEXTDATES);
  wxString value = hostCtrl->GetValue();
  if (host != NULL)
    free(host);
  if (value.Len() == 0)
    host = NULL;
  else
    {
      host = (char *) malloc(value.Len() * 4 + 1);
      strcpy(host, value.ToUTF8());
    }
  value = addrCtrl->GetValue();
  if (hostaddr != NULL)
    free(hostaddr);
  if (value.Len() == 0)
    hostaddr = NULL;
  else
    {
      hostaddr = (char *) malloc(value.Len() * 4 + 1);
      strcpy(hostaddr, value.ToUTF8());
    }
  port = portCtrl->GetValue();
  value = dbCtrl->GetValue();
  if (dbname != NULL)
    free(dbname);
  if (value.Len() == 0)
    dbname = NULL;
  else
    {
      dbname = (char *) malloc(value.Len() * 4 + 1);
      strcpy(dbname, value.ToUTF8());
    }
  value = userCtrl->GetValue();
  if (user != NULL)
    free(user);
  if (value.Len() == 0)
    user = NULL;
  else
    {
      user = (char *) malloc(value.Len() * 4 + 1);
      strcpy(user, value.ToUTF8());
    }
  value = pwdCtrl->GetValue();
  if (password != NULL)
    free(password);
  if (value.Len() == 0)
    password = NULL;
  else
    {
      password = (char *) malloc(value.Len() * 4 + 1);
      strcpy(password, value.ToUTF8());
    }
  if (rdonly_sel->GetSelection() == 0)
    ReadOnly = true;
  else
    ReadOnly = false;
  if (textdates_sel->GetSelection() == 0)
    TextDates = false;
  else
    TextDates = true;
  wxDialog::EndModal(wxID_OK);
}

MyPostGisGeometry::MyPostGisGeometry(wxString & name, wxString & type, int srid,
                                     int dims)
{
// ctor
  Name = name;
  GeomType = type;
  MultiType = false;
  Srid = srid;
  Dims = dims;
  if (type.Len() == 0)
    return;
  if (type.Cmp(wxT("MULTIPOINT")) == 0)
    MultiType = true;
  if (type.Cmp(wxT("MULTILINESTRING")) == 0)
    MultiType = true;
  if (type.Cmp(wxT("MULTIPOLYGON")) == 0)
    MultiType = true;
  Next = NULL;
}

MyPostgresTable::MyPostgresTable(wxString & name)
{
// ctor
  Name = name;
  PkChecked = false;
  GrantSelect = false;
  GrantInsertUpdateDelete = false;
  FirstPK = NULL;
  LastPK = NULL;
  First = NULL;
  Last = NULL;
  Next = NULL;
}

MyPostgresTable::~MyPostgresTable()
{
// dtor
  MyPostGisGeometry *pG;
  MyPostGisGeometry *pGn;
  MyPostgresPK *pK;
  MyPostgresPK *pKn;
  pG = First;
  while (pG != NULL)
    {
      pGn = pG->GetNext();
      delete pG;
      pG = pGn;
    }
  pK = FirstPK;
  while (pK != NULL)
    {
      pKn = pK->GetNext();
      delete pK;
      pK = pKn;
    }
}

void MyPostgresTable::AddPK(wxString & name)
{
// adding a PostgreSQL PK column
  MyPostgresPK *pK = FirstPK;
  while (pK != NULL)
    {
      if (pK->GetName().Cmp(name) == 0)
        return;                 // already defined
      pK = pK->GetNext();
    }

// inserting a new PostgreSQL PK column
  pK = new MyPostgresPK(name);
  if (FirstPK == NULL)
    FirstPK = pK;
  if (LastPK != NULL)
    LastPK->SetNext(pK);
  LastPK = pK;
}

void MyPostgresTable::Add(wxString & geometry, wxString & type, int srid,
                          int dims)
{
//
// adding a PostGIS geometry to a Table
//
  if (type.Len() == 0)
    return;

  if (geometry.Len() == 0)
    return;

  MyPostGisGeometry *pG = First;
  while (pG != NULL)
    {
      if (pG->GetName().Cmp(geometry) == 0)
        return;                 // already defined
      pG = pG->GetNext();
    }

// inserting a new PostGIS geometry
  pG = new MyPostGisGeometry(geometry, type, srid, dims);
  if (First == NULL)
    First = pG;
  if (Last != NULL)
    Last->SetNext(pG);
  Last = pG;
}

bool MyPostgresTable::IsPkColumn(wxString & name)
{
// checks if a PostgreSQL Column is a PK column
  MyPostgresPK *pK = FirstPK;
  while (pK != NULL)
    {
      if (pK->GetName().Cmp(name) == 0)
        return true;
      pK = pK->GetNext();
    }
  return false;
}

bool MyPostgresTable::HasPK()
{
// checks if a PostgreSQL Table has any PK column
  if (FirstPK == NULL)
    return false;
  return true;
}

MyPostGisGeometry *MyPostgresTable::Find(wxString & name)
{
// attempting to find a Geometry Column
  MyPostGisGeometry *pG = First;
  while (pG != NULL)
    {
      if (pG->GetName().Cmp(name) == 0)
        return pG;
      pG = pG->GetNext();
    }
  return NULL;
}

MyPostgresView::MyPostgresView(wxString & name)
{
// ctor
  Name = name;
  GrantSelect = false;
  GrantInsertUpdateDelete = false;
  Next = NULL;
}

MyPostgresSchema::MyPostgresSchema(wxString & name)
{
// ctor
  Name = name;
  FirstTable = NULL;
  LastTable = NULL;
  CurrentTable = NULL;
  FirstView = NULL;
  LastView = NULL;
  Next = NULL;
}

MyPostgresSchema::~MyPostgresSchema()
{
// dtor
  MyPostgresTable *pT;
  MyPostgresTable *pTn;
  MyPostgresView *pV;
  MyPostgresView *pVn;
  pT = FirstTable;
  while (pT != NULL)
    {
      pTn = pT->GetNext();
      delete pT;
      pT = pTn;
    }
  pV = FirstView;
  while (pV != NULL)
    {
      pVn = pV->GetNext();
      delete pV;
      pV = pVn;
    }
}

void MyPostgresSchema::Add(wxString & name, wxString & geometry,
                           wxString & type, int srid, int dims)
{
//
// adding a Table to a Schema
//
  MyPostgresTable *pT = CurrentTable;
  if (pT != NULL)
    {
      if (pT->GetName().Cmp(name) == 0)
        {
          // continuing with the current table
          if (geometry.Len() > 0 && type.Len() > 0)
            pT->Add(geometry, type, srid, dims);
          return;
        }
    }

  pT = FirstTable;
  while (pT != NULL)
    {
      if (pT->GetName().Cmp(name) == 0)
        {
          // already defined
          if (geometry.Len() > 0 && type.Len() > 0)
            pT->Add(geometry, type, srid, dims);
          return;
        }
      pT = pT->GetNext();
    }

// adding a new Table
  pT = new MyPostgresTable(name);
  if (geometry.Len() > 0 && type.Len() > 0)
    pT->Add(geometry, type, srid, dims);
  if (FirstTable == NULL)
    FirstTable = pT;
  if (LastTable != NULL)
    LastTable->SetNext(pT);
  LastTable = pT;
  CurrentTable = pT;
}

void MyPostgresSchema::Add(wxString & name)
{
//
// adding a View to a Schema
//
  MyPostgresView *pV = FirstView;
  while (pV != NULL)
    {
      if (pV->GetName().Cmp(name) == 0)
        return;                 // already defined
      pV = pV->GetNext();
    }

// adding a new View
  pV = new MyPostgresView(name);
  if (FirstView == NULL)
    FirstView = pV;
  if (LastView != NULL)
    LastView->SetNext(pV);
  LastView = pV;
}

MyPostgresConn::MyPostgresConn(wxString & host, wxString & hostaddr, int port,
                               wxString & dbname, wxString & user,
                               bool readOnly, bool textDates)
{
// ctor
  Host = host;
  HostAddr = hostaddr;
  Port = port;
  DbName = dbname;
  User = user;
  ReadOnly = readOnly;
  TextDates = textDates;
  First = NULL;
  Last = NULL;
  Current = NULL;
  Prev = NULL;
  Next = NULL;
}

MyPostgresConn::~MyPostgresConn()
{
// dtor
  MyPostgresSchema *pS;
  MyPostgresSchema *pSn;
  pS = First;
  while (pS != NULL)
    {
      pSn = pS->GetNext();
      delete pS;
      pS = pSn;
    }
}

void MyPostgresConn::Add(wxString & schema, wxString & table,
                         wxString & geometry, wxString & type, int srid,
                         int dims)
{
//
// adding a Table to a PostgreSQL connection
//
  MyPostgresSchema *pS = Current;
  if (pS != NULL)
    {
      if (pS->GetName().Cmp(schema) == 0)
        {
          // continuing with the current Schema
          pS->Add(table, geometry, type, srid, dims);
          return;
        }
    }

  pS = First;
  while (pS != NULL)
    {
      if (pS->GetName().Cmp(schema) == 0)
        {
          // the Schema is already defined
          pS->Add(table, geometry, type, srid, dims);
          return;
        }
      pS = pS->GetNext();
    }

// adding a new Schema
  pS = new MyPostgresSchema(schema);
  pS->Add(table, geometry, type, srid, dims);
  if (First == NULL)
    First = pS;
  if (Last != NULL)
    Last->SetNext(pS);
  Last = pS;
  Current = pS;
}

void MyPostgresConn::Add(wxString & schema, wxString & view)
{
//
// adding a View to a PostgreSQL connection
//
  MyPostgresSchema *pS = Current;
  if (pS != NULL)
    {
      if (pS->GetName().Cmp(schema) == 0)
        {
          // continuing with the current Schema
          pS->Add(view);
          return;
        }
      pS = pS->GetNext();
    }

  pS = First;
  while (pS != NULL)
    {
      if (pS->GetName().Cmp(schema) == 0)
        {
          // the Schema is already defined
          pS->Add(view);
          return;
        }
      pS = pS->GetNext();
    }

// adding a new Schema
  pS = new MyPostgresSchema(schema);
  pS->Add(view);
  if (First == NULL)
    First = pS;
  if (Last != NULL)
    Last->SetNext(pS);
  Last = pS;
  Current = pS;
}

void MyPostgres::Clear()
{
//
// memory cleanup - clearing a MyPostgres object
//
  MyPostgresConn *pC;
  MyPostgresConn *pCn;
  pC = First;
  while (pC != NULL)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
  First = NULL;
  Last = NULL;
  Current = NULL;
}

MyPostgresConn *MyPostgres::Insert(wxString & host, wxString & hostaddr,
                                   int port, wxString & dbname, wxString & user,
                                   bool readOnly, bool textDates)
{
//
// inserting into the list a new connection to PostgreSQL
// (or returning an already existing one)
//
  MyPostgresConn *pC = Current;
  if (pC != NULL)
    {
      if (pC->GetHost().Cmp(host) == 0 && pC->GetHostAddr().Cmp(hostaddr) == 0
          && pC->GetPort() == port && pC->GetDbName().Cmp(dbname) == 0
          && pC->GetUser().Cmp(user) == 0)
        return pC;              // continuing with the current connection
    }

  pC = Find(host, hostaddr, port, dbname, user);
  if (pC != NULL)
    return pC;                  // already defined

// adding a new connection
  pC =
    new MyPostgresConn(host, hostaddr, port, dbname, user, readOnly, textDates);
  if (First == NULL)
    First = pC;
  pC->SetPrev(Last);
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
  Current = pC;
  return pC;
}

MyPostgresConn *MyPostgres::Find(wxString & host, wxString & hostaddr, int port,
                                 wxString & dbname, wxString & user)
{
//
// searching for a given connection
//
  MyPostgresConn *pC = First;
  while (pC != NULL)
    {
      if (pC->GetHost().Cmp(host) == 0 && pC->GetHostAddr().Cmp(hostaddr) == 0
          && pC->GetPort() == port && pC->GetDbName().Cmp(dbname) == 0
          && pC->GetUser().Cmp(user) == 0)
        return pC;
      pC = pC->GetNext();
    }
  return NULL;
}

void MyPostgres::Remove(MyPostgresConn * conn)
{
//
// removing a connection from the list 
//
  MyPostgresConn *pC = First;
  while (pC != NULL)
    {
      if (pC == conn)
        {
          MyPostgresConn *x = pC->GetPrev();
          if (x != NULL)
            x->SetNext(pC->GetNext());
          x = pC->GetNext();
          if (x != NULL)
            x->SetPrev(pC->GetPrev());
          if (pC == First)
            First = pC->GetNext();
          if (pC == Last)
            Last = pC->GetPrev();
          if (pC == Current)
            Current = Last;
          delete pC;
          return;
        }
      pC = pC->GetNext();
    }
}

bool MyPostgres::CheckUniqueVirtName(wxString & virtName)
{
//
// testing for PostgreSQL unique virtual names
//
  MyPostgresConn *pC = First;
  while (pC != NULL)
    {
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          MyPostgresTable *pT = pS->GetFirstTable();
          while (pT != NULL)
            {
              if (virtName.Cmp(pT->GetVirtName()) == 0)
                return false;
              if (virtName.Cmp(pT->GetPostGisName()) == 0)
                return false;
              pT = pT->GetNext();
            }
          MyPostgresView *pV = pS->GetFirstView();
          while (pV != NULL)
            {
              if (virtName.Cmp(pV->GetVirtName()) == 0)
                return false;
              pV = pV->GetNext();
            }
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
  return true;
}

void MyPostgres::MakeUniqueVirtName(wxString & baseName, wxString & uniqueName)
{
//
// ensuring to create PostgreSQL unique virtual names
//
  if (CheckUniqueVirtName(baseName) == true)
    {
      uniqueName = baseName;
      return;
    }

  int alias = 0;
  while (1)
    {
      wxString suffix;
      suffix.Printf(wxT("_%d"), alias);
      wxString test = baseName + suffix;
      if (CheckUniqueVirtName(test) == true)
        {
          uniqueName = test;
          return;
        }
      alias++;
    }
}

MyPostgresTable *MyPostgres::FindTable(MyFrame * parent, wxString & virtName)
{
// attempting to retrieve a PostgreSQL Table
  MyPostgresConn *pC = First;
  while (pC != NULL)
    {
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          MyPostgresTable *pT = pS->GetFirstTable();
          while (pT != NULL)
            {
              if (virtName.Cmp(pT->GetVirtName()) == 0)
                {
                  if (parent != NULL)
                    parent->InitPostgresPkColumns(pC, pS, pT);
                  return pT;
                }
              pT = pT->GetNext();
            }
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
  return NULL;
}

MyPostgresTable *MyPostgres::FindPostGisView(MyFrame * parent,
                                             wxString & virtName)
{
// attempting to retrieve the PostgreSQL Table supporting a PostGIS View (wrapper)
  MyPostgresConn *pC = First;
  while (pC != NULL)
    {
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          MyPostgresTable *pT = pS->GetFirstTable();
          while (pT != NULL)
            {
              if (virtName.Cmp(pT->GetPostGisName()) == 0)
                {
                  if (parent != NULL)
                    parent->InitPostgresPkColumns(pC, pS, pT);
                  return pT;
                }
              pT = pT->GetNext();
            }
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
  return NULL;
}

MyPostgresColumns::~MyPostgresColumns()
{
// dtor
  MyPostgresCol *pC;
  MyPostgresCol *pCn;
  pC = First;
  while (pC != NULL)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
}

void MyPostgresColumns::Add(wxString & name)
{
// adding a Column into the list
  MyPostgresCol *col = new MyPostgresCol(name);
  if (First == NULL)
    First = col;
  if (Last != NULL)
    Last->SetNext(col);
  Last = col;
}

void MyPostgresColumns::SetPK(wxString & name)
{
// setting a PK Column
  MyPostgresCol *pC;
  pC = First;
  while (pC != NULL)
    {
      if (pC->GetName().Cmp(name) == 0)
        {
          pC->SetPK();
          return;
        }
      pC = pC->GetNext();
    }
}

char *MyPostgresColumns::BuildWhere()
{
// building a WHERE clause
  char *where = NULL;
  char *prev;
  int first = 1;
  char name[1024];
  char *xname;
  MyPostgresCol *pC = First;
  while (pC != NULL)
    {
      if (pC->IsPK() == false)
        {
          pC = pC->GetNext();
          continue;
        }
      if (first)
        {
          first = 0;
          strcpy(name, pC->GetName().ToUTF8());
          xname = gaiaDoubleQuotedSql(name);
          where = sqlite3_mprintf("WHERE \"%s\" = OLD.\"%s\"", xname, xname);
          free(xname);
      } else
        {
          prev = where;
          strcpy(name, pC->GetName().ToUTF8());
          xname = gaiaDoubleQuotedSql(name);
          where = sqlite3_mprintf("%s AND \"%s\" = OLD.\"%s\"", xname, xname);
          free(xname);
          sqlite3_free(prev);
        }
      pC = pC->GetNext();
    }
  return where;

}
