%no-lines
%x COMMENT MAIN

%%

.*                         {
                               _StoredLine = matched();
                               push(_StoredLine);
                               popStartCondition();
                               _Reset = true;
                           }

<COMMENT>
{
"*/"                       popStartCondition(); // 多行注释结束
.                          // 跳过所有多行注释内容
\n                         pushStartCondition(StartCondition_::INITIAL);
<<EOF>>                    throw;
}

<MAIN>
{
"//".*\n                   // 跳过单行注释
"/*"                       pushStartCondition(StartCondition_::COMMENT); // 开始识别多行注释

void                       return Parser::VOID;
int                        return Parser::INT;
return                     return Parser::RETURN;
if                         return Parser::IF;
else                       return Parser::ELSE;
while                      return Parser::WHILE;
struct                     return Parser::STRUCT;

[[:alpha:]_][[:alnum:]_]*  return Parser::ID_TEXT;
[0-9]+                     return Parser::NUM;

[ \t]                      // 跳过空白符
[+\-*/<>=()\[\]\{\},;]     return matched()[0]; // 单字符TOKEN
"<="                       return Parser::LE;
">="                       return Parser::GE;
"=="                       return Parser::EQ;
"!="                       return Parser::NE;

\n                         pushStartCondition(StartCondition_::INITIAL);
.                          throw; // 未知字符
}
