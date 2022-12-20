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
				dw dat013, dat014-dat013
				dw dat014, dat015-dat014
				dw dat015, dat016-dat015
				dw dat016, dat017-dat016
				dw dat017, dat018-dat017
				dw dat018, dat019-dat018

				dw	mdat1, mdat2-mdat1
				dw mdat2, mdat3-mdat2

				dw dat021, dat022-dat021
				dw dat022, dat023-dat022
				dw dat023, dat024-dat023
				dw dat024, dat025-dat024
				dw dat025, dat026-dat025
				dw dat026, dat027-dat026
				dw dat027, dat028-dat027
				dw dat028, dat029-dat028
				dw dat029, dat030-dat029
				dw dat030, dat031-dat030
				dw dat031, dat032-dat031
				dw dat032, dat033-dat032
				dw dat033, dat034-dat033
				dw dat034, dat035-dat034
				dw dat035, dat036-dat035
				dw dat036, dat037-dat036
				dw dat037, dat038-dat037
				dw dat038, dat039-dat038
				dw dat039, dat040-dat039
				dw dat040, dat041-dat040
				dw dat041, dat042-dat041
				dw dat042, dat043-dat042
				dw dat043, dat044-dat043
				dw dat044, dat045-dat044
				dw dat045, dat046-dat045
				dw dat046, dat047-dat046
				dw dat047, dat048-dat047
				dw dat048, dat049-dat048
				dw dat049, dat050-dat049
				dw dat050, dat051-dat050
				dw dat051, dat052-dat051
				dw dat052, dat053-dat052
				dw dat053, dat054-dat053
				dw dat054, dat055-dat054
				dw dat055, dat056-dat055
				dw dat056, dat057-dat056
				dw dat057, dat058-dat057
				dw dat058, dat059-dat058
				dw dat059, dat060-dat059
				dw dat060, dat061-dat060
				dw dat061, dat062-dat061
				dw dat062, dat063-dat062
				dw dat063, dat064-dat063
				dw dat064, dat065-dat064
				dw dat065, dat066-dat065
				dw dat066, dat067-dat066
				dw dat067, dat068-dat067
				dw dat068, dat069-dat068
				dw dat069, dat070-dat069
				dw dat070, dat071-dat070
				dw dat071, dat072-dat071
				dw dat072, dat073-dat072
				dw dat073, dat074-dat073
				dw dat074, dat075-dat074
				dw dat075, dat076-dat075
				dw dat076, dat077-dat076
				dw dat077, dat999-dat077
files_end




;****** File data ******
     	cnop	0,2048

dat000	file	\striker\misc_gfx\loadscn3.tim
dat001	file	\striker\frontend\mod.tex
dat002	file	\striker\frontend\anim.san
dat003	file	\striker\frontend\mod.ted
dat004	file	\striker\frontend\player.skd
dat005	file	\striker\frontend\editman\pl_faces.pak
dat006	file	\striker\frontend\editman\pl_faces.pal
dat007	file	\striker\frontend\ballanim.mdt        
dat008	
dat009	file	\striker\frontend\tbar_out.mdt        		
dat010	file	\striker\frontend\editman\pl_nums.pak        			       
dat011	file	\striker\frontend\editman\kits.pak
dat012	file	\striker\frontend\editman\kits.pal
dat013	file	\striker\frontend\langtxts.pak
dat014	
dat015	
dat016	file	\striker\sfx\fsfx.dat
dat017 	file	\striker\sfx\sfxfront.vbs
dat018	file	\striker\sfx\sfxfront.dat
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
dat031		
dat032	
dat033		
dat034	
dat035	
dat036	
dat037	
dat038	 
dat039	
dat040	
dat041	
dat042
dat043
dat044
dat045
dat046
dat047
dat048
dat049
dat050
dat051
dat052
dat053
dat054
dat055
dat056
dat057
dat058
dat059
dat060
dat061
dat062
dat063
dat064
dat065
dat066
dat067
dat068
dat069
dat070
dat071
dat072
dat073
dat074	
dat075
dat076
dat077
dat078	
dat079	
dat999	




mdat1		incbin \striker\sfx\sfxfront.vbs
mdat2		incbin \striker\sfx\sfxfront.dat
mdat3
