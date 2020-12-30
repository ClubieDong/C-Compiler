%scanner ../Scanner/Scanner.h
%baseclass-preinclude includes.h
%default-actions off
%no-lines

%stype std::unique_ptr<ast::Base>

%token VOID INT
%token RETURN IF ELSE WHILE STRUCT
%token NUM
%token ID

%token PTR
%token ARR

%left ','
%right '='
%left '<' '>' LE GE EQ NE
%left '+' '-'
%left '*' '/'
%right NEG
%token '(' ')'

%%

Program:
  StatementList  { $1->Show(); }
;

Expression:
  NUM                            { $$ = std::make_unique<ast::Constant>(std::stod(_Scanner->matched()));       }
| ID                             { $$ = std::make_unique<ast::Variable>(_Scanner->matched());                  }
| Expression '=' Expression      { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::ASSIGN);        }
| Expression '<' Expression      { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::LESS);          }
| Expression '>' Expression      { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::GREATER);       }
| Expression LE Expression       { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::LESS_EQUAL);    }
| Expression GE Expression       { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::GREATER_EQUAL); }
| Expression EQ Expression       { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::EQUAL);         }
| Expression NE Expression       { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::NOT_EQUAL);     }
| Expression '+' Expression      { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::ADD);           }
| Expression '-' Expression      { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::SUB);           }
| Expression '*' Expression      { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::MUL);           }
| Expression '/' Expression      { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::DIV);           }
| '-' Expression %prec NEG       { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::NEG);               }
| Expression '(' OptArgList ')'  { $$ = std::move($3); ast::cast<ast::CallExpr>($$)->SetFunc($1);              }
| '(' Expression ')'             { $$ = std::move($2);                                                         }
;

Statement:
  ';'                                             { $$ = std::make_unique<ast::ExprStmt>();                           }
| Expression ';'                                  { $$ = std::make_unique<ast::ExprStmt>($1);                         }
| IF '(' Expression ')' Statement                 { $$ = std::make_unique<ast::IfStmt>($3, $5);                       }
| IF '(' Expression ')' Statement ELSE Statement  { $$ = std::make_unique<ast::IfStmt>($3, $5, $7);                   }
| WHILE '(' Expression ')' Statement              { $$ = std::make_unique<ast::WhileStmt>($3, $5);                    }
| RETURN ';'                                      { $$ = std::make_unique<ast::ReturnStmt>();                         }
| RETURN Expression ';'                           { $$ = std::make_unique<ast::ReturnStmt>($2);                       }
| '{' StatementList '}'                           { $$ = std::move($3);                                               }
| PrimitiveType VarDeclList ';'                   { $$ = std::move($2); ast::cast<ast::VarDeclStmt>($$)->SetType($1); }
;

StatementList:
  /* empty */              { $$ = std::make_unique<ast::StatementList>();                        }
| StatementList Statement  { $$ = std::move($1); ast::cast<ast::StatementList>($$)->AddStmt($2); }
;

OptArgList:
  /* empty */  { $$ = std::make_unique<ast::CallExpr>(); }
| ArgList      { $$ = std::move($1);                     }
;

ArgList:
  Expression              { $$ = std::make_unique<ast::CallExpr>($1);                     }
| ArgList ',' Expression  { $$ = std::move($1); ast::cast<ast::CallExpr>($$)->AddArg($3); }
;

VarDecl:
  ID                                    { $$ = std::make_unique<ast::PlainVarDecl>(_Scanner->matched()); }
| VarDecl '[' ']' %prec ARR             { $$ = std::make_unique<ast::ArrayVarDecl>($1);                  }
| VarDecl '[' Expression ']' %prec ARR  { $$ = std::make_unique<ast::ArrayVarDecl>($1, $3);              }
| '*' VarDecl %prec PTR                 { $$ = std::make_unique<ast::PointerVarDecl>($2);                }
| '(' VarDecl ')'                       { $$ = std::move($2);                                            }
;

InitVarDecl:
  VarDecl                 { $$ = std::make_unique<ast::InitVarDecl>($1);     }
| VarDecl '=' Expression  { $$ = std::make_unique<ast::InitVarDecl>($1, $3); }
;

PrimitiveType:
  VOID       { $$ = std::make_unique<ast::BasicType>(ast::BasicType::VOID); }
| INT        { $$ = std::make_unique<ast::BasicType>(ast::BasicType::INT);  }
| STRUCT ID  { $$ = std::make_unique<ast::CustomType>(_Scanner->matched()); }
;

VarDeclList:
  InitVarDecl                  { $$ = std::make_unique<ast::VarDeclStmt>($1);                     }
| VarDeclList ',' InitVarDecl  { $$ = std::move($1); ast::cast<ast::VarDeclStmt>($$)->AddVar($3); }
;

OptParamList:
  /* empty */
| VOID
| ParamList
;

ParamList:
  PrimitiveType VarDecl
| ParamList ',' PrimitiveType VarDecl
;

