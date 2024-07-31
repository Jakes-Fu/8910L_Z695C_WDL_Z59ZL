SET MOCOR_LABEL=MOCOR.W11.50
SET MOCOR_VIEW=E:\guowenli\main_6530
SET TEMP_VERSION_PATH=E:\guowenli\TEMP_VERSION

rmdir /s /q %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party
del /q %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party.rar
del /q \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_MAINTRUNK\%MOCOR_LABEL%\%MOCOR_LABEL%_Third-party.rar


xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\formal %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\qq %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\qq
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\TencentMcare %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\TencentMcare
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\Mplapi\Arm_lib %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\Mplapi\Arm_lib
attrib -r /s /d %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\Mplapi\Arm_lib\normal\mplapi*.a
copy /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_240x400PDA_le_builddir\lib\mplapi*.a %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\Mplapi\Arm_lib\normal 
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\MS_MMI\source\mmi_app\app\ia_java %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\ia_java_source
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\email %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\email

rem xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\cstar %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\cstar
rem rmdir /s /q %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\Dm
rem rmdir /s /q %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\Jungo
rem rmdir /s /q %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\Mplapi
rem rmdir /s /q %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party\netfront

pushd %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party

"C:\Program Files\winrar\rar" a -r %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party.rar

md \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_MAINTRUNK\%MOCOR_LABEL%
copy /y %TEMP_VERSION_PATH%\%MOCOR_LABEL%_Third-party.rar \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_MAINTRUNK\%MOCOR_LABEL%
