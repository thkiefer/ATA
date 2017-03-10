\name{itembank_example01}
\alias{itembank_example01}
\docType{data}

\title{
Example data
}

\description{
list example data here
}

\usage{
data(itembank_example01)
}

\format{
\itemize{
\item \strong{itembank_example01}: Example itembank comprising 7 variables for 
250 items.

\ifelse{latex}{%start latex
\itemize{
\item \code{item}: item name. 
\item \code{format}: response format.
\item \code{construct}: location in construct.
\item \code{p}: estimated relative frequency of a correct response.
\item \code{p_group}: categorized \code{p} into 'easy', 'medium', 'hard'.
\item \code{itemdiff}: item difficulty scaled to N(100, 20)-metric.
\item \code{proc_time}: expected processing time needed to answer this item 
correctly.
}}{
\tabular{ll}{
\code{item} \tab item name. \cr
\code{format} \tab response format. \cr
\code{construct} \tab location in construct.\cr
\code{p} \tab estimated relative frequency of a correct response.\cr
\code{p_group} \tab categorized \code{p} into 'easy', 'medium', 'hard'. \cr
\code{itemdiff} \tab item difficulty scaled to N(100, 20)-metric. \cr
\code{proc_time} \tab expected processing time needed to answer the item 
correctly.
}%end tabular
}%end else / end ifelse latex

\ifelse{latex}{%start latex
\code{'data.frame':	250 obs. of  7 variables:}\cr
\code{ $ item     : chr  "It0003" "It0004" "It0006" "It0007" ...}\cr
\code{ $ format   : Factor w/ 4 levels "discrete_open_ended",..: 1 2 2 1 2 2 2 1 3 2 ...}\cr
\code{ $ construct: int  1 4 4 2 3 4 1 2 3 3 ...}\cr
\code{ $ p        : num  0.84 0.56 0.34 0.45 0.2 0.42 0.77 0.42 0.34 0.71 ...}\cr
\code{ $ p_group  : Factor w/ 3 levels "hard","medium",..: 3 3 3 3 2 3 3 3 3 3 ...}\cr
\code{ $ itemdiff : num  80.8 114 135.2 124.4 152.2 ...}\cr
\code{ $ proc_time: int  90 60 90 120 90 150 90 30 120 90 ...}\cr
}{\verb{'data.frame':	250 obs. of  7 variables:
 $ item     : chr  "It0003" "It0004" "It0006" "It0007" ...
 $ format   : Factor w/ 4 levels "discrete_open_ended",..: 1 2 2 1 2 2 2 1 3 2 ...
 $ construct: int  1 4 4 2 3 4 1 2 3 3 ...
 $ p        : num  0.84 0.56 0.34 0.45 0.2 0.42 0.77 0.42 0.34 0.71 ...
 $ p_group  : Factor w/ 3 levels "hard","medium",..: 3 3 3 3 2 3 3 3 3 3 ...
 $ itemdiff : num  80.8 114 135.2 124.4 152.2 ...
 $ proc_time: int  90 60 90 120 90 150 90 30 120 90 ...
}%end verb
}%end else / end ifelse latex
} % end itemize
} % end format

\keyword{datasets}
