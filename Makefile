.PHONY: all clean format

all:
	./run.sh

clean:
	@rm -rf *.elf *.map

format:
	clang-format -i *.c *.h