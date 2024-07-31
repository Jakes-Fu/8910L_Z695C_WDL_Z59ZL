REM **********************************注意设置变量值后不能带有空格！！！否则路径会识别出错*****************

SET CONFIG_SPEC_FILE=D:\Yihui\yihui.hu_MOCOR10A_view\MS_Code\mocor10a_cs.txt
SET VIEW_TAG=yihui.hu_MOCOR10A_view
SET VIEW_PATH=D:\Yihui\yihui.hu_MOCOR10A_view
SET SCRIPT_PATH=%VIEW_PATH%\MS_Code\project\management\version\mocor_10a\build

REM ----------Setting for output_log-------------
SET CYGWIN_DIR=C:\cygwin\bin
SET MK_GENLIB_BAT=%SCRIPT_PATH%\Getgen_lib.bat

pushd %VIEW_PATH%

rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\OffsizeGenerator\OffsizeGenerator_8801h_Data
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\OffsizeGenerator\OffsizeGenerator_8801h_little_endian_Data
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\Debug
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\obj\*.*
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\out-12672--pun-common_ms_nw
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\out-6768--pun-common_ms_nw
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\Common\Bin\ArmLogel.exp
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\Common\Bin\ArmLogel.lib
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\Common\Bin\OffsizeGenerator_8801h.axf
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\Common\Bin\OffsizeGenerator_8801h.bin
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\Common\Bin\OffsizeGenerator_8801h_little_endian.axf
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\Common\Bin\OffsizeGenerator_8801h_little_endian.bin
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\error_8801h_gen.lst
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\error_8801h_lib.lst
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\infra_offsize.i
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\libuetoolsup.exp
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\libuetoolsup.pdb
del /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\*.bak
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\Debug

cleartool update -rename -log NUL %VIEW_PATH%\MS_Code\mocor10a_cs.txt
cleartool setcs -current 
cls

rem call %VIEW_PATH%\UpdateMocorView.BAT

REM **********************************生成Gen文件 *************************************************************
:GETGENFILES
attrib -r /s /d %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\*.*
attrib -r /s /d %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\Common\Bin\*.*
attrib -r /s /d %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build\*.*

ECHO ------------START GET GEN-LIB!!------------

call %CYGWIN_DIR%\bash --login -i < %MK_GENLIB_BAT%
pushd %VIEW_PATH%\MS_Code\P_TianJi2\Source\msd\build
call uetoolsup_build_8801h_dm.bat

rem build Little endian Only for 10A branch 
rem :GetOffsize
rem cmdide /t DebugRel /r /b /v y %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\OffsizeGenerator\OffsizeGenerator_8801h.mcp

rem If %ERRORLEVEL% EQU 0 GOTO GetOffsizelittleendian
rem Echo **Error occurs at GetOffsize.
rem pause
rem GOTO GetOffsize

:GetOffsizelittleendian
cmdide /t DebugRel /r /b /v y %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\OffsizeGenerator\OffsizeGenerator_8801h_little_endian.mcp

If %ERRORLEVEL% EQU 0 GOTO Runoffsize
Echo **Error occurs at GetOffsize.
pause
GOTO GetOffsizelittleendian

:Runoffsize
rem PUSHD %VIEW_PATH%\MS_Code\project\management\version\mocor_10a\build\GenOffsize

rem Run\run Axd -exec %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\Common\Bin\OffsizeGenerator_8801h.axf

rem Kill\kill /f axd
Echo Check the ms_offsize.dat Files has been generated successfully!
pause
