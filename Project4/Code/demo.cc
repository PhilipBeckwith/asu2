#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "parser.h"

struct StatementNode * parse_generate_intermediate_representation()
{
  Parser parse;
  return parse.parse();
}
