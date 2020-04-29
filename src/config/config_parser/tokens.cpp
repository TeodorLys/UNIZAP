#include "tokens.h"

const std::string separators::s_separators[separators::size_of_array]{ ";", "{", "}", "[", "]", ",", "(", ")"/*, ":"*/ };
//const std::string arithmetic_operators::s_arithmetic_operators[arithmetic_operators::size_of_array]{ "+", "-", "*", "/", "%" };
//const std::string rational_operators::s_rational_operators[rational_operators::size_of_array]{ "==", ">", "<", ">=", "<=", "!=" };
//const std::string logical_operators::s_logical_operators[logical_operators::size_of_array]{ "&&", "||" };
//const std::string unary_operators::s_unary_operators[unary_operators::size_of_array]{ "+", "-", "++", "--", "!" };
const std::string assignment_operators::s_assignment_operators[assignment_operators::size_of_array]{ ":=", "=" };
const std::string types::s_types[types::size_of_array]{ "ACCESS", "DIR_PATH", "FILES", "FLAGS", "FILE_NAME", "OUTPUT_PATH"};  // UNUSED
const std::string predefined_literals::s_predefined_literals[predefined_literals::size_of_array]{ "INCLUDE_DIRECTORY", "FORCE", "DISABLE_CONFIRM_CURL", "UPLOAD", "DOWNLOAD" };
//const std::string keywords::s_keywords[keywords::size_of_array]{ "if", "else", "for", "while", "_declvar", "end", "const" };
