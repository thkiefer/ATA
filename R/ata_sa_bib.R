#
ata_sa_bib <- function(tcnt, ccnt, pcnt, fitFnct = ata_sa_default_bibFitFnct, 
                       control = list(), # default: list("tMin" = 1e-7, "L" = 1e5, "maxGen" = 100, "decr" = NULL)
                       verbose = TRUE){
  
  # INITIALIZATION
  l <- 1
  t <- 1
  control <- control
  crtl.default <- list("tMin" = 1e-7, "L" = 1e5, "maxGen" = 100, "decr" = NULL) 
  control <- c(control, crtl.default[setdiff(names(crtl.default), names(control))])
  if(is.null(control$decr)) control$decr <- with(control, (tMin / t)^(1 / (maxGen))) 
  tMin <- control$tMin; L <- control$L; maxGen <- control$maxGen; decr <- control$decr
  
  marginGens <- round(log(maxGen, 10)) + 1
  marginL <- round(log(L, 10)) + 1

  # INCUMBENT
  inc  <- array(0, dim = c(ccnt, pcnt, tcnt))
  inc[cbind(1:ccnt, 1, 1:tcnt)] <- 1
  for(tt in 1:tcnt){
    inc[cbind(sample(1:ccnt, pcnt-1), 2:pcnt, tt)] <- 1
  }
  
  attr(inc, "fit") <- fitFnct(inc)
  tGen <- as.integer(log(t) / log(decr) + 1)
  attr(inc, "Generation") <- tGen
  attr(inc, "Individual") <- paste0("Gen", add.lead(tGen, marginGens), "_Indiv", add.lead(l, marginL))
  
  # ALGORITHM
  best <- inc
  begin <- Sys.time()
  while(t > tMin){
    while(l < L){
      
      # INITIALIZATION
      child <- inc
      
      # PERTURBATION
      this <- (l - 1) %% tcnt + 1
      clusterIn <- which(rowSums(inc[,, this]) > 0)
      clusterOut <- sample(clusterIn, 1)
      clusterInto <- sample(setdiff(1:ccnt, clusterIn), 1)
      
      child[clusterInto, , this] <- child[clusterOut, , this]
      child[clusterOut, , this] <- 0

      # Fit and Survival
      attr(child, "fit") <- fitFnct(child)
      
      keep <- attr(child, "fit") < attr(inc, "fit")
      if(!keep){
        pt <- exp((attr(inc, "fit") - attr(child, "fit")) / t)
        keep <- runif(1) <= pt
      }
      
      if(keep){
        inc <- child
        attr(inc, "Generation") <- tGen
        attr(inc, "Individual") <- paste0("Gen", add.lead(attr(inc, "Generation"), marginGens), "_Indiv", add.lead(l, marginL))
      }
      
      # CONTROL AND VERBOSE
      if(attr(inc, "fit") < attr(best, "fit")){
        best <- inc
      }    
      
      if(verbose & l %% 500 == 0) ata_sa_bib_verbose(inc, best, begin, tGen, l, tMin, L, maxGen)
      
      l <- l + 1
    }
    l <- 1
    t <- t * decr
    tGen <- as.integer(log(t) / log(decr) + 1)
  }
  ende <- Sys.time()
  
  # OUTPUT
  class(best) <- "ata_bib"
  return(best)  
}

#
ata_sa_default_bibFitFnct <- function(des){
  # INITIALIZATION
  ccnt <- dim(des)[[1]]
  pcnt <- dim(des)[[2]]
  tcnt <- dim(des)[[3]]
  tb <- aperm(des, c(2, 1, 3))
  
  # CALCULATIONS
  repPos <- tcnt * pcnt / ccnt
  pairCnt <- (repPos * (pcnt - 1)) / (ccnt - 1)
  
  desAllPos <- 1 * (tensor(des, rep(1, pcnt), 2, 1) > 0)
  indBP <- upper.tri(diag(nrow = ccnt))
  bp <- crossprod(desAllPos)[indBP]                                    # pairwise occurrence
  
  # CONSTRAINTS
  cnstr <- c(
    sum(abs(rowSums(des, dims = 2) - repPos) / (pcnt * tcnt)),         # positional balance
    sum(abs(colSums(des, dims = 2) - pcnt) / (pcnt * tcnt)),           # each booklet contains exactly 'pcnt' no. clusters
    sum(colSums((colSums(tb)) > 1) / (ccnt * tcnt))                    # each cluster exactly once in a booklet
  )
  cnstrWgt <- c(6, 4, 8)
  
  # OBJECTIVES
  zBP <- bp                                                            # departure from optimal pariwise occurrence
  zBP[bp >= 2] <- bp[bp >= 2] - ceiling(pairCnt)
  zBP[bp <= 1] <- floor(pairCnt) - bp[bp <= 1]                         
  
  objective <- c(
    sum(zBP) / (2 * length(zBP)), 
    diff(range(zBP[zBP > 0])) / repPos
  )
  objWgt <- c(-1, 0)
  
  # OUTPUT
  cnstrWgtNorm <- 2^cnstrWgt / (sum(2^cnstrWgt) + sum(2^objWgt))
  objWgtNorm <- 2^objWgt / (sum(2^cnstrWgt) + sum(2^objWgt))
  fit <- objWgtNorm %*% objective + cnstrWgtNorm %*% cnstr
  return(fit)
}

#
ata_sa_generate_bibFitFnct <- function(){
  # shiny-app
}

#
ata_sa_bib_verbose <- function(inc, best, begin, tGen, l, tMin, L, maxGen){
  cat("\r")
  cat(paste("l =", l), 
      paste("t =", add.lead(tGen, 3)),
      paste("max Gen =", maxGen),
      paste("runtime = ", paste.ata.difftime(Sys.time(), begin)),
      paste("fit(inc) =", sprintf("%6.4f", round(attr(inc, "fit"), 4))), 
      paste("fit(best) =", sprintf("%6.4f", round(attr(best, "fit"), 4))), 
      paste("Ind(best) =", attr(best, "Individual")), 
      sep=";   ")
  cat("                     ")
  flush.console()
}

#
as.ata_bib <- function(array){
  # class coercion method
}