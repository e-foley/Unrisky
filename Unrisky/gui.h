#pragma once

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "markets_list_view.h"

class UnriskyGui : public wxApp {
  public:
    virtual bool OnInit();

  private:
    MarketsListView* lv_markets_;
};
