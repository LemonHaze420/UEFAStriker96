;******************************************
;*														*
;* All data output into datafile.dat		*
;*														*
;******************************************

file	macro
	incbin	\1
	cnop	0,2048
	endm


				org	$00000000


;****** Offset to each file ******

files_start	dw	(files_end-files_start)/8
				dw	dat000, dat001-dat000
				dw	dat001, dat002-dat001
				dw	dat002, dat003-dat002
				dw	dat003, dat004-dat003
				dw	dat004, dat005-dat004
				dw dat005, dat006-dat005
				dw dat006, dat007-dat006
				dw dat007, dat008-dat007
				dw dat008, dat009-dat008
				dw dat009, dat010-dat009
				dw dat010, dat011-dat010
				dw dat011, dat012-dat011
				dw dat012, dat013-dat012

				dw	mdat1, mdat2-mdat1
				dw mdat2, mdat3-mdat2

				dw dat015, dat016-dat015
				dw dat016, dat017-dat016
				dw dat017, dat018-dat017
				dw dat018, dat019-dat018
				dw dat019, dat020-dat019
				dw dat020, dat021-dat020
				dw dat021, dat022-dat021
				dw dat022, dat023-dat022
				dw dat023, dat024-dat023
				dw dat024, dat025-dat024
				dw dat025, dat026-dat025
				dw dat026, dat027-dat026
				dw dat027, dat028-dat027
				dw dat028, dat029-dat028
				dw dat029, dat030-dat029
				dw dat030, dat999-dat030
files_end




;****** File data ******
     	cnop	0,2048

dat000	file	\striker\frontend\loadman\loadpic.pak
dat001	file	\striker\frontend\modtex.pak
dat002	file	\striker\frontend\anim.san
dat003	file	\striker\frontend\mod.ted
dat004	file	\striker\frontend\player.skd
dat005	file	\striker\frontend\editman\pl_faces.pak
dat006	file	\striker\frontend\editman\pl_faces.pal
dat007	file	\striker\frontend\editman\pl_nums.pak        			       
dat008	file	\striker\frontend\editman\kits.pak
dat009	file	\striker\frontend\editman\kits.pal
dat010	file	\striker\sfx\fsfx.dat
dat011 	file	\striker\sfx\sfxfront.vbs
dat012	file	\striker\sfx\sfxfront.dat
dat013
dat014
dat015	file	\striker\frontend\loadman\mantex.pak
dat016	
dat017	
dat018	
dat019   
dat020	
dat021	
dat022	
dat023	
dat024	
dat025		
dat026		
dat027		
dat028		
dat029		
dat030		
dat999	




mdat1		incbin \striker\sfx\sfxfront.vbs
mdat2		incbin \striker\sfx\sfxfront.dat
mdat3
