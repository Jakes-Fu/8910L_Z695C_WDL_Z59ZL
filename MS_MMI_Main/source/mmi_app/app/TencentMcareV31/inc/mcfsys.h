#ifndef _MCFSYS_H_
#define _MCFSYS_H_

#include "mcfcommon.h"
//#include"mcare_id.h"

#define MCARE_SCREEN_BASE   (MCARE_WIN_ID_START_PLAT)//���ܷ��ڿ��Ҫ���ų���
#define MCARE_SCREEN_HOME   (MCARE_WIN_ID_START_PLAT+1)//���ܷ��ڿ��Ҫ���ų���
#define MCARE_SCREEN_EDITOR (MCARE_WIN_ID_START_PLAT+2)//���ܷ��ڿ��Ҫ���ų���

typedef MVOID (*pfnFunc)(MVOID);
typedef MBYTE (*pfnDelFunc)(MVOID*);

//˵��������mcare UIǰ��Ҫִ�еĶ���������MTK���EntryNewScreen������ƽ̨�ɱ���inactive��resume����uiǰ��һЩ׼����ʵ�֡�
MINT McfSys_Init(pfnFunc inactiveFunc, pfnFunc deepInactiveFunc, pfnFunc resumeFunc, pfnFunc exitFunc);

//�̻�����á��˳�Mcare UI��Ҫ���õĺ�����
//nExitType �C �˳���־�������ϴε�λ�û����˳���idle��
MINT McfSys_DeInit(MINT nExitType);


//˵�����̻�����ã���̨�һ���Ҫִ�еĲ����� 
//nExitType �C �˳���־�������ϴε�λ�û����˳���idle��
MINT McfSys_Background(MINT nExitType);


//============================= �ⲿ���ú��� =====================================
//˵�����ɹ�����ƽ̨���ⲿ���ã��һ��ָ�ǰ��Ҫִ�еĲ�����
MINT McfSys_Foreground(MVOID);

//˵����ֱ�ӵ��ù̻����Suspend�ĵ�������������ƽ̨��suspend��Ϣ�ɵ��á�
MINT McfSys_Suspend(MVOID);

//˵����ֱ�ӵ��ùһ���Resume�ĵ�������������ƽ̨�յ�resume��Ϣ�ɵ���
MINT McfSys_Resume(MVOID);
//====================================================================

MVOID McfSys_InvalidateCpuCache(MBYTE *start, MUINT size);


#endif//_MCFSYS_H_

