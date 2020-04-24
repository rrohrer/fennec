FENNEC_LDFLAGS := -Ldeps/fennec/build/lib -lfennec
FENNEC_CFLAGS := -Ideps/fennec/include

.fennec:
	@$(MAKE) -C deps/fennec
