# This folder is meant to be a presentation deck

Here you will find only the markdown source of the presentation deck. 
[pandoc](https://pandoc.org/) is used to convert the source code to a pdf file.

To convert it to PDF use the following command:

```bash
docker run --rm -v "$(pwd)":/data pandoc/latex /data/pres.md -t beamer -o /data/pres.pdf
```