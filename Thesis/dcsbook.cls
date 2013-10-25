%
%  $Id: dcsbook.cls 3196 2013-01-15 09:26:50Z sobaniec $
%

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{dcsbook}[2012/03/01 v2.2 DCS Group book class]
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{book}}
\ProcessOptions\relax
\LoadClass{book}


\RequirePackage[a4paper]{geometry}
\RequirePackage[pagestyles,clearempty,psfloats]{titlesec}
\RequirePackage{titletoc}
\RequirePackage{dcslib}[2012/03/01]
\RequirePackage[nottoc,notlot,notlof]{tocbibind}
\RequirePackage{calc}
\RequirePackage{translator}
\RequirePackage{flafter}
\RequirePackage{enumitem}


% default monospace font
\ifunicodeengine
  % can be obtained from http://www.fontsquirrel.com/fonts/DejaVu-Sans-Mono
  \setallmonofonts[Scale=MatchLowercase]{DejaVu Sans Mono}
\else
  \RequirePackage{lmodern}
  \RequirePackage[scaled=0.82]{beramono}
\fi


% default sectioning font
\newcommand{\secfont}{\sffamily}
\newcommand{\headingfont}{\small\itshape}


\usedictionary{translator-thesis-dictionary}

% default page layout
\geometry{twoside=true,
          tmargin=3cm,bmargin=3cm,lmargin=2.5cm,rmargin=2.5cm,
          headsep=30pt,footskip=0cm,
          bindingoffset=1cm,
          reversemarginpar,marginparwidth=2.5cm,marginparsep=0.5cm}

% oneside
\DeclareOption{oneside}{
  \geometry{twoside=false,lmargin=3cm,rmargin=3cm,bindingoffset=0cm}
}


% front/back matters
\renewcommand\frontmatter{%
  \cleardoublepage
  \@mainmatterfalse
  \pagenumbering{arabic}}
\renewcommand\mainmatter{%
  \cleardoublepage
  \@mainmattertrue}


% page styles
\newpagestyle{main}{
  \sethead[\sffamily\usepage][]%
  [\footnotesize\sffamily
    \if@mainmatter
      \ifthenelse{\value{chapter}>0}{\thechapter~~}{}
    \fi
    \chaptertitle]%
  {\footnotesize\sffamily
    \if@mainmatter
      \ifthesection{\thesection~~\sectiontitle}%
                   {\ifthenelse{\value{chapter}>0}{\thechapter~~}{}\chaptertitle}
    \else
      \chaptertitle
    \fi
  }{}{\sffamily\usepage}
  \setfoot{}{}{}
  \setfloathead{}{}{}{}[p]
}
\renewpagestyle{plain}{
  \setfoot{}{}{}
}
\pagestyle{main}


% url
\def\UrlFont{\sffamily}

% Metadata:
% supervisor
\newcommand{\@supervisorname}
  {\textcolor{red}{\texttt{\small Unknown. Use \textbackslash{}supervisor command.}}}
\newcommand{\supervisor}[1]{\renewcommand{\@supervisorname}{#1}}
% subtitle
\newcommand{\@subtitle}{\translate{Master's Thesis}}
\newcommand{\subtitle}[1]{\renewcommand{\@subtitle}{#1}}
% university
\newcommand{\@university}{\translate{Poznań University of Technology}}
\newcommand{\university}[1]{\renewcommand{\@university}{#1}}
% institute
\newcommand{\@institute}{\translate{Institute of Computing Science}}
\newcommand{\institute}[1]{\renewcommand{\@institute}{#1}}

\renewcommand{\and}{\\}
\newenvironment{abstract}{
  \chapter*{\abstractname}
  %\addcontentsline{toc}{section}{\abstractname}
}{}

% titlesec — default settings
\titleformat{\paragraph}{\bfseries\filright}{}{0em}{}
\titlespacing*{\paragraph}{0pt}{1.2em}{0.5em}
\titleformat{\subparagraph}[runin]{\bfseries}{}{0em}{}
\titlespacing{\subparagraph}{0pt}{\bigskipamount}{1em}


% TOC formatting
\newlength{\@chaptertocsep}
\newlength{\@sectiontocsep}
\newlength{\@subsectiontocsep}
\DeclareOption{dcstoc}{
  % titletoc
  \setlength{\@chaptertocsep}{1.9em}
  \setlength{\@sectiontocsep}{2.5em}
  \setlength{\@subsectiontocsep}{3.2em}
  \titlecontents{part}[2em]{\vspace{2.2ex}}%
    {\flushleft\secfont\bfseries\Large\contentslabel{1.5em}}{\hspace*{-1.5em}}%
    {~\hfill\Large\bfseries\contentspage}[\vspace{0.5ex}]
  \titlecontents{chapter}[\@chaptertocsep]{\vspace{1.5ex}}%
    {\bfseries\contentslabel{\@chaptertocsep}}{\bfseries\hspace*{-\@chaptertocsep}}%
    {~\hfill\bfseries\contentspage}[\vspace{0.3ex}]
  \titlecontents{section}[\@chaptertocsep+\@sectiontocsep]{}%
    {\contentslabel{\@sectiontocsep}}{\hspace*{-\@sectiontocsep}}%
    {\titlerule*[1pc]{.}\contentspage}
  \titlecontents{subsection}[\@chaptertocsep+\@sectiontocsep+\@subsectiontocsep]{}%
    {\contentslabel{\@subsectiontocsep}}{\hspace*{-\@subsectiontocsep}}%
    {\titlerule*[1pc]{.}\contentspage}
}

\newcommand{\@institution}{Politechnika Pozna\'nska\\Instytut Informatyki}
\newcommand{\institution}[1]{\renewcommand{\@institution}{#1}}

\DeclareOption{textbook}{
  \renewcommand{\maketitle}{%
    \begin{titlepage}
    \sffamily
    \setlength{\parindent}{0pt}
    \parbox[b]{0.9\textwidth}{\secfont\bfseries\huge\raggedright\textcolor{dkgray}{%
      \begin{spacing}{0.85}\@title\end{spacing}}}
    \vspace{3pt}\par
    \colorbox{gray}{\parbox{\textwidth}{
      \raggedleft
      \textcolor{white}{\bfseries\raisebox{0ex}[1.5ex][0.2ex]{\@date~}}}}

    \vspace{0.10\textheight}
    \hfill\parbox{0.95\textwidth}{\raggedleft\large\@author}

    \vfill
    \begin{center}
    \normalsize
    \rule{0.5\textwidth}{0.5pt}\\
    \@institution
    \end{center}
    \end{titlepage}
    \cleardoublepage
  }

  % titlesec
  \titleformat{\part}{\secfont\Large\bfseries}{\thepart}{1em}{}
  \titleformat{\chapter}[block]{\filleft\secfont\fontsize{32}{32}\selectfont}{}{0em}%
      {\parbox{0.9\textwidth}%
      {\raggedleft\sffamily\fontsize{65}{65}\selectfont%
       \textcolor{gray}{\thechapter}}\vspace{0.5ex}\\}
  \titleformat{name=\chapter,numberless}[block]{\secfont\fontsize{32}{32}\selectfont\filleft}{}{0em}{}
  \titlespacing*{\chapter}{0pt}{0cm}{4.5cm}
  \titleformat{\section}{\secfont\LARGE\bfseries\color{seccolor}\filright}{\thesection}{1em}{}
  \titleformat{\subsection}{\secfont\Large\bfseries\color{seccolor}\filright}{\thesubsection}{1em}{}
  \titleformat{\subsubsection}{\secfont\large\bfseries\color{seccolor}\filright}{\thesubsubsection}{1em}{}
  \titleformat{\paragraph}{\secfont\bfseries\filright}{}{0em}{}

  \ExecuteOptions{dcstoc}
}

\DeclareOption{half}{
  \geometry{twoside=false,a5paper,
            tmargin=1cm,bmargin=1cm,lmargin=1cm,rmargin=1cm,
            headsep=30pt,footskip=0cm,
            bindingoffset=0cm,
            reversemarginpar,marginparwidth=2.5cm,marginparsep=0.5cm}
}

% Master's or Bachelor's Thesis
\DeclareOption{thesis}{
  % line skip
  \setstretch{1.1}

  \renewcommand{\maketitle}{%
    \begin{titlepage}
    \begin{center}
    \includegraphics[scale=0.2]{logopp/logopp-sygnet-en}
    \end{center}

    \vfill
    \begin{center}
    {\large\secfont\bfseries\@author}\par
    \vspace{2ex}
    {\huge\secfont\begin{spacing}{0.95}\@title\end{spacing}}\par
    \vspace{4ex}
    {\large\@subtitle}
    \end{center}
    \vfill\vfill

    \begin{center}
    {\large \translate{Supervisor}:~ \@supervisorname}\par
    {\bigskip\large \@date}
    \end{center}

    \end{titlepage}
    \cleardoublepage
  }

  % titlesec
  \titleformat{\part}{\secfont\Large\bfseries}{\thepart}{1em}{}
  \titleformat{\chapter}[display]{\normalfont\filcenter\secfont}
    {\Large\hrulefill~~\chaptertitlename~\thechapter~~\hrulefill}{0pc}{\fontsize{28}{28}\selectfont}
  \titleformat{name=\chapter,numberless}[display]{\normalfont\filcenter\secfont}
    {}{0.5pc}{\fontsize{28}{28}\selectfont}
  \titlespacing*{\chapter}{0pt}{0cm}{4cm}
  \titleformat{\section}{\secfont\LARGE\bfseries\filright}{\thesection}{1em}{}
  \titleformat{\subsection}{\secfont\Large\bfseries\filright}{\thesubsection}{1em}{}
  \titleformat{\subsubsection}{\secfont\large\bfseries\filright}{\thesubsubsection}{1em}{}
  \titleformat{\paragraph}{\secfont\bfseries\filright}{}{0em}{}

  \renewenvironment{abstract}
    {
     \cleardoublepage
     \thispagestyle{plain}
     \mbox{}\vfill
     \begin{list}{}{
       \setlength{\leftmargin}{0.1\textwidth}
       \setlength{\rightmargin}{0.1\textwidth}
       \begin{center}
         \secfont\Large\abstractname
       \end{center}
       \smallskip
       \small
       \setstretch{1}
     }
     \item\relax
    }
    {\end{list}\vfill\vfill\vfill\mbox{}}

    \ExecuteOptions{dcstoc}
}

\DeclareOption{msc}{
  \@latex@error{Use "thesis" option instead of "msc"}
}

% Technical reports
\DeclareOption{report}{
  \renewcommand{\and}{\par}
  \renewcommand{\maketitle}{%
    \begin{titlepage}
    \secfont
    \setlength{\parindent}{0pt}
    \null\vfill
    \hfill\begin{minipage}{12.5cm}
      \begin{center}
        {\fontsize{18}{20}\selectfont\@title\par}
        \fontsize{12}{14}\selectfont
        \vspace{5mm}
        {\@author}\par
        \vspace{5mm}
        {\fontsize{10}{12}\selectfont \techreportname\ \@date}
      \end{center}
    \end{minipage}\hspace{0.5cm}\hfill\null
    \vfill\null
    \end{titlepage}
    \cleardoublepage
  }

  \renewcommand\tableofcontents{%
    \chapter*{\contentsname}
    \pagestyle{empty}
    \@starttoc{toc}
    \cleardoublepage
    \pagestyle{main}}

  % page layout
  \geometry{tmargin=3.5cm,bmargin=3cm,lmargin=2.75cm,rmargin=2.75cm,
            headsep=7ex,footskip=0cm,bindingoffset=0.5cm}

  % page styles
  \renewpagestyle{main}[\footnotesize\sffamily]{
    \headrule
    \sethead[\normalsize\usepage][]%
    [\if@mainmatter
       \ifthenelse{\value{chapter}>0}{\thechapter~~}{}
     \fi
     \chaptertitle]%
    {\if@mainmatter
       \ifthesection{\thesection~~\sectiontitle}%
                    {\ifthenelse{\value{chapter}>0}{\thechapter~~}{}\chaptertitle}
     \else
       \chaptertitle
     \fi
    }{}{\normalsize\usepage}
    \setfoot{}{}{}
    \setfloathead{}{}{}{\setheadrule{0pt}}[p]
  }
  \pagestyle{main}

  % line space
  \setstretch{1.1}

  % titlesec
  \titleformat{\chapter}[block]{\filright\secfont\fontsize{28}{28}\selectfont}{}{0em}%
      {\textcolor{midgray}{\LARGE\@chapapp~\thechapter}\\}
  \titleformat{name=\chapter,numberless}[block]{\secfont\Huge\filright}{}{0em}{}
  \titlespacing*{\chapter}{0pt}{0cm}{4cm}

  \titleformat{\section}{\secfont\LARGE\filright}{\thesection}{1em}{}
  \titleformat{\subsection}{\secfont\Large\filright}{\thesubsection}{1em}{}
  \titleformat{\subsubsection}{\secfont\large\filright}{\thesubsubsection}{1em}{}
  \titleformat{\paragraph}{\secfont\bfseries\filright}{}{0em}{}
  \titleformat{\subparagraph}[runin]{\secfont\bfseries}{}{0em}{}

  \ExecuteOptions{dcstoc}

  \setlength{\parindent}{1pc}

  % parskip
  \setlength{\parskip}{0.6ex}

  % abstract environment
  \renewenvironment{abstract}
    {
     \thispagestyle{plain}
     \mbox{}\vfill
     \begin{list}{}{
       \setlength{\leftmargin}{0.1\textwidth}
       \setlength{\rightmargin}{0.1\textwidth}
       \begin{center}
         \secfont\Large\abstractname
       \end{center}
       }
       \item\relax
    }
    {\end{list}\vfill\vfill\vfill\mbox{}
     \cleardoublepage
    }
}

% default colors
\lstset{language=c}
\lstset{keywordstyle=\bfseries\color{dkgreen},
        commentstyle=\itshape\color{dkblue},
        stringstyle=\color{dkred},
        directivestyle=\color{directive}}
\lstset{language=}


% black & white version
\DeclareOption{bw}{
  \lstset{language=c}
  \lstset{keywordstyle=\bfseries,commentstyle=\itshape,
          stringstyle=,directivestyle=}
  \lstset{language=}
  \definecolor{directive}{rgb}{0, 0, 0}
  \definecolor{linkcolor}{rgb}{0, 0, 0}
  \definecolor{pathcolor}{rgb}{0, 0, 0}
  \definecolor{namecolor}{rgb}{0, 0, 0}
  \definecolor{cmdcolor}{rgb}{0, 0, 0}
  \definecolor{mancolor}{rgb}{0, 0, 0}
  \definecolor{codecolor}{rgb}{0, 0, 0}
  \definecolor{seccolor}{rgb}{0, 0, 0}
}


% lists
\setlist{itemsep=0.5\itemsep,parsep=0.5\parsep,topsep=0.7\topsep}
\setlist[enumerate,2]{label=\alph*), ref=\theenumi\alph*}
\setlist[itemize,3]{label=$\cdot$}
\renewenvironment{quote}
  {\begin{itemize}[label=]\item\relax}
  {\end{itemize}}
\renewenvironment{center}
  {\begin{itemize}[label=,leftmargin=0pt]\item\relax\centering}
  {\end{itemize}}


% labeling environment
\newenvironment{labeling}[1][]%
  {\begin{description}[font=\normalfont,style=nextline,#1]}
  {\end{description}}


\ProcessOptions\relax
