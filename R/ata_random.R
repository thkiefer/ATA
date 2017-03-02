ata_random <- function(icnt, tcnt, len){
  out <- .Call("ata_random", icnt, tcnt, len, PACKAGE="ATA")
}