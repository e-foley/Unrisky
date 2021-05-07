#include "markets_list_view.h"

MarketsListView::MarketsListView(wxWindow* parent, wxWindowID winid, const wxPoint& pos,
    const wxSize& size) : wxListView(parent, winid, pos, size, wxLC_REPORT) {
  // TODO: Replace with InsertColumn() and more modular structure.
  AppendColumn("ID");
  AppendColumn("Name");
  AppendColumn("Profit");
}

void MarketsListView::display(const MarketsModel& markets_model) {
  int index = 0;
  for (const auto& market : markets_model.markets_) {
    // TODO: Replace with same modular column logic.
    InsertItem(index, std::to_string(market.id));
    SetItem(index, 1, market.name);
    SetItem(index, 2, formatFinanceString(market.risk));
    ++index;
  }
}

std::string MarketsListView::formatFinanceString(const float money) {
  char buffer[100];
  int count;
  if (money >= 0.0f) {
    count = snprintf(buffer, 100, "$%4.2f", money);
  } else {
    count = snprintf(buffer, 100, "($%4.2f)", -money);
  }

  return buffer;
}
