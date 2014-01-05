SRCDIR = ../src
OBJDIR = ../obj
BINDIR = ../bin

DIRS = $(OBJDIR) $(BINDIR)

LIBS = -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi

CC = g++ -g

CFLAGS = -Wall -std=c++11

TGT = 1401


OBJS=\
      $(OBJDIR)/shader.o \
      $(OBJDIR)/shaderProgram.o \
      $(OBJDIR)/node.o \
      $(OBJDIR)/camera.o \
      $(OBJDIR)/mesh.o \
      $(OBJDIR)/surface.o \
      $(OBJDIR)/main.o


all: $(TGT)

 
$(TGT): $(DIRS) $(BINDIR)/$(TGT)
	@echo "$@ up to date"


$(BINDIR)/$(TGT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CC) $(CFLAGS) -c -o $@ $?

$(BINDIR):
	@mkdir $(BINDIR)

$(OBJDIR):
	python mkdir.py $(OBJDIR)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -rf $(TGT)

fresh: clean all

