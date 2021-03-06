%baseclass-preinclude includes.hpp
%default-actions off
%no-lines

%stype std::unique_ptr<ast::Base>

%token VOID BOOL CHAR SHORT INT LONG FLOAT DOUBLE
%token TRUE FALSE RETURN IF ELSE WHILE STRUCT FOR INC DEC
%token ID_TEXT
%token CONSTCHAR CONSTINT CONSTINT_BIN CONSTINT_OCT CONSTINT_HEX CONSTFP

%right '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN SHL_ASSIGN SHR_ASSIGN AND_ASSIGN OR_ASSIGN XOR_ASSIGN
%left '|'
%left '^'
%left '&'
%left EQ NE
%left '<' '>' LE GE
%left SHL SHR
%left '+' '-'
%left '*' '/' '%'

%%

Program:
  DeclarationList  { _Root = std::move($1); }
;

ID:
  ID_TEXT  { $$ = std::make_unique<ast::ID>(_Scanner->matched(), _Scanner->GetLocation()); }
;

PrimaryExpression:
  TRUE                { $$ = std::make_unique<ast::Constant>(true, _Scanner->GetLocation());                                        }
| FALSE               { $$ = std::make_unique<ast::Constant>(false, _Scanner->GetLocation());                                       }
| CONSTCHAR           { $$ = std::make_unique<ast::Constant>(_Scanner->matched()[1], _Scanner->GetLocation());                      }
| CONSTINT            { $$ = std::make_unique<ast::Constant>(std::stoi(_Scanner->matched()), _Scanner->GetLocation());              }
| CONSTINT_BIN        { $$ = std::make_unique<ast::Constant>(std::stoi(_Scanner->matched(), nullptr, 2), _Scanner->GetLocation());  }
| CONSTINT_OCT        { $$ = std::make_unique<ast::Constant>(std::stoi(_Scanner->matched(), nullptr, 8), _Scanner->GetLocation());  }
| CONSTINT_HEX        { $$ = std::make_unique<ast::Constant>(std::stoi(_Scanner->matched(), nullptr, 16), _Scanner->GetLocation()); }
| CONSTFP             { $$ = std::make_unique<ast::Constant>(std::stod(_Scanner->matched()), _Scanner->GetLocation());              }
| ID                  { $$ = std::make_unique<ast::Variable>($1);                                                                   }
| '(' Expression ')'  { $$ = std::move($2);                                                                                         }
| '(' error ')'       { $$ = nullptr;                                                                                               }
;

PostExpression:
  PrimaryExpression                  { $$ = std::move($1);                                                }
| PostExpression '(' OptArgList ')'  { $$ = std::move($3); ast::cast<ast::CallExpr>($$)->SetFunc($1);     }
| PostExpression '[' Expression ']'  { $$ = std::make_unique<ast::IndexExpr>($1, $3);                     }
| PostExpression INC                 { $$ = std::make_unique<ast::UnOpExpr>($1, ast::UnOpExpr::INC_POST); }
| PostExpression DEC                 { $$ = std::make_unique<ast::UnOpExpr>($1, ast::UnOpExpr::DEC_POST); }
| PostExpression '(' error ')'       { $$ = nullptr;                                                      }
;

PreExpression:
  PostExpression     { $$ = std::move($1);                                               }
| '+' PreExpression  { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::POS);     }
| '-' PreExpression  { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::NEG);     }
| '*' PreExpression  { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::DEREF);   }
| '&' PreExpression  { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::ADDR);    }
| '!' PreExpression  { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::NOT);     }
| '~' PreExpression  { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::NOT_BIT); }
| INC PreExpression  { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::INC_PRE); }
| DEC PreExpression  { $$ = std::make_unique<ast::UnOpExpr>($2, ast::UnOpExpr::DEC_PRE); }
;

Expression:
  PreExpression                     { $$ = std::move($1);                                                         }
| Expression '=' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::ASSIGN);        }
| Expression ADD_ASSIGN Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::ADD_ASSIGN);    }
| Expression SUB_ASSIGN Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::SUB_ASSIGN);    }
| Expression MUL_ASSIGN Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::MUL_ASSIGN);    }
| Expression DIV_ASSIGN Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::DIV_ASSIGN);    }
| Expression MOD_ASSIGN Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::MOD_ASSIGN);    }
| Expression SHL_ASSIGN Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::SHL_ASSIGN);    }
| Expression SHR_ASSIGN Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::SHR_ASSIGN);    }
| Expression AND_ASSIGN Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::AND_ASSIGN);    }
| Expression OR_ASSIGN Expression   { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::OR_ASSIGN);     }
| Expression XOR_ASSIGN Expression  { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::XOR_ASSIGN);    }
| Expression '|' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::OR);            }
| Expression '^' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::XOR);           }
| Expression '&' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::AND);           }
| Expression EQ Expression          { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::EQUAL);         }
| Expression NE Expression          { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::NOT_EQUAL);     }
| Expression '<' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::LESS);          }
| Expression '>' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::GREATER);       }
| Expression LE Expression          { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::LESS_EQUAL);    }
| Expression GE Expression          { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::GREATER_EQUAL); }
| Expression SHL Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::SHL);           }
| Expression SHR Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::SHR);           }
| Expression '+' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::ADD);           }
| Expression '-' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::SUB);           }
| Expression '*' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::MUL);           }
| Expression '/' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::DIV);           }
| Expression '%' Expression         { $$ = std::make_unique<ast::BiOpExpr>($1, $3, ast::BiOpExpr::MOD);           }
;

Statement:
  ';'                                                       { $$ = std::make_unique<ast::ExprStmt>();                              }
| error ';'                                                 { $$ = nullptr;                                                        }
| Expression ';'                                            { $$ = std::make_unique<ast::ExprStmt>($1);                            }
| Declaration                                               { $$ = std::move($1);                                                  }
| IF '(' Expression ')' Statement                           { $$ = std::make_unique<ast::IfStmt>($3, $5);                          }
| IF '(' Expression ')' Statement ELSE Statement            { $$ = std::make_unique<ast::IfStmt>($3, $5, $7);                      }
| WHILE '(' Expression ')' Statement                        { $$ = std::make_unique<ast::WhileStmt>($3, $5);                       }
| FOR '(' Statement Expression ';'Expression ')' Statement  { $$ = std::make_unique<ast::ForStmt>($3, $4, $6, $8);                 }
| RETURN ';'                                                { $$ = std::make_unique<ast::ReturnStmt>(_Scanner->GetLocation());     }
| RETURN Expression ';'                                     { $$ = std::make_unique<ast::ReturnStmt>($2, _Scanner->GetLocation()); }
| '{' StatementList '}'                                     { $$ = std::move($2);                                                  }
| '{' error '}'                                             { $$ = nullptr;                                                        }
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

PrimaryVarDecl:
  ID               { $$ = std::make_unique<ast::VarDecl>($1);       }
| '(' VarDecl ')'  { $$ = std::move($2);                            }
| '(' error ')'    { $$ = nullptr;                                  }
;

PostVarDecl:
  PrimaryVarDecl                  { $$ = std::move($1);                            }
| PostVarDecl '[' ']'             { $$ = std::make_unique<ast::ArrayDecl>($1);     }
| PostVarDecl '[' Expression ']'  { $$ = std::make_unique<ast::ArrayDecl>($1, $3); }
| PostVarDecl '[' error ']'       { $$ = nullptr;                                  }
;

VarDecl:
  PostVarDecl  { $$ = std::move($1);                            }
| '*' VarDecl  { $$ = std::make_unique<ast::PointerDecl>($2);   }
| '&' VarDecl  { $$ = std::make_unique<ast::ReferenceDecl>($2); }
;

PrimaryFuncDecl:
  ID '(' OptParamList ')'  { $$ = std::move($3); ast::cast<ast::FuncDecl>($$)->SetName($1); }
| '(' FuncDecl ')'         { $$ = std::move($2);                                            }
| '(' error ')'            { $$ = nullptr;                                                  }
;

PostFuncDecl:
  PrimaryFuncDecl                  { $$ = std::move($1);                            }
| PostFuncDecl '[' ']'             { $$ = std::make_unique<ast::ArrayDecl>($1);     }
| PostFuncDecl '[' Expression ']'  { $$ = std::make_unique<ast::ArrayDecl>($1, $3); }
| PostFuncDecl '[' error ']'       { $$ = nullptr;                                  }
;

FuncDecl:
  PostFuncDecl  { $$ = std::move($1);                            }
| '*' FuncDecl  { $$ = std::make_unique<ast::PointerDecl>($2);   }
| '&' FuncDecl  { $$ = std::make_unique<ast::ReferenceDecl>($2); }
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
