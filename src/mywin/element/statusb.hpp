//
// statusb.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

// class Statusbar
class CStatusBar : public CWnd {
  void DrawStatusBar();
protected:
  void OnPaint();
public:
  BOOL Create( CWnd *Parent );
};

