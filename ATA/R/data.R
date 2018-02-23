#' Example data 01
#' 
#' list example data here  
#' 
#' @format
#' {\strong{itembank_example01}: Example itembank comprising 7 variables for 
#'     250 items.
#'     \describe{\item{}{\ifelse{latex}{ % start latex
#'             \itemize{
#'                 \item \code{item}: item name. 
#'                 \item \code{format}: response format.
#'                 \item \code{dim_1}: location in first construct dimension.
#'                 \item \code{dim_2}: location in second construct dimension.
#'                 \item \code{p}: estimated relative frequency of a correct response.
#'                 \item \code{p_group}: categorized \code{p} into 'easy', 'medium', 'hard'.
#'                 \item \code{itemdiff}: item difficulty scaled to N(100, 20)-metric.
#'                 \item \code{proc_time}: expected processing time needed to answer this item correctly.
#'              }}{ % end itemize, end latex (if), start html
#'              \tabular{ll}{
#'                 \code{item} \tab item name. \cr
#'                 \code{format} \tab response format. \cr
#'                 \code{dim_1} \tab location in first construct dimension.\cr
#'                 \code{dim_2} \tab location in second construct dimension.\cr
#'                 \code{p} \tab estimated relative frequency of a correct response.\cr
#'                 \code{p_group} \tab categorized \code{p} into 'easy', 'medium', 'hard'. \cr
#'                 \code{itemdiff} \tab item difficulty scaled to N(100, 20)-metric. \cr
#'                 \code{proc_time} \tab expected processing time needed to answer the item correctly.
#'              }}} % end tabular, end html (else), end item
#'             \item{}{\ifelse{latex}{ % start latex
#'                 \code{'data.frame':	250 obs. of  7 variables:}\cr
#'                 \code{ $ item     : chr  "It0003" "It0004" "It0006" "It0007" ...}\cr
#'                 \code{ $ format   : Factor w/ 4 levels "discrete_open_ended",..: 1 2 2 1 2 2 2 1 3 2 ...}\cr
#'                 \code{ $ dim_1    : int  1 4 4 2 3 4 1 2 3 3 ...}\cr
#'                 \code{ $ dim_2    : int  3 4 3 1 1 4 1 4 3 1 ...}\cr
#'                 \code{ $ p        : num  0.84 0.56 0.34 0.45 0.2 0.42 0.77 0.42 0.34 0.71 ...}\cr
#'                 \code{ $ p_group  : Factor w/ 3 levels "hard","medium",..: 3 3 3 3 2 3 3 3 3 3 ...}\cr
#'                 \code{ $ itemdiff : num  80.8 114 135.2 124.4 152.2 ...}\cr
#'                 \code{ $ proc_time: int  90 60 90 120 90 150 90 30 120 90 ...}\cr
#'                 }{ % end latex (if), start html
#'                 \verb{'data.frame':	250 obs. of  7 variables:
#'  $ item     : chr  "It0003" "It0004" "It0006" "It0007" ...
#'  $ format   : Factor w/ 4 levels "discrete_open_ended",..: 1 2 2 1 2 2 2 1 3 2 ...
#'  $ dim_1    : int  1 4 4 2 3 4 1 2 3 3 ...
#'  $ dim_2    : int  3 4 3 1 1 4 1 4 3 1 ...
#'  $ p        : num  0.84 0.56 0.34 0.45 0.2 0.42 0.77 0.42 0.34 0.71 ...
#'  $ p_group  : Factor w/ 3 levels "hard","medium",..: 3 3 3 3 2 3 3 3 3 3 ...
#'  $ itemdiff : num  80.8 114 135.2 124.4 152.2 ...
#'  $ proc_time: int  90 60 90 120 90 150 90 30 120 90 ...
#'             }}} % end verb, end html (else), end item
#'      } % end describe     
#' }   
#' 
#' @details details
#' 
#' @examples
#' ## first block design
#' set.seed(1338)
#' data(itembank_example01)
#' str(itembank_example01)
#'
#' nBooklet <- 10
#' nPos <- 4
#' nCluster <- 10
#'
#' design_bib <- ata_sa_bib(nCluster, nPos, nBooklet, control = list("maxGen" = 25, "L" = 5e3)) # debug()
#'
#' design_bib_mx <- apply(design_bib, 3, function(bb){
#'   1:nrow(bb) %*% bb
#' })
#' design_bib_mx
#' design_bib_allpos <- tensor(design_bib, rep(1, nPos), 2, 1)
#' block_paar <- crossprod(design_bib_allpos)
#' 
#' ## first testdesign
#' set.seed(1339)
#' data(itembank_example01)
#' data(bib_example01)
#' 
#' @name example01
NULL

#' @rdname example01
"itembank_example01"

#' @rdname example01
"bib_example01"
