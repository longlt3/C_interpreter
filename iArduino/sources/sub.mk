# Add inputs and outputs from these tool invocations to the build variables 
OBJS += \
out/main_cpp_.o \
out/test_cpp_.o \

DEPS += \
out/main_cpp_.d \
out/test_cpp_.d \

SRCS += \
main.cpp \
test.cpp \


# Each subdirectory must supply rules for building sources it contributes
out/main_cpp_.o: ./main.cpp 
	@/bin/echo -e 'Building target #1: '$(COLOR_GRN)'$<'$(COLOR_WHE)
	@/bin/echo '--------------------------------------'
	"$(cppBuild)" -MMD -MP -MF"$(@:%.o=%.d)" @"$(MakeResDirPath)/includes.args" $(CFLAGS) $(INCFLAGS) "$<" @"$(MakeSrcDirPath)/main_cpp_.args" @"$(MakeSrcDirPath)/sub.args" @"$(MakeResDirPath)/common.args" $(LDFLAGS) $(LIBFLAGS) -o "$@"
	@/bin/echo '======================================'
	@/bin/echo -e ''$(COLOR_BLE)'Finished building: $@'$(COLOR_WHE)''
	@/bin/echo ' '

out/test_cpp_.o: ./test.cpp 
	@/bin/echo -e 'Building target #2: '$(COLOR_GRN)'$<'$(COLOR_WHE)
	@/bin/echo '--------------------------------------'
	"$(cppBuild)" -MMD -MP -MF"$(@:%.o=%.d)" @"$(MakeResDirPath)/includes.args" $(CFLAGS) $(INCFLAGS) "$<" @"$(MakeSrcDirPath)/test_cpp_.args" @"$(MakeSrcDirPath)/sub.args" @"$(MakeResDirPath)/common.args" $(LDFLAGS) $(LIBFLAGS) -o "$@"
	@/bin/echo '======================================'
	@/bin/echo -e ''$(COLOR_BLE)'Finished building: $@'$(COLOR_WHE)''
	@/bin/echo ' '

