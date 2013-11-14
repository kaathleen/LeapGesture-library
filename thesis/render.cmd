@echo off

pdflatex thesis-bachelor-english.tex 
bibtex   thesis-bachelor-english
pdflatex thesis-bachelor-english.tex 
pdflatex thesis-bachelor-english.tex 

pdflatex thesis-master-english.tex 
bibtex   thesis-master-english
pdflatex thesis-master-english.tex 
pdflatex thesis-master-english.tex 

pdflatex thesis-bachelor-polski.tex 
bibtex   thesis-bachelor-polski
pdflatex thesis-bachelor-polski.tex 
pdflatex thesis-bachelor-polski.tex 

pdflatex thesis-master-polski.tex 
bibtex   thesis-master-polski
pdflatex thesis-master-polski.tex 
pdflatex thesis-master-polski.tex 


del *.aux *.bak *.log *.blg *.bbl *.toc *.out