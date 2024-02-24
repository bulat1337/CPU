PREF_SPU_OBJ    = ../obj/SPU_obj/
PREF_ASM_OBJ    = ../obj/assembler_obj/
PREF_STK_OBJ    = ../obj/stack_obj/
PREF_GLOBAL_OBJ = ../obj/global_obj/
PREF_SPU        = ../src/SPU/
PREF_ASM        = ../src/assembler/
PREF_EXE        = ../executables/
PREF_STK        = ../stack_src/
PREF_GLOBAL     = ../src/global/

CC = g++

TARGET_1 = ../executables/SPU.out
TARGET_2 = ../executables/assembler.out

FLAGS = -D _DEBUG -ggdb3 \
    -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat        \
    -Wmissing-declarations -Wcast-qual -Wchar-subscripts  \
    -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security \
    -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual \
    -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
    -Wstrict-overflow=2 \
    -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef \
    -Wunreachable-code -Wunused -Wvariadic-macros \
    -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs \
    -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow \
    -fno-omit-frame-pointer -Wlarger-than=8192 \
    -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

LINK_FLAGS = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

$(TARGET_1): $(PREF_SPU_OBJ)SPU_main.o $(PREF_SPU_OBJ)SPU.o $(PREF_SPU_OBJ)SPU_additional.o $(PREF_GLOBAL_OBJ)secondary.o $(PREF_STK_OBJ)stack_additional.o $(PREF_STK_OBJ)stack.o
	@ $(CC) $(LINK_FLAGS) $(PREF_SPU_OBJ)SPU_main.o $(PREF_SPU_OBJ)SPU.o $(PREF_SPU_OBJ)SPU_additional.o $(PREF_GLOBAL_OBJ)secondary.o $(PREF_STK_OBJ)stack_additional.o $(PREF_STK_OBJ)stack.o -o $(TARGET_1)

$(PREF_SPU_OBJ)SPU_main.o: $(PREF_SPU)SPU_main.cpp
	@ $(CC) -c $(PREF_SPU)SPU_main.cpp -o $(PREF_SPU_OBJ)SPU_main.o $(FLAGS)

$(PREF_SPU_OBJ)SPU.o: $(PREF_SPU)SPU.cpp
	@ $(CC) -c $(PREF_SPU)SPU.cpp -o $(PREF_SPU_OBJ)SPU.o $(FLAGS)

$(PREF_SPU_OBJ)SPU_additional.o: $(PREF_SPU)SPU_additional.cpp
	@ $(CC) -c $(PREF_SPU)SPU_additional.cpp -o $(PREF_SPU_OBJ)SPU_additional.o $(FLAGS)

$(TARGET_2): $(PREF_ASM_OBJ)asm_main.o $(PREF_ASM_OBJ)assembler.o $(PREF_ASM_OBJ)assembler_additional.o $(PREF_ASM_OBJ)file_parse.o $(PREF_STK_OBJ)stack_additional.o $(PREF_STK_OBJ)stack.o $(PREF_GLOBAL_OBJ)secondary.o
	@ $(CC) $(LINK_FLAGS) $(PREF_ASM_OBJ)asm_main.o $(PREF_ASM_OBJ)assembler.o $(PREF_ASM_OBJ)assembler_additional.o $(PREF_ASM_OBJ)file_parse.o $(PREF_STK_OBJ)stack_additional.o $(PREF_STK_OBJ)stack.o $(PREF_GLOBAL_OBJ)secondary.o -o $(TARGET_2)

$(PREF_ASM_OBJ)asm_main.o: $(PREF_ASM)asm_main.cpp
	@ $(CC) -c $(PREF_ASM)asm_main.cpp -o $(PREF_ASM_OBJ)asm_main.o $(FLAGS)

$(PREF_ASM_OBJ)file_parse.o: $(PREF_ASM)file_parse.cpp
	@ $(CC) -c $(PREF_ASM)file_parse.cpp -o $(PREF_ASM_OBJ)file_parse.o $(FLAGS)

$(PREF_ASM_OBJ)assembler.o: $(PREF_ASM)assembler.cpp
	@ $(CC) -c $(PREF_ASM)assembler.cpp -o $(PREF_ASM_OBJ)assembler.o $(FLAGS)

$(PREF_ASM_OBJ)assembler_additional.o: $(PREF_ASM)assembler_additional.cpp
	@ $(CC) -c $(PREF_ASM)assembler_additional.cpp -o $(PREF_ASM_OBJ)assembler_additional.o $(FLAGS)

$(PREF_STK_OBJ)stack.o: $(PREF_STK)stack.cpp
	@ $(CC) -c $(PREF_STK)stack.cpp -o $(PREF_STK_OBJ)stack.o $(FLAGS)

$(PREF_STK_OBJ)stack_additional.o: $(PREF_STK)stack_additional.cpp
	@ $(CC) -c $(PREF_STK)stack_additional.cpp -o $(PREF_STK_OBJ)stack_additional.o $(FLAGS)

$(PREF_GLOBAL_OBJ)secondary.o: $(PREF_GLOBAL)secondary.cpp
	@ $(CC) -c $(PREF_GLOBAL)secondary.cpp -o $(PREF_GLOBAL_OBJ)secondary.o $(FLAGS)
clean:
	rm $(PREF_EXEJ)*.obj $(PREF_SPU_OBJ)*.o $(TARGET_1) $(PREF_ASM_OBJ)*.o $(TARGET_2)
