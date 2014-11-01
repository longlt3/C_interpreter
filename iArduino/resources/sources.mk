#.o files: 
OBJS :=

#.d files: 
DEPS :=

#source files: 
SRCS :=

#linker files: 
LINKER_FILES :=

#compile arguments: 
INCFLAGS :=
LIBFLAGS :=

#running file name: 
PROGRAM := program.exe

#clean command: 
RM := rm -f

#GNU build for cpp: 
cppBuild := $(GNUMakeInstallDirPath)g++
#Program build command: 
progBuild := $(cppBuild)
