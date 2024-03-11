SUBDIRS = ../src/global/ ../stack_src/ ../src/assembler/ ../src/SPU/
ASM_SUBDIRS = ../src/global/ ../stack_src/ ../src/assembler/
SPU_SUBDIRS = ../src/global/ ../stack_src/ ../src/SPU/

all:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

asm:
	@for dir in $(ASM_SUBDIRS); do $(MAKE) -C $$dir; done

spu:
	@for dir in $(SPU_SUBDIRS); do $(MAKE) -C $$dir; done


clean_asm:
	@for dir in $(ASM_SUBDIRS); do $(MAKE) clean -C $$dir; done

clean_spu:
	@for dir in $(SPU_SUBDIRS); do $(MAKE) clean -C $$dir; done

clean: clean_asm clean_spu
