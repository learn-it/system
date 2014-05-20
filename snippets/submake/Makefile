SUBDIRS = a b
expansion_mode = "define uses simple expansion!"
export exported_check = "Exported variable was not overridden by target-specific definition!"

define expansion_check
	@echo $(expansion_mode)
	@echo "I'm in target '$@' btw..."
endef

define make-recursive
	for subdir in $(SUBDIRS); do \
		make -C $$subdir $@; \
	done
endef

install: expansion_mode = "define uses recursive expansion!"
install: exported_check = "Exported variable was overridden by target-specific definition!"
install:
	$(expansion_check)
	$(make-recursive)
