include gib/bundle/boot.mk

watch: .gib.bin
	@./.gib.bin -w 60

clean:
	@rm -rf _build
