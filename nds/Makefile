#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TARGET	:=	SuperPhotoStudio
NITRODATA		:=	nitrofiles

include $(DEVKITARM)/ds_rules

.PHONY: clean makearm7 makearm9

all:	$(TARGET).nds $(TARGET).ds.gba

makearm7:
	$(MAKE) -C arm7
	cp arm7/$(TARGET).elf $(TARGET).arm7.elf

makearm9:
	$(MAKE) -C arm9
	cp arm9/$(TARGET).elf $(TARGET).arm9.elf

$(TARGET).nds:	makearm7 makearm9
	ndstool	-c $(TARGET).nds -7 $(TARGET).arm7.elf -9 $(TARGET).arm9.elf -d $(NITRODATA) \
			-g HPHA 00 "SUPERPHOTO" -z 80040000 -u 00030004 -a 00000138 -b icon.bmp "Super Photo Studio;RocketRobz"

$(TARGET).ds.gba:	$(TARGET).nds
	ndstool	-c $(TARGET)_ds.nds -7 $(TARGET).arm7.elf -9 $(TARGET).arm9.elf -d $(NITRODATA) \
			-h 0x200 -g HPHA 00 "SUPERPHOTO" -b icon.bmp "Super Photo Studio;RocketRobz"
	cat loader.bin $(TARGET)_ds.nds > $(TARGET).ds.gba
	@rm -fr $(TARGET)_ds.nds

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).ds.gba
	@rm -fr $(TARGET).arm7.elf
	@rm -fr $(TARGET).arm9.elf
	@$(MAKE) -C arm9 clean
	@$(MAKE) -C arm7 clean
