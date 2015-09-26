CXXFLAGS=-O2

all: pearson-rm pearson-cm

pearson-rm: pearson.cpp
	$(CXX) $(CXXFLAGS) -S -o $@$(CXXFLAGS).s -c $^
	$(CXX) $(CXXFLAGS) -o $@ -DMATRIX_ORDER_ROW_MAJOR $^

pearson-cm: pearson.cpp
	$(CXX) $(CXXFLAGS) -S -o $@$(CXXFLAGS).s -c $^
	$(CXX) $(CXXFLAGS) -o $@ -UMATRIX_ORDER_ROW_MAJOR $^

clean:
	rm -rf pearson-rm pearson-cm *.s *.dSYM
