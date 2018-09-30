CC=gcc
CFLAGS=-Wall -Wextra -g -c -I$(INCDIR)

SRCDIR=src
INCDIR=inc
OBJDIR=obj
TARGET=basic

CODES 	 := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS	 := $(CODES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


all: $(TARGET)

run: $(TARGET)
	@./basic

clean:
	@-rm -rf $(OBJDIR)
	@-rm -f $(TARGET)
	@echo "Clean"


$(TARGET): $(OBJDIR) $(OBJECTS)
	@echo "Create  $(TARGET)"
	@$(CC) -g $(CODES) -o $@ -I$(INCDIR)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)
	@echo Compile $<
	@$(CC) $(CFLAGS) $< -o $@

$(OBJDIR):
	@mkdir $@
