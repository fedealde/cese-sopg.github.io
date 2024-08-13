.PHONY: clean all

all:
	@echo "#Cleaning..."
	$(MAKE) --no-print-directory clean
	@echo "\n#Compiling..."
	$(MAKE) --no-print-directory writer
	$(MAKE) --no-print-directory reader
	@echo "\n#Done..."

clean:
	rm -f myfifo writer reader log.txt signals.txt

reader: reader.c
	gcc -std=gnu11 -Wall -pedantic reader.c -o reader

writer: writer.c
	gcc -std=gnu11 -Wall -pedantic writer.c -o writer