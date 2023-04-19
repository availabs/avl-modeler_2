/*
/ Styles.cpp
/ various dialog classes
/
/ version 1.8, 2015 March 13
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2015  Alessandro Furieri
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

#include "wx/spinctrl.h"
#include "wx/filename.h"

#include "rasterlite2/rasterlite2.h"
#include "rasterlite2/rl2graphics.h"

#include "icons/dustbin.xpm"

bool LoadRasterStyleDialog::Create(MyFrame * parent, wxArrayString & paths,
                                   wxString & path)
{
//
// creating the dialog
//
  MainFrame = parent;
  Paths = paths;
  Path = path;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Loading New SLD/SE Raster Style(s)")) == false)
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

void LoadRasterStyleDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: files to be imported
  wxBoxSizer *fileSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(fileSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *fileLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Import &File(s):"));
  fileSizer->Add(fileLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *filesCtrl = new wxTextCtrl(this, wxID_ANY, Path,
                                         wxDefaultPosition, wxSize(600, 60),
                                         wxTE_MULTILINE | wxTE_READONLY);
  fileSizer->Add(filesCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: progress report
  wxBoxSizer *progrSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(progrSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *doneCtrl = new wxTextCtrl(this, ID_LOAD_STYLE_DONE, ListDone,
                                        wxDefaultPosition, wxSize(650, 100),
                                        wxTE_MULTILINE | wxTE_READONLY |
                                        wxTE_RICH2);
  progrSizer->Add(doneCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Import"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *abort = new wxButton(this, ID_LOAD_ABORT, wxT("&Abort"));
  abort->Enable(false);
  okCancelBox->Add(abort, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadRasterStyleDialog::OnOk);
  Connect(ID_LOAD_ABORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadRasterStyleDialog::OnCmdAbort);
  Connect(ID_LOAD_RASTER_STYLE_START, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadRasterStyleDialog::OnRequestStart);
  Connect(ID_LOAD_RASTER_STYLE_STOP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadRasterStyleDialog::OnRequestStop);
  Connect(ID_LOAD_RASTER_STYLE_SKIP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadRasterStyleDialog::OnRequestSkip);
  Connect(ID_LOAD_RASTER_STYLE_THREAD_FINISHED, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadRasterStyleDialog::OnThreadFinished);
}

void LoadRasterStyleDialog::OnCmdAbort(wxCommandEvent & WXUNUSED(event))
{
//
// aborting the Raster Style(s) Import process
//
  if (Params.IsAbortPending() == true)
    return;
  Params.RequestAbort();
  wxString report =
    wxT("\nan ABORT request is now pending and will be accepted ASAP");
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_STYLE_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(192, 0, 0);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(report);
}

void LoadRasterStyleDialog::OnRequestStart(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_STYLE_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(0, 0, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(msg);
  doneCtrl->MarkDirty();
}

void LoadRasterStyleDialog::OnRequestStop(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_STYLE_DONE);
  ListDone += msg;
  doneCtrl->Clear();
  wxColour fore = wxColour(0, 0, 0);
  wxColour back = wxColour(255, 255, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(ListDone);
  doneCtrl->MarkDirty();
}

void LoadRasterStyleDialog::OnRequestSkip(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_STYLE_DONE);
  ListDone += msg;
  doneCtrl->Clear();
  wxColour fore = wxColour(0, 0, 0);
  wxColour back = wxColour(255, 0, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(ListDone);
  doneCtrl->MarkDirty();
}

#if defined(_WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteRasterStylesLoad(void *arg)
#else
void *DoExecuteRasterStylesLoad(void *arg)
#endif
{
//
// threaded function: processing a Raster Style Import operation
//
  RasterStylesLoadParams *params = (RasterStylesLoadParams *) arg;
  const char *sql;
  int ret;
  sqlite3_stmt *stmt = NULL;
  int count = params->GetPathsCount();
  int i;
  clock_t clock_start;
  clock_t clock_end;
  double seconds;
  char elapsed[64];
  char ordinal[64];
  wxString report;
  wxString path;
  void *blob;
  int blob_size;
  wxCommandEvent evt_start(wxEVT_COMMAND_BUTTON_CLICKED,
                           ID_LOAD_RASTER_STYLE_START);
  wxCommandEvent evt_stop(wxEVT_COMMAND_BUTTON_CLICKED,
                          ID_LOAD_RASTER_STYLE_STOP);
  wxCommandEvent evt_skip(wxEVT_COMMAND_BUTTON_CLICKED,
                          ID_LOAD_RASTER_STYLE_SKIP);

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

  sql = "SELECT SE_RegisterRasterStyle(?)";
  ret =
    sqlite3_prepare_v2(params->GetMainFrame()->GetSqlite(), sql, strlen(sql),
                       &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      params->SetError();
      goto error;
    }

  for (i = 0; i < count; i++)
    {
      // loading and verifying each Raster Style
      if (params->IsAbortPending() == true)
        {
          report = wxT("STOP .... aborted by the user !!!!");
          evt_start.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
          break;
        }
      path = params->GetPathByIndex(i);
      params->SetCurrentPath(path);
      report = wxT("Parsing and Validating: ") + path;
      evt_start.SetString(report);
      params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
      clock_start = clock();
      if (params->GetMainFrame()->ValidateRasterStyle(path.ToUTF8(), &blob,
                                                      &blob_size) == true)
        {
          if (params->GetDlg()->RegisterRasterStyle(stmt, blob, blob_size) !=
              true)
            {
              params->SetError();
              goto error;
            }
          clock_end = clock();
          seconds =
            (double) (clock_end - clock_start) / (double) CLOCKS_PER_SEC;
          MyResultSetView::FormatElapsedTime(seconds, elapsed);
          sprintf(ordinal, "done %d/%d: ", i + 1, count);
          report =
            wxString::FromUTF8(ordinal) + path + wxT("    [") +
            wxString::FromUTF8(elapsed) + wxT("]\n");
          evt_stop.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
          params->Done();
      } else
        {
          clock_end = clock();
          seconds =
            (double) (clock_end - clock_start) / (double) CLOCKS_PER_SEC;
          MyResultSetView::FormatElapsedTime(seconds, elapsed);
          sprintf(ordinal, "discarded %d/%d (not a valid Raster Style): ",
                  i + 1, count);
          report =
            wxString::FromUTF8(ordinal) + path + wxT("    [") +
            wxString::FromUTF8(elapsed) + wxT("]\n");
          evt_skip.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_skip);
        }
    }
  sqlite3_finalize(stmt);
  goto end;

error:
  sqlite3_finalize(stmt);
  report = wxT("FAILED: ") + path;
  evt_stop.SetString(report);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
  params->SetError();
end:
  wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED,
                       ID_LOAD_RASTER_STYLE_THREAD_FINISHED);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(event);
#if defined(_WIN32) && !defined(__MINGW32__)
  return 0;
#else
  pthread_exit(NULL);
  return NULL;
#endif
}

void LoadRasterStyleDialog::OnThreadFinished(wxCommandEvent & WXUNUSED(event))
{
// resuming execution when the Import Raster thread quits
  ::wxEndBusyCursor();
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  quitBtn->Enable(true);
  abortBtn->Enable(false);
  sqlite3_exec(Params.GetMainFrame()->GetSqlite(), "COMMIT", NULL, NULL, NULL);
  if (Params.GetError() == true)
    {
      char dummy[80];
      sprintf(dummy, "%d Raster Styles have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nA fatal error occurred while loading:\n") +
                   Params.GetCurrentPath(), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
  } else if (Params.IsAbortPending() == true)
    {
      char dummy[80];
      sprintf(dummy, "%d Raster Styles have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nStopped by an Abort user request"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  } else
    {
      char dummy[80];
      sprintf(dummy, "%d Raster Styles have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
    }
}

bool MyFrame::ValidateRasterStyle(const char *path, void **blob, int *blob_size)
{
//
// attempting to parse and validate a Raster Style
//
  int ret;
  sqlite3_stmt *stmt;
  void *xblob = NULL;
  int xblob_size;
  int valid = 0;

// Schema validation
  char *sql = sqlite3_mprintf("SELECT XB_Create(XB_LoadXML(%Q), 1, 1)", path);
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return false;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              const void *xxblob = sqlite3_column_blob(stmt, 0);
              xblob_size = sqlite3_column_bytes(stmt, 0);
              xblob = malloc(xblob_size);
              memcpy(xblob, xxblob, xblob_size);
            }
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (xblob == NULL)
    return false;

// Checking if really is a Raster Style
  stmt = NULL;
  sql = sqlite3_mprintf("SELECT XB_IsSldSERasterStyle(?)");
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    goto invalid;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_blob(stmt, 1, xblob, xblob_size, SQLITE_STATIC);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        goto invalid;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;
  if (!valid)
    goto invalid;
  *blob = xblob;
  *blob_size = xblob_size;
  return true;

invalid:
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  free(xblob);
  *blob = NULL;
  *blob_size = 0;
  return false;
}

bool LoadRasterStyleDialog::RegisterRasterStyle(sqlite3_stmt * stmt, void *blob,
                                                int blob_size)
{
//
// attempting to register the new Raster Style
//
  int ret;
  int valid = 0;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_blob(stmt, 1, blob, blob_size, free);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        return false;
    }
  if (valid)
    return true;
  return false;
}

void LoadRasterStyleDialog::DoRunLoad()
{
//
// executing the Load Raster Style(s) process in a separate Thread
//
#if defined(_WIN32) && !defined(__MINGW32__)
  HANDLE thread_handle;
  DWORD dwThreadId;
#else
  pthread_t thread_id;
#endif
  Params.Initialize(MainFrame, this, Paths);
#if defined(_WIN32) && !defined(__MINGW32__)
  thread_handle =
    CreateThread(NULL, 0, DoExecuteRasterStylesLoad, &Params, 0, &dwThreadId);
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
              pthread_create(&thread_id, &attr, DoExecuteRasterStylesLoad,
                             &Params);
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
                  pthread_create(&thread_id, &attr, DoExecuteRasterStylesLoad,
                                 &Params);
                  pthread_detach(thread_id);
                }
            }
        }
    }
#endif
  if (!ok_prior)
    {
      // failure: using standard priority
      pthread_create(&thread_id, NULL, DoExecuteRasterStylesLoad, &Params);
      pthread_detach(thread_id);
    }
#endif
}

void LoadRasterStyleDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxButton *loadBtn = (wxButton *) FindWindow(wxID_OK);
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  loadBtn->Enable(false);
  quitBtn->Enable(false);
  abortBtn->Enable(true);
  ::wxBeginBusyCursor();
  char *errMsg = NULL;
  int ret = sqlite3_exec(MainFrame->GetSqlite(), "BEGIN", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  DoRunLoad();
}

bool LoadVectorStyleDialog::Create(MyFrame * parent, wxArrayString & paths,
                                   wxString & path)
{
//
// creating the dialog
//
  MainFrame = parent;
  Paths = paths;
  Path = path;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Loading New SLD/SE Vector Style(s)")) == false)
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

void LoadVectorStyleDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: files to be imported
  wxBoxSizer *fileSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(fileSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *fileLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Import &File(s):"));
  fileSizer->Add(fileLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *filesCtrl = new wxTextCtrl(this, wxID_ANY, Path,
                                         wxDefaultPosition, wxSize(600, 60),
                                         wxTE_MULTILINE | wxTE_READONLY);
  fileSizer->Add(filesCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: progress report
  wxBoxSizer *progrSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(progrSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *doneCtrl = new wxTextCtrl(this, ID_LOAD_STYLE_DONE, ListDone,
                                        wxDefaultPosition, wxSize(650, 100),
                                        wxTE_MULTILINE | wxTE_READONLY |
                                        wxTE_RICH2);
  progrSizer->Add(doneCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Import"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *abort = new wxButton(this, ID_LOAD_ABORT, wxT("&Abort"));
  abort->Enable(false);
  okCancelBox->Add(abort, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadVectorStyleDialog::OnOk);
  Connect(ID_LOAD_ABORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadVectorStyleDialog::OnCmdAbort);
  Connect(ID_LOAD_VECTOR_STYLE_START, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadVectorStyleDialog::OnRequestStart);
  Connect(ID_LOAD_VECTOR_STYLE_STOP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadVectorStyleDialog::OnRequestStop);
  Connect(ID_LOAD_VECTOR_STYLE_SKIP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadVectorStyleDialog::OnRequestSkip);
  Connect(ID_LOAD_VECTOR_STYLE_THREAD_FINISHED, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadVectorStyleDialog::OnThreadFinished);
}

void LoadVectorStyleDialog::OnCmdAbort(wxCommandEvent & WXUNUSED(event))
{
//
// aborting the Vector Style(s) Import process
//
  if (Params.IsAbortPending() == true)
    return;
  Params.RequestAbort();
  wxString report =
    wxT("\nan ABORT request is now pending and will be accepted ASAP");
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_STYLE_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(192, 0, 0);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(report);
}

void LoadVectorStyleDialog::OnRequestStart(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_STYLE_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(0, 0, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(msg);
  doneCtrl->MarkDirty();
}

void LoadVectorStyleDialog::OnRequestStop(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_STYLE_DONE);
  ListDone += msg;
  doneCtrl->Clear();
  wxColour fore = wxColour(0, 0, 0);
  wxColour back = wxColour(255, 255, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(ListDone);
  doneCtrl->MarkDirty();
}

void LoadVectorStyleDialog::OnRequestSkip(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_STYLE_DONE);
  ListDone += msg;
  doneCtrl->Clear();
  wxColour fore = wxColour(0, 0, 0);
  wxColour back = wxColour(255, 0, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(ListDone);
  doneCtrl->MarkDirty();
}

#if defined(_WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteVectorStylesLoad(void *arg)
#else
void *DoExecuteVectorStylesLoad(void *arg)
#endif
{
//
// threaded function: processing a Vector Style Import operation
//
  VectorStylesLoadParams *params = (VectorStylesLoadParams *) arg;
  const char *sql;
  int ret;
  sqlite3_stmt *stmt = NULL;
  int count = params->GetPathsCount();
  int i;
  clock_t clock_start;
  clock_t clock_end;
  double seconds;
  char elapsed[64];
  char ordinal[64];
  wxString report;
  wxString path;
  void *blob;
  int blob_size;
  wxCommandEvent evt_start(wxEVT_COMMAND_BUTTON_CLICKED,
                           ID_LOAD_VECTOR_STYLE_START);
  wxCommandEvent evt_stop(wxEVT_COMMAND_BUTTON_CLICKED,
                          ID_LOAD_VECTOR_STYLE_STOP);
  wxCommandEvent evt_skip(wxEVT_COMMAND_BUTTON_CLICKED,
                          ID_LOAD_VECTOR_STYLE_SKIP);

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

  sql = "SELECT SE_RegisterVectorStyle(?)";
  ret =
    sqlite3_prepare_v2(params->GetMainFrame()->GetSqlite(), sql, strlen(sql),
                       &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      params->SetError();
      goto error;
    }

  for (i = 0; i < count; i++)
    {
      // loading and verifying each Vector Style
      if (params->IsAbortPending() == true)
        {
          report = wxT("STOP .... aborted by the user !!!!");
          evt_start.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
          break;
        }
      path = params->GetPathByIndex(i);
      params->SetCurrentPath(path);
      report = wxT("Parsing and Validating: ") + path;
      evt_start.SetString(report);
      params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
      clock_start = clock();
      if (params->GetMainFrame()->ValidateVectorStyle(path.ToUTF8(), &blob,
                                                      &blob_size) == true)
        {
          if (params->GetDlg()->RegisterVectorStyle(stmt, blob, blob_size) !=
              true)
            {
              params->SetError();
              goto error;
            }
          clock_end = clock();
          seconds =
            (double) (clock_end - clock_start) / (double) CLOCKS_PER_SEC;
          MyResultSetView::FormatElapsedTime(seconds, elapsed);
          sprintf(ordinal, "done %d/%d: ", i + 1, count);
          report =
            wxString::FromUTF8(ordinal) + path + wxT("    [") +
            wxString::FromUTF8(elapsed) + wxT("]\n");
          evt_stop.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
          params->Done();
      } else
        {
          clock_end = clock();
          seconds =
            (double) (clock_end - clock_start) / (double) CLOCKS_PER_SEC;
          MyResultSetView::FormatElapsedTime(seconds, elapsed);
          sprintf(ordinal, "discarded %d/%d (not a valid Vector Style): ",
                  i + 1, count);
          report =
            wxString::FromUTF8(ordinal) + path + wxT("    [") +
            wxString::FromUTF8(elapsed) + wxT("]\n");
          evt_skip.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_skip);
        }
    }
  sqlite3_finalize(stmt);
  goto end;

error:
  sqlite3_finalize(stmt);
  report = wxT("FAILED: ") + path;
  evt_stop.SetString(report);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
  params->SetError();
end:
  wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED,
                       ID_LOAD_VECTOR_STYLE_THREAD_FINISHED);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(event);
#if defined(_WIN32) && !defined(__MINGW32__)
  return 0;
#else
  pthread_exit(NULL);
  return NULL;
#endif
}

void LoadVectorStyleDialog::OnThreadFinished(wxCommandEvent & WXUNUSED(event))
{
// resuming execution when the Import Vector thread quits
  ::wxEndBusyCursor();
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  quitBtn->Enable(true);
  abortBtn->Enable(false);
  sqlite3_exec(Params.GetMainFrame()->GetSqlite(), "COMMIT", NULL, NULL, NULL);
  if (Params.GetError() == true)
    {
      char dummy[80];
      sprintf(dummy, "%d Vector Styles have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nA fatal error occurred while loading:\n") +
                   Params.GetCurrentPath(), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
  } else if (Params.IsAbortPending() == true)
    {
      char dummy[80];
      sprintf(dummy, "%d Vector Styles have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nStopped by an Abort user request"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  } else
    {
      char dummy[80];
      sprintf(dummy, "%d Vector Styles have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
    }
}

bool MyFrame::ValidateVectorStyle(const char *path, void **blob, int *blob_size)
{
//
// attempting to parse and validate a Vector Style
//
  int ret;
  sqlite3_stmt *stmt;
  void *xblob = NULL;
  int xblob_size;
  int valid = 0;

// Schema validation
  char *sql = sqlite3_mprintf("SELECT XB_Create(XB_LoadXML(%Q), 1, 1)", path);
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return false;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              const void *xxblob = sqlite3_column_blob(stmt, 0);
              xblob_size = sqlite3_column_bytes(stmt, 0);
              xblob = malloc(xblob_size);
              memcpy(xblob, xxblob, xblob_size);
            }
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (xblob == NULL)
    return false;

// Checking if really is a Vector Style
  stmt = NULL;
  sql = sqlite3_mprintf("SELECT XB_IsSldSEVectorStyle(?)");
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    goto invalid;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_blob(stmt, 1, xblob, xblob_size, SQLITE_STATIC);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        goto invalid;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;
  if (!valid)
    goto invalid;
  *blob = xblob;
  *blob_size = xblob_size;
  return true;

invalid:
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  free(xblob);
  *blob = NULL;
  *blob_size = 0;
  return false;
}

bool LoadVectorStyleDialog::RegisterVectorStyle(sqlite3_stmt * stmt, void *blob,
                                                int blob_size)
{
//
// attempting to register the new Vector Style
//
  int ret;
  int valid = 0;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_blob(stmt, 1, blob, blob_size, free);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        return false;
    }
  if (valid)
    return true;
  return false;
}

void LoadVectorStyleDialog::DoRunLoad()
{
//
// executing the Load Vector Style(s) process in a separate Thread
//
#if defined(_WIN32) && !defined(__MINGW32__)
  HANDLE thread_handle;
  DWORD dwThreadId;
#else
  pthread_t thread_id;
#endif
  Params.Initialize(MainFrame, this, Paths);
#if defined(_WIN32) && !defined(__MINGW32__)
  thread_handle =
    CreateThread(NULL, 0, DoExecuteVectorStylesLoad, &Params, 0, &dwThreadId);
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
              pthread_create(&thread_id, &attr, DoExecuteVectorStylesLoad,
                             &Params);
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
                  pthread_create(&thread_id, &attr, DoExecuteVectorStylesLoad,
                                 &Params);
                  pthread_detach(thread_id);
                }
            }
        }
    }
#endif
  if (!ok_prior)
    {
      // failure: using standard priority
      pthread_create(&thread_id, NULL, DoExecuteVectorStylesLoad, &Params);
      pthread_detach(thread_id);
    }
#endif
}

void LoadVectorStyleDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxButton *loadBtn = (wxButton *) FindWindow(wxID_OK);
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  loadBtn->Enable(false);
  quitBtn->Enable(false);
  abortBtn->Enable(true);
  ::wxBeginBusyCursor();
  char *errMsg = NULL;
  int ret = sqlite3_exec(MainFrame->GetSqlite(), "BEGIN", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  DoRunLoad();
}

bool RasterCoverageStylesDialog::Create(MyFrame * parent, wxString & coverage)
{
//
// creating the dialog
//
  MainFrame = parent;
  int pos = coverage.Find(wxT(" [SRID="));
  if (pos != wxNOT_FOUND)
    CoverageName = coverage.Left(pos);
  else
    CoverageName = coverage;
  List = MainFrame->FindRasterCoverageStyles(CoverageName);
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Raster Coverage supported SLD/SE Styles")) == false)
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

void RasterCoverageStylesDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(350, 22),
                                       wxTE_READONLY);
  cvgCtrl->Enable(false);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered SLD/SE Styles"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  RasterCoverageStyle *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Style ID"));
  GridCtrl->SetColLabelValue(1, wxT("Name"));
  GridCtrl->SetColLabelValue(2, wxT("Title"));
  GridCtrl->SetColLabelValue(3, wxT("Abstract"));
  GridCtrl->SetColLabelValue(4, wxT("Schema Validated"));
  GridCtrl->SetColLabelValue(5, wxT("Schema URI"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      sprintf(dummy, "%d", pS->GetStyleID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 0, cell);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 1, pS->GetName());
      GridCtrl->SetCellValue(count, 2, pS->GetTitle());
      GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
      GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
      GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *quit = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(quit, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *addNew = new wxButton(this, ID_SLD_SE_ADD, wxT("&Add Style(s)"));
  btnBox->Add(addNew, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterCoverageStylesDialog::OnQuit);
  Connect(ID_SLD_SE_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterCoverageStylesDialog::OnAddStyle);
  Connect(ID_SLD_SE_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          RasterCoverageStylesDialog::OnCmdRemoveStyle);
  Connect(wxID_ANY, wxEVT_GRID_CELL_RIGHT_CLICK,
          (wxObjectEventFunction) & RasterCoverageStylesDialog::OnRightClick);
}

void RasterCoverageStylesDialog::OnRightClick(wxGridEvent & event)
{
//
// right click on some cell [mouse action]
//
  wxMenu menu;
  wxMenuItem *menuItem;
  wxPoint pt = event.GetPosition();
  CurrentRow = event.GetRow();
  GridCtrl->SelectRow(CurrentRow);
  wxString id = GridCtrl->GetCellValue(CurrentRow, 0);
  long style_id;
  id.ToLong(&style_id);
  CurrentStyleID = style_id;
  menuItem = new wxMenuItem(&menu, ID_SLD_SE_REMOVE, wxT("&Remove"));
  menu.Append(menuItem);
  GridCtrl->PopupMenu(&menu, pt);
}

void RasterCoverageStylesDialog::
OnCmdRemoveStyle(wxCommandEvent & WXUNUSED(event))
{
//
// deleting a Style [mouse action]
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_UnRegisterRasterStyledLayer(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_int(stmt, 2, CurrentStyleID);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    GridCtrl->DeleteRows(CurrentRow, 1);
  sqlite3_finalize(stmt);
}

void RasterCoverageStylesDialog::
DoRegistetRasterCoverageStyles(ListRasterStylesDialog * dlg)
{
//
// attempting to register the Raster Coverage Style(s)
//
  sqlite3_stmt *stmt = NULL;
  char *dummy;
  const char *sql = "SELECT SE_RegisterRasterStyledLayer(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return;

  int max = dlg->GetSelectedCount();
  for (int i = 0; i < max; i++)
    {
      int style_id = dlg->GetSelectedStyleId(i);
      if (style_id >= 0)
        {
          sqlite3_reset(stmt);
          sqlite3_clear_bindings(stmt);
          dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
          strcpy(dummy, CoverageName.ToUTF8());
          sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
          free(dummy);
          sqlite3_bind_int(stmt, 2, style_id);
          ret = sqlite3_step(stmt);
          if (ret == SQLITE_DONE || ret == SQLITE_ROW)
            ;
          else
            {
              sqlite3_finalize(stmt);
              return;
            }
        }
    }
  sqlite3_finalize(stmt);
}

void RasterCoverageStylesDialog::OnAddStyle(wxCommandEvent & WXUNUSED(event))
{
//
// adding a new Style [button action]
//
  ListRasterStylesDialog dlg;
  dlg.Create(MainFrame);
  if (dlg.ShowModal() == wxID_OK)
    {
      // attempting to register the new styles
      DoRegistetRasterCoverageStyles(&dlg);

      // updating the Grid
      int tot_rows = GridCtrl->GetNumberRows();
      if (tot_rows > 0)
        GridCtrl->DeleteRows(0, tot_rows);
      if (List != NULL)
        delete List;
      List = MainFrame->FindRasterCoverageStyles(CoverageName);
      int count = 0;
      RasterCoverageStyle *pS = List->GetFirst();
      while (pS)
        {
          // counting how many lines are there
          count++;
          pS = pS->GetNext();
        }
      GridCtrl->AppendRows(count);
      count = 0;
      char dummy[1024];
      wxString cell;
      pS = List->GetFirst();
      while (pS)
        {
          // feeding grid rows
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          sprintf(dummy, "%d", pS->GetStyleID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 0, cell);
          GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 1, pS->GetName());
          GridCtrl->SetCellValue(count, 2, pS->GetTitle());
          GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
          GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
          GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
          count++;
          pS = pS->GetNext();
        }
      GridCtrl->AutoSizeColumns();
    }
}

void RasterCoverageStylesDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool ListRasterStylesDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  List = MainFrame->FindRasterStyles();
  if (wxDialog::Create(parent, wxID_ANY, wxT("Registered SLD/SE Raster Styles"))
      == false)
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

void ListRasterStylesDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered SLD/SE Styles"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  RasterCoverageStyle *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Style ID"));
  GridCtrl->SetColLabelValue(1, wxT("Name"));
  GridCtrl->SetColLabelValue(2, wxT("Title"));
  GridCtrl->SetColLabelValue(3, wxT("Abstract"));
  GridCtrl->SetColLabelValue(4, wxT("Schema Validated"));
  GridCtrl->SetColLabelValue(5, wxT("Schema URI"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      sprintf(dummy, "%d", pS->GetStyleID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 0, cell);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 1, pS->GetName());
      GridCtrl->SetCellValue(count, 2, pS->GetTitle());
      GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
      GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
      GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Ok"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & ListRasterStylesDialog::OnOk);
}

int ListRasterStylesDialog::GetSelectedCount()
{
//
// computing how many selected Style there are
//
  int count = 0;
  RasterCoverageStyle *pS = List->GetFirst();
  while (pS != NULL)
    {
      if (pS->IsSelected() == true)
        count++;
      pS = pS->GetNext();
    }
  return count;
}

int ListRasterStylesDialog::GetSelectedStyleId(int idx)
{
//
// returning the Nth selected StyleID
//
  int count = 0;
  RasterCoverageStyle *pS = List->GetFirst();
  while (pS != NULL)
    {
      if (pS->IsSelected() == true)
        {
          if (count == idx)
            return pS->GetStyleID();
          count++;
        }
      pS = pS->GetNext();
    }
  return -1;
}

void ListRasterStylesDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          wxString cell_id = GridCtrl->GetCellValue(i, 0);
          long style_id;
          if (cell_id.ToLong(&style_id) == true)
            List->MarkSelected(style_id);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

RasterCoverageStyle::RasterCoverageStyle(int style_id, wxString & name,
                                         wxString & title, wxString & abstract,
                                         wxString & validated,
                                         wxString & schema_uri)
{
// constructor
  StyleID = style_id;
  Name = name;
  Title = title;
  Abstract = abstract;
  SchemaValidated = validated;
  SchemaURI = schema_uri;
  Selected = false;
  Next = NULL;
}

RasterCoverageStylesList::RasterCoverageStylesList()
{
// constructor
  First = NULL;
  Last = NULL;
}

RasterCoverageStylesList::~RasterCoverageStylesList()
{
// destructor
  RasterCoverageStyle *pS;
  RasterCoverageStyle *pSn;
  pS = First;
  while (pS != NULL)
    {
      pSn = pS->GetNext();
      delete pS;
      pS = pSn;
    }
}

void RasterCoverageStylesList::Add(int style_id, wxString & name,
                                   wxString & title, wxString & abstract,
                                   wxString & validated, wxString & schema_uri)
{
// inserting a new Style
  RasterCoverageStyle *pS =
    new RasterCoverageStyle(style_id, name, title, abstract, validated,
                            schema_uri);
  if (First == NULL)
    First = pS;
  if (Last != NULL)
    Last->SetNext(pS);
  Last = pS;
}

void RasterCoverageStylesList::MarkSelected(int styleId)
{
// marking a Selected Style
  RasterCoverageStyle *pS;
  pS = First;
  while (pS != NULL)
    {
      if (pS->GetStyleID() == styleId)
        {
          pS->MarkSelected();
          break;
        }
      pS = pS->GetNext();
    }
}

RasterCoverageStylesList *MyFrame::FindRasterCoverageStyles(wxString & coverage)
{
// will retrieve all SLD/SE Styles for the given Coverage
  RasterCoverageStylesList *list = new RasterCoverageStylesList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  wxString sql;
  char *xsql;

  sql =
    wxT
    ("SELECT style_id, name, title, abstract, schema_validated, schema_uri ");
  sql +=
    wxT("FROM SE_raster_styled_layers_view WHERE coverage_name = '") + coverage;
  sql += wxT("' ORDER BY style_id");
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
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          int style_id = atoi(value);
          value = results[(i * columns) + 1];
          wxString name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString validated = wxT("Yes");
          if (atoi(value) == 0)
            validated = wxT("No");
          value = results[(i * columns) + 5];
          wxString schema_uri = wxString::FromUTF8(value);
          list->Add(style_id, name, title, abstract, validated, schema_uri);
        }
    }
  sqlite3_free_table(results);
  return list;
}

RasterCoverageStylesList *MyFrame::FindRasterStyles()
{
// will retrieve all registered SLD/SE Raster Styles
  RasterCoverageStylesList *list = new RasterCoverageStylesList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;

  sql =
    "SELECT style_id, style_name, XB_GetTitle(style), XB_GetAbstract(style), "
    "XB_IsSchemaValidated(style), XB_GetSchemaURI(style) "
    "FROM SE_raster_styles ORDER BY style_name";
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          int style_id = atoi(value);
          value = results[(i * columns) + 1];
          wxString name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString validated = wxT("Yes");
          if (atoi(value) == 0)
            validated = wxT("No");
          value = results[(i * columns) + 5];
          wxString schema_uri = wxString::FromUTF8(value);
          list->Add(style_id, name, title, abstract, validated, schema_uri);
        }
    }
  sqlite3_free_table(results);
  return list;
}

CandidateVectorCoveragesList::~CandidateVectorCoveragesList()
{
// destructor
  CandidateVectorCoverage *pC;
  CandidateVectorCoverage *pCn;
  pC = First;
  while (pC != NULL)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
}

void CandidateVectorCoveragesList::Add(wxString & table_name,
                                       wxString & geometry, int srid,
                                       wxString & type)
{
// inserting a new candidate Vector Coverage
  CandidateVectorCoverage *pC =
    new CandidateVectorCoverage(table_name, geometry, srid, type);
  if (First == NULL)
    First = pC;
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
}

void CandidateVectorCoveragesList::MarkVectorCoverage(wxString & table,
                                                      wxString & geometry)
{
// marking an already registered Vector Coverage
  CandidateVectorCoverage *pC;
  pC = First;
  while (pC != NULL)
    {
      if (pC->GetTableName().CmpNoCase(table) == 0
          && pC->GetGeometryColumn().CmpNoCase(geometry) == 0)
        {
          pC->MarkVectorCoverage();
          break;
        }
      pC = pC->GetNext();
    }
}

void CandidateVectorCoveragesList::MarkRasterCoverage(wxString & table,
                                                      wxString & geometry)
{
// marking some table related to a Raster Coverage
  CandidateVectorCoverage *pC;
  pC = First;
  while (pC != NULL)
    {
      if (pC->GetTableName().CmpNoCase(table) == 0
          && pC->GetGeometryColumn().CmpNoCase(geometry) == 0)
        {
          pC->MarkRasterCoverage();
          break;
        }
      pC = pC->GetNext();
    }
}

void CandidateVectorCoveragesList::MarkTopoGeoCoverage(wxString & table,
                                                       wxString & geometry)
{
// marking some table related to Topology-Geometry
  CandidateVectorCoverage *pC;
  pC = First;
  while (pC != NULL)
    {
      if (pC->GetTableName().CmpNoCase(table) == 0
          && pC->GetGeometryColumn().CmpNoCase(geometry) == 0)
        {
          pC->MarkTopoGeoCoverage();
          break;
        }
      pC = pC->GetNext();
    }
}

void CandidateVectorCoveragesList::MarkTopoNetCoverage(wxString & table,
                                                       wxString & geometry)
{
// marking some table related to Topology-Network
  CandidateVectorCoverage *pC;
  pC = First;
  while (pC != NULL)
    {
      if (pC->GetTableName().CmpNoCase(table) == 0
          && pC->GetGeometryColumn().CmpNoCase(geometry) == 0)
        {
          pC->MarkTopoNetCoverage();
          break;
        }
      pC = pC->GetNext();
    }
}

CandidateVectorCoveragesList *MyFrame::FindUnregisteredVectorCoverages()
{
// will retrieve all not yet registered GeoTables
  CandidateVectorCoveragesList *list = new CandidateVectorCoveragesList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;

// retrieving all entries from GEOMETRY_COLUMNS
  sql =
    "SELECT f_table_name, f_geometry_column, srid, geometry_type "
    "FROM MAIN.geometry_columns ORDER BY f_table_name, f_geometry_column";
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString table = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString geometry = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          int srid = atoi(value);
          value = results[(i * columns) + 3];
          int gtype = atoi(value);
          wxString type = wxT("***  UNKNOWN  ***");
          switch (gtype)
            {
              case 0:
              case 1000:
              case 2000:
              case 3000:
                type = wxT("GEOMETRY");
                break;
              case 1:
              case 1001:
              case 2001:
              case 3001:
                type = wxT("POINT");
                break;
              case 2:
              case 1002:
              case 2002:
              case 3002:
                type = wxT("LINESTRING");
                break;
              case 3:
              case 1003:
              case 2003:
              case 3003:
                type = wxT("POLYGON");
                break;
              case 4:
              case 1004:
              case 2004:
              case 3004:
                type = wxT("MULTIPOINT");
                break;
              case 5:
              case 1005:
              case 2005:
              case 3005:
                type = wxT("MULTILINESTRING");
                break;
              case 6:
              case 1006:
              case 2006:
              case 3006:
                type = wxT("MULTIPOLYGON");
                break;
              case 7:
              case 1007:
              case 2007:
              case 3007:
                type = wxT("GEOMETRYCOLLECTION");
                break;
              default:
                type = wxT("***  UNKNOWN  ***");
                break;
            };
          switch (gtype)
            {
              case 0:
              case 1:
              case 2:
              case 3:
              case 4:
              case 5:
              case 6:
              case 7:
                type += wxT(" XY");
                break;
              case 1000:
              case 1001:
              case 1002:
              case 1003:
              case 1004:
              case 1005:
              case 1006:
              case 1007:
                type += wxT(" XYZ");
                break;
              case 2000:
              case 2001:
              case 2002:
              case 2003:
              case 2004:
              case 2005:
              case 2006:
              case 2007:
                type += wxT(" XYM");
                break;
              case 3000:
              case 3001:
              case 3002:
              case 3003:
              case 3004:
              case 3005:
              case 3006:
              case 3007:
                type += wxT(" XYZM");
                break;
            };
          list->Add(table, geometry, srid, type);
        }
    }
  sqlite3_free_table(results);

// marking any already registered Vector Coverage
  sql = "SELECT f_table_name, f_geometry_column FROM MAIN.vector_coverages "
    "WHERE f_table_name IS NOT NULL AND f_geometry_column IS NOT NULL";
  ret = sqlite3_get_table(SqliteHandle, sql, &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString table = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString geometry = wxString::FromUTF8(value);
          list->MarkVectorCoverage(table, geometry);
        }
    }
  sqlite3_free_table(results);

// marking any table related to some Raster Coverage
  sql = "SELECT coverage_name FROM MAIN.raster_coverages";
  ret = sqlite3_get_table(SqliteHandle, sql, &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString table = wxString::FromUTF8(value) + wxT("_sections");
          wxString geometry = wxT("geometry");
          list->MarkRasterCoverage(table, geometry);
          table = wxString::FromUTF8(value) + wxT("_tiles");
          list->MarkRasterCoverage(table, geometry);
        }
    }
  sqlite3_free_table(results);

// marking any table related to some Topology-Geometry
  sql = "SELECT topology_name FROM MAIN.topologies";
  ret = sqlite3_get_table(SqliteHandle, sql, &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString table = wxString::FromUTF8(value) + wxT("_face");
          wxString geometry = wxT("mbr");
          list->MarkTopoGeoCoverage(table, geometry);
          geometry = wxT("geom");
          table = wxString::FromUTF8(value) + wxT("_node");
          list->MarkTopoGeoCoverage(table, geometry);
          table = wxString::FromUTF8(value) + wxT("_edge");
          list->MarkTopoGeoCoverage(table, geometry);
          table = wxString::FromUTF8(value) + wxT("_seeds");
          list->MarkTopoGeoCoverage(table, geometry);
        }
    }
  sqlite3_free_table(results);

// marking any table related to some Topology-Network
  sql = "SELECT network_name FROM MAIN.networks";
  ret = sqlite3_get_table(SqliteHandle, sql, &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString table = wxString::FromUTF8(value) + wxT("_link");
          wxString geometry = wxT("geometry");
          list->MarkTopoNetCoverage(table, geometry);
          table = wxString::FromUTF8(value) + wxT("_node");
          list->MarkTopoNetCoverage(table, geometry);
          table = wxString::FromUTF8(value) + wxT("_seeds");
          list->MarkTopoNetCoverage(table, geometry);
        }
    }
  sqlite3_free_table(results);
  return list;
}

CandidateSpatialViewCoveragesList::~CandidateSpatialViewCoveragesList()
{
// destructor
  CandidateSpatialViewCoverage *pC;
  CandidateSpatialViewCoverage *pCn;
  pC = First;
  while (pC != NULL)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
}

void CandidateSpatialViewCoveragesList::Add(wxString & view_name,
                                            wxString & geometry, int srid,
                                            wxString & type)
{
// inserting a new candidate Vector Coverage - SpatialView
  CandidateSpatialViewCoverage *pC =
    new CandidateSpatialViewCoverage(view_name, geometry, srid, type);
  if (First == NULL)
    First = pC;
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
}

void CandidateSpatialViewCoveragesList::MarkVectorCoverage(wxString & view,
                                                           wxString & geometry)
{
// marking an already registered Vector Coverage
  CandidateSpatialViewCoverage *pC;
  pC = First;
  while (pC != NULL)
    {
      if (pC->GetViewName().CmpNoCase(view) == 0
          && pC->GetViewGeometry().CmpNoCase(geometry) == 0)
        {
          pC->MarkVectorCoverage();
          break;
        }
      pC = pC->GetNext();
    }
}

CandidateSpatialViewCoveragesList
  * MyFrame::FindUnregisteredSpatialViewCoverages()
{
// will retrieve all not yet registered SpatialViews
  CandidateSpatialViewCoveragesList *list =
    new CandidateSpatialViewCoveragesList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;

// retrieving all entries from VIEWS_GEOMETRY_COLUMNS
  sql =
    "SELECT v.view_name, v.view_geometry, g.srid, g.geometry_type "
    "FROM views_geometry_columns AS v "
    "JOIN geometry_columns AS g ON (Lower(v.f_table_name) = Lower(g.f_table_name) "
    "AND Lower(v.f_geometry_column) = Lower(g.f_geometry_column)) "
    "ORDER BY v.view_name, v.view_geometry";
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString view = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString geometry = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          int srid = atoi(value);
          value = results[(i * columns) + 3];
          int gtype = atoi(value);
          wxString type = wxT("***  UNKNOWN  ***");
          switch (gtype)
            {
              case 0:
              case 1000:
              case 2000:
              case 3000:
                type = wxT("GEOMETRY");
                break;
              case 1:
              case 1001:
              case 2001:
              case 3001:
                type = wxT("POINT");
                break;
              case 2:
              case 1002:
              case 2002:
              case 3002:
                type = wxT("LINESTRING");
                break;
              case 3:
              case 1003:
              case 2003:
              case 3003:
                type = wxT("POLYGON");
                break;
              case 4:
              case 1004:
              case 2004:
              case 3004:
                type = wxT("MULTIPOINT");
                break;
              case 5:
              case 1005:
              case 2005:
              case 3005:
                type = wxT("MULTILINESTRING");
                break;
              case 6:
              case 1006:
              case 2006:
              case 3006:
                type = wxT("MULTIPOLYGON");
                break;
              case 7:
              case 1007:
              case 2007:
              case 3007:
                type = wxT("GEOMETRYCOLLECTION");
                break;
              default:
                type = wxT("***  UNKNOWN  ***");
                break;
            };
          switch (gtype)
            {
              case 0:
              case 1:
              case 2:
              case 3:
              case 4:
              case 5:
              case 6:
              case 7:
                type += wxT(" XY");
                break;
              case 1000:
              case 1001:
              case 1002:
              case 1003:
              case 1004:
              case 1005:
              case 1006:
              case 1007:
                type += wxT(" XYZ");
                break;
              case 2000:
              case 2001:
              case 2002:
              case 2003:
              case 2004:
              case 2005:
              case 2006:
              case 2007:
                type += wxT(" XYM");
                break;
              case 3000:
              case 3001:
              case 3002:
              case 3003:
              case 3004:
              case 3005:
              case 3006:
              case 3007:
                type += wxT(" XYZM");
                break;
            };
          list->Add(view, geometry, srid, type);
        }
    }
  sqlite3_free_table(results);

// marking any already registered SpatialView Coverage
  sql = "SELECT view_name, view_geometry FROM MAIN.vector_coverages "
    "WHERE view_name IS NOT NULL AND view_geometry IS NOT NULL";
  ret = sqlite3_get_table(SqliteHandle, sql, &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString table = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString geometry = wxString::FromUTF8(value);
          list->MarkVectorCoverage(table, geometry);
        }
    }
  sqlite3_free_table(results);
  return list;
}

CandidateVirtualTableCoveragesList::~CandidateVirtualTableCoveragesList()
{
// destructor
  CandidateVirtualTableCoverage *pC;
  CandidateVirtualTableCoverage *pCn;
  pC = First;
  while (pC != NULL)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
}

void CandidateVirtualTableCoveragesList::Add(wxString & virt_name,
                                             wxString & geometry, int srid,
                                             wxString & type)
{
// inserting a new candidate Vector Coverage - VirtualTable
  CandidateVirtualTableCoverage *pC =
    new CandidateVirtualTableCoverage(virt_name, geometry, srid, type);
  if (First == NULL)
    First = pC;
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
}

void CandidateVirtualTableCoveragesList::MarkVectorCoverage(wxString & table,
                                                            wxString & geometry)
{
// marking an already registered Vector Coverage
  CandidateVirtualTableCoverage *pC;
  pC = First;
  while (pC != NULL)
    {
      if (pC->GetVirtName().CmpNoCase(table) == 0
          && pC->GetVirtGeometry().CmpNoCase(geometry) == 0)
        {
          pC->MarkVectorCoverage();
          break;
        }
      pC = pC->GetNext();
    }
}

CandidateVirtualTableCoveragesList
  * MyFrame::FindUnregisteredVirtualTableCoverages()
{
// will retrieve all not yet registered VirtualTable
  CandidateVirtualTableCoveragesList *list =
    new CandidateVirtualTableCoveragesList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;

// retrieving all entries from VIERTS_GEOMETRY_COLUMNS
  sql =
    "SELECT virt_name, virt_geometry, srid, geometry_type "
    "FROM virts_geometry_columns AS v " "ORDER BY virt_name, v.virt_geometry";
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString table = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString geometry = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          int srid = atoi(value);
          value = results[(i * columns) + 3];
          int gtype = atoi(value);
          wxString type = wxT("***  UNKNOWN  ***");
          switch (gtype)
            {
              case 0:
              case 1000:
              case 2000:
              case 3000:
                type = wxT("GEOMETRY");
                break;
              case 1:
              case 1001:
              case 2001:
              case 3001:
                type = wxT("POINT");
                break;
              case 2:
              case 1002:
              case 2002:
              case 3002:
                type = wxT("LINESTRING");
                break;
              case 3:
              case 1003:
              case 2003:
              case 3003:
                type = wxT("POLYGON");
                break;
              case 4:
              case 1004:
              case 2004:
              case 3004:
                type = wxT("MULTIPOINT");
                break;
              case 5:
              case 1005:
              case 2005:
              case 3005:
                type = wxT("MULTILINESTRING");
                break;
              case 6:
              case 1006:
              case 2006:
              case 3006:
                type = wxT("MULTIPOLYGON");
                break;
              case 7:
              case 1007:
              case 2007:
              case 3007:
                type = wxT("GEOMETRYCOLLECTION");
                break;
              default:
                type = wxT("***  UNKNOWN  ***");
                break;
            };
          switch (gtype)
            {
              case 0:
              case 1:
              case 2:
              case 3:
              case 4:
              case 5:
              case 6:
              case 7:
                type += wxT(" XY");
                break;
              case 1000:
              case 1001:
              case 1002:
              case 1003:
              case 1004:
              case 1005:
              case 1006:
              case 1007:
                type += wxT(" XYZ");
                break;
              case 2000:
              case 2001:
              case 2002:
              case 2003:
              case 2004:
              case 2005:
              case 2006:
              case 2007:
                type += wxT(" XYM");
                break;
              case 3000:
              case 3001:
              case 3002:
              case 3003:
              case 3004:
              case 3005:
              case 3006:
              case 3007:
                type += wxT(" XYZM");
                break;
            };
          list->Add(table, geometry, srid, type);
        }
    }
  sqlite3_free_table(results);

// marking any already registered VirtualShape Coverage
  sql = "SELECT virt_name, virt_geometry FROM MAIN.vector_coverages "
    "WHERE virt_name IS NOT NULL AND virt_geometry IS NOT NULL";
  ret = sqlite3_get_table(SqliteHandle, sql, &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString table = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString geometry = wxString::FromUTF8(value);
          list->MarkVectorCoverage(table, geometry);
        }
    }
  sqlite3_free_table(results);
  return list;
}

CandidateTopoGeoCoveragesList::~CandidateTopoGeoCoveragesList()
{
// destructor
  CandidateTopoGeoCoverage *pC;
  CandidateTopoGeoCoverage *pCn;
  pC = First;
  while (pC != NULL)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
}

void CandidateTopoGeoCoveragesList::Add(wxString & topology, int srid,
                                        wxString & dimensions, double tolerance)
{
// inserting a new candidate TopoGeo Coverage
  CandidateTopoGeoCoverage *pC =
    new CandidateTopoGeoCoverage(topology, srid, dimensions, tolerance);
  if (First == NULL)
    First = pC;
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
}

void CandidateTopoGeoCoveragesList::MarkVectorCoverage(wxString & topology)
{
// marking an already registered TopoGeo Coverage
  CandidateTopoGeoCoverage *pC;
  pC = First;
  while (pC != NULL)
    {
      if (pC->GetTopologyName().CmpNoCase(topology) == 0)
        {
          pC->MarkVectorCoverage();
          break;
        }
      pC = pC->GetNext();
    }
}

CandidateTopoGeoCoveragesList *MyFrame::FindUnregisteredTopoGeoCoverages()
{
// will retrieve all not yet registered Topology-Geometries
  CandidateTopoGeoCoveragesList *list = new CandidateTopoGeoCoveragesList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;

// retrieving all entries from TOPOLOGIES
  sql =
    "SELECT topology_name, srid, has_z, tolerance "
    "FROM topologies ORDER BY topology_name";
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString topology = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          int srid = atoi(value);
          wxString dimensions = wxT("2D");
          value = results[(i * columns) + 2];
          if (atoi(value))
            dimensions = wxT("3D");
          value = results[(i * columns) + 3];
          double tolerance = atof(value);
          list->Add(topology, srid, dimensions, tolerance);
        }
    }
  sqlite3_free_table(results);

// marking any already registered Vector Coverage
  sql = "SELECT topology_name FROM MAIN.vector_coverages "
    "WHERE topology_name IS NOT NULL";
  ret = sqlite3_get_table(SqliteHandle, sql, &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString topology = wxString::FromUTF8(value);
          list->MarkVectorCoverage(topology);
        }
    }
  sqlite3_free_table(results);
  return list;
}

bool MyFrame::DoRegisterVectorCoverage(wxString & name, wxString & table,
                                       wxString & geometry, wxString & title,
                                       wxString & abstract,
                                       wxString & copyright, wxString & license,
                                       bool isQueryable, bool isEditable)
{
//
// attempting to register a Vector Coverage - SpatialTable
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_RegisterVectorCoverage(?, ?, ?, ?, ?, ?, ?)";
  int ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((table.Len() * 4) + 1);
  strcpy(dummy, table.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((geometry.Len() * 4) + 1);
  strcpy(dummy, geometry.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((title.Len() * 4) + 1);
  strcpy(dummy, title.ToUTF8());
  sqlite3_bind_text(stmt, 4, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((abstract.Len() * 4) + 1);
  strcpy(dummy, abstract.ToUTF8());
  sqlite3_bind_text(stmt, 5, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (isQueryable == false)
    sqlite3_bind_int(stmt, 6, 0);
  else
    sqlite3_bind_int(stmt, 6, 1);
  if (isEditable == false)
    sqlite3_bind_int(stmt, 7, 0);
  else
    sqlite3_bind_int(stmt, 7, 1);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;

  sql = "SELECT SE_SetVectorCoverageCopyright(?, ?, ?)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (copyright.Len() == 0)
    sqlite3_bind_null(stmt, 2);
  else
    {
      dummy = (char *) malloc((copyright.Len() * 4) + 1);
      strcpy(dummy, copyright.ToUTF8());
      sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
      free(dummy);
    }
  dummy = (char *) malloc((license.Len() * 4) + 1);
  strcpy(dummy, license.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  return true;
}

bool MyFrame::DoRegisterSpatialViewCoverage(wxString & name, wxString & view,
                                            wxString & geometry,
                                            wxString & title,
                                            wxString & abstract,
                                            wxString & copyright,
                                            wxString & license,
                                            bool isQueryable, bool isEditable)
{
//
// attempting to register a Vector Coverage - SpatialView
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql =
    "SELECT SE_RegisterSpatialViewCoverage(?, ?, ?, ?, ?, ?, ?)";
  int ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((view.Len() * 4) + 1);
  strcpy(dummy, view.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((geometry.Len() * 4) + 1);
  strcpy(dummy, geometry.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((title.Len() * 4) + 1);
  strcpy(dummy, title.ToUTF8());
  sqlite3_bind_text(stmt, 4, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((abstract.Len() * 4) + 1);
  strcpy(dummy, abstract.ToUTF8());
  sqlite3_bind_text(stmt, 5, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (isQueryable == false)
    sqlite3_bind_int(stmt, 6, 0);
  else
    sqlite3_bind_int(stmt, 6, 1);
  if (isEditable == false)
    sqlite3_bind_int(stmt, 7, 0);
  else
    sqlite3_bind_int(stmt, 7, 1);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;

  sql = "SELECT SE_SetVectorCoverageCopyright(?, ?, ?)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (copyright.Len() == 0)
    sqlite3_bind_null(stmt, 2);
  else
    {
      dummy = (char *) malloc((copyright.Len() * 4) + 1);
      strcpy(dummy, copyright.ToUTF8());
      sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
      free(dummy);
    }
  dummy = (char *) malloc((license.Len() * 4) + 1);
  strcpy(dummy, license.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  return true;
  return true;
}

bool MyFrame::DoRegisterVirtualTableCoverage(wxString & name, wxString & table,
                                             wxString & geometry,
                                             wxString & title,
                                             wxString & abstract,
                                             wxString & copyright,
                                             wxString & license,
                                             bool isQueryable)
{
//
// attempting to register a Vector Coverage - VirtualTable
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_RegisterVirtualTableCoverage(?, ?, ?, ?, ?, ?)";
  int ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((table.Len() * 4) + 1);
  strcpy(dummy, table.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((geometry.Len() * 4) + 1);
  strcpy(dummy, geometry.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((title.Len() * 4) + 1);
  strcpy(dummy, title.ToUTF8());
  sqlite3_bind_text(stmt, 4, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((abstract.Len() * 4) + 1);
  strcpy(dummy, abstract.ToUTF8());
  sqlite3_bind_text(stmt, 5, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (isQueryable == false)
    sqlite3_bind_int(stmt, 6, 0);
  else
    sqlite3_bind_int(stmt, 6, 1);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;

  sql = "SELECT SE_SetVectorCoverageCopyright(?, ?, ?)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (copyright.Len() == 0)
    sqlite3_bind_null(stmt, 2);
  else
    {
      dummy = (char *) malloc((copyright.Len() * 4) + 1);
      strcpy(dummy, copyright.ToUTF8());
      sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
      free(dummy);
    }
  dummy = (char *) malloc((license.Len() * 4) + 1);
  strcpy(dummy, license.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  return true;
  return true;
}

bool MyFrame::DoRegisterTopoGeoCoverage(wxString & name, wxString & topology,
                                        wxString & title, wxString & abstract,
                                        wxString & copyright,
                                        wxString & license, bool isQueryable,
                                        bool isEditable)
{
//
// attempting to register a TopoGeo Coverage
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_RegisterTopoGeoCoverage(?, ?, ?, ?, ?, ?)";
  int ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((topology.Len() * 4) + 1);
  strcpy(dummy, topology.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((title.Len() * 4) + 1);
  strcpy(dummy, title.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((abstract.Len() * 4) + 1);
  strcpy(dummy, abstract.ToUTF8());
  sqlite3_bind_text(stmt, 4, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (isQueryable == false)
    sqlite3_bind_int(stmt, 5, 0);
  else
    sqlite3_bind_int(stmt, 5, 1);
  if (isEditable == false)
    sqlite3_bind_int(stmt, 6, 0);
  else
    sqlite3_bind_int(stmt, 6, 1);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;

  sql = "SELECT SE_SetVectorCoverageCopyright(?, ?, ?)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (copyright.Len() == 0)
    sqlite3_bind_null(stmt, 2);
  else
    {
      dummy = (char *) malloc((copyright.Len() * 4) + 1);
      strcpy(dummy, copyright.ToUTF8());
      sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
      free(dummy);
    }
  dummy = (char *) malloc((license.Len() * 4) + 1);
  strcpy(dummy, license.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  return true;
  return true;
}

CandidateTopoNetCoveragesList::~CandidateTopoNetCoveragesList()
{
// destructor
  CandidateTopoNetCoverage *pC;
  CandidateTopoNetCoverage *pCn;
  pC = First;
  while (pC != NULL)
    {
      pCn = pC->GetNext();
      delete pC;
      pC = pCn;
    }
}

void CandidateTopoNetCoveragesList::Add(wxString & network, int srid,
                                        wxString & dimensions)
{
// inserting a new candidate TopoNet Coverage
  CandidateTopoNetCoverage *pC =
    new CandidateTopoNetCoverage(network, srid, dimensions);
  if (First == NULL)
    First = pC;
  if (Last != NULL)
    Last->SetNext(pC);
  Last = pC;
}

void CandidateTopoNetCoveragesList::MarkVectorCoverage(wxString & network)
{
// marking an already registered TopoNet Coverage
  CandidateTopoNetCoverage *pC;
  pC = First;
  while (pC != NULL)
    {
      if (pC->GetNetworkName().CmpNoCase(network) == 0)
        {
          pC->MarkVectorCoverage();
          break;
        }
      pC = pC->GetNext();
    }
}

CandidateTopoNetCoveragesList *MyFrame::FindUnregisteredTopoNetCoverages()
{
// will retrieve all not yet registered Topology-Networks
  CandidateTopoNetCoveragesList *list = new CandidateTopoNetCoveragesList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;

// retrieving all entries from NETWORKS
  sql = "SELECT network_name, srid, has_z FROM networks ORDER BY network_name";
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString topology = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          int srid = atoi(value);
          wxString dimensions = wxT("2D");
          value = results[(i * columns) + 2];
          if (atoi(value))
            dimensions = wxT("3D");
          list->Add(topology, srid, dimensions);
        }
    }
  sqlite3_free_table(results);

// marking any already registered Vector Coverage
  sql = "SELECT network_name FROM MAIN.vector_coverages "
    "WHERE network_name IS NOT NULL";
  ret = sqlite3_get_table(SqliteHandle, sql, &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString topology = wxString::FromUTF8(value);
          list->MarkVectorCoverage(topology);
        }
    }
  sqlite3_free_table(results);
  return list;
}

bool MyFrame::DoRegisterTopoNetCoverage(wxString & name, wxString & network,
                                        wxString & title, wxString & abstract,
                                        wxString & copyright,
                                        wxString & license, bool isQueryable,
                                        bool isEditable)
{
//
// attempting to register a TopoNet Coverage
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_RegisterTopoNetCoverage(?, ?, ?, ?, ?, ?)";
  int ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((network.Len() * 4) + 1);
  strcpy(dummy, network.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((title.Len() * 4) + 1);
  strcpy(dummy, title.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((abstract.Len() * 4) + 1);
  strcpy(dummy, abstract.ToUTF8());
  sqlite3_bind_text(stmt, 4, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (isQueryable == false)
    sqlite3_bind_int(stmt, 5, 0);
  else
    sqlite3_bind_int(stmt, 5, 1);
  if (isEditable == false)
    sqlite3_bind_int(stmt, 6, 0);
  else
    sqlite3_bind_int(stmt, 6, 1);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;

  sql = "SELECT SE_SetVectorCoverageCopyright(?, ?, ?)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((name.Len() * 4) + 1);
  strcpy(dummy, name.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (copyright.Len() == 0)
    sqlite3_bind_null(stmt, 2);
  else
    {
      dummy = (char *) malloc((copyright.Len() * 4) + 1);
      strcpy(dummy, copyright.ToUTF8());
      sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
      free(dummy);
    }
  dummy = (char *) malloc((license.Len() * 4) + 1);
  strcpy(dummy, license.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  return true;
  return true;
}

bool VectorCoverageStylesDialog::Create(MyFrame * parent, wxString & coverage)
{
//
// creating the dialog
//
  MainFrame = parent;
  wxString clean;
  if (coverage.StartsWith(wxT("table."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("view."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("shp."), &clean))
    coverage = clean;
  int pos = coverage.Find(wxT(" [SRID="));
  if (pos != wxNOT_FOUND)
    CoverageName = coverage.Left(pos);
  else
    CoverageName = coverage;
  List = MainFrame->FindVectorCoverageStyles(CoverageName);
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Vector Coverage supported SLD/SE Styles")) == false)
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

void VectorCoverageStylesDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(350, 22),
                                       wxTE_READONLY);
  cvgCtrl->Enable(false);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered SLD/SE Styles"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  VectorCoverageStyle *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Style ID"));
  GridCtrl->SetColLabelValue(1, wxT("Name"));
  GridCtrl->SetColLabelValue(2, wxT("Title"));
  GridCtrl->SetColLabelValue(3, wxT("Abstract"));
  GridCtrl->SetColLabelValue(4, wxT("Schema Validated"));
  GridCtrl->SetColLabelValue(5, wxT("Schema URI"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      sprintf(dummy, "%d", pS->GetStyleID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 0, cell);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 1, pS->GetName());
      GridCtrl->SetCellValue(count, 2, pS->GetTitle());
      GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
      GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
      GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *quit = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(quit, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *addNew = new wxButton(this, ID_SLD_SE_ADD, wxT("&Add Style(s)"));
  btnBox->Add(addNew, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorCoverageStylesDialog::OnQuit);
  Connect(ID_SLD_SE_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorCoverageStylesDialog::OnAddStyle);
  Connect(ID_SLD_SE_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          VectorCoverageStylesDialog::OnCmdRemoveStyle);
  Connect(wxID_ANY, wxEVT_GRID_CELL_RIGHT_CLICK,
          (wxObjectEventFunction) & VectorCoverageStylesDialog::OnRightClick);
}

void VectorCoverageStylesDialog::OnRightClick(wxGridEvent & event)
{
//
// right click on some cell [mouse action]
//
  wxMenu menu;
  wxMenuItem *menuItem;
  wxPoint pt = event.GetPosition();
  CurrentRow = event.GetRow();
  GridCtrl->SelectRow(CurrentRow);
  wxString id = GridCtrl->GetCellValue(CurrentRow, 0);
  long style_id;
  id.ToLong(&style_id);
  CurrentStyleID = style_id;
  menuItem = new wxMenuItem(&menu, ID_SLD_SE_REMOVE, wxT("&Remove"));
  menu.Append(menuItem);
  GridCtrl->PopupMenu(&menu, pt);
}

void VectorCoverageStylesDialog::
OnCmdRemoveStyle(wxCommandEvent & WXUNUSED(event))
{
//
// deleting a Style [mouse action]
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_UnRegisterVectorStyledLayer(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_int(stmt, 2, CurrentStyleID);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    GridCtrl->DeleteRows(CurrentRow, 1);
  sqlite3_finalize(stmt);
}

void VectorCoverageStylesDialog::
DoRegistetVectorCoverageStyles(ListVectorStylesDialog * dlg)
{
//
// attempting to register the Vector Coverage Style(s)
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_RegisterVectorStyledLayer(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return;

  int max = dlg->GetSelectedCount();
  for (int i = 0; i < max; i++)
    {
      int style_id = dlg->GetSelectedStyleId(i);
      if (style_id >= 0)
        {
          sqlite3_reset(stmt);
          sqlite3_clear_bindings(stmt);
          dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
          strcpy(dummy, CoverageName.ToUTF8());
          sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
          free(dummy);
          sqlite3_bind_int(stmt, 2, style_id);
          ret = sqlite3_step(stmt);
          if (ret == SQLITE_DONE || ret == SQLITE_ROW)
            ;
          else
            {
              sqlite3_finalize(stmt);
              return;
            }
        }
    }
  sqlite3_finalize(stmt);
}

void VectorCoverageStylesDialog::OnAddStyle(wxCommandEvent & WXUNUSED(event))
{
//
// adding a new Style [button action]
//
  ListVectorStylesDialog dlg;
  dlg.Create(MainFrame);
  if (dlg.ShowModal() == wxID_OK)
    {
      // attempting to register the new styles
      DoRegistetVectorCoverageStyles(&dlg);

      // updating the Grid
      int tot_rows = GridCtrl->GetNumberRows();
      if (tot_rows > 0)
        GridCtrl->DeleteRows(0, tot_rows);
      if (List != NULL)
        delete List;
      List = MainFrame->FindVectorCoverageStyles(CoverageName);
      int count = 0;
      VectorCoverageStyle *pS = List->GetFirst();
      while (pS)
        {
          // counting how many lines are there
          count++;
          pS = pS->GetNext();
        }
      GridCtrl->AppendRows(count);
      count = 0;
      char dummy[1024];
      wxString cell;
      pS = List->GetFirst();
      while (pS)
        {
          // feeding grid rows
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          sprintf(dummy, "%d", pS->GetStyleID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 0, cell);
          GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 1, pS->GetName());
          GridCtrl->SetCellValue(count, 2, pS->GetTitle());
          GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
          GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
          GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
          count++;
          pS = pS->GetNext();
        }
      GridCtrl->AutoSizeColumns();
    }
}

void VectorCoverageStylesDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool ListVectorStylesDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  List = MainFrame->FindVectorStyles();
  if (wxDialog::Create(parent, wxID_ANY, wxT("Registered SLD/SE Vector Styles"))
      == false)
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

void ListVectorStylesDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered SLD/SE Styles"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  VectorCoverageStyle *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Style ID"));
  GridCtrl->SetColLabelValue(1, wxT("Name"));
  GridCtrl->SetColLabelValue(2, wxT("Title"));
  GridCtrl->SetColLabelValue(3, wxT("Abstract"));
  GridCtrl->SetColLabelValue(4, wxT("Schema Validated"));
  GridCtrl->SetColLabelValue(5, wxT("Schema URI"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      sprintf(dummy, "%d", pS->GetStyleID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 0, cell);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 1, pS->GetName());
      GridCtrl->SetCellValue(count, 2, pS->GetTitle());
      GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
      GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
      GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Ok"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & ListVectorStylesDialog::OnOk);
}

int ListVectorStylesDialog::GetSelectedCount()
{
//
// computing how many selected Style there are
//
  int count = 0;
  VectorCoverageStyle *pS = List->GetFirst();
  while (pS != NULL)
    {
      if (pS->IsSelected() == true)
        count++;
      pS = pS->GetNext();
    }
  return count;
}

int ListVectorStylesDialog::GetSelectedStyleId(int idx)
{
//
// returning the Nth selected StyleID
//
  int count = 0;
  VectorCoverageStyle *pS = List->GetFirst();
  while (pS != NULL)
    {
      if (pS->IsSelected() == true)
        {
          if (count == idx)
            return pS->GetStyleID();
          count++;
        }
      pS = pS->GetNext();
    }
  return -1;
}

void ListVectorStylesDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          wxString cell_id = GridCtrl->GetCellValue(i, 0);
          long style_id;
          if (cell_id.ToLong(&style_id) == true)
            List->MarkSelected(style_id);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

VectorCoverageStyle::VectorCoverageStyle(int style_id, wxString & name,
                                         wxString & title, wxString & abstract,
                                         wxString & validated,
                                         wxString & schema_uri)
{
// constructor
  StyleID = style_id;
  Name = name;
  Title = title;
  Abstract = abstract;
  SchemaValidated = validated;
  SchemaURI = schema_uri;
  Selected = false;
  Next = NULL;
}

VectorCoverageStylesList::VectorCoverageStylesList()
{
// constructor
  First = NULL;
  Last = NULL;
}

VectorCoverageStylesList::~VectorCoverageStylesList()
{
// destructor
  VectorCoverageStyle *pS;
  VectorCoverageStyle *pSn;
  pS = First;
  while (pS != NULL)
    {
      pSn = pS->GetNext();
      delete pS;
      pS = pSn;
    }
}

void VectorCoverageStylesList::Add(int style_id, wxString & name,
                                   wxString & title, wxString & abstract,
                                   wxString & validated, wxString & schema_uri)
{
// inserting a new Style
  VectorCoverageStyle *pS =
    new VectorCoverageStyle(style_id, name, title, abstract, validated,
                            schema_uri);
  if (First == NULL)
    First = pS;
  if (Last != NULL)
    Last->SetNext(pS);
  Last = pS;
}

void VectorCoverageStylesList::MarkSelected(int styleId)
{
// marking a Selected Style
  VectorCoverageStyle *pS;
  pS = First;
  while (pS != NULL)
    {
      if (pS->GetStyleID() == styleId)
        {
          pS->MarkSelected();
          break;
        }
      pS = pS->GetNext();
    }
}

VectorCoverageStylesList *MyFrame::FindVectorCoverageStyles(wxString & coverage)
{
// will retrieve all SLD/SE Styles for the given Coverage
  VectorCoverageStylesList *list = new VectorCoverageStylesList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  wxString sql;
  char *xsql;

  sql =
    wxT
    ("SELECT style_id, name, title, abstract, schema_validated, schema_uri ");
  sql +=
    wxT("FROM SE_Vector_styled_layers_view WHERE coverage_name = '") + coverage;
  sql += wxT("' ORDER BY style_id");
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
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          int style_id = atoi(value);
          value = results[(i * columns) + 1];
          wxString name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString validated = wxT("Yes");
          if (atoi(value) == 0)
            validated = wxT("No");
          value = results[(i * columns) + 5];
          wxString schema_uri = wxString::FromUTF8(value);
          list->Add(style_id, name, title, abstract, validated, schema_uri);
        }
    }
  sqlite3_free_table(results);
  return list;
}

VectorCoverageStylesList *MyFrame::FindVectorStyles()
{
// will retrieve all registered SLD/SE Vector Styles
  VectorCoverageStylesList *list = new VectorCoverageStylesList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;

  sql =
    "SELECT style_id, style_name, XB_GetTitle(style), XB_GetAbstract(style), "
    "XB_IsSchemaValidated(style), XB_GetSchemaURI(style) "
    "FROM SE_Vector_styles ORDER BY style_name";
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          int style_id = atoi(value);
          value = results[(i * columns) + 1];
          wxString name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString validated = wxT("Yes");
          if (atoi(value) == 0)
            validated = wxT("No");
          value = results[(i * columns) + 5];
          wxString schema_uri = wxString::FromUTF8(value);
          list->Add(style_id, name, title, abstract, validated, schema_uri);
        }
    }
  sqlite3_free_table(results);
  return list;
}

bool VectorRegisterDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  LicenseID = 0;
  List = MainFrame->FindUnregisteredVectorCoverages();
  if (wxDialog::Create(parent, wxID_ANY, wxT("Register SpatialTable Coverage"))
      == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  int count = 0;
  CandidateVectorCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true && pC->IsRasterCoverage() != true
          && pC->IsTopoGeo() != true && pC->IsTopoNet() != true)
        count++;
      pC = pC->GetNext();
    }
  if (count == 0)
    {
      wxButton *ok = (wxButton *) FindWindow(wxID_OK);
      ok->Enable(false);
      wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
      nameCtrl->Enable(false);
      wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
      titleCtrl->Enable(false);
      wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
      absCtrl->Enable(false);
      wxMessageBox(wxT
                   ("There are no possible Candidates to be eventually registered"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
    }
  return true;
}

void VectorRegisterDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT
                                         ("Candidate SpatialTable Coverages"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_VECTOR_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  CandidateVectorCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true && pC->IsRasterCoverage() != true
          && pC->IsTopoGeo() != true && pC->IsTopoNet() != true)
        count++;
      pC = pC->GetNext();
    }
  GridCtrl->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("f_table_name"));
  GridCtrl->SetColLabelValue(1, wxT("f_geometry_column"));
  GridCtrl->SetColLabelValue(2, wxT("SRID"));
  GridCtrl->SetColLabelValue(3, wxT("GeometryType"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pC = List->GetFirst();
  while (pC)
    {
      // feeding grid rows
      if (pC->IsVectorCoverage() != true && pC->IsRasterCoverage() != true
          && pC->IsTopoGeo() != true && pC->IsTopoNet() != true)
        {
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          GridCtrl->SetCellValue(count, 0, pC->GetTableName());
          GridCtrl->SetCellValue(count, 1, pC->GetGeometryColumn());
          sprintf(dummy, "%d", pC->GetSRID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 2, cell);
          GridCtrl->SetCellAlignment(count, 2, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 3, pC->GetGeometryType());
          count++;
        }
      pC = pC->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_VECTOR_COVERAGE, wxT(""),
                                        wxDefaultPosition, wxSize(550, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_VECTOR_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(550, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, ID_VECTOR_ABSTRACT, wxT(""),
                                            wxDefaultPosition, wxSize(550, 60),
                                            wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyrightCtrl =
    new wxTextCtrl(this, ID_VECTOR_COPYRIGHT, Copyright,
                   wxDefaultPosition, wxSize(550, 60),
                   wxTE_MULTILINE);
  copySizer->Add(copyrightCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Data &License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *licenseCtrl =
    new wxComboBox(this, ID_VECTOR_LICENSE, wxT(""), wxDefaultPosition,
                   wxSize(550, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  licSizer->Add(licenseCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  PopulateDataLicenses(licenseCtrl);
// seventh row: queryable / editable
  wxBoxSizer *radioSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(radioSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString yesno[2];
  yesno[0] = wxT("&No");
  yesno[1] = wxT("&Yes");
  wxRadioBox *queryableBox = new wxRadioBox(this, ID_VECTOR_QUERYABLE,
                                            wxT("Is &Queryable"),
                                            wxDefaultPosition,
                                            wxDefaultSize, 2,
                                            yesno, 1,
                                            wxRA_SPECIFY_ROWS);
  radioSizer->Add(queryableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  queryableBox->SetSelection(1);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  wxRadioBox *editableBox = new wxRadioBox(this, ID_VECTOR_EDITABLE,
                                           wxT("Is &Editable"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 2,
                                           yesno, 1,
                                           wxRA_SPECIFY_ROWS);
  radioSizer->Add(editableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  editableBox->SetSelection(1);

// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Register"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorRegisterDialog::OnOk);
}

void VectorRegisterDialog::PopulateDataLicenses(wxComboBox * licCtrl)
{
// populating the Data Licenses List
  const char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int sel = -1;

  sql =
    sqlite3_mprintf("SELECT id, name FROM MAIN.data_licenses ORDER BY name");
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      int id = atoi(results[(i * columns) + 0]);
      const char *name = results[(i * columns) + 1];
      wxString str = wxString::FromUTF8(name);
      licCtrl->Append(str);
      if (id == LicenseID)
        sel = i - 1;
    }
  sqlite3_free_table(results);
  licCtrl->SetSelection(sel);
}

void VectorRegisterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
  wxTextCtrl *copyCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COPYRIGHT);
  wxComboBox *licCtrl = (wxComboBox *) FindWindow(ID_VECTOR_LICENSE);
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_VECTOR_QUERYABLE);
  wxRadioBox *editableBox = (wxRadioBox *) FindWindow(ID_VECTOR_EDITABLE);
  wxString str = nameCtrl->GetValue().Trim();
  CoverageName = str.Trim(false);
  if (CoverageName.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Coverage Name !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = titleCtrl->GetValue().Trim();
  Title = str.Trim(false);
  if (Title.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Title !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = absCtrl->GetValue().Trim();
  Abstract = str.Trim(false);
  if (Abstract.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Abstract !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  int selCount = 0;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          TableName = GridCtrl->GetCellValue(i, 0);
          GeometryColumn = GridCtrl->GetCellValue(i, 1);
          selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT("You must select a Candidate to be registered !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT("You must select just a single Candidate to be registered !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = copyCtrl->GetValue().Trim();
  Copyright = str.Trim(false);
  DataLicense = licCtrl->GetValue();
  if (queryableBox->GetSelection() == 1)
    Queryable = true;
  else
    Queryable = false;
  if (editableBox->GetSelection() == 1)
    Editable = true;
  else
    Editable = false;
  wxDialog::EndModal(wxID_OK);
}

bool SpatialViewRegisterDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  LicenseID = 0;
  List = MainFrame->FindUnregisteredSpatialViewCoverages();
  if (wxDialog::Create(parent, wxID_ANY, wxT("Register SpatialView Coverage"))
      == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  int count = 0;
  CandidateSpatialViewCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true)
        count++;
      pC = pC->GetNext();
    }
  if (count == 0)
    {
      wxButton *ok = (wxButton *) FindWindow(wxID_OK);
      ok->Enable(false);
      wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
      nameCtrl->Enable(false);
      wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
      titleCtrl->Enable(false);
      wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
      absCtrl->Enable(false);
      wxMessageBox(wxT
                   ("There are no possible Candidates to be eventually registered"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
    }
  return true;
}

void SpatialViewRegisterDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Candidate SpatialView Coverages"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_VECTOR_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  CandidateSpatialViewCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true)
        count++;
      pC = pC->GetNext();
    }
  GridCtrl->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("view_name"));
  GridCtrl->SetColLabelValue(1, wxT("view_geometry"));
  GridCtrl->SetColLabelValue(2, wxT("SRID"));
  GridCtrl->SetColLabelValue(3, wxT("GeometryType"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pC = List->GetFirst();
  while (pC)
    {
      // feeding grid rows
      if (pC->IsVectorCoverage() != true)
        {
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          GridCtrl->SetCellValue(count, 0, pC->GetViewName());
          GridCtrl->SetCellValue(count, 1, pC->GetViewGeometry());
          sprintf(dummy, "%d", pC->GetSRID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 2, cell);
          GridCtrl->SetCellAlignment(count, 2, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 3, pC->GetGeometryType());
          count++;
        }
      pC = pC->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_VECTOR_COVERAGE, wxT(""),
                                        wxDefaultPosition, wxSize(550, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_VECTOR_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(550, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, ID_VECTOR_ABSTRACT, wxT(""),
                                            wxDefaultPosition, wxSize(550, 60),
                                            wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyrightCtrl =
    new wxTextCtrl(this, ID_VECTOR_COPYRIGHT, Copyright,
                   wxDefaultPosition, wxSize(550, 60),
                   wxTE_MULTILINE);
  copySizer->Add(copyrightCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Data &License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *licenseCtrl =
    new wxComboBox(this, ID_VECTOR_LICENSE, wxT(""), wxDefaultPosition,
                   wxSize(550, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  licSizer->Add(licenseCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  PopulateDataLicenses(licenseCtrl);
// seventh row: queryable / editable
  wxBoxSizer *radioSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(radioSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString yesno[2];
  yesno[0] = wxT("&No");
  yesno[1] = wxT("&Yes");
  wxRadioBox *queryableBox = new wxRadioBox(this, ID_VECTOR_QUERYABLE,
                                            wxT("Is &Queryable"),
                                            wxDefaultPosition,
                                            wxDefaultSize, 2,
                                            yesno, 1,
                                            wxRA_SPECIFY_ROWS);
  radioSizer->Add(queryableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  queryableBox->SetSelection(1);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  wxRadioBox *editableBox = new wxRadioBox(this, ID_VECTOR_EDITABLE,
                                           wxT("Is &Editable"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 2,
                                           yesno, 1,
                                           wxRA_SPECIFY_ROWS);
  radioSizer->Add(editableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  editableBox->SetSelection(1);

// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Register"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SpatialViewRegisterDialog::OnOk);
}

void SpatialViewRegisterDialog::PopulateDataLicenses(wxComboBox * licCtrl)
{
// populating the Data Licenses List
  const char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int sel = -1;

  sql =
    sqlite3_mprintf("SELECT id, name FROM MAIN.data_licenses ORDER BY name");
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      int id = atoi(results[(i * columns) + 0]);
      const char *name = results[(i * columns) + 1];
      wxString str = wxString::FromUTF8(name);
      licCtrl->Append(str);
      if (id == LicenseID)
        sel = i - 1;
    }
  sqlite3_free_table(results);
  licCtrl->SetSelection(sel);
}

void SpatialViewRegisterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
  wxTextCtrl *copyCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COPYRIGHT);
  wxComboBox *licCtrl = (wxComboBox *) FindWindow(ID_VECTOR_LICENSE);
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_VECTOR_QUERYABLE);
  wxRadioBox *editableBox = (wxRadioBox *) FindWindow(ID_VECTOR_EDITABLE);
  wxString str = nameCtrl->GetValue().Trim();
  CoverageName = str.Trim(false);
  if (CoverageName.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Coverage Name !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = titleCtrl->GetValue().Trim();
  Title = str.Trim(false);
  if (Title.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Title !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = absCtrl->GetValue().Trim();
  Abstract = str.Trim(false);
  if (Abstract.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Abstract !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  int selCount = 0;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          ViewName = GridCtrl->GetCellValue(i, 0);
          ViewGeometry = GridCtrl->GetCellValue(i, 1);
          selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT("You must select a Candidate to be registered !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT("You must select just a single Candidate to be registered !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = copyCtrl->GetValue().Trim();
  Copyright = str.Trim(false);
  DataLicense = licCtrl->GetValue();
  if (queryableBox->GetSelection() == 1)
    Queryable = true;
  else
    Queryable = false;
  if (editableBox->GetSelection() == 1)
    Editable = true;
  else
    Editable = false;
  wxDialog::EndModal(wxID_OK);
}

bool VirtualTableRegisterDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  LicenseID = 0;
  List = MainFrame->FindUnregisteredVirtualTableCoverages();
  if (wxDialog::Create(parent, wxID_ANY, wxT("Register VirtualTable Coverage"))
      == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  int count = 0;
  CandidateVirtualTableCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true)
        count++;
      pC = pC->GetNext();
    }
  if (count == 0)
    {
      wxButton *ok = (wxButton *) FindWindow(wxID_OK);
      ok->Enable(false);
      wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
      nameCtrl->Enable(false);
      wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
      titleCtrl->Enable(false);
      wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
      absCtrl->Enable(false);
      wxMessageBox(wxT
                   ("There are no possible Candidates to be eventually registered"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
    }
  return true;
}

void VirtualTableRegisterDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT
                                         ("Candidate VirtualTable Coverages"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_VECTOR_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  CandidateVirtualTableCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true)
        count++;
      pC = pC->GetNext();
    }
  GridCtrl->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("virt_name"));
  GridCtrl->SetColLabelValue(1, wxT("virt_geometry"));
  GridCtrl->SetColLabelValue(2, wxT("SRID"));
  GridCtrl->SetColLabelValue(3, wxT("GeometryType"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pC = List->GetFirst();
  while (pC)
    {
      // feeding grid rows
      if (pC->IsVectorCoverage() != true)
        {
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          GridCtrl->SetCellValue(count, 0, pC->GetVirtName());
          GridCtrl->SetCellValue(count, 1, pC->GetVirtGeometry());
          sprintf(dummy, "%d", pC->GetSRID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 2, cell);
          GridCtrl->SetCellAlignment(count, 2, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 3, pC->GetGeometryType());
          count++;
        }
      pC = pC->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_VECTOR_COVERAGE, wxT(""),
                                        wxDefaultPosition, wxSize(550, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_VECTOR_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(550, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, ID_VECTOR_ABSTRACT, wxT(""),
                                            wxDefaultPosition, wxSize(550, 60),
                                            wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyrightCtrl =
    new wxTextCtrl(this, ID_VECTOR_COPYRIGHT, Copyright,
                   wxDefaultPosition, wxSize(550, 60),
                   wxTE_MULTILINE);
  copySizer->Add(copyrightCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Data &License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *licenseCtrl =
    new wxComboBox(this, ID_VECTOR_LICENSE, wxT(""), wxDefaultPosition,
                   wxSize(550, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  licSizer->Add(licenseCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  PopulateDataLicenses(licenseCtrl);
// seventh row: queryable
  wxBoxSizer *radioSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(radioSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString yesno[2];
  yesno[0] = wxT("&No");
  yesno[1] = wxT("&Yes");
  wxRadioBox *queryableBox = new wxRadioBox(this, ID_VECTOR_QUERYABLE,
                                            wxT("Is &Queryable"),
                                            wxDefaultPosition,
                                            wxDefaultSize, 2,
                                            yesno, 1,
                                            wxRA_SPECIFY_ROWS);
  radioSizer->Add(queryableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  queryableBox->SetSelection(1);

// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Register"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VirtualTableRegisterDialog::OnOk);
}

void VirtualTableRegisterDialog::PopulateDataLicenses(wxComboBox * licCtrl)
{
// populating the Data Licenses List
  const char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int sel = -1;

  sql =
    sqlite3_mprintf("SELECT id, name FROM MAIN.data_licenses ORDER BY name");
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      int id = atoi(results[(i * columns) + 0]);
      const char *name = results[(i * columns) + 1];
      wxString str = wxString::FromUTF8(name);
      licCtrl->Append(str);
      if (id == LicenseID)
        sel = i - 1;
    }
  sqlite3_free_table(results);
  licCtrl->SetSelection(sel);
}

void VirtualTableRegisterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
  wxTextCtrl *copyCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COPYRIGHT);
  wxComboBox *licCtrl = (wxComboBox *) FindWindow(ID_VECTOR_LICENSE);
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_VECTOR_QUERYABLE);
  wxString str = nameCtrl->GetValue().Trim();
  CoverageName = str.Trim(false);
  if (CoverageName.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Coverage Name !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = titleCtrl->GetValue().Trim();
  Title = str.Trim(false);
  if (Title.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Title !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = absCtrl->GetValue().Trim();
  Abstract = str.Trim(false);
  if (Abstract.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Abstract !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  int selCount = 0;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          VirtName = GridCtrl->GetCellValue(i, 0);
          VirtGeometry = GridCtrl->GetCellValue(i, 1);
          selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT("You must select a Candidate to be registered !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT("You must select just a single Candidate to be registered !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = copyCtrl->GetValue().Trim();
  Copyright = str.Trim(false);
  DataLicense = licCtrl->GetValue();
  if (queryableBox->GetSelection() == 1)
    Queryable = true;
  else
    Queryable = false;
  wxDialog::EndModal(wxID_OK);
}

bool TopoGeoRegisterDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  LicenseID = 0;
  List = MainFrame->FindUnregisteredTopoGeoCoverages();
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Register Topology-Geometry Coverage")) == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  int count = 0;
  CandidateTopoGeoCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true)
        count++;
      pC = pC->GetNext();
    }
  if (count == 0)
    {
      wxButton *ok = (wxButton *) FindWindow(wxID_OK);
      ok->Enable(false);
      wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
      nameCtrl->Enable(false);
      wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
      titleCtrl->Enable(false);
      wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
      absCtrl->Enable(false);
      wxMessageBox(wxT
                   ("There are no possible Candidates to be eventually registered"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
    }
  return true;
}

void TopoGeoRegisterDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT
                                         ("Candidate Topology-Geometry Coverages"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_VECTOR_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  CandidateTopoGeoCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true)
        count++;
      pC = pC->GetNext();
    }
  GridCtrl->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("topology_name"));
  GridCtrl->SetColLabelValue(1, wxT("SRID"));
  GridCtrl->SetColLabelValue(2, wxT("Dimensions"));
  GridCtrl->SetColLabelValue(3, wxT("Tolerance"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pC = List->GetFirst();
  while (pC)
    {
      // feeding grid rows
      if (pC->IsVectorCoverage() != true)
        {
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          GridCtrl->SetCellValue(count, 0, pC->GetTopologyName());
          sprintf(dummy, "%d", pC->GetSRID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 1, cell);
          GridCtrl->SetCellAlignment(count, 1, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 2, pC->GetDimensions());
          sprintf(dummy, "%1.9f", pC->GetTolerance());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 3, cell);
          GridCtrl->SetCellAlignment(count, 3, wxALIGN_RIGHT, wxALIGN_TOP);
          count++;
        }
      pC = pC->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_VECTOR_COVERAGE, wxT(""),
                                        wxDefaultPosition, wxSize(550, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_VECTOR_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(550, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, ID_VECTOR_ABSTRACT, wxT(""),
                                            wxDefaultPosition, wxSize(550, 60),
                                            wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyrightCtrl =
    new wxTextCtrl(this, ID_VECTOR_COPYRIGHT, Copyright,
                   wxDefaultPosition, wxSize(550, 60),
                   wxTE_MULTILINE);
  copySizer->Add(copyrightCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Data &License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *licenseCtrl =
    new wxComboBox(this, ID_VECTOR_LICENSE, wxT(""), wxDefaultPosition,
                   wxSize(550, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  licSizer->Add(licenseCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  PopulateDataLicenses(licenseCtrl);
// seventh row: queryable / editable
  wxBoxSizer *radioSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(radioSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString yesno[2];
  yesno[0] = wxT("&No");
  yesno[1] = wxT("&Yes");
  wxRadioBox *queryableBox = new wxRadioBox(this, ID_VECTOR_QUERYABLE,
                                            wxT("Is &Queryable"),
                                            wxDefaultPosition,
                                            wxDefaultSize, 2,
                                            yesno, 1,
                                            wxRA_SPECIFY_ROWS);
  radioSizer->Add(queryableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  queryableBox->SetSelection(1);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  wxRadioBox *editableBox = new wxRadioBox(this, ID_VECTOR_EDITABLE,
                                           wxT("Is &Editable"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 2,
                                           yesno, 1,
                                           wxRA_SPECIFY_ROWS);
  radioSizer->Add(editableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  editableBox->SetSelection(1);

// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Register"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & TopoGeoRegisterDialog::OnOk);
}

void TopoGeoRegisterDialog::PopulateDataLicenses(wxComboBox * licCtrl)
{
// populating the Data Licenses List
  const char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int sel = -1;

  sql =
    sqlite3_mprintf("SELECT id, name FROM MAIN.data_licenses ORDER BY name");
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      int id = atoi(results[(i * columns) + 0]);
      const char *name = results[(i * columns) + 1];
      wxString str = wxString::FromUTF8(name);
      licCtrl->Append(str);
      if (id == LicenseID)
        sel = i - 1;
    }
  sqlite3_free_table(results);
  licCtrl->SetSelection(sel);
}

void TopoGeoRegisterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
  wxTextCtrl *copyCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COPYRIGHT);
  wxComboBox *licCtrl = (wxComboBox *) FindWindow(ID_VECTOR_LICENSE);
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_VECTOR_QUERYABLE);
  wxRadioBox *editableBox = (wxRadioBox *) FindWindow(ID_VECTOR_EDITABLE);
  wxString str = nameCtrl->GetValue().Trim();
  CoverageName = str.Trim(false);
  if (CoverageName.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Coverage Name !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = titleCtrl->GetValue().Trim();
  Title = str.Trim(false);
  if (Title.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Title !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = absCtrl->GetValue().Trim();
  Abstract = str.Trim(false);
  if (Abstract.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Abstract !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  int selCount = 0;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          TopologyName = GridCtrl->GetCellValue(i, 0);
          selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT("You must select a Candidate to be registered !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT("You must select just a single Candidate to be registered !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = copyCtrl->GetValue().Trim();
  Copyright = str.Trim(false);
  DataLicense = licCtrl->GetValue();
  if (queryableBox->GetSelection() == 1)
    Queryable = true;
  else
    Queryable = false;
  if (editableBox->GetSelection() == 1)
    Editable = true;
  else
    Editable = false;
  wxDialog::EndModal(wxID_OK);
}

bool TopoNetRegisterDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  LicenseID = 0;
  List = MainFrame->FindUnregisteredTopoNetCoverages();
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Register Topology-Network Coverage")) == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  int count = 0;
  CandidateTopoNetCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true)
        count++;
      pC = pC->GetNext();
    }
  if (count == 0)
    {
      wxButton *ok = (wxButton *) FindWindow(wxID_OK);
      ok->Enable(false);
      wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
      nameCtrl->Enable(false);
      wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
      titleCtrl->Enable(false);
      wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
      absCtrl->Enable(false);
      wxMessageBox(wxT
                   ("There are no possible Candidates to be eventually registered"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
    }
  return true;
}

void TopoNetRegisterDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT
                                         ("Candidate Topology-Network Coverages"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_VECTOR_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  CandidateTopoNetCoverage *pC = List->GetFirst();
  while (pC)
    {
      // counting how many lines are there
      if (pC->IsVectorCoverage() != true)
        count++;
      pC = pC->GetNext();
    }
  GridCtrl->CreateGrid(count, 3, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("network_name"));
  GridCtrl->SetColLabelValue(1, wxT("SRID"));
  GridCtrl->SetColLabelValue(2, wxT("Dimensions"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pC = List->GetFirst();
  while (pC)
    {
      // feeding grid rows
      if (pC->IsVectorCoverage() != true)
        {
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          GridCtrl->SetCellValue(count, 0, pC->GetNetworkName());
          sprintf(dummy, "%d", pC->GetSRID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 1, cell);
          GridCtrl->SetCellAlignment(count, 1, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 2, pC->GetDimensions());
          count++;
        }
      pC = pC->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_VECTOR_COVERAGE, wxT(""),
                                        wxDefaultPosition, wxSize(550, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_VECTOR_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(550, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, ID_VECTOR_ABSTRACT, wxT(""),
                                            wxDefaultPosition, wxSize(550, 60),
                                            wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyrightCtrl =
    new wxTextCtrl(this, ID_VECTOR_COPYRIGHT, Copyright,
                   wxDefaultPosition, wxSize(550, 60),
                   wxTE_MULTILINE);
  copySizer->Add(copyrightCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Data &License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *licenseCtrl =
    new wxComboBox(this, ID_VECTOR_LICENSE, wxT(""), wxDefaultPosition,
                   wxSize(550, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  licSizer->Add(licenseCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  PopulateDataLicenses(licenseCtrl);
// seventh row: queryable / editable
  wxBoxSizer *radioSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(radioSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString yesno[2];
  yesno[0] = wxT("&No");
  yesno[1] = wxT("&Yes");
  wxRadioBox *queryableBox = new wxRadioBox(this, ID_VECTOR_QUERYABLE,
                                            wxT("Is &Queryable"),
                                            wxDefaultPosition,
                                            wxDefaultSize, 2,
                                            yesno, 1,
                                            wxRA_SPECIFY_ROWS);
  radioSizer->Add(queryableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  queryableBox->SetSelection(1);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  wxRadioBox *editableBox = new wxRadioBox(this, ID_VECTOR_EDITABLE,
                                           wxT("Is &Editable"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 2,
                                           yesno, 1,
                                           wxRA_SPECIFY_ROWS);
  radioSizer->Add(editableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  editableBox->SetSelection(1);

// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Register"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & TopoNetRegisterDialog::OnOk);
}

void TopoNetRegisterDialog::PopulateDataLicenses(wxComboBox * licCtrl)
{
// populating the Data Licenses List
  const char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int sel = -1;

  sql =
    sqlite3_mprintf("SELECT id, name FROM MAIN.data_licenses ORDER BY name");
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      int id = atoi(results[(i * columns) + 0]);
      const char *name = results[(i * columns) + 1];
      wxString str = wxString::FromUTF8(name);
      licCtrl->Append(str);
      if (id == LicenseID)
        sel = i - 1;
    }
  sqlite3_free_table(results);
  licCtrl->SetSelection(sel);
}

void TopoNetRegisterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COVERAGE);
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
  wxTextCtrl *copyCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COPYRIGHT);
  wxComboBox *licCtrl = (wxComboBox *) FindWindow(ID_VECTOR_LICENSE);
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_VECTOR_QUERYABLE);
  wxRadioBox *editableBox = (wxRadioBox *) FindWindow(ID_VECTOR_EDITABLE);
  wxString str = nameCtrl->GetValue().Trim();
  CoverageName = str.Trim(false);
  if (CoverageName.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Coverage Name !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = titleCtrl->GetValue().Trim();
  Title = str.Trim(false);
  if (Title.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Title !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = absCtrl->GetValue().Trim();
  Abstract = str.Trim(false);
  if (Abstract.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Abstract !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  int selCount = 0;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          NetworkName = GridCtrl->GetCellValue(i, 0);
          selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT("You must select a Candidate to be registered !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT("You must select just a single Candidate to be registered !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = copyCtrl->GetValue().Trim();
  Copyright = str.Trim(false);
  DataLicense = licCtrl->GetValue();
  if (queryableBox->GetSelection() == 1)
    Queryable = true;
  else
    Queryable = false;
  if (editableBox->GetSelection() == 1)
    Editable = true;
  else
    Editable = false;
  wxDialog::EndModal(wxID_OK);
}

bool VectorInfosDialog::Create(MyFrame * parent, wxString & coverage)
{
//
// creating the dialog
//
  MainFrame = parent;
  wxString clean;
  if (coverage.StartsWith(wxT("table."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("view."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("shp."), &clean))
    coverage = clean;
  int pos = coverage.Find(wxT(" [SRID="));
  if (pos != wxNOT_FOUND)
    CoverageName = coverage.Left(pos);
  else
    CoverageName = coverage;
  if (DoReadCoverage() == false)
    return false;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Edit Vector Coverage Infos"))
      == false)
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

void VectorInfosDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_VECTOR_COVERAGE, CoverageName,
                                        wxDefaultPosition, wxSize(550, 22),
                                        wxTE_READONLY);
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_VECTOR_TITLE, Title,
                                         wxDefaultPosition, wxSize(550, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, ID_VECTOR_ABSTRACT, Abstract,
                                            wxDefaultPosition, wxSize(550, 60),
                                            wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyrightCtrl =
    new wxTextCtrl(this, ID_VECTOR_COPYRIGHT, Copyright,
                   wxDefaultPosition, wxSize(550, 60),
                   wxTE_MULTILINE);
  copySizer->Add(copyrightCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Data &License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *licenseCtrl =
    new wxComboBox(this, ID_VECTOR_LICENSE, wxT(""), wxDefaultPosition,
                   wxSize(550, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  licSizer->Add(licenseCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  PopulateDataLicenses(licenseCtrl);
// sixth row: queryable / editable
  wxBoxSizer *radioSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(radioSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString yesno[2];
  yesno[0] = wxT("&No");
  yesno[1] = wxT("&Yes");
  wxRadioBox *queryableBox = new wxRadioBox(this, ID_VECTOR_QUERYABLE,
                                            wxT("Is &Queryable"),
                                            wxDefaultPosition,
                                            wxDefaultSize, 2,
                                            yesno, 1,
                                            wxRA_SPECIFY_ROWS);
  radioSizer->Add(queryableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  if (IsQueryable == false)
    queryableBox->SetSelection(0);
  else
    queryableBox->SetSelection(1);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  radioSizer->AddSpacer(25);
  wxRadioBox *editableBox = new wxRadioBox(this, ID_VECTOR_EDITABLE,
                                           wxT("Is &Editable"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 2,
                                           yesno, 1,
                                           wxRA_SPECIFY_ROWS);
  radioSizer->Add(editableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  if (IsEditable == false)
    editableBox->SetSelection(0);
  else
    editableBox->SetSelection(1);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&OK"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorInfosDialog::OnOk);
}

void VectorInfosDialog::PopulateDataLicenses(wxComboBox * licCtrl)
{
// populating the Data Licenses List
  const char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int sel = -1;

  sql =
    sqlite3_mprintf("SELECT id, name FROM MAIN.data_licenses ORDER BY name");
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      int id = atoi(results[(i * columns) + 0]);
      const char *name = results[(i * columns) + 1];
      wxString str = wxString::FromUTF8(name);
      licCtrl->Append(str);
      if (id == LicenseID)
        sel = i - 1;
    }
  sqlite3_free_table(results);
  licCtrl->SetSelection(sel);
}

bool VectorInfosDialog::DoReadCoverage()
{
// will retrieve to read the given Coverage definition
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  char *sql;
  char cvg[1024];
  int count = 0;

  wxCharBuffer buffer = CoverageName.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  sql =
    sqlite3_mprintf
    ("SELECT title, abstract, copyright, license, is_queryable, is_editable "
     "FROM MAIN.vector_coverages WHERE Lower(coverage_name) = Lower(%Q)", cvg);
  int ret = sqlite3_get_table(MainFrame->GetSqlite(), sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
        {
          value = results[(i * columns) + 0];
          Title = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          Abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          if (value == NULL)
            Copyright = wxT("");
          else
            Copyright = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          LicenseID = atoi(value);
          value = results[(i * columns) + 4];
          if (atoi(value) == 0)
            IsQueryable = false;
          else
            IsQueryable = true;
          value = results[(i * columns) + 5];
          if (atoi(value) == 0)
            IsEditable = false;
          else
            IsEditable = true;
          count++;
        }
    }
  sqlite3_free_table(results);
  if (count == 1)
    return true;
  return false;
}

void VectorInfosDialog::DoUpdateCoverage()
{
// will retrieve to update the given Coverage definition
  int ret;
  sqlite3_stmt *stmt = NULL;
  char *dummy;
  bool error = false;

// updating the Title and Abstract
  const char *sql = "SELECT SE_SetVectorCoverageInfos(?, ?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetVectorCoverageInfos: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((Title.Len() * 4) + 1);
  strcpy(dummy, Title.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((Abstract.Len() * 4) + 1);
  strcpy(dummy, Abstract.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (IsQueryable == false)
    sqlite3_bind_int(stmt, 4, 0);
  else
    sqlite3_bind_int(stmt, 4, 1);
  if (IsEditable == false)
    sqlite3_bind_int(stmt, 5, 0);
  else
    sqlite3_bind_int(stmt, 5, 1);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetVectorCoverageInfos: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error)
    return;

// updating the Copyright and License
  sql = "SELECT SE_SetVectorCoverageCopyright(?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetVectorCoverageCopyright: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (Copyright.Len() == 0)
    sqlite3_bind_null(stmt, 2);
  else
    {
      dummy = (char *) malloc((Copyright.Len() * 4) + 1);
      strcpy(dummy, Copyright.ToUTF8());
      sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
      free(dummy);
    }
  dummy = (char *) malloc((DataLicense.Len() * 4) + 1);
  strcpy(dummy, DataLicense.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetVectorCoverageCopyright ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
}

void VectorInfosDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_TITLE);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_ABSTRACT);
  wxTextCtrl *copyCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_COPYRIGHT);
  wxComboBox *licCtrl = (wxComboBox *) FindWindow(ID_VECTOR_LICENSE);
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_VECTOR_QUERYABLE);
  wxRadioBox *editableBox = (wxRadioBox *) FindWindow(ID_VECTOR_EDITABLE);
  wxString str = titleCtrl->GetValue().Trim();
  Title = str.Trim(false);
  if (Title.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Title !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = absCtrl->GetValue().Trim();
  Abstract = str.Trim(false);
  if (Abstract.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Abstract !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = copyCtrl->GetValue().Trim();
  Copyright = str.Trim(false);
  DataLicense = licCtrl->GetValue();
  if (queryableBox->GetSelection() == 1)
    IsQueryable = true;
  else
    IsQueryable = false;
  if (editableBox->GetSelection() == 1)
    IsEditable = true;
  else
    IsEditable = false;
  DoUpdateCoverage();
  wxDialog::EndModal(wxID_OK);
}

bool ReloadVectorStyleDialog::Create(MyFrame * parent, wxString & path)
{
//
// creating the dialog
//
  MainFrame = parent;
  Path = path;
  List = MainFrame->FindVectorStyles();
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Reloading an SLD/SE Vector Style")) == false)
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

void ReloadVectorStyleDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Style's Path
  wxBoxSizer *pathSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(pathSizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 0);
  wxStaticText *pathLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&SLD/SE Style:"));
  pathSizer->Add(pathLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *pathCtrl = new wxTextCtrl(this, wxID_ANY, Path,
                                        wxDefaultPosition, wxSize(550, 22),
                                        wxTE_READONLY);
  pathCtrl->Enable(false);
  pathSizer->Add(pathCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered SLD/SE Vector Styles"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  VectorCoverageStyle *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Style ID"));
  GridCtrl->SetColLabelValue(1, wxT("Name"));
  GridCtrl->SetColLabelValue(2, wxT("Title"));
  GridCtrl->SetColLabelValue(3, wxT("Abstract"));
  GridCtrl->SetColLabelValue(4, wxT("Schema Validated"));
  GridCtrl->SetColLabelValue(5, wxT("Schema URI"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      sprintf(dummy, "%d", pS->GetStyleID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 0, cell);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 1, pS->GetName());
      GridCtrl->SetCellValue(count, 2, pS->GetTitle());
      GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
      GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
      GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Reload"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & ReloadVectorStyleDialog::OnOk);
}

bool ReloadVectorStyleDialog::DoReloadVectorStyle(int style_id, void *blob,
                                                  int blob_size)
{
//
// attempting to reload the Vector Style
//
  int ret;
  int valid = 0;
  const char *sql;
  sqlite3_stmt *stmt = NULL;

  sql = "SELECT SE_ReloadVectorStyle(?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_int(stmt, 1, style_id);
  sqlite3_bind_blob(stmt, 2, blob, blob_size, free);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (valid)
    return true;
  return false;
}

void ReloadVectorStyleDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  int selCount = 0;
  long style_id;
  void *blob = NULL;
  int blob_size;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          wxString cell_id = GridCtrl->GetCellValue(i, 0);
          if (cell_id.ToLong(&style_id) == true)
            selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT("You must select a Vector Style to be reloaded !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT("You must select just a single Vector Style to be reloaded !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  ::wxBeginBusyCursor();
  if (MainFrame->ValidateVectorStyle(Path.ToUTF8(), &blob, &blob_size) == true)
    {
      char dummy[80];
      sprintf(dummy, "%ld", style_id);
      if (DoReloadVectorStyle(style_id, blob, blob_size) == true)
        {
          ::wxEndBusyCursor();
          wxMessageBox(wxT("Vector Style (style_id=") +
                       wxString::FromUTF8(dummy) +
                       wxT(") successfully reloaded"), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
      } else
        {
          ::wxEndBusyCursor();
          wxMessageBox(wxT
                       ("Some error occurred: unable to reload Vector Style (style_id=")
                       + wxString::FromUTF8(dummy) + wxT(")"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
        }
  } else
    {
      ::wxEndBusyCursor();
      wxString msg = Path + wxT("\n\nnot a valid SLD/SE Vector Style");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
    }
  wxDialog::EndModal(wxID_OK);
}

bool ReloadRasterStyleDialog::Create(MyFrame * parent, wxString & path)
{
//
// creating the dialog
//
  MainFrame = parent;
  Path = path;
  List = MainFrame->FindRasterStyles();
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Reloading an SLD/SE Raster Style")) == false)
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

void ReloadRasterStyleDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Style's Path
  wxBoxSizer *pathSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(pathSizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 0);
  wxStaticText *pathLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&SLD/SE Style:"));
  pathSizer->Add(pathLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *pathCtrl = new wxTextCtrl(this, wxID_ANY, Path,
                                        wxDefaultPosition, wxSize(550, 22),
                                        wxTE_READONLY);
  pathCtrl->Enable(false);
  pathSizer->Add(pathCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered SLD/SE Raster Styles"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  RasterCoverageStyle *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Style ID"));
  GridCtrl->SetColLabelValue(1, wxT("Name"));
  GridCtrl->SetColLabelValue(2, wxT("Title"));
  GridCtrl->SetColLabelValue(3, wxT("Abstract"));
  GridCtrl->SetColLabelValue(4, wxT("Schema Validated"));
  GridCtrl->SetColLabelValue(5, wxT("Schema URI"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      sprintf(dummy, "%d", pS->GetStyleID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 0, cell);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 1, pS->GetName());
      GridCtrl->SetCellValue(count, 2, pS->GetTitle());
      GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
      GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
      GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Reload"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & ReloadRasterStyleDialog::OnOk);
}

bool ReloadRasterStyleDialog::DoReloadRasterStyle(int style_id, void *blob,
                                                  int blob_size)
{
//
// attempting to reload the Raster Style
//
  int ret;
  int valid = 0;
  const char *sql;
  sqlite3_stmt *stmt = NULL;

  sql = "SELECT SE_ReloadRasterStyle(?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_int(stmt, 1, style_id);
  sqlite3_bind_blob(stmt, 2, blob, blob_size, free);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (valid)
    return true;
  return false;
}

void ReloadRasterStyleDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  int selCount = 0;
  long style_id;
  void *blob = NULL;
  int blob_size;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          wxString cell_id = GridCtrl->GetCellValue(i, 0);
          if (cell_id.ToLong(&style_id) == true)
            selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT("You must select a Raster Style to be reloaded !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT("You must select just a single Raster Style to be reloaded !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  ::wxBeginBusyCursor();
  if (MainFrame->ValidateRasterStyle(Path.ToUTF8(), &blob, &blob_size) == true)
    {
      char dummy[80];
      sprintf(dummy, "%ld", style_id);
      if (DoReloadRasterStyle(style_id, blob, blob_size) == true)
        {
          ::wxEndBusyCursor();
          wxMessageBox(wxT("Raster Style (style_id=") +
                       wxString::FromUTF8(dummy) +
                       wxT(") successfully reloaded"), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
      } else
        {
          ::wxEndBusyCursor();
          wxMessageBox(wxT
                       ("Some error occurred: unable to reload Raster Style (style_id=")
                       + wxString::FromUTF8(dummy) + wxT(")"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
        }
  } else
    {
      ::wxEndBusyCursor();
      wxString msg = Path + wxT("\n\nnot a valid SLD/SE Raster Style");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
    }
  wxDialog::EndModal(wxID_OK);
}

bool UnregisterRasterStyleDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  List = MainFrame->FindRasterStyles();
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Unregistering an SLD/SE Raster Style")) == false)
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

void UnregisterRasterStyleDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered SLD/SE Raster Styles"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  RasterCoverageStyle *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Style ID"));
  GridCtrl->SetColLabelValue(1, wxT("Name"));
  GridCtrl->SetColLabelValue(2, wxT("Title"));
  GridCtrl->SetColLabelValue(3, wxT("Abstract"));
  GridCtrl->SetColLabelValue(4, wxT("Schema Validated"));
  GridCtrl->SetColLabelValue(5, wxT("Schema URI"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      sprintf(dummy, "%d", pS->GetStyleID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 0, cell);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 1, pS->GetName());
      GridCtrl->SetCellValue(count, 2, pS->GetTitle());
      GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
      GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
      GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Unregister"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & UnregisterRasterStyleDialog::OnOk);
}

bool UnregisterRasterStyleDialog::DoUnregisterRasterStyle(int style_id)
{
//
// attempting to unregister the Raster Style
//
  int ret;
  int valid = 0;
  const char *sql;
  sqlite3_stmt *stmt = NULL;

  sql = "SELECT SE_UnregisterRasterStyle(?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_int(stmt, 1, style_id);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (valid)
    return true;
  return false;
}

bool UnregisterRasterStyleDialog::DoCheckUnreferencedRasterStyle(int style_id)
{
//
// checking if a Raster Style is currently referenced or not
//
  int ret;
  int count = 1;
  const char *sql;
  sqlite3_stmt *stmt = NULL;

  sql = "SELECT Count(*) FROM SE_raster_styles AS s "
    "JOIN SE_raster_styled_layers AS l ON (l.style_id = s.style_id) "
    "WHERE s.style_id = ?";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_int(stmt, 1, style_id);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            count = sqlite3_column_int(stmt, 0);
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (!count)
    return true;
  return false;
}

void UnregisterRasterStyleDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  int selCount = 0;
  long style_id;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          wxString cell_id = GridCtrl->GetCellValue(i, 0);
          if (cell_id.ToLong(&style_id) == true)
            selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT("You must select a Raster Style to be unregistered !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT
        ("You must select just a single Raster Style to be unregistered !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  ::wxBeginBusyCursor();
  char dummy[80];
  sprintf(dummy, "%ld", style_id);
  if (DoCheckUnreferencedRasterStyle(style_id) == true)
    {
      if (DoUnregisterRasterStyle(style_id) == true)
        {
          ::wxEndBusyCursor();
          wxMessageBox(wxT("Raster Style (style_id=") +
                       wxString::FromUTF8(dummy) +
                       wxT(") successfully unregistered"),
                       wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      } else
        {
          ::wxEndBusyCursor();
          wxMessageBox(wxT
                       ("Some error occurred: unable to unregister Raster Style (style_id=")
                       + wxString::FromUTF8(dummy) + wxT(")"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
        }
  } else
    {
      ::wxEndBusyCursor();
      wxMessageBox(wxT("You can't unregister Raster Style (style_id=") +
                   wxString::FromUTF8(dummy) +
                   wxT
                   (")\nbecause it's currently referenced by at least one Raster Coverage"),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
    }
  wxDialog::EndModal(wxID_OK);
}

bool UnregisterVectorStyleDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  List = MainFrame->FindVectorStyles();
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Unregistering an SLD/SE Vector Style")) == false)
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

void UnregisterVectorStyleDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered SLD/SE Vector Styles"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  VectorCoverageStyle *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Style ID"));
  GridCtrl->SetColLabelValue(1, wxT("Name"));
  GridCtrl->SetColLabelValue(2, wxT("Title"));
  GridCtrl->SetColLabelValue(3, wxT("Abstract"));
  GridCtrl->SetColLabelValue(4, wxT("Schema Validated"));
  GridCtrl->SetColLabelValue(5, wxT("Schema URI"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      sprintf(dummy, "%d", pS->GetStyleID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 0, cell);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 1, pS->GetName());
      GridCtrl->SetCellValue(count, 2, pS->GetTitle());
      GridCtrl->SetCellValue(count, 3, pS->GetAbstract());
      GridCtrl->SetCellValue(count, 4, pS->GetSchemaValidated());
      GridCtrl->SetCellValue(count, 5, pS->GetSchemaURI());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Unregister"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & UnregisterVectorStyleDialog::OnOk);
}

bool UnregisterVectorStyleDialog::DoUnregisterVectorStyle(int style_id)
{
//
// attempting to unregister the Vector Style
//
  int ret;
  int valid = 0;
  const char *sql;
  sqlite3_stmt *stmt = NULL;

  sql = "SELECT SE_UnregisterVectorStyle(?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_int(stmt, 1, style_id);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (valid)
    return true;
  return false;
}

bool UnregisterVectorStyleDialog::DoCheckUnreferencedVectorStyle(int style_id)
{
//
// checking if a Vector Style is currently referenced or not
//
  int ret;
  int count = 1;
  const char *sql;
  sqlite3_stmt *stmt = NULL;

  sql = "SELECT Count(*) FROM SE_vector_styles AS s "
    "JOIN SE_vector_styled_layers AS l ON (l.style_id = s.style_id) "
    "WHERE s.style_id = ?";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_int(stmt, 1, style_id);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            count = sqlite3_column_int(stmt, 0);
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (!count)
    return true;
  return false;
}

void UnregisterVectorStyleDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  int selCount = 0;
  long style_id;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          wxString cell_id = GridCtrl->GetCellValue(i, 0);
          if (cell_id.ToLong(&style_id) == true)
            selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT("You must select a Vector Style to be unregistered !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT
        ("You must select just a single Vector Style to be unregistered !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  ::wxBeginBusyCursor();
  char dummy[80];
  sprintf(dummy, "%ld", style_id);
  if (DoCheckUnreferencedVectorStyle(style_id) == true)
    {
      if (DoUnregisterVectorStyle(style_id) == true)
        {
          ::wxEndBusyCursor();
          wxMessageBox(wxT("Vector Style (style_id=") +
                       wxString::FromUTF8(dummy) +
                       wxT(") successfully unregistered"),
                       wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      } else
        {
          ::wxEndBusyCursor();
          wxMessageBox(wxT
                       ("Some error occurred: unable to unregister Vector Style (style_id=")
                       + wxString::FromUTF8(dummy) + wxT(")"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
        }
  } else
    {
      ::wxEndBusyCursor();
      wxMessageBox(wxT("You can't unregister Vector Style (style_id=") +
                   wxString::FromUTF8(dummy) +
                   wxT
                   (")\nbecause it's currently referenced by at least one Vector Coverage"),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
    }
  wxDialog::EndModal(wxID_OK);
}

bool VectorSRIDsDialog::Create(MyFrame * parent, wxString & coverage)
{
//
// creating the dialog
//
  MainFrame = parent;
  wxString clean;
  if (coverage.StartsWith(wxT("table."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("view."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("shp."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("topogeo."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("toponet."), &clean))
    coverage = clean;
  int pos = coverage.Find(wxT(" [SRID="));
  if (pos != wxNOT_FOUND)
    CoverageName = coverage.Left(pos);
  else
    CoverageName = coverage;
  List = MainFrame->FindVectorAlternativeSRIDs(CoverageName);
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Vector Coverage: alternative SRIDs")) == false)
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

void VectorSRIDsDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Vector Coverage
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(550, 22),
                                       wxTE_READONLY);
  cvgCtrl->Enable(false);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered alternative SRIDs"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_VECTOR_SRID_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  VectorCoverageSRID *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectCells);
  GridCtrl->SetColLabelValue(0, wxT("Remove"));
  GridCtrl->SetColLabelValue(1, wxT("Native"));
  GridCtrl->SetColLabelValue(2, wxT("SRID"));
  GridCtrl->SetColLabelValue(3, wxT("Auth Name"));
  GridCtrl->SetColLabelValue(4, wxT("Auth SRID"));
  GridCtrl->SetColLabelValue(5, wxT("RefSys Name"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      if (pS->IsNative() != true)
        {
          MyBitmapCellRenderer *renderer = new MyBitmapCellRenderer;
          wxBitmap bmp = wxBitmap(dustbin_xpm);
          renderer->SetGraphic(bmp);
          GridCtrl->SetCellRenderer(count, 0, renderer);
        }
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      if (pS->IsNative() == true)
        GridCtrl->SetCellValue(count, 1, wxT("yes"));
      else
        GridCtrl->SetCellValue(count, 1, wxT(""));
      sprintf(dummy, "%d", pS->GetSRID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 2, cell);
      GridCtrl->SetCellAlignment(count, 2, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 3, pS->GetAuthName());
      sprintf(dummy, "%d", pS->GetAuthSRID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 4, cell);
      GridCtrl->SetCellAlignment(count, 4, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 5, pS->GetRefSysName());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// SRID selection
  wxBoxSizer *sridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *sridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Adding an alternative SRID"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *SridSizer = new wxStaticBoxSizer(sridBox, wxHORIZONTAL);
  sridBoxSizer->Add(SridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *sridSizer = new wxBoxSizer(wxHORIZONTAL);
  SridSizer->Add(sridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *sridLabel = new wxStaticText(this, wxID_STATIC, wxT("&SRID:"));
  sridSizer->Add(sridLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  int srid = 0;
  wxSpinCtrl *sridCtrl = new wxSpinCtrl(this, ID_VECTOR_SRID, wxEmptyString,
                                        wxDefaultPosition, wxSize(80, 20),
                                        wxSP_ARROW_KEYS,
                                        -1, 1000000, srid);
  sridSizer->Add(sridCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl =
    new wxTextCtrl(this, ID_VECTOR_REFSYS, wxT("Undefined"),
                   wxDefaultPosition, wxSize(350, 22),
                   wxTE_READONLY);
  UpdateRefSysName(srid);
  sridSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *addNew = new wxButton(this, ID_VECTOR_SRID_ADD, wxT("&Add"));
  addNew->Enable(false);
  sridSizer->Add(addNew, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorSRIDsDialog::OnQuit);
  Connect(ID_VECTOR_SRID_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorSRIDsDialog::OnCmdAddSrid);
  Connect(ID_VECTOR_SRID_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & VectorSRIDsDialog::OnCmdRemoveSrid);
  Connect(wxID_ANY, wxEVT_GRID_CELL_RIGHT_CLICK,
          (wxObjectEventFunction) & VectorSRIDsDialog::OnRightClick);
  Connect(wxID_ANY, wxEVT_GRID_SELECT_CELL,
          (wxObjectEventFunction) & VectorSRIDsDialog::OnCellSelected);
  Connect(ID_VECTOR_SRID, wxEVT_COMMAND_SPINCTRL_UPDATED,
          (wxObjectEventFunction) & VectorSRIDsDialog::OnSridChanged);
  Connect(ID_VECTOR_SRID, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) & VectorSRIDsDialog::OnSridChanged);
}

bool VectorSRIDsDialog::UpdateRefSysName(int srid)
{
//
// updating the RefSys Name
//
  sqlite3 *sqlite = MainFrame->GetSqlite();
  char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  char *value;
  bool valid = false;
  wxString name(wxT("Undefined"));

  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_REFSYS);
  if (srid <= 0)
    goto end;

  sql =
    sqlite3_mprintf("SELECT ref_sys_name FROM spatial_ref_sys WHERE srid = %d",
                    srid);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto end;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          name = wxString::FromUTF8(value);
          valid = true;
        }
    }
  sqlite3_free_table(results);
end:
  nameCtrl->SetValue(name);
  return valid;
}

void VectorSRIDsDialog::OnSridChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Srid changed
//
  wxButton *addNew = (wxButton *) FindWindow(ID_VECTOR_SRID_ADD);
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_VECTOR_SRID);
  int srid = sridCtrl->GetValue();
  bool ret = UpdateRefSysName(srid);
  addNew->Enable(ret);
}

void VectorSRIDsDialog::OnRightClick(wxGridEvent & event)
{
//
// right click on some cell [mouse action]
//
  wxMenu menu;
  wxMenuItem *menuItem;
  wxPoint pt = event.GetPosition();
  CurrentRow = event.GetRow();
  GridCtrl->SelectRow(CurrentRow);
  wxString native = GridCtrl->GetCellValue(CurrentRow, 1);
  if (native.CmpNoCase(wxT("yes")) == 0)
    return;
  wxString id = GridCtrl->GetCellValue(CurrentRow, 2);
  long srid;
  id.ToLong(&srid);
  CurrentSRID = srid;
  menuItem = new wxMenuItem(&menu, ID_VECTOR_SRID_REMOVE, wxT("&Remove"));
  menu.Append(menuItem);
  GridCtrl->PopupMenu(&menu, pt);
}

void VectorSRIDsDialog::OnCellSelected(wxGridEvent & event)
{
//
// cell selection changed
//
  if (GridCtrl->IsShown() == false)
    return;
  CurrentRow = event.GetRow();
  GridCtrl->SelectRow(CurrentRow);
  wxString native = GridCtrl->GetCellValue(CurrentRow, 1);
  if (event.GetCol() != 0)
    return;
  if (native.CmpNoCase(wxT("yes")) == 0)
    return;
  wxString id = GridCtrl->GetCellValue(CurrentRow, 2);
  long srid;
  id.ToLong(&srid);
  CurrentSRID = srid;
  DoRemoveSRID();
}

void VectorSRIDsDialog::OnCmdRemoveSrid(wxCommandEvent & WXUNUSED(event))
{
//
// deleting a SRID [mouse action]
//
  GridCtrl->SelectRow(CurrentRow);
  wxString native = GridCtrl->GetCellValue(CurrentRow, 1);
  if (native.CmpNoCase(wxT("yes")) == 0)
    return;
  wxString id = GridCtrl->GetCellValue(CurrentRow, 2);
  long srid;
  id.ToLong(&srid);
  CurrentSRID = srid;
  DoRemoveSRID();
}

void VectorSRIDsDialog::DoRemoveSRID()
{
//
// deleting a SRID
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;

  dummy =
    sqlite3_mprintf
    ("Do you really intend removing SRID=%d from this Vector Coverage ?",
     CurrentSRID);
  wxString msg = wxString::FromUTF8(dummy);
  sqlite3_free(dummy);
  if (wxMessageBox
      (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_QUESTION, this) != wxYES)
    return;

  const char *sql = "SELECT SE_UnRegisterVectorCoverageSrid(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_int(stmt, 2, CurrentSRID);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    {
      GridCtrl->DeleteRows(CurrentRow, 1);
      List->MarkDeleted(CurrentSRID);
    }
  sqlite3_finalize(stmt);
}

bool VectorSRIDsDialog::DoRegistetVectorCoverageSrid(int srid)
{
//
// attempting to register an alternative Vector Coverage SRID
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  int value = 0;
  const char *sql = "SELECT SE_RegisterVectorCoverageSrid(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_int(stmt, 2, srid);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE)
    ;
  else if (ret == SQLITE_ROW)
    value = sqlite3_column_int(stmt, 0);
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  if (!value)
    return false;
  return true;
}

void VectorSRIDsDialog::OnCmdAddSrid(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_VECTOR_SRID);
  int srid = sridCtrl->GetValue();
  if (srid <= 0)
    {
      wxMessageBox(wxT("You must specify some SRID value !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
  } else if (MainFrame->SRIDnotExists(srid) == true)
    {
      wxMessageBox(wxT("invalid SRID value"), wxT("spatialite_gui"),
                   wxOK | wxICON_WARNING, this);
      return;
    }
  if (List->IsNativeSRID(srid) == true)
    {
      wxMessageBox(wxT("Can't register twice the Native SRID"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (List->IsAlreadyDefinedSRID(srid) == true)
    {
      wxMessageBox(wxT("Already defined alternative SRID"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  // attempting to register the alternative SRID
  if (DoRegistetVectorCoverageSrid(srid) == true)
    {
      // updating the Grid
      GridCtrl->Show(false);
      int tot_rows = GridCtrl->GetNumberRows();
      if (tot_rows > 0)
        GridCtrl->DeleteRows(0, tot_rows);
      if (List != NULL)
        delete List;
      List = MainFrame->FindVectorAlternativeSRIDs(CoverageName);
      int count = 0;
      VectorCoverageSRID *pS = List->GetFirst();
      while (pS)
        {
          // counting how many lines are there
          count++;
          pS = pS->GetNext();
        }
      GridCtrl->AppendRows(count);
      GridCtrl->ClearSelection();
      count = 0;
      char dummy[1024];
      wxString cell;
      pS = List->GetFirst();
      while (pS)
        {
          // feeding grid rows
          if (pS->IsNative() != true)
            {
              MyBitmapCellRenderer *renderer = new MyBitmapCellRenderer;
              wxBitmap bmp = wxBitmap(dustbin_xpm);
              renderer->SetGraphic(bmp);
              GridCtrl->SetCellRenderer(count, 0, renderer);
            }
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          if (pS->IsNative() == true)
            GridCtrl->SetCellValue(count, 1, wxT("yes"));
          else
            GridCtrl->SetCellValue(count, 1, wxT(""));
          sprintf(dummy, "%d", pS->GetSRID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 2, cell);
          GridCtrl->SetCellAlignment(count, 2, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 3, pS->GetAuthName());
          sprintf(dummy, "%d", pS->GetAuthSRID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 4, cell);
          GridCtrl->SetCellAlignment(count, 4, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 5, pS->GetRefSysName());
          count++;
          pS = pS->GetNext();
        }
      GridCtrl->AutoSizeColumns();
      GridCtrl->Show(true);
    }
  return;
}

void VectorSRIDsDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

VectorCoverageSRIDsList::~VectorCoverageSRIDsList()
{
// destructor
  VectorCoverageSRID *pS;
  VectorCoverageSRID *pSn;
  pS = First;
  while (pS != NULL)
    {
      pSn = pS->GetNext();
      delete pS;
      pS = pSn;
    }
}

void VectorCoverageSRIDsList::Add(bool native, int srid, wxString & auth_name,
                                  int auth_srid, wxString & name)
{
// inserting a new SRID
  VectorCoverageSRID *pS =
    new VectorCoverageSRID(native, srid, auth_name, auth_srid, name);
  if (First == NULL)
    First = pS;
  if (Last != NULL)
    Last->SetNext(pS);
  Last = pS;
}

void VectorCoverageSRIDsList::MarkDeleted(int srid)
{
// marking some Srid as deleted
  VectorCoverageSRID *pS;
  pS = First;
  while (pS != NULL)
    {
      if (pS->GetSRID() == srid)
        {
          pS->MarkDeleted();
          return;
        }
      pS = pS->GetNext();
    }
}

bool VectorCoverageSRIDsList::IsNativeSRID(int srid)
{
// checking for the Native SRID
  VectorCoverageSRID *pS;
  pS = First;
  while (pS != NULL)
    {
      if (pS->GetSRID() == srid)
        return pS->IsNative();
      pS = pS->GetNext();
    }
  return false;
}

bool VectorCoverageSRIDsList::IsAlreadyDefinedSRID(int srid)
{
// checking for an already defined alternative SRID
  VectorCoverageSRID *pS;
  pS = First;
  while (pS != NULL)
    {
      if (pS->GetSRID() == srid)
        {
          if (pS->IsDeleted() == true)
            return false;
          return true;
        }
      pS = pS->GetNext();
    }
  return false;
}

int MyFrame::FindVectorType(char *cvg)
{
//
// determining the Vector Type
//
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value1;
  char *value2;
  int type = VECTOR_UNKNOWN;
  char *sql;

  sql =
    sqlite3_mprintf
    ("SELECT f_table_name, f_geometry_column, view_name, view_geometry, "
     "virt_name, virt_geometry, topology_name, network_name "
     "FROM MAIN.vector_coverages WHERE coverage_name = %Q", cvg);
  ret = sqlite3_get_table(SqliteHandle, sql, &results,
                          &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return VECTOR_UNKNOWN;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value1 = results[(i * columns) + 0];
          value2 = results[(i * columns) + 1];
          if (value1 != NULL && value2 != NULL)
            type = VECTOR_GEOTABLE;
          value1 = results[(i * columns) + 2];
          value2 = results[(i * columns) + 3];
          if (value1 != NULL && value2 != NULL)
            type = VECTOR_SPATIALVIEW;
          value1 = results[(i * columns) + 4];
          value2 = results[(i * columns) + 5];
          if (value1 != NULL && value2 != NULL)
            type = VECTOR_VIRTUALTABLE;
          value1 = results[(i * columns) + 6];
          if (value1 != NULL)
            type = VECTOR_TOPOGEO;
          value1 = results[(i * columns) + 7];
          if (value1 != NULL)
            type = VECTOR_TOPONET;
        }
    }
  sqlite3_free_table(results);
  return type;
}

VectorCoverageSRIDsList *MyFrame::FindVectorAlternativeSRIDs(wxString &
                                                             coverage)
{
// will retrieve all alternative SRIDs for the given Coverage
  VectorCoverageSRIDsList *list = new VectorCoverageSRIDsList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  char *sql;
  char cvg[1024];

  wxCharBuffer buffer = coverage.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  int type = FindVectorType(cvg);

  switch (type)
    {
      case VECTOR_GEOTABLE:
        sql =
          sqlite3_mprintf
          ("SELECT 1, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.vector_coverages AS v "
           "JOIN MAIN.geometry_columns AS x ON (v.f_table_name = x.f_table_name AND v.f_geometry_column = x.f_geometry_column) "
           "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q UNION "
           "SELECT 0, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.vector_coverages AS v "
           "JOIN MAIN.vector_coverages_srid AS x ON (v.coverage_name = x.coverage_name) "
           "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q ORDER BY 2", cvg, cvg);
        break;
      case VECTOR_SPATIALVIEW:
        sql =
          sqlite3_mprintf
          ("SELECT 1, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM main.vector_coverages AS v "
           "JOIN MAIN.views_geometry_columns AS y ON (v.view_name = y.view_name AND v.view_geometry = y.view_geometry) "
           "LEFT JOIN MAIN.geometry_columns AS x ON (y.f_table_name = x.f_table_name AND y.f_geometry_column = x.f_geometry_column) "
           "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q UNION "
           "SELECT 0, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.vector_coverages AS v "
           "JOIN MAIN.vector_coverages_srid AS x ON (v.coverage_name = x.coverage_name) "
           "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q ORDER BY 2", cvg, cvg);
        break;
      case VECTOR_VIRTUALTABLE:
        sql =
          sqlite3_mprintf
          ("SELECT 1, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.vector_coverages AS v "
           "JOIN MAIN.virts_geometry_columns AS x ON (v.virt_name = x.virt_name AND v.virt_geometry = x.virt_geometry) "
           "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q UNION "
           "SELECT 0, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.vector_coverages AS v "
           "JOIN MAIN.vector_coverages_srid AS x ON (v.coverage_name = x.coverage_name) "
           "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q ORDER BY 2", cvg, cvg);
        break;
      case VECTOR_TOPOGEO:
        sql =
          sqlite3_mprintf
          ("SELECT 1, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.vector_coverages AS v "
           "JOIN MAIN.topologies AS x ON (v.topology_name = x.topology_name) "
           "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q UNION "
           "SELECT 0, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.vector_coverages AS v "
           "JOIN MAIN.vector_coverages_srid AS x ON (v.coverage_name = x.coverage_name) "
           "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q ORDER BY 2", cvg, cvg);
        break;
      case VECTOR_TOPONET:
        sql =
          sqlite3_mprintf
          ("SELECT 1, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.vector_coverages AS v "
           "JOIN networks AS x ON (v.network_name = x.network_name) "
           "LEFT JOIN spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q UNION "
           "SELECT 0, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.vector_coverages AS v "
           "JOIN MAIN.vector_coverages_srid AS x ON (v.coverage_name = x.coverage_name) "
           "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
           "WHERE v.coverage_name = %Q ORDER BY 2", cvg, cvg);
        break;
      case VECTOR_UNKNOWN:
      default:
        return list;
        break;
    };

  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          bool native = false;
          value = results[(i * columns) + 0];
          if (atoi(value) != 0)
            native = true;
          value = results[(i * columns) + 1];
          int srid = atoi(value);
          value = results[(i * columns) + 2];
          wxString auth_name = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          int auth_srid = atoi(value);
          value = results[(i * columns) + 4];
          wxString name = wxString::FromUTF8(value);
          list->Add(native, srid, auth_name, auth_srid, name);
        }
    }
  sqlite3_free_table(results);
  return list;
}

bool VectorKeywordsDialog::Create(MyFrame * parent, wxString & coverage)
{
//
// creating the dialog
//
  MainFrame = parent;
  wxString clean;
  if (coverage.StartsWith(wxT("table."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("view."), &clean))
    coverage = clean;
  else if (coverage.StartsWith(wxT("shp."), &clean))
    coverage = clean;
  int pos = coverage.Find(wxT(" [SRID="));
  if (pos != wxNOT_FOUND)
    CoverageName = coverage.Left(pos);
  else
    CoverageName = coverage;
  List = MainFrame->FindVectorKeywords(CoverageName);
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Vector Coverage: Keywords")) == false)
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

void VectorKeywordsDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Vector Coverage
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(550, 22),
                                       wxTE_READONLY);
  cvgCtrl->Enable(false);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered Keywords"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_VECTOR_KEYWORD_GRID, wxDefaultPosition,
               wxSize(640, 200));
  int count = 0;
  VectorCoverageKeyword *pK = List->GetFirst();
  while (pK)
    {
      // counting how many lines are there
      count++;
      pK = pK->GetNext();
    }
  GridCtrl->CreateGrid(count, 1, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Keyword"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pK = List->GetFirst();
  while (pK)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      GridCtrl->SetCellValue(count, 0, pK->GetKeyword());
      count++;
      pK = pK->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// Keyword selection
  wxBoxSizer *keywordBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(keywordBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *keywordBox = new wxStaticBox(this, wxID_STATIC,
                                            wxT("Adding a Keyword"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *KeywordSizer = new wxStaticBoxSizer(keywordBox, wxHORIZONTAL);
  keywordBoxSizer->Add(KeywordSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *keywordSizer = new wxBoxSizer(wxHORIZONTAL);
  KeywordSizer->Add(keywordSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *keywordLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Keyword:"));
  keywordSizer->Add(keywordLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *keywordCtrl = new wxTextCtrl(this, ID_VECTOR_KEYWORD, Keyword,
                                           wxDefaultPosition, wxSize(400, 22));
  keywordSizer->Add(keywordCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  keywordSizer->AddSpacer(100);
  wxButton *addNew =
    new wxButton(this, ID_VECTOR_KEYWORD_ADD, wxT("&Add Keyword"));
  keywordSizer->Add(addNew, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorKeywordsDialog::OnQuit);
  Connect(ID_VECTOR_KEYWORD_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorKeywordsDialog::OnCmdAddKeyword);
  Connect(ID_VECTOR_KEYWORD_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & VectorKeywordsDialog::OnCmdRemoveKeyword);
  Connect(wxID_ANY, wxEVT_GRID_CELL_RIGHT_CLICK,
          (wxObjectEventFunction) & VectorKeywordsDialog::OnRightClick);
}

void VectorKeywordsDialog::OnRightClick(wxGridEvent & event)
{
//
// right click on some cell [mouse action]
//
  wxMenu menu;
  wxMenuItem *menuItem;
  wxPoint pt = event.GetPosition();
  CurrentRow = event.GetRow();
  GridCtrl->SelectRow(CurrentRow);
  Keyword = GridCtrl->GetCellValue(CurrentRow, 0);
  menuItem = new wxMenuItem(&menu, ID_VECTOR_KEYWORD_REMOVE, wxT("&Remove"));
  menu.Append(menuItem);
  GridCtrl->PopupMenu(&menu, pt);
}

void VectorKeywordsDialog::OnCmdRemoveKeyword(wxCommandEvent & WXUNUSED(event))
{
//
// deleting a Keyword [mouse action]
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_UnRegisterVectorCoverageKeyword(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((Keyword.Len() * 4) + 1);
  strcpy(dummy, Keyword.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    {
      GridCtrl->DeleteRows(CurrentRow, 1);
      List->MarkDeleted(Keyword);
    }
  sqlite3_finalize(stmt);
}

bool VectorKeywordsDialog::DoRegistetVectorCoverageKeyword(wxString & keyword)
{
//
// attempting to register a Vector Coverage Keyword
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  int value = 0;
  const char *sql = "SELECT SE_RegisterVectorCoverageKeyword(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((keyword.Len() * 4) + 1);
  strcpy(dummy, keyword.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE)
    ;
  else if (ret == SQLITE_ROW)
    value = sqlite3_column_int(stmt, 0);
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  if (!value)
    return false;
  return true;
}

void VectorKeywordsDialog::OnCmdAddKeyword(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *keywordCtrl = (wxTextCtrl *) FindWindow(ID_VECTOR_KEYWORD);
  wxString keyword = keywordCtrl->GetValue();
  if (keyword.Len() <= 0)
    {
      wxMessageBox(wxT("You must specify some Keyword !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (List->IsAlreadyDefinedKeyword(keyword) == true)
    {
      wxMessageBox(wxT("Already defined Keyword"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  // attempting to register the Keyword
  if (DoRegistetVectorCoverageKeyword(keyword) == true)
    {
      // updating the Grid
      int tot_rows = GridCtrl->GetNumberRows();
      if (tot_rows > 0)
        GridCtrl->DeleteRows(0, tot_rows);
      if (List != NULL)
        delete List;
      List = MainFrame->FindVectorKeywords(CoverageName);
      int count = 0;
      VectorCoverageKeyword *pK = List->GetFirst();
      while (pK)
        {
          // counting how many lines are there
          count++;
          pK = pK->GetNext();
        }
      GridCtrl->AppendRows(count);
      count = 0;
      char dummy[1024];
      wxString cell;
      pK = List->GetFirst();
      while (pK)
        {
          // feeding grid rows
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          GridCtrl->SetCellValue(count, 0, pK->GetKeyword());
          count++;
          pK = pK->GetNext();
        }
      GridCtrl->AutoSizeColumns();
    }
  return;
}

void VectorKeywordsDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

VectorCoverageKeywordsList::~VectorCoverageKeywordsList()
{
// destructor
  VectorCoverageKeyword *pK;
  VectorCoverageKeyword *pKn;
  pK = First;
  while (pK != NULL)
    {
      pKn = pK->GetNext();
      delete pK;
      pK = pKn;
    }
}

void VectorCoverageKeywordsList::Add(wxString & keyword)
{
// inserting a new Keyword
  VectorCoverageKeyword *pK = new VectorCoverageKeyword(keyword);
  if (First == NULL)
    First = pK;
  if (Last != NULL)
    Last->SetNext(pK);
  Last = pK;
}

void VectorCoverageKeywordsList::MarkDeleted(wxString & keyword)
{
// marking some Keyword as deleted
  VectorCoverageKeyword *pK;
  pK = First;
  while (pK != NULL)
    {
      if (pK->GetKeyword().CmpNoCase(keyword) == 0)
        {
          pK->MarkDeleted();
          return;
        }
      pK = pK->GetNext();
    }
}

bool VectorCoverageKeywordsList::IsAlreadyDefinedKeyword(wxString & keyword)
{
// checking for an already defined Keyword
  VectorCoverageKeyword *pK;
  pK = First;
  while (pK != NULL)
    {
      if (pK->GetKeyword().CmpNoCase(keyword) == 0)
        {
          if (pK->IsDeleted() == true)
            return false;
          return true;
        }
      pK = pK->GetNext();
    }
  return false;
}

VectorCoverageKeywordsList *MyFrame::FindVectorKeywords(wxString & coverage)
{
// will retrieve all Keywords for the given Coverage
  VectorCoverageKeywordsList *list = new VectorCoverageKeywordsList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  char *sql;
  char cvg[1024];

  wxCharBuffer buffer = coverage.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  sql =
    sqlite3_mprintf
    ("SELECT keyword FROM MAIN.vector_coverages_keyword WHERE coverage_name = %Q ORDER BY 1",
     cvg);
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString keyword = wxString::FromUTF8(value);
          list->Add(keyword);
        }
    }
  sqlite3_free_table(results);
  return list;
}

bool RasterSRIDsDialog::Create(MyFrame * parent, wxString & coverage)
{
//
// creating the dialog
//
  MainFrame = parent;
  int pos = coverage.Find(wxT(" [SRID="));
  if (pos != wxNOT_FOUND)
    CoverageName = coverage.Left(pos);
  else
    CoverageName = coverage;
  List = MainFrame->FindRasterAlternativeSRIDs(CoverageName);
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Raster Coverage: alternative SRIDs")) == false)
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

void RasterSRIDsDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Raster Coverage
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(550, 22),
                                       wxTE_READONLY);
  cvgCtrl->Enable(false);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered alternative SRIDs"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_RASTER_SRID_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  RasterCoverageSRID *pS = List->GetFirst();
  while (pS)
    {
      // counting how many lines are there
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->CreateGrid(count, 6, wxGrid::wxGridSelectCells);
  GridCtrl->SetColLabelValue(0, wxT("Remove"));
  GridCtrl->SetColLabelValue(1, wxT("Native"));
  GridCtrl->SetColLabelValue(2, wxT("SRID"));
  GridCtrl->SetColLabelValue(3, wxT("Auth Name"));
  GridCtrl->SetColLabelValue(4, wxT("Auth SRID"));
  GridCtrl->SetColLabelValue(5, wxT("RefSys Name"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pS = List->GetFirst();
  while (pS)
    {
      // feeding grid rows
      if (pS->IsNative() != true)
        {
          MyBitmapCellRenderer *renderer = new MyBitmapCellRenderer;
          wxBitmap bmp = wxBitmap(dustbin_xpm);
          renderer->SetGraphic(bmp);
          GridCtrl->SetCellRenderer(count, 0, renderer);
        }
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      if (pS->IsNative() == true)
        GridCtrl->SetCellValue(count, 1, wxT("yes"));
      else
        GridCtrl->SetCellValue(count, 1, wxT(""));
      sprintf(dummy, "%d", pS->GetSRID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 2, cell);
      GridCtrl->SetCellAlignment(count, 2, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 3, pS->GetAuthName());
      sprintf(dummy, "%d", pS->GetAuthSRID());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 4, cell);
      GridCtrl->SetCellAlignment(count, 4, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 5, pS->GetRefSysName());
      count++;
      pS = pS->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// SRID selection
  wxBoxSizer *sridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *sridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Adding an alternative SRID"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *SridSizer = new wxStaticBoxSizer(sridBox, wxHORIZONTAL);
  sridBoxSizer->Add(SridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *sridSizer = new wxBoxSizer(wxHORIZONTAL);
  SridSizer->Add(sridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *sridLabel = new wxStaticText(this, wxID_STATIC, wxT("&SRID:"));
  sridSizer->Add(sridLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  int srid = 0;
  wxSpinCtrl *sridCtrl = new wxSpinCtrl(this, ID_RASTER_SRID, wxEmptyString,
                                        wxDefaultPosition, wxSize(80, 20),
                                        wxSP_ARROW_KEYS,
                                        -1, 1000000, srid);
  sridSizer->Add(sridCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl =
    new wxTextCtrl(this, ID_RASTER_REFSYS, wxT("Undefined"),
                   wxDefaultPosition, wxSize(350, 22),
                   wxTE_READONLY);
  UpdateRefSysName(srid);
  sridSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *addNew = new wxButton(this, ID_RASTER_SRID_ADD, wxT("&Add"));
  addNew->Enable(false);
  sridSizer->Add(addNew, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSRIDsDialog::OnQuit);
  Connect(ID_RASTER_SRID_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSRIDsDialog::OnCmdAddSrid);
  Connect(ID_RASTER_SRID_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & RasterSRIDsDialog::OnCmdRemoveSrid);
  Connect(wxID_ANY, wxEVT_GRID_CELL_RIGHT_CLICK,
          (wxObjectEventFunction) & RasterSRIDsDialog::OnRightClick);
  Connect(wxID_ANY, wxEVT_GRID_SELECT_CELL,
          (wxObjectEventFunction) & RasterSRIDsDialog::OnCellSelected);
  Connect(ID_RASTER_SRID, wxEVT_COMMAND_SPINCTRL_UPDATED,
          (wxObjectEventFunction) & RasterSRIDsDialog::OnSridChanged);
  Connect(ID_RASTER_SRID, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) & RasterSRIDsDialog::OnSridChanged);
}

bool RasterSRIDsDialog::UpdateRefSysName(int srid)
{
//
// updating the RefSys Name
//
  sqlite3 *sqlite = MainFrame->GetSqlite();
  char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  char *value;
  bool valid = false;
  wxString name(wxT("Undefined"));

  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_RASTER_REFSYS);
  if (srid <= 0)
    goto end;

  sql =
    sqlite3_mprintf("SELECT ref_sys_name FROM spatial_ref_sys WHERE srid = %d",
                    srid);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto end;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          name = wxString::FromUTF8(value);
          valid = true;
        }
    }
  sqlite3_free_table(results);
end:
  nameCtrl->SetValue(name);
  return valid;
}

void RasterSRIDsDialog::OnSridChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Srid changed
//
  wxButton *addNew = (wxButton *) FindWindow(ID_RASTER_SRID_ADD);
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_RASTER_SRID);
  int srid = sridCtrl->GetValue();
  bool ret = UpdateRefSysName(srid);
  addNew->Enable(ret);
}

void RasterSRIDsDialog::OnRightClick(wxGridEvent & event)
{
//
// right click on some cell [mouse action]
//
  wxMenu menu;
  wxMenuItem *menuItem;
  wxPoint pt = event.GetPosition();
  CurrentRow = event.GetRow();
  GridCtrl->SelectRow(CurrentRow);
  wxString native = GridCtrl->GetCellValue(CurrentRow, 0);
  if (native.CmpNoCase(wxT("yes")) == 0)
    return;
  wxString id = GridCtrl->GetCellValue(CurrentRow, 1);
  long srid;
  id.ToLong(&srid);
  CurrentSRID = srid;
  menuItem = new wxMenuItem(&menu, ID_RASTER_SRID_REMOVE, wxT("&Remove"));
  menu.Append(menuItem);
  GridCtrl->PopupMenu(&menu, pt);
}

void RasterSRIDsDialog::OnCellSelected(wxGridEvent & event)
{
//
// cell selection changed
//
  if (GridCtrl->IsShown() == false)
    return;
  CurrentRow = event.GetRow();
  GridCtrl->SelectRow(CurrentRow);
  wxString native = GridCtrl->GetCellValue(CurrentRow, 1);
  if (event.GetCol() != 0)
    return;
  if (native.CmpNoCase(wxT("yes")) == 0)
    return;
  wxString id = GridCtrl->GetCellValue(CurrentRow, 2);
  long srid;
  id.ToLong(&srid);
  CurrentSRID = srid;
  DoRemoveSRID();
}

void RasterSRIDsDialog::OnCmdRemoveSrid(wxCommandEvent & WXUNUSED(event))
{
//
// deleting a SRID [mouse action]
//
  GridCtrl->SelectRow(CurrentRow);
  wxString native = GridCtrl->GetCellValue(CurrentRow, 1);
  if (native.CmpNoCase(wxT("yes")) == 0)
    return;
  wxString id = GridCtrl->GetCellValue(CurrentRow, 2);
  long srid;
  id.ToLong(&srid);
  CurrentSRID = srid;
  DoRemoveSRID();
}

void RasterSRIDsDialog::DoRemoveSRID()
{
//
// deleting a SRID
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;

  dummy =
    sqlite3_mprintf
    ("Do you really intend removing SRID=%d from this Raster Coverage ?",
     CurrentSRID);
  wxString msg = wxString::FromUTF8(dummy);
  sqlite3_free(dummy);
  if (wxMessageBox
      (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_QUESTION, this) != wxYES)
    return;

  const char *sql = "SELECT SE_UnRegisterRasterCoverageSrid(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_int(stmt, 2, CurrentSRID);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    {
      GridCtrl->DeleteRows(CurrentRow, 1);
      List->MarkDeleted(CurrentSRID);
    }
  sqlite3_finalize(stmt);
}

bool RasterSRIDsDialog::DoRegistetRasterCoverageSrid(int srid)
{
//
// attempting to register an alternative Raster Coverage SRID
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  int value = 0;
  const char *sql = "SELECT SE_RegisterRasterCoverageSrid(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_int(stmt, 2, srid);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE)
    ;
  else if (ret == SQLITE_ROW)
    value = sqlite3_column_int(stmt, 0);
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  if (!value)
    return false;
  return true;
}

void RasterSRIDsDialog::OnCmdAddSrid(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_RASTER_SRID);
  int srid = sridCtrl->GetValue();
  if (srid <= 0)
    {
      wxMessageBox(wxT("You must specify some SRID value !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
  } else if (MainFrame->SRIDnotExists(srid) == true)
    {
      wxMessageBox(wxT("invalid SRID value"), wxT("spatialite_gui"),
                   wxOK | wxICON_WARNING, this);
      return;
    }
  if (List->IsNativeSRID(srid) == true)
    {
      wxMessageBox(wxT("Can't register twice the Native SRID"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (List->IsAlreadyDefinedSRID(srid) == true)
    {
      wxMessageBox(wxT("Already defined alternative SRID"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  // attempting to register the alternative SRID
  if (DoRegistetRasterCoverageSrid(srid) == true)
    {
      // updating the Grid
      GridCtrl->Show(false);
      int tot_rows = GridCtrl->GetNumberRows();
      if (tot_rows > 0)
        GridCtrl->DeleteRows(0, tot_rows);
      if (List != NULL)
        delete List;
      List = MainFrame->FindRasterAlternativeSRIDs(CoverageName);
      int count = 0;
      RasterCoverageSRID *pS = List->GetFirst();
      while (pS)
        {
          // counting how many lines are there
          count++;
          pS = pS->GetNext();
        }
      GridCtrl->AppendRows(count);
      GridCtrl->ClearSelection();
      count = 0;
      char dummy[1024];
      wxString cell;
      pS = List->GetFirst();
      while (pS)
        {
          // feeding grid rows
          if (pS->IsNative() != true)
            {
              MyBitmapCellRenderer *renderer = new MyBitmapCellRenderer;
              wxBitmap bmp = wxBitmap(dustbin_xpm);
              renderer->SetGraphic(bmp);
              GridCtrl->SetCellRenderer(count, 0, renderer);
            }
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          if (pS->IsNative() == true)
            GridCtrl->SetCellValue(count, 1, wxT("yes"));
          else
            GridCtrl->SetCellValue(count, 1, wxT(""));
          sprintf(dummy, "%d", pS->GetSRID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 2, cell);
          GridCtrl->SetCellAlignment(count, 2, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 3, pS->GetAuthName());
          sprintf(dummy, "%d", pS->GetAuthSRID());
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetCellValue(count, 4, cell);
          GridCtrl->SetCellAlignment(count, 4, wxALIGN_RIGHT, wxALIGN_TOP);
          GridCtrl->SetCellValue(count, 5, pS->GetRefSysName());
          count++;
          pS = pS->GetNext();
        }
      GridCtrl->AutoSizeColumns();
      GridCtrl->Show(true);
    }
  return;
}

void RasterSRIDsDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

RasterCoverageSRIDsList::~RasterCoverageSRIDsList()
{
// destructor
  RasterCoverageSRID *pS;
  RasterCoverageSRID *pSn;
  pS = First;
  while (pS != NULL)
    {
      pSn = pS->GetNext();
      delete pS;
      pS = pSn;
    }
}

void RasterCoverageSRIDsList::Add(bool native, int srid, wxString & auth_name,
                                  int auth_srid, wxString & name)
{
// inserting a new SRID
  RasterCoverageSRID *pS =
    new RasterCoverageSRID(native, srid, auth_name, auth_srid, name);
  if (First == NULL)
    First = pS;
  if (Last != NULL)
    Last->SetNext(pS);
  Last = pS;
}

void RasterCoverageSRIDsList::MarkDeleted(int srid)
{
// marking some Srid as deleted
  RasterCoverageSRID *pS;
  pS = First;
  while (pS != NULL)
    {
      if (pS->GetSRID() == srid)
        {
          pS->MarkDeleted();
          return;
        }
      pS = pS->GetNext();
    }
}

bool RasterCoverageSRIDsList::IsNativeSRID(int srid)
{
// checking for the Native SRID
  RasterCoverageSRID *pS;
  pS = First;
  while (pS != NULL)
    {
      if (pS->GetSRID() == srid)
        return pS->IsNative();
      pS = pS->GetNext();
    }
  return false;
}

bool RasterCoverageSRIDsList::IsAlreadyDefinedSRID(int srid)
{
// checking for an already defined alternative SRID
  RasterCoverageSRID *pS;
  pS = First;
  while (pS != NULL)
    {
      if (pS->GetSRID() == srid)
        {
          if (pS->IsDeleted() == true)
            return false;
          return true;
        }
      pS = pS->GetNext();
    }
  return false;
}

RasterCoverageSRIDsList *MyFrame::FindRasterAlternativeSRIDs(wxString &
                                                             coverage)
{
// will retrieve all alternative SRIDs for the given Coverage
  RasterCoverageSRIDsList *list = new RasterCoverageSRIDsList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  char *sql;
  char cvg[1024];

  wxCharBuffer buffer = coverage.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  sql =
    sqlite3_mprintf
    ("SELECT 1, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.raster_coverages AS v "
     "LEFT JOIN spatial_ref_sys AS s ON (v.srid = s.srid) "
     "WHERE v.coverage_name = %Q UNION "
     "SELECT 0, s.srid, s.auth_name, s.auth_srid, s.ref_sys_name FROM MAIN.raster_coverages AS v "
     "JOIN MAIN.raster_coverages_srid AS x ON (v.coverage_name = x.coverage_name) "
     "LEFT JOIN MAIN.spatial_ref_sys AS s ON (x.srid = s.srid) "
     "WHERE v.coverage_name = %Q ORDER BY 2", cvg, cvg);
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          bool native = false;
          value = results[(i * columns) + 0];
          if (atoi(value) != 0)
            native = true;
          value = results[(i * columns) + 1];
          int srid = atoi(value);
          value = results[(i * columns) + 2];
          wxString auth_name = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          int auth_srid = atoi(value);
          value = results[(i * columns) + 4];
          wxString name = wxString::FromUTF8(value);
          list->Add(native, srid, auth_name, auth_srid, name);
        }
    }
  sqlite3_free_table(results);
  return list;
}

bool RasterKeywordsDialog::Create(MyFrame * parent, wxString & coverage)
{
//
// creating the dialog
//
  MainFrame = parent;
  int pos = coverage.Find(wxT(" [SRID="));
  if (pos != wxNOT_FOUND)
    CoverageName = coverage.Left(pos);
  else
    CoverageName = coverage;
  List = MainFrame->FindRasterKeywords(CoverageName);
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Raster Coverage: Keywords")) == false)
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

void RasterKeywordsDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Raster Coverage
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(550, 22),
                                       wxTE_READONLY);
  cvgCtrl->Enable(false);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered Keywords"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_RASTER_KEYWORD_GRID, wxDefaultPosition,
               wxSize(640, 200));
  int count = 0;
  RasterCoverageKeyword *pK = List->GetFirst();
  while (pK)
    {
      // counting how many lines are there
      count++;
      pK = pK->GetNext();
    }
  GridCtrl->CreateGrid(count, 1, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Keyword"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  count = 0;
  char dummy[1024];
  wxString cell;
  pK = List->GetFirst();
  while (pK)
    {
      // feeding grid rows
      sprintf(dummy, "%d", count + 1);
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetRowLabelValue(count, cell);
      GridCtrl->SetCellValue(count, 0, pK->GetKeyword());
      count++;
      pK = pK->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// Keyword selection
  wxBoxSizer *keywordBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(keywordBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *keywordBox = new wxStaticBox(this, wxID_STATIC,
                                            wxT("Adding a Keyword"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *KeywordSizer = new wxStaticBoxSizer(keywordBox, wxHORIZONTAL);
  keywordBoxSizer->Add(KeywordSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *keywordSizer = new wxBoxSizer(wxHORIZONTAL);
  KeywordSizer->Add(keywordSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *keywordLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Keyword:"));
  keywordSizer->Add(keywordLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *keywordCtrl = new wxTextCtrl(this, ID_RASTER_KEYWORD, Keyword,
                                           wxDefaultPosition, wxSize(400, 22));
  keywordSizer->Add(keywordCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  keywordSizer->AddSpacer(100);
  wxButton *addNew =
    new wxButton(this, ID_RASTER_KEYWORD_ADD, wxT("&Add Keyword"));
  keywordSizer->Add(addNew, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterKeywordsDialog::OnQuit);
  Connect(ID_RASTER_KEYWORD_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterKeywordsDialog::OnCmdAddKeyword);
  Connect(ID_RASTER_KEYWORD_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & RasterKeywordsDialog::OnCmdRemoveKeyword);
  Connect(wxID_ANY, wxEVT_GRID_CELL_RIGHT_CLICK,
          (wxObjectEventFunction) & RasterKeywordsDialog::OnRightClick);
}

void RasterKeywordsDialog::OnRightClick(wxGridEvent & event)
{
//
// right click on some cell [mouse action]
//
  wxMenu menu;
  wxMenuItem *menuItem;
  wxPoint pt = event.GetPosition();
  CurrentRow = event.GetRow();
  GridCtrl->SelectRow(CurrentRow);
  Keyword = GridCtrl->GetCellValue(CurrentRow, 0);
  menuItem = new wxMenuItem(&menu, ID_RASTER_KEYWORD_REMOVE, wxT("&Remove"));
  menu.Append(menuItem);
  GridCtrl->PopupMenu(&menu, pt);
}

void RasterKeywordsDialog::OnCmdRemoveKeyword(wxCommandEvent & WXUNUSED(event))
{
//
// deleting a Keyword [mouse action]
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_UnRegisterRasterCoverageKeyword(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((Keyword.Len() * 4) + 1);
  strcpy(dummy, Keyword.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    {
      GridCtrl->DeleteRows(CurrentRow, 1);
      List->MarkDeleted(Keyword);
    }
  sqlite3_finalize(stmt);
}

bool RasterKeywordsDialog::DoRegistetRasterCoverageKeyword(wxString & keyword)
{
//
// attempting to register a Raster Coverage Keyword
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  int value = 0;
  const char *sql = "SELECT SE_RegisterRasterCoverageKeyword(?, ?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((keyword.Len() * 4) + 1);
  strcpy(dummy, keyword.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE)
    ;
  else if (ret == SQLITE_ROW)
    value = sqlite3_column_int(stmt, 0);
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  if (!value)
    return false;
  return true;
}

void RasterKeywordsDialog::OnCmdAddKeyword(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *keywordCtrl = (wxTextCtrl *) FindWindow(ID_RASTER_KEYWORD);
  wxString keyword = keywordCtrl->GetValue();
  if (keyword.Len() <= 0)
    {
      wxMessageBox(wxT("You must specify some Keyword !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (List->IsAlreadyDefinedKeyword(keyword) == true)
    {
      wxMessageBox(wxT("Already defined Keyword"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  // attempting to register the Keyword
  if (DoRegistetRasterCoverageKeyword(keyword) == true)
    {
      // updating the Grid
      int tot_rows = GridCtrl->GetNumberRows();
      if (tot_rows > 0)
        GridCtrl->DeleteRows(0, tot_rows);
      if (List != NULL)
        delete List;
      List = MainFrame->FindRasterKeywords(CoverageName);
      int count = 0;
      RasterCoverageKeyword *pK = List->GetFirst();
      while (pK)
        {
          // counting how many lines are there
          count++;
          pK = pK->GetNext();
        }
      GridCtrl->AppendRows(count);
      count = 0;
      char dummy[1024];
      wxString cell;
      pK = List->GetFirst();
      while (pK)
        {
          // feeding grid rows
          sprintf(dummy, "%d", count + 1);
          cell = wxString::FromUTF8(dummy);
          GridCtrl->SetRowLabelValue(count, cell);
          GridCtrl->SetCellValue(count, 0, pK->GetKeyword());
          count++;
          pK = pK->GetNext();
        }
      GridCtrl->AutoSizeColumns();
    }
  return;
}

void RasterKeywordsDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

RasterCoverageKeywordsList::~RasterCoverageKeywordsList()
{
// destructor
  RasterCoverageKeyword *pK;
  RasterCoverageKeyword *pKn;
  pK = First;
  while (pK != NULL)
    {
      pKn = pK->GetNext();
      delete pK;
      pK = pKn;
    }
}

void RasterCoverageKeywordsList::Add(wxString & keyword)
{
// inserting a new Keyword
  RasterCoverageKeyword *pK = new RasterCoverageKeyword(keyword);
  if (First == NULL)
    First = pK;
  if (Last != NULL)
    Last->SetNext(pK);
  Last = pK;
}

void RasterCoverageKeywordsList::MarkDeleted(wxString & keyword)
{
// marking some Keyword as deleted
  RasterCoverageKeyword *pK;
  pK = First;
  while (pK != NULL)
    {
      if (pK->GetKeyword().CmpNoCase(keyword) == 0)
        {
          pK->MarkDeleted();
          return;
        }
      pK = pK->GetNext();
    }
}

bool RasterCoverageKeywordsList::IsAlreadyDefinedKeyword(wxString & keyword)
{
// checking for an already defined Keyword
  RasterCoverageKeyword *pK;
  pK = First;
  while (pK != NULL)
    {
      if (pK->GetKeyword().CmpNoCase(keyword) == 0)
        {
          if (pK->IsDeleted() == true)
            return false;
          return true;
        }
      pK = pK->GetNext();
    }
  return false;
}

RasterCoverageKeywordsList *MyFrame::FindRasterKeywords(wxString & coverage)
{
// will retrieve all Keywords for the given Coverage
  RasterCoverageKeywordsList *list = new RasterCoverageKeywordsList();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  char *sql;
  char cvg[1024];

  wxCharBuffer buffer = coverage.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  sql =
    sqlite3_mprintf
    ("SELECT keyword FROM MAIN.raster_coverages_keyword WHERE coverage_name = %Q ORDER BY 1",
     cvg);
  int ret = sqlite3_get_table(SqliteHandle, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return list;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          wxString keyword = wxString::FromUTF8(value);
          list->Add(keyword);
        }
    }
  sqlite3_free_table(results);
  return list;
}

bool LoadExternalGraphicDialog::Create(MyFrame * parent, wxArrayString & paths,
                                       wxString & path)
{
//
// creating the dialog
//
  MainFrame = parent;
  Paths = paths;
  Path = path;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Loading New External Graphic resource(s)")) ==
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

void LoadExternalGraphicDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: files to be imported
  wxBoxSizer *fileSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(fileSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *fileLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Import &File(s):"));
  fileSizer->Add(fileLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *filesCtrl = new wxTextCtrl(this, wxID_ANY, Path,
                                         wxDefaultPosition, wxSize(600, 60),
                                         wxTE_MULTILINE | wxTE_READONLY);
  fileSizer->Add(filesCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: progress report
  wxBoxSizer *progrSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(progrSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *doneCtrl = new wxTextCtrl(this, ID_LOAD_EXTERNAL_DONE, ListDone,
                                        wxDefaultPosition, wxSize(650, 100),
                                        wxTE_MULTILINE | wxTE_READONLY |
                                        wxTE_RICH2);
  progrSizer->Add(doneCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Import"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *abort = new wxButton(this, ID_LOAD_ABORT, wxT("&Abort"));
  abort->Enable(false);
  okCancelBox->Add(abort, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadExternalGraphicDialog::OnOk);
  Connect(ID_LOAD_ABORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadExternalGraphicDialog::OnCmdAbort);
  Connect(ID_LOAD_EXTERNAL_START, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadExternalGraphicDialog::OnRequestStart);
  Connect(ID_LOAD_EXTERNAL_STOP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadExternalGraphicDialog::OnRequestStop);
  Connect(ID_LOAD_EXTERNAL_SKIP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadExternalGraphicDialog::OnRequestSkip);
  Connect(ID_LOAD_EXTERNAL_THREAD_FINISHED, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          LoadExternalGraphicDialog::OnThreadFinished);
}

void LoadExternalGraphicDialog::OnCmdAbort(wxCommandEvent & WXUNUSED(event))
{
//
// aborting the External Graphic(s) Import process
//
  if (Params.IsAbortPending() == true)
    return;
  Params.RequestAbort();
  wxString report =
    wxT("\nan ABORT request is now pending and will be accepted ASAP");
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_EXTERNAL_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(192, 0, 0);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(report);
}

void LoadExternalGraphicDialog::OnRequestStart(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_EXTERNAL_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(0, 0, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(msg);
  doneCtrl->MarkDirty();
}

void LoadExternalGraphicDialog::OnRequestStop(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_EXTERNAL_DONE);
  ListDone += msg;
  doneCtrl->Clear();
  wxColour fore = wxColour(0, 0, 0);
  wxColour back = wxColour(255, 255, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(ListDone);
  doneCtrl->MarkDirty();
}

void LoadExternalGraphicDialog::OnRequestSkip(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_EXTERNAL_DONE);
  ListDone += msg;
  doneCtrl->Clear();
  wxColour fore = wxColour(0, 0, 0);
  wxColour back = wxColour(255, 0, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(ListDone);
  doneCtrl->MarkDirty();
}

#if defined(_WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteExternalGraphicLoad(void *arg)
#else
void *DoExecuteExternalGraphicLoad(void *arg)
#endif
{
//
// threaded function: processing an External Graphic Import operation
//
  ExternalGraphicLoadParams *params = (ExternalGraphicLoadParams *) arg;
  const char *sql;
  int ret;
  sqlite3_stmt *stmt = NULL;
  int count = params->GetPathsCount();
  int i;
  clock_t clock_start;
  clock_t clock_end;
  double seconds;
  char elapsed[64];
  char ordinal[64];
  wxString report;
  wxString path;
  void *blob;
  int blob_size;
  wxCommandEvent evt_start(wxEVT_COMMAND_BUTTON_CLICKED,
                           ID_LOAD_EXTERNAL_START);
  wxCommandEvent evt_stop(wxEVT_COMMAND_BUTTON_CLICKED, ID_LOAD_EXTERNAL_STOP);
  wxCommandEvent evt_skip(wxEVT_COMMAND_BUTTON_CLICKED, ID_LOAD_EXTERNAL_SKIP);

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

  sql = "SELECT SE_RegisterExternalGraphic(?, ?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(params->GetMainFrame()->GetSqlite(), sql, strlen(sql),
                       &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      params->SetError();
      goto error;
    }

  for (i = 0; i < count; i++)
    {
      // loading and verifying each External Graphic resource
      if (params->IsAbortPending() == true)
        {
          report = wxT("STOP .... aborted by the user !!!!");
          evt_start.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
          break;
        }
      path = params->GetPathByIndex(i);
      params->SetCurrentPath(path);
      report = wxT("Loading and Validating: ") + path;
      evt_start.SetString(report);
      params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
      clock_start = clock();
      wxString abstract;
      if (params->GetMainFrame()->
          ValidateExternalGraphicResource(path.ToUTF8(), &blob, &blob_size,
                                          abstract) == true)
        {
          wxFileName fn(path);
          wxString xlink_href =
            wxT("http://www.utopia.gov/") + fn.GetFullName();
          wxString filename = fn.GetFullName();
          wxString title = fn.GetName();
          if (params->GetDlg()->
              RegisterExternalGraphic(stmt, xlink_href.ToUTF8(), title.ToUTF8(),
                                      abstract.ToUTF8(), filename.ToUTF8(),
                                      blob, blob_size) != true)
            {
              params->SetError();
              goto error;
            }
          clock_end = clock();
          seconds =
            (double) (clock_end - clock_start) / (double) CLOCKS_PER_SEC;
          MyResultSetView::FormatElapsedTime(seconds, elapsed);
          sprintf(ordinal, "done %d/%d: ", i + 1, count);
          report =
            wxString::FromUTF8(ordinal) + path + wxT("    [") +
            wxString::FromUTF8(elapsed) + wxT("]\n");
          evt_stop.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
          params->Done();
      } else
        {
          clock_end = clock();
          seconds =
            (double) (clock_end - clock_start) / (double) CLOCKS_PER_SEC;
          MyResultSetView::FormatElapsedTime(seconds, elapsed);
          sprintf(ordinal, "discarded %d/%d (not a valid External Graphic): ",
                  i + 1, count);
          report =
            wxString::FromUTF8(ordinal) + path + wxT("    [") +
            wxString::FromUTF8(elapsed) + wxT("]\n");
          evt_skip.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_skip);
        }
    }
  sqlite3_finalize(stmt);
  goto end;

error:
  sqlite3_finalize(stmt);
  report = wxT("FAILED: ") + path;
  evt_stop.SetString(report);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
  params->SetError();
end:
  wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED,
                       ID_LOAD_EXTERNAL_THREAD_FINISHED);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(event);
#if defined(_WIN32) && !defined(__MINGW32__)
  return 0;
#else
  pthread_exit(NULL);
  return NULL;
#endif
}

void LoadExternalGraphicDialog::
OnThreadFinished(wxCommandEvent & WXUNUSED(event))
{
// resuming execution when the Import External Graphic thread quits
  ::wxEndBusyCursor();
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  quitBtn->Enable(true);
  abortBtn->Enable(false);
  sqlite3_exec(Params.GetMainFrame()->GetSqlite(), "COMMIT", NULL, NULL, NULL);
  if (Params.GetError() == true)
    {
      char dummy[80];
      sprintf(dummy,
              "%d External Graphic resources have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nA fatal error occurred while loading:\n") +
                   Params.GetCurrentPath(), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
  } else if (Params.IsAbortPending() == true)
    {
      char dummy[80];
      sprintf(dummy,
              "%d External Graphic resources have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nStopped by an Abort user request"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  } else
    {
      char dummy[80];
      sprintf(dummy,
              "%d External Graphic resources have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
    }
}

bool MyFrame::ValidateExternalGraphicResource(const char *path, void **blob,
                                              int *blob_size,
                                              wxString & abstract)
{
//
// attempting to load and validate an External Graphic
//
  int ret;
  sqlite3_stmt *stmt;
  void *xblob = NULL;
  int xblob_size;
  int valid = 0;

// Loading from file - testing for an eventual SVG image
  char *sql = sqlite3_mprintf("SELECT XB_Create(BlobFromFile(%Q), 1)", path);
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return false;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              const void *xxblob = sqlite3_column_blob(stmt, 0);
              xblob_size = sqlite3_column_bytes(stmt, 0);
              xblob = malloc(xblob_size);
              memcpy(xblob, xxblob, xblob_size);
            }
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (xblob != NULL)
    goto ok_svg;
// Loading from file - testing for an eventual SVG image
  sql = sqlite3_mprintf("SELECT BlobFromFile(%Q)", path);
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return false;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              const void *xxblob = sqlite3_column_blob(stmt, 0);
              xblob_size = sqlite3_column_bytes(stmt, 0);
              xblob = malloc(xblob_size);
              memcpy(xblob, xxblob, xblob_size);
            }
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (xblob == NULL)
    return false;
ok_svg:

// Checking if really is a valid External Graphic resource
  stmt = NULL;
  sql = sqlite3_mprintf("SELECT GetMimeType(?)");
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    goto invalid;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_blob(stmt, 1, xblob, xblob_size, SQLITE_STATIC);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_TEXT)
            {
              rl2RasterPtr raster;
              unsigned int width;
              unsigned int height;
              char dummy[128];
              const char *mime_type =
                (const char *) sqlite3_column_text(stmt, 0);
              if (strcmp(mime_type, "image/gif") == 0)
                {
                  abstract = wxT("GIF icon: ");
                  raster =
                    rl2_raster_from_gif((const unsigned char *) xblob,
                                        xblob_size);
                  if (raster != NULL)
                    {
                      rl2_get_raster_size(raster, &width, &height);
                      sprintf(dummy, "%u X %u", width, height);
                      abstract += wxString::FromUTF8(dummy);
                      rl2_destroy_raster(raster);
                    }
                  valid = 1;
                }
              if (strcmp(mime_type, "image/png") == 0)
                {
                  abstract = wxT("PNG icon: ");
                  raster =
                    rl2_raster_from_png((const unsigned char *) xblob,
                                        xblob_size, 1);
                  if (raster != NULL)
                    {
                      rl2_get_raster_size(raster, &width, &height);
                      sprintf(dummy, "%u X %u", width, height);
                      abstract += wxString::FromUTF8(dummy);
                      rl2_destroy_raster(raster);
                    }
                  valid = 1;
                }
              if (strcmp(mime_type, "image/jpeg") == 0)
                {
                  abstract = wxT("JPEG icon: ");
                  raster =
                    rl2_raster_from_jpeg((const unsigned char *) xblob,
                                         xblob_size);
                  if (raster != NULL)
                    {
                      rl2_get_raster_size(raster, &width, &height);
                      sprintf(dummy, "%u X %u", width, height);
                      abstract += wxString::FromUTF8(dummy);
                      rl2_destroy_raster(raster);
                    }
                  valid = 1;
                }
              if (strcmp(mime_type, "image/svg+xml") == 0)
                {
                  abstract = wxT("SVG symbol");
                  valid = 1;
                }
            }
      } else
        goto invalid;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;
  if (!valid)
    goto invalid;
  *blob = xblob;
  *blob_size = xblob_size;
  return true;

invalid:
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  free(xblob);
  *blob = NULL;
  *blob_size = 0;
  return false;
}

bool LoadExternalGraphicDialog::RegisterExternalGraphic(sqlite3_stmt * stmt,
                                                        const char *xlink_href,
                                                        const char *title,
                                                        const char *abstract,
                                                        const char *filename,
                                                        void *blob,
                                                        int blob_size)
{
//
// attempting to register the new Raster Style
//
  int ret;
  int valid = 0;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_text(stmt, 1, xlink_href, strlen(xlink_href), SQLITE_STATIC);
  sqlite3_bind_blob(stmt, 2, blob, blob_size, free);
  sqlite3_bind_text(stmt, 3, title, strlen(title), SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, abstract, strlen(abstract), SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, filename, strlen(filename), SQLITE_STATIC);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        return false;
    }
  if (valid)
    return true;
  return false;
}

void LoadExternalGraphicDialog::DoRunLoad()
{
//
// executing the Load External Graphic resource(s) process in a separate Thread
//
#if defined(_WIN32) && !defined(__MINGW32__)
  HANDLE thread_handle;
  DWORD dwThreadId;
#else
  pthread_t thread_id;
#endif
  Params.Initialize(MainFrame, this, Paths);
#if defined(_WIN32) && !defined(__MINGW32__)
  thread_handle =
    CreateThread(NULL, 0, DoExecuteExternalGraphicLoad, &Params, 0,
                 &dwThreadId);
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
              pthread_create(&thread_id, &attr, DoExecuteExternalGraphicLoad,
                             &Params);
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
                  pthread_create(&thread_id, &attr,
                                 DoExecuteExternalGraphicLoad, &Params);
                  pthread_detach(thread_id);
                }
            }
        }
    }
#endif
  if (!ok_prior)
    {
      // failure: using standard priority
      pthread_create(&thread_id, NULL, DoExecuteExternalGraphicLoad, &Params);
      pthread_detach(thread_id);
    }
#endif
}

void LoadExternalGraphicDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxButton *loadBtn = (wxButton *) FindWindow(wxID_OK);
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  loadBtn->Enable(false);
  quitBtn->Enable(false);
  abortBtn->Enable(true);
  ::wxBeginBusyCursor();
  char *errMsg = NULL;
  int ret = sqlite3_exec(MainFrame->GetSqlite(), "BEGIN", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  ret =
    sqlite3_exec(MainFrame->GetSqlite(), "SELECT CreateStylingTables()", NULL,
                 NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CreateStylingTables() error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }

  DoRunLoad();
}

bool UnregisterExternalGraphicDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  List = MainFrame->FindExternalGraphic(false);
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Unregistering an External Graphic resource")) ==
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

void UnregisterExternalGraphicDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT
                                         ("Registered External Graphic resources"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  ExternalGraphic *pE = List->GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl->CreateGrid(count, 5, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("xlink:href"));
  GridCtrl->SetColLabelValue(1, wxT("Graphic"));
  GridCtrl->SetColLabelValue(2, wxT("Title"));
  GridCtrl->SetColLabelValue(3, wxT("Abstract"));
  GridCtrl->SetColLabelValue(4, wxT("MimeType"));
  count = 0;
  pE = List->GetFirst();
  while (pE)
    {
      // feeding grid rows
      GridCtrl->SetCellValue(count, 0, pE->GetXLinkHref());
      MyGraphicCellRenderer *renderer = new MyGraphicCellRenderer;
      renderer->SetGraphic(pE->GetGraphic());
      GridCtrl->SetCellRenderer(count, 1, renderer);
      GridCtrl->SetCellValue(count, 2, pE->GetTitle());
      GridCtrl->SetCellValue(count, 3, pE->GetAbstract());
      GridCtrl->SetCellValue(count, 4, pE->GetMimeType());
      count++;
      pE = pE->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Unregister"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & UnregisterExternalGraphicDialog::OnOk);
}

bool UnregisterExternalGraphicDialog::DoUnregisterExternalGraphic(const char
                                                                  *xlink_href)
{
//
// attempting to unregister the External Graphic resource
//
  int ret;
  int valid = 0;
  const char *sql;
  sqlite3_stmt *stmt = NULL;

  sql = "SELECT SE_UnregisterExternalGraphic(?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_text(stmt, 1, xlink_href, strlen(xlink_href), SQLITE_STATIC);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (valid)
    return true;
  return false;
}

void UnregisterExternalGraphicDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  int selCount = 0;
  wxString xlink_href;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          xlink_href = GridCtrl->GetCellValue(i, 0);
          selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT
                   ("You must select an External Graphic resource to be unregistered !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT
        ("You must select just a single External Graphic resource to be unregistered !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  ::wxBeginBusyCursor();
  if (DoUnregisterExternalGraphic(xlink_href.ToUTF8()) == true)
    {
      ::wxEndBusyCursor();
      wxMessageBox(wxT("External Graphic (xlink:href=") + xlink_href +
                   wxT(") successfully unregistered"),
                   wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  } else
    {
      ::wxEndBusyCursor();
      wxMessageBox(wxT
                   ("Some error occurred: unable to unregister External Graphic (xlink:href=")
                   + xlink_href + wxT(")"), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
    }

  wxDialog::EndModal(wxID_OK);
}

bool LoadTextFontDialog::Create(MyFrame * parent, wxArrayString & paths,
                                wxString & path)
{
//
// creating the dialog
//
  MainFrame = parent;
  Paths = paths;
  Path = path;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Loading New Text Font(s)")) == false)
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

void LoadTextFontDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: files to be imported
  wxBoxSizer *fileSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(fileSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *fileLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Import &File(s):"));
  fileSizer->Add(fileLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *filesCtrl = new wxTextCtrl(this, wxID_ANY, Path,
                                         wxDefaultPosition, wxSize(600, 60),
                                         wxTE_MULTILINE | wxTE_READONLY);
  fileSizer->Add(filesCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: progress report
  wxBoxSizer *progrSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(progrSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *doneCtrl = new wxTextCtrl(this, ID_LOAD_FONT_DONE, ListDone,
                                        wxDefaultPosition, wxSize(650, 100),
                                        wxTE_MULTILINE | wxTE_READONLY |
                                        wxTE_RICH2);
  progrSizer->Add(doneCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Import"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *abort = new wxButton(this, ID_LOAD_ABORT, wxT("&Abort"));
  abort->Enable(false);
  okCancelBox->Add(abort, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadTextFontDialog::OnOk);
  Connect(ID_LOAD_ABORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadTextFontDialog::OnCmdAbort);
  Connect(ID_LOAD_FONT_START, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadTextFontDialog::OnRequestStart);
  Connect(ID_LOAD_FONT_STOP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadTextFontDialog::OnRequestStop);
  Connect(ID_LOAD_FONT_SKIP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadTextFontDialog::OnRequestSkip);
  Connect(ID_LOAD_FONT_THREAD_FINISHED, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & LoadTextFontDialog::OnThreadFinished);
}

void LoadTextFontDialog::OnCmdAbort(wxCommandEvent & WXUNUSED(event))
{
//
// aborting the Text Font(s) Import process
//
  if (Params.IsAbortPending() == true)
    return;
  Params.RequestAbort();
  wxString report =
    wxT("\nan ABORT request is now pending and will be accepted ASAP");
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_FONT_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(192, 0, 0);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(report);
}

void LoadTextFontDialog::OnRequestStart(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_FONT_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(0, 0, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(msg);
  doneCtrl->MarkDirty();
}

void LoadTextFontDialog::OnRequestStop(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_FONT_DONE);
  ListDone += msg;
  doneCtrl->Clear();
  wxColour fore = wxColour(0, 0, 0);
  wxColour back = wxColour(255, 255, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(ListDone);
  doneCtrl->MarkDirty();
}

void LoadTextFontDialog::OnRequestSkip(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_FONT_DONE);
  ListDone += msg;
  doneCtrl->Clear();
  wxColour fore = wxColour(0, 0, 0);
  wxColour back = wxColour(255, 0, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(ListDone);
  doneCtrl->MarkDirty();
}

#if defined(_WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteTextFontLoad(void *arg)
#else
void *DoExecuteTextFontLoad(void *arg)
#endif
{
//
// threaded function: processing a Text Font Import operation
//
  TextFontLoadParams *params = (TextFontLoadParams *) arg;
  const char *sql;
  int ret;
  sqlite3_stmt *stmt = NULL;
  int count = params->GetPathsCount();
  int i;
  clock_t clock_start;
  clock_t clock_end;
  double seconds;
  char elapsed[64];
  char ordinal[64];
  wxString report;
  wxString path;
  wxCommandEvent evt_start(wxEVT_COMMAND_BUTTON_CLICKED, ID_LOAD_FONT_START);
  wxCommandEvent evt_stop(wxEVT_COMMAND_BUTTON_CLICKED, ID_LOAD_FONT_STOP);
  wxCommandEvent evt_skip(wxEVT_COMMAND_BUTTON_CLICKED, ID_LOAD_FONT_SKIP);

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

  sql = "SELECT RL2_LoadFontFromFile(?)";
  ret =
    sqlite3_prepare_v2(params->GetMainFrame()->GetSqlite(), sql, strlen(sql),
                       &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      params->SetError();
      goto error;
    }

  for (i = 0; i < count; i++)
    {
      // loading and verifying each Text Font resource
      if (params->IsAbortPending() == true)
        {
          report = wxT("STOP .... aborted by the user !!!!");
          evt_start.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
          break;
        }
      path = params->GetPathByIndex(i);
      params->SetCurrentPath(path);
      report = wxT("Loading and Validating: ") + path;
      evt_start.SetString(report);
      params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
      clock_start = clock();
      wxString abstract;
      if (params->GetDlg()->RegisterTextFont(stmt, path.ToUTF8()) != true)
        {
          clock_end = clock();
          seconds =
            (double) (clock_end - clock_start) / (double) CLOCKS_PER_SEC;
          MyResultSetView::FormatElapsedTime(seconds, elapsed);
          sprintf(ordinal, "discarded %d/%d (not a valid Text Font): ",
                  i + 1, count);
          report =
            wxString::FromUTF8(ordinal) + path + wxT("    [") +
            wxString::FromUTF8(elapsed) + wxT("]\n");
          evt_skip.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_skip);
          continue;
        }
      clock_end = clock();
      seconds = (double) (clock_end - clock_start) / (double) CLOCKS_PER_SEC;
      MyResultSetView::FormatElapsedTime(seconds, elapsed);
      sprintf(ordinal, "done %d/%d: ", i + 1, count);
      report =
        wxString::FromUTF8(ordinal) + path + wxT("    [") +
        wxString::FromUTF8(elapsed) + wxT("]\n");
      evt_stop.SetString(report);
      params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
      params->Done();
    }
  sqlite3_finalize(stmt);
  goto end;

error:
  sqlite3_finalize(stmt);
  report = wxT("FAILED: ") + path;
  evt_stop.SetString(report);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
  params->SetError();
end:
  wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED,
                       ID_LOAD_FONT_THREAD_FINISHED);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(event);
#if defined(_WIN32) && !defined(__MINGW32__)
  return 0;
#else
  pthread_exit(NULL);
  return NULL;
#endif
}

void LoadTextFontDialog::OnThreadFinished(wxCommandEvent & WXUNUSED(event))
{
// resuming execution when the Import Text Font thread quits
  ::wxEndBusyCursor();
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  quitBtn->Enable(true);
  abortBtn->Enable(false);
  sqlite3_exec(Params.GetMainFrame()->GetSqlite(), "COMMIT", NULL, NULL, NULL);
  if (Params.GetError() == true)
    {
      char dummy[80];
      sprintf(dummy,
              "%d TextFonts have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nA fatal error occurred while loading:\n") +
                   Params.GetCurrentPath(), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
  } else if (Params.IsAbortPending() == true)
    {
      char dummy[80];
      sprintf(dummy,
              "%d TextFonts have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nStopped by an Abort user request"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  } else
    {
      char dummy[80];
      sprintf(dummy,
              "%d TextFonts have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
    }
}

bool LoadTextFontDialog::RegisterTextFont(sqlite3_stmt * stmt, const char *path)
{
//
// attempting to register the new Text Font
//
  int ret;
  int valid = 0;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_text(stmt, 1, path, strlen(path), SQLITE_STATIC);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        return false;
    }
  if (valid)
    return true;
  return false;
}

void LoadTextFontDialog::DoRunLoad()
{
//
// executing the Load Text Font(s) process in a separate Thread
//
#if defined(_WIN32) && !defined(__MINGW32__)
  HANDLE thread_handle;
  DWORD dwThreadId;
#else
  pthread_t thread_id;
#endif
  Params.Initialize(MainFrame, this, Paths);
#if defined(_WIN32) && !defined(__MINGW32__)
  thread_handle =
    CreateThread(NULL, 0, DoExecuteTextFontLoad, &Params, 0, &dwThreadId);
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
              pthread_create(&thread_id, &attr, DoExecuteTextFontLoad, &Params);
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
                  pthread_create(&thread_id, &attr, DoExecuteTextFontLoad,
                                 &Params);
                  pthread_detach(thread_id);
                }
            }
        }
    }
#endif
  if (!ok_prior)
    {
      // failure: using standard priority
      pthread_create(&thread_id, NULL, DoExecuteTextFontLoad, &Params);
      pthread_detach(thread_id);
    }
#endif
}

void LoadTextFontDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxButton *loadBtn = (wxButton *) FindWindow(wxID_OK);
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  loadBtn->Enable(false);
  quitBtn->Enable(false);
  abortBtn->Enable(true);
  ::wxBeginBusyCursor();
  char *errMsg = NULL;
  int ret = sqlite3_exec(MainFrame->GetSqlite(), "BEGIN", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  ret =
    sqlite3_exec(MainFrame->GetSqlite(), "SELECT CreateStylingTables()", NULL,
                 NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CreateStylingTables() error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  DoRunLoad();
}

bool UnregisterTextFontDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  List = MainFrame->FindTextFont();
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Unregistering a Text Font")) == false)
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

void UnregisterTextFontDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// a GRID to show results
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Registered Text Fonts"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SLD_SE_GRID, wxDefaultPosition, wxSize(640, 200));
  int count = 0;
  TextFont *pF = List->GetFirst();
  while (pF)
    {
      // counting how many lines are there
      count++;
      pF = pF->GetNext();
    }
  GridCtrl->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("FaceName"));
  GridCtrl->SetColLabelValue(1, wxT("Bold"));
  GridCtrl->SetColLabelValue(2, wxT("Italic"));
  GridCtrl->SetColLabelValue(3, wxT("Sample"));
  count = 0;
  pF = List->GetFirst();
  while (pF)
    {
      // feeding grid rows
      GridCtrl->SetCellValue(count, 0, pF->GetFacename());
      MyFontCellRenderer *renderer = new MyFontCellRenderer;
      if (pF->IsBold() == true)
        GridCtrl->SetCellValue(count, 1, wxT("YES"));
      if (pF->IsItalic() == true)
        GridCtrl->SetCellValue(count, 2, wxT("YES"));
      renderer->SetFontExample(pF->GetFontExample());
      GridCtrl->SetCellRenderer(count, 3, renderer);
      GridCtrl->SetCellBackgroundColour(count, 3, wxColour(255, 255, 255));
      count++;
      pF = pF->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Unregister"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & UnregisterTextFontDialog::OnOk);
}

bool UnregisterTextFontDialog::DoUnregisterTextFont(int font_id)
{
//
// attempting to unregister the Text Font
//
  int ret;
  int valid = 0;
  const char *sql;
  sqlite3_stmt *stmt = NULL;

  sql = "DELETE FROM SE_Fonts WHERE font_id = ?";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_int(stmt, 1, font_id);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        {
          valid = 1;
          break;
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (valid)
    return true;
  return false;
}

void UnregisterTextFontDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  int selCount = 0;
  int FontId;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          wxString cell_id = GridCtrl->GetCellValue(i, 0);
          long id;
          cell_id.ToLong(&id);
          FontId = id;
          selCount++;
        }
    }
  if (selCount < 1)
    {
      wxMessageBox(wxT
                   ("You must select a Text Font to be unregistered !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (selCount > 1)
    {
      wxString msg =
        wxT("You must select just a single Text Font to be unregistered !!!\n");
      msg += wxT("Multiple selection is not supported");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  ::wxBeginBusyCursor();
  char dummy[128];
  sprintf(dummy, "(FontID=%d)", FontId);
  if (DoUnregisterTextFont(FontId) == true)
    {
      ::wxEndBusyCursor();
      wxMessageBox(wxT("Text Font ") + wxString::FromUTF8(dummy) +
                   wxT(" successfully unregistered"),
                   wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  } else
    {
      ::wxEndBusyCursor();
      wxMessageBox(wxT
                   ("Some error occurred: unable to unregister Text Font ")
                   + wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
    }

  wxDialog::EndModal(wxID_OK);
}

TextFontList *MyFrame::FindTextFont()
{
// will retrieve all registered Text Fonts
  TextFontList *list = new TextFontList();
  sqlite3_stmt *stmt = NULL;
  const char *sql;

  sql = "SELECT font FROM SE_fonts";
  int ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      delete list;
      return NULL;
    }
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          const unsigned char *blob = NULL;
          int blob_sz = 0;
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              blob = (const unsigned char *) sqlite3_column_blob(stmt, 0);
              blob_sz = sqlite3_column_bytes(stmt, 0);
              list->Add(RL2PrivateData, blob, blob_sz);
            }
      } else
        {
          sqlite3_finalize(stmt);
          delete list;
          return NULL;
        }
    }
  sqlite3_finalize(stmt);
  return list;
}

void MyFrame::CheckTTFont(const char *facename, bool *bold, bool *italic)
{
// will check if a FontFace is Bold and/or Italica
  sqlite3_stmt *stmt = NULL;
  char *sql;

  *bold = false;
  *italic = false;

  sql =
    sqlite3_mprintf("SELECT font FROM SE_fonts WHERE font_facename = %Q",
                    facename);
  int ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          const unsigned char *blob = NULL;
          int blob_sz = 0;
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              blob = (const unsigned char *) sqlite3_column_blob(stmt, 0);
              blob_sz = sqlite3_column_bytes(stmt, 0);
              if (rl2_is_encoded_font_bold(blob, blob_sz) <= 0)
                *bold = false;
              else
                *bold = true;
              if (rl2_is_encoded_font_italic(blob, blob_sz) <= 0)
                *italic = false;
              else
                *italic = true;
            }
        }
    }
  sqlite3_finalize(stmt);
}

TextFont::TextFont(const void *priv_data, const unsigned char *blob,
                   int blob_sz)
{
// ctor
  Next = NULL;
  char *facename = rl2_get_encoded_font_facename(blob, blob_sz);
  Facename = wxString::FromUTF8(facename);
  if (facename != NULL)
    free(facename);
  if (rl2_is_encoded_font_bold(blob, blob_sz) <= 0)
    Bold = false;
  else
    Bold = true;
  if (rl2_is_encoded_font_italic(blob, blob_sz) <= 0)
    Italic = false;
  else
    Italic = true;

// creating the Font Preview
  rl2GraphicsContextPtr ctx = rl2_graph_create_context(priv_data, 600, 22);
  rl2_graph_set_brush(ctx, 255, 255, 255, 255);
  rl2_graph_draw_rectangle(ctx, -1, -1, 602, 24);
  const char *sample = "the quick brown fox jumps over the lazy dog";
  rl2GraphicsFontPtr font =
    rl2_graph_create_TrueType_font(priv_data, blob, blob_sz, 16.0);
  rl2_graph_set_font(ctx, font);
  rl2_graph_draw_text(ctx, sample, 5.0, 18.0, 0.0, 0.0, 0.0);
  rl2_graph_release_font(ctx);
  rl2_graph_destroy_font(font);
// creating a raster image
  unsigned char *rgb = rl2_graph_get_context_rgb_array(ctx);
  rl2_graph_destroy_context(ctx);
  rl2RasterPtr raster =
    rl2_create_raster(600, 22, RL2_SAMPLE_UINT8, RL2_PIXEL_RGB, 3, rgb,
                      600 * 22 * 3, NULL, NULL, 0, NULL);
  unsigned char *rgbaArray = NULL;
  int rgbaSize;
  unsigned int width;
  unsigned int height;
  wxImage *Image = NULL;
  if (raster)
    {
      if (rl2_get_raster_size(raster, &width, &height) == RL2_OK)
        {
          if (rl2_raster_data_to_RGBA(raster, &rgbaArray, &rgbaSize) != RL2_OK)
            rgbaArray = NULL;
        }
      rl2_destroy_raster(raster);
    }
  if (rgbaArray)
    {
      // creating the Image from RGB array
      unsigned int x;
      unsigned int y;
      Image = new wxImage(width, height);
      unsigned char *p = rgbaArray;
      Image->SetAlpha();
      for (y = 0; y < height; y++)
        {
          for (x = 0; x < width; x++)
            {
              unsigned char r = *p++;
              unsigned char g = *p++;
              unsigned char b = *p++;
              unsigned char alpha = *p++;
              Image->SetRGB(x, y, r, g, b);
              Image->SetAlpha(x, y, alpha);
            }
        }
      free(rgbaArray);
    }
  if (Image != NULL)
    FontExample = Image;
  else
    FontExample = new wxImage(180, 16);
}

TextFontList::~TextFontList()
{
// dtor
  TextFont *pF;
  TextFont *pFn;
  pF = First;
  while (pF != NULL)
    {
      pFn = pF->GetNext();
      delete pF;
      pF = pFn;
    }
}

void TextFontList::Add(const void *priv_data, const unsigned char *blob,
                       int blob_sz)
{
// inserting a new Text Font into the list
  TextFont *pF = new TextFont(priv_data, blob, blob_sz);
  if (First == NULL)
    First = pF;
  if (Last != NULL)
    Last->SetNext(pF);
  Last = pF;
}

void TextFontList::FindByIndex(int idx, wxString & face_name, int *style,
                               int *weight)
{
// searching an entry by its position
  TextFont *pF;
  int count = 0;
  pF = First;
  while (pF != NULL)
    {
      if (idx == count)
        {
          face_name = pF->GetFacename();
          if (pF->IsBold() == true)
            *weight = RL2_FONTWEIGHT_BOLD;
          else
            *weight = RL2_FONTWEIGHT_NORMAL;
          if (pF->IsItalic() == true)
            *style = RL2_FONTSTYLE_ITALIC;
          else
            *style = RL2_FONTSTYLE_NORMAL;
          return;
        }
      count++;
      pF = pF->GetNext();
    }
  face_name = wxT("");
  *style = RL2_FONTSTYLE_NORMAL;
  *weight = RL2_FONTWEIGHT_NORMAL;
}

int TextFontList::FindByFaceName(wxString & face_name)
{
// searching an entry by Facename
  TextFont *pF;
  int count = 0;
  pF = First;
  while (pF != NULL)
    {
      if (face_name.CmpNoCase(pF->GetFacename()) == 0)
        return count;
      count++;
      pF = pF->GetNext();
    }
  return -1;
}

void MyFontCellRenderer::Draw(wxGrid & grid, wxGridCellAttr & attr,
                              wxDC & dc, const wxRect & rect, int row,
                              int col, bool isSelected)
{
// drawing a Font Example cell
  if (row == col && isSelected)
    col = row;                  // silencing stupid compiler warnings about unused args

  wxBitmap bmp = wxBitmap(*FontExample);
  wxColour color = attr.GetBackgroundColour();
  if (grid.IsEnabled() == false)
    {
      color = wxSystemSettings::GetColour(wxSYS_COLOUR_MENU);
      wxImage img = FontExample->ConvertToGreyscale();
      bmp = wxBitmap(img);
    }
  dc.SetBrush(wxBrush(color));
  dc.SetPen(*wxTRANSPARENT_PEN);
  dc.DrawRectangle(rect);
  int x = (rect.GetWidth() - bmp.GetWidth()) / 2;
  int y = (rect.GetHeight() - bmp.GetHeight()) / 2;
  dc.DrawBitmap(bmp, rect.x + x, rect.y + y, true);
}

void MyFontCellRenderer::SetFontExample(const void *priv_data,
                                        const char *facename)
{
// creating the Font Preview
  if (facename == NULL)
    {
      FontExample = new wxImage(180, 16);
      return;
    }
  rl2GraphicsContextPtr ctx = rl2_graph_create_context(priv_data, 600, 22);
  rl2_graph_set_brush(ctx, 255, 255, 255, 255);
  rl2_graph_draw_rectangle(ctx, -1, -1, 602, 24);
  const char *sample = "the quick brown fox jumps over the lazy dog";
  rl2GraphicsFontPtr font =
    rl2_graph_create_toy_font(facename, 16.0, RL2_FONTSTYLE_NORMAL,
                              RL2_FONTWEIGHT_NORMAL);
  rl2_graph_set_font(ctx, font);
  rl2_graph_draw_text(ctx, sample, 5.0, 18.0, 0.0, 0.0, 0.0);
  rl2_graph_release_font(ctx);
  rl2_graph_destroy_font(font);
// creating a raster image
  unsigned char *rgb = rl2_graph_get_context_rgb_array(ctx);
  rl2_graph_destroy_context(ctx);
  rl2RasterPtr raster =
    rl2_create_raster(600, 22, RL2_SAMPLE_UINT8, RL2_PIXEL_RGB, 3, rgb,
                      600 * 22 * 3, NULL, NULL, 0, NULL);
  unsigned char *rgbaArray = NULL;
  int rgbaSize;
  unsigned int width;
  unsigned int height;
  wxImage *Image = NULL;
  if (raster)
    {
      if (rl2_get_raster_size(raster, &width, &height) == RL2_OK)
        {
          if (rl2_raster_data_to_RGBA(raster, &rgbaArray, &rgbaSize) != RL2_OK)
            rgbaArray = NULL;
        }
      rl2_destroy_raster(raster);
    }
  if (rgbaArray)
    {
      // creating the Image from RGB array
      unsigned int x;
      unsigned int y;
      Image = new wxImage(width, height);
      unsigned char *p = rgbaArray;
      Image->SetAlpha();
      for (y = 0; y < height; y++)
        {
          for (x = 0; x < width; x++)
            {
              unsigned char r = *p++;
              unsigned char g = *p++;
              unsigned char b = *p++;
              unsigned char alpha = *p++;
              Image->SetRGB(x, y, r, g, b);
              Image->SetAlpha(x, y, alpha);
            }
        }
      free(rgbaArray);
    }
  if (Image != NULL)
    FontExample = Image;
  else
    FontExample = new wxImage(180, 16);
}
