echo "starting building ......"
set jobn=16
set jn=8

for %%p in (AA AE2 AE3 AH EA ED EI ER IC IM IJ IE MF MS MA MG MD ME MR MH MP QA ZZZZ) do call:buildAllRes %%p
echo "Building Other Projects resource starting ...... "

make\make_cmd\make -r -R p=UIS8910FF_refphone job=%jobn% -j%jn% PID=QA m="resource_main" > err_3
make\make_cmd\make -r -R p=UIS8910FF_refphone job=%jobn% -j%jn% PID=MH m="resource_main" > err_3
make\make_cmd\make -r -R p=UIS8910FF_refphone job=%jobn% -j%jn% PID=AA m="resource_main" > err_3
make\make_cmd\make -r -R p=UIS8910FF_refphone job=%jobn% -j%jn% PID=IM m="resource_main" > err_3
make\make_cmd\make -r -R p=UIS8910FF_refphone job=%jobn% -j%jn% PID=EI m="resource_main" > err_3
echo "build UIS8910FF_refphone finish"

make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim job=%jobn% -j%jn% PID=QA m="resource_main" > err_4
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim job=%jobn% -j%jn% PID=MH m="resource_main" > err_4
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim job=%jobn% -j%jn% PID=AY m="resource_main" > err_4
echo "build UIS8910FF_mpphone_evt_dualsim MH & QA finish"

make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_ct job=%jobn% -j%jn% PID=QA m="resource_main" > err_5
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_ct job=%jobn% -j%jn% PID=MR m="resource_main" > err_5
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_ct job=%jobn% -j%jn% PID=MH m="resource_main" > err_5
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_ct job=%jobn% -j%jn% PID=IM m="resource_main" > err_5
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_ct job=%jobn% -j%jn% PID=EI m="resource_main" > err_5
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_ct job=%jobn% -j%jn% PID=AA m="resource_main" > err_5
echo "build UIS8910FF_mpphone_evt_dualsim_ct finish"

make\make_cmd\make -r -R p=UIS8910FF_refphone_user job=%jobn% -j%jn% PID=QA m="resource_main" > err_6
make\make_cmd\make -r -R p=UIS8910GF_192X256_320X240_refphone job=%jobn% -j%jn% PID=QA m="resource_main" > err_6
echo "build User QA and GF MR finish"

make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_mp job=%jobn% -j%jn% PID=QA m="resource_main" > err_7
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_mp job=%jobn% -j%jn% PID=AA m="resource_main" > err_7
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_mp job=%jobn% -j%jn% PID=MH m="resource_main" > err_7
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_mp job=%jobn% -j%jn% PID=MR m="resource_main" > err_7
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_mp job=%jobn% -j%jn% PID=IM m="resource_main" > err_7

make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim_ct job=%jobn% -j%jn% PID=MR m="resource_main" > err_8

make\make_cmd\make -r -R p=UIS8910FF_mpphone_ds_nosimlock job=%jobn% -j%jn% PID=AA m="resource_main" > err_9
make\make_cmd\make -r -R p=UIS8910FF_mpphone_ds_nosimlock job=%jobn% -j%jn% PID=QA m="resource_main" > err_9
make\make_cmd\make -r -R p=UIS8910FF_mpphone_ds_nosimlock job=%jobn% -j%jn% PID=IM m="resource_main" > err_9

make\make_cmd\make -r -R p=UIS8910FF_DS_MP job=%jobn% -j%jn% PID=QA m="resource_main" > err_10
make\make_cmd\make -r -R p=UIS8910FF_DS_MP job=%jobn% -j%jn% PID=AA m="resource_main" > err_10

make\make_cmd\make -r -R p=UIS8910FF_SS_MP job=%jobn% -j%jn% PID=AA m="resource_main" > err_10
make\make_cmd\make -r -R p=UIS8910FF_SS_MP job=%jobn% -j%jn% PID=EI m="resource_main" > err_10

make\make_cmd\make -r -R p=UIS8910FF_SS_MP_ORTH job=%jobn% -j%jn% PID=ER@23QUE71OD00 m="resource_main" > err_11
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim_orth job=%jobn% -j%jn% PID=ER@23QUE71OD00 m="resource_main" > err_11

echo "starting building dualsim_vf ......"
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_vf job=%jobn% -j%jn% PID=VF m=resource_main >err_12

echo "starting building singlesim_vf ......"
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim_vf job=%jobn% -j%jn% PID=VF m=resource_main >err_12

echo "starting building DS_MP_vf ......"
make\make_cmd\make -r -R p=UIS8910FF_DS_MP_vf job=%jobn% -j%jn% PID=VF m=resource_main >err_12

echo "starting building SS_MP_vf ......"
make\make_cmd\make -r -R p=UIS8910FF_SS_MP_vf job=%jobn% -j%jn% PID=VF m=resource_main >err_12

echo.&pause&GOTO:EOF

:buildAllRes
echo start building project=UIS8910FF_mpphone_evt_dualsim  PID=%1 
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim job=%jobn% -j%jn% PID=%1 m="resource_main" > err_1

echo start building project=UIS8910FF_mpphone_evt_singlesim  PID=%1 
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim job=%jobn% -j%jn% PID=%1 m="resource_main" > err_2
echo " Builing AA AE2 AE3 AH EA ED EI ER IC IM IJ IE MF MS MA MG MD ME MR MH MP QA VF ZZZZ ......"
GOTO:EOF