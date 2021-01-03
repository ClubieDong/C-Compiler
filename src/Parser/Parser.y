%baseclass-preinclude includes.hpp
%default-actions off
%no-lines

%stype std::unique_ptr<ast::Base>

%token VOID BOOL CHAR SHORT INT LONG FLOAT DOUBLE
%token TRUE FALSE RETURN IF ELSE WHILE STRUCT
%token ID_TEXT
%token CONSTINT CONSTINT_BIN CONSTINT_OCT CONSTINT_HEX CONSTFP

%right PTR
%left ARR

%right '='
%left '<' '>' LE GE EQ NE
%left '+' '-'
%left '*' '/'

%%

Program:
  DeclarationList  { _Root = std::move($1); }
;

ID:
  ID_TEXT  { $$ = std::make_unique<ast::ID>(_Scanner->matched(), _Scanner->GetLocation()); }
;

PrimaryExpression:
  TRUE                { $$ = std::make_unique<ast::Constant>(true);                                         }
| FALSE               { $$ = std::make_unique<ast::Constant>(false);                                        }
| CONSTINT            { $$ = std::make_unique<ast::Constant>(std::stoll(_Scanner->matched()));              }
| CONSTINT_BIN        { $$ = std::make_unique<ast::Constant>(std::stoll(_Scanner->matched(), nullptr, 2));  }
| CONSTINT_OCT        { $$ = std::make_unique<ast::Constant>(std::stoll(_Scanner->matched(), nullptr, 8));  }
| CONSTINT_HEX        { $$ = std::make_unique<ast::Constant>(std::stoll(_Scanner->matched(), nullptr, 16)); }
| CONSTFP             { $$ = std::make_unique<ast::Constant>(std::stod(_Scanner->matched()));               }
| ID                  { $$ = std::make_unique<ast::Variable>($1);                                           }
| '(' Expression ')'  { $$ = std::move($2);                                                                 }
| '(' error ')'       { $$ = nullptr;                                                                       }
;

PostExpression:
  PrimaryExpression                  { $$ = std::move($1);                                            }
| PostExpression '(' OptArgList ')'  { $$ = std::move($3); ast::cast<ast::CallExpr>($$)->SetFunc($1); }
| PostExpression '(' error ')'       { $$ = nullptr;                                                  }
;

PreExpression:
  PostExpression     { $$ = std::move($1);                                           }
| '-' PreExpression  { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::NEG); }
;

Expression:
  PreExpression              { $$ = std::move($1);                                                         }
| Expression '=' Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::ASSIGN);        }
| Expression '<' Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::LESS);          }
| Expression '>' Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::GREATER);       }
| Expression LE Expression   { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::LESS_EQUAL);    }
| Expression GE Expression   { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::GREATER_EQUAL); }
| Expression EQ Expression   { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::EQUAL);         }
| Expression NE Expression   { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::NOT_EQUAL);     }
| Expression '+' Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::ADD);           }
| Expression '-' Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::SUB);           }
| Expression '*' Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::MUL);           }
| Expression '/' Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::DIV);           }
;

Statement:
  ';'                                             { $$ = std::make_unique<ast::ExprStmt>();         }
| error ';'                                       { $$ = nullptr;                                   }
| Expression ';'                                  { $$ = std::make_unique<ast::ExprStmt>($1);       }
| Declaration                                     { $$ = std::move($1);                             }
| IF '(' Expression ')' Statement                 { $$ = std::make_unique<ast::IfStmt>($3, $5);     }
| IF '(' Expression ')' Statement ELSE Statement  { $$ = std::make_unique<ast::IfStmt>($3, $5, $7); }
| WHILE '(' Expression ')' Statement              { $$ = std::make_unique<ast::WhileStmt>($3, $5);  }
| RETURN ';'                                      { $$ = std::make_unique<ast::ReturnStmt>();       }
| RETURN Expression ';'                           { $$ = std::make_unique<ast::ReturnStmt>($2);     }
| '{' StatementList '}'                           { $$ = std::move($2);                             }
| '{' error '}'                                   { $$ = nullptr;                                   }
;

StatementList:
  /* empty */              { $$ = std::make_unique<ast::StatementList>();                        }
| StatementList Statement  { $$ = std::move($1); ast::cast<ast::StatementList>($$)->AddStmt($2); }
| StatementList error      { $$ = std::move($1);                                                 }
;

OptArgList:
  /* empty */  { $$ = std::make_unique<ast::CallExpr>(); }
| ArgList      { $$ = std::move($1);                     }
;

ArgList:
  Expression              { $$ = std::make_unique<ast::CallExpr>($1);                     }
| ArgList ',' Expression  { $$ = std::move($1); ast::cast<ast::CallExpr>($$)->AddArg($3); }
| ArgList ',' error       { $$ = std::move($1);                                           }
;

VarDecl:
  ID                                    { $$ = std::make_unique<ast::VarDecl>($1);       }
| VarDecl '[' ']' %prec ARR             { $$ = std::make_unique<ast::ArrayDecl>($1);     }
| VarDecl '[' Expression ']' %prec ARR  { $$ = std::make_unique<ast::ArrayDecl>($1, $3); }
| VarDecl '[' error ']' %prec ARR       { $$ = nullptr;                                  }
| '*' VarDecl %prec PTR                 { $$ = std::make_unique<ast::PointerDecl>($2);   }
| '&' VarDecl %prec PTR                 { $$ = std::make_unique<ast::ReferenceDecl>($2); }
| '(' VarDecl ')'                       { $$ = std::move($2);                            }
| '(' error ')'                         { $$ = nullptr;                                  }
;

FuncDecl:
  ID '(' OptParamList ')'                { $$ = std::move($3); ast::cast<ast::FuncDecl>($$)->SetName($1); }
| FuncDecl '[' ']' %prec ARR             { $$ = std::make_unique<ast::ArrayDecl>($1);                     }
| FuncDecl '[' Expression ']' %prec ARR  { $$ = std::make_unique<ast::ArrayDecl>($1, $3);                 }
| FuncDecl '[' error ']' %prec ARR       { $$ = nullptr;                                                  }
| '*' FuncDecl %prec PTR                 { $$ = std::make_unique<ast::PointerDecl>($2);                   }
| '&' FuncDecl %prec PTR                 { $$ = std::make_unique<ast::ReferenceDecl>($2);                 }
| '(' FuncDecl ')'                       { $$ = std::move($2);                                            }
| '(' error ')'                          { $$ = nullptr;                                                  }
;

EmptyID:
  /* empty */  { $$ = std::make_unique<ast::ID>("", _Scanner->GetLocation()); }
;

PureDecl:
  EmptyID                            { $$ = std::make_unique<ast::VarDecl>($1);                       }
| EmptyID '(' OptParamList ')'       { $$ = std::move($3); ast::cast<ast::FuncDecl>($$)->SetName($1); }
| Decl '[' ']' %prec ARR             { $$ = std::make_unique<ast::ArrayDecl>($1);                     }
| Decl '[' Expression ']' %prec ARR  { $$ = std::make_unique<ast::ArrayDecl>($1, $3);                 }
| Decl '[' error ']' %prec ARR       { $$ = nullptr;                                                  }
| '*' Decl %prec PTR                 { $$ = std::make_unique<ast::PointerDecl>($2);                   }
| '&' Decl %prec PTR                 { $$ = std::make_unique<ast::ReferenceDecl>($2);                 }
| '(' Decl ')'                       { $$ = std::move($2);                                            }
| '(' error ')'                      { $$ = nullptr;                                                  }
;

Decl:
  VarDecl   { $$ = std::move($1); }
| FuncDecl  { $$ = std::move($1); }
;

InitDecl:
  Decl                 { $$ = std::make_unique<ast::InitDecl>($1);     }
| Decl '=' Expression  { $$ = std::make_unique<ast::InitDecl>($1, $3); }
;

TypePrimitive:
  VOID       { $$ = std::make_unique<ast::BasicType>(ast::BasicType::VOID);   }
| BOOL       { $$ = std::make_unique<ast::BasicType>(ast::BasicType::BOOL);   }
| CHAR       { $$ = std::make_unique<ast::BasicType>(ast::BasicType::CHAR);   }
| SHORT      { $$ = std::make_unique<ast::BasicType>(ast::BasicType::SHORT);  }
| INT        { $$ = std::make_unique<ast::BasicType>(ast::BasicType::INT);    }
| LONG       { $$ = std::make_unique<ast::BasicType>(ast::BasicType::LONG);   }
| FLOAT      { $$ = std::make_unique<ast::BasicType>(ast::BasicType::FLOAT);  }
| DOUBLE     { $$ = std::make_unique<ast::BasicType>(ast::BasicType::DOUBLE); }
| STRUCT ID  { $$ = std::make_unique<ast::CustomType>($2);                    }
;

DeclList:
  InitDecl               { $$ = std::make_unique<ast::VarDeclaration>($1);                     }
| DeclList ',' InitDecl  { $$ = std::move($1); ast::cast<ast::VarDeclaration>($$)->AddVar($3); }
| DeclList ',' error     { $$ = std::move($1);                                                 }
;

OptParamList:
  /* empty */  { $$ = std::make_unique<ast::FuncDecl>(); }
| VOID         { $$ = std::make_unique<ast::FuncDecl>(); }
| ParamList    { $$ = std::move($1);                     }
;

ParamList:
  TypePrimitive Decl                { $$ = std::make_unique<ast::FuncDecl>($1, $2);                       }
| ParamList ',' TypePrimitive Decl  { $$ = std::move($1); ast::cast<ast::FuncDecl>($$)->AddParam($3, $4); }
| ParamList ',' error               { $$ = std::move($1);                                                 }
;

Declaration:
  TypePrimitive DeclList ';'                    { $$ = std::move($2); ast::cast<ast::VarDeclaration>($$)->SetType($1); }
| TypePrimitive FuncDecl '{' StatementList '}'  { $$ = std::make_unique<ast::FuncDeclaration>($1, $2, $4);             }
| TypePrimitive FuncDecl '{' error '}'          { $$ = nullptr;                                                        }
;

DeclarationList:
  Declaration                  { $$ = std::make_unique<ast::DeclarationList>($1);                      }
| DeclarationList Declaration  { $$ = std::move($1); ast::cast<ast::DeclarationList>($$)->AddDecl($2); }
| DeclarationList error        { $$ = std::move($1);                                                   }
;
