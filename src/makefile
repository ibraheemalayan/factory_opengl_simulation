.DEFAULT_GOAL := run_all

# use the rest as arguments for "run"
RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
# ...and turn them into do-nothing targets
$(eval $(RUN_ARGS):;@:)

PWD := $(shell pwd)

# Standalone GUI
compile_gui:
	@echo "Compiling..."
	gcc ui/ui.c -lGL -lGLU -lglut -lm -o bin/gui.o


run_gui: compile_gui;
	@echo "Running GUI..."
	$(shell export DISPLAY=:0)
	./bin/gui.o

# Debug Standalone GUI
compile_gui_debug:
	@echo "Compiling..."
	gcc ui/ui.c -g -lGL -lGLU -lglut -lm -o bin/d_gui.o


debug_gui: compile_gui_debug;
	@echo "Running GUI..."
	$(shell export DISPLAY=:0)
	gdb ./bin/d_gui.o

#	All
compile_all:
	@echo "Compiling..."
	gcc -pthread cli/main.c -o bin/main.o
#	gcc -pthread cli/child.c -o bin/child.o
	gcc ui/ui.c -lGL -lGLU -lglut -lm -o bin/gui.o

compile_all_for_debug:
	@echo "Compiling..."
	gcc -g -pthread cli/main.c -o bin/d_main.o
#	gcc -g -pthread cli/child.c -o bin/d_child.o
	gcc -g ui/ui.c -lGL -lGLU -lglut -lm -o bin/gui.o

run_all: compile_all;
	@echo "Running CLI..."
	./bin/main.o $(RUN_ARGS)

debug_all: compile_all_for_debug;
	@echo "Running CLI..."
	gdb ./bin/d_main.o

#	StandAlone CLI
test_compile_cli:
	gcc cli/test_sender.c -g -o bin/test1.o
#   gcc cli/test_reader.c -g -o bin/test2.o
#	gcc cli/standalone_child.c -lm -o bin/standalone_child.o


run_test_server: test_compile_cli;
	@echo "Running Test CLI..."
	./bin/test1.o $(RUN_ARGS)

run_test_client: test_compile_cli;
	@echo "Running Test CLI..."
	./bin/test2.o $(RUN_ARGS)
