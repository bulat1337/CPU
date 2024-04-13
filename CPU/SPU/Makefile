PATH_SPU_OBJ    = ../../obj/SPU_obj/
PATH_SPU        = ./src/

SPU_SRC = $(wildcard $(PATH_SPU)*.cpp)
SPU_OBJ = $(patsubst $(PATH_SPU)%.cpp, $(PATH_SPU_OBJ)%.o, $(SPU_SRC))

SPU_TARGET = ../../executables/SPU.out

CC = g++

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

Include = -I./include/ -I../../Global/include/ -I../../Stack/include/ -I../../File_parser/include/ -I../../Drivers/include/ -I/opt/homebrew/Cellar/sfml/2.6.1/include/

$(SPU_TARGET): $(SPU_OBJ)
	@ $(CC) $(LINK_FLAGS) $^ -o $@ -L../../libs -lstack -lglobal -lfile_parser -ldrivers -L/opt/homebrew/Cellar/sfml/2.6.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system

$(PATH_SPU_OBJ)%.o: $(PATH_SPU)%.cpp
	@ $(CC) -c $< -o $@ $(FLAGS) $(Include)

clean:
	@ rm $(SPU_TARGET) $(PATH_SPU_OBJ)*.o
