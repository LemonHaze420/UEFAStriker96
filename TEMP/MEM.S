  	public	on
	section .data


	include "genasm.s"

time_and_date db "\#_hours:\#_minutes \#_day/\#_month/\#_year",0

	section limbo 

sysfont_spr		incbin "c:\psx\sysfont\sysfnta.spr"
sysfont_pal		incbin "c:\psx\sysfont\sysfnta.pal"
card_icon_tim	incbin "d:\striker\gfx\cardicon\mcicon1.tim"


end_of_ram	dsb	16
	public	off

