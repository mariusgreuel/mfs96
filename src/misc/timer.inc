;timer.inc, declarations for 8253/8254 and compatible
;Marius Greuel 3'96

TIMER_CTRL              = 43h         ;control word for timer
TIMER_0                 = 40h         ;i/o port timer 0
CW_INIT_TIMERMODE2      = 00110100b   ;initialize timer 0 command (mode 2)
CW_INIT_TIMERMODE3      = 00110110b   ;initialize timer 0 command (mode 3)
CW_READ_TIMER           = 00000100b   ;read timer 0 command
