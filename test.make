g++ test.cpp \
    dlstorm.cpp  \
    md5.cpp \
    c_log.cpp \
    c_data.cpp \
    c_gfx.cpp \
    -o test \
    -D _DLCS_SDL \
    -Wno-write-strings \
    -Wno-format-security \
    -I /home/sparson/develop/projects/dlstorm \
    -I /home/sparson/develop/projects/dlstorm/sys \
    -I /home/sparson/develop/projects/dlstorm/pre \
    -I /usr/include/tcl8.6 \
    -I /home/sparson/develop/projects/SDK/SDL-1.2.15/include  \
    -I /usr/include/GL/ \
    -I /home/sparson/develop/projects/SDK/DevIL-SDK-x86-1.7.8/include/IL



# the compiler: gcc for C program, define as g++ for C++
# CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
# CFLAGS  = -g -Wall

# the build target executable:
# TARGET = myprog

# all: $(TARGET)

# $(TARGET): $(TARGET).c
# $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

# clean:
# $(RM) $(TARGET)