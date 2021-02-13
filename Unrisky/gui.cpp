#include "gui.h"
#include "markets_frame.h"
#include <wx/frame.h>

bool UnriskyGui::OnInit() {
  MarketsFrame* frame = new MarketsFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
  frame->Show(true);
  return true;
}
