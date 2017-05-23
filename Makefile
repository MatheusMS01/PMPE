all:
		+$(MAKE) -C execute_postponed
		+$(MAKE) -C scheduler
		+$(MAKE) -C shutdown
		+$(MAKE) -C hello_node
clean:
		+$(MAKE) -C execute_postponed clean
		+$(MAKE) -C scheduler clean
		+$(MAKE) -C shutdown clean
		+$(MAKE) -C hello_node clean
