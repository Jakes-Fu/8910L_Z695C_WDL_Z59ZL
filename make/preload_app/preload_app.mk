include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/preload_C_MACRO.txt)
endif

MMI_RES_DIR := $(strip $(MMI_RES_DIR))

#set preload_img.bin size ,unit is KB
#这个大小应该和内置C盘的大小相匹配，请参照实际分区表确定大小
IMG_SIZE = 4096

OLD_PRELOAD = FALSE

.PHONEY:all
all :builddir preload_def
ifeq ($(strip $(OLD_PRELOAD)), FALSE)
	$(PERL) make/perl_script/preload.pl "$(ROOT_DIR)/$(BUILD_DIR)/preload/preload_def.h" "$(ROOT_DIR)/MS_MMI/source/resource/$(MMI_RES_DIR)" "$(ROOT_DIR)/$(BUILD_DIR)/preload" "$(ROOT_DIR)/$(BUILD_DIR)/img/preload_img.bin" $(IMG_SIZE)
	@-$(PERL) make/perl_script/rm_dir.pl $(BUILD_DIR)/preload ""
else	
  ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
		make\make_cmd\img_proc.exe MS_MMI\source\resource\$(strip $(MMI_RES_DIR))\preload_img\preload_le.img build\$(PROJECT)_builddir\img\preload_img.bin
  else
		make\make_cmd\img_proc.exe MS_MMI\source\resource\$(strip $(MMI_RES_DIR))\preload_img\preload.img build\$(PROJECT)_builddir\img\preload_img.bin
  endif
endif

.PHONEY:builddir
builddir:
ifeq ($(strip $(OLD_PRELOAD)), FALSE)
	@$(PERL) make/perl_script/mk_dir.pl "build" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)" ""
	@$(PERL) make/perl_script/rm_dir.pl preload $(BUILD_DIR)/
	@-$(PERL) make/perl_script/rm_file.pl preload_img.bin $(BUILD_DIR)/img/
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/preload" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/tmp" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/dep" ""
endif


	
.PHONEY:preload_def
preload_def:
ifeq ($(strip $(OLD_PRELOAD)), FALSE)
	@$(ECHO) Compiling preload project file preload_def.h ...
	@$(PERL) make/perl_script/rm_dir.pl $*.h build/$(PROJECT)_builddir/preload/
	@$(ARMCC) -via $(BUILD_DIR)/dep/preload_C_MACRO.txt -D$(PROJECT) -E MS_MMI/source/resource/preload_def.h  -o build/$(PROJECT)_builddir/preload/preload_def.h
	@$(ECHO) $*  build finished
endif	

.PHONY:clean
clean:
	@$(PERL) make/perl_script/rm_file.pl preload_img.bin  build/$(PROJECT)_builddir/img/ 