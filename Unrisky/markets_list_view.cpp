#include "markets_list_view.h"

MarketsListView::MarketsListView(wxWindow* parent, wxWindowID winid, const wxPoint& pos,
    const wxSize& size) : wxListView(parent, winid, pos, size, wxLC_REPORT) {
  // TODO: Replace with InsertColumn() and more modular structure.
  AppendColumn("ID");
  AppendColumn("Name");
  AppendColumn("Profit");
}

void display(const MarketsModel& markets_model) {
  return;
}
