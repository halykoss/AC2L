# Impostazioni varie
CC=gcc
CFLAG=-ll -ly -o
LEXFLAG=-ll
LEXER=flex
PARSER=bison
CLISP=sbcl
# Controlla se FILE è settato
ifeq ($(FILE),)
FILE := file
endif
# Controlla se OUTPUT è settato
ifeq ($(OUTPUT),)
OUTPUT := output
endif
# Funzione che compila i file FLEX/BISON
define generate_files
	$(LEXER) ac2l.lex
	$(PARSER) -d ac2l.y
	$(CC) lex.yy.c $(CFLAG) ac2l
endef
# Compila i file FLEX/BISON e permette di inserire codice da bash
main:
	$(call generate_files)
	./ac2l
# Esegue il codice scitto da bash in LISP
.PHONY: ac2l
ac2l:
	$(call generate_files)
	./ac2l | $(CLISP)
# Genera l'equivalente lisp del programma C in FILE e lo salva in OUTPUT
.PHONY: out
out:
	mkdir -p out
	$(call generate_files)
	./ac2l < test/$(FILE).txt > out/$(OUTPUT).txt
#  Prende un file in input e lo esegue in LISP
.PHONY: tof
tol:
	$(call generate_files)
	./ac2l < test/$(FILE).txt | $(CLISP)
.PHONY: clean
clean:
	rm -r ac2l.tab.h ac2l.tab.c lex.yy.c ac2l