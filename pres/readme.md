# This folder is meant to host the presentation deck source-code 

Here you will find only the markdown source of the presentation deck. 
[pandoc](https://pandoc.org/) is used to convert the source code to a pdf file
using [beamer](https://en.wikipedia.org/wiki/Beamer_(LaTeX)) format and an addon 
to pandoc called [pandoc-plantuml](https://www.npmjs.com/package/pandoc-plantuml).

To generate the presentation PDF run: 

1. Build the image (which will compile the markdown):
    ```bash
    docker build -t pandoc-latex-plantuml .
    ```
2. Run the compilation inside the built container (this will generate a file `pres.pdf` under this folder): 
   ```bash
   docker run --rm -v "$(pwd)":/data --user `id -u`:`id -g` pandoc-latex-plantuml  /data/pres.md -t beamer -F pandoc-plantuml -M date="$(date "+%d %B %Y")"  -V classoption:aspectratio=169 -H preamble.tex -o /data/pres.pdf
   ```