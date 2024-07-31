SET VIEW_PATH=D:\Yihui\yihui.hu_MOCOR10A_view
pushd %VIEW_PATH%

net share Yihui /DELETE /y
sleep 10s

rd /s /q %VIEW_PATH%\MS_Code\build
rd /s /q %VIEW_PATH%\MS_Code\lib
rd /s /q %VIEW_PATH%\MS_Code\ARMRomCode\seine\arm\drv_sc6600v\build
rem rd /s /q %VIEW_PATH%\MS_Code\ARMRomCode\seine\arm\drv_sc6600v\8801h_h5
rd /s /q %VIEW_PATH%\MS_Code\ARMRomCode\seine\arm\drv_sc6600v\lib
rd /s /q %VIEW_PATH%\MS_Code\CMMB\source\mtv_adapter\inc
rd /s /q %VIEW_PATH%\MS_Code\sim\source\test\SIMConPad\Debug
rd /s /q %VIEW_PATH%\MS_Code\sim\source\test\SIMConPad\Release
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\GUI\Debug
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\GUI\Release
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\MSKernel\MSBridge\Debug
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\MSKernel\MSBridge\Release
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\ControlPanel_bl7\Debug
del /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\ControlPanel_bl7\MS_Log.txt
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\ControlPanel_bl7\Release
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\Common
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\filesys\FileSystem___Win32_Debug_Unicode
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\LogelSidedPS___Win32_Release_For_MOCOR_DM
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\LogelSidedPS___Win32_Release_For_MOCOR_DM_BASE
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel___Win32_Release_For_MOCOR_DM
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\Debug
rd /s /q %VIEW_PATH%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\filesys\Release
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\src
rd /s /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\src
rd /s /q %VIEW_PATH%\MS_Code\PS\source\sdl_project\gen
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\External\rlc_t4code.c
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel.ncb
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel.opt
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel.plg
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel_Mocor.ncb
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel_Mocor.opt
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel_Mocor_DM.ncb
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\Logel\Logel_Mocor_DM.opt
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\LogelSidedPS.plg
del /q %VIEW_PATH%\MS_Code\P_TianJi2\Source\infra\test-infra\Logel\Source_tt_2\LogelSidedPS\LogelSidedPS_Mocor_DM.plg
del /f /s /q *.keep*

net share Yihui=D:\Yihui /GRANT:sh-rd\clearcase,FULL /GRANT:Everyone,READ

pushd %VIEW_PATH%
cleartool update -overwrite -log NUL %VIEW_PATH%\MS_Code\mocor10a_cs.txt
cleartool setcs -current 
cls

rem pause
rem cleartool update 
pushd %VIEW_PATH%\MS_Code
call make p=sc6800h_sp6804h new job=16 -j4
call make p=sc6800h_openphone new job=16 -j5

call make p=sc6800h_sp6801h new job=16 -j5

rem START "sc6800h_sp6801h logel building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h m=logel
rem sleep 30s
rem pushd %VIEW_PATH%\Tools\Common\Bin\MsgDefBuild
rem call MapForPS.bat
rem copy /y %VIEW_PATH%\Tools\Common\Bin\MapStruct.map %VIEW_PATH%\MS_Code\tools\DEBUG_TOOL\LOGEL\Bin
pushd %VIEW_PATH%\MS_Code

START "sc6800h_sp6801h simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h m=simulator run=1 ib=1 lite=0

call make p=sc6800h_openphone_240X400 new job=16 -j5
call make p=sc6800h_sp6801h_nor_128X64 new job=16 -j5

call make p=sc6800h_sp6804h_240x400 new job=16 -j5
START "sc6800h_sp6804h simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6804h m=simulator run=1 ib=1

call make p=sc6800h_s509h new job=16 -j5
call make p=sc6800h_sp6804h_sogou new job=16 -j5
call make p=sc6800h_sp6804h_ml new job=16 -j5
call make p=sc6800h_sp6804h_ml_1 new job=16 -j5

rem cleartool update D:\Yihui\yihui.hu_MOCOR10A_view\MS_Code\tools\DEBUG_TOOL\LOGEL
rem cleartool update D:\Yihui\yihui.hu_MOCOR10A_view\MS_Code\tools\DEBUG_TOOL\Logel_for_TD
rem cleartool update D:\Yihui\yihui.hu_MOCOR10A_view\MS_Code\tools\DEBUG_TOOL\Logel_for_TD_cust
rem call make p=sc8800g_cmmb new job=16 -j4
rem call make p=sc8800g_cmmb JAVA_SUPPORT=SUN m=sun_java job=9

call make p=sc8800g2_sp8803g_le new job=16 -j4
rem START "sc8800g2_sp8803g_le logel building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_sp8803g_le m=logel

call make p=sc8800g2_openphone_le new job=16 -j4

call make p=sc8800g_dualsim new job=16 -j4
START "sc8800g2_sp8803g_le simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_sp8803g_le m=simulator run=1 ib=1

call make p=fp880xg2_dualsim new job=16 -j5

call make p=sc8800g2_sp8803g_le_modem new job=16 -j4
call make p=sc8800g2_openphone_240X400_le new job=16 -j4


rem call make p=sc6800h_sp6801h_yz988 new job=16 -j5
rem call make p=sc6800h_sp6803h_quasim new job=16 -j4
rem call make p=sc6800h_sp6801h_slide new job=16 -j5
rem call make p=sc6800h_sp6801h_fui new job=16 -j5
rem call make p=sc6800h_sp6801h new job=16 -j5
rem call make p=sc8800g2_sp8803g_le_mc new job=16 -j4

pushd %VIEW_PATH%\MS_Code\project\management\version\mocor_10a\build
call r2_SendMailARM.bat

pushd %VIEW_PATH%\MS_Code

START "sc6800h_openphone simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_openphone m=simulator run=1 ib=1
sleep 1m
START "sc6800h_sp6801h_nor_128X64 simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h_nor_128X64 m=simulator run=1 ib=1
sleep 1m
START "sc6800h_sp6804h_240x400 simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6804h_240x400 m=simulator run=1 ib=1
sleep 1m
START "sc6800h_openphone_240X400 simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_openphone_240X400 m=simulator run=1 ib=1
sleep 1m
START "sc6800h_s509h simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_s509h m=simulator run=1 ib=1
sleep 1m
START "sc8800g_dualsim simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g_dualsim m=simulator run=1 ib=1
sleep 1m
START "sc8800g2_openphone_240X400_le simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_openphone_240X400_le m=simulator run=1 ib=1
sleep 1m
START "sc8800g2_openphone_le simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_openphone_le m=simulator run=1 ib=1
sleep 1m
START "fp880xg2_dualsim simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=fp880xg2_dualsim m=simulator run=1 ib=1
sleep 1m
START "sc6800h_sp6804h_sogou simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6804h_sogou m=simulator run=1 ib=1
sleep 1m
START "sc6800h_sp6804h_ml simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6804h_ml m=simulator run=1 ib=1
sleep 1m
START "sc6800h_sp6804h_ml_1 simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6804h_ml_1 m=simulator run=1 ib=1

rem sleep 1m
rem START "sc6800h_sp6801h_slide simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h_slide m=simulator run=1 ib=1
rem sleep 1m
rem START "sc6800h_sp6801h_fui simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h_fui m=simulator run=1 ib=1
rem sleep 1m
rem START "sc8800g2_sp8803g_le_mc simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_sp8803g_le_mc m=simulator run=1 ib=1
rem sleep 1m
rem START "sc8800g2_sp8803g_le_iajava simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_sp8803g_le_iajava m=simulator run=1 ib=1
rem sleep 1m
rem START "sc8800g2_sp8803g_le_NFTL simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_sp8803g_le_NFTL m=simulator run=1 ib=1
rem sleep 1m

