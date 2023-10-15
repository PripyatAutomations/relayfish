clean:	arduino-clean
	${RM} ${extra_clean}

	@for i in ${clean_dirs}; do \
		${MAKE} -C $$i $@; \
	done

distclean: clean firmware-distclean esp-serial-distclean
	${RM} -r bin/
	${RM} *~ include/*~
	${RM} ${extra_distclean}
	@for i in ${distclean_dirs}; do \
		${MAKE} -C $$i $@; \
	done
