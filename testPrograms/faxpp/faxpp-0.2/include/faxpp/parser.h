/*
 * Copyright 2007 Doxological Ltd.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __FAXPP__PARSER_H
#define __FAXPP__PARSER_H

#include <stdio.h>

#include "error.h"
#include "transcode.h"
#include "event.h"

/**
 * \mainpage
 *
 * Faxpp is a small, fast XML pull parser written in C with an API that can return UTF-8 or UTF-16 strings. It currently has no DTD support, although it is planned.
 *
 * Faxpp is written by John Snelson, and is released under the terms of the Apache Licence v2.
 *
 * Faxpp provides two different APIs:
 *
 * \li The FAXPP_Parser API would normally suit the needs of most users.
 * It provides an event based pull API for parsing XML documents.
 * \li The FAXPP_Tokenizer API is a low level API giving direct access to the XML tokenizer.
 * Output from this is not compliant with the XML specifications.
 *
 * \section Performance
 *
 * Faxpp's main performance benefit comes from it's ability to not copy strings - instead
 * the FAXPP_Text structure will point directly to the tokenizer's buffer when possible.
 * This is possible when:
 *
 * \li The output string encoding is the same as the XML document's encoding.
 * \li The event / token does not cross a buffer boundary when streaming input.
 * \li The parser is not set to null terminate it's strings.
 *
 * Therefore, to maximize the performance from faxpp the following steps can be taken:
 *
 * \li Choose not to null terminate output strings.
 * \li Choose an output string encoding that is the same as most of the input XML documents that will be parsed.
 * \li Stream XML documents using a large a buffer as possible.
 * \li Choose to always recieve the output strings in the same encoding as the document, by setting FAXPP_set_encode() to 0.
 *
 * \section Downloads
 *
 * \li Faxpp can be downloaded from here: http://sourceforge.net/project/showfiles.php?group_id=201903
 * \li Anonymous access to the subversion repository is also available. Instructions can be found here: http://sourceforge.net/svn/?group_id=201903
 * \li You can browse the subversion repository here: http://faxpp.svn.sourceforge.net/viewvc/faxpp/
 *
 * \section Support
 *
 * \li There is a user mailing list, faxpp-user, which is the appropriate place to direct any questions or problems. You can join the mailing list here:
 * https://lists.sourceforge.net/lists/listinfo/faxpp-user
 * \li Archives for the mailing list are available here: http://sourceforge.net/mailarchive/forum.php?forum_name=faxpp-user
 * \li If you have a bug to report, you may wish to enter it into our bug database, here: http://sourceforge.net/tracker/?atid=979420&group_id=201903
 *
 */

/**
 * \struct FAXPP_Parser
 * The parser structure. Details of the structure are private.
 *
 * \see parser.h
 */

/// The parser structure. Details of the structure are private.
typedef struct FAXPP_ParserEnv_s FAXPP_Parser;

/// The type of checks to perform whilst parsing
typedef enum {
  /// Perform no additional checks above those performed by tokenization - using this mode, the parser
  /// will \b not be compliant with the XML specifications.
  NO_CHECKS_PARSE_MODE,
  /// Perform the checks required of a well formed parser, as well as resolving
  /// namespace prefixes to their respective URIs.
  WELL_FORMED_PARSE_MODE
} FAXPP_ParseMode;

/**
 * The function called when faxpp recieves a PREMATURE_END_OF_BUFFER error from the
 * tokenizer. The function should read the next chunk of input into the buffer provided,
 * returning the length of the data read.
 *
 * \param userData The user data supplied to the FAXPP_init_parse_callback() method
 * \param[out] buffer The buffer to read the data into
 * \param length The length of the buffer
 *
 * \return The number of bytes read - this will be less than length if the end of the input is reached
 */
typedef unsigned int (*FAXPP_ReadCallback)(void *userData, void *buffer, unsigned int length);

/**
 * Creates a parser object
 *
 * \param mode The type of checks the parser should perform
 * \param encode The encoding function to use when encoding event values
 *
 * \return A pointer to the parser object, or 0 if out of memory.
 *
 * \relatesalso FAXPP_Parser
 */
FAXPP_Parser *FAXPP_create_parser(FAXPP_ParseMode mode, FAXPP_EncodeFunction encode);

/**
 * Frees a parser object
 *
 * \param parser The parser to free
 *
 * \relatesalso FAXPP_Parser
 */
void FAXPP_free_parser(FAXPP_Parser *parser);

/**
 * Sets whether the parser will null terminate the strings in the event values.
 * The default is not to null terminate strings, as this is generally more efficient.
 *
 * Setting this parameter whilst a parse is in progress has undefined results.
 *
 * Null terminating the event strings will involve copying the strings, and so
 * will be slower where copying strings was not otherwise necessary. The
 * FAXPP_Text::len field will not include the null in it's count of bytes, and so
 * will be identical whether the parser is null terminating strings or not.
 * 
 * \param parser
 * \param boolean Whether to null terminate the event strings or not
 *
 * \relatesalso FAXPP_Parser
 */
void FAXPP_set_null_terminate(FAXPP_Parser *parser, unsigned int boolean);

/**
 * Sets whether the parser will normalize attributes values into a single string.
 * This option is off by default for NO_CHECKS_PARSE_MODE, and on for other parser
 * modes.
 *
 * The XML specification requires conformant parsers to normalize attribute values
 * by expanding entity references and turning all whitespace to &amp;#x20; characters.
 * This option will have no effect on a parser in NO_CHECKS_PARSE_MODE, since this
 * mode will never normalize attribute values.
 *
 * Setting this parameter whilst a parse is in progress has undefined results.
 *
 * Normalizing attribute values will involve copying the strings, and so
 * will be slower where copying strings was not otherwise necessary.
 * 
 * \param parser
 * \param boolean Whether to normalize attribute values
 *
 * \relatesalso FAXPP_Parser
 */
void FAXPP_set_normalize_attrs(FAXPP_Parser *parser, unsigned int boolean);

/**
 * Sets the encoding that the parser will use when encoding event values.
 *
 * Setting this parameter whilst a parse is in progress has undefined results.
 *
 * \param parser
 * \param encode The encoding function to use when encoding event values
 *
 * \relatesalso FAXPP_Parser
 */
void FAXPP_set_encode(FAXPP_Parser *parser, FAXPP_EncodeFunction encode);

/**
 * Returns the current FAXPP_DecodeFunction that the parser is using.
 * 
 * \param parser
 * \return The decode function
 *
 * \relatesalso FAXPP_Parser
 */
FAXPP_DecodeFunction FAXPP_get_decode(const FAXPP_Parser *parser);

/**
 * Sets the FAXPP_DecodeFunction that the parser uses to decode the XML document.
 * This will typically be called when an encoding declaration is read, to switch to
 * the correct decode function.
 * 
 * \param parser
 * \param decode The decode function
 *
 * \relatesalso FAXPP_Parser
 */
void FAXPP_set_decode(FAXPP_Parser *parser, FAXPP_DecodeFunction decode);

/**
 * Initialize the parser to parse the given buffer. This will halt any
 * parse that was already in progress.
 *
 * The buffer provided must remain valid and unchanged during the time that
 * the parser is using it, since a copy of it is \e not made. The user remains
 * responsible for deleting the buffer.
 *
 * \param parser The parser to initialize
 * \param buffer A pointer to the start of the buffer to parse
 * \param length The length of the given buffer
 * \param done Set to non-zero if this is the last buffer from the input
 *
 * \retval UNSUPPORTED_ENCODING If the encoding sniffing algorithm cannot recognize
 * the encoding of the buffer
 * \retval OUT_OF_MEMORY
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Parser
 */
FAXPP_Error FAXPP_init_parse(FAXPP_Parser *parser, void *buffer, unsigned int length, unsigned int done);

/**
 * Initialize the parser to parse the given file. This will halt any
 * parse that was already in progress.
 *
 * The file provided must remain valid during the time that the parser is using it.
 * The user remains responsible for closing the file after parsing has ended..
 *
 * \param parser The parser to initialize
 * \param file The file descriptor of the file to parse
 *
 * \retval UNSUPPORTED_ENCODING If the encoding sniffing algorithm cannot recognize
 * the encoding of the buffer
 * \retval OUT_OF_MEMORY
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Parser
 */
/*FAXPP_Error FAXPP_init_parse_file(FAXPP_Parser *parser, FILE *file);*/

/**
 * Initialize the parser to parse using the given read callback. This will halt any
 * parse that was already in progress.
 *
 * \param parser The parser to initialize
 * \param callback The read callback function to use to retrieve the parse input
 * \param userData The user data to be passed to the callback function when it is called
 *
 * \retval UNSUPPORTED_ENCODING If the encoding sniffing algorithm cannot recognize
 * the encoding of the buffer
 * \retval OUT_OF_MEMORY
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Parser
 */
FAXPP_Error FAXPP_init_parse_callback(FAXPP_Parser *parser, FAXPP_ReadCallback callback, void *userData);

/**
 * Instructs the parser to release any dependencies it has on it's current buffer.
 *
 * This is typically called on recieving a PREMATURE_END_OF_BUFFER error, before
 * using FAXPP_continue_parse() to provide a new buffer. In this case, the buffer data
 * between *buffer_position and the end of the buffer need to be copied into the start of
 * the new buffer.
 *
 * \param parser
 * \param[out] buffer_position Set to a pointer in the current buffer that the tokenizer
 * has tokenized up to
 *
 * \retval OUT_OF_MEMORY
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Parser
 */
FAXPP_Error FAXPP_release_buffer(FAXPP_Parser *parser, void **buffer_position);

/**
 * Provides a new buffer for the parser to continue parsing.
 *
 * FAXPP_release_buffer() should have been called before this,
 * and the remaining data in the old buffer transferred to the new one.
 * 
 * \param parser
 * \param buffer A pointer to the start of the buffer to parse
 * \param length The length of the given buffer
 * \param done Set to non-zero if this is the last buffer from the input
 *
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Parser
 */
FAXPP_Error FAXPP_continue_parse(FAXPP_Parser *parser, void *buffer,
                                 unsigned int length, unsigned int done);

/**
 * Parses the next event, placing the information for it
 * into the current event.
 * 
 * \param parser
 *
 * \retval ELEMENT_NAME_MISMATCH
 * \retval NO_URI_FOR_PREFIX
 * \retval DUPLICATE_ATTRIBUTES
 * \retval DOUBLE_DASH_IN_COMMENT
 * \retval PREMATURE_END_OF_BUFFER
 * \retval INVALID_START_OF_COMMENT
 * \retval INVALID_CHAR_IN_START_ELEMENT
 * \retval INVALID_CHAR_IN_ATTRIBUTE
 * \retval INVALID_CHAR_IN_END_ELEMENT
 * \retval NON_WHITESPACE_OUTSIDE_DOC_ELEMENT
 * \retval BAD_ENCODING
 * \retval UNSUPPORTED_ENCODING
 * \retval ADDITIONAL_DOCUMENT_ELEMENT
 * \retval INVALID_CHAR_IN_PI_NAME
 * \retval INVALID_PI_NAME_OF_XML
 * \retval INVALID_CHAR_IN_ELEMENT_NAME
 * \retval INVALID_CHAR_IN_ATTRIBUTE_NAME
 * \retval RESTRICTED_CHAR
 * \retval INVALID_CHAR_IN_ENTITY_REFERENCE
 * \retval INVALID_CHAR_IN_CHAR_REFERENCE
 * \retval INVALID_CHAR_IN_XML_DECL
 * \retval EXPECTING_EQUALS
 * \retval EXPECTING_WHITESPACE
 * \retval UNKNOWN_XML_VERSION
 * \retval INVALID_ENCODING_VALUE
 * \retval OUT_OF_MEMORY
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Parser
 */
FAXPP_Error FAXPP_next_event(FAXPP_Parser *parser);

/**
 * Returns the current event produced by the parser when FAXPP_next_event() was called.
 * 
 * \param parser
 * \return The current event
 *
 * \relatesalso FAXPP_Parser
 */
const FAXPP_Event *FAXPP_get_current_event(const FAXPP_Parser *parser);

/**
 * Look up the given prefix in the parser's namespace mappings, returning the namespace URI
 * in the uri parameter.
 *
 * This method will not work correctly if the NO_CHECKS_PARSE_MODE is used, since the namespace
 * mappings are not maintained in this mode.
 *
 * \param parser
 * \param prefix The prefix to loookup
 * \param[out] uri The URI that the prefix maps to
 *
 * \retval NO_URI_FOR_PREFIX If a URI cannot be found
 * \retval NO_ERROR
 *
 * \relatesalso FAXPP_Parser
 */
FAXPP_Error FAXPP_lookup_namespace_uri(const FAXPP_Parser *parser, const FAXPP_Text *prefix, FAXPP_Text *uri);

/**
 * Returns the current element nesting level in the XML document.
 * 
 * \param parser
 * \return The current nesting level
 *
 * \relatesalso FAXPP_Parser
 */
unsigned int FAXPP_get_nesting_level(const FAXPP_Parser *parser);

/**
 * Returns the line that the current error occured on.
 * 
 * \param parser
 * \return The line number
 *
 * \relatesalso FAXPP_Parser
 */
unsigned int FAXPP_get_error_line(const FAXPP_Parser *parser);

/**
 * Returns the column that the current error occured on.
 * 
 * \param parser
 * \return The column number
 *
 * \relatesalso FAXPP_Parser
 */
unsigned int FAXPP_get_error_column(const FAXPP_Parser *parser);

#endif
