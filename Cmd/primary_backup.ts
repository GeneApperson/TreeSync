;***********************************************************
;
; Main backup command file to copy the shared folders on
; Gonzo to the backup drive. This uses the command file
; main_backup.ts to do the actual backup. It sets the
; symbols "DrvS" and "DrvD" and then executes main_backup.ts
;
;***********************************************************
; Revision History:
; 03/20/2011(Genea): Created
;
;***********************************************************

log on

opt = cd,cf
opt = pd,pf
;opt = pxd,pxf

print "***********************************************************"
print "* Gonzo Primary Backup"
print "*    Date: ",date
print "*    Time: ",time
print "***********************************************************"
print
print

set DrvS = "e:\"
set DrvD = "f:\"

run "MainBackup.ts"

print
print elapsed
print

log off

;***********************************************************
