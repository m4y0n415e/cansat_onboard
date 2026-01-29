@echo off  
echo Generating Doxygen documentation...  
doxygen Doxyfile  
 
echo Compiling LaTeX to PDF...  
cd docs/latex  
pdflatex refman.tex > NUL 2>&1  # Suppress output (remove > NUL... to debug)  
pdflatex refman.tex > NUL 2>&1  
 
echo PDF generated! Opening...  
start refman.pdf  # Opens the PDF automatically  
 
cd ..  # Return to project root  
echo Done! 