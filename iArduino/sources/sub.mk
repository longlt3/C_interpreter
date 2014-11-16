# Add inputs and outputs from these tool invocations to the build variables 
OBJS += \
out/debug_c_.o \
out/main_c_.o \
out/resource_c_.o \
out/test_c_.o \

DEPS += \
out/debug_c_.d \
out/main_c_.d \
out/resource_c_.d \
out/test_c_.d \

SRCS += \
debug.c \
main.c \
resource.c \
test.c \


# Each subdirectory must supply rules for building sources it contributes
out/debug_c_.o: ./debug.c 
	@/bin/echo -e 'Building target #1: '$(COLOR_GRN)'$<'$(COLOR_WHE)
	@/bin/echo '--------------------------------------'
	"$(cBuild)" -MMD -MP -MF"$(@:%.o=%.d)" @"$(MakeResDirPath)/includes.args" $(CFLAGS) $(INCFLAGS) "$<" @"$(MakeSrcDirPath)/debug_c_.args" @"$(MakeSrcDirPath)/sub.args" @"$(MakeResDirPath)/common.args" $(LDFLAGS) $(LIBFLAGS) -o "$@"
	@/bin/echo '======================================'
	@/bin/echo -e ''$(COLOR_BLE)'Finished building: $@'$(COLOR_WHE)''
	@/bin/echo ' '

out/main_c_.o: ./main.c 
	@/bin/echo -e 'Building target #2: '$(COLOR_GRN)'$<'$(COLOR_WHE)
	@/bin/echo '--------------------------------------'
	"$(cBuild)" -MMD -MP -MF"$(@:%.o=%.d)" @"$(MakeResDirPath)/includes.args" $(CFLAGS) $(INCFLAGS) "$<" @"$(MakeSrcDirPath)/main_c_.args" @"$(MakeSrcDirPath)/sub.args" @"$(MakeResDirPath)/common.args" $(LDFLAGS) $(LIBFLAGS) -o "$@"
	@/bin/echo '======================================'
	@/bin/echo -e ''$(COLOR_BLE)'Finished building: $@'$(COLOR_WHE)''
	@/bin/echo ' '

out/resource_c_.o: ./resource.c 
	@/bin/echo -e 'Building target #3: '$(COLOR_GRN)'$<'$(COLOR_WHE)
	@/bin/echo '--------------------------------------'
	"$(cBuild)" -MMD -MP -MF"$(@:%.o=%.d)" @"$(MakeResDirPath)/includes.args" $(CFLAGS) $(INCFLAGS) "$<" @"$(MakeSrcDirPath)/resource_c_.args" @"$(MakeSrcDirPath)/sub.args" @"$(MakeResDirPath)/common.args" $(LDFLAGS) $(LIBFLAGS) -o "$@"
	@/bin/echo '======================================'
	@/bin/echo -e ''$(COLOR_BLE)'Finished building: $@'$(COLOR_WHE)''
	@/bin/echo ' '

out/test_c_.o: ./test.c 
	@/bin/echo -e 'Building target #4: '$(COLOR_GRN)'$<'$(COLOR_WHE)
	@/bin/echo '--------------------------------------'
	"$(cBuild)" -MMD -MP -MF"$(@:%.o=%.d)" @"$(MakeResDirPath)/includes.args" $(CFLAGS) $(INCFLAGS) "$<" @"$(MakeSrcDirPath)/test_c_.args" @"$(MakeSrcDirPath)/sub.args" @"$(MakeResDirPath)/common.args" $(LDFLAGS) $(LIBFLAGS) -o "$@"
	@/bin/echo '======================================'
	@/bin/echo -e ''$(COLOR_BLE)'Finished building: $@'$(COLOR_WHE)''
	@/bin/echo ' '

