all:
	+make -C src/libgfx $@
	+make -C src/test $@

clean:
	+make -C src/libgfx $@
	+make -C src/test $@
