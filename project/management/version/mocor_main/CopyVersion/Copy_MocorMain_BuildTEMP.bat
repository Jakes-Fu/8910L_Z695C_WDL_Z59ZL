SET MOCOR_VIEW=E:\guowenli\main_6530
SET TARGET_DIR=\\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\TEMP
SET MOCOR_LABEL=MOCOR.W11.50

rd /s /q %TARGET_DIR%\%MOCOR_LABEL%

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

mkdir %TARGET_DIR%\%MOCOR_LABEL%\Doc

xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_240X400PDA_le_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_openphone_240X400PDA_le_builddir

xcopy /e /y /i %MOCOR_VIEW%\MS_Code\tools %TARGET_DIR%\%MOCOR_LABEL%\tools

copy /y "%MOCOR_VIEW%\MS_Code\version\documents\Release Notes.xls" %TARGET_DIR%\%MOCOR_LABEL%\Doc
copy /y %MOCOR_VIEW%\MS_Code\version\documents\DM_BASE_ReleaseNotes.xls %TARGET_DIR%\%MOCOR_LABEL%\Doc
copy /y %MOCOR_VIEW%\MS_Code\version\documents\Release_Notes_DM_BASE-1.xls %TARGET_DIR%\%MOCOR_LABEL%\Doc
copy /y %MOCOR_VIEW%\MS_Code\version\documents\BASE_ReleaseNotes.xls %TARGET_DIR%\%MOCOR_LABEL%\Doc

rem net use P: %TARGET_DIR%
rem P:
rem "C:\Program Files\winrar\rar" a -r %MOCOR_LABEL%.rar P:\%MOCOR_LABEL%
rem C:
rem net use P: /delete
rem pause