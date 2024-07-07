CPP = g++
CPPFLAGS := -lstdc++ -std=c++17 -Wall -Werror -Wextra
LDFLAGS =

DIST_DIR=s21_SmartCalc-2.0
WORK_DIR=view
INST_DIR=~/SmartCalc-2.0
APP_NAME=smart_calc


EXECUTABLE = test_calc
SRC_TEST = test/calc.cc
SRC_MODEL = model/calc_model.cc
SRC_CONTROLLER = controller/calc_controller.cc
SRC_CREDIT_MODEL = model/credit_model.cc
SOURCE = $(SRC_TEST) $(SRC_MODEL) $(SRC_CONTROLLER) $(SRC_CREDIT_MODEL)

UNAME = $(shell uname)

LEAKS :=

ifeq ($(UNAME), Linux)
OPEN_REPORT = xdg-open report/html/index.html
LEAKS += valgrind --leak-check=full --tool=memcheck -s --track-origins=yes
LDFLAGS += --coverage
GTEST_LIB = -lgtest -pthread
endif

ifeq ($(UNAME), Darwin)
OPEN_REPORT = open report/html/index.html
LEAKS += leaks -atExit --
LDFLAGS += -fprofile-arcs --coverage
GTEST_LIB = -lgtest
endif

all: clean dist install gcov_report 

tests: clean
	$(CPP) -c $(CPPFLAGS) $(SOURCE) $(GTEST_LIB) $(LDFLAGS)
	$(CPP) $(CPPFLAGS) *.o -o $(EXECUTABLE) $(GTEST_LIB) $(LDFLAGS)
	./$(EXECUTABLE)

leaks_check: tests
	@$(LEAKS) ./$(EXECUTABLE)

gcov_report: clean
	mkdir -p report
	@$(CPP) $(CFLAGS) $(SOURCE) $(GTEST_LIB) -o $(EXECUTABLE) $(LDFLAGS)
	@./$(EXECUTABLE)
	lcov --capture --directory . --output-file report/coverage.info
	lcov --remove report/coverage.info '/usr/*' '*/gtest/*' '*/v1/*' --output-file report/coverage_filtered.info
	genhtml report/coverage_filtered.info --output-directory report/html
	$(OPEN_REPORT)


check:
	@echo
	@echo ------------------------------------------ CHECK START ------------------------------------------------
	clang-format -n --style=Google view/*.cc
	clang-format -n --style=Google view/*.h
	clang-format -n --style=Google model/*.h
	clang-format -n --style=Google model/*.cc
	clang-format -n --style=Google controller/*.h
	clang-format -n --style=Google controller/*.cc
	clang-format -n --style=Google test/*.cc
	@echo ------------------------------------------ CHECK END --------------------------------------------------
	@echo


clean: uninstall
	-rm -rf *.o *.html *.gcda *.gcno *.css *.a *.gcov *.info *.out *.cfg *.txt a.out.dSYM tests main 
	-rm -rf report
	-rm -rf $(WORK_DIR)/$(APP_NAME)
	-rm -rf $(INST_DIR)
	-rm -rf *.tar
	-rm -rf build
	-rm -rf $(EXECUTABLE)

install:
	@echo "===>    Installation has started     <==="
	cd $(WORK_DIR); qmake $(APP_NAME).pro
	cd $(WORK_DIR); make
	cd $(WORK_DIR); make clean 
	cd $(WORK_DIR); rm -rf Makefile
	cd $(WORK_DIR); rm -rf .qmake.stash
	-mkdir $(INST_DIR)
	cd $(WORK_DIR); mv smart_calc $(INST_DIR)/s21_smart_calc_v2.0
	open $(INST_DIR)

uninstall:
	-rm -rf $(INST_DIR)

dist:
	-mkdir $(DIST_DIR)
	-mkdir $(DIST_DIR)/$(WORK_DIR)
	cp Makefile manual.texi $(DIST_DIR)
	cp -R $(WORK_DIR) $(DIST_DIR)
	tar --totals -cvf $(DIST_DIR).tar $(DIST_DIR)
	-rm -rf $(DIST_DIR)

run: install
	$(INST_DIR)/./s21_smart_calc_v2.0

dvi:
	open ./manual/index.html

dvi_install:
	makeinfo manual.texi --html
	open ./manual/index.html

develop: clean
	@echo --------------------------------- DEVELOP START ---------------------------------------
	clang-format -n --style=Google model/*.h
	clang-format -n --style=Google model/*.cc
	clang-format -n --style=Google controller/*.h
	clang-format -n --style=Google controller/*.cc
	cppcheck --language=c++ --std=c++17 --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction console_view.cc
	$(CPP) -c $(CPPFLAGS) console_view.cc model/calc_model.cc controller/calc_controller.cc model/credit_model.cc
	$(CPP) $(CPPFLAGS) *.o -o main
	@echo
	./main
	@echo --------------------------------- DEVELOP END -----------------------------------------
	@echo


.PHONY: all clean test gcov_report style clang_format leaks_check run