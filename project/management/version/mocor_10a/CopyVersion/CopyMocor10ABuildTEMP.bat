SET MOCOR_VIEW=D:\Yihui\yihui.hu_MOCOR10A_view
SET TARGET_DIR=\\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\TEMP
SET MOCOR_LABEL=MOCOR_10A.W11.44

rd /s /q %TARGET_DIR%\%MOCOR_LABEL%

cleartool update %MOCOR_VIEW%\MS_Code\tools\DEBUG_TOOL
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

xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_openphone_240X400_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_openphone_240X400_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_openphone_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_openphone_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_240X400_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_240X400_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_nor_128X64_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_nor_128X64_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_s509h_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_s509h_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_sogou_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_sogou_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_ml_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_ml_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_ml_1_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_ml_1_builddir

rem xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_builddir
rem xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_nor_128X32_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_nor_128X32_builddir

xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc8800g2_sp8803g_le_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_sp8803g_le_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_le_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_openphone_le_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc8800g_dualsim_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g_dualsim_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc8800g2_sp8803g_le_modem_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_sp8803g_le_modem_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_240X400_le_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_openphone_240X400_le_builddir
xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\fp880xg2_dualsim_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\fp880xg2_dualsim_builddir

rem xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6803h_quasim_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6803h_quasim_builddir

rem xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc8800g2_sp8803g_le_iajava_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\sc8800g2_sp8803g_le_iajava_builddir
rem xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc8800g2_dphone_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\sc8800g2_dphone_builddir
rem xcopy /e /y /i %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_NFTL_builddir\img %TARGET_DIR%\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_NFTL_builddir

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