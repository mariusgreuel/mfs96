;object.inc, Strukturen aus object.hpp
;Marius Greuel '95

COLOR_NULL      = -1
COLOR_SHADOW    = -2

_SPOINT         struc
SX              dw ?
SY              dw ?
_SPOINT         ends

_LPOINT         struc
LX              dd ?
LY              dd ?
_LPOINT         ends

_SRECT          struc
SLeft           dw ?
STop            dw ?
SRight          dw ?
SBottom         dw ?
_SRECT          ends

_LRECT          struc
LLeft           dd ?
LTop            dd ?
LRight          dd ?
LBottom         dd ?
_LRECT          ends

_VECTOR         struc
X               dd ?
Y               dd ?
Z               dd ?
_VECTOR         ends

_MATRIX         struc
                dd 4*3 dup(?)
_MATRIX         ends

_POINT3D        struc
OP              _VECTOR ?
TP              _VECTOR ?
PP              _LPOINT ?
_POINT3D        ends

_PGCOLOR        struc
FGD             db ?
FGF             db ?
BGD             db ?
BGF             db ?
_PGCOLOR        ends

_POLYGON        struc
PG_Before       dd ?
PG_Behind       dd ?
PG_Color        _PGCOLOR<>
PG_Points       dd ?
PG_Point        dd ?
_POLYGON        ends

_OBJECT         struc
OBJ_Next        dd ?
OBJ_Prev        dd ?
OBJ_Points      dd ?
OBJ_Polygons    dd ?
OBJ_Point       dd ?
OBJ_BspRoot     dd ?
_OBJECT         ends

