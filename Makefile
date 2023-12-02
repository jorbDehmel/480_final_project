writeup.pdf:	writeup.tex
	echo "Building table of contents..."
	pdflatex writeup.tex

	echo "building final pdf..."
	pdflatex writeup.tex

clean:
	rm *.aux *.o *.out *.log writeup.pdf
