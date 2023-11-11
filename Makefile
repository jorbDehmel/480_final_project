writeup.pdf:	writeup.tex
	pdflatex writeup.tex

clean:
	rm *.aux *.o *.out *.log writeup.pdf
