%no-lines
%x COMMENT

%%

"//".*\n          // 跳过单行注释
"/*"              begin(StartCondition_::COMMENT); // 开始识别多行注释
<COMMENT>"*/"     begin(StartCondition_::INITIAL); // 多行注释结束
<COMMENT>.|\n     // 跳过所有多行注释内容
<COMMENT><<EOF>>  ErrorHandler::PrintWarning("Unterminated comment", GetLocation());

void    return Parser::VOID;
bool    return Parser::BOOL;
char    return Parser::CHAR;
short   return Parser::SHORT;
int     return Parser::INT;
long    return Parser::LONG;
float   return Parser::FLOAT;
double  return Parser::DOUBLE;

true    return Parser::TRUE;
false   return Parser::FALSE;
return  return Parser::RETURN;
if      return Parser::IF;
else    return Parser::ELSE;
while   return Parser::WHILE;
struct  return Parser::STRUCT;

[[:alpha:]_][[:alnum:]_]*                                  return Parser::ID_TEXT;
[0-9]+                                                     return Parser::CONSTINT;
0[bB][0-1]+                                                return Parser::CONSTINT_BIN;
0[0-7]+                                                    return Parser::CONSTINT_OCT;
0[xX][0-9a-fA-F]+                                          return Parser::CONSTINT_HEX;
(([0-9]+\.[0-9]+)|(\.[0-9]+)|([0-9]+\.))(e(\+|-)?[0-9]+)?  return Parser::CONSTFP;

[ \t\n]                  // 跳过空白符
[+\-*/<>=()\[\]\{\},;&]  return matched()[0]; // 单字符TOKEN
"<="                     return Parser::LE;
">="                     return Parser::GE;
"=="                     return Parser::EQ;
"!="                     return Parser::NE;

.                        ErrorHandler::PrintWarning("Unknown character ignored: " + matched(), GetLocation()); // 未知字符
