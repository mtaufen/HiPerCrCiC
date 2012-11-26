#ifndef _Scroll_View_h_
#define _Scroll_View_h_

#include "ScrollBar.h"

class ScrollView : public View
{
private:
  HorizScrollBar * _horizScrollBar;
  VertScrollBar * _vertScrollBar;
  View * _contentView;

public:

private:

public:
  ScrollView(CGRect const& rect);
  virtual ~ScrollView();

  // whenever stuff is added to the content view's bounds, we resize thr
  // content view's bounds to fit the content, with no limit on max size and
  // the min size limited to the size of the scroll view minus the bars. 
  // The bottom bar should only appear if the size of the content view
  // exceeds the width of the scroll view. The side bar is always present,
  // and the size of the slider on each bar is set relative to the
  // ratio of size of the content view to the scroll view
  View* insertSubView(View * view); // adds a subview to the contentview
  virtual void draw();

};

#endif // _Scroll_View_h_