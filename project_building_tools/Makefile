SUBDIRS = ../src/global/ ../stack_src/ ../src/assembler/ ../src/SPU/

ASM_SUBDIRS = ../src/global/ ../stack_src/ ../src/assembler/
SPU_SUBDIRS = ../src/global/ ../stack_src/ ../src/SPU/

TXT_JUNK = $(wildcard *.txt)
BIN_JUNK = $(wildcard *.bin)

all:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

asm:
	@for dir in $(ASM_SUBDIRS); do $(MAKE) -C $$dir; done

spu:
	@for dir in $(SPU_SUBDIRS); do $(MAKE) -C $$dir; done

clean_junk:
	rm $(TXT_JUNK) $(BIN_JUNK)

clean_asm:
	@for dir in $(ASM_SUBDIRS); do $(MAKE) clean -C $$dir; done

clean_spu:
	@for dir in $(SPU_SUBDIRS); do $(MAKE) clean -C $$dir; done

clean: clean_junk clean_asm clean_spu
