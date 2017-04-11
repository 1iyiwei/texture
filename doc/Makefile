DRAFT = draft
FINAL = final

BIB_SOURCES = paper.bib

PAPER_SOURCES := $(wildcard *.tex)
DRAFT_SOURCES = draft.tex $(PAPER_SOURCES)
FINAL_SOURCES = final.tex $(PAPER_SOURCES)

HAND_DIR = figs/handdrawn/
RASTER_DIR = figs/raster/
HAND_FIGS = 
SVG_FIGS = $(wildcard $(HAND_DIR)/*.svg) $(wildcard $(RASTER_DIR)/*.svg)
RASTER_FIGS = $(RASTER_DIR)/161.jpg

HAND_EPS = $(HAND_FIGS:.fig=.eps)
HAND_PDF = $(HAND_EPS:.eps=.pdf)
SVG_PDF = $(SVG_FIGS:.svg=.pdf)

ALL_FIGS = $(HAND_FIGS) $(RASTER_FIGS) $(SVG_PDF)

all: draft final

draft: $(DRAFT).pdf
final: $(FINAL).pdf

%.ps : %.fig
	fig2dev -L ps -c $< > $@

# fig2eps is nice, you can download it from
# http://soliton.science.uva.nl/~kager/figepspdf/fig2eps.htm
%.eps : %.fig
	 ./fig2eps -f $<

%.pdfnoembed : %.eps
	 epstopdf $< --outfile $@

%.pdf : %.pdfnoembed
	ps2pdf13 -dPDFSETTINGS=/prepress $< $@

%.pdf : %.svg
	inkscape $< --export-pdf $@

$(DRAFT).pdf : $(DRAFT_SOURCES) $(ALL_FIGS) $(DRAFT).bbl
	pdflatex $(DRAFT)

$(DRAFT).aux : $(DRAFT_SOURCES) $(ALL_FIGS)
	pdflatex $(DRAFT)

$(DRAFT).bbl : $(DRAFT_SOURCES) $(BIB_SOURCES) $(DRAFT).aux 
	bibtex $(DRAFT)

$(FINAL).pdf : $(FINAL_SOURCES) $(ALL_FIGS) $(FINAL).bbl
	pdflatex $(FINAL)

$(FINAL).aux : $(FINAL_SOURCES) $(ALL_FIGS)
	pdflatex $(FINAL)

$(FINAL).bbl : $(FINAL_SOURCES) $(BIB_SOURCES) $(FINAL).aux 
	bibtex $(FINAL)

clean:
	rm -f $(DRAFT).pdf $(DRAFT).out $(DRAFT).dvi $(DRAFT).bbl $(DRAFT).aux $(DRAFT).blg $(DRAFT).log $(FINAL).pdf $(FINAL).out $(FINAL).dvi $(FINAL).bbl $(FINAL).aux $(FINAL).blg $(FINAL).log $(SVG_PDF)
