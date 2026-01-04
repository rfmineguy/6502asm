.org $0800 ; hi
.org $4000
.org $3412

; Use at least one of each ora addressing mode
ora #43
ora $53
ora $53, X ;4145 2947 7816
ora $5313
ora $5313, X
ora $5313, Y
ora ($41, X)
ora ($56), Y

.org $31
.org $356

; music:
.org $ff
.byte $34,$53,#42
.byte #13,$31
