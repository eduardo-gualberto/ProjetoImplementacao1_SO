all:
	+$(MAKE) -C implementação_Lucas
	#+$(MAKE) -C implementacao1
	+$(MAKE) -C implementacao2

clean:
	rm */build/*