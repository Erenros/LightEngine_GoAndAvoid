@echo off 

SolutionGenerator.exe -bp -make

copy /Y "..\lib\SDL2\lib\x64\*.dll" "..\ide\gcle\"

copy /Y "..\lib\SDL2_ttf\lib\x64\*.dll" "..\ide\gcle\"

copy /Y "..\lib\SDL2_Image\lib\x64\*.dll" "..\ide\gcle\"
copy /Y "..\lib\SDL2_Image\lib\x64\optional\*.dll" "..\ide\gcle\"

copy /Y "..\lib\SDL2_mixer\lib\x64\*.dll" "..\ide\gcle\"
copy /Y "..\lib\SDL2_mixer\lib\x64\optional\*.dll" "..\ide\gcle\"

pause