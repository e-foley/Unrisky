#include "derisker.h"
#include "gui.h"
#include "markets_frame.h"
#include <wx/frame.h>
#include <wx/listctrl.h>

bool UnriskyGui::OnInit() {
  MarketsFrame* frame = new MarketsFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
  
  wxPanel* panel = new wxPanel(frame, wxID_ANY);

  lv_markets_ = new MarketsListView(panel);
  lv_markets_->Show();

  frame->Show(true);
  return true;
}
