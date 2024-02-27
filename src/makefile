CC = gcc
CFLAGS = -Wall -Wextra -I/usr/include/gtk-3.0 -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/harfbuzz -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/fribidi -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/x86_64-linux-gnu -I/usr/include/gio-unix-2.0 -I/usr/include/atk-1.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -pthread

LDFLAGS = -lgtk-3 -lgdk-3 -lz -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0

SOURCES = main.c graphics/graphics.c receive/receive.c send/send.c utils/utils.c
OBJECTS = $(SOURCES:.c=.o)  # List of object files

EXECUTABLE = yet_another_messenger_application

# Intermediate target for compilation
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(OBJECTS:.o=.d)

# Target for building the executable and cleaning up
build: $(EXECUTABLE)
	@echo "Build successful."

# Target for cleaning up object and .d files
clean:
	rm -f $(OBJECTS) $(OBJECTS:.o=.d)
	rm -f $(wildcard *.o) $(wildcard *.d) $(wildcard */*.o) $(wildcard */*.d)
	@echo "Cleanup complete."

# Default target
all: build
