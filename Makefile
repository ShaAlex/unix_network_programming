all:
	make -C client && make -C server

.PHONY: clean
clean:
	find . -name *.o|xargs rm -f
