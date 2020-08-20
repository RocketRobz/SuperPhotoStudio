#---------------------------------------------------------------------------------
# Goals for Build
#---------------------------------------------------------------------------------
.PHONY: all nds 3ds

all:	nds 3ds

nds:
	@$(MAKE) -C nds

3ds:
	@$(MAKE) -C 3ds

clean:
	@echo clean build directories
	@$(MAKE) -C nds clean
	@$(MAKE) -C 3ds clean
