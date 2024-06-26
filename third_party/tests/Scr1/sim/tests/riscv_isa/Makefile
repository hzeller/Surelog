
include rv32_tests.inc

override ARCH := imfc

src_dir := $(CURDIR)
obj_dir   := $(bld_dir)/riscv_objs
test_list := $(patsubst %.S, %, $(notdir $(rv32_isa_tests)))
objs      := $(addprefix $(obj_dir)/,$(test_list:%=%.o))
test_elf  := $(addprefix $(bld_dir)/,$(test_list:%=%.elf))
test_hex  := $(addprefix $(bld_dir)/,$(test_list:%=%.hex))
test_dump := $(addprefix $(bld_dir)/,$(test_list:%=%.dump))

CFLAGS := -I$(inc_dir) -I$(src_dir) -DASM -Wa,-march=rv32$(ARCH) -march=rv32$(ARCH) -mabi=ilp32f -D__riscv_xlen=32 
LDFLAGS := -static -fvisibility=hidden -nostdlib -nostartfiles -T$(inc_dir)/link.ld -march=rv32$(ARCH) -mabi=ilp32f

VPATH += $(src_dir) $(bld_dir) $(obj_dir) $(RISCV_TESTS)

default: check_riscv_tests $(test_elf) $(test_hex) $(test_dump)

define compile_template
$(obj_dir)/$$(basename $(notdir $(SRC))).o: $$(SRC) | $(obj_dir)
	$(RISCV_GCC) -c $$< $(CFLAGS) -o $$@
 endef

$(foreach SRC,$(rv32_isa_tests), $(eval $(compile_template)))

$(obj_dir) :
	mkdir -p $(obj_dir)

$(bld_dir)/%.elf: $(obj_dir)/%.o | $(obj_dir)
	$(RISCV_GCC) $^ $(LDFLAGS) -o $@

$(bld_dir)/%.hex: $(bld_dir)/%.elf
	$(RISCV_OBJCOPY) $^ $@

$(bld_dir)/%.dump: $(bld_dir)/%.elf
	$(RISCV_OBJDUMP) $^ > $@

clean:
	$(RM) $(test_elf) $(test_hex) $(test_dump) $(objs)
	$(RM) -R $(obj_dir)


.PHONY: check_riscv_tests

riscv_tests_dir    := $(if $(RISCV_TESTS), $(RISCV_TESTS), ./undefined)
riscv_tests_commit := a9433c4daa287fbe101025f2a079261a10149225
## commit hash readed from local copy of https://github.com/riscv/riscv-tests
tmp_commit = $(shell cd $(riscv_tests_dir) 2>/dev/null && git log -1 | grep "commit" | cut -f2 -d ' ')
is_commit_good = $(if $(subst $(riscv_tests_commit),,$(tmp_commit)),false,true)

# Color
RED=\033[0;31m
NC=\033[0m

check_riscv_tests : $(riscv_tests_dir)
	@if [ ! -d $(riscv_tests_dir) ]; then \
		echo -e "$(RED)==========================================================================" &&\
		echo "   Error! Environment variable RISCV_TESTS='$(riscv_tests_dir)' " &&\
		echo "      directory not exist!" && \
		echo "==========================================================================$(NC)" ; \
	fi
ifneq (,$(is_repo_changed))
	@echo -e "$(RED)=========================================================================="
	@echo "   Error! Repo '$(riscv_tests_dir)' "
	@echo "     must be unchanged!"
	@echo -e "==========================================================================$(NC)"
	exit 1
endif
ifneq ($(is_commit_good),true)
	@echo -e "$(RED)=========================================================================="
	@echo "   Error! riscv-tests must point to commit $(riscv_tests_commit)"
	@echo -e "==========================================================================$(NC)"
	exit 1
endif

$(riscv_tests_dir) :.
ifndef RISCV_TESTS
	@echo -e "$(RED)=========================================================================="
	@echo "    Error! Environment variable RISCV_TESTS not set!"
	@echo "    You must set the environment variable RISCV_TESTS"
	@echo "    The variable should point to the local copy of the"
	@echo "      repository https://github.com/riscv/riscv-tests"
	@echo "      with the commit $(riscv_tests_commit)"
	@echo -e "==========================================================================$(NC)"
	exit 1
endif
