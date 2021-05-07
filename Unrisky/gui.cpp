#include "derisker.h"
#include "gui.h"
#include "markets_frame.h"
#include "markets_model.h"
#include <fstream>
#include <sstream>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>

bool UnriskyGui::OnInit() {
  MarketsFrame* frame = new MarketsFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
  wxPanel* panel = new wxPanel(frame, wxID_ANY);
  wxBoxSizer* sz_top = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer* sz_markets = new wxStaticBoxSizer(wxVERTICAL, panel, "Markets");
  sz_top->Add(sz_markets, 1, wxEXPAND | wxALL, 5);
  lv_markets_ = new MarketsListView(panel);
  sz_markets->Add(lv_markets_, 1, wxEXPAND);

  panel->Show();
  //frame->SetSizer()

  // TODO: Change this, obviously.
  std::ifstream t("all.json");
  std::stringstream buffer;
  buffer << t.rdbuf();

  std::vector<Market> markets;
  Derisker::buildMarketListJson(buffer, &markets);
  if (markets.empty()) {
    std::cout << "No markets can be found in the JSON file. (Is PredictIt down?)" << std::endl;
    return false;
  }

  Derisker::buildMarketListJson(buffer, &markets);

  MarketsModel markets_model{};

  for (const auto& market : markets) {
    markets_model.markets_.push_back(market);
  }

  lv_markets_->display(markets_model);

  panel->SetSizer(sz_top);
  frame->Center();
  frame->Show(true);
  return true;
}
