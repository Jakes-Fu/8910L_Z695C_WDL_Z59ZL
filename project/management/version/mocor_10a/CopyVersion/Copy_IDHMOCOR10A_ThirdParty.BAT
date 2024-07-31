SET MOCOR_VIEW=D:\Yihui\yihui.hu_MOCOR10A_view
SET MOCOR_LABEL=MOCOR_10A.W11.44

rmdir /s /q D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party
del /q D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party.rar
del /q \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_10A\%MOCOR_LABEL%\%MOCOR_LABEL%_Third-party.rar


xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\formal D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\qq D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\qq
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\TencentMcare D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\TencentMcare
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\Mplapi\Arm_lib D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\Mplapi\Arm_lib
attrib -r /s /d D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\Mplapi\Arm_lib\normal\mplapi*.a
copy /y %MOCOR_VIEW%\MS_Code\build\sc8800g2_openphone_le_builddir\lib\mplapi*.a D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\Mplapi\Arm_lib\normal 
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\MS_MMI\source\mmi_app\app\ia_java D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\ia_java_source
xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\email D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\email

rem xcopy /e /i /y %MOCOR_VIEW%\MS_Code\Third-party\cstar D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\cstar
rem rmdir /s /q D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\Dm
rem rmdir /s /q D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\Jungo
rem rmdir /s /q D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\Mplapi
rem rmdir /s /q D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party\netfront

pushd D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party

"C:\Program Files\winrar\rar" a -r D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party.rar

md \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_10A\%MOCOR_LABEL%
copy /y D:\Yihui\TMP_VERSION\%MOCOR_LABEL%_Third-party.rar \\scmserver\Test_Products_Library\2011-2\PLD\MOCOR\MOCOR_10A\%MOCOR_LABEL%
