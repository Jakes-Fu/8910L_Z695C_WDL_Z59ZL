SET MOCOR_VIEW=D:\Yihui\yihui.hu_MOCOR10A_view
SET MOCOR_LABEL=MOCOR_10A.W11.44

net share Yihui /DELETE /y
sleep 10s

rmdir /s /q D:\Yihui\TMP_VERSION\%MOCOR_LABEL%
del /q \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_10A\%MOCOR_LABEL%\%MOCOR_LABEL%_ReleaseNotes.rar
del /q \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_10A\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H.rar
del /q \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_10A\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G.rar
del /q \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_10A\%MOCOR_LABEL%\Tools.rar

cleartool update %MOCOR_VIEW%\MS_Code\version
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

rem ====================== Copy ReleaseNotes ======================
mkdir D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\ReleaseNotes
copy /y "%MOCOR_VIEW%\MS_Code\version\documents\Release Notes.xls" D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\ReleaseNotes
copy /y %MOCOR_VIEW%\MS_Code\version\documents\DM_BASE_ReleaseNotes.xls D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\ReleaseNotes
copy /y %MOCOR_VIEW%\MS_Code\version\documents\Release_Notes_DM_BASE-1.xls D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\ReleaseNotes
copy /y %MOCOR_VIEW%\MS_Code\version\documents\BASE_releasenotes_2011.xls D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\ReleaseNotes
pushd D:\Yihui\TMP_VERSION\%MOCOR_LABEL%
START "ReleaseNotes compressing ReleaseNotes" /d D:\Yihui\TMP_VERSION\%MOCOR_LABEL% /MAX "C:\Program Files\winrar\rar" a -r %MOCOR_LABEL%_ReleaseNotes.rar ReleaseNotes

rem ====================== Copy SC8800G Project ARM ======================
mkdir D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G
move /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_sp8803g_le_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_sp8803g_le_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_le_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_openphone_le_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc8800g_dualsim_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g_dualsim_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_sp8803g_le_modem_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_sp8803g_le_modem_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_240X400_le_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_openphone_240X400_le_builddir
move /y %MOCOR_VIEW%\MS_Code\build\fp880xg2_dualsim_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\fp880xg2_dualsim_builddir

rem move /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_sp8803g_le_NFTL_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_sp8803g_le_NFTL_builddir
rem move /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_sp8803g_le_iajava_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_sp8803g_le_iajava_builddir
rem move /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_dphone_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_dphone_builddir

pushd D:\Yihui\TMP_VERSION\%MOCOR_LABEL%
START "SC8800G Project ARM Compressing" /d D:\Yihui\TMP_VERSION\%MOCOR_LABEL% /MAX "C:\Program Files\winrar\rar" a -r %MOCOR_LABEL%_SC8800G.rar %MOCOR_LABEL%_SC8800G

rem ====================== Copy Tools ======================
move /y %MOCOR_VIEW%\MS_Code\tools D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\tools
pushd D:\Yihui\TMP_VERSION\%MOCOR_LABEL%
START "Tools compressing ReleaseNotes" /d D:\Yihui\TMP_VERSION\%MOCOR_LABEL% /MAX "C:\Program Files\winrar\rar" a -r Tools.rar tools

rem ====================== Copy SC6800H Project ARM ======================
mkdir D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_openphone_240X400_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_openphone_240X400_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_openphone_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_openphone_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_240X400_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_240X400_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_nor_128X64_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_nor_128X64_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_yz988_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_yz988_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_s509h_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_s509h_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_sogou_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_sogou_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_ml_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_ml_builddir
move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_ml_1_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_ml_1_builddir

rem move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_nor_128X32_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_nor_128X32_builddir
rem move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6803h_quasim_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6803h_quasim_builddir
rem move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_slide_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_slide_builddir
rem move /y %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_NFTL_builddir\img D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_NFTL_builddir

pushd D:\Yihui\TMP_VERSION\%MOCOR_LABEL%
"C:\Program Files\winrar\rar" a -r %MOCOR_LABEL%_SC6800H.rar %MOCOR_LABEL%_SC6800H
sleep 2m
md \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_10A\%MOCOR_LABEL%
copy /y *.rar \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_10A\%MOCOR_LABEL%

REM MOVE BACK ALL Image and tools files. VERIFY Excution sucessfully!
pause

move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\tools %MOCOR_VIEW%\MS_Code\tools

pause

move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_sp8803g_le_builddir %MOCOR_VIEW%\MS_Code\build\sc8800g2_sp8803g_le_builddir\img
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_openphone_le_builddir %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_le_builddir\img
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g_dualsim_builddir %MOCOR_VIEW%\MS_Code\build\sc8800g_dualsim_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_sp8803g_le_modem_builddir %MOCOR_VIEW%\MS_Code\build\sc8800g2_sp8803g_le_modem_builddir\img
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC8800G\sc8800g2_openphone_240X400_le_builddir %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_240X400_le_builddir\img
pause

move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_openphone_240X400_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_openphone_240X400_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_openphone_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_openphone_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_240X400_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_240X400_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_nor_128X64_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_nor_128X64_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6801h_yz988_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6801h_yz988_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_s509h_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_s509h_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_sogou_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_sogou_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_ml_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_ml_builddir\img 
move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6804h_ml_1_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6804h_ml_1_builddir\img 

rem move /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%\%MOCOR_LABEL%_SC6800H\sc6800h_sp6803h_quasim_builddir %MOCOR_VIEW%\MS_Code\build\sc6800h_sp6803h_quasim_builddir\img 
pause

net share Yihui=D:\Yihui /GRANT:sh-rd\clearcase,FULL /GRANT:Everyone,READ
