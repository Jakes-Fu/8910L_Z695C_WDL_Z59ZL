SET VIEW_PATH=E:\guowenli\main_6530
pushd %VIEW_PATH%

pushd %VIEW_PATH%\MS_Code

call make p=sc8800g2_openphone_240X400PDA_le m=simulator clean

rem del /f /s /q *.keep*

pushd %VIEW_PATH%\MS_Code

call make p=sc8800g2_openphone_240X400PDA_le update job=16 -j5

pushd %VIEW_PATH%\MS_Code\project\management\version\mocor_main\build
call r2_SendMailARM.bat

pushd %VIEW_PATH%\MS_Code
START "sc8800g2_openphone_240X400PDA_le simulator building" /d %VIEW_PATH%\MS_Code /MAX call make p=sc8800g2_openphone_240X400PDA_le m=simulator run=1 ib=1
sleep 1m
