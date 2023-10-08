
.PHONY: all
all: build
	ninja -C build

build:
	meson build

.PHONY: watch
watch: build
	while inotifywait -e close_write,moved_to,create src test; do make test; done

.PHONY: test
test: build
	meson test -C build --print-errorlogs --no-stdsplit --verbose

.PHONY: install
install: build
	ninja -C build install

.PHONY: uninstall
uninstall: build
	ninja -C build uninstall

.PHONY: clean
clean:
	rm -rf build
