# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

PWD ?= (shell pwd)

SHADERS_DIR              := $(PWD)/src/renderer/shaders
SHADERS_OUT_DIR          := $(PWD)/src/renderer/shaders/c
SHADERS                  := $(shell find $(SHADERS_DIR) -regex '.*\.\(fs\|vs\|gs\)')

EXAMPLES_SHADERS_DIR     := $(PWD)/examples/assets/shaders
EXAMPLES_SHADERS_OUT_DIR := $(PWD)/examples/assets/shaders/c
EXAMPLES_SHADERS         := $(shell find $(EXAMPLES_SHADERS_DIR) -regex '.*\.\(fs\|vs\|gs\)')

.PHONY: shaders
shaders: shaders-brenta shaders-examples

.PHONY: shaders-brenta
shaders-brenta: $(SHADERS)
	python3 utils/shaders_to_c.py --out-dir $(SHADERS_OUT_DIR) $(SHADERS)

.PHONY: shaders-examples
shaders-examples:
	python3 utils/shaders_to_c.py --out-dir $(EXAMPLES_SHADERS_OUT_DIR) $(EXAMPLES_SHADERS)	

DOCS_DIR := docs
HTML_DIR := docs/html
DOCS := ${wildcard ${DOCS_DIR}/*.md}
HTML := ${patsubst ${DOCS_DIR}/%.md, ${HTML_DIR}/%.html, ${DOCS}}
HTML_INTRO := utils/website/intro.html
HTML_OUTRO := utils/website/outro.html
TMP_FILE := /tmp/padoc-out.html
HIGHLIGHT_STYLE := tango
PANDOC_FLAGS := --highlight-style ${HIGHLIGHT_STYLE}

html: doxygen ${HTML}

doxygen:
	doxygen ./utils/doxygen/doxygen.conf
	mv ${HTML_DIR}/index.html ${HTML_DIR}/doxygen.html
	cp -r utils/docs-images ${HTML_DIR}/images
	cp -r utils/website/*.gif ${HTML_DIR}/images
	cp -r docs/*.png ${HTML_DIR}

$(HTML_DIR)/%.html: ${DOCS_DIR}/%.md ${HTML_INTRO} ${HTML_OUTRO} | ${HTML_DIR}
	pandoc $< -o ${TMP_FILE} ${PANDOC_FLAGS}
	cp ${HTML_INTRO} $@
	cat ${TMP_FILE} >> $@
	cat ${HTML_OUTRO} >> $@
	sed -i 's/\.md/\.html/g' $@
	sed -i 's/.\/html/.\//g' $@

$(HTML_DIR):
	mkdir -p ${HTML_DIR}

clean-html:  ## Remove the html docs directory
	rm ${HTML_DIR}/*.html
