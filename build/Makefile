SUBDIRS = ../Global/ ../Stack/ ../File_parser/ ../Drivers/ ../CPU/Assembler/ ../CPU/SPU/

ASM_SUBDIRS = ../Global/ ../Stack/ ../File_parser/ ../CPU/Assembler/
SPU_SUBDIRS = ../Global/ ../Stack/ ../File_parser/ ../Drivers/ ../CPU/SPU/

TXT_JUNK = $(wildcard *.txt)
BIN_JUNK = $(wildcard *.bin)
EXE_JUNK = $(wildcard ../executables/*.out)
LIB_JUNK = $(wildcard ../libs/*.a)

all:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

asm:
	@for dir in $(ASM_SUBDIRS); do $(MAKE) -C $$dir; done

spu:
	@for dir in $(SPU_SUBDIRS); do $(MAKE) -C $$dir; done

clean_junk:
	@rm $(TXT_JUNK) $(BIN_JUNK) $(EXE_JUNK) $(LIB_JUNK)

clean_spu:
	@for dir in $(SPU_SUBDIRS); do $(MAKE) clean -C $$dir; done

clean_all:
	@for dir in $(SUBDIRS); do $(MAKE) clean -C $$dir; done

clean: clean_junk clean_all
