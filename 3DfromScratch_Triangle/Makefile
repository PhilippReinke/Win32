# Object files to create for the executable
OBJS = obj/resource.o obj/main.o

# Warnings to be raised by the C compiler
WARNS = -Wall

# Names of tools to use when building
CC 	= g++
RC 	= windres
EXE = Triangle.exe

# Linker flags
LDFLAGS = -mwindows

# Compiler flags
CFLAGS = -O2 -D UNICODE -D _UNICODE -D _WIN32_IE=0x0500 -D WINVER=0x0500 ${WARNS} -Iinclude

# Build the exectuable
${EXE}: ${OBJS}
	${CC} -o "$@" ${OBJS} ${LDFLAGS}
	if exist obj\* del /q obj\*
	if exist obj  rmdir obj
	./${EXE}

# Create build output directories if they don't exist
bin obj:
	@if not exist "$@" mkdir "$@"

# Compile object files for executable
obj/%.o: %.cpp | obj
	${CC} ${CFLAGS} -c "$<" -o "$@"

# Build the resources
obj/resource.o: res/resource.rc res/application.manifest | obj
	${RC} -I./include -I./res -i "$<" -o "$@"

# Delete build output
.PHONY: clean
clean:
	if exist obj\* del /q obj\*
	if exist obj  rmdir obj