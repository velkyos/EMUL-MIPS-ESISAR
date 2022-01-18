MYNAME = Dreano_Louvrier_Robert
SRCDOC = README.md

TESTDIR = testfiles/
FILEDIR = files
OBJDIR = files/obj

IGNORE = A_MakeFile

BIN = emul-mips
PREFIX = MIPS3
CC = gcc
FLAGS = -Wall
C_FILES = $(wildcard $(FILEDIR)/*.c)
OBJ_FILES = $(addprefix $(OBJDIR)/, $(notdir $(C_FILES:.c=.o)))

all: $(OBJ_FILES)
	$(CC) $(FLAGS) -o $(BIN) $^

$(OBJ_FILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(FILEDIR)/%.c
	$(CC) $(FLAGS) -c -o $@ $^

clean:
	rm -fr $(OBJDIR)/*.o *~ $(BIN) *.hex *.s $(basename $(SRCDOC)).pdf *.state

doc: $(SRCDOC)
	pandoc -f markdown -t pdf $(SRCDOC) -o $(basename $(SRCDOC)).pdf

tar: clean
	dir=$$(basename $$PWD) && echo "compressing $(dir)" && cd .. && \
    tar cvfz "$(PREFIX)-$(MYNAME).tgz" --exclude-vcs \
    --transform="s,^$$dir,$(PREFIX)-$(MYNAME)," \
    --exclude="$(IGNORE)" "$$dir" --verbose --show-transformed-names