\name{ata_sa}
\alias{ata_sa}
%- Also NEED an '\alias' for EACH other topic documented here.
\alias{ata_sa_bib}
\alias{ata_sa_izb}
\alias{ata_sa_gen_fitFnct}

\title{
Simulated annealing assignment of items according to specifications  
}

\description{
The function assigns items to tests according to specifications using simulated
annealing algorithm.  
}

\usage{
ata_sa_bib(tcnt, ccnt, pcnt, fitFnct = ata_sa_default_bibFitFnct, 
  control = list(), verbose = TRUE)

ata_sa_default_bibFitFnct(des)
ata_sa_generate_bibFitFnct()

as.ata_bib(array)

ata_sa_izb(bib, itembank, fitFnct)
ata_sa_default_izbFitFnct(des)
ata_sa_generate_izbFitFnct()
}  

\arguments{

  \item{ccnt}{Number of clusters in test design.  
  }
  \item{tcnt}{Number of tests to be administered.  
  }
  \item{pcnt}{Number of positions in a booklet.  
  }
  \item{bib}{(p)BIB design object obtained from \code{ata_sa_bib} or \code{as.ata_bib}.    
  }
  \item{itembank}{matrix including the item information.
  }
  \item{fitFnct}{Fit function for design.
  }
  \item{des}{Current design to be evaluatet by fitFnct.    
  }
  \item{array}{Three dimensional array block by position by booklet beeing coerced to a bib-design object.  
  }
  \item{control}{control parameters.  
  }
  \item{verbose}{Print output?  
  }

}

%\details{
%%  ~~ If necessary, more details than the description above ~~
%}

\value{
multiple -- to be parted into several RD-Files
}

%\references{
%% ~put references to the literature/web site here ~
%}

%\author{
%% Alexander Robitzsch
%}

%\note{
%%  ~~further notes~~
%}

%% ~Make other sections like Warning with \section{Warning }{....} ~

%\seealso{
%% ~~objects to See Also as \code{\link{help}}, ~~~
%}

\examples{
#############################################################################
# EXAMPLE 1: 
#############################################################################
	
design <- ata_sa_bib(200, 5, 25)  
  
\dontrun{
}
}


% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
%\keyword{anova}
%\keyword{ ~kwd2 }% __ONLY ONE__ keyword per line
