design <- ata_random(200, 5, 25) 
ata_bib <- ata_sa_bib(tcnt = 30, ccnt = 15, pcnt = 4, 
                      fitFnct = ata_sa_default_bibFitFnct, 
                      control = list("t" = 1, "tMin" = 1e-5, "L" = 5e4, "maxGen" = 70, "decr" = (tMin / 1)^(1 / (expMax))), 
                      verbose = TRUE)
