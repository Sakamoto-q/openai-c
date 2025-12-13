.PHONY: remove install

remove:
	$(eval os := $(word 2,$(MAKECMDGOALS)))
	vcpkg remove openai-c:$(os) --overlay-ports=./ports

install:
	$(eval os := $(word 2,$(MAKECMDGOALS)))
	vcpkg install openai-c:$(os) --overlay-ports=./ports

%:
	@: