

rmdir /s .\src_64

mkdir .\src_64

copy ..\build-CaX-Desktop_Qt_5_12_10_MSVC2017_64bit-Release\release\CaX.exe .\src_64

c:\apps\Qt5.12.10\5.12.10\msvc2017_64\bin\windeployqt.exe .\src_64\CaX.exe