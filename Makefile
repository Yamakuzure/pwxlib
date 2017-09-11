all:
	ninja -C build

clean:
	rm -rf build

install:
	DESTDIR=$(DESTDIR) ninja -C build install
