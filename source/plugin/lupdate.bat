cd .
::set path=D:\Qt5\5.10.0\mingw53_32\bin
set path=D:\Qt55\5.10.0\msvc2015\bin
::set path=D:\Qt5\5.10.0\msvc2017_64\bin
::set path=D:\Qt\5.10.0\msvc2017_64\bin
%path%\lupdate.exe plugins.pro -ts ..\..\res\ts\mct_zh_cn.ts
%path%\lupdate.exe plugins.pro -ts ..\..\res\ts\mct_zh_tw.ts