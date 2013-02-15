;***********************************************************
;
; Main command file for backing up the shared folders on Gonzo.
; This file is intended to be included by another command file
; that specifies the source and destination drives.
; This file assumes that the symbols "DrvS" and "DrvD" have been
; set to specify the source and destination drives.
; It also assumes that the log has been turned on by the
; outer command file.
;
;***********************************************************
; Revision History:
; 03/20/2011(Genea): Created
;
;***********************************************************

set root = "Archive"
run "UpdateFolder.ts"

set root = "Backup"
run "UpdateFolder.ts"

set root = "Documents"
run "UpdateFolder.ts"

set root = "Media"
run "UpdateFolder.ts"

set root = "Reference"
run "UpdateFolder.ts"

set root = "SysAdmin"
run "UpdateFolder.ts"

;***********************************************************
