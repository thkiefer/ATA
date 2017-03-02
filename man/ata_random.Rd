\name{ata_random}
\alias{ata_random}
%- Also NEED an '\alias' for EACH other topic documented here.

\title{
Greedy random assignment of items according to specifications
}


\description{
The function randomly assigns items to tests according to specifications.  
}

\usage{
ata_random(icnt, tcnt, len)  
}  

\arguments{
  \item{icnt}{Number of items in itembank.  
  }
  \item{tcnt}{Number of tests to be administered.  
  }
  \item{len}{Length of tests.  
  }
}

%\details{
%%  ~~ If necessary, more details than the description above ~~
%}

\value{
A binary matrix containing the assignment of the items to the tests.
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
	
design <- ata_random(200, 5, 25)  
  
\dontrun{
}
}


% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
%\keyword{anova}
%\keyword{ ~kwd2 }% __ONLY ONE__ keyword per line
