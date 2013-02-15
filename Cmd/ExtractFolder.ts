;***********************************************************
;
; This command file will extract files from the destination
; folder that differ from the source folder and place them
; in the extraction folder. The extraction options should be
; set before this is invoked.
;
;***********************************************************
; Revision History:
; 00/04/2011(Genea): Created
;
;***********************************************************

set src = RootS + target
set dst = RootD + target
set xtr = RootX + target

set banner = "********** Extract " + target + " **********"
print banner
print "    SRC path = ",src
print "    DST path = ",dst
print "    XTR path = ",xtr
print banner
print

extract src, dst, xtr

print
print stat
clear stat
print
print

;***********************************************************
