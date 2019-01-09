//
// mfs.hpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

class MfsWindow : public CWnd {
protected:
  void OnPaint();
  void OnCommand( DWORD );
  void OnChar( DWORD Key );
public:
  MfsWindow();
};


