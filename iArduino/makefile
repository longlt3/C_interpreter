first: all
COLOR_WHE="\x1b[0m"
COLOR_GRN="\x1b[32;01m"
COLOR_BLE="\x1b[34;01m"
COLOR_RED="\x1b[31;01m"
COLOR_YEW="\x1b[33;01m"
# All of the sources participating in the build are defined here
-include resources/makefile.local
-include resources/sources.mk
-include resources/userSrc.mk

-include sources/sub.mk

all: $(PROGRAM)

$(PROGRAM): $(OBJS) $(USER_OBJS) $(LINKER_FILES)
	@/bin/echo -e 'Building target #5: '$(COLOR_GRN)'$@'$(COLOR_WHE)
	@/bin/echo '--------------------------------------'
	$(progBuild) @"$(MakeResDirPath)/includes.args" $(CFLAGS) $(INCFLAGS) @"$(MakeResDirPath)/userOBJS.args" @"$(MakeResDirPath)/OBJS.args" @"$(MakeResDirPath)/userLIBS.args" $(LIBFLAGS) $(LDFLAGS) @"$(MakeResDirPath)/makefile.args" -o "$@"
	@/bin/echo '======================================'
	@/bin/echo -e ''$(COLOR_BLE)'Finished building target: $@'$(COLOR_WHE)''
	@/bin/echo ' '

clean:
	-$(RM) $(OBJS) $(DEPS) $(PROGRAM)
	@/bin/echo ' '
