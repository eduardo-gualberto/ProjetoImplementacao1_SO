all:
	+$(MAKE) -C implementacao1
	+$(MAKE) -C implementacao2
	+$(MAKE) -C implementacao3

clean:
	rm */build/*