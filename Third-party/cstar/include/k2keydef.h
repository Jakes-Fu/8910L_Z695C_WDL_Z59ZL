/*******************************************************************************
	K2KeyDef.h

    Descriptin:   vitual key enum define
    
    Copyright (C) 2000-2005 Beijing Komoxo Technology Ltd.

    All rights reserved.
*******************************************************************************/

#ifndef __K2_KEY_DEF_H__
#define __K2_KEY_DEF_H__

#define CS_VK_NONE          0x0

/* �����, ��һ�����Ϲ���û������*/

#define CS_VK_1             0x31
#define CS_VK_2             0x32
#define CS_VK_3             0x33
#define CS_VK_4             0x34
#define CS_VK_5             0x35
#define CS_VK_6             0x36
#define CS_VK_7             0x37
#define CS_VK_8             0x38
#define CS_VK_9             0x39
#define CS_VK_A				0x61
#define CS_VK_BACKSPACE     0x08
#define CS_VK_LEFT          0x25
#define CS_VK_UP            0x26
#define CS_VK_RIGHT         0x27
#define CS_VK_DOWN          0x28
#define CS_VK_SELECT        0x29


/* �����, ��һ�����Ϸֲ���ͬ�Ĺ���, ��ת���ɹ��ܼ�ʹ��*/
/*     Symbian    ��ƴ��\�ʻ�����ΪCS_VK_SPACEʹ��,�������ΪCS_VK_0*/
/*     Smartphone ��ƴ�����뷨����ΪCS_VK_AUXʹ��,���������ΪCS_VK_0*/

#define CS_VK_0             0x30


/* �����, ��һ�����Ϸֲ���ͬ�Ĺ���, ��ת���ɹ��ܼ�ʹ��*/
/*     Symbian    ��ƴ�����뷨����ΪCS_VK_AUXʹ��,���������ΪCS_VK_START*/
/*     Smartphone ��ΪCS_VK_IMEʹ��*/

#define CS_VK_STAR          0x2a


/* �����, ��һ�����Ϸֲ���ͬ�Ĺ���, ��ת���ɹ��ܼ�ʹ��*/
/*     Symbian    ��ΪCS_VK_IMEʹ��*/
/*     Smartphone ��ΪCS_VK_SPACEʹ��*/

#define CS_VK_POUND         0x23


/* ���ܼ�,û�������ֱ�Ӷ�Ӧ,������뷨�������յ����¼�����ֱ�ӽ���Providerʹ��*/

#define CS_VK_SPACE         0x20  /* �ո�\�з�*/
#define CS_VK_AUX           0x41  /* ��ʾAUX(Ŀǰƴ��ר��)*/
#define CS_VK_IME           0x40  /* �л����뷨*/


/* ��  ��,û�������ֱ�Ӷ�Ӧ,Ŀǰ���뷨�����������յ�,��Clientֱ�Ӵ���*/

#define CS_VK_PLUS          0x42

#define CS_VK_LONGCLICK		0x00010000		/* ��ʾ����������Ϣ*/

#endif /*__K2_KEY_DEF_H__*/
