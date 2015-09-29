CXXFLAGS=-O2
AUXCXXFLAGS=-fno-inline
CXXOMP=clang-omp++

all: ex1 ex2
ex1: pearson-rm pearson-cm 
ex2: simplemc-omp simplemc-st

pearson-rm: pearson.cpp
	$(CXX) $(CXXFLAGS) $(AUXCXXFLAGS) -o $@ -DMATRIX_ORDER_ROW_MAJOR $^

pearson-cm: pearson.cpp
	$(CXX) $(CXXFLAGS) $(AUXCXXFLAGS) -o $@ -UMATRIX_ORDER_ROW_MAJOR $^

simplemc-omp: simplemc.cpp
	$(CXXOMP) -DUSE_OPENMP -fopenmp -o $@ $^

simplemc-st: simplemc.cpp
	$(CXXOMP) -UUSE_OPENMP -o $@ $^

clean:
	rm -rf pearson-rm pearson-cm simplemc-omp simplemc-st *.s *.dSYM
