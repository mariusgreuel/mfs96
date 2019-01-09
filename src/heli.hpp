//
// heli.hpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

class CHelicopter {
  FIX16 MotorRPM;
  CVec16 Force, Moment;
  CVec16 GroundSpeed, RotateSpeed, AirSpeed;
protected:
public:
  CHelicopter();
  void Init();
  void CalcFlightPath( CMatrix30 &Matrix );
  CVec16 GetMoveVector();
  CVec16 GetRotateVector();
};


