/*
/ WmsDialog.cpp
/ various dialog classes supporting WMS datasources
/
/ version 2.0, 2016 December 13
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

#include <float.h>

#include "Classdef.h"

#include "wx/html/htmlwin.h"
#include "wx/spinctrl.h"
#include "wx/imaglist.h"
#include "wx/colordlg.h"

//
// ICONs in XPM format [universally portable]
//
#include "icons/wms.xpm"
#include "icons/group.xpm"
#include "icons/layer.xpm"

bool WmsDialog::Create(MyFrame * parent, wxString & proxy)
{
//
// creating the dialog
//
  MainFrame = parent;
  SwapXY = 0;
  HttpProxy = proxy;
  if (HttpProxy.Len() == 0)
    ProxyEnabled = false;
  else
    ProxyEnabled = true;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Register a WMS datasource")) ==
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

void WmsDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// HTTP Proxy
  wxStaticBox *proxyBox = new wxStaticBox(this, wxID_STATIC,
                                          wxT("HTTP Proxy"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *proxyBoxSizer = new wxStaticBoxSizer(proxyBox, wxHORIZONTAL);
  boxSizer->Add(proxyBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxCheckBox *enableProxyCtrl = new wxCheckBox(this, ID_WMS_ENABLE_PROXY,
                                               wxT("Enable"),
                                               wxDefaultPosition,
                                               wxDefaultSize);
  enableProxyCtrl->SetValue(ProxyEnabled);
  proxyBoxSizer->Add(enableProxyCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *proxyCtrl = new wxTextCtrl(this, ID_WMS_PROXY, HttpProxy,
                                         wxDefaultPosition, wxSize(600, 22));
  proxyBoxSizer->Add(proxyCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  proxyCtrl->Enable(ProxyEnabled);
// URL group box
  wxStaticBox *urlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("WMS URL - GetCapabilities"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *urlBoxSizer = new wxStaticBoxSizer(urlBox, wxVERTICAL);
  boxSizer->Add(urlBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// First row: GetCapabilities URL
  wxBoxSizer *urlSizer = new wxBoxSizer(wxVERTICAL);
  urlBoxSizer->Add(urlSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *urlCtrl = new wxTextCtrl(this, ID_WMS_URL, wxT("http://"),
                                       wxDefaultPosition, wxSize(680, 22));
  urlSizer->Add(urlCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// command buttons
  wxBoxSizer *url2Sizer = new wxBoxSizer(wxHORIZONTAL);
  urlSizer->Add(url2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *catBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("WMS Catalog"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *catBoxSizer = new wxStaticBoxSizer(catBox, wxVERTICAL);
  url2Sizer->Add(catBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  catBoxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *query = new wxButton(this, ID_WMS_CATALOG, wxT("&Load"));
  okCancelBox->Add(query, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxButton *reset = new wxButton(this, ID_WMS_RESET, wxT("&Reset"));
  reset->Enable(false);
  okCancelBox->Add(reset, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxButton *quit = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(quit, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  url2Sizer->AddSpacer(60);
  wxStaticBox *srvBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Search WMS Server"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *srvBoxSizer = new wxStaticBoxSizer(srvBox, wxVERTICAL);
  url2Sizer->Add(srvBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *serverBox = new wxBoxSizer(wxHORIZONTAL);
  srvBoxSizer->Add(serverBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *server =
    new wxButton(this, ID_WMS_SERVER, wxT("&Registered Servers"));
  serverBox->Add(server, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// the Catalog tree view
  wxBoxSizer *treeSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(treeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  WmsTree = new WmsCatalogTree(this, wxSize(330, 250));
  treeSizer->Add(WmsTree, 0, wxALIGN_LEFT | wxALL, 5);
// Title / Abstract panel
  wxBoxSizer *title0BoxSizer = new wxBoxSizer(wxVERTICAL);
  treeSizer->Add(title0BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *titleBox = new wxStaticBox(this, wxID_ANY,
                                          wxT("Layer Title"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *titleBoxSizer = new wxStaticBoxSizer(titleBox, wxHORIZONTAL);
  title0BoxSizer->Add(titleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxHtmlWindow *titleCtrl = new wxHtmlWindow(this, ID_WMS_TITLE,
                                             wxDefaultPosition, wxSize(350,
                                                                       75));
  titleBoxSizer->Add(titleCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *abstractBox = new wxStaticBox(this, wxID_ANY,
                                             wxT("Layer Abstract"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  wxBoxSizer *abstractBoxSizer =
    new wxStaticBoxSizer(abstractBox, wxHORIZONTAL);
  title0BoxSizer->Add(abstractBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxHtmlWindow *abstractCtrl = new wxHtmlWindow(this, ID_WMS_ABSTRACT,
                                                wxDefaultPosition, wxSize(350,
                                                                          120));
  abstractBoxSizer->Add(abstractCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// selected Layer group box
  wxBoxSizer *kkSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(kkSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *lyrBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Selected WMS Layer"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *lyrBox0Sizer = new wxStaticBoxSizer(lyrBox, wxHORIZONTAL);
  kkSizer->Add(lyrBox0Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// Second row: Layer name
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  lyrBox0Sizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer Name"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_WMS_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(570, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *swapBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Swap Axes"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *swapBoxSizer = new wxStaticBoxSizer(swapBox, wxHORIZONTAL);
  nameSizer->Add(swapBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *swapCtrl = new wxCheckBox(this, ID_WMS_SWAP,
                                        wxT("Swap Y,X"),
                                        wxDefaultPosition, wxDefaultSize);
  swapCtrl->SetValue(false);
  swapCtrl->Enable(false);
  swapBoxSizer->Add(swapCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// First row: WMS options
  wxBoxSizer *wmsSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(wmsSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString ver[4];
  ver[0] = wxT("WMS &1.0.0");
  ver[1] = wxT("WMS &1.1.0");
  ver[2] = wxT("WMS &1.1.1");
  ver[3] = wxT("WMS &1.3.0");
  wxRadioBox *versionBox = new wxRadioBox(this, ID_WMS_VERSION,
                                          wxT("WMS &Version"),
                                          wxDefaultPosition,
                                          wxDefaultSize, 4,
                                          ver, 4,
                                          wxRA_SPECIFY_ROWS);
  versionBox->Enable(false);
  versionBox->SetSelection(3);
  wmsSizer->Add(versionBox, 0, wxALIGN_TOP | wxALL, 5);
  wxBoxSizer *xxSizer = new wxBoxSizer(wxVERTICAL);
  wmsSizer->Add(xxSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *crsBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Reference System"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *crsBoxSizer = new wxStaticBoxSizer(crsBox, wxVERTICAL);
  xxSizer->Add(crsBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *crsList =
    new wxComboBox(this, ID_WMS_CRS, wxT(""), wxDefaultPosition,
                   wxSize(150, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_SORT | wxCB_READONLY);
  crsList->Enable(false);
  crsBoxSizer->Add(crsList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticBox *stlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *stlBoxSizer = new wxStaticBoxSizer(stlBox, wxVERTICAL);
  xxSizer->Add(stlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *stlList =
    new wxComboBox(this, ID_WMS_STYLE, wxT(""), wxDefaultPosition,
                   wxSize(150, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_SORT | wxCB_READONLY);
  stlList->Enable(false);
  stlBoxSizer->Add(stlList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *yySizer = new wxBoxSizer(wxVERTICAL);
  wmsSizer->Add(yySizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *fmtBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Image Format"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *fmtBoxSizer = new wxStaticBoxSizer(fmtBox, wxVERTICAL);
  yySizer->Add(fmtBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *fmtList =
    new wxComboBox(this, ID_WMS_FORMAT, wxT(""), wxDefaultPosition,
                   wxSize(150, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_SORT | wxCB_READONLY);
  fmtList->Enable(false);
  fmtBoxSizer->Add(fmtList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxString trans[2];
  trans[0] = wxT("yes");
  trans[1] = wxT("opaque");
  wxRadioBox *transBox = new wxRadioBox(this, ID_WMS_TRANSPARENT,
                                        wxT("Transparent Layer"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 2,
                                        trans, 2,
                                        wxRA_SPECIFY_COLS);
  transBox->Enable(false);
  transBox->SetSelection(1);
  yySizer->Add(transBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxStaticBox *tileBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Tile size"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *tileBoxSizer = new wxStaticBoxSizer(tileBox, wxVERTICAL);
  wmsSizer->Add(tileBoxSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxCheckBox *tiledCtrl = new wxCheckBox(this, ID_WMS_TILED,
                                         wxT("enable Tiles"),
                                         wxDefaultPosition, wxDefaultSize);
  tiledCtrl->SetValue(false);
  tiledCtrl->Enable(false);
  tileBoxSizer->Add(tiledCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *tile1BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  tileBoxSizer->Add(tile1BoxSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *wLabel = new wxStaticText(this, wxID_ANY, wxT("&Width:"),
                                          wxDefaultPosition, wxDefaultSize,
                                          wxALIGN_RIGHT);
  tile1BoxSizer->Add(wLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxSpinCtrl *wCtrl = new wxSpinCtrl(this, ID_WMS_WIDTH, wxEmptyString,
                                     wxDefaultPosition, wxSize(80, 20),
                                     wxSP_ARROW_KEYS,
                                     0, 0, 0);
  wCtrl->Enable(false);
  wCtrl->SetValue(wxT(""));
  tile1BoxSizer->Add(wCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *tile2BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  tileBoxSizer->Add(tile2BoxSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *hLabel = new wxStaticText(this, wxID_ANY, wxT("&Height:"),
                                          wxDefaultPosition, wxDefaultSize,
                                          wxALIGN_RIGHT);
  tile2BoxSizer->Add(hLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxSpinCtrl *hCtrl = new wxSpinCtrl(this, ID_WMS_HEIGHT, wxEmptyString,
                                     wxDefaultPosition, wxSize(80, 20),
                                     wxSP_ARROW_KEYS,
                                     0, 0, 0);
  tile2BoxSizer->Add(hCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  hCtrl->Enable(false);
  hCtrl->SetValue(wxT(""));
  wxButton *wms_ok = new wxButton(this, ID_WMS_OK, wxT("&Register"));
  wms_ok->Enable(false);
  wmsSizer->Add(wms_ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handlers
  Connect(ID_WMS_ENABLE_PROXY, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & WmsDialog::OnProxy);
  Connect(ID_WMS_CRS, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & WmsDialog::OnCrsChanged);
  Connect(ID_WMS_VERSION, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & WmsDialog::OnVersionChanged);
  Connect(ID_WMS_SWAP, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & WmsDialog::OnSwapXYChanged);
  Connect(ID_WMS_TILED, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & WmsDialog::OnTiledChanged);
  Connect(ID_WMS_CATALOG, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsDialog::OnCatalog);
  Connect(ID_WMS_SERVER, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsDialog::OnWmsServer);
  Connect(ID_WMS_RESET, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsDialog::OnReset);
  Connect(ID_WMS_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsDialog::OnWmsOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsDialog::OnQuit);
}

void WmsDialog::OnProxy(wxCommandEvent & WXUNUSED(event))
{
//
// enabling/disabling HTTP Proxy
//
  wxTextCtrl *proxy = (wxTextCtrl *) FindWindow(ID_WMS_PROXY);
  if (ProxyEnabled == false)
    ProxyEnabled = true;
  else
    ProxyEnabled = false;
  proxy->Enable(ProxyEnabled);
  if (ProxyEnabled == false)
    {
      HttpProxy = wxT("");
      proxy->SetValue(wxT(""));
    }
}

void WmsDialog::OnCrsChanged(wxCommandEvent & WXUNUSED(event))
{
//
// CRS changed: updating SWAP XY
//
  UpdateSwapXY();
}

void WmsDialog::OnVersionChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Version changed: updating SWAP XY
//
  UpdateSwapXY();
}

void WmsDialog::UpdateSwapXY()
{
//
// updating SWAP XY
//
  wxCheckBox *swapBox = (wxCheckBox *) FindWindow(ID_WMS_SWAP);
  wxRadioBox *versionBox = (wxRadioBox *) FindWindow(ID_WMS_VERSION);
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  if (versionBox->GetSelection() != 3)
    {
      swapBox->SetValue(false);
      SwapXY = 0;
      return;
    };
  wxString value = comboCtrl->GetValue();
  char *crs = new char[value.Len() + 1];
  strcpy(crs, value.ToUTF8());
  if (MainFrame->HasFlippedAxes(crs) == true)
    {
      swapBox->SetValue(true);
      SwapXY = 1;
  } else
    {
      swapBox->SetValue(false);
      SwapXY = 0;
    }
  delete[]crs;
}

void WmsDialog::SelectLayer()
{
//
// setting the currently selected WMS Layer [NULL type]
//
  CurrentLayer = NULL;
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_WMS_NAME);
  wxHtmlWindow *titleCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_TITLE);
  wxHtmlWindow *abstractCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_ABSTRACT);
  nameCtrl->SetValue(wxT(""));
  nameCtrl->Enable(false);
  titleCtrl->SetPage(wxT("<html><body></body></html>"));
  titleCtrl->Enable(false);
  abstractCtrl->SetPage(wxT("<html><body></body></html>"));
  abstractCtrl->Enable(false);
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  comboCtrl->Clear();
  comboCtrl->Enable(false);
  comboCtrl = (wxComboBox *) FindWindow(ID_WMS_FORMAT);
  comboCtrl->Clear();
  comboCtrl->Enable(false);
  comboCtrl = (wxComboBox *) FindWindow(ID_WMS_STYLE);
  comboCtrl->Clear();
  comboCtrl->Enable(false);
  wxRadioBox *trans = (wxRadioBox *) FindWindow(ID_WMS_TRANSPARENT);
  trans->Enable(false);
  wxCheckBox *tiled = (wxCheckBox *) FindWindow(ID_WMS_TILED);
  tiled->Enable(false);
  wxRadioBox *versionBox = (wxRadioBox *) FindWindow(ID_WMS_VERSION);
  versionBox->Enable(false);
  wxCheckBox *swapBox = (wxCheckBox *) FindWindow(ID_WMS_SWAP);
  swapBox->Enable(false);
  wxButton *btn = (wxButton *) FindWindow(ID_WMS_OK);
  btn->Enable(false);
}

void WmsDialog::SelectLayer(rl2WmsLayerPtr layer)
{
//
// setting the currently selected WMS Layer [ordinary WMS layer]
//
  bool enabledLayer = true;
  if (get_wms_layer_name(layer) == NULL)
    enabledLayer = false;
  CurrentLayer = layer;
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_WMS_NAME);
  wxHtmlWindow *titleCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_TITLE);
  wxHtmlWindow *abstractCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_ABSTRACT);
  wxString lyr_name;
  const char *x_name = get_wms_layer_name(layer);
  lyr_name = wxString::FromUTF8(x_name);
  nameCtrl->SetValue(lyr_name);
  x_name = get_wms_layer_title(layer);
  lyr_name = wxString::FromUTF8(x_name);
  lyr_name.Replace(wxT("\n"), wxT("<br>"));
  titleCtrl->SetPage(wxT("<html><body>") + lyr_name + wxT("</body></html>"));
  x_name = get_wms_layer_abstract(layer);
  lyr_name = wxString::FromUTF8(x_name);
  lyr_name.Replace(wxT("\n"), wxT("<br>"));
  abstractCtrl->SetPage(wxT("<html><body>") + lyr_name + wxT("</body></html>"));
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  comboCtrl->Clear();
  int max = get_wms_layer_crs_count(layer);
  if (max > 0)
    {
      for (int s = 0; s < max; s++)
        {
          wxString str = wxString::FromUTF8(get_wms_layer_crs(layer, s));
          if (comboCtrl->FindString(str) == wxNOT_FOUND)
            comboCtrl->Append(str);
        }
      wxString str = wxString::FromUTF8(get_wms_layer_crs(layer, 0));
      int sel = comboCtrl->FindString(str);
      if (sel == wxNOT_FOUND)
        comboCtrl->SetSelection(0);
      else
        comboCtrl->SetSelection(sel);
      comboCtrl->Enable(enabledLayer);
    }
  comboCtrl = (wxComboBox *) FindWindow(ID_WMS_FORMAT);
  comboCtrl->Clear();
  max = get_wms_format_count(Catalog, 1);
  if (max > 0)
    {
      for (int s = 0; s < max; s++)
        {
          wxString str = wxString::FromUTF8(get_wms_format(Catalog, s, 1));
          if (comboCtrl->FindString(str) == wxNOT_FOUND)
            comboCtrl->Append(str);
        }
      wxString str = wxString::FromUTF8(get_wms_format(Catalog, 0, 1));
      int sel = comboCtrl->FindString(str);
      if (sel == wxNOT_FOUND)
        comboCtrl->SetSelection(0);
      else
        comboCtrl->SetSelection(sel);
      comboCtrl->Enable(enabledLayer);
    }
  comboCtrl = (wxComboBox *) FindWindow(ID_WMS_STYLE);
  comboCtrl->Clear();
  max = get_wms_layer_style_count(layer);
  if (max > 0)
    {
      for (int s = 0; s < max; s++)
        {
          wxString str = wxString::FromUTF8(get_wms_layer_style_name(layer, s));
          if (comboCtrl->FindString(str) == wxNOT_FOUND)
            comboCtrl->Append(str);
        }
      wxString str = wxString::FromUTF8(get_wms_layer_style_name(layer, 0));
      int sel = comboCtrl->FindString(str);
      if (sel == wxNOT_FOUND)
        comboCtrl->SetSelection(0);
      else
        comboCtrl->SetSelection(sel);
      comboCtrl->Enable(enabledLayer);
    }
  wxRadioBox *trans = (wxRadioBox *) FindWindow(ID_WMS_TRANSPARENT);
  if (is_wms_layer_opaque(layer) <= 0)
    {
      trans->SetSelection(0);
      trans->Enable(enabledLayer);
  } else
    {
      trans->SetSelection(1);
      trans->Enable(false);
    }
  wxCheckBox *tiled = (wxCheckBox *) FindWindow(ID_WMS_TILED);
  tiled->SetValue(false);
  tiled->Enable(enabledLayer);
  wxRadioBox *versionBox = (wxRadioBox *) FindWindow(ID_WMS_VERSION);
  versionBox->Enable(enabledLayer);
  wxCheckBox *swapBox = (wxCheckBox *) FindWindow(ID_WMS_SWAP);
  swapBox->Enable(enabledLayer);
  wxButton *btn = (wxButton *) FindWindow(ID_WMS_OK);
  btn->Enable(enabledLayer);
  const char *version = get_wms_version(Catalog);
  if (version == NULL)
    {
      versionBox->Enable(0, enabledLayer);
      versionBox->Enable(1, false);
      versionBox->Enable(2, false);
      versionBox->Enable(3, false);
      versionBox->SetSelection(0);
  } else
    {
      if (strcmp(version, "1.3.0") == 0)
        {
          versionBox->Enable(0, enabledLayer);
          versionBox->Enable(1, enabledLayer);
          versionBox->Enable(2, enabledLayer);
          versionBox->Enable(3, enabledLayer);
          versionBox->SetSelection(3);
      } else if (strcmp(version, "1.1.1") == 0)
        {
          versionBox->Enable(0, enabledLayer);
          versionBox->Enable(1, enabledLayer);
          versionBox->Enable(2, enabledLayer);
          versionBox->Enable(3, false);
          versionBox->SetSelection(2);
      } else if (strcmp(version, "1.1.0") == 0)
        {
          versionBox->Enable(0, enabledLayer);
          versionBox->Enable(1, enabledLayer);
          versionBox->Enable(2, false);
          versionBox->Enable(3, false);
          versionBox->SetSelection(1);
      } else
        {
          versionBox->Enable(0, enabledLayer);
          versionBox->Enable(1, false);
          versionBox->Enable(2, false);
          versionBox->Enable(3, false);
          versionBox->SetSelection(0);
        }
    }
  UpdateSwapXY();
}

void WmsDialog::OnSwapXYChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Swap XY selection changed
//
  if (SwapXY == 0)
    SwapXY = 1;
  else
    SwapXY = 0;
}

void WmsDialog::OnTiledChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Monolithic / Tiled selection changed
//
  wxCheckBox *tiledCtrl = (wxCheckBox *) FindWindow(ID_WMS_TILED);
  wxSpinCtrl *wCtrl = (wxSpinCtrl *) FindWindow(ID_WMS_WIDTH);
  wxSpinCtrl *hCtrl = (wxSpinCtrl *) FindWindow(ID_WMS_HEIGHT);
  if (tiledCtrl->GetValue() == false)
    {
      wCtrl->SetRange(0, 0);
      wCtrl->SetValue(wxT(""));
      wCtrl->Enable(false);
      hCtrl->SetRange(0, 0);
      hCtrl->SetValue(wxT(""));
      hCtrl->Enable(false);
  } else
    {
      wCtrl->SetRange(256, MaxWidth);
      wCtrl->SetValue(wxT("512"));
      wCtrl->Enable(true);
      hCtrl->SetRange(256, MaxHeight);
      hCtrl->SetValue(wxT("512"));
      hCtrl->Enable(true);
    }
}

void WmsDialog::OnCatalog(wxCommandEvent & WXUNUSED(event))
{
//
// event handler
//
  ::wxBeginBusyCursor();
  PrepareCatalog();
  ::wxEndBusyCursor();
}

void WmsDialog::PrepareCatalog()
{
//
// attempting to create a WMS Catalog from GetCapabilities
//
  wxTextCtrl *proxyCtrl = (wxTextCtrl *) FindWindow(ID_WMS_PROXY);
  wxTextCtrl *urlCtrl = (wxTextCtrl *) FindWindow(ID_WMS_URL);
  wxButton *catalogBtn = (wxButton *) FindWindow(ID_WMS_CATALOG);
  wxButton *resetBtn = (wxButton *) FindWindow(ID_WMS_RESET);
  if (ProxyEnabled == true)
    {
      HttpProxy = proxyCtrl->GetValue();
      if (HttpProxy.Len() == 0)
        {
          wxMessageBox(wxT("You must specify some HTTP Proxy URL !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
    }
  wxString url = urlCtrl->GetValue();
  if (url.Len() < 1)
    {
      wxMessageBox(wxT("You must specify some WMS GetCapabilities URL !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  char xurl[1024 * 64];
  char *err_msg = NULL;
  strcpy(xurl, url.ToUTF8());
  char *normalized_url = NormalizeUrl(xurl);
  if (normalized_url != NULL)
    {
      strcpy(xurl, normalized_url);
      DestroyNormalizedUrl(normalized_url);
      url = wxString::FromUTF8(xurl);
      urlCtrl->SetValue(url);
    }
  char *proxy = NULL;
  if (ProxyEnabled == true && HttpProxy.Len() > 0)
    {
      proxy = new char[HttpProxy.Len() + 1];
      strcpy(proxy, HttpProxy.ToUTF8());
    }
  Catalog = create_wms_catalog(NULL, xurl, proxy);
  if (proxy != NULL)
    delete[]proxy;
  if (Catalog == NULL)
    {
      wxString msg = wxString::FromUTF8(err_msg);
      wxMessageBox(wxT("unable to get a WMS Catalog\n\n") + msg,
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  } else
    {
      MaxWidth = get_wms_max_width(Catalog);
      MaxHeight = get_wms_max_height(Catalog);
      if (MaxWidth <= 0)
        MaxWidth = 2048;
      if (MaxHeight <= 0)
        MaxHeight = 2048;

      int nLayers = get_wms_catalog_count(Catalog);
      WmsTree->Show(false);
      WmsTree->FlushAll();
      for (int i = 0; i < nLayers; i++)
        {
          // populating the WMS Catalog [ordinary layers]
          rl2WmsLayerPtr layer = get_wms_catalog_layer(Catalog, i);
          const char *x_name = get_wms_layer_title(layer);
          WmsTree->AddLayer(layer, x_name);
        }

      WmsTree->ExpandAll();
      WmsTree->Show(true);
      WmsTree->RootAutoSelect();
      urlCtrl->Enable(false);
      catalogBtn->Enable(false);
      resetBtn->Enable(true);
    }
  if (err_msg != NULL)
    free(err_msg);
}

void WmsDialog::OnWmsServer(wxCommandEvent & WXUNUSED(event))
{
//
// searching a registered WMS server
//
  WmsServersDialog dlg;
  dlg.Create(MainFrame);
  int ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      wxTextCtrl *urlCtrl = (wxTextCtrl *) FindWindow(ID_WMS_URL);
      urlCtrl->SetValue(dlg.GetURL());
    }
}

void WmsDialog::OnReset(wxCommandEvent & WXUNUSED(event))
{
//
// resetting to initial empty state
//
  CurrentLayer = NULL;
  wxTextCtrl *urlCtrl = (wxTextCtrl *) FindWindow(ID_WMS_URL);
  wxButton *catalogBtn = (wxButton *) FindWindow(ID_WMS_CATALOG);
  wxButton *resetBtn = (wxButton *) FindWindow(ID_WMS_RESET);
  if (Catalog != NULL)
    destroy_wms_catalog(Catalog);
  Catalog = NULL;
  urlCtrl->Enable(true);
  catalogBtn->Enable(true);
  resetBtn->Enable(false);
  WmsTree->FlushAll();
  wxHtmlWindow *titleCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_TITLE);
  titleCtrl->SetPage(wxT(""));
  wxHtmlWindow *abstractCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_ABSTRACT);
  abstractCtrl->SetPage(wxT(""));
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_WMS_NAME);
  nameCtrl->SetValue(wxT(""));
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  comboCtrl->Clear();
  comboCtrl->SetSelection(wxNOT_FOUND);
  comboCtrl->Enable(false);
  comboCtrl = (wxComboBox *) FindWindow(ID_WMS_FORMAT);
  comboCtrl->Clear();
  comboCtrl->SetSelection(wxNOT_FOUND);
  comboCtrl->Enable(false);
  comboCtrl = (wxComboBox *) FindWindow(ID_WMS_STYLE);
  comboCtrl->Clear();
  comboCtrl->SetSelection(wxNOT_FOUND);
  comboCtrl->Enable(false);
  wxRadioBox *trans = (wxRadioBox *) FindWindow(ID_WMS_TRANSPARENT);
  trans->SetSelection(1);
  trans->Enable(false);
  wxCheckBox *tiled = (wxCheckBox *) FindWindow(ID_WMS_TILED);
  tiled->SetValue(false);
  tiled->Enable(false);
  wxRadioBox *versionBox = (wxRadioBox *) FindWindow(ID_WMS_VERSION);
  versionBox->Enable(false);
  wxCheckBox *swapBox = (wxCheckBox *) FindWindow(ID_WMS_SWAP);
  swapBox->Enable(false);
  wxButton *load = (wxButton *) FindWindow(ID_WMS_OK);
  load->Enable(false);
}

int WmsDialog::IsTiled()
{
// return the Tiled mode
  wxCheckBox *tiled = (wxCheckBox *) FindWindow(ID_WMS_TILED);
  if (tiled->GetValue() == false)
    return 0;
  return 1;
}

int WmsDialog::GetTileWidth()
{
// return the TileWidth
  wxSpinCtrl *wCtrl = (wxSpinCtrl *) FindWindow(ID_WMS_WIDTH);
  return wCtrl->GetValue();
}

int WmsDialog::GetTileHeight()
{
// return the TileHeight
  wxSpinCtrl *hCtrl = (wxSpinCtrl *) FindWindow(ID_WMS_HEIGHT);
  return hCtrl->GetValue();
}

int WmsDialog::IsOpaque()
{
// return the Opaque mode
  wxRadioBox *trans = (wxRadioBox *) FindWindow(ID_WMS_TRANSPARENT);
  if (trans->GetSelection() == 0)
    return 0;
  return 1;
}

const char *WmsDialog::GetVersion()
{
// return the Version string
  wxRadioBox *versionBox = (wxRadioBox *) FindWindow(ID_WMS_VERSION);
  if (Version != NULL)
    delete[]Version;
  Version = new char[6];
  switch (versionBox->GetSelection())
    {
      case 0:
        strcpy(Version, "1.0.0");
        break;
      case 1:
        strcpy(Version, "1.1.0");
        break;
      case 2:
        strcpy(Version, "1.1.1");
        break;
      default:
        strcpy(Version, "1.3.0");
        break;
    };
  return Version;
}

const char *WmsDialog::GetStyleName()
{
// return the Style Name string
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_STYLE);
  wxString value = comboCtrl->GetValue();
  if (value.Len() == 0)
    return NULL;
  int cnt = get_wms_layer_style_count(CurrentLayer);
  for (int i = 0; i < cnt; i++)
    {
      const char *str = get_wms_layer_style_name(CurrentLayer, i);
      wxString style = wxString::FromUTF8(str);
      if (style == value)
        return str;
    }
  return NULL;
}

const char *WmsDialog::GetStyleTitle()
{
// return the Style Title string
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_STYLE);
  wxString value = comboCtrl->GetValue();
  if (value.Len() == 0)
    return NULL;
  int cnt = get_wms_layer_style_count(CurrentLayer);
  for (int i = 0; i < cnt; i++)
    {
      const char *stl = get_wms_layer_style_name(CurrentLayer, i);
      const char *str = get_wms_layer_style_title(CurrentLayer, i);
      wxString style = wxString::FromUTF8(stl);
      if (style == value)
        return str;
    }
  return NULL;
}

const char *WmsDialog::GetStyleAbstract()
{
// return the Style Abstract string
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_STYLE);
  wxString value = comboCtrl->GetValue();
  if (value.Len() == 0)
    return NULL;
  int cnt = get_wms_layer_style_count(CurrentLayer);
  for (int i = 0; i < cnt; i++)
    {
      const char *stl = get_wms_layer_style_name(CurrentLayer, i);
      const char *str = get_wms_layer_style_abstract(CurrentLayer, i);
      wxString style = wxString::FromUTF8(stl);
      if (style == value)
        return str;
    }
  return NULL;
}

const char *WmsDialog::GetFormat()
{
// return the Format string
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_FORMAT);
  wxString value = comboCtrl->GetValue();
  if (value.Len() == 0)
    return NULL;
  int cnt = get_wms_format_count(Catalog, 1);
  for (int i = 0; i < cnt; i++)
    {
      const char *str = get_wms_format(Catalog, i, 1);
      wxString fmt = wxString::FromUTF8(str);
      if (fmt == value)
        return str;
    }
  return NULL;
}

const char *WmsDialog::GetCRS()
{
// return the CRS string
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  wxString value = comboCtrl->GetValue();
  if (value.Len() == 0)
    return NULL;
  int cnt = get_wms_layer_crs_count(CurrentLayer);
  for (int i = 0; i < cnt; i++)
    {
      const char *str = get_wms_layer_crs(CurrentLayer, i);
      wxString crs = wxString::FromUTF8(str);
      if (crs == value)
        return str;
    }
  return NULL;
}

bool WmsDialog::GetStyleByIndex(int idx, const char **name, const char **title,
                                const char **abstract)
{
// return some WMS Format (retrieved by its Index)
  *name = get_wms_layer_style_name(CurrentLayer, idx);
  *title = get_wms_layer_style_title(CurrentLayer, idx);
  *abstract = get_wms_layer_style_abstract(CurrentLayer, idx);
  if (*name == NULL)
    return false;
  return true;
}

double WmsDialog::GetGeoMinX()
{
// return the WMS Geographic MinX
  double minx;
  double miny;
  double maxx;
  double maxy;
  if (!get_wms_layer_geo_bbox(CurrentLayer, &minx, &maxx, &miny, &maxy))
    return DBL_MAX;
  return minx;
}

double WmsDialog::GetGeoMaxX()
{
// return the WMS Geographic MaxX
  double minx;
  double miny;
  double maxx;
  double maxy;
  if (!get_wms_layer_geo_bbox(CurrentLayer, &minx, &maxx, &miny, &maxy))
    return DBL_MAX;
  return maxx;
}

double WmsDialog::GetGeoMinY()
{
// return the WMS Geographic MinY
  double minx;
  double miny;
  double maxx;
  double maxy;
  if (!get_wms_layer_geo_bbox(CurrentLayer, &minx, &maxx, &miny, &maxy))
    return DBL_MAX;
  return miny;
}

double WmsDialog::GetGeoMaxY()
{
// return the WMS Geographic MaxY
  double minx;
  double miny;
  double maxx;
  double maxy;
  if (!get_wms_layer_geo_bbox(CurrentLayer, &minx, &maxx, &miny, &maxy))
    return DBL_MAX;
  return maxy;
}

double WmsDialog::GetMinX()
{
// return the WMS CRS MinX
  double minx;
  double miny;
  double maxx;
  double maxy;
  const char *crs_str = NULL;
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  wxString value = comboCtrl->GetValue();
  if (value.Len() == 0)
    return DBL_MAX;
  int cnt = get_wms_layer_crs_count(CurrentLayer);
  for (int i = 0; i < cnt; i++)
    {
      const char *str = get_wms_layer_crs(CurrentLayer, i);
      wxString crs = wxString::FromUTF8(str);
      if (crs == value)
        {
          crs_str = str;
          break;
        }
    }
  if (crs_str == NULL)
    return DBL_MAX;
  if (!get_wms_layer_bbox(CurrentLayer, crs_str, &minx, &maxx, &miny, &maxy))
    {
      if (!get_wms_layer_geo_bbox(CurrentLayer, &minx, &maxx, &miny, &maxy))
        return DBL_MAX;
      if (MainFrame->BBoxFromLongLat(crs_str, &minx, &maxx, &miny, &maxy) ==
          true)
        {
          if (IsSwapXY() == true)
            return miny;
          else
            return minx;
      } else
        return DBL_MAX;
    }
  return minx;
}

double WmsDialog::GetMaxX()
{
// return the WMS CRS MaxX
  double minx;
  double miny;
  double maxx;
  double maxy;
  const char *crs_str = NULL;
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_WMS_NAME);
  wxString name = nameCtrl->GetValue();
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  wxString value = comboCtrl->GetValue();
  if (value.Len() == 0)
    return DBL_MAX;
  int cnt = get_wms_layer_crs_count(CurrentLayer);
  for (int i = 0; i < cnt; i++)
    {
      const char *str = get_wms_layer_crs(CurrentLayer, i);
      wxString crs = wxString::FromUTF8(str);
      if (crs == value)
        {
          crs_str = str;
          break;
        }
    }
  if (crs_str == NULL)
    return DBL_MAX;
  if (!get_wms_layer_bbox(CurrentLayer, crs_str, &minx, &maxx, &miny, &maxy))
    {
      if (!get_wms_layer_geo_bbox(CurrentLayer, &minx, &maxx, &miny, &maxy))
        return DBL_MAX;
      if (MainFrame->BBoxFromLongLat(crs_str, &minx, &maxx, &miny, &maxy) ==
          true)
        {
          if (IsSwapXY() == true)
            return maxy;
          else
            return maxx;
      } else
        return DBL_MAX;
    }
  return maxx;
}

double WmsDialog::GetMinY()
{
// return the WMS CRS MinY
  double minx;
  double miny;
  double maxx;
  double maxy;
  const char *crs_str = NULL;
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  wxString value = comboCtrl->GetValue();
  if (value.Len() == 0)
    return DBL_MAX;
  int cnt = get_wms_layer_crs_count(CurrentLayer);
  for (int i = 0; i < cnt; i++)
    {
      const char *str = get_wms_layer_crs(CurrentLayer, i);
      wxString crs = wxString::FromUTF8(str);
      if (crs == value)
        {
          crs_str = str;
          break;
        }
    }
  if (crs_str == NULL)
    return DBL_MAX;
  if (!get_wms_layer_bbox(CurrentLayer, crs_str, &minx, &maxx, &miny, &maxy))
    {
      if (!get_wms_layer_geo_bbox(CurrentLayer, &minx, &maxx, &miny, &maxy))
        return DBL_MAX;
      if (MainFrame->BBoxFromLongLat(crs_str, &minx, &maxx, &miny, &maxy) ==
          true)
        {
          if (IsSwapXY() == true)
            return minx;
          else
            return miny;
      } else
        return DBL_MAX;
    }
  return miny;
}

double WmsDialog::GetMaxY()
{
// return the WMS CRS MaxY
  double minx;
  double miny;
  double maxx;
  double maxy;
  const char *crs_str = NULL;
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  wxString value = comboCtrl->GetValue();
  if (value.Len() == 0)
    return DBL_MAX;
  int cnt = get_wms_layer_crs_count(CurrentLayer);
  for (int i = 0; i < cnt; i++)
    {
      const char *str = get_wms_layer_crs(CurrentLayer, i);
      wxString crs = wxString::FromUTF8(str);
      if (crs == value)
        {
          crs_str = str;
          break;
        }
    }
  if (crs_str == NULL)
    return DBL_MAX;
  if (!get_wms_layer_bbox(CurrentLayer, crs_str, &minx, &maxx, &miny, &maxy))
    {
      if (!get_wms_layer_geo_bbox(CurrentLayer, &minx, &maxx, &miny, &maxy))
        return DBL_MAX;
      if (MainFrame->BBoxFromLongLat(crs_str, &minx, &maxx, &miny, &maxy) ==
          true)
        {
          if (IsSwapXY() == true)
            return maxx;
          else
            return maxy;
      } else
        return DBL_MAX;
    }
  return maxy;
}

void WmsDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void WmsDialog::OnWmsOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *proxyCtrl = (wxTextCtrl *) FindWindow(ID_WMS_PROXY);
  if (ProxyEnabled == false)
    HttpProxy = wxT("");
  else
    HttpProxy = proxyCtrl->GetValue();
  wxTextCtrl *urlCtrl = (wxTextCtrl *) FindWindow(ID_WMS_URL);
  URL = urlCtrl->GetValue();
  wxDialog::EndModal(wxID_OK);
}

bool WmsServersDialog::Create(MyFrame * grandparent)
{
//
// creating the dialog
//
  MainFrame = grandparent;
  List = NULL;
  Count = 0;
  if (wxDialog::Create(grandparent, wxID_ANY,
                       wxT("Select a registered WMS Server")) == false)
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

void WmsServersDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// the registered WMS servers List
  PopulateList();
  wxBoxSizer *listSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(listSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  WmsServers =
    new wxListBox(this, ID_WMS_LIST, wxDefaultPosition, wxSize(700, 300), Count,
                  List, wxLB_SINGLE | wxLB_HSCROLL | wxLB_ALWAYS_SB);
  listSizer->Add(WmsServers, 0, wxALIGN_RIGHT | wxALL, 5);
// selected WMS datasource group box
  wxBoxSizer *kkSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(kkSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *dsBox = new wxStaticBox(this, wxID_STATIC,
                                       wxT("WMS Server"),
                                       wxDefaultPosition,
                                       wxDefaultSize);
  wxBoxSizer *dsBox0Sizer = new wxStaticBoxSizer(dsBox, wxHORIZONTAL);
  kkSizer->Add(dsBox0Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *dsBoxSizer = new wxBoxSizer(wxVERTICAL);
  dsBox0Sizer->Add(dsBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// the GetCapabilities URL
  wxBoxSizer *urlSizer = new wxBoxSizer(wxHORIZONTAL);
  dsBoxSizer->Add(urlSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *urlCtrl = new wxTextCtrl(this, ID_WMS_URL, wxT(""),
                                       wxDefaultPosition, wxSize(700, 22),
                                       wxTE_READONLY);
  urlSizer->Add(urlCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Title / Abstract panel
  wxBoxSizer *title0BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  dsBoxSizer->Add(title0BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *titleBox = new wxStaticBox(this, wxID_ANY,
                                          wxT("Service Title"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *titleBoxSizer = new wxStaticBoxSizer(titleBox, wxHORIZONTAL);
  title0BoxSizer->Add(titleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxHtmlWindow *titleCtrl = new wxHtmlWindow(this, ID_WMS_TITLE,
                                             wxDefaultPosition, wxSize(300,
                                                                       120));
  titleBoxSizer->Add(titleCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *abstractBox = new wxStaticBox(this, wxID_ANY,
                                             wxT("Service Abstract"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  wxBoxSizer *abstractBoxSizer =
    new wxStaticBoxSizer(abstractBox, wxHORIZONTAL);
  title0BoxSizer->Add(abstractBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxHtmlWindow *abstractCtrl = new wxHtmlWindow(this, ID_WMS_ABSTRACT,
                                                wxDefaultPosition, wxSize(350,
                                                                          120));
  abstractBoxSizer->Add(abstractCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// OK-Cancel buttons
  wxBoxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
  dsBoxSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *okBtn = new wxButton(this, wxID_OK, wxT("&OK"));
  okBtn->Enable(false);
  btnSizer->Add(okBtn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancelBtn = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnSizer->Add(cancelBtn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handlers
  Connect(ID_WMS_LIST, wxEVT_COMMAND_LISTBOX_SELECTED,
          (wxObjectEventFunction) & WmsServersDialog::OnUrlSelected);
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsServersDialog::OnOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsServersDialog::OnCancel);
}

void WmsServersDialog::PopulateList()
{
//
// populating the WMS Servers list
//
  int ret;
  const char *sql;
  char **results;
  int rows;
  int columns;

  if (List != NULL)
    {
      delete[]List;
      List = NULL;
    }
  Count = 0;

// preparing the SQL query
  sql = "SELECT url FROM MAIN.wms_getcapabilities ORDER BY id";
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;

  Count = rows;
  List = new wxString[Count];
  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      wxString url = wxString::FromUTF8(results[(i * columns) + 0]);
      *(List + i - 1) = url;
    }
  sqlite3_free_table(results);
}

void WmsServersDialog::OnUrlSelected(wxCommandEvent & WXUNUSED(event))
{
//
// GetCapabilities URL selection changed
//
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  wxListBox *listCtrl = (wxListBox *) FindWindow(ID_WMS_LIST);
  URL = listCtrl->GetStringSelection();
  char *url = (char *) malloc((URL.Len() * 4) + 1);
  strcpy(url, URL.ToUTF8());
  wxTextCtrl *urlCtrl = (wxTextCtrl *) FindWindow(ID_WMS_URL);
  urlCtrl->SetValue(URL);
  wxButton *okBtn = (wxButton *) FindWindow(wxID_OK);
  okBtn->Enable(true);

// preparing the SQL query
  sql =
    sqlite3_mprintf
    ("SELECT title, abstract FROM MAIN.wms_getcapabilities WHERE url = %Q",
     url);
  free(url);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      wxString title = wxString::FromUTF8(results[(i * columns) + 0]);
      wxString abstract = wxString::FromUTF8(results[(i * columns) + 1]);
      wxHtmlWindow *titleCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_TITLE);
      wxHtmlWindow *abstractCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_ABSTRACT);
      title.Replace(wxT("\n"), wxT("<br>"));
      titleCtrl->SetPage(wxT("<html><body>") + title + wxT("</body></html>"));
      abstract.Replace(wxT("\n"), wxT("<br>"));
      abstractCtrl->SetPage(wxT("<html><body>") + abstract +
                            wxT("</body></html>"));
    }
  sqlite3_free_table(results);
}

void WmsServersDialog::OnCancel(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void WmsServersDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *urlCtrl = (wxTextCtrl *) FindWindow(ID_WMS_URL);
  URL = urlCtrl->GetValue();
  wxDialog::EndModal(wxID_OK);
}

bool WmsLayerDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  CurrentEvtRow = -1;
  CurrentEvtColumn = -1;
  ListUrl = NULL;
  ListLayer = NULL;
  Count = 0;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Select a registered WMS Layer")) == false)
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

void WmsLayerDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// the registered WMS GetMap List
  PopulateList();
  wxBoxSizer *listSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(listSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  WmsLayers = new wxGrid(this, ID_WMS_LIST, wxPoint(5, 5), wxSize(700, 200));
  WmsLayers->CreateGrid(Count, 2);
  WmsLayers->EnableEditing(false);
  WmsLayers->SetColLabelValue(0, wxT("Layer Name"));
  WmsLayers->SetColLabelValue(1, wxT("GetMap URL"));
  for (int i = 0; i < Count; i++)
    {
      WmsLayers->SetCellValue(i, 0, *(ListLayer + i));
      WmsLayers->SetCellValue(i, 1, *(ListUrl + i));
    }
  delete[]ListUrl;
  delete[]ListLayer;
  ListUrl = NULL;
  ListLayer = NULL;
  WmsLayers->SetRowLabelSize(wxGRID_AUTOSIZE);
  WmsLayers->AutoSize();
  WmsLayers->SetSize(700, 200);
  listSizer->Add(WmsLayers, 0, wxALIGN_RIGHT | wxALL, 5);
// selected WMS datasource group box
  wxBoxSizer *kkSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(kkSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *dsBox = new wxStaticBox(this, wxID_STATIC,
                                       wxT("WMS GetMap URL"),
                                       wxDefaultPosition,
                                       wxDefaultSize);
  wxBoxSizer *dsBox0Sizer = new wxStaticBoxSizer(dsBox, wxHORIZONTAL);
  kkSizer->Add(dsBox0Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *dsBoxSizer = new wxBoxSizer(wxVERTICAL);
  dsBox0Sizer->Add(dsBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// the GetMap URL
  wxBoxSizer *urlSizer = new wxBoxSizer(wxHORIZONTAL);
  dsBoxSizer->Add(urlSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *urlCtrl = new wxTextCtrl(this, ID_WMS_URL, wxT(""),
                                       wxDefaultPosition, wxSize(700, 22),
                                       wxTE_READONLY);
  urlSizer->Add(urlCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// the Layer Name
  wxBoxSizer *layerSizer = new wxBoxSizer(wxHORIZONTAL);
  dsBoxSizer->Add(layerSizer, 0, wxALIGN_LEFT | wxALL, 5);
  wxStaticText *layerLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Layer Name:"));
  layerSizer->Add(layerLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxTextCtrl *layerCtrl = new wxTextCtrl(this, ID_WMS_LAYER, wxT(""),
                                         wxDefaultPosition, wxSize(200, 22),
                                         wxTE_READONLY);
  layerSizer->Add(layerCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Title / Abstract panel
  wxBoxSizer *title0BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  dsBoxSizer->Add(title0BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *titleBox = new wxStaticBox(this, wxID_ANY,
                                          wxT("Layer Title"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *titleBoxSizer = new wxStaticBoxSizer(titleBox, wxHORIZONTAL);
  title0BoxSizer->Add(titleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxHtmlWindow *titleCtrl = new wxHtmlWindow(this, ID_WMS_TITLE,
                                             wxDefaultPosition, wxSize(300,
                                                                       120));
  titleBoxSizer->Add(titleCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *abstractBox = new wxStaticBox(this, wxID_ANY,
                                             wxT("Layer Abstract"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  wxBoxSizer *abstractBoxSizer =
    new wxStaticBoxSizer(abstractBox, wxHORIZONTAL);
  title0BoxSizer->Add(abstractBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxHtmlWindow *abstractCtrl = new wxHtmlWindow(this, ID_WMS_ABSTRACT,
                                                wxDefaultPosition, wxSize(350,
                                                                          120));
  abstractBoxSizer->Add(abstractCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// OK-Cancel buttons
  wxBoxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
  dsBoxSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *okBtn = new wxButton(this, wxID_OK, wxT("&OK"));
  okBtn->Enable(false);
  btnSizer->Add(okBtn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancelBtn = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnSizer->Add(cancelBtn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handlers
  Connect(ID_WMS_LIST, wxEVT_GRID_CELL_LEFT_CLICK,
          (wxObjectEventFunction) & WmsLayerDialog::OnLeftClick);
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsLayerDialog::OnOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsLayerDialog::OnCancel);
}

void WmsLayerDialog::PopulateList()
{
//
// populating the WMS Layers list
//
  int ret;
  const char *sql;
  char **results;
  int rows;
  int columns;

  if (ListUrl != NULL)
    {
      delete[]ListUrl;
      ListUrl = NULL;
    }
  if (ListLayer != NULL)
    {
      delete[]ListLayer;
      ListLayer = NULL;
    }
  Count = 0;

// preparing the SQL query
  sql = "SELECT url, layer_name FROM MAIN.wms_getmap ORDER BY id";
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;

  Count = rows;
  ListUrl = new wxString[Count];
  ListLayer = new wxString[Count];
  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      wxString url = wxString::FromUTF8(results[(i * columns) + 0]);
      *(ListUrl + i - 1) = url;
      wxString layer = wxString::FromUTF8(results[(i * columns) + 1]);
      *(ListLayer + i - 1) = layer;
    }
  sqlite3_free_table(results);
}

void WmsLayerDialog::OnLeftClick(wxGridEvent & event)
{
//
// left click on some cell [mouse action]
//
  int previous = CurrentEvtRow;
  CurrentEvtRow = event.GetRow();
  CurrentEvtColumn = event.GetCol();
  if (CurrentEvtRow != previous)
    SelectWmsLayer();
}

void WmsLayerDialog::SelectWmsLayer()
{
//
// GetMap URL selection changed
//
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  WmsLayers->Show(false);
  WmsLayers->ClearSelection();
  WmsLayers->SelectRow(CurrentEvtRow);
  WmsLayers->Show(true);
  LayerName = WmsLayers->GetCellValue(CurrentEvtRow, 0);
  URL = WmsLayers->GetCellValue(CurrentEvtRow, 1);

  char *url = (char *) malloc((URL.Len() * 4) + 1);
  strcpy(url, URL.ToUTF8());
  wxTextCtrl *urlCtrl = (wxTextCtrl *) FindWindow(ID_WMS_URL);
  urlCtrl->SetValue(URL);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  wxTextCtrl *layerCtrl = (wxTextCtrl *) FindWindow(ID_WMS_LAYER);
  layerCtrl->SetValue(LayerName);
  wxButton *okBtn = (wxButton *) FindWindow(wxID_OK);
  okBtn->Enable(true);

// preparing the SQL query
  sql =
    sqlite3_mprintf
    ("SELECT title, abstract FROM MAIN.wms_getmap WHERE url = %Q AND layer_name = %Q",
     url, layer);
  free(url);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      wxString title = wxString::FromUTF8(results[(i * columns) + 0]);
      wxString abstract = wxString::FromUTF8(results[(i * columns) + 1]);
      wxHtmlWindow *titleCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_TITLE);
      wxHtmlWindow *abstractCtrl = (wxHtmlWindow *) FindWindow(ID_WMS_ABSTRACT);
      title.Replace(wxT("\n"), wxT("<br>"));
      titleCtrl->SetPage(wxT("<html><body>") + title + wxT("</body></html>"));
      abstract.Replace(wxT("\n"), wxT("<br>"));
      abstractCtrl->SetPage(wxT("<html><body>") + abstract +
                            wxT("</body></html>"));
    }
  sqlite3_free_table(results);
}

void WmsLayerDialog::OnCancel(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void WmsLayerDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *urlCtrl = (wxTextCtrl *) FindWindow(ID_WMS_URL);
  URL = urlCtrl->GetValue();
  wxTextCtrl *layerCtrl = (wxTextCtrl *) FindWindow(ID_WMS_LAYER);
  LayerName = layerCtrl->GetValue();
  wxDialog::EndModal(wxID_OK);
}

bool WmsLayerUnregisterDialog::Create(MyFrame * parent, wxString & url,
                                      wxString & name, wxString & title,
                                      wxString & abstract, wxString & copyright,
                                      wxString & data_license)
{
//
// creating the dialog
//
  MainFrame = parent;
  URL = url;
  LayerName = name;
  Title = title;
  Abstract = abstract;
  Copyright = copyright;
  DataLicense = data_license;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Unregister WMS Layer")) == false)
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

void WmsLayerUnregisterDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the GetMap URL
  wxBoxSizer *urlSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(urlSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *urlLabel =
    new wxStaticText(this, wxID_STATIC, wxT("GetMap URL:"));
  urlSizer->Add(urlLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *urlCtrl = new wxTextCtrl(this, wxID_ANY, URL,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  urlSizer->Add(urlCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the WMS Layer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("WMS &Layer:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, wxID_ANY, LayerName,
                                        wxDefaultPosition, wxSize(600, 22),
                                        wxTE_READONLY);
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the WMS Layer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, wxID_ANY, Title,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the WMS Layer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, wxID_ANY, Abstract,
                                            wxDefaultPosition, wxSize(600, 60),
                                            wxTE_MULTILINE | wxTE_READONLY);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyCtrl = new wxTextCtrl(this, wxID_ANY, Copyright,
                                        wxDefaultPosition, wxSize(600, 60),
                                        wxTE_MULTILINE | wxTE_READONLY);
  copySizer->Add(copyCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Data License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *licCtrl = new wxTextCtrl(this, wxID_ANY, DataLicense,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  licSizer->Add(licCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row: confirm message
  wxBoxSizer *msgSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(msgSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
  wxString confirm = wxT("This WMS Layer will be UnRegistered");
  wxStaticText *msg1Label = new wxStaticText(this, wxID_STATIC, confirm);
  msgSizer->Add(msg1Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  confirm = wxT("Do you really confirm unregistering the WMS Layer ?");
  wxStaticText *msg2Label = new wxStaticText(this, wxID_STATIC, confirm);
  msgSizer->Add(msg2Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&UnRegister"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsLayerUnregisterDialog::OnOk);
}

bool WmsLayerUnregisterDialog::DoWmsLayerUnregister()
{
//
// UnRegistering a WMS Layer
//
  char *sql;
  char *errMsg = NULL;
  int ret;
  char *url = (char *) malloc((URL.Len() * 4) + 1);
  strcpy(url, URL.ToUTF8());
  char *layer_name = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer_name, LayerName.ToUTF8());
  sql = sqlite3_mprintf("SELECT WMS_UnRegisterGetMap(%Q, %Q)", url, layer_name);
  free(url);
  free(layer_name);
  ret = sqlite3_exec(MainFrame->GetSqlite(), sql, NULL, NULL, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("UnRegister WMS Layer error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  return true;
}

void WmsLayerUnregisterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  if (DoWmsLayerUnregister() == true)
    wxMessageBox(wxT("WMS Layer \"") + LayerName +
                 wxT("\" successfully UnRegistered."),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  else
    wxMessageBox(wxT("Failure: some unexpected error occurred"),
                 wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  wxDialog::EndModal(wxID_CANCEL);
}

bool WmsLayerInfosDialog::Create(MyFrame * parent, wxString & url,
                                 wxString & name, wxString & title,
                                 wxString & abstract, wxString & copyright,
                                 int license_id, bool is_queryable,
                                 wxString & getfeatureinfo_url)
{
//
// creating the dialog
//
  MainFrame = parent;
  URL = url;
  LayerName = name;
  Title = title;
  Abstract = abstract;
  Copyright = copyright;
  LicenseID = license_id;
  IsQueryable = is_queryable;
  GetFeatureInfoUrl = getfeatureinfo_url;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Edit WMS Layer Infos")) == false)
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

void WmsLayerInfosDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the GetMap URL
  wxBoxSizer *urlSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(urlSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *urlLabel =
    new wxStaticText(this, wxID_STATIC, wxT("GetMap URL:"));
  urlSizer->Add(urlLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *urlCtrl = new wxTextCtrl(this, ID_WMS_URL, URL,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  urlSizer->Add(urlCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the WMS Layer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("WMS &Layer:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_WMS_LAYER, LayerName,
                                        wxDefaultPosition, wxSize(600, 22),
                                        wxTE_READONLY);
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_WMS_TITLE, Title,
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, ID_WMS_ABSTRACT, Abstract,
                                            wxDefaultPosition, wxSize(600, 60),
                                            wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyrightCtrl = new wxTextCtrl(this, ID_WMS_COPYRIGHT, Copyright,
                                             wxDefaultPosition, wxSize(600, 60),
                                             wxTE_MULTILINE);
  copySizer->Add(copyrightCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Data &License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *licenseCtrl =
    new wxComboBox(this, ID_WMS_LICENSE, wxT(""), wxDefaultPosition,
                   wxSize(600, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  licSizer->Add(licenseCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  PopulateDataLicenses(licenseCtrl);
// seventh row: queryable
  wxBoxSizer *radioSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(radioSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString yesno[2];
  yesno[0] = wxT("&No");
  yesno[1] = wxT("&Yes");
  wxRadioBox *queryableBox = new wxRadioBox(this, ID_WMS_QUERYABLE,
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
// eigth row: the GetFeatureInfo URL
  wxBoxSizer *infoSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(infoSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *infoLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&GetFeatureInfo URL:"));
  infoSizer->Add(infoLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *getFeatureInfoCtrl =
    new wxTextCtrl(this, ID_WMS_GETFEATUREINFO, GetFeatureInfoUrl,
                   wxDefaultPosition, wxSize(600, 22));
  infoSizer->Add(getFeatureInfoCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  if (IsQueryable == false)
    {
      getFeatureInfoCtrl->SetValue(wxEmptyString);
      getFeatureInfoCtrl->Enable(false);
    }
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&OK"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsLayerInfosDialog::OnOk);
  Connect(ID_WMS_QUERYABLE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & WmsLayerInfosDialog::OnQueryableChanged);
}

void WmsLayerInfosDialog::OnQueryableChanged(wxCommandEvent & WXUNUSED(event))
{
//
// QUERYABLE changed
//
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_WMS_QUERYABLE);
  wxTextCtrl *getFeatureInfoCtrl =
    (wxTextCtrl *) FindWindow(ID_WMS_GETFEATUREINFO);
  if (queryableBox->GetSelection() == 1)
    {
      getFeatureInfoCtrl->SetValue(GetFeatureInfoUrl);
      getFeatureInfoCtrl->Enable(true);
  } else
    {
      getFeatureInfoCtrl->SetValue(wxEmptyString);
      getFeatureInfoCtrl->Enable(false);
    }
}

void WmsLayerInfosDialog::PopulateDataLicenses(wxComboBox * licCtrl)
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

void WmsLayerInfosDialog::DoUpdateWmsLayer()
{
// will try updating the given WMS Layer definitions
  int ret;
  sqlite3_stmt *stmt = NULL;
  char *dummy;
  bool error = false;

// updating the Title and Abstract
  const char *sql = "SELECT WMS_SetGetMapInfos(?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerInfos: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((URL.Len() * 4) + 1);
  strcpy(dummy, URL.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((Title.Len() * 4) + 1);
  strcpy(dummy, Title.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((Abstract.Len() * 4) + 1);
  strcpy(dummy, Abstract.ToUTF8());
  sqlite3_bind_text(stmt, 4, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerInfos: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error)
    return;

// updating the Copyright and License
  sql = "SELECT WMS_SetGetMapCopyright(?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerCopyright: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((URL.Len() * 4) + 1);
  strcpy(dummy, URL.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (Copyright.Len() == 0)
    sqlite3_bind_null(stmt, 3);
  else
    {
      dummy = (char *) malloc((Copyright.Len() * 4) + 1);
      strcpy(dummy, Copyright.ToUTF8());
      sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
      free(dummy);
    }
  dummy = (char *) malloc((DataLicense.Len() * 4) + 1);
  strcpy(dummy, DataLicense.ToUTF8());
  sqlite3_bind_text(stmt, 4, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerCopyright ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error)
    return;

// updating IsQueryable and GetFeatureInfo URL
  sql = "SELECT WMS_SetGetMapOptions(?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerOptions: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((URL.Len() * 4) + 1);
  strcpy(dummy, URL.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (IsQueryable == true)
    sqlite3_bind_int(stmt, 3, 1);
  else
    sqlite3_bind_int(stmt, 3, 0);
  dummy = (char *) malloc((GetFeatureInfoUrl.Len() * 4) + 1);
  strcpy(dummy, GetFeatureInfoUrl.ToUTF8());
  sqlite3_bind_text(stmt, 4, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerOptions ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
}

void WmsLayerInfosDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_WMS_TITLE);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_WMS_ABSTRACT);
  wxTextCtrl *copyCtrl = (wxTextCtrl *) FindWindow(ID_WMS_COPYRIGHT);
  wxComboBox *licCtrl = (wxComboBox *) FindWindow(ID_WMS_LICENSE);
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_WMS_QUERYABLE);
  wxTextCtrl *getFeatureInfoCtrl =
    (wxTextCtrl *) FindWindow(ID_WMS_GETFEATUREINFO);
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
  if (IsQueryable == true)
    {
      str = getFeatureInfoCtrl->GetValue().Trim();
      GetFeatureInfoUrl = str.Trim(false);
      if (GetFeatureInfoUrl.Len() == 0)
        {
          wxMessageBox(wxT("You must specify some GetFeatureInfo URL !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
    }
  DoUpdateWmsLayer();
  wxDialog::EndModal(wxID_OK);
}

WmsLayerConfigDialog::~WmsLayerConfigDialog()
{
  if (Version != NULL)
    free(Version);
  if (MaxVersion != NULL)
    free(MaxVersion);
  if (RefSys != NULL)
    free(RefSys);
  if (Style != NULL)
    free(Style);
  if (ImageFormat != NULL)
    free(ImageFormat);
  if (BgColor != NULL)
    free(BgColor);
}

bool WmsLayerConfigDialog::Create(MyFrame * parent, wxString url,
                                  wxString layer)
{
//
// creating the dialog
//
  MainFrame = parent;
  MapPanel = NULL;
  Layer = NULL;
  Url = url;
  DbPrefix = wxT("MAIN");
  LayerName = layer;
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (wxDialog::Create(parent, wxID_ANY, wxT("WMS Layer Configuration")) ==
      false)
    return false;
// populates individual controls
  LoadData();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

bool WmsLayerConfigDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  Url = wxString::FromUTF8(layer->GetWmsGetMapURL());
  InfoUrl = wxString::FromUTF8(layer->GetWmsGetFeatureInfoURL());
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Map Layer (WMS) Configuration"))
      == false)
    return false;
// populates individual controls
  InitData();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void WmsLayerConfigDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// First row: Layer name
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *urlSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(urlSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *urlBox = new wxStaticBox(this, wxID_ANY,
                                        wxT("WMS GetMap URL"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *urlBoxSizer = new wxStaticBoxSizer(urlBox, wxHORIZONTAL);
  urlSizer->Add(urlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *urlCtrl = new wxTextCtrl(this, ID_WMS_URL, Url,
                                       wxDefaultPosition, wxSize(800, 22),
                                       wxTE_READONLY);
  urlBoxSizer->Add(urlCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *urlInfoSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(urlInfoSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *urlInfoBox = new wxStaticBox(this, wxID_ANY,
                                            wxT("WMS GetFeatureInfo URL"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *urlInfoBoxSizer = new wxStaticBoxSizer(urlInfoBox, wxHORIZONTAL);
  urlInfoSizer->Add(urlInfoBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *urlInfoCtrl = new wxTextCtrl(this, ID_WMS_INFO_URL, InfoUrl,
                                           wxDefaultPosition, wxSize(800, 22),
                                           wxTE_READONLY);
  urlInfoBoxSizer->Add(urlInfoCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer Name"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_WMS_LAYER, LayerName,
                                        wxDefaultPosition, wxSize(420, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *swapBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Swap Axes"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *swapBoxSizer = new wxStaticBoxSizer(swapBox, wxHORIZONTAL);
  nameSizer->Add(swapBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *swapCtrl = new wxCheckBox(this, ID_WMS_SWAP,
                                        wxT("Swap Y,X"),
                                        wxDefaultPosition, wxDefaultSize);
  if (SwapXY == 0)
    swapCtrl->SetValue(false);
  else
    swapCtrl->SetValue(true);
  swapBoxSizer->Add(swapCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString cached[2];
  cached[0] = wxT("yes");
  cached[1] = wxT("no");
  wxRadioBox *cachedBox = new wxRadioBox(this, ID_WMS_CACHED,
                                         wxT("WMS caching"),
                                         wxDefaultPosition,
                                         wxDefaultSize, 2,
                                         cached, 2,
                                         wxRA_SPECIFY_COLS);
  if (Cached == 0)
    cachedBox->SetSelection(1);
  else
    cachedBox->SetSelection(0);
  nameSizer->Add(cachedBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticBox *bgColorBox = new wxStaticBox(this, wxID_ANY,
                                            wxT("Background Color"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *bgColorBoxSizer = new wxStaticBoxSizer(bgColorBox, wxHORIZONTAL);
  nameSizer->Add(bgColorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enableBgColorCtrl = new wxCheckBox(this, ID_WMS_ENABLE_BGCOLOR,
                                                 wxT("Enable"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  if (BgColor == NULL)
    {
      BgColorEnabled = false;
      enableBgColorCtrl->SetValue(false);
  } else
    {
      BgColorEnabled = true;
      enableBgColorCtrl->SetValue(true);
    }
  bgColorBoxSizer->Add(enableBgColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                       5);
  wxBitmap bgColorBmp;
  GetButtonBitmap(BgColor, bgColorBmp);
  wxBitmapButton *bgColorCtrl =
    new wxBitmapButton(this, ID_WMS_BGCOLOR, bgColorBmp);
  bgColorCtrl->Enable(BgColorEnabled);
  bgColorBoxSizer->Add(bgColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// First row: WMS options
  wxBoxSizer *wmsSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(wmsSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString ver[4];
  ver[0] = wxT("WMS &1.0.0");
  ver[1] = wxT("WMS &1.1.0");
  ver[2] = wxT("WMS &1.1.1");
  ver[3] = wxT("WMS &1.3.0");
  wxRadioBox *versionBox = new wxRadioBox(this, ID_WMS_VERSION,
                                          wxT("WMS &Version"),
                                          wxDefaultPosition,
                                          wxDefaultSize, 4,
                                          ver, 4,
                                          wxRA_SPECIFY_ROWS);
  FindMaxVersion();
  if (MaxVersion == NULL)
    {
      versionBox->Enable(1, false);
      versionBox->Enable(2, false);
      versionBox->Enable(3, false);
  } else
    {
      if (strcmp(MaxVersion, "1.1.0") == 0)
        {
          versionBox->Enable(2, false);
          versionBox->Enable(3, false);
      } else if (strcmp(MaxVersion, "1.1.1") == 0)
        {
          versionBox->Enable(3, false);
      } else if (strcmp(MaxVersion, "1.3.0") == 0)
        ;
      else
        {
          versionBox->Enable(1, false);
          versionBox->Enable(2, false);
          versionBox->Enable(3, false);
        }
    }
  if (strcmp(Version, "1.0.0") == 0)
    versionBox->SetSelection(0);
  else if (strcmp(Version, "1.1.0") == 0)
    versionBox->SetSelection(1);
  else if (strcmp(Version, "1.1.1") == 0)
    versionBox->SetSelection(2);
  else
    versionBox->SetSelection(3);
  wmsSizer->Add(versionBox, 0, wxALIGN_TOP | wxALL, 5);
  wxBoxSizer *xxSizer = new wxBoxSizer(wxVERTICAL);
  wmsSizer->Add(xxSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *crsBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Reference System"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *crsBoxSizer = new wxStaticBoxSizer(crsBox, wxVERTICAL);
  xxSizer->Add(crsBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *crsList =
    new wxComboBox(this, ID_WMS_CRS, wxT(""), wxDefaultPosition,
                   wxSize(250, 21), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  PopulateRefSys(crsList);
  crsBoxSizer->Add(crsList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticBox *stlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *stlBoxSizer = new wxStaticBoxSizer(stlBox, wxVERTICAL);
  xxSizer->Add(stlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *stlList =
    new wxComboBox(this, ID_WMS_STYLE, wxT(""), wxDefaultPosition,
                   wxSize(250, 21), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  PopulateStyles(stlList);
  stlBoxSizer->Add(stlList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *yySizer = new wxBoxSizer(wxVERTICAL);
  wmsSizer->Add(yySizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *fmtBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Image Format"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *fmtBoxSizer = new wxStaticBoxSizer(fmtBox, wxVERTICAL);
  yySizer->Add(fmtBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *fmtList =
    new wxComboBox(this, ID_WMS_FORMAT, wxT(""), wxDefaultPosition,
                   wxSize(150, 21), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  PopulateImageFormats(fmtList);
  fmtBoxSizer->Add(fmtList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxString trans[2];
  trans[0] = wxT("trasparent");
  trans[1] = wxT("opaque");
  wxRadioBox *transBox = new wxRadioBox(this, ID_WMS_TRANSPARENT,
                                        wxT("Transparent Layer"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 2,
                                        trans, 2,
                                        wxRA_SPECIFY_COLS);
  if (Opaque == 0)
    transBox->SetSelection(0);
  else
    transBox->SetSelection(1);
  yySizer->Add(transBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxStaticBox *tileBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Tile size"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *tileBoxSizer = new wxStaticBoxSizer(tileBox, wxVERTICAL);
  wmsSizer->Add(tileBoxSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxCheckBox *tiledCtrl = new wxCheckBox(this, ID_WMS_TILED,
                                         wxT("enable Tiles"),
                                         wxDefaultPosition, wxDefaultSize);
  if (Tiled == 0)
    tiledCtrl->SetValue(false);
  else
    tiledCtrl->SetValue(true);
  tileBoxSizer->Add(tiledCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *tile1BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  tileBoxSizer->Add(tile1BoxSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *wLabel = new wxStaticText(this, wxID_ANY, wxT("&Width:"),
                                          wxDefaultPosition, wxDefaultSize,
                                          wxALIGN_RIGHT);
  tile1BoxSizer->Add(wLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxSpinCtrl *wCtrl = new wxSpinCtrl(this, ID_WMS_WIDTH, wxEmptyString,
                                     wxDefaultPosition, wxSize(80, 20),
                                     wxSP_ARROW_KEYS,
                                     0, 0, 0);
  if (Tiled == 0)
    {
      wCtrl->Enable(false);
      wCtrl->SetValue(wxT(""));
  } else
    {
      wCtrl->SetRange(256, 5000);
      char t_w[64];
      sprintf(t_w, "%d", TileWidth);
      wxString tW = wxString::FromUTF8(t_w);
      wCtrl->SetValue(tW);
    }
  tile1BoxSizer->Add(wCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *tile2BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  tileBoxSizer->Add(tile2BoxSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *hLabel = new wxStaticText(this, wxID_ANY, wxT("&Height:"),
                                          wxDefaultPosition, wxDefaultSize,
                                          wxALIGN_RIGHT);
  tile2BoxSizer->Add(hLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxSpinCtrl *hCtrl = new wxSpinCtrl(this, ID_WMS_HEIGHT, wxEmptyString,
                                     wxDefaultPosition, wxSize(80, 20),
                                     wxSP_ARROW_KEYS,
                                     0, 0, 0);
  tile2BoxSizer->Add(hCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  if (Tiled == 0)
    {
      hCtrl->Enable(false);
      hCtrl->SetValue(wxT(""));
  } else
    {
      hCtrl->SetRange(256, 5000);
      char t_h[64];
      sprintf(t_h, "%d", TileHeight);
      wxString tH = wxString::FromUTF8(t_h);
      hCtrl->SetValue(tH);
    }
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
  wmsSizer->Add(buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxButton *wms_ok = new wxButton(this, ID_WMS_OK, wxT("&Apply Changes"));
  buttonSizer->Add(wms_ok, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *wms_quit = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  buttonSizer->Add(wms_quit, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// appends event handlers
  Connect(ID_WMS_CRS, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & WmsLayerConfigDialog::OnCrsChanged);
  Connect(ID_WMS_VERSION, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & WmsLayerConfigDialog::OnVersionChanged);
  Connect(ID_WMS_SWAP, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & WmsLayerConfigDialog::OnSwapXYChanged);
  Connect(ID_WMS_CACHED, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & WmsLayerConfigDialog::OnCachedChanged);
  Connect(ID_WMS_TILED, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & WmsLayerConfigDialog::OnTiledChanged);
  Connect(ID_WMS_ENABLE_BGCOLOR, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          WmsLayerConfigDialog::OnBgColorEnabledChanged);
  Connect(ID_WMS_BGCOLOR, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsLayerConfigDialog::OnBgColorChanged);
  Connect(ID_WMS_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsLayerConfigDialog::OnWmsOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsLayerConfigDialog::OnQuit);
}

unsigned char WmsLayerConfigDialog::ParseHex(const char *byte)
{
// parsing an Hex Byte
  unsigned char value = 0;
  switch (byte[0])
    {
      case '0':
        value = 16 * 0;
        break;
      case '1':
        value = 16 * 1;
        break;
      case '2':
        value = 16 * 2;
        break;
      case '3':
        value = 16 * 3;
        break;
      case '4':
        value = 16 * 4;
        break;
      case '5':
        value = 16 * 5;
        break;
      case '6':
        value = 16 * 6;
        break;
      case '7':
        value = 16 * 7;
        break;
      case '8':
        value = 16 * 8;
        break;
      case '9':
        value = 16 * 9;
        break;
      case 'A':
      case 'a':
        value = 16 * 10;
        break;
      case 'B':
      case 'b':
        value = 16 * 11;
        break;
      case 'C':
      case 'c':
        value = 16 * 12;
        break;
      case 'D':
      case 'd':
        value = 16 * 13;
        break;
      case 'E':
      case 'e':
        value = 16 * 14;
        break;
      case 'F':
      case 'f':
        value = 16 * 15;
        break;
    };
  switch (byte[1])
    {
      case '0':
        value += 0;
        break;
      case '1':
        value += 1;
        break;
      case '2':
        value += 2;
        break;
      case '3':
        value += 3;
        break;
      case '4':
        value += 4;
        break;
      case '5':
        value += 5;
        break;
      case '6':
        value += 6;
        break;
      case '7':
        value += 7;
        break;
      case '8':
        value += 8;
        break;
      case '9':
        value += 9;
        break;
      case 'A':
      case 'a':
        value += 10;
        break;
      case 'B':
      case 'b':
        value += 11;
        break;
      case 'C':
      case 'c':
        value += 12;
        break;
      case 'D':
      case 'd':
        value += 13;
        break;
      case 'E':
      case 'e':
        value += 14;
        break;
      case 'F':
      case 'f':
        value += 15;
        break;
    };
  return value;
}

void WmsLayerConfigDialog::ParseBgColor(const char *color, unsigned char *red,
                                        unsigned char *green,
                                        unsigned char *blue)
{
// parsing an Hex RGB color
  char byte[2];
  *red = 0x80;
  *green = 0x80;
  *blue = 0x80;
  if (color == NULL)
    return;

  byte[0] = *(color + 0);
  byte[1] = *(color + 1);
  *red = ParseHex(byte);
  byte[0] = *(color + 2);
  byte[1] = *(color + 3);
  *green = ParseHex(byte);
  byte[0] = *(color + 4);
  byte[1] = *(color + 5);
  *blue = ParseHex(byte);
}

void WmsLayerConfigDialog::GetButtonBitmap(const char *bgcolor, wxBitmap & bmp)
{
// creating a Bitmap representing some Color
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  ParseBgColor(bgcolor, &red, &green, &blue);
  wxColour color = wxColour(red, green, blue);
  bmp = wxBitmap(64, 24);
  wxMemoryDC *dc = new wxMemoryDC(bmp);
  dc->SetBrush(wxBrush(color));
  dc->DrawRectangle(-1, -1, 66, 26);
  delete dc;
}

void WmsLayerConfigDialog::InitData()
{
// initializing the current configuration
  int len;
  WmsLayerSettings *config = Layer->GetWmsConfig();
  Version = NULL;
  if (config->GetVersion() != NULL)
    {
      len = strlen(config->GetVersion());
      Version = (char *) malloc(len + 1);
      strcpy(Version, config->GetVersion());
    }
  RefSys = NULL;
  if (config->GetRefSys() != NULL)
    {
      len = strlen(config->GetRefSys());
      RefSys = (char *) malloc(len + 1);
      strcpy(RefSys, config->GetRefSys());
    }
  ImageFormat = NULL;
  if (config->GetImageFormat() != NULL)
    {
      len = strlen(config->GetImageFormat());
      ImageFormat = (char *) malloc(len + 1);
      strcpy(ImageFormat, config->GetImageFormat());
    }
  Style = NULL;
  if (config->GetStyle() != NULL)
    {
      len = strlen(config->GetStyle());
      Style = (char *) malloc(len + 1);
      strcpy(Style, config->GetStyle());
    }
  Opaque = config->IsOpaque();
  SwapXY = config->IsSwapXY();
  Cached = config->IsCached();
  Tiled = config->IsTiled();
  TileWidth = config->GetTileWidth();
  TileHeight = config->GetTileHeight();
  BgColor = NULL;
  BgColorEnabled = false;
  if (config->GetBgColor() != NULL)
    {
      len = strlen(config->GetBgColor());
      BgColor = (char *) malloc(len + 1);
      strcpy(BgColor, config->GetBgColor());
      BgColorEnabled = config->IsBgColorEnabled();
    }
}

void WmsLayerConfigDialog::LoadData()
{
// loading the current configuration
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT version, srs, format, style, is_queryable, flip_axes, "
     "is_cached, tiled, tile_width, tile_height, bgcolor, transparent "
     "FROM \"%s\".wms_getmap WHERE url = %Q AND layer_name = %Q",
     xdb_prefix, url, layer);
  free(url);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      Version = NULL;
      RefSys = NULL;
      ImageFormat = NULL;
      Style = NULL;
      Opaque = 0;
      SwapXY = 0;
      Cached = 0;
      Tiled = 0;
      TileWidth = 0;
      TileHeight = 0;
      BgColor = NULL;
      return;
    }

  for (int i = 1; i <= rows; i++)
    {
      // fetching data
      int len;
      const char *version = results[(i * columns) + 0];
      const char *ref_sys = results[(i * columns) + 1];
      const char *format = results[(i * columns) + 2];
      const char *style = results[(i * columns) + 3];
      if (atoi(results[(i * columns) + 5]) == 0)
        SwapXY = 0;
      else
        SwapXY = 1;
      if (atoi(results[(i * columns) + 6]) == 0)
        Cached = 0;
      else
        Cached = 1;
      if (atoi(results[(i * columns) + 7]) == 0)
        Tiled = 0;
      else
        Tiled = 1;
      TileWidth = atoi(results[(i * columns) + 8]);
      TileHeight = atoi(results[(i * columns) + 9]);
      const char *bgcolor = results[(i * columns) + 10];
      if (atoi(results[(i * columns) + 11]) == 0)
        Opaque = 1;
      else
        Opaque = 0;
      if (version == NULL)
        Version = NULL;
      else
        {
          len = strlen(version);
          Version = (char *) malloc(len + 1);
          strcpy(Version, version);
        }
      if (ref_sys == NULL)
        RefSys = NULL;
      else
        {
          len = strlen(ref_sys);
          RefSys = (char *) malloc(len + 1);
          strcpy(RefSys, ref_sys);
        }
      if (format == NULL)
        ImageFormat = NULL;
      else
        {
          len = strlen(format);
          ImageFormat = (char *) malloc(len + 1);
          strcpy(ImageFormat, format);
        }
      if (style == NULL)
        Style = NULL;
      else
        {
          len = strlen(style);
          Style = (char *) malloc(len + 1);
          strcpy(Style, style);
        }
      if (bgcolor == NULL)
        BgColor = NULL;
      else
        {
          if (strlen(bgcolor) == 6)
            {
              BgColor = (char *) malloc(7);
              strcpy(BgColor, bgcolor);
          } else
            BgColor = NULL;
        }
    }
  sqlite3_free_table(results);
}

void WmsLayerConfigDialog::FindMaxVersion()
{
// identifying the Max WMS Version
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  MaxVersion = NULL;

  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql = sqlite3_mprintf("SELECT Max(s.value) FROM \"%s\".wms_getmap AS m "
                        "JOIN \"%s\".wms_settings AS s ON (s.parent_id = m.id) "
                        "WHERE m.url = %Q AND m.layer_name = %Q AND s.key = 'version'",
                        xdb_prefix, xdb_prefix, url, layer);
  free(url);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // fetching data
      int len;
      const char *version = results[(i * columns) + 0];
      if (version == NULL)
        MaxVersion = NULL;
      else
        {
          len = strlen(version);
          MaxVersion = (char *) malloc(len + 1);
          strcpy(MaxVersion, version);
        }
    }
  sqlite3_free_table(results);
}

void WmsLayerConfigDialog::PopulateRefSys(wxComboBox * crsList)
{
// populating the RefSys List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql = sqlite3_mprintf("SELECT s.srs FROM \"%s\".wms_getmap AS m "
                        "JOIN \"%s\".wms_ref_sys AS s ON (s.parent_id = m.id) "
                        "WHERE m.url = %Q AND m.layer_name = %Q ORDER BY s.id",
                        xdb_prefix, xdb_prefix, url, layer);
  free(url);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *crs = results[(i * columns) + 0];
      wxString str = wxString::FromUTF8(crs);
      crsList->Append(str);
      if (strcmp(RefSys, crs) == 0)
        crsList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

void WmsLayerConfigDialog::PopulateStyles(wxComboBox * stlList)
{
// populating the Styles List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql = sqlite3_mprintf("SELECT s.value FROM \"%s\".wms_getmap AS m "
                        "JOIN \"%s\".wms_settings AS s ON (s.parent_id = m.id) "
                        "WHERE m.url = %Q AND m.layer_name = %Q AND s.key = 'style' ORDER BY s.id",
                        xdb_prefix, xdb_prefix, url, layer);
  free(url);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *style = results[(i * columns) + 0];
      wxString str = wxString::FromUTF8(style);
      stlList->Append(str);
      if (strcmp(Style, style) == 0)
        stlList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

void WmsLayerConfigDialog::PopulateImageFormats(wxComboBox * fmtList)
{
// populating the Image Formats List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql = sqlite3_mprintf("SELECT s.value FROM \"%s\".wms_getmap AS m "
                        "JOIN \"%s\".wms_settings AS s ON (s.parent_id = m.id) "
                        "WHERE m.url = %Q AND m.layer_name = %Q AND s.key = 'format' ORDER BY s.id",
                        xdb_prefix, xdb_prefix, url, layer);
  free(url);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *format = results[(i * columns) + 0];
      wxString str = wxString::FromUTF8(format);
      fmtList->Append(str);
      if (strcmp(ImageFormat, format) == 0)
        fmtList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

void WmsLayerConfigDialog::OnCrsChanged(wxCommandEvent & WXUNUSED(event))
{
//
// CRS changed: updating SWAP XY
//
  UpdateSwapXY();
}

void WmsLayerConfigDialog::OnVersionChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Version changed: updating SWAP XY
//
  UpdateSwapXY();
}

void WmsLayerConfigDialog::UpdateSwapXY()
{
//
// updating SWAP XY
//
  wxCheckBox *swapBox = (wxCheckBox *) FindWindow(ID_WMS_SWAP);
  wxRadioBox *versionBox = (wxRadioBox *) FindWindow(ID_WMS_VERSION);
  wxComboBox *comboCtrl = (wxComboBox *) FindWindow(ID_WMS_CRS);
  if (versionBox->GetSelection() != 3)
    {
      swapBox->SetValue(false);
      SwapXY = 0;
      return;
    };
  wxString value = comboCtrl->GetValue();
  char *crs = new char[value.Len() + 1];
  strcpy(crs, value.ToUTF8());
  if (MainFrame->HasFlippedAxes(crs) == true)
    {
      swapBox->SetValue(true);
      SwapXY = 1;
  } else
    {
      swapBox->SetValue(false);
      SwapXY = 0;
    }
  delete[]crs;
}

void WmsLayerConfigDialog::OnSwapXYChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Swap XY selection changed
//
  if (SwapXY == 0)
    SwapXY = 1;
  else
    SwapXY = 0;
}

void WmsLayerConfigDialog::OnCachedChanged(wxCommandEvent & WXUNUSED(event))
{
//
// WMS Cached selection changed
//
  if (Cached == 0)
    Cached = 1;
  else
    Cached = 0;
}

void WmsLayerConfigDialog::
OnBgColorEnabledChanged(wxCommandEvent & WXUNUSED(event))
{
//
// BgColor Enabled selection changed
//
  wxBitmapButton *btn = (wxBitmapButton *) FindWindow(ID_WMS_BGCOLOR);
  if (BgColorEnabled == false)
    {
      if (BgColor != NULL)
        free(BgColor);
      BgColor = (char *) malloc(7);
      strcpy(BgColor, "ffffff");
      wxBitmap bmp;
      GetButtonBitmap(BgColor, bmp);
      btn->SetBitmapLabel(bmp);
      BgColorEnabled = true;
      btn->Enable(true);
  } else
    {
      BgColorEnabled = false;
      btn->Enable(false);
      if (BgColor != NULL)
        {
          free(BgColor);
          BgColor = NULL;
        }
    }
}

void WmsLayerConfigDialog::OnBgColorChanged(wxCommandEvent & WXUNUSED(event))
{
// BgColor selection
  wxColourData initColor;
  int ret;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  ParseBgColor(BgColor, &red, &green, &blue);
  wxColour color = wxColour(red, green, blue);
  initColor.SetChooseFull(false);
  initColor.SetColour(color);
  wxColourDialog colorDialog(this, &initColor);
  ret = colorDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxColourData colorData = colorDialog.GetColourData();
      color = colorData.GetColour();
      char byte[3];
      sprintf(byte, "%02x", color.Red());
      memcpy(BgColor, byte, 2);
      sprintf(byte, "%02x", color.Green());
      memcpy(BgColor + 2, byte, 2);
      sprintf(byte, "%02x", color.Blue());
      memcpy(BgColor + 4, byte, 2);
      *(BgColor + 6) = '\0';
      wxBitmap bmp;
      GetButtonBitmap(BgColor, bmp);
      wxBitmapButton *bgColorCtrl =
        (wxBitmapButton *) FindWindow(ID_WMS_BGCOLOR);
      bgColorCtrl->SetBitmapLabel(bmp);
    }
}

void WmsLayerConfigDialog::OnTiledChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Monolithic / Tiled selection changed
//
  wxCheckBox *tiledCtrl = (wxCheckBox *) FindWindow(ID_WMS_TILED);
  wxSpinCtrl *wCtrl = (wxSpinCtrl *) FindWindow(ID_WMS_WIDTH);
  wxSpinCtrl *hCtrl = (wxSpinCtrl *) FindWindow(ID_WMS_HEIGHT);
  if (tiledCtrl->GetValue() == false)
    {
      wCtrl->SetRange(0, 0);
      wCtrl->SetValue(wxT(""));
      wCtrl->Enable(false);
      hCtrl->SetRange(0, 0);
      hCtrl->SetValue(wxT(""));
      hCtrl->Enable(false);
  } else
    {
      wCtrl->SetRange(256, 5000);
      wCtrl->SetValue(wxT("512"));
      wCtrl->Enable(true);
      hCtrl->SetRange(256, 5000);
      hCtrl->SetValue(wxT("512"));
      hCtrl->Enable(true);
    }
}

void WmsLayerConfigDialog::DoConfigureWmsLayer()
{
// will try updating the given WMS Layer definitions
  int ret;
  sqlite3_stmt *stmt = NULL;
  char *dummy;
  bool error = false;

// updating Opaque and Flip Axes
  const char *sql = "SELECT WMS_SetGetMapOptions(?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerOptions: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(dummy, Url.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (Opaque == true)
    sqlite3_bind_int(stmt, 3, 0);
  else
    sqlite3_bind_int(stmt, 3, 1);
  if (SwapXY == true)
    sqlite3_bind_int(stmt, 4, 1);
  else
    sqlite3_bind_int(stmt, 4, 0);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerOptions ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error == true)
    return;

// updating Tiled and Tile Size
  sql = "SELECT WMS_SetGetMapOptions(?, ?, ?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerOptions: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(dummy, Url.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_int(stmt, 3, Tiled);
  sqlite3_bind_int(stmt, 4, Cached);
  sqlite3_bind_int(stmt, 5, TileWidth);
  sqlite3_bind_int(stmt, 6, TileHeight);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerOptions ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error == true)
    return;

// updating Background Color
  sql = "SELECT WMS_SetGetMapOptions(?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerOptions: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(dummy, Url.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (BgColor == NULL || BgColorEnabled == false)
    sqlite3_bind_null(stmt, 3);
  sqlite3_bind_text(stmt, 3, BgColor, 6, SQLITE_TRANSIENT);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetWmsLayerOptions ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error == true)
    return;

// updating the Version default setting
  sql = "SELECT WMS_DefaultSetting(?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - WmsLayerDefaultSetting: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(dummy, Url.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_text(stmt, 3, "version", -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, Version, -1, SQLITE_TRANSIENT);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - WmsLayerDefaultSetting: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error == true)
    return;

// updating the ImageFormat default setting
  sql = "SELECT WMS_DefaultSetting(?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - WmsLayerDefaultSetting: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(dummy, Url.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_text(stmt, 3, "format", -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, ImageFormat, -1, SQLITE_TRANSIENT);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - WmsLayerDefaultSetting: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error == true)
    return;

// updating the Style default setting
  sql = "SELECT WMS_DefaultSetting(?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - WmsLayerDefaultSetting: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(dummy, Url.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_text(stmt, 3, "style", -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, Style, -1, SQLITE_TRANSIENT);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - WmsLayerDefaultSetting: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error == true)
    return;

// updating the SRS default setting
  sql = "SELECT WMS_DefaultRefSys(?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - WmsDefaultRefSys: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(dummy, Url.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(dummy, LayerName.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  sqlite3_bind_text(stmt, 3, RefSys, -1, SQLITE_TRANSIENT);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - WmsDefaultRefSys: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
}

void WmsLayerConfigDialog::DoConfigureMapLayer()
{
// updating the Map Layer definitions
  WmsLayerSettings *config = Layer->GetWmsConfig();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (strcmp(Version, config->GetVersion()) != 0)
    {
      config->SetVersion(Version);
      IsConfigChanged = true;
    }
  if (strcmp(RefSys, config->GetRefSys()) != 0)
    {
      config->SetRefSys(RefSys);
      IsConfigChanged = true;
    }
  if (strcmp(Style, config->GetStyle()) != 0)
    {
      config->SetStyle(Style);
      IsConfigChanged = true;
    }
  if (strcmp(ImageFormat, config->GetImageFormat()) != 0)
    {
      config->SetImageFormat(ImageFormat);
      IsConfigChanged = true;
    }
  if ((BgColor == NULL && config->GetBgColor() != NULL)
      || (BgColor != NULL && config->GetBgColor() == NULL))
    {
      config->SetBgColor(BgColor);
      IsConfigChanged = true;
  } else if (BgColor == NULL && config->GetBgColor() == NULL)
    ;
  else
    {
      if (strcmp(BgColor, config->GetBgColor()) != 0)
        {
          config->SetBgColor(BgColor);
          IsConfigChanged = true;
        }
    }
  if (Opaque != config->IsOpaque())
    {
      config->SetOpaque(Opaque);
      IsConfigChanged = true;
    }
  if (SwapXY != config->IsSwapXY())
    {
      config->SetSwapXY(SwapXY);
      IsConfigChanged = true;
    }
  if (Cached != config->IsCached())
    {
      config->SetCached(Cached);
      IsConfigChanged = true;
    }
  if (Tiled != config->IsTiled())
    {
      config->SetTiled(Tiled);
      IsConfigChanged = true;
    }
  if (TileWidth != config->GetTileWidth())
    {
      config->SetTileWidth(TileWidth);
      IsConfigChanged = true;
    }
  if (TileHeight != config->GetTileHeight())
    {
      config->SetTileHeight(TileHeight);
      IsConfigChanged = true;
    }
}

void WmsLayerConfigDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void WmsLayerConfigDialog::OnWmsOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxRadioBox *versionBox = (wxRadioBox *) FindWindow(ID_WMS_VERSION);
  wxComboBox *crsList = (wxComboBox *) FindWindow(ID_WMS_CRS);
  wxComboBox *stlList = (wxComboBox *) FindWindow(ID_WMS_STYLE);
  wxComboBox *fmtList = (wxComboBox *) FindWindow(ID_WMS_FORMAT);
  wxCheckBox *enableBgColorCtrl =
    (wxCheckBox *) FindWindow(ID_WMS_ENABLE_BGCOLOR);
  wxRadioBox *transBox = (wxRadioBox *) FindWindow(ID_WMS_TRANSPARENT);
  wxCheckBox *swapCtrl = (wxCheckBox *) FindWindow(ID_WMS_SWAP);
  wxRadioBox *cachedBox = (wxRadioBox *) FindWindow(ID_WMS_CACHED);
  wxCheckBox *tiledCtrl = (wxCheckBox *) FindWindow(ID_WMS_TILED);
  wxSpinCtrl *wCtrl = (wxSpinCtrl *) FindWindow(ID_WMS_WIDTH);
  wxSpinCtrl *hCtrl = (wxSpinCtrl *) FindWindow(ID_WMS_HEIGHT);

  if (Version != NULL)
    free(Version);
  Version = (char *) malloc(6);
  switch (versionBox->GetSelection())
    {
      case 0:
        strcpy(Version, "1.0.0");
        break;
      case 1:
        strcpy(Version, "1.1.0");
        break;
      case 2:
        strcpy(Version, "1.1.1");
        break;
      default:
        strcpy(Version, "1.3.0");
        break;
    };
  wxString crs = crsList->GetValue();
  if (RefSys != NULL)
    free(RefSys);
  RefSys = (char *) malloc((crs.Len() * 4) + 1);
  strcpy(RefSys, crs.ToUTF8());
  wxString stl = stlList->GetValue();
  if (Style != NULL)
    free(Style);
  Style = (char *) malloc((stl.Len() * 4) + 1);
  strcpy(Style, stl.ToUTF8());
  wxString fmt = fmtList->GetValue();
  if (ImageFormat != NULL)
    free(ImageFormat);
  ImageFormat = (char *) malloc((fmt.Len() * 4) + 1);
  strcpy(ImageFormat, fmt.ToUTF8());
  BgColorEnabled = enableBgColorCtrl->GetValue();
  if (transBox->GetSelection() == 1)
    Opaque = 1;
  else
    Opaque = 0;
  if (swapCtrl->GetValue() == true)
    SwapXY = 1;
  else
    SwapXY = 0;
  if (cachedBox->GetSelection() == 0)
    Cached = 1;
  else
    Cached = 0;
  if (tiledCtrl->GetValue() == true)
    Tiled = 1;
  else
    Tiled = 0;
  TileWidth = wCtrl->GetValue();
  TileHeight = hCtrl->GetValue();
  if (Layer != NULL)
    DoConfigureMapLayer();
  else
    DoConfigureWmsLayer();
  wxDialog::EndModal(wxID_OK);
}

bool MyFrame::HasFlippedAxes(const char *crs)
{
// testing if some CRS requires swapped axes 
  int len;
  int i;
  int srid = -1;
  int flipped = 0;
  if (crs == NULL)
    return false;
  if (strcmp(crs, "CRS:84") == 0)
    return false;
  len = strlen(crs);
  for (i = 0; i < len; i++)
    {
      if (*(crs + i) == ':')
        {
          // delimiter found
          if (i == 4 && memcmp(crs, "EPSG", 4) == 0)
            {
              srid = atoi(crs + i + 1);
              break;
            }
        }
    }
  if (!srid_has_flipped_axes(SqliteHandle, srid, &flipped))
    flipped = 0;
  if (flipped)
    return true;
  return false;
}

bool MyFrame::BBoxFromLongLat(const char *crs, double *minx, double *maxx,
                              double *miny, double *maxy)
{
// converting a BBox into some CRS from Long/Lat coords
  int srid = -1;
  char *proj_from;
  char *proj_to;
  gaiaGeomCollPtr g_min_in;
  gaiaGeomCollPtr g_min_out;
  gaiaGeomCollPtr g_max_in;
  gaiaGeomCollPtr g_max_out;
  gaiaPointPtr pt;
  if (crs == NULL)
    return false;
  if (strcmp(crs, "CRS:84") == 0)
    srid = 4326;
  else
    {
      int i;
      int len = strlen(crs);
      for (i = 0; i < len; i++)
        {
          if (*(crs + i) == ':')
            {
              // delimiter found
              if (i == 4 && memcmp(crs, "EPSG", 4) == 0)
                {
                  srid = atoi(crs + i + 1);
                  break;
                }
            }
        }
    }
  if (srid <= 0)
    return false;

// attempting to reproject from Long/Lat into the given CRS
  proj_from = GetProjParams(4326);
  proj_to = GetProjParams(srid);
  if (proj_to == NULL || proj_from == NULL)
    {
      if (proj_from)
        free(proj_from);
      if (proj_to)
        free(proj_to);
      return false;
    }

  g_min_in = gaiaAllocGeomColl();
  gaiaAddPointToGeomColl(g_min_in, *minx, *miny);
  g_max_in = gaiaAllocGeomColl();
  gaiaAddPointToGeomColl(g_max_in, *maxx, *maxy);
  g_min_out = gaiaTransform(g_min_in, proj_from, proj_to);
  g_max_out = gaiaTransform(g_max_in, proj_from, proj_to);
  free(proj_from);
  free(proj_to);
  gaiaFreeGeomColl(g_min_in);
  gaiaFreeGeomColl(g_max_in);
  pt = g_min_out->FirstPoint;
  *minx = pt->X;
  *miny = pt->Y;
  gaiaFreeGeomColl(g_min_out);
  pt = g_max_out->FirstPoint;
  *maxx = pt->X;
  *maxy = pt->Y;
  gaiaFreeGeomColl(g_max_out);
  return true;
}

char *MyFrame::GetProjParams(int srid)
{
// attempting to retrieve PROJ4 geodetic definitions
  sqlite3 *sqlite = SqliteHandle;
  char *sql;
  char **results;
  int rows;
  int columns;
  int i;
  int ret;
  int len;
  const char *proj4text;
  char *proj_params = NULL;
  char *errMsg = NULL;
  sql = sqlite3_mprintf
    ("SELECT proj4text FROM spatial_ref_sys WHERE srid = %d", srid);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      sqlite3_free(errMsg);
      return NULL;
    }
  for (i = 1; i <= rows; i++)
    {
      proj4text = results[(i * columns)];
      if (proj4text != NULL)
        {
          len = strlen(proj4text);
          proj_params = (char *) malloc(len + 1);
          strcpy(proj_params, proj4text);
        }
    }
  sqlite3_free_table(results);
  return proj_params;
}

void MyFrame::CreateWmsTables()
{
// just in case, attempting to (re)create WMS support tables
  sqlite3_exec(SqliteHandle, "SELECT WMS_CreateTables()", NULL, NULL, NULL);
}

bool MyFrame::IsDefinedWmsGetCapabilities(const char *url)
{
// checking if a WMS GetCapabilities already exists
  int ret;
  char *err_msg = NULL;
  char *sql;
  char **results;
  int rows;
  int columns;
  bool exists = false;

  sql =
    sqlite3_mprintf
    ("SELECT Count(*) FROM MAIN.wms_getcapabilities WHERE url = %Q", url);
  ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, &err_msg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("Check WMS GetCapabilities SQL error: ") +
                   wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(err_msg);
      return false;
    }
  if (rows != 1 || columns != 1)
    return false;
  if (atoi(*(results + 1)) != 0)
    exists = true;
  sqlite3_free_table(results);
  return exists;
}

bool MyFrame::IsDefinedWmsGetMap(const char *getmap_url, const char *layer_name)
{
// checking if a WMS GetMap already exists
  int ret;
  char *err_msg = NULL;
  char *sql;
  char **results;
  int rows;
  int columns;
  bool exists = false;

  sql =
    sqlite3_mprintf
    ("SELECT Count(*) FROM MAIN.wms_getmap WHERE url = %Q AND layer_name = %Q",
     getmap_url, layer_name);
  ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, &err_msg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("Check WMS GetMap SQL error: ") +
                   wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(err_msg);
      return false;
    }
  if (rows != 1 || columns != 1)
    return false;
  if (atoi(*(results + 1)) != 0)
    exists = true;
  sqlite3_free_table(results);
  return exists;
}

bool MyFrame::RegisterWmsGetCapabilities(const char *url, const char *title,
                                         const char *abstract)
{
// attempting to (re)register a WMS GetCapabilities
  int ret;
  char *err_msg = NULL;
  char *sql;
  char **results;
  int rows;
  int columns;

  if (title == NULL && abstract == NULL)
    sql = sqlite3_mprintf("SELECT WMS_RegisterGetCapabilities(%Q)", url);
  else
    {
      if (title == NULL)
        title = "";
      if (abstract == NULL)
        abstract = "";
      sql =
        sqlite3_mprintf("SELECT WMS_RegisterGetCapabilities(%Q, %Q, %Q)", url,
                        title, abstract);
    }
  ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, &err_msg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("WMS_RegisterGetCapabilities SQL error: ") +
                   wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(err_msg);
      return false;
    }
  if (rows != 1 || columns != 1)
    {
      char dummy[80];
      sprintf(dummy, "(rows=%d columns=%d)", rows, columns);
      wxMessageBox(wxT("WMS_RegisterGetCapabilities invalid result: ") +
                   wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free_table(results);
      return false;
    }
  if (atoi(*(results + 1)) != 1)
    {
      char dummy[80];
      sprintf(dummy, "(%d)", atoi(*(results + 1)));
      wxMessageBox(wxT("WMS_RegisterGetCapabilities invalid result: ") +
                   wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free_table(results);
      return false;
    }
  sqlite3_free_table(results);
  return true;
}

bool MyFrame::RegisterWmsGetMap(const char *getcapabilities_url,
                                const char *getmap_url, const char *layer_name,
                                const char *title, const char *abstract,
                                const char *version, const char *ref_sys,
                                const char *image_format, const char *style,
                                int transparent, int flip_axes,
                                int is_queryable, int tiled, int cached,
                                int tile_width, int tile_height,
                                const char *bgcolor, const char *getfeatureinfo)
{
// attempting to register a WMS GetMap
  int ret;
  char *err_msg = NULL;
  char *sql;
  char **results;
  int rows;
  int columns;
  char *xbgcolor;
  char *xgetfeatureinfo;

  if (style == NULL)
    style = "";
  if (title == NULL)
    title = "";
  if (abstract == NULL)
    abstract = "";
  if (bgcolor == NULL)
    xbgcolor = sqlite3_mprintf("NULL");
  else
    xbgcolor = sqlite3_mprintf("%Q", bgcolor);
  if (getfeatureinfo == NULL)
    xgetfeatureinfo = sqlite3_mprintf("NULL");
  else
    xgetfeatureinfo = sqlite3_mprintf("%Q", getfeatureinfo);
  sql =
    sqlite3_mprintf
    ("SELECT WMS_RegisterGetMap(%Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %d, %d, %d, %d, %d, %d, %s, %d, %s)",
     getcapabilities_url, getmap_url, layer_name, title, abstract,
     version, ref_sys, image_format, style, transparent, flip_axes,
     tiled, cached, tile_width, tile_height, xbgcolor, is_queryable,
     xgetfeatureinfo);
  sqlite3_free(xbgcolor);
  sqlite3_free(xgetfeatureinfo);
  ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, &err_msg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("WMS_RegisterGetMap SQL error: ") +
                   wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(err_msg);
      return false;
    }
  if (rows != 1 || columns != 1)
    {
      char dummy[80];
      sprintf(dummy, "(rows=%d columns=%d)", rows, columns);
      wxMessageBox(wxT("WMS_RegisterGetMap invalid result: ") +
                   wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free_table(results);
      return false;
    }
  if (atoi(*(results + 1)) != 1)
    {
      char dummy[80];
      sprintf(dummy, "(%d)", atoi(*(results + 1)));
      wxMessageBox(wxT("WMS_RegisterGetMap invalid result: ") +
                   wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free_table(results);
      return false;
    }
  sqlite3_free_table(results);
  return true;
}

bool MyFrame::RegisterWmsSetting(const char *url, const char *layer_name,
                                 const char *key, const char *value,
                                 int is_default)
{
// attempting to register a WMS GetMap Setting
  char *sql;
  char *errMsg = NULL;
  int ret;
  sql = sqlite3_mprintf("SELECT WMS_RegisterSetting(%Q, %Q, %Q, %Q, %d)",
                        url, layer_name, key, value, is_default);
  ret = sqlite3_exec(SqliteHandle, sql, NULL, NULL, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("Register WMS GetMap Setting error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  return true;
}

bool MyFrame::RegisterWmsSRS(const char *url, const char *layer_name,
                             const char *ref_sys, double minx, double miny,
                             double maxx, double maxy, int is_default)
{
// attempting to register a WMS GetMap SRS
  char *sql;
  char *errMsg = NULL;
  int ret;
  sql =
    sqlite3_mprintf
    ("SELECT WMS_RegisterRefSys(%Q, %Q, %Q, %1.8f, %1.8f, %1.8f, %1.8f, %d)",
     url, layer_name, ref_sys, minx, miny, maxx, maxy, is_default);
  ret = sqlite3_exec(SqliteHandle, sql, NULL, NULL, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("Register WMS GetMap RefSys error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  return true;
}

bool MyFrame::TransformWmsBBox(const char *ref_sys, double geoMinX,
                               double geoMinY, double geoMaxX, double geoMaxY,
                               double *minx, double *miny, double *maxx,
                               double *maxy)
{
// tranforming a WMS BBOX from long/lat
  int srid = -1;
  char *proj_from;
  char *proj_to;
  gaiaGeomCollPtr g_min_in;
  gaiaGeomCollPtr g_min_out;
  gaiaGeomCollPtr g_max_in;
  gaiaGeomCollPtr g_max_out;
  gaiaPointPtr pt;

  if (strcasecmp(ref_sys, "CRS:84") == 0)
    {
      *minx = geoMinX;
      *miny = geoMinY;
      *maxx = geoMaxX;
      *maxy = geoMaxY;
      return true;
    }

  if (strncmp(ref_sys, "EPSG:", 5) == 0)
    srid = atoi(ref_sys + 5);
  if (srid <= 0)
    return false;
  if (srid == 4326)
    {
      *minx = geoMinX;
      *miny = geoMinY;
      *maxx = geoMaxX;
      *maxy = geoMaxY;
      return true;
    }
// attempting to reproject from Long/Lat into the given CRS
  proj_from = GetProjParams(4326);
  proj_to = GetProjParams(srid);
  if (proj_to == NULL || proj_from == NULL)
    {
      if (proj_from)
        free(proj_from);
      if (proj_to)
        free(proj_to);
      return false;
    }

  g_min_in = gaiaAllocGeomColl();
  gaiaAddPointToGeomColl(g_min_in, geoMinX, geoMinY);
  g_max_in = gaiaAllocGeomColl();
  gaiaAddPointToGeomColl(g_max_in, geoMaxX, geoMaxY);
  g_min_out = gaiaTransform(g_min_in, proj_from, proj_to);
  g_max_out = gaiaTransform(g_max_in, proj_from, proj_to);
  free(proj_from);
  free(proj_to);
  gaiaFreeGeomColl(g_min_in);
  gaiaFreeGeomColl(g_max_in);
  pt = g_min_out->FirstPoint;
  *minx = pt->X;
  *miny = pt->Y;
  gaiaFreeGeomColl(g_min_out);
  pt = g_max_out->FirstPoint;
  *maxx = pt->X;
  *maxy = pt->Y;
  gaiaFreeGeomColl(g_max_out);
  return true;
}

void MyFrame::DoRegisterWMS()
{
//
// registering a new WMS Layer
//
  WmsDialog dlg;
  dlg.Create(this, HttpProxy);
  int ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      wxString urlStr = dlg.GetURL();
      char *url = (char *) malloc((urlStr.Len() * 4) + 1);
      strcpy(url, urlStr.ToUTF8());
      int tiled = dlg.IsTiled();
      int tileWidth = dlg.GetTileWidth();
      int tileHeight = dlg.GetTileHeight();
      int transparent = 1;
      if (dlg.IsOpaque())
        transparent = 0;
      const char *serviceTitle = dlg.GetServiceTitle();
      const char *serviceAbstract = dlg.GetServiceAbstract();
      const char *getMapUrl = dlg.GetURL_GetMap_Get();
      const char *layerName = dlg.GetName();
      const char *title = dlg.GetTitle();
      const char *abstract = dlg.GetAbstract();
      const char *version = dlg.GetVersion();
      const char *style = dlg.GetStyleName();
      const char *imageFormat = dlg.GetFormat();
      const char *refSys = dlg.GetCRS();
      const char *getFeatureInfo = dlg.GetURL_GetFeatureInfo_Get();
      int flipAxes = dlg.IsSwapXY();
      int isQueryable = dlg.IsQueryable();

      CreateWmsTables();
      if (IsDefinedWmsGetMap(getMapUrl, layerName) == true)
        {
          wxMessageBox(wxT
                       ("WMS_RegisterGetMap: already registered WMS layer: "),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
      if (IsDefinedWmsGetCapabilities(url) == false)
        {
          if (!RegisterWmsGetCapabilities(url, serviceTitle, serviceAbstract))
            {
              free(url);
              return;
            }
        }
      if (!RegisterWmsGetMap
          (url, getMapUrl, layerName, title, abstract, version, refSys,
           imageFormat, style, transparent, flipAxes, isQueryable, tiled, 0,
           tileWidth, tileHeight, NULL, getFeatureInfo))
        {
          free(url);
          return;
        }
      free(url);

      if (strcmp(version, "1.3.0") == 0)
        {
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.0.0", 0))
            return;
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.1.0", 0))
            return;
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.1.1", 0))
            return;
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.3.0", 1))
            return;
        }
      if (strcmp(version, "1.1.1") == 0)
        {
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.0.0", 0))
            return;
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.1.0", 0))
            return;
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.1.1", 1))
            return;
        }
      if (strcmp(version, "1.1.0") == 0)
        {
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.0.0", 0))
            return;
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.1.0", 1))
            return;
        }
      if (strcmp(version, "1.0.0") == 0)
        {
          if (!RegisterWmsSetting(getMapUrl, layerName, "version", "1.0.0", 1))
            return;
        }

      int i;
      int cnt = dlg.CountFormats();
      for (i = 0; i < cnt; i++)
        {
          int dflt = 0;
          const char *format = dlg.GetFormatByIndex(i);
          if (strcasecmp(format, dlg.GetFormat()) == 0)
            dflt = 1;
          if (!RegisterWmsSetting(getMapUrl, layerName, "format", format, dflt))
            return;
        }
      cnt = dlg.CountStyles();
      for (i = 0; i < cnt; i++)
        {
          int dflt = 0;
          const char *style;
          const char *title;
          const char *abstract;
          if (!dlg.GetStyleByIndex(i, &style, &title, &abstract))
            return;
          if (strcasecmp(style, dlg.GetStyleName()) == 0)
            dflt = 1;
          if (!RegisterWmsSetting(getMapUrl, layerName, "style", style, dflt))
            return;
        }

      double geoMinX = dlg.GetGeoMinX();
      double geoMinY = dlg.GetGeoMinY();
      double geoMaxX = dlg.GetGeoMaxX();
      double geoMaxY = dlg.GetGeoMaxY();
      cnt = dlg.CountCRS();
      for (i = 0; i < cnt; i++)
        {
          int dflt = 0;
          bool already_defined = false;
          int j;
          const char *ref_sys = dlg.GetCrsByIndex(i);
          for (j = 0; j < i; j++)
            {
              const char *prev = dlg.GetCrsByIndex(j);
              if (strcasecmp(ref_sys, prev) == 0)
                {
                  already_defined = true;
                  break;
                }
            }
          if (already_defined)
            continue;
          if (strcasecmp(ref_sys, dlg.GetCRS()) == 0)
            dflt = 1;

          double minx;
          double miny;
          double maxx;
          double maxy;
          if (!TransformWmsBBox
              (ref_sys, geoMinX, geoMinY, geoMaxX, geoMaxY, &minx, &miny, &maxx,
               &maxy))
            continue;
          if (!RegisterWmsSRS
              (getMapUrl, layerName, ref_sys, minx, miny, maxx, maxy, dflt))
            return;
        }
    }
}

WmsCatalogTree::WmsCatalogTree(WmsDialog * parent, wxSize sz, wxWindowID id):wxTreeCtrl(parent, id, wxDefaultPosition,
           sz)
{
//
// constructor: TREE control to manage Map Layers
//
  MainDialog = parent;
  CurrentItem = wxTreeItemId();
// setting up icons 
  Images = new wxImageList(16, 16, true);
  wxIcon icons[3];
  icons[0] = wxIcon(wms_xpm);
  icons[1] = wxIcon(group_xpm);
  icons[2] = wxIcon(layer_xpm);
  Images->Add(icons[0]);
  Images->Add(icons[1]);
  Images->Add(icons[2]);
  SetImageList(Images);

// setting up event handlers 
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_SEL_CHANGED,
          (wxObjectEventFunction) & WmsCatalogTree::OnSelChanged);
}

WmsCatalogTree::~WmsCatalogTree()
{
// destructor
  if (Images)
    delete Images;
}

void WmsCatalogTree::AddLayer(rl2WmsLayerPtr handle, const char *layer)
{
//
// adds a layer to the Catalog TREE list
//
  if (Root.IsOk() == true)
    {
      AddLayer(Root, handle, layer);
      return;
    }
  int icon = 2;
  wxString name = wxString::FromUTF8(layer);
  WmsCatalogObject *obj = new WmsCatalogObject(handle, layer);
  if (wms_layer_has_children(handle))
    icon = 1;
  Root = AddRoot(name, icon);
  SetItemData(Root, (wxTreeItemData *) obj);
  if (wms_layer_has_children(handle))
    {
      // expanding Root's Children
      int nLayers = get_wms_layer_children_count(handle);
      for (int i = 0; i < nLayers; i++)
        {
          rl2WmsLayerPtr layer = get_wms_child_layer(handle, i);
          const char *x_name = get_wms_layer_title(layer);
          AddLayer(Root, layer, x_name);
        }
    }
}

void WmsCatalogTree::AddLayer(wxTreeItemId parent, rl2WmsLayerPtr handle,
                              const char *layer)
{
//
// adds a child layer to its parent
//
  wxTreeItemId item;
  int icon = 2;
  wxString name = wxString::FromUTF8(layer);
  WmsCatalogObject *obj = new WmsCatalogObject(handle, layer);
  if (wms_layer_has_children(handle))
    icon = 1;
  item = AppendItem(parent, name, icon);
  SetItemData(item, (wxTreeItemData *) obj);
  ExpandChildren(item, handle);
}

void WmsCatalogTree::AddTiledRoot(const char *layer)
{
//
// adds a tiled layer Root to the Catalog TREE list
//
  wxString name = wxString::FromUTF8(layer);
  WmsCatalogObject *obj = new WmsCatalogObject(layer);
  Root = AddRoot(name, 1);
  SetItemData(Root, (wxTreeItemData *) obj);
}

void WmsCatalogTree::OnSelChanged(wxTreeEvent & event)
{
//
// selecting some node [mouse action]
//
  wxTreeItemId item = event.GetItem();
  CurrentItem = item;
  MarkCurrentItem();
  WmsCatalogObject *obj = (WmsCatalogObject *) GetItemData(item);
  if (obj->GetHandle() != NULL)
    MainDialog->SelectLayer(obj->GetHandle());
  else
    MainDialog->SelectLayer();
}

void WmsCatalogTree::RootAutoSelect()
{
//
// auto-selecting the Root node
//
  if (Root.IsOk() == false)
    return;
  CurrentItem = Root;
  SetItemBold(Root, true);
  SelectItem(Root);
  MarkCurrentItem();
  WmsCatalogObject *obj = (WmsCatalogObject *) GetItemData(Root);
  MainDialog->SelectLayer(obj->GetHandle());
}

void WmsCatalogTree::MarkCurrentItem()
{
//
// setting the Current Item as BOLD
//
  wxTreeItemId item;
  wxTreeItemIdValue cookie;
  item = GetFirstChild(Root, cookie);
  while (item.IsOk() == true)
    {
      if (item == CurrentItem)
        SetItemBold(item, true);
      else
        SetItemBold(item, false);
      item = GetNextChild(Root, cookie);
    }
}

void WmsCatalogTree::ExpandChildren(wxTreeItemId parent, rl2WmsLayerPtr handle)
{
//
// expanding Children Layers
//
  if (parent.IsOk() == false)
    return;
  int nLayers = get_wms_layer_children_count(handle);
  for (int i = 0; i < nLayers; i++)
    {
      rl2WmsLayerPtr layer = get_wms_child_layer(handle, i);
      const char *x_name = get_wms_layer_title(layer);
      AddLayer(parent, layer, x_name);
    }
}
