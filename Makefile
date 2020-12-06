cc = g++
prom = codesim
deps = $(shell find ./ -maxdepth 1 -name "*.h")
src = $(shell find ./ -maxdepth 1 -name "*.cpp")
obj = $(src:%.c=%.o) 
 
$(prom): $(obj)
	$(cc) -o $(prom) $(obj)
 
%.o: %.c $(deps)
	$(cc) -c $< -o $@
 
clean:
	rm *.o $(prom)
