%scanner ../Scanner/Scanner.h
%baseclass-preinclude includes.h
%default-actions off
%no-lines

%stype std::unique_ptr<ast::Base>

%token VOID INT RETURN IF ELSE WHILE ID NUM

%left ','
%right '='
%left '<' '>' LE GE EQ NE
%left '+' '-'
%left '*' '/'
%right NEG
%token '(' ')'

%%

Program:
  StatementList  { std::cout << *$1; }
;

Expression:
  NUM                            { $$ = ast::Constant::Create(std::stod(scanner.matched()));                             }
| ID                             { $$ = ast::Variable::Create(scanner.matched());                                        }
| Expression '=' Expression      { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::ASSIGN);        }
| Expression '<' Expression      { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::LESS);          }
| Expression '>' Expression      { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::GREATER);       }
| Expression LE Expression       { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::LESS_EQUAL);    }
| Expression GE Expression       { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::GREATER_EQUAL); }
| Expression EQ Expression       { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::EQUAL);         }
| Expression NE Expression       { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::NOT_EQUAL);     }
| Expression '+' Expression      { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::ADD);           }
| Expression '-' Expression      { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::SUB);           }
| Expression '*' Expression      { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::MUL);           }
| Expression '/' Expression      { $$ = ast::BiOpExpr::Create(std::move($1), std::move($3), ast::BiOpExpr::DIV);           }
| '-' Expression %prec NEG       { $$ = ast::UnOpExpr::Create(std::move($2), ast::UnOpExpr::NEG);                          }
| Expression '(' OptArgList ')'  { $$ = ast::CallExpr::Create(std::move($1), std::move($3));                               }
| '(' Expression ')'             { $$ = std::move($2);                                                                     }
;

Statement:
  ';'                                             { $$ = ast::ExprStmt::Create();                                          }
| Expression ';'                                  { $$ = ast::ExprStmt::Create(std::move($1));                             }
| IF '(' Expression ')' Statement                 { $$ = ast::IfStmt::Create(std::move($3), std::move($5));                }
| IF '(' Expression ')' Statement ELSE Statement  { $$ = ast::IfStmt::Create(std::move($3), std::move($5), std::move($7)); }
| WHILE '(' Expression ')' Statement              { $$ = ast::WhileStmt::Create(std::move($3), std::move($5));             }
| RETURN ';'                                      { $$ = ast::ReturnStmt::Create();                                        }
| RETURN Expression ';'                           { $$ = ast::ReturnStmt::Create(std::move($2));                           }
| '{' StatementList '}'                           { $$ = std::move($2);                                                    }
;

StatementList:
  /* empty */              { $$ = ast::StatementList::Create();                                                   }
| StatementList Statement  { $$ = std::move($1); dynamic_cast<ast::StatementList*>($$.get())->Add(std::move($2)); }
;

OptArgList:
  /* empty */  { $$ = ast::ArgList::Create(); }
| ArgList      { $$ = std::move($1);          }
;

ArgList:
  Expression              { $$ = ast::ArgList::Create(std::move($1));                                      }
| ArgList ',' Expression  { $$ = std::move($1); dynamic_cast<ast::ArgList*>($$.get())->Add(std::move($3)); }
;
