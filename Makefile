.PHONY: format
format:
	find include/brenta src examples tests -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -style=file:utils/.clang-format -i

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
