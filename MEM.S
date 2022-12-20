  	public	on
	section .data

card_icon_tim	incbin "d:\striker\gfx\cardicon\mcicon1.tim"



	include "genasm.s"



	cnop	0,4

time_and_date db "\#_hours:\#_minutes \#_day/\#_month/\#_year",0

	section .limbo 

sysfont_spr			incbin "c:\psx\sysfont\sysfnta.spr"
sysfont_pal			incbin "c:\psx\sysfont\sysfnta.pal"
ability_file_addr	incbin "\striker\frontend\ability\ability.dat"


	CNOP 0,4
end_of_ram	dsb	16

rob_end_of_ram dsb 4


	public	off

