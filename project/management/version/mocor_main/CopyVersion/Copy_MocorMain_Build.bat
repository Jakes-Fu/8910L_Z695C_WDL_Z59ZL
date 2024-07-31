SET MOCOR_LABEL=MOCOR.W11.50
SET MOCOR_VIEW=E:\guowenli\main_6530
SET TEMP_VERSION_PATH=E:\guowenli\TEMP_VERSION

net share Yihui /DELETE /y
sleep 10s

svn update %MOCOR_VIEW%\MS_Code\version
svn update %MOCOR_VIEW%\MS_Code\tools\DEBUG_TOOL
rd /s /q %MOCOR_VIEW%\MS_Code\tools\DEBUG_TOOL\Logel_for_TD\Bin\MsgDefBuild
rd /s /q %MOCOR_VIEW%\MS_Code\tools\DEBUG_TOOL\Logel_for_TD\Bin\tcl8.0
rd /s /q %MOCOR_VIEW%\MS_Code\tools\DEBUG_TOOL\Logel_for_TD_cust\Bin\MsgDefBuild
rd /s /q %MOCOR_VIEW%\MS_Code\tools\DEBUG_TOOL\Logel_for_TD_cust\Bin\tcl8.0

pushd %MOCOR_VIEW%\MS_Code\tools\DEBUG_TOOL\Logel_for_TD\Bin
del /q ArmLogelC.exe
del /q ASN1DPSrv.lib
del /q ASN1DPSrv_sys.lib
del /q ChanMand.dll
del /q CommUartd.dll
del /q CS域GSM到TD切换成功.WAV
del /q CS域GSM到TD切换失败.WAV
del /q CS域TD到GSM切换成功.WAV
del /q CS域TD到GSM切换失败.WAV
del /q CS域注册成功.WAV
del /q GenOffsize.bat
del /q GSM到TD重选成功.WAV
del /q GSM到TD重选失败.WAV
del /q Logel_tt.exp
del /q Logel_tt.lib
del /q Logel_tt.pdb
del /q Logel_tt_cust.exe
del /q Logel_tt_tp.exe
del /q LogelSidedPSc.dll
del /q OffsizeGenerator_8801h.axf
del /q OffsizeGenerator_8801h.bin
del /q OffsizeGenerator_8801h_little_endian.axf
del /q OffsizeGenerator_8801h_little_endian.bin
del /q PS域GSM到TD切换成功.WAV
del /q PS域GSM到TD切换失败.WAV
del /q PS域TD到GSM切换成功.WAV
del /q PS域TD到GSM切换失败.WAV
del /q PS域注册成功.WAV
del /q SciUart.dll
del /q tcl80.lib
del /q tcl83.lib
del /q TD到GSM重选成功.WAV
del /q TD到GSM重选失败.WAV
pushd %MOCOR_VIEW%\MS_Code\tools\DEBUG_TOOL\Logel_for_TD_cust\Bin
del /q ASN1DPSrv.lib
del /q ASN1DPSrv_sys.lib
del /q ChanMand.dll
del /q CommUartd.dll
del /q CS域GSM到TD切换成功.WAV
del /q CS域GSM到TD切换失败.WAV
del /q CS域TD到GSM切换成功.WAV
del /q CS域TD到GSM切换失败.WAV
del /q CS域注册成功.WAV
del /q GenOffsize.bat
del /q GSM到TD重选成功.WAV
del /q GSM到TD重选失败.WAV
del /q Logel_tt.exp
del /q Logel_tt.lib
del /q Logel_tt.pdb
del /q Logel_tt_cust.exe
del /q Logel_tt_tp.exe
del /q LogelSidedPSc.dll
del /q OffsizeGenerator_8801h.axf
del /q OffsizeGenerator_8801h.bin
del /q OffsizeGenerator_8801h_little_endian.axf
del /q OffsizeGenerator_8801h_little_endian.bin
del /q PS域GSM到TD切换成功.WAV
del /q PS域GSM到TD切换失败.WAV
del /q PS域TD到GSM切换成功.WAV
del /q PS域TD到GSM切换失败.WAV
del /q PS域注册成功.WAV
del /q SciUart.dll
del /q tcl80.lib
del /q tcl83.lib
del /q TD到GSM重选成功.WAV
del /q TD到GSM重选失败.WAV
rd /s /q %MOCOR_VIEW%\MS_Code\tools\DEBUG_TOOL\DSPLOGGER\Source

rem ====================== Copy ReleaseNotes and compress ======================
mkdir %TEMP_VERSION_PATH%\%MOCOR_LABEL%\ReleaseNotes
copy /y "%MOCOR_VIEW%\MS_Code\version\documents\Release Notes.xls" %TEMP_VERSION_PATH%\%MOCOR_LABEL%\ReleaseNotes
copy /y %MOCOR_VIEW%\MS_Code\version\documents\DM_BASE_ReleaseNotes.xls %TEMP_VERSION_PATH%\%MOCOR_LABEL%\ReleaseNotes
copy /y %MOCOR_VIEW%\MS_Code\version\documents\Release_Notes_DM_BASE-1.xls %TEMP_VERSION_PATH%\%MOCOR_LABEL%\ReleaseNotes
copy /y %MOCOR_VIEW%\MS_Code\version\documents\BASE_releasenotes_2011.xls %TEMP_VERSION_PATH%\%MOCOR_LABEL%\ReleaseNotes

pushd %TEMP_VERSION_PATH%\%MOCOR_LABEL%
START "SC6530 ReleaseNotes compressing " /d %TEMP_VERSION_PATH%\%MOCOR_LABEL% /MAX "C:\Program Files\winrar\rar" a -r %MOCOR_LABEL%_ReleaseNotes.rar ReleaseNotes

rem ====================== Copy SC6530 image and compress======================
mkdir %TEMP_VERSION_PATH%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6530
move /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_240X400PDA_le_builddir\img %TEMP_VERSION_PATH%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6530\sc8800g2_openphone_240X400PDA_le_builddir

pushd %TEMP_VERSION_PATH%\%MOCOR_LABEL%
START "SC6530 image Compressing" /d %TEMP_VERSION_PATH%\%MOCOR_LABEL% /MAX "C:\Program Files\winrar\rar" a -r %MOCOR_LABEL%_SC6530.rar %MOCOR_LABEL%_SC6530

rem ====================== Copy Tools and compress======================
move /y %MOCOR_VIEW%\MS_Code\tools %TEMP_VERSION_PATH%\%MOCOR_LABEL%\tools

pushd %TEMP_VERSION_PATH%\%MOCOR_LABEL%
START "SC6530 Tools compressing " /d %TEMP_VERSION_PATH%\%MOCOR_LABEL% /MAX "C:\Program Files\winrar\rar" a -r Tools.rar tools

rem XXXXXXXXXXXXXXXXXXXXXXXXXXXX Copy SC6630 Project to scmserver, waiting compressing XXXXXXXXXXXXXXXXXX
pause
rem XXXXXXXXXXXXXXXXXXXXXXXXXXXX Copy SC6630 Project to scmserver, waiting compressing XXXXXXXXXXXXXXXXXX
pause

md \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_MAINTRUNK\%MOCOR_LABEL%
copy /y *.rar \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_MAINTRUNK\%MOCOR_LABEL%

rem ******************* Copy sc650 image and tools back *********************
move /y %TEMP_VERSION_PATH%\%MOCOR_LABEL%\tools %MOCOR_VIEW%\MS_Code\tools 
move /y %TEMP_VERSION_PATH%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6530\sc8800g2_openphone_240X400PDA_le_builddir %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_240X400PDA_le_builddir\img
pause
