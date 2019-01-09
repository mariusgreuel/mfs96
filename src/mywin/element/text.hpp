//
// text.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

class CText : public CWnd {
  COLOR Color;
  WORD  Align;
private:
  void Draw();
protected:
  void OnPaint() { Draw(); }
public:
  CText()
  {
    Color = BLACK;
    Align = TA_LEFT|TA_TOP;
  }
  Create( const char *Text, const CRect &r, CWnd *Parent )
  {
    CWnd::Create( Text, WS_VISIBLE|WS_CHILD, r, Parent );
  }
  void SetTextColor( COLOR Color )
  {
    CText::Color = Color;
  }
  void SetTextAlign( WORD Align )
  {
    CText::Align = Align;
  }
};


