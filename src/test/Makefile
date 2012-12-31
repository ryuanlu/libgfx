TEST_BINARY:=$(patsubst %.c,%,$(wildcard *.c))
TEST_LDFLAGS:=-I../../include -L../libgfx -lgfx `pkg-config --libs --cflags gtk+-2.0` -g 

all: $(TEST_BINARY)

clean:
	rm -f $(TEST_BINARY)

%: %.c
	@echo "	CCLD	$@"
	@$(CC) $< -o $@ $(TEST_LDFLAGS)
