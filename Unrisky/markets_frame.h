#pragma once

#include <wx/frame.h>

class MarketsFrame : public wxFrame {
 public:
  MarketsFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

 private:
  void OnHello(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);

  wxDECLARE_EVENT_TABLE();
};

enum { ID_Hello = 1 };
