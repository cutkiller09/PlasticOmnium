%% Exemple de CV en LaTeX. 
%% �milia Robin (Emilia.Robin@ens.fr)
%% janvier 2000

\documentclass[a4paper,16pt]{article} % fonte 11 points, papier a4

%%\usepackage[francais]{babel}    % faire du fran�ais
\usepackage[latin1]{inputenc}   % accents dans le source
\usepackage[T1]{fontenc}        % accents dans le DVI
\usepackage{url}                % citer des adresses �lectroniques et des URL
\usepackage[table]{xcolor}

% La page
%#########
\pagestyle{plain}                % On num�rote les pages
%\usepackage{vmargin}            % red�finir les marges
%\setmarginsrb{2.5cm}{3cm}{2.5cm}{3cm}{0cm}{0cm}{0cm}{0cm}
\usepackage{layout}
\usepackage[top=2cm, bottom=2cm, left=2cm, right=2cm]{geometry}

% Marge gauche, haute, droite, basse; espace entre la marge et le texte �
% gauche, en  haut, � droite, en bas

% Je red�finis le comportement des guillemets
%#############################################
\catcode`\�=\active             
\catcode`\�=\active
\def�{\og\ignorespaces}
\def�{{\fg}}

% Diverses nouvelles commandes
%#############################

% Pour laisser de l'espace entre les lignes du tableau
\newcommand\espace{\vrule height 20pt width 0pt}

% Pour mes grands titres
\newcommand{\titre}[1]{%
	\begin{center}
	\rule{\textwidth}{1pt}
	\par\vspace{0.3cm}
        \textbf{\large #1}
	\par\rule{\textwidth}{1pt}
	\end{center}
	\bigskip
	}

% D�but du document
%###################
\begin{document}
%###################

\titre{\Large M\'enage des parties communes }
%#############
\vspace{-0.5cm}

\renewcommand{\arraystretch}{1.5}

\begin{center}
\begin{tabular}{||p{12em}|p{15em}|p{18em}|}
\hline
\bf Date & \bf Signature & \bf Commentaire \\
\hline
lundi 20 juin 2016 &  &  \\
\cline {1-3}
lundi 27 juin 2016 &  &  \\
\cline {1-3}
lundi 4 juillet 2016 &  &  \\
\cline {1-3}
lundi 11 juillet 2016 &  &  \\
\cline {1-3}
lundi 18 juillet 2016 &  &  \\
\cline {1-3}
lundi 25 juillet 2016 &  &  \\
\cline {1-3}
lundi 1 ao�t 2016 &  &  \\
\cline {1-3}
lundi 8 ao�t 2016 &  &  \\
\cline {1-3}
lundi 15 ao�t 2016 &  &  \\
\cline {1-3}
lundi 22 ao�t 2016 &  &  \\
\cline {1-3}
lundi 29 ao�t 2016 &  &  \\
\cline {1-3}
lundi 5 septembre 2016 &  &  \\
\cline {1-3}
lundi 12 septembre 2016 &  &  \\
\cline {1-3}
lundi 19 septembre 2016 &  &  \\
\cline {1-3}
lundi 26 septembre 2016 &  &  \\
\cline {1-3}
lundi 3 octobre 2016 &  &  \\
\cline {1-3}
lundi 10 octobre 2016 &  &  \\
\cline {1-3}
lundi 17 octobre 2016 &  &  \\
\cline {1-3}
lundi 24 octobre 2016 &  &  \\
\cline {1-3}
lundi 31 octobre 2016 &  &  \\
\cline {1-3}
lundi 7 novembre 2016 &  &  \\
\cline {1-3}
lundi 14 novembre 2016 &  &  \\
\cline {1-3}
lundi 21 novembre 2016 &  &  \\
\cline {1-3}
lundi 28 novembre 2016 &  &  \\
\cline {1-3}
lundi 5 d�cembre 2016 &  &  \\
\cline {1-3}
lundi 12 d�cembre 2016 &  &  \\
\cline {1-3}
lundi 19 d�cembre 2016 &  &  \\
\cline {1-3}
lundi 26 d�cembre 2016 &  &  \\
\cline {1-3}
lundi 2 janvier 2017 &  &  \\
\cline {1-3}
lundi 9 janvier 2017 &  &  \\
\cline {1-3}
lundi 16 janvier 2017 &  &  \\
\cline {1-3}
lundi 23 janvier 2017 &  &  \\
\cline {1-3}
lundi 30 janvier 2017 &  &  \\
\cline {1-3}
lundi 6 f�vrier 2017 &  &  \\
\cline {1-3}
lundi 13 f�vrier 2017 &  &  \\
\cline {1-3}
lundi 20 f�vrier 2017 &  &  \\
\cline {1-3}
\hline


\end{tabular}
\end{center} 


\end{document}
