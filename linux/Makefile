SRCDIR = ../src
OBJDIR = ../obj
BINDIR = ../bin

DIRS = $(OBJDIR) $(BINDIR)

LIBS = -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lpng -lfreetype

CC = g++ -g

CFLAGS = -Wall -std=c++11

TGT    = game
TGTDIR = ..


OBJS=\
      $(OBJDIR)/helpers.o \
      $(OBJDIR)/shader.o \
      $(OBJDIR)/shaderProgram.o \
      $(OBJDIR)/node.o \
      $(OBJDIR)/camera.o \
      $(OBJDIR)/mesh.o \
      $(OBJDIR)/texture.o \
      $(OBJDIR)/meshManager.o \
      $(OBJDIR)/textureManager.o \
      $(OBJDIR)/shaderProgramManager.o \
      $(OBJDIR)/guiElement.o \
      $(OBJDIR)/guiBar.o \
      $(OBJDIR)/gui.o \
      $(OBJDIR)/scene.o \
      $(OBJDIR)/testScene.o \
      $(OBJDIR)/collisions.o \
      $(OBJDIR)/entity.o \
      $(OBJDIR)/character.o \
      $(OBJDIR)/surface.o \
      $(OBJDIR)/joystick.o \
      $(OBJDIR)/main.o


all: $(TGTDIR)/$(TGT)

 
$(TGTDIR)/$(TGT): $(DIRS) $(BINDIR)/$(TGT)
	cp $(BINDIR)/$(TGT) $(TGTDIR)/$(TGT)
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
	rm -rf $(TGTDIR)/$(TGT)

fresh: clean all

