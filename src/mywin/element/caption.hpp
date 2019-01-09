//
// caption.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#define CAPTION_HEIGHT          12

// class Caption
class CCaption : public CWnd {
  BOOL Captured;
  CPoint LastPos;
private:
  void Draw();
protected:
  void OnPaint() { Draw(); }
  void OnLButtonDown( WORD, CPoint );
  void OnLButtonUp( WORD, CPoint );
  void OnMouseMove( WORD, CPoint );
public:
  CCaption() { Captured = FALSE; }
  BOOL Create( CWnd *Parent );
};

