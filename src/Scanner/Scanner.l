%x COMMENT

%%

"//".*\n                   // 跳过单行注释
"/*"                       begin(StartCondition_::COMMENT); // 开始识别多行注释
<COMMENT>"*/"              begin(StartCondition_::INITIAL); // 多行注释结束
<COMMENT>.|\n              // 跳过所有多行注释内容
<COMMENT><<EOF>>           throw;

void                       return Parser::VOID;
int                        return Parser::INT;
return                     return Parser::RETURN;
if                         return Parser::IF;
else                       return Parser::ELSE;
while                      return Parser::WHILE;
struct                     return Parser::STRUCT;

[[:alpha:]_][[:alnum:]_]*  return Parser::ID;
[0-9]+                     return Parser::NUM;

[ \t\n]                    // 跳过空白符
[+\-*/<>=()\[\]\{\},;]     return matched()[0]; // 单字符TOKEN
"<="                       return Parser::LE;
">="                       return Parser::GE;
"=="                       return Parser::EQ;
"!="                       return Parser::NE;

.                          throw; // 未知字符
