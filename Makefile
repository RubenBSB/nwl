build:
	python setup.py build_ext --inplace

install:
	python setup.py install
	
clean:
	rm -rf *.out *.bin *.exe *.o *.a *.so *.egg-info test build dist