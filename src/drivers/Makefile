PATH_DRIVE_OBJ = ../../obj/drivers_obj/
PATH_DRIVE     = ./

DRIVE_SRC = $(wildcard $(PATH_DRIVE)*.cpp)
DRIVE_OBJ = $(patsubst $(PATH_DRIVE)%.cpp, $(PATH_DRIVE_OBJ)%.o, $(DRIVE_SRC))

PATH_LIB = ../../libs/

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

$(PATH_LIB)libdrivers.a: $(DRIVE_OBJ)
	@ ar rvs $@ $(DRIVE_OBJ)

$(PATH_DRIVE_OBJ)%.o: $(PATH_DRIVE)%.cpp
	@ $(CC) -c $< -o $@ $(FLAGS) -I../SPU/ -I../global/ -I../../stack_src
clean:
	@rm $(PATH_DRIVE_OBJ)*.o
