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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <faxpp/parser.h>

void
output_text(const FAXPP_Text *text, FILE *stream)
{
  char *buffer = (char*)text->ptr;
  char *buffer_end = buffer + text->len;

  while(buffer < buffer_end) {
    putc(*buffer++, stream);
  }
}

void output_attr_value(const FAXPP_AttrValue *atval, FILE *stream)
{
  while(atval) {
    switch(atval->type) {
    case CHARACTERS_EVENT:
      output_text(&atval->value, stream);
      break;
    case ENTITY_REFERENCE_EVENT:
      fprintf(stream, "&");
      output_text(&atval->name, stream);
      fprintf(stream, ";");
      break;
    case DEC_CHAR_REFERENCE_EVENT:
      fprintf(stream, "&#");
      output_text(&atval->name, stream);
      fprintf(stream, ";");
      break;
    case HEX_CHAR_REFERENCE_EVENT:
      fprintf(stream, "&#x");
      output_text(&atval->name, stream);
      fprintf(stream, ";");
      break;
    default:
      break;
    }
    atval = atval->next;
  }
}

void
output_event(const FAXPP_Event *event, FILE *stream)
{
  int i;

  switch(event->type) {
  case START_DOCUMENT_EVENT:
    if(event->version.ptr != 0) {
      fprintf(stream, "<?xml version=\"");
      output_text(&event->version, stream);
      if(event->encoding.ptr != 0) {
        fprintf(stream, "\" encoding=\"");
        output_text(&event->encoding, stream);
      }
      if(event->standalone.ptr != 0) {
        fprintf(stream, "\" standalone=\"");
        output_text(&event->standalone, stream);
      }
      fprintf(stream, "\"?>");
    }
    break;
  case END_DOCUMENT_EVENT:
    break;
  case START_ELEMENT_EVENT:
  case SELF_CLOSING_ELEMENT_EVENT:
    fprintf(stream, "<");
    if(event->prefix.ptr != 0) {
      output_text(&event->prefix, stream);
      fprintf(stream, ":");
    }
    output_text(&event->name, stream);

    for(i = 0; i < event->attr_count; ++i) {
      fprintf(stream, " ");
      if(event->attrs[i].prefix.ptr != 0) {
        output_text(&event->attrs[i].prefix, stream);
        fprintf(stream, ":");
      }
      output_text(&event->attrs[i].name, stream);
      fprintf(stream, "=\"");
      output_attr_value(&event->attrs[i].value, stream);
      fprintf(stream, "\"");
    }

    if(event->type == SELF_CLOSING_ELEMENT_EVENT)
      fprintf(stream, "/>");
    else
      fprintf(stream, ">");
    break;
  case END_ELEMENT_EVENT:
    fprintf(stream, "</");
    if(event->prefix.ptr != 0) {
      output_text(&event->prefix, stream);
      fprintf(stream, ":");
    }
    output_text(&event->name, stream);
    fprintf(stream, ">");
    break;
  case CHARACTERS_EVENT:
    output_text(&event->value, stream);
    break;
  case CDATA_EVENT:
    fprintf(stream, "<![CDATA[");
    output_text(&event->value, stream);
    fprintf(stream, "]]>");
    break;
  case IGNORABLE_WHITESPACE_EVENT:
    output_text(&event->value, stream);
    break;
  case COMMENT_EVENT:
    fprintf(stream, "<!--");
    output_text(&event->value, stream);
    fprintf(stream, "-->");
    break;
  case PI_EVENT:
    fprintf(stream, "<?");
    output_text(&event->name, stream);
    if(event->value.ptr != 0) {
      fprintf(stream, " ");
      output_text(&event->value, stream);
    }
    fprintf(stream, "?>");
    break;
  case ENTITY_REFERENCE_EVENT:
    fprintf(stream, "&");
    output_text(&event->name, stream);
    fprintf(stream, ";");
    break;
  case DEC_CHAR_REFERENCE_EVENT:
    fprintf(stream, "&#");
    output_text(&event->name, stream);
    fprintf(stream, ";");
    break;
  case HEX_CHAR_REFERENCE_EVENT:
    fprintf(stream, "&#x");
    output_text(&event->name, stream);
    fprintf(stream, ";");
    break;
  case NO_EVENT:
    break;
  }
}

void error(FAXPP_Error err, unsigned int line, unsigned int column)
{
  if(line != 0) {
    printf("%03d:%03d FAXPP_Error: %s\n", line, column, FAXPP_err_to_string(err));
  } else {
    printf("FAXPP_Error: %s\n", FAXPP_err_to_string(err));
  }
  exit(1);
}

void printIndent(unsigned int indent)
{
  while(indent--) printf("  ");
}

#define text_equal(text, str) (((text).len == strlen((str))) ? memcmp((text).ptr, (str), (text).len) == 0 : 0)

const FAXPP_Attribute *find_attribute(const FAXPP_Event *event, const char *name)
{
  int i;

  for(i = 0; i < event->attr_count; ++i) {
    if(text_equal(event->attrs[i].name, name))
      return &event->attrs[i];
  }

  return 0;
}

void calculateBase(const char *testFile, const FAXPP_AttrValue *atval, char *out)
{
  char *ptr = out;

  int len = strlen(testFile);
  memcpy(ptr, testFile, len);
  ptr += len;

  // Search backwards to the last "/", or the begining of the string
  while(ptr > out && *(ptr - 1) != '/') --ptr;

  while(atval) {
    memcpy(ptr, atval->value.ptr, atval->value.len);
    ptr += atval->value.len;
    atval = atval->next;
  }

  // null terminate
  *ptr = 0;
}

FAXPP_Error run_test_case(const char *filename, unsigned int *errLine)
{
  FAXPP_Parser *testparser = FAXPP_create_parser(WELL_FORMED_PARSE_MODE, FAXPP_utf8_encode);

  FILE *file = fopen(filename, "r");
  if(file == 0) {
    printf("Could not open xml file '%s': %s\n", filename, strerror(errno));
    exit(1);
  }

  FAXPP_Error err = FAXPP_init_parse_file(testparser, file);
  if(err == NO_ERROR) {
    while((err = FAXPP_next_event(testparser)) == 0) {
      if(FAXPP_get_current_event(testparser)->type == END_DOCUMENT_EVENT)
        break;
    }
  }

  if(err != 0) {
    *errLine = FAXPP_get_error_line(testparser);
  }

  fclose(file);
  FAXPP_free_parser(testparser);

  return err;
}

int
main(int argc, char **argv)
{
  FAXPP_Error err;
  const FAXPP_Event *event;
  const FAXPP_Attribute *attr;
  char base_buffer[1024];
  char file_buffer[1024];
  FAXPP_Error result;
  unsigned int errLine;
  int output_events = 0;

  int test_failures = 0;
  int test_passes = 0;
  int test_skips = 0;

  if(argc < 2) {
    printf("Too few arguments\n");
    exit(-1);
  }

  FAXPP_Parser *parser = FAXPP_create_parser(WELL_FORMED_PARSE_MODE, FAXPP_utf8_encode);

  const char *testFile = argv[1];

  FILE *file = fopen(testFile, "r");
  if(file == 0) {
    printf("Could not open test file: %s\n", strerror(errno));
    exit(1);
  }

  err = FAXPP_init_parse_file(parser, file);
  if(err != NO_ERROR) error(err, 0, 0);

  while((err = FAXPP_next_event(parser)) == 0) {
    event = FAXPP_get_current_event(parser);

    switch(event->type) {
    case START_ELEMENT_EVENT:
    case SELF_CLOSING_ELEMENT_EVENT:
      if(text_equal(event->name, "TESTSUITE")) {
        attr = find_attribute(event, "PROFILE");
        if(attr) {
          printf("\n");
          printIndent(FAXPP_get_nesting_level(parser) - 1);
          output_attr_value(&attr->value, stdout);
          printf(": ");
        }
      }

      else if(text_equal(event->name, "TESTCASES")) {
        attr = find_attribute(event, "PROFILE");
        if(attr) {
          printf("\n");
          printIndent(FAXPP_get_nesting_level(parser) - 1);
          output_attr_value(&attr->value, stdout);
          printf(": ");
        }

        attr = find_attribute(event, "base");
        if(attr && attr->xml_attr) {
          calculateBase(testFile, &attr->value, base_buffer);
        }
      }

      else if(text_equal(event->name, "TEST")) {
/*         if(find_attribute(event, "OUTPUT")) { */
/*           printf("^"); */
/*           fflush(stdout); */
/*           ++test_skips; */
/*           break; */
/*         } */

        attr = find_attribute(event, "URI");
        calculateBase(base_buffer, &attr->value, file_buffer);

        result = run_test_case(file_buffer, &errLine);
        if(result == DOCTYPE_NOT_IMPLEMENTED) {
          printf("^");
          fflush(stdout);
          ++test_skips;
          break;
        }

        // @TYPE is not-wf, error, invalid, or valid
        attr = find_attribute(event, "TYPE");
        if(text_equal(attr->value.value, "not-wf") ||
           text_equal(attr->value.value, "error")) {
          if(result != NO_ERROR) {
            printf(".");
            fflush(stdout);
            ++test_passes;
            break;
          }
        }
        else if(result == NO_ERROR) {
          printf(".");
          fflush(stdout);
          ++test_passes;
          break;
        }

        printf("!");
        fflush(stdout);
        ++test_failures;

        fprintf(stderr, "File: %s\nType: ", file_buffer);
        output_attr_value(&attr->value, stderr);

        attr = find_attribute(event, "ENTITIES");
        if(attr) {
          fprintf(stderr, "\nEntities: ");
          output_attr_value(&attr->value, stderr);
        }

        attr = find_attribute(event, "SECTIONS");
        if(attr) {
          fprintf(stderr, "\nSections: ");
          output_attr_value(&attr->value, stderr);
        }

        if(result != NO_ERROR) {
          fprintf(stderr, "\nError: %s:%i", FAXPP_err_to_string(result), errLine);
        }

        fprintf(stderr, "\n");
        if(event->type == SELF_CLOSING_ELEMENT_EVENT) {
          fprintf(stderr, "\n");
        }
        else {
          output_events = 1;
        }
      }

      break;
    case END_ELEMENT_EVENT:
      if(output_events) {
        output_events = 0;
        fprintf(stderr, "\n\n");
      }
      break; 
    default:
      if(output_events) output_event(event, stderr);
      break;
    case END_DOCUMENT_EVENT:
      goto cleanup;
    }
  }

  if(err != NO_ERROR) error(err, FAXPP_get_error_line(parser),
                            FAXPP_get_error_column(parser));

 cleanup:
  printf("\n\nTests run: %d, Tests passed: %d, Tests skipped: %d, Tests failed: %d (%.3f%%)\n", test_passes + test_failures + test_skips,
         test_passes, test_skips, test_failures, ((double)test_passes / (double)(test_passes + test_failures)) * 100);


  fclose(file);

  FAXPP_free_parser(parser);

  return 0;
}
