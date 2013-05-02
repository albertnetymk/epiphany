ifeq (,$(filter obj, $(notdir $(CURDIR))))
OBJDIR := obj
MAKETARGET = $(MAKE) $(MFLAGS) --no-print-directory -C $@ -f ../Makefile \
			      ROOT_DIR=.. $(MAKECMDGOALS)

.PHONY: $(OBJDIR)
$(OBJDIR):
	mkdir -p $@
	@$(MAKETARGET)

Makefile : ;

.PHONY: clean
clean:
	@rm -rf $(OBJDIR)/*

% : $(OBJDIR);
else

# PATH := ${PATH}:/home/albert/tmp/tools/gnu/bin:/home/albert/tmp/tools/host/armv7l/bin

#
.PHONY: default
ifeq (albert,$(USER))
default: compile
ESDK_NEW := /home/albert/tmp
ESDK := /home/albert/tmp
else
default: load-run
ESDK_NEW := /opt/adapteva/esdk.3.12.03.29
ESDK := /opt/adapteva/esdk
endif

all :

COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
	# -I${ESDK}/tools/host/armv7l/include

host_program = host/host.elf

ifneq (,$(findstring host,$(MAKECMDGOALS)))
  CC = gcc
  CPPFLAGS = -D__HOST__ -I$(ROOT_DIR)/include \
	-I${ESDK_NEW}/tools/host/x86_64/include
  CFLAGS = -O0 -g3 -Wall -c -fmessage-length=0
  sources = $(addprefix host/, $(notdir $(wildcard $(ROOT_DIR)/host/*.c)))
  objects = $(subst .c,.o,$(sources))
  dependencies += $(subst .o,.d,$(objects))

vpath %.c $(ROOT_DIR)
$(shell mkdir -p host)
host-run : $(host_program)
	./$^

.PHONY: host-compile
host-compile: $(objects)

.PHONY: host-build
host-build : $(host_program)

$(host_program) : $(objects)
	# $(CC) $^ -le-hal -le-loader -L${ESDK}/tools/host/armv7l/lib -o $@
	$(CC) $^ -le_host -L${ESDK}/tools/host/x86_64/lib -o $@

else
  CC = e-gcc
  AR = e-ar
  CPPFLAGS = -I$(ROOT_DIR)/include \
    -I${ESDK_NEW}/tools/gnu/epiphany-elf/sys-include
  ifneq ($(COM_FIFO),)
    CPPFLAGS+=-D$(COM_FIFO)
  endif

  CFLAGS = -O0 -g3 -Wall -c -fmessage-length=0 \
		   -ffp-contract=fast -mlong-calls -mfp-mode=round-nearest


source_to_object = $(subst .c,.o,$1)
# modules := $(wildcard $(ROOT_DIR)/board/*)
modules += $(ROOT_DIR)/board/core0
modules += $(ROOT_DIR)/board/core1
modules += $(ROOT_DIR)/board/core2
modules += $(ROOT_DIR)/board/core3
modules += $(ROOT_DIR)/board/core4
modules += $(ROOT_DIR)/board/core5
vpath %.c $(ROOT_DIR)/board
vpath %.c $(ROOT_DIR)
define make_library
  programs += $1
  $2 : ;
  $(shell mkdir -p $(dir $1))
  objects += $(call source_to_object,$2)
  $1 : $(call source_to_object,$2) common/common.a
	${CC} -T ${ESDK}/bsps/emek3/fast.ldf $$^ -o $$@
endef
$(foreach m, $(modules), \
	$(eval $(call make_library, $(notdir $(m))/main.elf, \
			$(addprefix $(notdir $(m))/, $(notdir $(wildcard $(m)/*.c))))))

dependencies += $(subst .o,.d,$(objects))

common_objects = $(addprefix common/, \
  $(call source_to_object, $(notdir $(wildcard $(ROOT_DIR)/common/*.c))))
$(shell mkdir -p common)
dependencies += $(subst .o,.d,$(common_objects))
common/common.a: $(common_objects)
	$(AR) r $@ $^ 2>/dev/null

main.srec : $(programs)
	$(ROOT_DIR)/translate $^
	cat */*.srec > main.srec

all : main.srec

.PHONY: compile
compile : $(objects) common/common.a
	@$(MAKE) $(MFLAGS) -f ../Makefile ROOT_DIR=.. host-compile

.PHONY: build
build : $(programs)
	@$(MAKE) $(MFLAGS) -f ../Makefile ROOT_DIR=.. host-build

.PHONY: tran
tran : main.srec
	ls $<

.PHONY: server
server :
	# EPIPHANY_HW_DEF_FILE="${ESDK}/bsps/emek3/emek3.hdf"; echo $$EPIPHANY_HW_DEF_FILE; e-server -hdf ${ESDK}/bsps/emek3/emek3.xml
	e-server -xml ${ESDK}/bsps/emek3/emek3.xml -no-test-memory

.PHONY: server-test
server-test :
	e-server -xml ${ESDK}/bsps/emek3/emek3.xml

define switch-branch
	-git branch -D $@
	git checkout -b $@
	cat /dev/null > $(ROOT_DIR)/include/flags.h
	echo 'Destination buffer'
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DESTINATION_BUFFER
	cat /dev/null > $(ROOT_DIR)/include/flags.h
	echo 'Both buffer'
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_BOTH_BUFFER
	cat /dev/null > $(ROOT_DIR)/include/flags.h
	echo 'Double buffer'
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DOUBLE_BUFFER
	git reset --hard
endef

.PHONY: static-test
static-test:
	$(call switch-branch)

.PHONY: acceptance-test
acceptance-test:
	-git branch -D $@
	git checkout -b $@
	cat /dev/null > $(ROOT_DIR)/include/flags.h
	echo 'Destination buffer'
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DESTINATION_BUFFER load
	$(MAKE) $(MFLAGS) -C .. host-build
	./$(host_program) 9
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DESTINATION_BUFFER load
	$(MAKE) $(MFLAGS) -C .. host-build
	./$(host_program) 10
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DESTINATION_BUFFER load
	$(MAKE) $(MFLAGS) -C .. host-build
	./$(host_program) 11

	cat /dev/null > $(ROOT_DIR)/include/flags.h
	echo 'Both buffer'
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_BOTH_BUFFER load
	$(MAKE) $(MFLAGS) -C .. host-build
	./$(host_program) 9
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_BOTH_BUFFER load
	$(MAKE) $(MFLAGS) -C .. host-build
	./$(host_program) 10
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_BOTH_BUFFER load
	$(MAKE) $(MFLAGS) -C .. host-build
	./$(host_program) 11

	cat /dev/null > $(ROOT_DIR)/include/flags.h
	echo 'Double buffer'
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DOUBLE_BUFFER load
	$(MAKE) $(MFLAGS) -C .. host-build
	./$(host_program) 9
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DOUBLE_BUFFER load
	$(MAKE) $(MFLAGS) -C .. host-build
	./$(host_program) 10
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DOUBLE_BUFFER load
	$(MAKE) $(MFLAGS) -C .. host-build
	./$(host_program) 11
	git reset --hard

.PHONY: load
load : main.srec
	e-loader -run-target $< 1>/dev/null 2>/dev/null

.PHONY: load-run
load-run : main.srec
	e-loader -run-target $< 1>/dev/null 2>/dev/null
	@$(MAKE) $(MFLAGS) -f ../Makefile ROOT_DIR=.. host-run
endif

define make_depend
	$(CC) -MM $(CPPFLAGS) $1 | \
	sed 's,\($(notdir $2)\) *:,$2 $3: ,' > $3.tmp
	mv $3.tmp $3
endef

%.o: %.c
	$(call make_depend,$<,$@,$(subst .o,.d,$@))
	$(COMPILE.c) $(OUTPUT_OPTION) $< -o $@


ifneq "$(strip $(MAKECMDGOALS))" "clean"
-include $(dependencies)
endif

endif
