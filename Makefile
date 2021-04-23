run: resistance
	@for i in {1..4};do\
		cat test/in$$i.txt | ./resistance > test/result$$i.txt;\
		echo "test case $$i:";\
		diff test/out$$i.txt test/result$$i.txt && echo "************** succesfull" || echo "************* failed" ;\
	done
resistance: resistance.cpp
	g++ resistance.cpp -o resistance
clean:
	@if test -e resistance; then rm resistance;fi
	@if test -e test/result1.txt ; then rm test/result*;fi