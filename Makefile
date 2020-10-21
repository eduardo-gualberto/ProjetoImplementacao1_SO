all:
	+$(MAKE) -C implementacao1
	+$(MAKE) -C implementacao2
	+$(MAKE) -C implementacao3
	+$(MAKE) -C implementacao4

clean:
	rm -rf */build