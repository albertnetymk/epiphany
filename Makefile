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
  ifneq ($(BUFFER_NUMBER),)
    CPPFLAGS+=-DBUFFER_NUMBER=$(BUFFER_NUMBER)
  endif

  CFLAGS = -O0 -g3 -Wall -c -fmessage-length=0 \
		   -ffp-contract=fast -mlong-calls -mfp-mode=round-nearest


source_to_object = $(subst .c,.o,$1)
vpath %.c $(ROOT_DIR)/board
vpath %.c $(ROOT_DIR)/src

libraries :=
libraries += actors
libraries += util
libraries_archive := $(addsuffix .a, $(libraries))
libraries_archive += actors.a
libraries_archive += util.a
libraries := $(addprefix $(ROOT_DIR)/src/, $(libraries))
define make_library
  $2 : ;
  $(shell mkdir -p $1)
  libraries-objects += $(call source_to_object,$2)
  $1.a : $(call source_to_object,$2)
	$(AR) r $$@ $$^ 2>/dev/null
endef
$(foreach l, $(libraries), \
	$(eval $(call make_library, $(notdir $(l)), \
			$(addprefix $(notdir $(l))/, $(notdir $(wildcard $(l)/*.c))))))

dependencies += $(subst .o,.d,$(libraries-objects))

# modules := $(wildcard $(ROOT_DIR)/board/*)
modules += $(ROOT_DIR)/board/core0
modules += $(ROOT_DIR)/board/core1
modules += $(ROOT_DIR)/board/core2
modules += $(ROOT_DIR)/board/core3
modules += $(ROOT_DIR)/board/core4
modules += $(ROOT_DIR)/board/core5
modules += $(ROOT_DIR)/board/core6
modules += $(ROOT_DIR)/board/core7
modules += $(ROOT_DIR)/board/core8
modules += $(ROOT_DIR)/board/core9
modules += $(ROOT_DIR)/board/core10
modules += $(ROOT_DIR)/board/core11
modules += $(ROOT_DIR)/board/core12
modules += $(ROOT_DIR)/board/core13
modules += $(ROOT_DIR)/board/core14
# modules += $(ROOT_DIR)/board/core15
define make_core
  cores += $1
  $2 : ;
  $(shell mkdir -p $(dir $1))
  core-objects += $(call source_to_object,$2)
  $1 : $(call source_to_object,$2) $(libraries_archive)
	${CC} -T ${ESDK}/bsps/emek3/fast.ldf $$+ -o $$@
	$(ROOT_DIR)/checksize.bash $$@
endef
$(foreach m, $(modules), \
	$(eval $(call make_core, $(notdir $(m))/main.elf, \
			$(addprefix $(notdir $(m))/, $(notdir $(wildcard $(m)/*.c))))))

dependencies += $(subst .o,.d,$(core-objects))

main.srec : $(cores)
	$(ROOT_DIR)/translate $^
	cat */*.srec > main.srec

all : main.srec

.PHONY: compile
compile : $(core-objects) $(libraries_archive)
	@$(MAKE) $(MFLAGS) -f ../Makefile ROOT_DIR=.. host-compile

.PHONY: build
build : $(cores)
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
	cat /dev/null > $(ROOT_DIR)/include/util/flags.h
	echo 'Destination buffer'
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DESTINATION_BUFFER
	cat /dev/null > $(ROOT_DIR)/include/util/flags.h
	echo 'Both buffer'
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_BOTH_BUFFER
	cat /dev/null > $(ROOT_DIR)/include/util/flags.h
	echo 'Double buffer'
	$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DOUBLE_BUFFER
	git reset --hard
endef

.PHONY: static-test
static-test:
	$(call switch-branch)

test-input-size = 9 10 15

.PHONY: acceptance-test
acceptance-test:
	-git branch -D $@
	git checkout -b $@
	cat /dev/null > $(ROOT_DIR)/include/util/flags.h
	echo 'Destination buffer'
	for arg in $(test-input-size); do \
		$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DESTINATION_BUFFER load; \
		$(MAKE) $(MFLAGS) -C .. host-build; \
		./$(host_program) $$arg; \
	done

	cat /dev/null > $(ROOT_DIR)/include/util/flags.h
	echo 'Both buffer'
	for arg in $(test-input-size); do \
		$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_BOTH_BUFFER load; \
		$(MAKE) $(MFLAGS) -C .. host-build; \
		./$(host_program) $$arg; \
	done

	cat /dev/null > $(ROOT_DIR)/include/util/flags.h
	echo 'Double buffer'
	for arg in $(test-input-size); do \
		$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_DOUBLE_BUFFER load; \
		$(MAKE) $(MFLAGS) -C .. host-build; \
		./$(host_program) $$arg; \
	done

	cat /dev/null > $(ROOT_DIR)/include/util/flags.h
	echo 'Multiple buffer with 3 arraies'
	for arg in $(test-input); do \
		$(MAKE) $(MFLAGS) -C .. -e COM_FIFO=USE_MULTIPLE_BUFFER -e BUFFER_NUMBER=3 load; \
		$(MAKE) $(MFLAGS) -C .. host-build; \
		./$(host_program) $$arg; \
		done
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

.PHONY: source_actor
source_actor:
	$(ROOT_DIR)/source.pl  $(ROOT_DIR)/src/actors/actor_Scale.c > $(ROOT_DIR)/src/actors/actor_Scale_source.c
	 sed -r 's!actor_Scale!actor_Scale_source!g' $(ROOT_DIR)/include/actors/actor_Scale.h > $(ROOT_DIR)/include/actors/actor_Scale_source.h.tmp
	 echo '#include "util/source.h"' > $(ROOT_DIR)/include/actors/actor_Scale_source.h
	 cat $(ROOT_DIR)/include/actors/actor_Scale_source.h.tmp >> $(ROOT_DIR)/include/actors/actor_Scale_source.h
	 rm $(ROOT_DIR)/include/actors/actor_Scale_source.h.tmp

.PHONY: sink_actor
sink_actor:
	$(ROOT_DIR)/sink.pl  $(ROOT_DIR)/src/actors/actor_Final.c > $(ROOT_DIR)/src/actors/actor_Final_sink.c
	 sed -r 's!actor_Final!actor_Final_sink!g' $(ROOT_DIR)/include/actors/actor_Final.h > $(ROOT_DIR)/include/actors/actor_Final_sink.h.tmp
	 echo '#include "util/sink.h"' > $(ROOT_DIR)/include/actors/actor_Final_sink.h
	 cat $(ROOT_DIR)/include/actors/actor_Final_sink.h.tmp >> $(ROOT_DIR)/include/actors/actor_Final_sink.h
	 rm $(ROOT_DIR)/include/actors/actor_Final_sink.h.tmp


endif
