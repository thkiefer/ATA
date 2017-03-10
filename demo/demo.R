set.seed(1337)
design <- ata_random(200, 5, 25) 

set.seed(1338)
str(itembank_example01)

nBooklet <- 10
nPos <- 4
nCluster <- 10

design_bib <- ata_sa_bib(nCluster, nPos, nBooklet, control = list("maxGen" = 25, "L" = 5e3)) # debug()

design_bib_mx <- apply(design_bib, 3, function(bb){
  1:nrow(bb) %*% bb
})
design_bib_mx

design_bib_allpos <- tensor(design_bib, rep(1, nPos), 2, 1)
block_paar <- crossprod(design_bib_allpos)

# ## blockdesign ...\Draft\Kapitel04_Testdesign\_Zweitfassung
# 
# ## -------------------------------------------------------------
# ## ?bung 2.3: ATA Methode f?r die Item-zu-Blockzuweisung
# ## -------------------------------------------------------------
# 
# set.seed(1338)
# 
# #
# nTh <- nrow(dat.bib)
# nPos <- ncol(dat.bib)
# nBl <- length(unique(unlist(dat.bib)))
# 
# nI <- nrow(dat.ib)
# 
# # Startdesign ----
# inc <- array(0, dim = c(nI, nBl))
# for(bb in 1:nBl){
#   inc[sample(1:nI, 12), bb] <- 1
# }
# 
# # IIF ----
# theta <- c(350, 475, 650)
# IIF <- matrix(0 , nrow=nI, ncol=length(theta))
# for(tt in 1:length(theta)){
#   P <- plogis(theta[tt] - dat.ib$itemdiff, location = 0, scale = 100)
#   IIF[,tt] <- P*(1-P)
# }
# 
# # Fitfunktion ----
# fit <- function(des, dat.ib, dat.bib){
#   # des <- inc
#   desTh <- des[, dat.bib[, 1]] + des[, dat.bib[, 2]] + 
#     des[, dat.bib[, 3]] + des[, dat.bib[, 4]]
#   
#   #
#   TIF <- (t(IIF) %*% desTh) / 48
#   
#   objective <- - sum(TIF) / prod(dim(TIF))
#   objWgt <- 2^0
#   
#   #
#   nb.key <- list(
#     (colSums(desTh > 1) > 0) / nTh
#   )
#   nb.key <- unlist(lapply(nb.key, sum))
#   nbWgt <- 2^6
#   
#   # 9
#   z.focus.block <- c(3, 3, 3, 3)
#   g.focus.block <- rowsum(des, dat.ib$focus) - z.focus.block
#   # 10
#   z.format.block <- c(4, 4, 1, 1)
#   g.format.block <- rowsum(des, dat.ib$format) - z.format.block
#   
#   nb.konstrukt <- list(
#     colSums(g.focus.block < 0) / prod(dim(g.focus.block)), 
#     colSums(g.format.block < 0) / prod(dim(g.format.block))
#   )
#   nb.konstrukt <- unlist(lapply(nb.konstrukt, sum))
#   nbWgt <- c(nbWgt, 2^c(4, 3))
#   
#   #
#   form.length <- (dat.ib$bearbeitungszeit %*% desTh) / 60
#   nb.stetig <- list(
#     (form.length > 75) / length(form.length),
#     diff(range(form.length)) / nTh
#   )
#   nb.stetig <- unlist(lapply(nb.stetig, sum))
#   nbWgt <- c(nbWgt, 2^c(4, 2))
#   
#   #
#   nb <- c(nb.key, nb.konstrukt, nb.stetig)
#   
#   nbWgt.norm <- nbWgt / (sum(nbWgt) + objWgt)
#   objWgt.norm <- objWgt / (sum(nbWgt) + objWgt)
#   oDes <- objWgt.norm %*% objective + nbWgt.norm %*% nb
#   
#   return(oDes)
# }
# 
# # Optimierungs-Algorithmus ----
# # t <- 1; t.min <- 1e-5; c <- 0.7; L <- 10000; l <- 1
# # t <- 1; t.min <- 1e-5; c <- 0.8; L <- 25000; l <- 1
# # t <- 1; t.min <- 1e-5; c <- 0.9; L <- 50000; l <- 1
# t <- 1; t.min <- 1e-7; c <- 0.9; L <- 100000; l <- 1
# 
# fitInc <- fit(inc, dat.ib, dat.bib)
# best <- list(inc, fitInc)
# vers <- versBest <- 1
# #
# start <- Sys.time()
# while(t > t.min){
#   while(l < L){
#     thisBl <- (l - 1) %% nBl + 1
#     
#     # Perturbation 
#     child <- inc
#     
#     items.raus <- which(child[, thisBl] == 1)
#     raus <- sample(items.raus, 1)
#     
#     bib.paar.bb <- dat.bib.paare[thisBl, ] != 0
#     items.bib.paare <- rowSums(child[, bib.paar.bb]) > 0
#     rein <- sample(which(!items.bib.paare), 1)
#     
#     child[c(raus, rein), thisBl] <- c(0, 1)
#     
#     # Fit und Survival
#     fitChild <- fit(child, dat.ib, dat.bib)
#     
#     behalte <- fitChild < fitInc
#     if(!behalte){
#       pt <- exp((fitInc - fitChild) / t)
#       behalte <- runif(1) <= pt
#     }
#     
#     if(behalte){
#       inc <- child
#       fitInc <- fitChild
#     }
#     
#     if(fitInc < best[[2]]){
#       best <- list(inc, fitInc)
#       versBest <- versBest + 1
#     }    
#     
#     # Kontroll-Ausgaben
#     if (identical(inc, child)) vers <- vers + 1
#     if (l %% 500 == 0) {
#       cat("\r")
#       cat(paste("l=", l), 
#           paste("t=", as.integer(log(t) / log(c) + 1)),
#           paste("versionen=", vers), 
#           paste("versionenBest=", versBest), 
#           paste("fit=", round(fitInc, 4)), 
#           paste("fitBest=", round(best[[2]], 4)), 
#           paste("pt=", round(pt, 5)),        
#           sep=";   ")
#       cat("                     ")
#       flush.console()
#     }
#     l <- l + 1
#   }
#   l <- 1
#   t <- t * c
# }
# end <- Sys.time()
# 
# 
# ## -------------------------------------------------------------
# ## Auswertung d. Testdesigns
# ## -------------------------------------------------------------
# des <- best[[1]]
# desTh <- des[, dat.bib[, 1]] + des[, dat.bib[, 2]] + 
#   des[, dat.bib[, 3]] + des[, dat.bib[, 4]]
# 
# #
# TIF <- (t(IIF) %*% desTh) / 48
# objective <- list(
#   sum(TIF) / prod(dim(TIF))
# )
# objective <- unlist(lapply(objective, sum))
# 
# thetafull <- 200:800
# IIFFull <- matrix(0 , nrow=nI, ncol=length(thetafull))
# for(tt in 1:length(thetafull)){
#   P <- plogis(thetafull[tt] - dat.ib$itemdiff, location = 0, scale = 100)
#   IIFFull[,tt] <- P*(1-P)
# }
# 
# TIFFull <- (t(IIFFull) %*% desTh) / (10*48)
# plot(x=thetafull, rowSums(TIFFull), type = "l", lwd = 2, ylim = c(0, 1))
# abline(v = theta)
# for(tt in 1:nTh){
#   lines(x = thetafull, y = colSums(IIFFull[desTh[,tt] == 1, ]) / 48, lty = 2, col = tt+1)  
# }
# lines(x = thetafull, rowSums(TIFFull), type = "l", lwd = 2)
# 
# #
# table(colSums(desTh > 1) > 0)
# 
# # 4
# rowsum(des, dat.ib$focus)
# # 5
# rowsum(des, dat.ib$format)
# 
# #
# formLength <- (dat.ib$bearbeitungszeit %*% desTh) / 60
# summary(c(formLength))
# 
# ###
# formP <- (dat.ib$p) %*% desTh / 48
# summary(c(formP))
# rowsum(desTh, dat.ib$p_cat)
# 
# best[[2]]
