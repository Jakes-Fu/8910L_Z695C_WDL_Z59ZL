/*******************************************************************************
	K2KeyDef.h

    Descriptin:   vitual key enum define
    
    Copyright (C) 2000-2005 Beijing Komoxo Technology Ltd.

    All rights reserved.
*******************************************************************************/

#ifndef __K2_KEY_DEF_H__
#define __K2_KEY_DEF_H__

#define CS_VK_NONE          0x0

/* 物理键, 在一个键上功能没有歧义*/

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


/* 物理键, 在一个键上分布不同的功能, 需转换成功能键使用*/
/*     Symbian    在拼音\笔画中作为CS_VK_SPACE使用,其他情况为CS_VK_0*/
/*     Smartphone 在拼音输入法中作为CS_VK_AUX使用,其他情况下为CS_VK_0*/

#define CS_VK_0             0x30


/* 物理键, 在一个键上分布不同的功能, 需转换成功能键使用*/
/*     Symbian    在拼音输入法中作为CS_VK_AUX使用,其他情况下为CS_VK_START*/
/*     Smartphone 作为CS_VK_IME使用*/

#define CS_VK_STAR          0x2a


/* 物理键, 在一个键上分布不同的功能, 需转换成功能键使用*/
/*     Symbian    作为CS_VK_IME使用*/
/*     Smartphone 作为CS_VK_SPACE使用*/

#define CS_VK_POUND         0x23


/* 功能键,没有与键盘直接对应,如果输入法管理器收到以下键可以直接交给Provider使用*/

#define CS_VK_SPACE         0x20  /* 空格\切分*/
#define CS_VK_AUX           0x41  /* 显示AUX(目前拼音专用)*/
#define CS_VK_IME           0x40  /* 切换输入法*/


/* 虚  键,没有与键盘直接对应,目前输入法管理器不会收到,由Client直接处理*/

#define CS_VK_PLUS          0x42

#define CS_VK_LONGCLICK		0x00010000		/* 表示长按键的消息*/

#endif /*__K2_KEY_DEF_H__*/
