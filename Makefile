CXX_FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
   -Wmissing-declarations -Wcast-qual -Wchar-subscripts                             \
   -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal           \
   -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline                   \
   -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked                     \
   -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo           \
   -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn                         \
   -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default               \
   -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast                    \
   -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing            \
   -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation             \
   -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192                  \
   -Wstack-usage=8192 -fsanitize=address -fsanitize=undefined -fPIE -Werror=vla -Wno-format

CXX = g++


v1:       ./bin/main.o ./bin/hashtable_v1.o
	$(CXX) ./bin/main.o ./bin/hashtable_v1.o $(CXXFLAGS) -o main

./bin/main.o:   src/main.cpp
	$(CXX) -c ./src/main.cpp $(CXXFLAGS) -o ./bin/main.o

./bin/hashtable_v1.o: src/hashtable_v1.cpp hpp/hashtable_v1.hpp
	$(CXX) -c ./src/hashtable_v1.cpp $(CXXFLAGS) -o ./bin/hashtable_v1.o



v1_O3:       ./bin/main_O3.o ./bin/hashtable_v1O3.o
	$(CXX) -O3 ./bin/main_O3.o ./bin/hashtable_v1O3.o $(CXXFLAGS) -o main_O3

./bin/main_O3.o:   src/main.cpp
	$(CXX) -O3 -c ./src/main.cpp $(CXXFLAGS) -o ./bin/main_O3.o

./bin/hashtable_v1O3.o: src/hashtable_v1.cpp hpp/hashtable_v1.hpp
	$(CXX) -O3 -c ./src/hashtable_v1.cpp $(CXXFLAGS) -o ./bin/hashtable_v1O3.o



v2_O3:       ./bin/main2_O3.o ./bin/hashtable_v2O3.o
	$(CXX) -O3 -msse4.2 ./bin/main2_O3.o ./bin/hashtable_v2O3.o $(CXXFLAGS) -o mainv2_O3

./bin/main2_O3.o:   src/main.cpp
	$(CXX) -O3 -msse4.2 -c ./src/main.cpp $(CXXFLAGS) -o ./bin/main2_O3.o

./bin/hashtable_v2O3.o: src/hashtable_v2.cpp hpp/hashtable_v1.hpp
	$(CXX) -O3 -msse4.2 -c ./src/hashtable_v2.cpp $(CXXFLAGS) -o ./bin/hashtable_v2O3.o



v3_O3:       ./bin/main3_O3.o ./bin/hashtable_v3O3.o ./bin/meowcmp.o
	$(CXX) -O3 -msse4.2 ./bin/main3_O3.o ./bin/meowcmp.o ./bin/hashtable_v3O3.o $(CXXFLAGS) -o mainv3_O3

./bin/main3_O3.o:   src/main.cpp
	$(CXX) -O3 -msse4.2 -c ./src/main.cpp $(CXXFLAGS) -o ./bin/main3_O3.o

./bin/hashtable_v3O3.o: src/hashtable_v3.cpp hpp/hashtable_v1.hpp
	$(CXX) -O3 -msse4.2 -c ./src/hashtable_v3.cpp $(CXXFLAGS) -o ./bin/hashtable_v3O3.o

./bin/meowcmp.o:  src/meowcmp.s
	nasm -f elf64 meowcmp.s -o ./bin/meowcmp.o



v4_O3:       ./bin/main4_O3.o ./bin/hashtable_v4O3.o ./bin/meowcmp.o ./bin/strlen_memcpy.o
	$(CXX) -O3 -msse4.2 ./bin/main4_O3.o ./bin/meowcmp.o ./bin/hashtable_v4O3.o ./bin/strlen_memcpy.o $(CXXFLAGS) -o mainv4_O3

./bin/main4_O3.o:   src/main.cpp
	$(CXX) -O3 -msse4.2 -c ./src/main.cpp $(CXXFLAGS) -o ./bin/main4_O3.o

./bin/hashtable_v4O3.o: src/hashtable_v4.cpp hpp/hashtable_v1.hpp
	$(CXX) -O3 -msse4.2 -c ./src/hashtable_v4.cpp $(CXXFLAGS) -o ./bin/hashtable_v4O3.o

./bin/strlen_memcpy.o: src/strlen_memcpy.s
	nasm -f elf64 strlen_memcpy.s -o ./bin/strlen_memcpy.o


parser: src/parser.cpp
	$(CXX) ./src/parser.cpp -o parser

clean:
	rm main parser ./bin/*.o
