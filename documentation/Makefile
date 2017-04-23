
MDFILES = abstract.md introduction.md overview.md threads.md handle.md

.PHONY: doc

doc: doc.md

doc.md: $(MDFILES:%=text/%)
	cat $+ > doc.md

