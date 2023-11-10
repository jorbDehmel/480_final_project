writeup.pdf:	writeup.tex
	pdflatex writeup.tex

clean:
	rm *.aux *.pdf *.o *.out *.log
