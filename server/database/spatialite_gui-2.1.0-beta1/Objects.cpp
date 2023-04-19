/*
/ Objects.cpp
/ miscellaneous ancillary classes implementation
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

MyObject::MyObject(int type)
{
//
// constructor - TreeItemData
//
  Type = type;
  SubType = type;
  if (type == MY_ROOT_USERDATA || type == MY_ROOT_TOPOLOGIES
      || type == MY_ROOT_NETWORKS || type == MY_ROOT_RASTER
      || type == MY_ROOT_VECTOR || type == MY_ROOT_WMS
      || type == MY_ROOT_STYLING || type == MY_ROOT_ISOMETADATA
      || type == MY_ROOT_METADATA || type == MY_ROOT_INTERNAL
      || type == MY_ROOT_RTREE)
    Type = MY_ROOT_NODE;
  DbAlias = wxEmptyString;
  MainName = wxEmptyString;
  ColName = wxEmptyString;
  PathOrURL = wxEmptyString;
  Restricted = true;
  TopoObject = false;
}

MyObject::MyObject(int type, wxString & dbAlias)
{
//
// constructor - TreeItemData
//
  Type = type;
  SubType = type;
  if (type == MY_ROOT_USERDATA || type == MY_ROOT_TOPOLOGIES
      || type == MY_ROOT_NETWORKS || type == MY_ROOT_RASTER
      || type == MY_ROOT_VECTOR || type == MY_ROOT_WMS
      || type == MY_ROOT_STYLING || type == MY_ROOT_ISOMETADATA
      || type == MY_ROOT_METADATA || type == MY_ROOT_INTERNAL
      || type == MY_ROOT_RTREE)
    Type = MY_ROOT_NODE;
  DbAlias = dbAlias;
  MainName = wxEmptyString;
  ColName = wxEmptyString;
  PathOrURL = wxEmptyString;
  Restricted = true;
  TopoObject = false;
}

MyObject::MyObject(int type, wxString & dbAlias, wxString & name,
                   bool restricted, bool topoObj)
{
//
// constructor - TreeItemData
//
  Type = type;
  SubType = type;
  DbAlias = dbAlias;
  MainName = name;
  ColName = wxEmptyString;
  PathOrURL = wxEmptyString;
  Restricted = restricted;
  TopoObject = topoObj;
}

MyObject::MyObject(int type, wxString & dbAlias, wxString & name,
                   wxString & column)
{
//
// constructor - TreeItemData
//
  Type = type;
  SubType = type;
  DbAlias = dbAlias;
  MainName = name;
  ColName = column;
  PathOrURL = wxEmptyString;
  Restricted = true;
  TopoObject = false;
}

MyObject::MyObject(int type, wxString & dbAlias, wxString & name, bool topo_obj,
                   wxString & column)
{
//
// constructor - TreeItemData
//
  Type = type;
  SubType = type;
  DbAlias = dbAlias;
  MainName = name;
  ColName = column;
  PathOrURL = wxEmptyString;
  Restricted = true;
  TopoObject = topo_obj;
}

MyObject::MyObject(int type, wxString & dbAlias, wxString & name,
                   wxString & column, wxString & path_or_url)
{
//
// constructor - TreeItemData
//
  Type = type;
  SubType = type;
  DbAlias = dbAlias;
  MainName = name;
  ColName = column;
  PathOrURL = path_or_url;
  Restricted = true;
  TopoObject = false;
}

MyObject::MyObject(int type, wxString & host, wxString & hostaddr, int port,
                   wxString & dbname, wxString & user, bool readOnly)
{
//
// constructor - TreeItemData
//
  Type = type;
  Host = host;
  HostAddr = hostaddr;
  Port = port;
  DbName = dbname;
  User = user;
  ReadOnly = readOnly;
}

MyObject::MyObject(int type, wxString & host, wxString & hostaddr, int port,
                   wxString & dbname, wxString & user, wxString & schema)
{
//
// constructor - TreeItemData
//
  Type = type;
  Host = host;
  HostAddr = hostaddr;
  Port = port;
  DbName = dbname;
  User = user;
  Schema = schema;
}

MyObject::MyObject(int type, wxString & host, wxString & hostaddr, int port,
                   wxString & dbname, wxString & user, wxString & schema,
                   wxString & name, wxString & virtName, bool readOnly, bool pk,
                   bool select, bool insertUpdateDelete)
{
//
// constructor - TreeItemData
//
  Type = type;
  Host = host;
  HostAddr = hostaddr;
  Port = port;
  DbName = dbname;
  User = user;
  Schema = schema;
  Name = name;
  VirtName = virtName;
  ReadOnly = readOnly;
  PK = pk;
  Select = select;
  InsertUpdateDelete = insertUpdateDelete;
}

MyObject::MyObject(int type, wxString & host, wxString & hostaddr, int port,
                   wxString & dbname, wxString & user, wxString & schema,
                   wxString & name, wxString & column, wxString & virtName)
{
//
// constructor - TreeItemData
//
  Type = type;
  Host = host;
  HostAddr = hostaddr;
  Port = port;
  DbName = dbname;
  User = user;
  Schema = schema;
  Name = name;
  Column = column;
  VirtName = virtName;
}

bool MyObject::IsEditable()
{
//
// testing for an EDITABLE TABLE or VIEW object
//
  if (Restricted)
    return false;
  if (Type == MY_TABLE)
    return true;
  return false;
}

bool MyObject::IsAttached()
{
//
// testing for an ATTACHED object
//
  if (IsTemporary())
    return false;
  if (DbAlias.Len() > 0)
    {
      if (DbAlias.IsSameAs(wxT("main"), false))
        return false;
      return true;
    }
  return false;
}

bool MyObject::IsTemporary()
{
//
// testing for a TEMP object
//
  if (DbAlias.IsSameAs(wxT("temp"), false))
    return true;
  return false;
}

bool MyObject::IsRootAttached()
{
//
// testing for a ROOT ATTACHED node
//
  if (Type != MY_ROOT_NODE)
    return false;
  return IsAttached();
}

bool MyObject::IsAttachedDB()
{
//
// testing for an ATTACHED-DB main node
//
  if (Type == MY_ATTACHED)
    return true;
  return false;
}

bool MyObject::IsTable()
{
//
// testing for TABLE nodes
//
  if (Type == MY_TABLE)
    return true;
  if (Type == MY_TILE_DATA)
    return true;
  if (Type == MY_VTABLE)
    return true;
  if (Type == MY_GPKG_TABLE)
    return true;
  if (Type == MY_GPKG_VTABLE)
    return true;
  if (Type == MY_FDO_TABLE)
    return true;
  if (Type == MY_FDO_VTABLE)
    return true;
  return false;
}

bool MyObject::IsView()
{
//
// testing for VIEW nodes
//
  if (Type == MY_VIEW)
    return true;
  return false;
}

bool MyObject::IsColumn()
{
//
// testing for COLUMN nodes
//
  if (Type == MY_COLUMN)
    return true;
  return false;
}

bool MyObject::IsViewColumn()
{
//
// testing for VIEW COLUMN nodes
//
  if (Type == MY_VIEW_COLUMN)
    return true;
  return false;
}

bool MyObject::IsVirtualColumn()
{
//
// testing for VIRTUAL COLUMN nodes
//
  if (Type == MY_VIRTUAL_COLUMN)
    return true;
  return false;
}

bool MyObject::IsGpkgColumn()
{
//
// testing for GPKG COLUMN nodes
//
  if (Type == MY_GPKG_COLUMN)
    return true;
  return false;
}

bool MyObject::IsFdoOgrColumn()
{
//
// testing for FDO/OGR COLUMN nodes
//
  if (Type == MY_FDO_COLUMN)
    return true;
  return false;
}

bool MyObject::IsAnyGeometryColumn()
{
//
// testing for generic GEOMETRY COLUMN nodes
//
  if (Type == MY_GEOMETRY)
    return true;
  if (Type == MY_GEOMETRY_INDEX)
    return true;
  if (Type == MY_GEOMETRY_CACHED)
    return true;
  if (Type == MY_VIEW_GEOMETRY)
    return true;
  if (Type == MY_VIEW_GEOMETRY_INDEX)
    return true;
  if (Type == MY_VIEW_GEOMETRY_CACHED)
    return true;
  if (Type == MY_GPKG_GEOMETRY)
    return true;
  if (Type == MY_GPKG_GEOMETRY_INDEX)
    return true;
  if (Type == MY_VIRTUAL_GEOMETRY)
    return true;
  if (Type == MY_VIRTUAL_GPKG_GEOMETRY)
    return true;
  if (Type == MY_FDO_GEOMETRY)
    return true;
  if (Type == MY_VIRTUAL_FDO_GEOMETRY)
    return true;
  return false;
}

bool MyObject::IsGpkgGeometryColumn()
{
//
// testing for GPKG GEOMETRY COLUMN nodes
//
  if (Type == MY_GPKG_GEOMETRY)
    return true;
  if (Type == MY_GPKG_GEOMETRY_INDEX)
    return true;
  return false;
}

bool MyObject::IsGpkgVirtualGeometryColumn()
{
//
// testing for GPKG VIRTUAL GEOMETRY COLUMN nodes
//
  if (Type == MY_VIRTUAL_GPKG_GEOMETRY)
    return true;
  return false;
}

bool MyObject::IsFdoOgrGeometryColumn()
{
//
// testing for FDO/OGR GEOMETRY COLUMN nodes
//
  if (Type == MY_FDO_GEOMETRY)
    return true;
  return false;
}

bool MyObject::IsFdoOgrVirtualGeometryColumn()
{
//
// testing for FDO/OGR VIRTUAL GEOMETRY COLUMN nodes
//
  if (Type == MY_VIRTUAL_FDO_GEOMETRY)
    return true;
  return false;
}

bool MyObject::IsGeometryColumn()
{
//
// testing for genuine GEOMETRY COLUMN nodes
//
  if (Type == MY_GEOMETRY)
    return true;
  if (Type == MY_GEOMETRY_INDEX)
    return true;
  if (Type == MY_GEOMETRY_CACHED)
    return true;
  return false;
}

bool MyObject::IsIndex()
{
//
// testing for INDEX nodes
//
  if (Type == MY_INDEX)
    return true;
  return false;
}

bool MyObject::IsIndexColumn()
{
//
// testing for INDEX COLUMN nodes
//
  if (Type == MY_INDEX_FLD)
    return true;
  return false;
}

bool MyObject::IsTrigger()
{
//
// testing for TRIGGER nodes
//
  if (Type == MY_TRIGGER)
    return true;
  return false;
}

bool MyObject::IsPrimaryKey()
{
//
// testing for PRIMARY KEY nodes
//
  if (Type == MY_PRIMARY_KEY)
    return true;
  return false;
}

bool MyObject::IsPrimaryKeyColumn()
{
//
// testing for PRIMARY KEY COLUMN nodes
//
  if (Type == MY_PRIMARY_KEY_FLD)
    return true;
  return false;
}

bool MyObject::IsForeignKey()
{
//
// testing for FOREIGN KEY nodes
//
  if (Type == MY_FOREIGN_KEY)
    return true;
  return false;
}

bool MyObject::IsForeignKeyColumn()
{
//
// testing for FOREIGN KEY COLUMN nodes
//
  if (Type == MY_FOREIGN_KEY_FLD)
    return true;
  return false;
}

bool MyObject::IsPostgreSQL()
{
//
// testing for PostgreSQL related nodes
//
  bool ok = false;
  switch (Type)
    {
      case MY_POSTGRES_CONN:
      case MY_POSTGRES_SCHEMA:
      case MY_POSTGRES_TABLE:
      case MY_POSTGRES_VIEW:
      case MY_POSTGIS_VIEW:
      case MY_POSTGRES_COLUMN:
      case MY_POSTGIS_GEOMETRY:
        ok = true;
        break;
      default:
        ok = false;
        break;
    };
  return ok;
}

bool MyObject::IsAlreadyDefinedVectorCoverage(sqlite3 * sqlite)
{
//
// testing if a Vector Coverage is already defined 
//
  int count = 1;
  int i;
  char **results;
  int rows;
  int columns;
  char *sql;
  if (DbAlias.Len() > 0)
    return true;
  char *table = (char *) malloc(MainName.Len() * 4);
  char *column = (char *) malloc(ColName.Len() * 4);
  strcpy(table, MainName.ToUTF8());
  strcpy(column, ColName.ToUTF8());
  if (Type == MY_GEOMETRY || Type == MY_GEOMETRY_INDEX
      || Type == MY_GEOMETRY_CACHED)
    sql =
      sqlite3_mprintf("SELECT Count(*) FROM vector_coverages WHERE "
                      "Upper(f_table_name) = Upper(%Q) AND Upper(f_geometry_column) = Upper(%Q)",
                      table, column);
  else if (Type == MY_VIEW_GEOMETRY || Type == MY_VIEW_GEOMETRY_INDEX
           || Type == MY_VIEW_GEOMETRY_CACHED)
    sql =
      sqlite3_mprintf("SELECT Count(*) FROM vector_coverages WHERE "
                      "Upper(view_name) = Upper(%Q) AND Upper(view_geometry) = Upper(%Q)",
                      table, column);
  else if (Type == MY_VIRTUAL_GEOMETRY)
    sql = sqlite3_mprintf("SELECT Count(*) FROM vector_coverages WHERE "
                          "Upper(virt_name) = Upper(%Q) AND Upper(virt_geometry) = Upper(%Q)",
                          table, column);
  else
    {
      // unknown Geometry type - quitting
      free(table);
      free(column);
      return true;
    }
  free(table);
  free(column);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return true;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        count = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
  if (count >= 1)
    return true;
  return false;
}

char *MyObject::DoFindUnusedCoverageName(sqlite3 * sqlite)
{
//
// finding an unused Coverage Name
//
  int count = 1;
  int i;
  char **results;
  int rows;
  int columns;
  char *sql;
  char *coverage_name;
  if (DbAlias.Len() > 0)
    return NULL;
  char *table = (char *) malloc(MainName.Len() * 4);
  strcpy(table, MainName.ToUTF8());

  coverage_name = sqlite3_mprintf("%s", table);
  sql = sqlite3_mprintf("SELECT Count(*) FROM vector_coverages WHERE "
                        "Upper(coverage_name) = Upper(%Q)", coverage_name);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return NULL;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        count = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
  if (count == 0)
    return coverage_name;       // the Coverage will have the same name of the Table

  int suffix = 1;
  while (1)
    {
      coverage_name = sqlite3_mprintf("%s_%d", table, suffix++);
      sql = sqlite3_mprintf("SELECT Count(*) FROM vector_coverages WHERE "
                            "Upper(coverage_name) = Upper(%Q)", coverage_name);
      ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, NULL);
      sqlite3_free(sql);
      if (ret != SQLITE_OK)
        return NULL;
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            count = atoi(results[(i * columns) + 0]);
        }
      sqlite3_free_table(results);
      if (count == 0)
        return coverage_name;   // the Coverage will have a suffix after the name of the Table
      sqlite3_free(coverage_name);
      if (suffix > 100)
        break;
    }

  return NULL;
}

void MyVariant::Copy(MyVariant * other)
{
// 
// transfers a BLOB value
//
  if (other->Type != MY_BLOB_VARIANT)
    return;
  if (!(other->Blob))
    return;
  if (Blob)
    delete[]Blob;
  Type = MY_BLOB_VARIANT;
  BlobSize = other->BlobSize;
  Blob = other->Blob;
  other->Type = MY_NULL_VARIANT;
  other->Blob = NULL;
  other->BlobSize = 0;
}

void MyVariant::Set(const unsigned char *text)
{
//
// sets a String value for this Variant value
//
  Type = MY_TXT_VARIANT;
  TxtValue = wxString::FromUTF8((const char *) text);
}

void MyVariant::Set(const void *blob, int size)
{
//
// sets a BLOB value for this Variant value
//
  if (size <= 0)
    return;
  Type = MY_BLOB_VARIANT;
  BlobSize = size;
  Blob = new unsigned char[size];
  memcpy(Blob, blob, size);
}

void MyRowVariant::Create(int cols)
{
//
// creating the row variant
//
  if (ColumnArray)
    delete[]ColumnArray;
  NumCols = cols;
  ColumnArray = new MyVariant[NumCols];
}

void MyRowVariant::Set(int col, sqlite3_int64 value)
{
//
// setting an Integer value for the Nth column
//
  MyVariant *var;
  if (col < 0 || col >= NumCols)
    return;
  var = ColumnArray + col;
  var->Set(value);
}

void MyRowVariant::Set(int col, double value)
{
//
// setting a Double value for the Nth column
//
  MyVariant *var;
  if (col < 0 || col >= NumCols)
    return;
  var = ColumnArray + col;
  var->Set(value);
}

void MyRowVariant::Set(int col, const unsigned char *value)
{
//
// setting a String value for the Nth column
//
  MyVariant *var;
  if (col < 0 || col >= NumCols)
    return;
  var = ColumnArray + col;
  var->Set(value);
}

void MyRowVariant::Set(int col, const void *blob, int size)
{
//
// setting a BLOB value for the Nth column
//
  MyVariant *var;
  if (col < 0 || col >= NumCols)
    return;
  var = ColumnArray + col;
  var->Set(blob, size);
}

MyVariant *MyRowVariant::GetColumn(int col)
{
//
// returns a Column Variant Value
//
  MyVariant *var;
  if (col < 0 || col >= NumCols)
    return NULL;
  var = ColumnArray + col;
  return var;
}

MyVariantList::MyVariantList()
{
//
// constructor - result set container
//
  NumCols = 0;
  ColumnName = NULL;
  First = NULL;
  Last = NULL;
}

MyVariantList::~MyVariantList()
{
//
// destructor - result set container
//
  Reset();
}

void MyVariantList::Reset(void)
{
// resetting the list to the initial (empty) state
  MyRowVariant *el;
  MyRowVariant *elN;
  if (ColumnName)
    delete[]ColumnName;
  el = First;
  while (el)
    {
      elN = el->GetNext();
      delete el;
      el = elN;
    }
  NumCols = 0;
  ColumnName = NULL;
  First = NULL;
  Last = NULL;
}

int MyVariantList::GetRows()
{
//
// counting how many rows are there
//
  int cnt = 0;
  MyRowVariant *el = First;
  while (el)
    {
      cnt++;
      el = el->GetNext();
    }
  return cnt;
}

MyRowVariant *MyVariantList::Add(int columns)
{
//
// adds a row into the result set
//
  if (!NumCols)
    {
      NumCols = columns;
      ColumnName = new wxString[NumCols];
    }
  MyRowVariant *el = new MyRowVariant(columns);
  if (!First)
    First = el;
  if (Last)
    Last->SetNext(el);
  Last = el;
  return el;
}

void MyVariantList::SetColumnName(int col, const char *name)
{
//
// storing a column name
//
  if (col < 0 || col >= NumCols)
    return;
  ColumnName[col] = wxString::FromUTF8(name);
}

wxString & MyVariantList::GetColumnName(int col)
{
//
// retrieving a column name
//
  return ColumnName[col];
}

MyBlobs::MyBlobs(int rows, int cols)
{
//
// constructor - a BLOB matrix
//
  int r;
  MyRowVariant *rowVar;
  NumRows = 0;
  NumCols = 0;
  Rows = NULL;
  if (rows < 1 || cols < 1)
    return;
  NumRows = rows;
  NumCols = cols;
  Rows = new MyRowVariant[rows];
  for (r = 0; r < rows; r++)
    {
      rowVar = Rows + r;
      rowVar->Create(cols);
    }
}

MyBlobs::~MyBlobs()
{
//
// destructor - a BLOB matrix
//
  if (Rows)
    delete[]Rows;
}

void MyBlobs::SetBlob(int row, int col, MyVariant * org)
{
//
// setting a BLOB value
//
  MyRowVariant *rowVar;
  MyVariant *dest;
  if (row < 0 || row >= NumRows)
    return;
  if (col < 0 || col >= NumCols)
    return;
  rowVar = Rows + row;
  if (!rowVar)
    return;
  dest = rowVar->GetColumn(col);
  if (!dest)
    return;
  dest->Copy(org);
}

MyVariant *MyBlobs::GetBlob(int row, int col)
{
//
// return a BLOB value
//
  MyRowVariant *rowVar;
  MyVariant *dest;
  if (row < 0 || row >= NumRows)
    return NULL;
  if (col < 0 || col >= NumCols)
    return NULL;
  rowVar = Rows + row;
  if (!rowVar)
    return NULL;
  dest = rowVar->GetColumn(col);
  if (!dest)
    return NULL;
  if (dest->GetType() != MY_BLOB_VARIANT)
    return NULL;
  return dest;
}

MyValues::MyValues(int rows, int cols)
{
//
// constructor - a generic values matrix
//
  int r;
  MyRowVariant *rowVar;
  NumRows = 0;
  NumCols = 0;
  Rows = NULL;
  if (rows < 1 || cols < 1)
    return;
  NumRows = rows;
  NumCols = cols;
  Rows = new MyRowVariant[rows];
  for (r = 0; r < rows; r++)
    {
      rowVar = Rows + r;
      rowVar->Create(cols);
    }
}

MyValues::~MyValues()
{
//
// destructor - a generic values matrix
//
  if (Rows)
    delete[]Rows;
}

void MyValues::SetValue(int row, int col, sqlite3_int64 value)
{
//
// setting an integer value
//
  MyRowVariant *rowVar;
  MyVariant *dest;
  if (row < 0 || row >= NumRows)
    return;
  if (col < 0 || col >= NumCols)
    return;
  rowVar = Rows + row;
  if (!rowVar)
    return;
  dest = rowVar->GetColumn(col);
  if (!dest)
    return;
  dest->Set(value);
}

void MyValues::SetValue(int row, int col, double value)
{
//
// setting a double value
//
  MyRowVariant *rowVar;
  MyVariant *dest;
  if (row < 0 || row >= NumRows)
    return;
  if (col < 0 || col >= NumCols)
    return;
  rowVar = Rows + row;
  if (!rowVar)
    return;
  dest = rowVar->GetColumn(col);
  if (!dest)
    return;
  dest->Set(value);
}

void MyValues::SetValue(int row, int col, wxString & value)
{
//
// setting a string value
//
  MyRowVariant *rowVar;
  MyVariant *dest;
  if (row < 0 || row >= NumRows)
    return;
  if (col < 0 || col >= NumCols)
    return;
  rowVar = Rows + row;
  if (!rowVar)
    return;
  dest = rowVar->GetColumn(col);
  if (!dest)
    return;
  dest->Set(value);
}

MyVariant *MyValues::GetValue(int row, int col)
{
//
// return a generic value
//
  MyRowVariant *rowVar;
  MyVariant *dest;
  if (row < 0 || row >= NumRows)
    return NULL;
  if (col < 0 || col >= NumCols)
    return NULL;
  rowVar = Rows + row;
  if (!rowVar)
    return NULL;
  dest = rowVar->GetColumn(col);
  if (!dest)
    return NULL;
  return dest;
}

MyRowVariant *MyValues::GetRow(int row)
{
//
// return a row of generic values
//
  MyRowVariant *rowVar;
  if (row < 0 || row >= NumRows)
    return NULL;
  rowVar = Rows + row;
  return rowVar;
}

MyColumnInfo::MyColumnInfo(wxString & name, bool pkey)
{
//
// constructor - a table column object
//
  Name = name;
  PrimaryKey = pkey;
  Geometry = false;
  GPKGGeometry = false;
  GPKGVirtualGeometry = false;
  GeometryIndex = false;
  GPKGGeometryIndex = false;
  FdoOgrGeometry = false;
  FdoOgrVirtualGeometry = false;
  MbrCache = false;
  Next = NULL;
}

MyIndexInfo::MyIndexInfo(wxString & name)
{
//
// constructor - a table index object
//
  Name = name;
  Next = NULL;
}

bool MyIndexInfo::ContainsOnlyPrimaryKeyColumns(sqlite3 * sqlite,
                                                wxString & indexName,
                                                MyColumnInfo * first_column)
{
//
// check if an Index does only contain Primary Key columns
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *xname;
  char *xname2;
  wxString Name;
  wxString sql;
  char *xsql;
  int pk_cnt = 0;
  int col_cnt = 0;
  sql = wxT("PRAGMA index_info(");
  xname = (char *) malloc((indexName.Len() * 4) + 1);
  strcpy(xname, indexName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT(")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(sqlite, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, NULL);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 2];
          Name = wxString::FromUTF8(name);
          col_cnt++;
          MyColumnInfo *elc = first_column;
          while (elc)
            {
              if (elc->GetName() == Name && elc->IsPrimaryKey())
                pk_cnt++;
              elc = elc->GetNext();
            }
        }
    }
  sqlite3_free_table(results);
  if (pk_cnt == col_cnt)
    return true;
  return false;
}

MyTriggerInfo::MyTriggerInfo(wxString & name)
{
//
// constructor - a table trigger object
//
  Name = name;
  Next = NULL;
}

MyTableInfo::~MyTableInfo()
{
//
// destructor - a table columns collection
//
  MyColumnInfo *elc;
  MyColumnInfo *elcN;
  MyIndexInfo *eli;
  MyIndexInfo *eliN;
  MyTriggerInfo *elt;
  MyTriggerInfo *eltN;
  elc = FirstColumn;
  while (elc)
    {
      elcN = elc->GetNext();
      delete elc;
      elc = elcN;
    }
  eli = FirstIndex;
  while (eli)
    {
      eliN = eli->GetNext();
      delete eli;
      eli = eliN;
    }
  elt = FirstTrigger;
  while (elt)
    {
      eltN = elt->GetNext();
      delete elt;
      elt = eltN;
    }
}

void MyTableInfo::AddColumn(wxString & name, bool pkey)
{
//
// inserting a column def into a table
//
  MyColumnInfo *el = new MyColumnInfo(name, pkey);
  if (!FirstColumn)
    FirstColumn = el;
  if (LastColumn)
    LastColumn->SetNext(el);
  LastColumn = el;
}

void MyTableInfo::SetGeometry(wxString & name, bool index, bool cached)
{
//
//setting a geometry column
//
  MyColumnInfo *elc;
  elc = FirstColumn;
  while (elc)
    {
      if (name.CmpNoCase(elc->GetName()) == 0)
        {
          elc->SetGeometry();
          if (index == true)
            elc->SetGeometryIndex();
          if (cached == true)
            elc->SetMbrCache();
        }
      elc = elc->GetNext();
    }
}

void MyTableInfo::AddIndex(wxString & name)
{
//
// inserting an index def into a table
//
  MyIndexInfo *el = new MyIndexInfo(name);
  if (!FirstIndex)
    FirstIndex = el;
  if (LastIndex)
    LastIndex->SetNext(el);
  LastIndex = el;
}

void MyTableInfo::AddTrigger(wxString & name)
{
//
// inserting a trigger def into a table
//
  MyTriggerInfo *el = new MyTriggerInfo(name);
  if (!FirstTrigger)
    FirstTrigger = el;
  if (LastTrigger)
    LastTrigger->SetNext(el);
  LastTrigger = el;
}

void MyTableInfo::CheckGPKG(MyFrame * MainFrame, sqlite3 * handle,
                            wxString & table)
{
//
// checking for a GPKG Geometry
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString Name;
  char *sql;
  char xname[1024];
  int ret;
  int is_virtual = 0;

  if (table.StartsWith(wxT("vgpkg_")) == true)
    {
      wxString clean;
      table.StartsWith(wxT("vgpkg_"), &clean);
      table = clean;
      is_virtual = 1;
    }

  strcpy(xname, table.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT g.column_name, e.column_name FROM main.gpkg_geometry_columns AS g "
     "LEFT JOIN main.gpkg_extensions AS e ON (g.table_name LIKE e.table_name AND "
     "g.column_name LIKE e.column_name AND e.extension_name LIKE 'gpkg_rtree_index') "
     "WHERE Lower(g.table_name) = Lower(%Q)", xname);
  ret = sqlite3_get_table(handle, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CheckGPKG error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, MainFrame);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *name = results[(i * columns) + 0];
          wxString Name = wxString::FromUTF8(name);
          int rtree = 1;
          const char *spidx = results[(i * columns) + 1];
          if (spidx == NULL)
            rtree = 0;
          MyColumnInfo *elc = FirstColumn;
          while (elc)
            {
              if (Name.CmpNoCase(elc->GetName()) == 0)
                {
                  if (is_virtual)
                    elc->SetGPKGVirtualGeometry();
                  else
                    {
                      if (rtree)
                        elc->SetGPKGGeometryIndex();
                      else
                        elc->SetGPKGGeometry();
                    }
                  break;
                }
              elc = elc->GetNext();
            }
        }
    }
  sqlite3_free_table(results);
}

void MyTableInfo::CheckFdoOgr(MyFrame * MainFrame, sqlite3 * handle,
                              wxString & table)
{
//
// checking for an FDO/OGR Geometry
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString Name;
  char *sql;
  char xname[1024];
  int ret;
  int is_virtual = 0;

  if (table.StartsWith(wxT("fdo_")) == true)
    {
      wxString clean;
      table.StartsWith(wxT("fdo_"), &clean);
      table = clean;
      is_virtual = 1;
    }

  strcpy(xname, table.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT f_geometry_column, geometry_format FROM main.geometry_columns "
     "WHERE Lower(f_table_name) = Lower(%Q)", xname);
  ret = sqlite3_get_table(handle, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CheckFdoOgr error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, MainFrame);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *name = results[(i * columns) + 0];
          wxString Name = wxString::FromUTF8(name);
          MyColumnInfo *elc = FirstColumn;
          while (elc)
            {
              if (Name.CmpNoCase(elc->GetName()) == 0)
                {
                  if (is_virtual)
                    elc->SetFdoOgrVirtualGeometry();
                  else
                    elc->SetFdoOgrGeometry();
                  break;
                }
              elc = elc->GetNext();
            }
        }
    }
  sqlite3_free_table(results);
}

MyViewInfo::~MyViewInfo()
{
//
// destructor - a view columns collection
//
  MyColumnInfo *elc;
  MyColumnInfo *elcN;
  MyTriggerInfo *elt;
  MyTriggerInfo *eltN;
  elc = First;
  while (elc)
    {
      elcN = elc->GetNext();
      delete elc;
      elc = elcN;
    }
  elt = FirstTrigger;
  while (elt)
    {
      eltN = elt->GetNext();
      delete elt;
      elt = eltN;
    }
}

void MyViewInfo::AddColumn(wxString & name)
{
//
// inserting a column def into a view
//
  MyColumnInfo *el = new MyColumnInfo(name, false);
  if (!First)
    First = el;
  if (Last)
    Last->SetNext(el);
  Last = el;
}

void MyViewInfo::AddTrigger(wxString & name)
{
//
// inserting a trigger def into a view
//
  MyTriggerInfo *el = new MyTriggerInfo(name);
  if (!FirstTrigger)
    FirstTrigger = el;
  if (LastTrigger)
    LastTrigger->SetNext(el);
  LastTrigger = el;
}

void MyViewInfo::SetGeometry(wxString & name, bool index, bool cached)
{
//
//setting a geometry column
//
  MyColumnInfo *elc;
  elc = First;
  while (elc)
    {
      if (name.CmpNoCase(elc->GetName()) == 0)
        {
          elc->SetGeometry();
          if (index == true)
            elc->SetGeometryIndex();
          if (cached == true)
            elc->SetMbrCache();
        }
      elc = elc->GetNext();
    }
}

MySqlHistory::~MySqlHistory()
{
//
// destructor - the SQL queries history
//
  MySqlQuery *elq;
  MySqlQuery *elqN;
  elq = First;
  while (elq)
    {
      elqN = elq->GetNext();
      delete elq;
      elq = elqN;
    }
}

void MySqlHistory::Prepend(wxString & sql)
{
//
// inserting an SQL query into the history [reverse order]
//
  if (First)
    {
      // avoiding stupid duplicates
      if (First->GetSql() == sql)
        return;
    }
  MySqlQuery *el = new MySqlQuery(sql);
  el->SetNext(First);
  if (First)
    First->SetPrev(el);
  if (Last == NULL)
    Last = el;
  First = el;
  Current = Last;
}

void MySqlHistory::Add(wxString & sql)
{
//
// inserting an SQL query into the history
//
  if (Last)
    {
      // avoiding stupid duplicates
      if (Last->GetSql() == sql)
        return;
    }
  MySqlQuery *el = new MySqlQuery(sql);
  if (!First)
    First = el;
  el->SetPrev(Last);
  if (Last)
    Last->SetNext(el);
  Last = el;
  Current = el;
}

MySqlQuery *MySqlHistory::GetNext()
{
//
// return the next SQL query
//
  if (Current)
    {
      if (Current->GetNext())
        {
          Current = Current->GetNext();
          return Current;
      } else
        return NULL;
    }
  return NULL;
}

MySqlQuery *MySqlHistory::GetPrev()
{
//
// return the previous SQL query
//
  if (Current)
    {
      if (Current->GetPrev())
        {
          Current = Current->GetPrev();
          return Current;
      } else
        return NULL;
    }
  return NULL;
}

bool MySqlHistory::TestNext()
{
//
// tests if the next SQL query exists
//
  if (Current)
    {
      if (Current->GetNext())
        return true;
      else
        return false;
    }
  return false;
}

bool MySqlHistory::TestPrev()
{
//
// tests if the previous SQL query exists
//
  if (Current)
    {
      if (Current->GetPrev())
        return true;
      else
        return false;
    }
  return false;
}

AutoFDOTables::~AutoFDOTables()
{
//
// destructor - auto FDO-OGR wrapper linked list
//
  AutoFDOTable *el;
  AutoFDOTable *elN;
  el = First;
  while (el)
    {
      elN = el->GetNext();
      delete el;
      el = elN;
    }
}

void AutoFDOTables::Add(const char *name, const int len)
{
//
// adding a table name to the auto FDO-OGR wrapper linked list
//
  AutoFDOTable *el = new AutoFDOTable(name, len);
  if (!First)
    First = el;
  if (Last)
    Last->SetNext(el);
  Last = el;
}

AutoGPKGTables::~AutoGPKGTables()
{
//
// destructor - auto GPKG wrapper linked list
//
  AutoGPKGTable *el;
  AutoGPKGTable *elN;
  el = First;
  while (el)
    {
      elN = el->GetNext();
      delete el;
      el = elN;
    }
}

void AutoGPKGTables::Add(const char *name, const int len)
{
//
// adding a table name to the auto GPKG wrapper linked list
//
  AutoGPKGTable *el = new AutoGPKGTable(name, len);
  if (!First)
    First = el;
  if (Last)
    Last->SetNext(el);
  Last = el;
}

GeomColumn::GeomColumn(wxString & name, wxString & type, wxString & dims,
                       int srid, int idx)
{
// Geometry Column - constructor
  GeometryName = name;
  GeometryType = type;
  CoordDims = dims;
  SRID = srid;
  RTree = false;
  MbrCache = false;
  if (idx == 1)
    RTree = true;
  if (idx == 2)
    MbrCache = true;
  NotNull = false;
  Next = NULL;
}

GeomColsList::GeomColsList()
{
// Geometry Columns List - constructor
  First = NULL;
  Last = NULL;
}

GeomColsList::~GeomColsList()
{
// Geometry Columns List - destructor
  GeomColumn *pG;
  GeomColumn *pGn;
  pG = First;
  while (pG)
    {
      pGn = pG->GetNext();
      delete pG;
      pG = pGn;
    }
}

void GeomColsList::Add(wxString & name, wxString & type, wxString & dims,
                       int srid, int idx)
{
// adding a Geometry Column to the list
  GeomColumn *p = new GeomColumn(name, type, dims, srid, idx);
  if (!First)
    First = p;
  if (Last)
    Last->SetNext(p);
  Last = p;
}

void GeomColsList::SetNotNull(wxString & name)
{
// setting a NOT NULL geometry
  GeomColumn *pG = First;
  while (pG)
    {
      if (pG->GetGeometryName() == name)
        {
          pG->SetNotNull();
          return;
        }
      pG = pG->GetNext();
    }
}

IndexColumn::IndexColumn(wxString & name)
{
// Index Column - constructor
  ColumnName = name;
  Valid = true;
  Next = NULL;
}

TblIndex::TblIndex(wxString & name, bool unique)
{
// Table Index - constructor
  IndexName = name;
  Unique = unique;
  Valid = true;
  First = NULL;
  Last = NULL;
  Next = NULL;
}

TblIndex::~TblIndex()
{
// Table Index - destructor
  IndexColumn *pC;
  IndexColumn *pCn;
  pC = First;
  while (pC)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
}

void TblIndex::Add(wxString & name)
{
// adding a Column to the Index
  IndexColumn *p = new IndexColumn(name);
  if (!First)
    First = p;
  if (Last)
    Last->SetNext(p);
  Last = p;
}

void TblIndex::Invalidate(wxString & name)
{
// invalidating a column/index
  IndexColumn *pC = First;
  while (pC)
    {
      if (pC->GetColumnName() == name)
        {
          pC->Invalidate();
          Valid = false;
          return;
        }
      pC = pC->GetNext();
    }
}

TblIndexList::TblIndexList()
{
// Table Index List - constructor
  First = NULL;
  Last = NULL;
}

TblIndexList::~TblIndexList()
{
// Table Index List - destructor
  TblIndex *pI;
  TblIndex *pIn;
  pI = First;
  while (pI)
    {
      pIn = pI->GetNext();
      delete pI;
      pI = pIn;
    }
}

void TblIndexList::Add(wxString & name, bool unique)
{
// adding an Index to the List
  TblIndex *p = new TblIndex(name, unique);
  if (!First)
    First = p;
  if (Last)
    Last->SetNext(p);
  Last = p;
}

void TblIndexList::Invalidate(wxString & name)
{
// invalidating a column/index
  TblIndex *pI = First;
  while (pI)
    {
      pI->Invalidate(name);
      pI = pI->GetNext();
    }
}

MyChartData::MyChartData()
{
// constructor (unique values)
  Initialized = false;
  Array = NULL;
  First = NULL;
  Last = NULL;
  MaxFreq = INT_MIN;
  TotFreq = 0;
  MaxClasses = 0;
  NumClasses = 0;
  OtherUniquesFreq = 0;
  OtherUniquesCount = 0;
  Valid = false;
  ByIntervals = false;
}

MyChartData::~MyChartData()
{
// destructor
  MyChartUniqueClass *p;
  MyChartUniqueClass *pN;
  if (Array)
    delete[]Array;
  p = First;
  while (p)
    {
      pN = p->GetNext();
      delete p;
      p = pN;
    }
}

bool MyChartData::Check(bool by_interval, int classes)
{
// checking if current Data are still valid
  if (Initialized == false)
    return false;
  if (by_interval == true && Array == NULL)
    return false;
  if (by_interval == false && First == NULL)
    return false;
  if (by_interval == true && classes != NumClasses)
    return false;
  if (by_interval == false && classes != MaxClasses)
    return false;
  return true;
}

void MyChartData::CleanData()
{
// resetting to the initial (not initialized) state

  MyChartUniqueClass *p;
  MyChartUniqueClass *pN;
  if (Array)
    delete[]Array;
  p = First;
  while (p)
    {
      pN = p->GetNext();
      delete p;
      p = pN;
    }
  Initialized = false;
  Array = NULL;
  First = NULL;
  Last = NULL;
  MaxFreq = INT_MIN;
  TotFreq = 0;
  MaxClasses = 0;
  NumClasses = 0;
  OtherUniquesFreq = 0;
  OtherUniquesCount = 0;
  Valid = false;
  ByIntervals = false;
}

bool MyChartData::Create(int max_classes)
{
// constructor (unique values)
  if (Initialized == true)
    return false;
  MaxClasses = max_classes;
  Valid = false;
  ByIntervals = false;
  return true;
}

bool MyChartData::Create(double min, double max, int classes)
{
// constructor (interval values)
  int ind;
  double base = min;
  double step;
  if (Initialized == true)
    return false;
  Array = new MyChartIntervalClass[classes];
  Min = min;
  Max = max;
  NumClasses = classes;
  Valid = false;
  ByIntervals = true;
  step = (max - min) / (double) classes;
  for (ind = 0; ind < classes; ind++)
    {
      MyChartIntervalClass *p = GetClass(ind);
      p->Create(base, base + step);
      base += step;
    }
  return true;
}

MyChartIntervalClass *MyChartData::GetClass(int idx)
{
// retriving an interval class by index
  if (idx >= 0 && idx < NumClasses)
    return Array + idx;
  return NULL;
}

void MyChartData::Add(double value)
{
// incrementing the frequence count for an interval class
  double step = (Max - Min) / (double) NumClasses;
  int idx = (int) floor((value - Min) / step);
  if (idx >= 0 && idx < NumClasses)
    {
      MyChartIntervalClass *p = GetClass(idx);
      p->Add();
      TotFreq++;
      if (p->GetCount() > MaxFreq)
        MaxFreq = p->GetCount();
    }
}

void MyChartData::Add(wxString & value, int count)
{
// appending a new Unique Value (if limit not already reached)
  if (NumClasses < MaxClasses)
    {
      MyChartUniqueClass *p = new MyChartUniqueClass(value, count);
      if (First == NULL)
        First = p;
      if (Last != NULL)
        Last->SetNext(p);
      Last = p;
      NumClasses++;
      if (count > MaxFreq)
        MaxFreq = count;
  } else
    {
      OtherUniquesFreq += count;
      OtherUniquesCount++;
      if (OtherUniquesFreq > MaxFreq)
        MaxFreq = OtherUniquesFreq;
    }
  TotFreq += count;
}

MyChartScaleLabels::~MyChartScaleLabels()
{
// destructor
  MyChartScaleLabel *p;
  MyChartScaleLabel *pN;
  p = First;
  while (p)
    {
      pN = p->GetNext();
      delete p;
      p = pN;
    }
}

void MyChartScaleLabels::Initialize(double span, int max_freq)
{
// setting the Chart Scale labels
  int tic;
  int tic2;
  char text[128];
  double pos;

  if (max_freq < 20)
    {
      tic = 1;
      while (tic < max_freq)
        {
          sprintf(text, "%d", tic);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic++;
        }
  } else if (max_freq < 200)
    {
      tic = 10;
      while (tic < max_freq)
        {
          sprintf(text, "%d", tic);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic += 10;
        }
  } else if (max_freq < 2000)
    {
      tic = 100;
      while (tic < max_freq)
        {
          sprintf(text, "%d", tic);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic += 100;
        }
  } else if (max_freq < 20000)
    {
      tic = 1000;
      tic2 = 1;
      while (tic < max_freq)
        {
          sprintf(text, "%dK", tic2);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic += 1000;
          tic2++;
        }
  } else if (max_freq < 200000)
    {
      tic = 10000;
      tic2 = 1;
      while (tic < max_freq)
        {
          sprintf(text, "%d0K", tic2);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic += 10000;
          tic2++;
        }
  } else if (max_freq < 2000000)
    {
      tic = 100000;
      tic2 = 1;
      while (tic < max_freq)
        {
          sprintf(text, "%d00K", tic2);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic += 100000;
          tic2++;
        }
  } else if (max_freq < 20000000)
    {
      tic = 1000000;
      tic2 = 1;
      while (tic < max_freq)
        {
          sprintf(text, "%dM", tic2);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic += 1000000;
          tic2++;
        }
  } else if (max_freq < 200000000)
    {
      tic = 10000000;
      tic2 = 1;
      while (tic < max_freq)
        {
          sprintf(text, "%d0M", tic2);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic += 10000000;
          tic2++;
        }
  } else if (max_freq < 200000000)
    {
      tic = 100000000;
      tic2 = 1;
      while (tic < max_freq)
        {
          sprintf(text, "%d00M", tic2);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic += 100000000;
          tic2++;
        }
  } else
    {
      tic = 1000000000;
      tic2 = 1;
      while (tic < max_freq)
        {
          sprintf(text, "%dG", tic2);
          pos = span * ((double) tic / max_freq);
          Add(text, pos);
          tic += 1000000000;
          tic2++;
        }
    }
}

void MyChartScaleLabels::Add(const char *label, double pos)
{
// adding a Scale Label
  wxString text = wxString::FromUTF8(label);
  MyChartScaleLabel *p = new MyChartScaleLabel(text, pos);
  if (First == NULL)
    First = p;
  if (Last != NULL)
    Last->SetNext(p);
  Last = p;
}

MyPieChartLabels::MyPieChartLabels()
{
// constructor
  First = NULL;
  Last = NULL;
  NumLeftLabels = 0;
  LeftLabels = NULL;
  NumRightLabels = 0;
  RightLabels = NULL;
}

MyPieChartLabels::~MyPieChartLabels()
{
// destructor
  MyPieChartLabel *p;
  MyPieChartLabel *pN;
  p = First;
  while (p)
    {
      pN = p->GetNext();
      delete p;
      p = pN;
    }
  if (LeftLabels)
    delete[]LeftLabels;
  if (RightLabels)
    delete[]RightLabels;
}

void MyPieChartLabels::Add(const char *label, double x, double y)
{
// adding a PieChart Label
  wxString text = wxString::FromUTF8(label);
  MyPieChartLabel *p = new MyPieChartLabel(text, x, y);
  if (First == NULL)
    First = p;
  if (Last != NULL)
    Last->SetNext(p);
  Last = p;
}

void MyPieChartLabels::Sort(double cx)
{
// sorting the Left/Right arrays 
  MyPieChartLabel *p;
  MyPieChartLabel *p2;
  int idx;
  bool ok;

  if (LeftLabels)
    delete[]LeftLabels;
  if (RightLabels)
    delete[]RightLabels;
  NumLeftLabels = 0;
  LeftLabels = NULL;
  NumRightLabels = 0;
  RightLabels = NULL;

  p = First;
  while (p)
    {
      // counting how many 'left' labels are there
      if (p->GetX() < cx)
        NumLeftLabels++;
      p = p->GetNext();
    }
  p = First;
  while (p)
    {
      // counting how many 'right' labels are there
      if (p->GetX() >= cx)
        NumRightLabels++;
      p = p->GetNext();
    }
  if (NumLeftLabels > 0)
    {
      // allocating the Left array
      LeftLabels = new MyPieChartLabel *[NumLeftLabels];
      idx = 0;
      p = First;
      while (p)
        {
          // initializing the Left array
          if (p->GetX() < cx)
            {
              *(LeftLabels + idx) = p;
              idx++;
            }
          p = p->GetNext();
        }
      ok = true;
      while (ok)
        {
          // bubble-sorting the Left array
          ok = false;
          for (idx = 1; idx < NumLeftLabels; idx++)
            {
              p = *(LeftLabels + idx - 1);
              p2 = *(LeftLabels + idx);
              if (p->GetY() > p2->GetY())
                {
                  *(LeftLabels + idx - 1) = p2;
                  *(LeftLabels + idx) = p;
                  ok = true;
                }
            }
        }
    }
  if (NumRightLabels > 0)
    {
      // allocating the Right array
      RightLabels = new MyPieChartLabel *[NumRightLabels];
      idx = 0;
      p = First;
      while (p)
        {
          // initializing the Right array
          if (p->GetX() >= cx)
            {
              *(RightLabels + idx) = p;
              idx++;
            }
          p = p->GetNext();
        }
      ok = true;
      while (ok)
        {
          // bubble-sorting the Right array
          ok = false;
          for (idx = 1; idx < NumRightLabels; idx++)
            {
              p = *(RightLabels + idx - 1);
              p2 = *(RightLabels + idx);
              if (p->GetY() > p2->GetY())
                {
                  *(RightLabels + idx - 1) = p2;
                  *(RightLabels + idx) = p;
                  ok = true;
                }
            }
        }
    }
}

MyPieChartLabel *MyPieChartLabels::GetLeftLabel(int idx)
{
// return a Left Label pointer by index
  if (idx >= 0 && idx < NumLeftLabels)
    return *(LeftLabels + idx);
  return NULL;
}

MyPieChartLabel *MyPieChartLabels::GetRightLabel(int idx)
{
// return a Right Label pointer by index
  if (idx >= 0 && idx < NumRightLabels)
    return *(RightLabels + idx);
  return NULL;
}

TopoGeo::TopoGeo(MyTableTree * tree, wxTreeItemId & root,
                 wxString & name, int srid, bool has_z)
{
// constructor: TopoGeo Tree Node
  DbHandle = tree->GetSQLiteHandle();
  DbPrefix = wxT("");
  Name = name;
  wxString xname;
  char bufSrid[64];
  xname = Name;
  sprintf(bufSrid, " [SRID=%d]", srid);
  xname += wxString::FromUTF8(bufSrid);
  if (has_z == true)
    xname += wxT(" 3D");
  else
    xname += wxT(" 2D");
  TopologyNode = tree->AppendItem(root, xname);
  tree->SetItemImage(TopologyNode, 20);
  wxString empty = wxEmptyString;
  tree->SetItemData(TopologyNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_TOPO_GEO, empty, Name,
                                                 empty, empty)));
  Next = NULL;
}

TopoGeo::TopoGeo(MyTableTree * tree, wxTreeItemId & root,
                 wxString & db_prefix, wxString & name, int srid, bool has_z)
{
// constructor: TopoGeo Tree Node
  DbHandle = tree->GetSQLiteHandle();
  DbPrefix = db_prefix;
  Name = name;
  wxString xname;
  char bufSrid[64];
  xname = Name;
  sprintf(bufSrid, " [SRID=%d]", srid);
  xname += wxString::FromUTF8(bufSrid);
  if (has_z == true)
    xname += wxT(" 3D");
  else
    xname += wxT(" 2D");
  TopologyNode = tree->AppendItem(root, xname);
  tree->SetItemImage(TopologyNode, 20);
  wxString empty = wxEmptyString;
  tree->SetItemData(TopologyNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_TOPO_GEO, db_prefix, Name,
                                                 empty, empty)));
  Next = NULL;
}

bool TopoGeo::CheckTopoFeature(wxString & table)
{
// checking if some table is a TopoFeatures table
  char topo_name[1024];
  char *xprefix;
  char *ztable;
  char *xtable;
  char *sql;
  int ret;
  int i;
  char **results;
  int rows;
  int columns;

  bool ok = false;

  strcpy(topo_name, DbPrefix.ToUTF8());
  xprefix = gaiaDoubleQuotedSql(topo_name);
  strcpy(topo_name, Name.ToUTF8());
  ztable = sqlite3_mprintf("%s_topolayers", topo_name);
  xtable = gaiaDoubleQuotedSql(ztable);
  sqlite3_free(ztable);
  sql =
    sqlite3_mprintf("SELECT topolayer_id FROM \"%s\".\"%s\"", xprefix, xtable);
  free(xprefix);
  free(xtable);
  ret = sqlite3_get_table(DbHandle, sql, &results, &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return false;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *id = results[(i * columns) + 0];
          xtable = sqlite3_mprintf("%s_topofeatures_%s", topo_name, id);
          wxString cfr = wxString::FromUTF8(xtable);
          sqlite3_free(xtable);
          if (cfr.CmpNoCase(table) == 0)
            ok = true;
        }
    }
  sqlite3_free_table(results);
  return ok;
}

wxTreeItemId *TopoGeo::Check(wxString & table)
{
// checking if some table belongs to this TopoGeo
  wxString cfr = Name + wxT("_face");
  if (cfr.CmpNoCase(table) == 0)
    return &TopologyNode;
  cfr = Name + wxT("_edge");
  if (cfr.CmpNoCase(table) == 0)
    return &TopologyNode;
  cfr = Name + wxT("_node");
  if (cfr.CmpNoCase(table) == 0)
    return &TopologyNode;
  cfr = Name + wxT("_seeds");
  if (cfr.CmpNoCase(table) == 0)
    return &TopologyNode;
  cfr = Name + wxT("_edge_seeds");
  if (cfr.CmpNoCase(table) == 0)
    return &TopologyNode;
  cfr = Name + wxT("_face_seeds");
  if (cfr.CmpNoCase(table) == 0)
    return &TopologyNode;
  cfr = Name + wxT("_face_geoms");
  if (cfr.CmpNoCase(table) == 0)
    return &TopologyNode;
  cfr = Name + wxT("_topolayers");
  if (cfr.CmpNoCase(table) == 0)
    return &TopologyNode;
  cfr = Name + wxT("_topofeatures");
  if (cfr.CmpNoCase(table) == 0)
    return &TopologyNode;
  if (CheckTopoFeature(table) == true)
    return &TopologyNode;
  return NULL;
}

void TopoGeoList::Flush()
{
// resetting TopoGeoList to initial empty state
  TopoGeo *pT;
  TopoGeo *pTn;
  pT = First;
  while (pT)
    {
      pTn = pT->GetNext();
      delete pT;
      pT = pTn;
    }
  First = NULL;
  Last = NULL;
  Count = 0;
}

void TopoGeoList::Add(class MyTableTree * tree, wxTreeItemId & root,
                      wxString & name, int srid, bool has_z)
{
// inserting a TopoGeo into the list
  TopoGeo *pT = new TopoGeo(tree, root, name, srid, has_z);
  Count++;
  if (First == NULL)
    First = pT;
  if (Last != NULL)
    Last->SetNext(pT);
  Last = pT;
}

void TopoGeoList::Add(class MyTableTree * tree, wxTreeItemId & root,
                      wxString & db_prefix, wxString & name, int srid,
                      bool has_z)
{
// inserting a TopoGeo into the list
  TopoGeo *pT = new TopoGeo(tree, root, db_prefix, name, srid, has_z);
  Count++;
  if (First == NULL)
    First = pT;
  if (Last != NULL)
    Last->SetNext(pT);
  Last = pT;
}

wxTreeItemId *TopoGeoList::FindNode(wxString & table)
{
// searching corresponding TopoGeo Node (if any)
  TopoGeo *pT = First;
  while (pT)
    {
      wxTreeItemId *node = pT->Check(table);
      if (node != NULL)
        return node;
      pT = pT->GetNext();
    }
  return NULL;
}

TopoNet::TopoNet(MyTableTree * tree, wxTreeItemId & root,
                 wxString & name, bool spatial, int srid, bool has_z)
{
// constructor: TopoNet Tree Node
  DbPrefix = wxT("");
  Name = name;
  wxString xname;
  char bufSrid[64];
  xname = Name;
  if (spatial == false)
    xname += wxT(" - Logical Network");
  else
    {
      sprintf(bufSrid, " [SRID=%d]", srid);
      xname += wxString::FromUTF8(bufSrid);
      if (has_z == true)
        xname += wxT(" 3D");
      else
        xname += wxT(" 2D");
    }
  NetworkNode = tree->AppendItem(root, xname);
  tree->SetItemImage(NetworkNode, 25);
  wxString empty = wxEmptyString;
  tree->SetItemData(NetworkNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_TOPO_NET, empty, Name,
                                                 empty, empty)));
  Next = NULL;
}

TopoNet::TopoNet(MyTableTree * tree, wxTreeItemId & root,
                 wxString & dbPrefix, wxString & name, bool spatial, int srid,
                 bool has_z)
{
// constructor: TopoNet Tree Node
  DbPrefix = dbPrefix;
  Name = name;
  wxString xname;
  char bufSrid[64];
  xname = Name;
  if (spatial == false)
    xname += wxT(" - Logical Network");
  else
    {
      sprintf(bufSrid, " [SRID=%d]", srid);
      xname += wxString::FromUTF8(bufSrid);
      if (has_z == true)
        xname += wxT(" 3D");
      else
        xname += wxT(" 2D");
    }
  NetworkNode = tree->AppendItem(root, xname);
  tree->SetItemImage(NetworkNode, 25);
  wxString empty = wxEmptyString;
  tree->SetItemData(NetworkNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_TOPO_NET, DbPrefix, Name,
                                                 empty, empty)));
  Next = NULL;
}

wxTreeItemId *TopoNet::Check(wxString & table)
{
// checking if some table belongs to this TopoNet
  wxString cfr = Name + wxT("_link");
  if (cfr.CmpNoCase(table) == 0)
    return &NetworkNode;
  cfr = Name + wxT("_node");
  if (cfr.CmpNoCase(table) == 0)
    return &NetworkNode;
  cfr = Name + wxT("_seeds");
  if (cfr.CmpNoCase(table) == 0)
    return &NetworkNode;
  return NULL;
}

void TopoNetList::Flush()
{
// resetting TopoNetList to initial empty state
  TopoNet *pT;
  TopoNet *pTn;
  pT = First;
  while (pT)
    {
      pTn = pT->GetNext();
      delete pT;
      pT = pTn;
    }
  First = NULL;
  Last = NULL;
  Count = 0;
}

void TopoNetList::Add(class MyTableTree * tree, wxTreeItemId & root,
                      wxString & name, bool spatial, int srid, bool has_z)
{
// inserting a TopoNet into the list
  TopoNet *pT = new TopoNet(tree, root, name, spatial, srid, has_z);
  Count++;
  if (First == NULL)
    First = pT;
  if (Last != NULL)
    Last->SetNext(pT);
  Last = pT;
}

void TopoNetList::Add(class MyTableTree * tree, wxTreeItemId & root,
                      wxString & dbPrefix, wxString & name, bool spatial,
                      int srid, bool has_z)
{
// inserting a TopoNet into the list
  TopoNet *pT = new TopoNet(tree, root, dbPrefix, name, spatial, srid, has_z);
  Count++;
  if (First == NULL)
    First = pT;
  if (Last != NULL)
    Last->SetNext(pT);
  Last = pT;
}

wxTreeItemId *TopoNetList::FindNode(wxString & table)
{
// searching corresponding TopoNet Node (if any)
  TopoNet *pT = First;
  while (pT)
    {
      wxTreeItemId *node = pT->Check(table);
      if (node != NULL)
        return node;
      pT = pT->GetNext();
    }
  return NULL;
}

RasterCoverage::RasterCoverage(MyTableTree * tree, wxTreeItemId & root,
                               wxString & coverage, int srid)
{
// constructor: Raster Coverage Tree Node
  DbPrefix = wxT("");
  Name = coverage;
  wxString xname;
  char bufSrid[64];
  xname = Name;
  sprintf(bufSrid, " [SRID=%d]", srid);
  xname += wxString::FromUTF8(bufSrid);
  CoverageNode = tree->AppendItem(root, xname);
  tree->SetItemImage(CoverageNode, 22);
  wxString empty = wxEmptyString;
  tree->SetItemData(CoverageNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_RASTER_COVERAGE, empty,
                                                 coverage, empty, empty)));
  Next = NULL;
}

RasterCoverage::RasterCoverage(MyTableTree * tree, wxTreeItemId & root,
                               wxString & dbPrefix, wxString & coverage,
                               int srid)
{
// constructor: Raster Coverage Tree Node
  DbPrefix = dbPrefix;
  Name = coverage;
  wxString xname;
  char bufSrid[64];
  xname = Name;
  sprintf(bufSrid, " [SRID=%d]", srid);
  xname += wxString::FromUTF8(bufSrid);
  CoverageNode = tree->AppendItem(root, xname);
  tree->SetItemImage(CoverageNode, 22);
  wxString empty = wxEmptyString;
  tree->SetItemData(CoverageNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_RASTER_COVERAGE, dbPrefix,
                                                 coverage, empty, empty)));
  Next = NULL;
}

VectorCoverage::VectorCoverage(MyTableTree * tree, wxTreeItemId & root,
                               wxString & prefix, wxString & coverage, int srid,
                               int type)
{
// constructor: Vector Coverage Tree Node
  DbPrefix = wxT("");
  Name = coverage;
  wxString xname;
  char bufSrid[64];
  xname = prefix + wxT(".") + Name;
  sprintf(bufSrid, " [SRID=%d]", srid);
  xname += wxString::FromUTF8(bufSrid);
  CoverageNode = tree->AppendItem(root, xname);
  int index = 24;
  switch (type)
    {
      case 1:
      case 1001:
      case 2001:
      case 3001:
      case 4:
      case 1004:
      case 2004:
      case 3004:
        index = 27;
        break;
      case 2:
      case 1002:
      case 2002:
      case 3002:
      case 5:
      case 1005:
      case 2005:
      case 3005:
        index = 28;
        break;
      case 3:
      case 1003:
      case 2003:
      case 3003:
      case 6:
      case 1006:
      case 2006:
      case 3006:
        index = 29;
        break;
      default:
        index = 24;
        break;
    };
  if (prefix.IsSameAs(wxT("topogeo")))
    index = 20;
  if (prefix.IsSameAs(wxT("toponet")))
    index = 25;
  tree->SetItemImage(CoverageNode, index);
  wxString empty = wxEmptyString;
  tree->SetItemData(CoverageNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_VECTOR_COVERAGE, empty,
                                                 coverage, empty, empty)));
  Next = NULL;
}

VectorCoverage::VectorCoverage(MyTableTree * tree, wxTreeItemId & root,
                               wxString & dbPrefix, wxString & prefix,
                               wxString & coverage, int srid, int type)
{
// constructor: Vector Coverage Tree Node
  DbPrefix = dbPrefix;
  Name = coverage;
  wxString xname;
  char bufSrid[64];
  xname = prefix + wxT(".") + Name;
  sprintf(bufSrid, " [SRID=%d]", srid);
  xname += wxString::FromUTF8(bufSrid);
  CoverageNode = tree->AppendItem(root, xname);
  int index = 24;
  switch (type)
    {
      case 1:
      case 1001:
      case 2001:
      case 3001:
      case 4:
      case 1004:
      case 2004:
      case 3004:
        index = 27;
        break;
      case 2:
      case 1002:
      case 2002:
      case 3002:
      case 5:
      case 1005:
      case 2005:
      case 3005:
        index = 28;
        break;
      case 3:
      case 1003:
      case 2003:
      case 3003:
      case 6:
      case 1006:
      case 2006:
      case 3006:
        index = 29;
        break;
      default:
        index = 24;
        break;
    };
  if (prefix.IsSameAs(wxT("topogeo")))
    index = 20;
  if (prefix.IsSameAs(wxT("toponet")))
    index = 25;
  tree->SetItemImage(CoverageNode, index);
  wxString empty = wxEmptyString;
  tree->SetItemData(CoverageNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_VECTOR_COVERAGE, dbPrefix,
                                                 coverage, empty, empty)));
  Next = NULL;
}

WmsLayer::WmsLayer(MyTableTree * tree, wxTreeItemId & root,
                   wxString & layer_name, wxString & url)
{
// constructor: WMS Layer Tree Node
  DbPrefix = wxT("");
  LayerName = layer_name;
  URL = url;
  CoverageNode = tree->AppendItem(root, LayerName);
  tree->SetItemImage(CoverageNode, 26);
  wxString empty = wxEmptyString;
  tree->SetItemData(CoverageNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_WMS_LAYER, empty,
                                                 layer_name, empty, url)));
  Next = NULL;
}

WmsLayer::WmsLayer(MyTableTree * tree, wxTreeItemId & root, wxString & dbPrefix,
                   wxString & layer_name, wxString & url)
{
// constructor: WMS Layer Tree Node
  DbPrefix = dbPrefix;
  LayerName = layer_name;
  URL = url;
  CoverageNode = tree->AppendItem(root, LayerName);
  tree->SetItemImage(CoverageNode, 26);
  wxString empty = wxEmptyString;
  tree->SetItemData(CoverageNode,
                    (wxTreeItemData *) (new
                                        MyObject(MY_WMS_LAYER, dbPrefix,
                                                 layer_name, empty, url)));
  Next = NULL;
}

wxTreeItemId *RasterCoverage::Check(wxString & table, bool *tile_data)
{
// checking if some table belongs to this Raster Coverage
  *tile_data = false;
  wxString cfr = Name + wxT("_sections");
  if (cfr.CmpNoCase(table) == 0)
    return &CoverageNode;
  cfr = Name + wxT("_tiles");
  if (cfr.CmpNoCase(table) == 0)
    return &CoverageNode;
  cfr = Name + wxT("_tile_data");
  if (cfr.CmpNoCase(table) == 0)
    {
      *tile_data = true;
      return &CoverageNode;
    }
  cfr = Name + wxT("_levels");
  if (cfr.CmpNoCase(table) == 0)
    return &CoverageNode;
  cfr = Name + wxT("_section_levels");
  if (cfr.CmpNoCase(table) == 0)
    return &CoverageNode;
  return NULL;
}

void RasterCoverageList::Flush()
{
// resetting RasterCovreageList to initial empty state
  RasterCoverage *pC;
  RasterCoverage *pCn;
  pC = First;
  while (pC)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
  First = NULL;
  Last = NULL;
  Count = 0;
}

void RasterCoverageList::Add(class MyTableTree * tree, wxTreeItemId & root,
                             wxString & coverage, int srid)
{
// inserting a Raster Coverage into the list
  RasterCoverage *pC = new RasterCoverage(tree, root, coverage, srid);
  Count++;
  if (First == NULL)
    First = pC;
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
}

void RasterCoverageList::Add(class MyTableTree * tree, wxTreeItemId & root,
                             wxString & dbPrefix, wxString & coverage, int srid)
{
// inserting a Raster Coverage into the list
  RasterCoverage *pC = new RasterCoverage(tree, root, dbPrefix, coverage, srid);
  Count++;
  if (First == NULL)
    First = pC;
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
}

wxTreeItemId *RasterCoverageList::FindNode(wxString & table, bool *tile_data)
{
// searching corresponding RasterCoverage Node (if any)
  RasterCoverage *pC = First;
  while (pC)
    {
      wxTreeItemId *node = pC->Check(table, tile_data);
      if (node != NULL)
        return node;
      pC = pC->GetNext();
    }
  *tile_data = false;
  return NULL;
}

void VectorCoverageList::Flush()
{
// resetting VectorCovreageList to initial empty state
  VectorCoverage *pC;
  VectorCoverage *pCn;
  pC = First;
  while (pC)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
  First = NULL;
  Last = NULL;
  Count = 0;
}

void VectorCoverageList::Add(class MyTableTree * tree, wxTreeItemId & root,
                             wxString & prefix, wxString & coverage, int srid,
                             int type)
{
// inserting a Vector Coverage into the list
  VectorCoverage *pC =
    new VectorCoverage(tree, root, prefix, coverage, srid, type);
  Count++;
  if (First == NULL)
    First = pC;
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
}

void VectorCoverageList::Add(class MyTableTree * tree, wxTreeItemId & root,
                             wxString & dbPrefix, wxString & prefix,
                             wxString & coverage, int srid, int type)
{
// inserting a Vector Coverage into the list
  VectorCoverage *pC =
    new VectorCoverage(tree, root, dbPrefix, prefix, coverage, srid, type);
  Count++;
  if (First == NULL)
    First = pC;
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
}

void WmsLayerList::Flush()
{
// resetting WmsLayerList to initial empty state
  WmsLayer *pW;
  WmsLayer *pWn;
  pW = First;
  while (pW)
    {
      pWn = pW->GetNext();
      delete pW;
      pW = pWn;
    }
  First = NULL;
  Last = NULL;
  Count = 0;
}

void WmsLayerList::Add(class MyTableTree * tree, wxTreeItemId & root,
                       wxString & layer_name, wxString & url)
{
// inserting a WMS Layer into the list
  WmsLayer *pW = new WmsLayer(tree, root, layer_name, url);
  Count++;
  if (First == NULL)
    First = pW;
  if (Last != NULL)
    Last->SetNext(pW);
  Last = pW;
}

void WmsLayerList::Add(class MyTableTree * tree, wxTreeItemId & root,
                       wxString & dbPrefix, wxString & layer_name,
                       wxString & url)
{
// inserting a WMS Layer into the list
  WmsLayer *pW = new WmsLayer(tree, root, dbPrefix, layer_name, url);
  Count++;
  if (First == NULL)
    First = pW;
  if (Last != NULL)
    Last->SetNext(pW);
  Last = pW;
}

PostGISColumn::PostGISColumn()
{
// constructor
  Nullable = true;
  PrimaryKey = false;
  Autoincrement = false;
  Null = 0;
  Boolean = 0;
  Int8 = 0;
  UInt8 = 0;
  Int16 = 0;
  UInt16 = 0;
  Int32 = 0;
  UInt32 = 0;
  Int64 = 0;
  Double = 0;
  Text = 0;
  MaxTextLen = 0;
  Date = 0;
  DateTime = 0;
  Blob = 0;
  Point = 0;
  MultiPoint = 0;
  LineString = 0;
  MultiLineString = 0;
  Polygon = 0;
  MultiPolygon = 0;
  GeometryCollection = 0;
  Srid1 = INT_MIN;
  Srid2 = INT_MIN;;
  CoordDims1 = GAIA_UNKNOWN;
  CoordDims2 = GAIA_UNKNOWN;
  DataType = PostGISHelper::DATA_TYPE_UNDEFINED;
}

void PostGISColumn::IncrPoint(int srid, int dims)
{
// increasing POINT statistics
  Point++;
  if (srid == Srid1 || srid == Srid2)
    ;
  else
    {
      if (Srid1 == INT_MIN)
        Srid1 = srid;
      else if (Srid2 == INT_MIN)
        Srid2 = srid;
    }
  if (dims == CoordDims1 || dims == CoordDims2)
    ;
  else
    {
      if (CoordDims1 == GAIA_UNKNOWN)
        CoordDims1 = dims;
      else if (CoordDims2 == GAIA_UNKNOWN)
        CoordDims2 = dims;
    }
}

void PostGISColumn::IncrMultiPoint(int srid, int dims)
{
// increasing MULTIPOINT statistics
  MultiPoint++;
  if (srid == Srid1 || srid == Srid2)
    ;
  else
    {
      if (Srid1 == INT_MIN)
        Srid1 = srid;
      else if (Srid2 == INT_MIN)
        Srid2 = srid;
    }
  if (dims == CoordDims1 || dims == CoordDims2)
    ;
  else
    {
      if (CoordDims1 == GAIA_UNKNOWN)
        CoordDims1 = dims;
      else if (CoordDims2 == GAIA_UNKNOWN)
        CoordDims2 = dims;
    }
}

void PostGISColumn::IncrLineString(int srid, int dims)
{
// increasing LINESTRING statistics
  LineString++;
  if (srid == Srid1 || srid == Srid2)
    ;
  else
    {
      if (Srid1 == INT_MIN)
        Srid1 = srid;
      else if (Srid2 == INT_MIN)
        Srid2 = srid;
    }
  if (dims == CoordDims1 || dims == CoordDims2)
    ;
  else
    {
      if (CoordDims1 == GAIA_UNKNOWN)
        CoordDims1 = dims;
      else if (CoordDims2 == GAIA_UNKNOWN)
        CoordDims2 = dims;
    }
}

void PostGISColumn::IncrMultiLineString(int srid, int dims)
{
// increasing MULTILINESTRING statistics
  MultiLineString++;
  if (srid == Srid1 || srid == Srid2)
    ;
  else
    {
      if (Srid1 == INT_MIN)
        Srid1 = srid;
      else if (Srid2 == INT_MIN)
        Srid2 = srid;
    }
  if (dims == CoordDims1 || dims == CoordDims2)
    ;
  else
    {
      if (CoordDims1 == GAIA_UNKNOWN)
        CoordDims1 = dims;
      else if (CoordDims2 == GAIA_UNKNOWN)
        CoordDims2 = dims;
    }
}

void PostGISColumn::IncrPolygon(int srid, int dims)
{
// increasing POLYGON statistics
  Polygon++;
  if (srid == Srid1 || srid == Srid2)
    ;
  else
    {
      if (Srid1 == INT_MIN)
        Srid1 = srid;
      else if (Srid2 == INT_MIN)
        Srid2 = srid;
    }
  if (dims == CoordDims1 || dims == CoordDims2)
    ;
  else
    {
      if (CoordDims1 == GAIA_UNKNOWN)
        CoordDims1 = dims;
      else if (CoordDims2 == GAIA_UNKNOWN)
        CoordDims2 = dims;
    }
}

void PostGISColumn::IncrMultiPolygon(int srid, int dims)
{
// increasing MULTIPOLYGON statistics
  MultiPolygon++;
  if (srid == Srid1 || srid == Srid2)
    ;
  else
    {
      if (Srid1 == INT_MIN)
        Srid1 = srid;
      else if (Srid2 == INT_MIN)
        Srid2 = srid;
    }
  if (dims == CoordDims1 || dims == CoordDims2)
    ;
  else
    {
      if (CoordDims1 == GAIA_UNKNOWN)
        CoordDims1 = dims;
      else if (CoordDims2 == GAIA_UNKNOWN)
        CoordDims2 = dims;
    }
}

void PostGISColumn::IncrGeometryCollection(int srid, int dims)
{
// increasing GEOMETRYCOLLECTION statistics
  GeometryCollection++;
  if (srid == Srid1 || srid == Srid2)
    ;
  else
    {
      if (Srid1 == INT_MIN)
        Srid1 = srid;
      else if (Srid2 == INT_MIN)
        Srid2 = srid;
    }
  if (dims == CoordDims1 || dims == CoordDims2)
    ;
  else
    {
      if (CoordDims1 == GAIA_UNKNOWN)
        CoordDims1 = dims;
      else if (CoordDims2 == GAIA_UNKNOWN)
        CoordDims2 = dims;
    }
}

bool PostGISColumn::IsDate(const char *txt)
{
// checking if a text string looks like a DATE
  if (txt == NULL)
    return false;
  if (strlen(txt) != 10)
    return false;
  if (*(txt + 0) >= '0' && *(txt + 0) <= '9')
    ;
  else
    return false;
  if (*(txt + 1) >= '0' && *(txt + 1) <= '9')
    ;
  else
    return false;
  if (*(txt + 2) >= '0' && *(txt + 2) <= '9')
    ;
  else
    return false;
  if (*(txt + 3) >= '0' && *(txt + 3) <= '9')
    ;
  else
    return false;
  if (*(txt + 4) != '-')
    return false;
  if (*(txt + 5) >= '0' && *(txt + 5) <= '9')
    ;
  else
    return false;
  if (*(txt + 6) >= '0' && *(txt + 6) <= '9')
    ;
  else
    return false;
  if (*(txt + 7) != '-')
    return false;
  if (*(txt + 8) >= '0' && *(txt + 8) <= '9')
    ;
  else
    return false;
  if (*(txt + 9) >= '0' && *(txt + 9) <= '9')
    ;
  else
    return false;
  return true;
}

bool PostGISColumn::IsDateTime(const char *txt)
{
// checking if a text string looks like a DATETIME
  if (txt == NULL)
    return false;
  if (strlen(txt) != 19)
    return false;
  if (*(txt + 0) >= '0' && *(txt + 0) <= '9')
    ;
  else
    return false;
  if (*(txt + 1) >= '0' && *(txt + 1) <= '9')
    ;
  else
    return false;
  if (*(txt + 2) >= '0' && *(txt + 2) <= '9')
    ;
  else
    return false;
  if (*(txt + 3) >= '0' && *(txt + 3) <= '9')
    ;
  else
    return false;
  if (*(txt + 4) != '-')
    return false;
  if (*(txt + 5) >= '0' && *(txt + 5) <= '9')
    ;
  else
    return false;
  if (*(txt + 6) >= '0' && *(txt + 6) <= '9')
    ;
  else
    return false;
  if (*(txt + 7) != '-')
    return false;
  if (*(txt + 8) >= '0' && *(txt + 8) <= '9')
    ;
  else
    return false;
  if (*(txt + 9) >= '0' && *(txt + 9) <= '9')
    ;
  else
    return false;
  if (*(txt + 10) != ' ')
    return false;
  if (*(txt + 11) >= '0' && *(txt + 11) <= '9')
    ;
  else
    return false;
  if (*(txt + 12) >= '0' && *(txt + 12) <= '9')
    ;
  else
    return false;
  if (*(txt + 13) != ':')
    return false;
  if (*(txt + 14) >= '0' && *(txt + 14) <= '9')
    ;
  else
    return false;
  if (*(txt + 15) >= '0' && *(txt + 15) <= '9')
    ;
  else
    return false;
  if (*(txt + 16) != ':')
    return false;
  if (*(txt + 17) >= '0' && *(txt + 17) <= '9')
    ;
  else
    return false;
  if (*(txt + 18) >= '0' && *(txt + 18) <= '9')
    ;
  else
    return false;
  return true;
}

void PostGISColumn::Prepare()
{
// determining the exact Column Data Type
  if (!Boolean && !Int8 && !UInt8 && !Int16 && !UInt16 && !Int32 && !UInt32
      && !Int64 && !Double && !Text && !Date && !DateTime && !Blob)
    {
      // testing for Geometry
      int type = PostGISHelper::DATA_TYPE_UNDEFINED;
      if ((Point + MultiPoint + LineString + MultiLineString + Polygon +
           MultiPolygon + GeometryCollection) > 0)
        type = PostGISHelper::DATA_TYPE_GEOMETRY;
      else
        {
          // surely not a Geometry: defaulting to TEXT
          DataType = PostGISHelper::DATA_TYPE_TEXT;
          return;
        }
      if (Point > 0 && !MultiPoint && !LineString && !MultiLineString
          && !Polygon && !MultiPolygon && !GeometryCollection)
        type = PostGISHelper::DATA_TYPE_POINT;
      if (MultiPoint > 0 && !LineString && !MultiLineString && !Polygon
          && !MultiPolygon && !GeometryCollection)
        type = PostGISHelper::DATA_TYPE_MULTIPOINT;
      if (!Point && !MultiPoint && LineString > 0 && !MultiLineString
          && !Polygon && !MultiPolygon && !GeometryCollection)
        type = PostGISHelper::DATA_TYPE_LINESTRING;
      if (!Point && !MultiPoint && MultiLineString > 0 && !Polygon
          && !MultiPolygon && !GeometryCollection)
        type = PostGISHelper::DATA_TYPE_MULTILINESTRING;
      if (!Point && !MultiPoint && !LineString && !MultiLineString
          && Polygon > 0 && !MultiPolygon && !GeometryCollection)
        type = PostGISHelper::DATA_TYPE_POLYGON;
      if (!Point && !MultiPoint && !LineString && !MultiLineString
          && MultiPolygon > 0 && !GeometryCollection)
        type = PostGISHelper::DATA_TYPE_MULTIPOLYGON;
      if (!Point && !MultiPoint && !LineString && !MultiLineString && !Polygon
          && !MultiPolygon && GeometryCollection > 0)
        type = PostGISHelper::DATA_TYPE_GEOMETRYCOLLECTION;
      if (type != PostGISHelper::DATA_TYPE_UNDEFINED)
        {
          if (Srid2 == INT_MIN && CoordDims2 == GAIA_UNKNOWN)
            {
              // ok, valid Geometry
              DataType = type;
              return;
          } else
            {
              // no, invalid Geometry
              DataType = PostGISHelper::DATA_TYPE_BLOB;
              return;
            }
        }
      DataType = PostGISHelper::DATA_TYPE_BLOB;
      return;
    }

  if (Boolean > 0 && !Int8 && !UInt8 && !Int16 && !UInt16 && !Int32 && !UInt32
      && !Int64 && !Double && !Text && !Date && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_BOOLEAN;
      return;
    }
  if ((Int8 > 0 && !UInt8) && !Int16 && !UInt16 && !Int32 && !UInt32 && !Int64
      && !Double && !Text && !Date && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_INT8;
      return;
    }
  if (UInt8 > 0 && !Int16 && !UInt16 && !Int32 && !UInt32 && !Int64 && !Double
      && !Text && !Date && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_UINT8;
      return;
    }
  if ((Int16 > 0 && !UInt16) && !Int32 && !UInt32 && !Int64 && !Double && !Text
      && !Date && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_INT16;
      return;
    }
  if (UInt16 > 0 && !Int32 && !UInt32 && !Int64 && !Double && !Text && !Date
      && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_UINT16;
      return;
    }
  if ((Int32 > 0 && !UInt32) && !Int64 && !Double && !Text && !Date && !DateTime
      && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_INT32;
      return;
    }
  if (UInt32 > 0 && !Int64 && !Double && !Text && !Date && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_UINT32;
      return;
    }
  if (Int64 > 0 && !Double && !Text && !Date && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_INT64;
      return;
    }
  if (Double > 0 && !Text && !Date && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_DOUBLE;
      return;
    }
  if (!Boolean && !Int8 && !UInt8 && !Int16 && !UInt16 && !Int32 && !UInt32
      && !Int64 && !Double && Text > 0 && !Date && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_TEXT;
      return;
    }
  if (!Boolean && !Int8 && !UInt8 && !Int16 && !UInt16 && !Int32 && !UInt32
      && !Int64 && !Double && !Text && Date > 0 && !DateTime && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_DATE;
      return;
    }
  if (!Boolean && !Int8 && !UInt8 && !Int16 && !UInt16 && !Int32 && !UInt32
      && !Int64 && !Double && !Text && DateTime > 0 && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_DATETIME;
      return;
    }
  if (!Boolean && !Int8 && !UInt8 && !Int16 && !UInt16 && !Int32 && !UInt32
      && !Int64 && !Double && !Text && !Date && !DateTime && Blob > 0)
    {
      DataType = PostGISHelper::DATA_TYPE_BLOB;
      return;
    }
  if ((Boolean + Int8 + UInt8 + Int16 + UInt16 + Int32 + UInt32 + Int64 +
       Double + Text + Date + DateTime) > 0 && !Blob)
    {
      DataType = PostGISHelper::DATA_TYPE_TEXT;
      return;
    }
  if (Null > 0)
    Nullable = true;
// unable to establish a valid Data Type
  DataType = PostGISHelper::DATA_TYPE_UNDEFINED;
}

bool PostGISColumn::IsGeometry()
{
// does this column corresponds to some Geometry ?
  switch (DataType)
    {
      case PostGISHelper::DATA_TYPE_POINT:
      case PostGISHelper::DATA_TYPE_LINESTRING:
      case PostGISHelper::DATA_TYPE_POLYGON:
      case PostGISHelper::DATA_TYPE_MULTIPOINT:
      case PostGISHelper::DATA_TYPE_MULTILINESTRING:
      case PostGISHelper::DATA_TYPE_MULTIPOLYGON:
      case PostGISHelper::DATA_TYPE_GEOMETRYCOLLECTION:
      case PostGISHelper::DATA_TYPE_GEOMETRY:
        return true;
    }
  return false;
}

PostGISIndex::~PostGISIndex()
{
// destructor
  PostGISIndexField *pI;
  PostGISIndexField *pIn;
  pI = First;
  while (pI)
    {
      pIn = pI->GetNext();
      delete pI;
      pI = pIn;
    }
}

void PostGISIndex::AddField(int seq, PostGISColumn * col)
{
// adding a Field into this Index
  PostGISIndexField *field = new PostGISIndexField(seq, col);
  if (First == NULL)
    First = field;
  if (Last != NULL)
    Last->SetNext(field);
  Last = field;
}

PostGISHelper::PostGISHelper()
{
// constructor
  DumbName = wxT("DumbColumn");
  Count = 0;
  Columns = NULL;
  FirstIdx = NULL;
  LastIdx = NULL;
  Autoincrement = false;
}

PostGISHelper::~PostGISHelper()
{
// destructor
  PostGISIndex *pI;
  PostGISIndex *pIn;
  if (Columns != NULL)
    delete[]Columns;
  pI = FirstIdx;
  while (pI)
    {
      pIn = pI->GetNext();
      delete pI;
      pI = pIn;
    }
}

void PostGISHelper::Alloc(int count)
{
//
// allocating the Columns array
//
  Count = count;
  if (Columns != NULL)
    delete[]Columns;
  Columns = new PostGISColumn[Count];
}

void PostGISHelper::SetName(int pos, const char *name)
{
//
// setting a Column Name
//
  if (pos >= 0 && pos < Count)
    {
      wxString Name = wxString::FromUTF8(name);
      Columns[pos].SetName(Name);
    }
}

void PostGISHelper::Eval(int pos, sqlite3_int64 val)
{
//
// evaluating an INT value
//
  if (pos >= 0 && pos < Count)
    {
      bool skip = false;
      if (val == 1 || val == 0)
        {
          Columns[pos].IncrBoolean();
          skip = true;
        }
      if (!skip)
        {
          if (val >= SCHAR_MIN && val <= SCHAR_MAX)
            {
              Columns[pos].IncrInt8();
              skip = true;
            }
          if (val > SCHAR_MAX && val <= UCHAR_MAX)
            {
              Columns[pos].IncrUInt8();
              skip = true;
            }
        }
      if (!skip)
        {
          if (val >= SHRT_MIN && val <= SHRT_MAX)
            {
              Columns->IncrInt16();
              skip = true;
            }
          if (val > SHRT_MAX && val <= USHRT_MAX)
            {
              Columns[pos].IncrUInt16();
              skip = true;
            }
        }
      if (!skip)
        {
          if (val >= INT_MIN && val <= INT_MAX)
            {
              Columns[pos].IncrInt32();
              skip = true;
            }
          if (val > INT_MAX && val <= UINT_MAX)
            {
              Columns[pos].IncrUInt32();
              skip = true;
            }
        }
      if (!skip)
        Columns[pos].IncrInt64();
    }
}

void PostGISHelper::Eval(int pos, double val)
{
//
// evaluating a DOUBLE value
//
  if (val > 0)
    val = 0.0;                  // suppressing stupid warnings
  if (pos >= 0 && pos < Count)
    Columns[pos].IncrDouble();
}

void PostGISHelper::Eval(int pos, const char *val)
{
//
// evaluating a TEXT value
//
  if (pos >= 0 && pos < Count)
    {
      int len = strlen(val);
      if (Columns[pos].IsDate(val) == true)
        Columns[pos].IncrDate();
      else if (Columns[pos].IsDateTime(val) == true)
        Columns[pos].IncrDateTime();
      else
        Columns[pos].IncrText(len);
    }
}

void PostGISHelper::Eval(int pos, gaiaGeomCollPtr geom)
{
//
// evaluating a Geometry value
//
  if (pos >= 0 && pos < Count)
    {
      switch (geom->DeclaredType)
        {
          case GAIA_POINT:
            Columns[pos].IncrPoint(geom->Srid, geom->DimensionModel);
            break;
          case GAIA_LINESTRING:
            Columns[pos].IncrLineString(geom->Srid, geom->DimensionModel);
            break;
          case GAIA_POLYGON:
            Columns[pos].IncrPolygon(geom->Srid, geom->DimensionModel);
            break;
          case GAIA_MULTIPOINT:
            Columns[pos].IncrMultiPoint(geom->Srid, geom->DimensionModel);
            break;
          case GAIA_MULTILINESTRING:
            Columns[pos].IncrMultiLineString(geom->Srid, geom->DimensionModel);
            break;
          case GAIA_MULTIPOLYGON:
            Columns[pos].IncrMultiPolygon(geom->Srid, geom->DimensionModel);
            break;
          case GAIA_GEOMETRYCOLLECTION:
            Columns[pos].IncrGeometryCollection(geom->Srid,
                                                geom->DimensionModel);
            break;
        };
    }
}

void PostGISHelper::EvalBlob(int pos)
{
//
// evaluating a BLOB value
//
  if (pos >= 0 && pos < Count)
    Columns[pos].IncrBlob();
}

void PostGISHelper::Eval(int pos)
{
//
// evaluating a NULL value
//
  if (pos >= 0 && pos < Count)
    Columns[pos].IncrNull();
}

wxString & PostGISHelper::GetName(int pos, bool to_lower)
{
//
// return a Column name [by relative position]
//
  wxString name;
  if (pos >= 0 && pos < Count)
    name = Columns[pos].GetName();
  else
    name = wxT("DumbColumn");
  if (to_lower)
    DumbName = name.MakeLower();
  else
    DumbName = name;
  return DumbName;
}

void PostGISHelper::Prepare()
{
//
// setting PostGIS data types for each Column
//
  int i;
  for (i = 0; i < Count; i++)
    Columns[i].Prepare();
}

bool PostGISHelper::IsSingleFieldPrimaryKey()
{
// testing if there is a single column PK
  int count = 0;
  int i;
  for (i = 0; i < Count; i++)
    {
      if (Columns[i].IsPrimaryKey() == true)
        count++;
    }
  if (count == 1)
    return true;
  return false;
}

bool PostGISHelper::IsGeometry(int pos)
{
//
// checking if a column is of any Geometry type
//
  if (pos >= 0 && pos < Count)
    return Columns[pos].IsGeometry();
  return false;
}

int PostGISHelper::GetDataType(int pos)
{
//
// return the data type for some column
//
  if (pos >= 0 && pos < Count)
    return Columns[pos].GetDataType();
  return DATA_TYPE_UNDEFINED;
}

int PostGISHelper::GetSrid(int pos)
{
//
// return the SRID for some Geometry column
//
  if (pos >= 0 && pos < Count)
    return Columns[pos].GetSrid();
  return -1;
}

int PostGISHelper::GetCoordDims(int pos)
{
//
// return the Coord Dimensions for some Geometry column
//
  if (pos >= 0 && pos < Count)
    return Columns[pos].GetCoordDims();
  return GAIA_XY;
}

void PostGISHelper::SetColumn(wxString & name, bool isNull, bool isPKey)
{
//
// setting up Column params
//
  int i;
  for (i = 0; i < Count; i++)
    {
      if (Columns[i].GetName() == name)
        {
          if (isNull)
            Columns[i].SetNotNull();
          if (isPKey)
            {
              Columns[i].SetPrimaryKey();
            }
          break;
        }
    }
}

void PostGISHelper::GetDataType(int pos, char *definition)
{
//
// return the data definition for some column
//
  if (pos >= 0 && pos < Count)
    {
      int len;
      int data_type = Columns[pos].GetDataType();
      switch (data_type)
        {
          case DATA_TYPE_BOOLEAN:
          case DATA_TYPE_INT8:
          case DATA_TYPE_UINT8:
          case DATA_TYPE_INT16:
            strcpy(definition, "smallint");
            break;
          case DATA_TYPE_UINT16:
          case DATA_TYPE_INT32:
            strcpy(definition, "integer");
            break;
          case DATA_TYPE_UINT32:
          case DATA_TYPE_INT64:
            strcpy(definition, "bigint");
            break;
          case DATA_TYPE_DOUBLE:
            strcpy(definition, "double precision");
            break;
          case DATA_TYPE_DATE:
            strcpy(definition, "date");
            break;
          case DATA_TYPE_DATETIME:
            strcpy(definition, "timestamp");
            break;
          case DATA_TYPE_TEXT:
            len = Columns[pos].GetMaxTextLen();
            if (len <= 255)
              {
                sprintf(definition, "character varying(%d)",
                        (len == 0) ? 255 : len);
            } else
              strcpy(definition, "text");
            break;
          case DATA_TYPE_BLOB:
            strcpy(definition, "bytea");
            break;
          default:
            strcpy(definition, "unknownType");
        };
      if (Columns[pos].IsNotNull() == true)
        strcat(definition, " NOT NULL");
      if (Columns[pos].IsPrimaryKey() == true
          && IsSingleFieldPrimaryKey() == true)
        {
          if (IsAutoincrement() == true)
            strcpy(definition, "SERIAL");
          else
            {
              // attempting to promote any PK as int-4
              switch (data_type)
                {
                  case DATA_TYPE_BOOLEAN:
                  case DATA_TYPE_INT8:
                  case DATA_TYPE_UINT8:
                  case DATA_TYPE_INT16:
                  case DATA_TYPE_UINT16:
                  case DATA_TYPE_INT32:
                    strcpy(definition, "integer");
                    break;
                }
            }
          strcat(definition, " PRIMARY KEY");
        }
      return;
    }
  *definition = '\0';
}

void PostGISHelper::OutputBooleanValue(FILE * out, sqlite3_int64 value)
{
// outputting a BOOLEAN value
  if (value == 0)
    fprintf(out, "0");
  else
    fprintf(out, "1");
}

void PostGISHelper::OutputValue(FILE * out, sqlite3_int64 value)
{
// outputting an INTEGER value
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
  fprintf(out, "%I64d", value);
#else
  fprintf(out, "%lld", value);
#endif
}

void PostGISHelper::OutputValue(FILE * out, double value)
{
// outputting a DOUBLE value
  fprintf(out, "%1.15f", value);
}

void PostGISHelper::OutputValue(FILE * out, const char *value)
{
// outputting a TEXT value 
  const char *p = value;
  fputc('\'', out);
  while (*p != '\0')
    {
      if (*p == '\'')
        fputc('\'', out);
      fputc(*p++, out);
    }
  fputc('\'', out);
}

void PostGISHelper::OutputValue(FILE * out, gaiaGeomCollPtr geom)
{
// outputting a GEOMETRY EWKB value
  gaiaOutBuffer out_buf;
  gaiaOutBufferInitialize(&out_buf);
  gaiaToEWKB(&out_buf, geom);
  if (out_buf.Error || out_buf.Buffer == NULL)
    fprintf(out, "NULL");
  else
    {
      int len = out_buf.WriteOffset;
      int i;
      fputc('\'', out);
      for (i = 0; i < len; i++)
        fputc(out_buf.Buffer[i], out);
      fputc('\'', out);
    }
  gaiaOutBufferReset(&out_buf);
}

void PostGISHelper::OutputValue(FILE * out, const unsigned char *value, int len)
{
// outputting a BLOB value as BYTEA
  int i;
  const char *in = (const char *) value;

// encoding a BLOB as BYTEA [escaped octects] 
  putc('E', out);
  putc('\'', out);
  for (i = 0; i < len; i++)
    {
      if (in[i] <= 31 || in[i] >= 127 || in[i] == '\'' || in[i] == '\\')
        fprintf(out, "\\\\%03o", in[i]);
      else
        putc(in[i], out);
    }
  putc('\'', out);
}

PostGISIndex *PostGISHelper::AddIndex(wxString & Name)
{
// adding a Primary Key to this Table
  wxString name = wxT("pk_") + Name;
  PostGISIndex *idx = new PostGISIndex(name);
  if (FirstIdx == NULL)
    FirstIdx = idx;
  if (LastIdx != NULL)
    LastIdx->SetNext(idx);
  LastIdx = idx;
  return idx;
}

PostGISIndex *PostGISHelper::AddIndex(wxString & name, bool unique)
{
// adding an Index to this Table
  PostGISIndex *idx = FirstIdx;
  while (idx)
    {
      if (idx->GetName() == name)
        {
          // already inserted
          return idx;
        }
      idx = idx->GetNext();
    }

  idx = new PostGISIndex(name, unique);
  if (FirstIdx == NULL)
    FirstIdx = idx;
  if (LastIdx != NULL)
    LastIdx->SetNext(idx);
  LastIdx = idx;
  return idx;
}

PostGISColumn *PostGISHelper::Find(wxString & name)
{
// retrieving a Column by name
  int i;
  for (i = 0; i < Count; i++)
    {
      PostGISColumn *pC = Columns + i;
      if (pC->GetName() == name)
        return pC;
    }
  return NULL;
}

void PostGISHelper::ExpandIndexFields(MyFrame * MainFrame, PostGISIndex * index,
                                      wxString & idxName)
{
//
// expanding the Index Fields
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  int seq;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  int ret;

  sql = wxT("PRAGMA index_info(");
  xname = (char *) malloc((idxName.Len() * 4) + 1);
  strcpy(xname, idxName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT(")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
                          &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("PostGIS SQL dump error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, MainFrame);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          seq = atoi(results[(i * columns) + 0]);
          name = results[(i * columns) + 2];
          Name = wxString::FromUTF8(name);
          PostGISColumn *col = Find(Name);
          index->AddField(seq, col);
        }
    }
  sqlite3_free_table(results);
}

void PostGISHelper::GetKeys(MyFrame * MainFrame, wxString & tableName)
{
//
// identifying the Primary Key (and any Index)
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  bool pKey;
  bool notNull;
  bool unique;
  wxString sql;
  char *xsql;
  int ret;
  char *xname;
  char *xname2;

// retrieving the Table Columns
  sql = wxT("PRAGMA table_info(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
                          &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("PostGIS SQL dump error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, MainFrame);
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
          if (atoi(results[(i * columns) + 3]) == 0)
            notNull = false;
          else
            notNull = true;
          if (atoi(results[(i * columns) + 5]) == 0)
            pKey = false;
          else
            pKey = true;
          Name = wxString::FromUTF8(name);
          SetColumn(Name, notNull, pKey);
        }
    }
  sqlite3_free_table(results);

// retrieving any Index bounded to this Table
  sql = wxT("PRAGMA index_list(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
                          &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("PostGIS SQL dump error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, MainFrame);
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
          if (atoi(results[(i * columns) + 2]) == 0)
            unique = false;
          else
            unique = true;
          Name = wxString::FromUTF8(name);
          PostGISIndex *index;
          if (Name.StartsWith(wxT("sqlite_autoindex_")) == true)
            index = AddIndex(tableName);
          else
            index = AddIndex(Name, unique);
          ExpandIndexFields(MainFrame, index, Name);
        }
    }
  sqlite3_free_table(results);

  if (IsSingleFieldPrimaryKey() == true)
    {
      // checking for AUTOINCREMENT
      sql =
        wxT("SELECT Count(*) FROM sqlite_sequence WHERE Lower(name) = Lower('");
      xname = (char *) malloc((tableName.Len() * 4) + 1);
      strcpy(xname, tableName.ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
                              &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        return;
      if (rows < 1)
        ;
      else
        {
          int cnt = 0;
          for (i = 1; i <= rows; i++)
            {
              cnt = atoi(results[(i * columns) + 0]);
            }
          if (cnt)
            Autoincrement = true;
        }
      sqlite3_free_table(results);
    }
}

TopoGeoItem::TopoGeoItem(wxString & name)
{
// constructor
  Name = name;
  Next = NULL;
}

TopoGeoSet::TopoGeoSet(const char *db_prefix, const char *name, int srid,
                       bool has_z)
{
// constructor
  DbPrefix = wxString::FromUTF8(db_prefix);
  Name = wxString::FromUTF8(name);
  SRID = srid;
  HasZ = has_z;
}

TopoNetItem::TopoNetItem(wxString & name)
{
// constructor
  Name = name;
  Next = NULL;
}

TopoNetSet::TopoNetSet(const char *name, bool spatial, int srid, bool has_z)
{
// constructor
  Name = wxString::FromUTF8(name);
  Spatial = spatial;
  SRID = srid;
  HasZ = has_z;
}

VectorCoverageSet::VectorCoverageSet(const char *prefix, const char *name,
                                     int srid, int type)
{
// constructor
  Prefix = wxString::FromUTF8(prefix);
  Name = wxString::FromUTF8(name);
  SRID = srid;
  GeometryType = type;
}

RasterCoverageSet::RasterCoverageSet(const char *name, int srid)
{
// constructor
  Name = wxString::FromUTF8(name);
  SRID = srid;
}

RasterCoverageItem::RasterCoverageItem(wxString & name, bool tile_data)
{
// constructor
  Name = name;
  TileData = tile_data;
  Next = NULL;
}

TableViewItem::TableViewItem(wxString & name, bool is_view, bool is_virtual)
{
// constructor
  Name = name;
  View = is_view;
  Virtual = is_virtual;
  Geometry = false;
  GeoPackageGeometry = false;
  GeoPackageVirtualGeometry = false;
  FdoOgrGeometry = false;
  FdoOgrVirtualGeometry = false;
  Next = NULL;
}

TableViewItem::TableViewItem(wxString & db, wxString & name)
{
// constructor
  DbName = db;
  Name = name;
  View = false;
  Virtual = false;
  Geometry = false;
  GeoPackageGeometry = false;
  GeoPackageVirtualGeometry = false;
  FdoOgrGeometry = false;
  FdoOgrVirtualGeometry = false;
  Next = NULL;
}

bool TableViewItem::IsPostgresTable(MyPostgres * list)
{
// testing for some PostgreSQL table or view
  MyPostgresConn *pC = list->GetFirst();
  while (pC != NULL)
    {
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          MyPostgresTable *pT = pS->GetFirstTable();
          while (pT != NULL)
            {
              if (Name.Cmp(pT->GetVirtName()) == 0)
                return true;
              if (Name.Cmp(pT->GetPostGisName()) == 0)
                return true;
              pT = pT->GetNext();
            }
          MyPostgresView *pV = pS->GetFirstView();
          while (pV != NULL)
            {
              if (Name.Cmp(pV->GetVirtName()) == 0)
                return true;
              pV = pV->GetNext();
            }
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
  return false;
}

bool TableViewItem::IsTmpMetadata()
{
// testing for some TEMPORARY METADATA TABLE
  if (DbName.Cmp(wxT("temp")) == 0
      && (Name.Cmp(wxT("postgis_geometry_columns")) == 0
          || Name.Cmp(wxT("tmp_vector_coverages")) == 0))
    return true;
  return false;
}

TableViewList::TableViewList()
{
// constructor
  First = NULL;
  Last = NULL;
  Count = 0;
  Sorted = NULL;
}

TableViewList::~TableViewList()
{
// destructor
  TableViewItem *ptv;
  TableViewItem *ptvN;
  ptv = First;
  while (ptv != NULL)
    {
      ptvN = ptv->GetNext();
      delete ptv;
      ptv = ptvN;
    }
  if (Sorted != NULL)
    delete[]Sorted;
}

void TableViewList::Add(wxString & name, bool isView, bool isVirtual)
{
// adding a new item to the list
  TableViewItem *item = new TableViewItem(name, isView, isVirtual);
  if (First == NULL)
    First = item;
  if (Last != NULL)
    Last->SetNext(item);
  Last = item;
}

void TableViewList::Add(wxString & db, wxString & name)
{
// adding a new item to the list
  TableViewItem *item = new TableViewItem(db, name);
  if (First == NULL)
    First = item;
  if (Last != NULL)
    Last->SetNext(item);
  Last = item;
}

static int cmp_tables1(const void *p1, const void *p2)
{
//
// compares two Tables or Views  by Name [for QSORT] 
//
  TableViewItem *ptv1 = *(TableViewItem **) p1;
  TableViewItem *ptv2 = *((TableViewItem **) p2);
  return ptv1->GetName().CmpNoCase(ptv2->GetName());
}

static int cmp_tables2(const void *p1, const void *p2)
{
//
// compares two Tables or Views by Name [for BSEARCH] 
//
  TableViewItem *ptv1 = (TableViewItem *) p1;
  TableViewItem *ptv2 = *((TableViewItem **) p2);
  return ptv1->GetName().CmpNoCase(ptv2->GetName());
}

void TableViewList::PrepareSorted()
{
// preparing the array of sorted pointers
  int cnt = 0;
  TableViewItem *item = First;
  while (item != NULL)
    {
      // counting how many items
      cnt++;
      item = item->GetNext();
    }
  Count = cnt;
  if (Sorted != NULL)
    delete[]Sorted;
  if (Count == 0)
    {
      Sorted = NULL;
      return;
    }
  Sorted = new TableViewItem *[Count];
  cnt = 0;
  item = First;
  while (item != NULL)
    {
      // populating the sorted array
      *(Sorted + cnt++) = item;
      item = item->GetNext();
    }
  qsort(Sorted, Count, sizeof(TableViewItem *), cmp_tables1);
}

void TableViewList::SetGeometry(wxString & name)
{
// marking a Geometry Table or View
  TableViewItem **ret;
  TableViewItem ptv(name, false, false);
  if (!Sorted)
    return;
  ret =
    (TableViewItem **) bsearch(&ptv, Sorted, Count, sizeof(TableViewItem *),
                               cmp_tables2);
  if (!ret)
    return;
  (*ret)->SetGeometry();
}

void TableViewList::SetGeoPackageGeometry(wxString & name)
{
// marking a GeoPackage Geometry Table 
  TableViewItem **ret;
  TableViewItem ptv(name, false, false);
  if (!Sorted)
    return;
  ret =
    (TableViewItem **) bsearch(&ptv, Sorted, Count, sizeof(TableViewItem *),
                               cmp_tables2);
  if (!ret)
    return;
  (*ret)->SetGeoPackageGeometry();
}

void TableViewList::SetGeoPackageVirtualGeometry(wxString & name)
{
// marking a GeoPackage Virtual Geometry Table 
  TableViewItem **ret;
  TableViewItem ptv(name, false, false);
  if (!Sorted)
    return;
  ret =
    (TableViewItem **) bsearch(&ptv, Sorted, Count, sizeof(TableViewItem *),
                               cmp_tables2);
  if (!ret)
    return;
  (*ret)->SetGeoPackageVirtualGeometry();
}

void TableViewList::SetFdoOgrGeometry(wxString & name)
{
// marking an FDO/OGR Geometry Table 
  TableViewItem **ret;
  TableViewItem ptv(name, false, false);
  if (!Sorted)
    return;
  ret =
    (TableViewItem **) bsearch(&ptv, Sorted, Count, sizeof(TableViewItem *),
                               cmp_tables2);
  if (!ret)
    return;
  (*ret)->SetFdoOgrGeometry();
}

void TableViewList::SetFdoOgrVirtualGeometry(wxString & name)
{
// marking an FDO/OGR Virtual Geometry Table 
  TableViewItem **ret;
  TableViewItem ptv(name, false, false);
  if (!Sorted)
    return;
  ret =
    (TableViewItem **) bsearch(&ptv, Sorted, Count, sizeof(TableViewItem *),
                               cmp_tables2);
  if (!ret)
    return;
  (*ret)->SetFdoOgrVirtualGeometry();
}

MyAttachedDB::MyAttachedDB(wxString & db_prefix, wxString & path)
{
// ctor
  DbPrefix = db_prefix;
  Path = path;
  GeoPackage = false;
  FdoOgr = false;
  Initialized = false;
  First = NULL;
  Last = NULL;
  Prev = NULL;
  Next = NULL;
}

MyAttachedDB::~MyAttachedDB()
{
// dtor
  MyAttachedTable *pT;
  MyAttachedTable *pTn;
  pT = First;
  while (pT != NULL)
    {
      pTn = pT->GetNext();
      delete pT;
      pT = pTn;
    }
}

void MyAttachedDB::AddGeoTable(wxString & name, int type)
{
// adding a GeoTable to some ATTACHED-DB
  MyAttachedTable *tbl = new MyAttachedTable(name, type);
  if (First == NULL)
    First = tbl;
  if (Last != NULL)
    Last->SetNext(tbl);
  Last = tbl;
}

MyAttachedDbList::MyAttachedDbList()
{
// ctor
  First = NULL;
  Last = NULL;
}

MyAttachedDbList::~MyAttachedDbList()
{
// dtor
  Flush();
}

void MyAttachedDbList::Flush()
{
// resetting the attached DB list to its empty initial state
  MyAttachedDB *pD;
  MyAttachedDB *pDn;
  pD = First;
  while (pD != NULL)
    {
      pDn = pD->GetNext();
      delete pD;
      pD = pDn;
    }
  First = NULL;
  Last = NULL;
}

MyAttachedDB *MyAttachedDbList::Find(wxString & db_prefix, wxString & path)
{
// searching an ATTACHED-DB item by name
  MyAttachedDB *pD = First;
  while (pD != NULL)
    {
      if (pD->GetDbPrefix() == db_prefix && pD->GetPath() == path)
        return pD;
      pD = pD->GetNext();
    }
  return NULL;
}

void MyAttachedDbList::Insert(wxString & db_prefix, wxString & path)
{
// inserting an ATTACHED-DB item
  MyAttachedDB *db = new MyAttachedDB(db_prefix, path);
  if (First == NULL)
    First = db;
  db->SetPrev(Last);
  if (Last != NULL)
    Last->SetNext(db);
  Last = db;
}

void MyAttachedDbList::Remove(MyAttachedDB * item)
{
// removing an ATTACHED-DB item
  if (item->GetPrev() != NULL)
    item->GetPrev()->SetNext(item->GetNext());
  if (item->GetNext() != NULL)
    item->GetNext()->SetPrev(item->GetPrev());
  if (First == item)
    First = item->GetNext();
  if (Last == item)
    Last = item->GetPrev();
  delete item;
}

void MyAttachedDbList::Remove(wxString & db_prefix)
{
// removing an ATTACHED-DB item
  MyAttachedDB *pD = First;
  while (pD != NULL)
    {
      if (pD->GetDbPrefix() == db_prefix)
        {
          Remove(pD);
          return;
        }
      pD = pD->GetNext();
    }
}

void MyAttachedDbList::AddGeoTable(wxString & db_prefix, wxString & table_name,
                                   int type)
{
// adding a GeoTable into an AttachedDB
  MyAttachedDB *pD = First;
  while (pD != NULL)
    {
      if (pD->GetDbPrefix() == db_prefix)
        {
          pD->AddGeoTable(table_name, type);
          return;
        }
      pD = pD->GetNext();
    }
}
