cd..

del c:\zips\stksrc.zip
call zip src

ren src.zip stksrc.zip


move stksrc.zip c:\zips
copy c:\zips\stksrc.zip \\birmingham1\server1\d_clarke
