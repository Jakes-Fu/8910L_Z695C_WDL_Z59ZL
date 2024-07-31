SET VIEW_PATH=D:\Yihui\yihui.hu_MOCOR10A_view
pushd %VIEW_PATH%\MS_Code

call make p=sc6800h_openphone m=simulator clean
call make p=sc6800h_sp6801h m=simulator clean
call make p=sc6800h_sp6801h_nor_128X64 m=simulator clean
call make p=sc6800h_openphone_240X400 m=simulator clean
call make p=sc6800h_sp6804h m=simulator clean
call make p=sc6800h_s509h m=simulator clean
call make p=sc6800h_sp6804h_240x400 m=simulator clean
call make p=sc6800h_sp6804h_sogou m=simulator clean
call make p=sc6800h_sp6804h_ml m=simulator clean
call make p=sc6800h_sp6804h_ml_1 m=simulator clean

del /f /s /q *.keep*

pushd %VIEW_PATH%
cleartool update -overwrite -log NUL %VIEW_PATH%\MS_Code\mocor10a_cs.txt
cleartool setcs -current 
cls

rem pause
rem cleartool update 
pushd %VIEW_PATH%\MS_Code
call make p=sc6800h_sp6804h update job=16 -j5
call make p=sc6800h_openphone update job=16 -j5

call make p=sc6800h_sp6801h update job=16 -j5

rem START "sc6800h_sp6801h logel building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h m=logel
rem sleep 30s
rem pushd %VIEW_PATH%\Tools\Common\Bin\MsgDefBuild
rem call MapForPS.bat
rem copy /y %VIEW_PATH%\Tools\Common\Bin\MapStruct.map %VIEW_PATH%\MS_Code\tools\DEBUG_TOOL\LOGEL\Bin
pushd %VIEW_PATH%\MS_Code

START "sc6800h_sp6801h simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6801h m=simulator run=1 ib=1 lite=0

call make p=sc6800h_openphone_240X400 update job=16 -j5
call make p=sc6800h_sp6801h_nor_128X64 update job=16 -j5

call make p=sc6800h_sp6804h_240x400 update job=16 -j5
START "sc6800h_sp6804h simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc6800h_sp6804h m=simulator run=1 ib=1

call make p=sc6800h_s509h update job=16 -j5
call make p=sc6800h_sp6804h_sogou update job=16 -j5
call make p=sc6800h_sp6804h_ml update job=16 -j5
call make p=sc6800h_sp6804h_ml_1 update job=16 -j5

rem cleartool update D:\Yihui\yihui.hu_MOCOR10A_view\MS_Code\tools\DEBUG_TOOL\LOGEL
rem cleartool update D:\Yihui\yihui.hu_MOCOR10A_view\MS_Code\tools\DEBUG_TOOL\Logel_for_TD
rem cleartool update D:\Yihui\yihui.hu_MOCOR10A_view\MS_Code\tools\DEBUG_TOOL\Logel_for_TD_cust
rem call make p=sc8800g_cmmb update job=16 -j5
rem call make p=sc8800g_cmmb JAVA_SUPPORT=SUN m=sun_java job=9

rem call make p=sc6800h_sp6803h_quasim update job=16 -j5
rem call make p=sc6800h_sp6801h_slide update job=16 -j5
rem call make p=sc6800h_sp6801h_fui update job=16 -j5
rem call make p=sc6800h_sp6801h update job=16 -j5
rem call make p=sc8800g2_sp8803g_le_mc update job=16 -j5


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

