################################################################################################################
#
# helper functions
# 
################################################################################################################

#-------------------------------------------------------------------------------------------------------------##
# Workspace Diagnostics                                                                                       ##
{
  # require(multilevelPSA)
  ws.diagn <- function(..., n = 10){
    
    most.objects <- sum(lsos(n = max(n, min(100, length(ls()))))[,"Size"])
    class(most.objects) <- "object_size"
    val <- capture.output(print(most.objects, units = "auto"))
    out <- rbind("available" = data.frame("Type" = "_SUMMARY", "Size" = memory.size(TRUE) * 1048576, 
                                          "PrettySize" = paste0(memory.size(TRUE), " Mb"), "Rows" = NA, "Columns" = NA), 
                 "used_all" = data.frame("Type" = "_SUMMARY", "Size" = sum(lsos(n = max(n, min(100, length(ls()))))[,"Size"]), 
                                         "PrettySize" = val, "Rows" = NA, "Columns" = NA),
                 lsos(n=n))
    return(out)
  }
}

#-------------------------------------------------------------------------------------------------------------##
# String Manipulation                                                                                         ##
{
  
  # Get Substring of length n from right side
  substrRight <- function(x, n){substr(x, nchar(x)-n+1, nchar(x))}
  
  # Get Substring of length n from left side
  substrLeft <- function(x, n){substr(x, 1, n)}
  
  # Replace dots with a comma in data frame xx
  replace.dot <- function(xx){
    if(length(xx)>1){
      return(unlist(sapply(xx, function(yy) gsub("([[:digit:]]{1,2}).([[:digit:]]{1,2})", "\\1,\\2", yy))))
    }else{ return(gsub("([[:digit:]]{1,2}).([[:digit:]]{1,2})", "\\1,\\2", xx))}
    
  }
  
  
  # Replace head and/or tail blanks
  replace.blanks <- function(xx, position=c("both", "head", "tail"), replacement=""){
    
    position <- match.arg(position)
    if( position %in% c("both", "head")){
      xx <- sub(pattern="^\\s+", replacement=replacement, xx)
    }
    if( position %in% c("both", "tail")){
      xx <- gsub(pattern="\\s+$", replacement=replacement, xx)
    }
    
    return(xx)
  }
  
  # generate BIFIE Time String
  bifieTimeString <- function(withClock = FALSE){
    format( Sys.time(), if(withClock) "%Y%m%d_%H%M" else "%Y%m%d" )
  }
  
  # add leading zeros
  add.lead <- function(x, width=max(nchar(x))){
    sprintf(paste('%0', width, 'i', sep=''), x) 
  }
  
  paste.ata.difftime <- function(t2, t1){
    dt <- as.numeric(round(difftime(t2, t1, units = "sec"), 1))
    paste0(add.lead(dt %/% 3600, 2), ":", add.lead((dt %% 3600) %/% 60, 2), ":",
           add.lead(floor(dt %% 60), 2), ".", 
           gsub("0.", "", sprintf("%2.1f", round(dt%%60 - floor(dt %% 60), 1)), fixed = TRUE))
  }
  
}


#-------------------------------------------------------------------------------------------------------------##
# Datastructure Manipulation                                                                                  ##
{
  
  # replace colnames colnamesOld with colnamesNew
  replace.colnames <- function(xx, colnamesOld, colnamesNew){
    mm <- match(colnames(xx) , colnamesOld)
    colnames(xx)[ !is.na(mm) ] <- colnamesNew[ mm[!is.na(mm)] ]
    return(xx)
  }
  
  # load specific structure from R workspace
  load.RData2 <- function(filename, objnameOut, objnameIn){
    load(filename)
    eval(parse(text = paste(objnameOut, "<<- ", objnameIn)))
  }
  
}