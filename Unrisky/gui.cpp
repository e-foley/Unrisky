#include "derisker.h"
#include "gui.h"
#include "markets_frame.h"
#include "markets_model.h"
#include <fstream>
#include <sstream>
#include <wx/frame.h>
#include <wx/listctrl.h>

bool UnriskyGui::OnInit() {
  MarketsFrame* frame = new MarketsFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
  
  wxPanel* panel = new wxPanel(frame, wxID_ANY);

  lv_markets_ = new MarketsListView(panel);
  lv_markets_->Show();

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

  frame->Show(true);
  return true;
}
