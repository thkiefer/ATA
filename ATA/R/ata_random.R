#' Greedy random assignment of items according to specifications
#' 
#' The function randomly assigns items to tests according to specifications.  
#' 
#' @param icnt Number of items in itembank.
#' @param tcnt Number of tests to be administered.
#' @param len Length of tests.
#' @details details
#' @return A binary matrix containing the assignment of the items to the tests.
#' @export
#' @examples
#' set.seed(1337)
#' design <- ata_random(200, 5, 25) 
#' 
ata_random <- function(icnt, tcnt, len){
  out <- .Call("ata_random", icnt, tcnt, len, PACKAGE="ATA")
}