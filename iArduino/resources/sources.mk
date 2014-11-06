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

#GNU build for c: 
cBuild := $(GNUMakeInstallDirPath)gcc
#Program build command: 
progBuild := $(cBuild)
