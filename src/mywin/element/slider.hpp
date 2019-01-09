//
// slide.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

class CSlider : public CWnd {
  enum { CAPTURED = 1 };
  BYTE Flag;
  float Value, Min, Max;
private:
  void Draw();
protected:
  void OnPaint() { Draw(); }
  void OnMouseMove( WORD, CPoint );
  void OnLButtonDown( WORD, CPoint );
  void OnLButtonUp( WORD, CPoint );
public:
  CSlider() { Flag = 0; Value = 0.0; Min = 0.0; Max = 10.0; }
  BOOL Create( const char *Text, const CRect &r, CWnd *Parent )
  {
    return CWnd::Create( Text, WS_VISIBLE|WS_CHILD, r, Parent );
  }
  void SetRange( float _Min, float _Max ) { Min = _Min; Max = _Max; }
  void SetValue( float v )
  {
    if( Value != v && v >= Min && v <= Max ) {
      Value = v;
      RedrawWindow();
    }
  }
  float GetValue() { return Value; }
};


