app:
	g++ -o webserver *.h *.cpp -lpthread

clean:
	rm -rf app
