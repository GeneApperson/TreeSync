;***********************************************************
;
; This command file will back up the root folder specified by
; the symbol "root". The symbol "DrvS" specifies the source
; drive and the symbol "DrvD" specifies the destination drive
; It assumes that logging has been turned on.
;
;***********************************************************
; Revision History:
; 03/20/2011(Genea): Created
;
;***********************************************************

set src = DrvS + root
set dst = DrvD + root

set banner = "********** Updating " + root + " **********"
print banner
print "    SRC path = ",src
print "    DST path = ",dst
print banner
print

update src,dst

print
print stat
clear stat
print
print

;***********************************************************
