# ATA
Automated Test Assembly in R

# How To

1. Install g++-Compiler (e.g., [Rtools](https://cran.r-project.org/bin/windows/Rtools/) for windows)
2. Run the following lines
```{r}
install.packages('devtools')
library('devtools')
install_github('thkiefer/ATA')
library(ATA)
?ata_random

design <- ata_random(200, 5, 25)  

?ata_sa_bib #not working properly yet
``` 

# Licence

[GPL (>= 3)](http://www.gnu.org/licenses/gpl-3.0.en.html).
