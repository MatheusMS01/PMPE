all:
		+$(MAKE) -C execute_postponed
		+$(MAKE) -C scheduler
		+$(MAKE) -C shutdown
clean:
		+$(MAKE) -C execute_postponed clean
		+$(MAKE) -C scheduler clean
		+$(MAKE) -C shutdown clean
