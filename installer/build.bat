

rmdir /s .\src

mkdir .\src

copy ..\build-CaX-Desktop_Qt_5_12_10_MSVC2017_32bit-Release\release\CaX.exe .\src

c:\apps\Qt5.12.10\5.12.10\msvc2017\bin\windeployqt.exe .\src\CaX.exe