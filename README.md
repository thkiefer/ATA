# ATA
Automated Test Assembly in R

# How To

1. Install g++-Compiler (e.g., 
[Rtools](https://cran.r-project.org/bin/windows/Rtools/) for windows)
2. Run the following lines

```{r}
update.packages('devtools')
library('devtools')
# setup_rtools() # if RTools not installed in default directory C:/Rtools/
install_github('thkiefer/ATA')
library(ATA)

?ata_random
design <- ata_random(200, 5, 25) 

?ata_sa_bib
design_bib <- ata_sa_bib(10, 4, 10, control = list("maxGen" = 25, "L" = 5e3))
``` 

# Licence

[GPL (>= 3)](http://www.gnu.org/licenses/gpl-3.0.en.html).

# TODOs

* ~add example itembank~
* make ata_sa_izb run with example itembank (together with ata_sa_bib)
* usability: dynamic fit function constructor
* think of class structure
* sa fit evaluation in C++
* Code-Documentation
    + function header
    + RD-files (there's a method to transfer function header documentation to 
    RD) 
    + setup demo to be used bei demo(package = "ATA")
* usability
    + shiny itembank analyzer
    + ~dynamic fit function constructor~
    + design evaluator / log
    + put names on objects (esp. for dimensions)
* input-check
    + check coherence of control parameters
* specials
    + specialized block designs
        - Youden Square
        - First cluster in each booklet ist rotating
* generalization for multiple possible solvers