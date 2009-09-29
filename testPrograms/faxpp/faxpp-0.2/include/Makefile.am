nobase_include_HEADERS = \
faxpp/error.h \
faxpp/event.h \
faxpp/parser.h \
faxpp/text.h \
faxpp/token.h \
faxpp/tokenizer.h \
faxpp/transcode.h

docs: $(top_srcdir)/docs/.api-timestamp

$(top_srcdir)/docs/.api-timestamp: $(nobase_include_HEADERS) $(top_srcdir)/docs/Doxyfile.api $(top_srcdir)/docs/header.html
	cd $(top_srcdir)/docs && \
		doxygen Doxyfile.api && \
		touch .api-timestamp
