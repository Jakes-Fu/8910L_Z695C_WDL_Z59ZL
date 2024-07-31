SET MOCOR_VIEW=E:\guowenli\main_6530
SET MOCOR_IDH_VIEW=D:\guowenli\IDHSource
SET MOCOR_LABEL=MOCOR.W11.50

del /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source.rar
rd /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%

pushd %MOCOR_VIEW%\MS_Code

echo =======================step1 删除scmserver上的IDH包========
del /q \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_MAINTRUNK\%MOCOR_LABEL%\%MOCOR_LABEL%_Source.rar

echo ========================step2 拷贝lib======================
rmdir /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\mmi_app\app\ia_java

rd /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_240x320_6432
rd /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_240x320_lightcolor
rd /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_240x320_universe
rem rd /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_240x400_universe
rd /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_320x480_universe

del /f /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_176x220\FONT\LANG_FONT_HAN*.lib
del /f /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_320x480\FONT\LANG_FONT_HAN*.lib
del /f /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_240x320\FONT\LANG_FONT_HAN*.lib
del /f /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_240x400\FONT\LANG_FONT_HAN*.lib
del /f /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_128x64\FONT\LANG_FONT_HAN*.lib
del /f /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_240x320_universe\FONT\LANG_FONT_HAN*.lib
del /f /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_240x400_universe\FONT\LANG_FONT_HAN*.lib
del /f /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\resource\mmi_res_320x480_universe\FONT\LANG_FONT_HAN*.lib

attrib -r /s /d %MOCOR_IDH_VIEW%\MS_Code\Third-party\Mplapi\Arm_lib\normal\mplapi*.a
attrib -r /s /d %MOCOR_IDH_VIEW%\MS_Code\Third-party\netfront
rd /s /q %MOCOR_IDH_VIEW%\MS_Code\Third-party\netfront\LittleEndian
xcopy /I /Y /E %MOCOR_VIEW%\MS_Code\Third-party\formal\netfront\LittleEndian %MOCOR_IDH_VIEW%\MS_Code\Third-party\netfront\LittleEndian

del /s /q /a:r "%MOCOR_IDH_VIEW%\MS_Code\version\Release Notes.xls"
copy /y "%MOCOR_VIEW%\MS_Code\version\documents\Release Notes.xls" %MOCOR_IDH_VIEW%\MS_Code\version

rd /s /q %MOCOR_IDH_VIEW%\MS_Code\tools\DEBUG_TOOL\Logel_for_TD

echo ========================step3 修改IDH的make文件=============
pause

pushd %MOCOR_IDH_VIEW%\MS_Code
ccperl IDHMakefile.pl project_sc8800g_openphone_240X400PDA_rvct_le.mk

del /f /s /q proj*.bak
pushd %MOCOR_IDH_VIEW%\MS_Code
md Third-party\bt_csr\bt_csr_15_3\Arm_lib\nostack
copy /y %MOCOR_VIEW%\MS_Code\Third-party\formal\bt_csr\bt_csr_15_3\Arm_lib\normal\bt_csr_arm9_le.a %MOCOR_IDH_VIEW%\MS_Code\Third-party\bt_csr\bt_csr_15_3\Arm_lib\nostack
copy /y %MOCOR_VIEW%\MS_Code\Third-party\formal\bt_csr\bt_csr_15_3\Arm_lib\normal\bt_csr_arm9.a %MOCOR_IDH_VIEW%\MS_Code\Third-party\bt_csr\bt_csr_15_3\Arm_lib\nostack


echo ========================step4 编译工程=============
pushd %MOCOR_IDH_VIEW%\MS_Code
move /y %MOCOR_VIEW%\MS_Code\lib\sc8800g_openphone_240X400PDA_rvct_le %MOCOR_IDH_VIEW%\MS_Code\lib\sc8800g_openphone_240X400PDA_rvct_le
START "sc8800g_openphone_240X400PDA_rvct_le building" /d %MOCOR_IDH_VIEW%\MS_Code /MAX call make p=sc8800g_openphone_240X400PDA_rvct_le new
sleep 3m
pause

echo ========================step5 打包编译结果=========
pushd %MOCOR_IDH_VIEW%
mkdir %MOCOR_IDH_VIEW%\%MOCOR_LABEL%

move /y %MOCOR_IDH_VIEW%\MS_Code\build\sc8800g_openphone_240X400PDA_rvct_le_builddir\img %MOCOR_IDH_VIEW%\%MOCOR_LABEL%\sc8800g_openphone_240X400PDA_rvct_le_builddir

rmdir /s /q %MOCOR_IDH_VIEW%\MS_Code\build
rmdir /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\Common
rd /s /q %MOCOR_IDH_VIEW%\MS_Code\Third-party\bt_csr\bt_csr_15_3

md %MOCOR_IDH_VIEW%\MS_Code\build

del /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\Common\Lib\MSDevBridge.lib
rmdir /s /q %MOCOR_IDH_VIEW%\MS_Code\MS_MMI\source\winsimulator_tp\win_platform\MSKernel\MSBridge\Debug
del /q %MOCOR_IDH_VIEW%\MS_Code\tools\DEBUG_TOOL\NVEDITOR\Bin\CommUart.log
rd /s /q %MOCOR_IDH_VIEW%\MS_Code\Third-party\netfront\LittleEndian
rd /s /q %MOCOR_IDH_VIEW%\MS_Code\Third-party\Mplapi

rename %MOCOR_IDH_VIEW%\MS_Code %MOCOR_LABEL%_Source
pause

pushd %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\lib
"C:\Program Files\winrar\rar" a -r sc8800g_openphone_240x400PDA_rvct_le.rar sc8800g_openphone_240x400PDA_rvct_le
Pause

md \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_MAINTRUNK\%MOCOR_LABEL%
copy /y %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\lib\*.rar \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_MAINTRUNK\%MOCOR_LABEL%
del /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\lib\*.rar

pushd %MOCOR_IDH_VIEW%
move /y %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\lib\sc8800g_openphone_240X400PDA_rvct_le  %MOCOR_VIEW%\MS_Code\lib

pause

rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\wintone
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\wap
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\ocr
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\dictionary
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\t9
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\s9
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\Java
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\qq
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\TencentMcare
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\email
rmdir /s /q %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source\Third-party\formal

pause

echo ========================step5 打包IDH source=========
"C:\Program Files\winrar\rar" a -r %MOCOR_LABEL%_Source.rar %MOCOR_LABEL%_Source
copy /y %MOCOR_LABEL%_Source.rar \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_MAINTRUNK\%MOCOR_LABEL%

rename %MOCOR_IDH_VIEW%\%MOCOR_LABEL%_Source MS_Code
rem del /f /q %MOCOR_LABEL%_Source.rar
pause
