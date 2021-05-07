#pragma once

#include "markets_model.h"
#include <wx/listctrl.h>

class MarketsListView : public wxListView {
  public:
    MarketsListView(wxWindow* parent, wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void display(const MarketsModel& markets_model);

    static std::string formatFinanceString(float money);
};
