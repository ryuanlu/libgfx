TEST_BINARY:=$(patsubst %.c,%,$(wildcard *.c))
TEST_LDFLAGS:=-I../../include -L../libgfx -lgfx -g

all: $(TEST_BINARY)

clean:
	rm -f $(TEST_BINARY)

%: %.c
	@echo "	CCLD	$@"
	@$(CC) $< -o $@ $(TEST_LDFLAGS)
