.PHONY: build install

TARGET=lstree
CC=clang
build:
	 $(CC) $(TARGET).c -o $(TARGET)
run: build
	./$(TARGET)
install: build
	mkdir -p ~/.local/bin/
	cp $(TARGET) ~/.local/bin/
