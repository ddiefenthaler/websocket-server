
MDFILES = abstract.md introduction.md overview.md threads.md handle.md outlook.md

.PHONY: doc

doc: result/doc.md

result/doc.md: $(MDFILES:%=text/%)
	-mkdir result
	-mv result/doc.md result/doc.md.bak
	cat $+ > result/doc.md

