cd 
set exe= D:\Qt55\5.10.0\msvc2015\bin\lupdate
%exe% ..\source\plugin\plugins.pro -ts ..\res\ts\mct_zh_cn.ts ..\res\ts\mct_zh_tw.ts ..\res\ts\mct_en.ts 
::%exe% mct.pro -ts ..\res\ts\mct_zh_cn.ts ..\res\ts\mct_zh_tw.ts ..\res\ts\mct_en.ts 
:: mct.pro
pause
:: 