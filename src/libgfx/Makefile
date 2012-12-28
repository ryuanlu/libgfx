# TARGET_PLATFORM:= (GFX_GLX_BUILD, GFX_WGL_BUILD)
TARGET_PLATFORM:=GFX_GLX_BUILD
OUTPUT_SHARED_OBJECT:=libgfx.so
SOURCE_FILES:=$(wildcard *.c)
OBJECT_FILES:=$(patsubst %.c,%.o,$(SOURCE_FILES))
PKG_CONFIG_PACKAGES:=pangocairo glew gl
LIBGFX_CFLAGS:=-D$(TARGET_PLATFORM) $(CFLAGS) $(shell pkg-config --cflags $(PKG_CONFIG_PACKAGES)) -I../../include -fPIC -Wall -g 
LIBGFX_LDFLAGS:=-shared $(shell pkg-config --libs $(PKG_CONFIG_PACKAGES)) -Wall

all: $(OUTPUT_SHARED_OBJECT)

clean:
	rm -f *.o $(OUTPUT_SHARED_OBJECT)

%.o: %.c
	@echo "	CC	$@"
	@$(CC) -c $< -o $@ $(LIBGFX_CFLAGS)

$(OUTPUT_SHARED_OBJECT): $(OBJECT_FILES)
	@echo "	LD	$@"
	@$(CC) $^ -o $@ $(LIBGFX_LDFLAGS)

