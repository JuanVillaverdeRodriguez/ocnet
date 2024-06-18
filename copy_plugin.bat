@echo off

set destination="C:\Program Files\Common Files\VST3\"
set source="C:\Users\TecNo\Desktop\COSAS\CLASE\TFG\ocnet2\ocnet\ocnet\Builds\VisualStudio2022\x64\Debug\VST3\ocnet.vst3\Contents\x86_64-win\ocnet.vst3"

if not exist %destination% (
    mkdir %destination%
)

xcopy /Y %source% %destination%