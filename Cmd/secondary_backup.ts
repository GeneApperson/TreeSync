;***********************************************************
;
; Secondary backup command file to copy the shared folders on the Gonzo
; backup drive to an external drive.
; This uses the command file main_backup.ts to do the actual backup.
; It sets the symbols "DrvS" and "DrvD" and then executes main_backup.ts;
;
;***********************************************************
; Revision History:
; 03/20/2011(Genea): Created
;
;***********************************************************

log on

opt = cd,cf
opt = pd,pf

print "***********************************************************"
print "* Gonzo Secondary Backup"
print "*    Date: ",date
print "*    Time: ",time
print "***********************************************************"
print
print

set DrvS = "f:\"
set DrvD = "g:\"

run "MainBackup.ts"

print
print elapsed
print

log off

;***********************************************************
