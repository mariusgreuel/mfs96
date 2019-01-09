;PIC.inc, für 8259A PICs
;Marius Greuel

MASTER_PIC              = 020h  ;Port des 8259A Master
SLAVE_PIC               = 0a0h  ;Port des 8259A Slave
OCW1                    = 1
OCW2                    = 0
OCW3                    = 0
OC_MASK                 = 1
CW_NON_SPEC_EOI         = 20h   ;Ende Interrupt Befehl
MASTER_FIRST_VECTOR     = 8
SLAVE_FIRST_VECTOR      = 70h


