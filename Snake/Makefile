# Object files to create for the executable
OBJS = obj/resource.o obj/snake.o obj/score.o obj/main.o

# Warnings to be raised by the C compiler
WARNS = -Wall

# Names of tools to use when building
CC 	= g++
RC 	= windres
EXE = Snake.exe

# Compiler flags. Compile ANSI build only if CHARSET=ANSI.
CFLAGS = -O2 -D UNICODE -D _UNICODE -D _WIN32_IE=0x0500 -D WINVER=0x0500 ${WARNS} -Iinclude

# Linker flags
LDFLAGS = -mwindows

# Build the exectuable
${EXE}: ${OBJS}
	${CC} -o "$@" ${OBJS} ${LDFLAGS}
	if exist obj\* del /q obj\*
	if exist obj  rmdir obj
	./${EXE}

# Create build output directories if they don't exist
obj:
	@if not exist "$@" mkdir "$@"

# Compile object files for executable
obj/%.o: src/%.cpp | obj
	${CC} ${CFLAGS} -c "$<" -o "$@"

# Build the resources
obj/resource.o: res/resource.rc res/application.manifest include/resource.h | obj
	${RC} -I./include -I./res -i "$<" -o "$@"

# C header dependencies
obj/score.o: include/score.h
obj/snake.o: include/snake.h
obj/main.o: include/score.h include/snake.h include/resource.h

# Delete build output
.PHONY: clean
clean:
	if exist obj\* del /q obj\*
	if exist obj  rmdir obj