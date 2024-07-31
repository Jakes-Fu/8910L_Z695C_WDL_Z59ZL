优同TTS在8910上面的使用，

1. Flash版本
   * 在project_xxxx.mk or UIS8910_phone_base_config.cfg 中打开 TTS_SUPPORT = YOUNGTONE
   * 将本目录下resource\×××××.dat 放置到T卡的system\tts\ 目录中

2. Rom 版本
   * 在project_xxxx.mk or UIS8910_phone_base_config.cfg 中打开 TTS_SUPPORT = YOUNGTONE_ROM