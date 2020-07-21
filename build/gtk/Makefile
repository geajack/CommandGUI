TARGET = ../../bin/CommandGUI
OBJECTS = artifacts/ApplicationGTK.o artifacts/cJSON.o artifacts/CommandDescriptor.o artifacts/CommandManager.o artifacts/CommandPage.o artifacts/CommandTemplateParser.o artifacts/main.o artifacts/MainWindow.o

VPATH = ../../lib ../../src ../../src/core ../../src/ui/gtk

all: $(TARGET)

$(TARGET): $(OBJECTS)
	./link

artifacts/%.o: %.cpp
	./compile $<

artifacts/%.o: %.c
	./compile $<