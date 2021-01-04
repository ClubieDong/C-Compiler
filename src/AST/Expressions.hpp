#pragma once

#include <sstream>
#include <variant>
#include <cassert>
#include "Base.hpp"

namespace ast
{
    class Expression : public Base
    {
    public:
        inline virtual const ErrorHandler::Location &GetLocation() const = 0;
        inline virtual opt<SymbolTable::Symbol> CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                                        llvm::IRBuilder<> &builder)
        {
            return std::nullopt;
        };

        inline static opt<SymbolTable::Symbol> Dereference(opt<SymbolTable::Symbol> sym, llvm::IRBuilder<> &builder)
        {
            if (!sym)
                return std::nullopt;
            if (!sym->IsRef)
                return sym;
            auto value = builder.CreateLoad(sym->Value, "");
            return SymbolTable::Symbol(value, false);
        }

        inline static opt<SymbolTable::Symbol> Arr2Ptr(opt<SymbolTable::Symbol> sym, llvm::IRBuilder<> &builder)
        {
            if (!sym)
                return std::nullopt;
            if (!sym->Value->getType()->isArrayTy())
                return sym;
            sym->Value = builder.CreateBitCast(sym->Value, sym->Value->getType()->getArrayElementType()->getPointerTo());
            return sym;
        }

        inline static opt<SymbolTable::Symbol> CastLLVMType(SymbolTable::Symbol sym, llvm::Type *toType, bool toIsRef,
                                                            const ErrorHandler::Location &loc, llvm::IRBuilder<> &builder)
        {
            if (toIsRef)
            {
                if (!sym.IsRef)
                {
                    ErrorHandler::PrintError("Requires an lvalue", loc);
                    return std::nullopt;
                }
                // refernece to reference
                if (sym.Value->getType() != toType)
                {
                    ErrorHandler::PrintError("References must have the same type", loc);
                    return std::nullopt;
                }
                return sym;
            }
            if (sym.IsRef)
                sym = *Dereference(sym, builder);
            if (toType->isPointerTy())
            {
                // pointer to pointer
                if (sym.Value->getType()->isPointerTy())
                {
                    if (sym.Value->getType()->getPointerElementType() != toType->getPointerElementType())
                    {
                        ErrorHandler::PrintWarning("Implicit pointer cast", loc);
                        sym.Value = builder.CreatePointerCast(sym.Value, toType);
                    }
                    return sym;
                }
                // array to pointer
                if (sym.Value->getType()->isArrayTy())
                {
                    if (sym.Value->getType()->getArrayElementType() != toType->getPointerElementType())
                    {
                        ErrorHandler::PrintError("Cannot cast array to pointer due to different element type", loc);
                        return std::nullopt;
                    }
                    return SymbolTable::Symbol(builder.CreateBitCast(sym.Value, toType), false);
                }
                ErrorHandler::PrintError("Cannot cast to pointer", loc);
                return std::nullopt;
            }
            if (toType->isArrayTy())
            {
                // pointer to array
                if (sym.Value->getType()->isPointerTy())
                {
                    if (sym.Value->getType()->getPointerElementType() != toType->getArrayElementType())
                    {
                        ErrorHandler::PrintError("Cannot cast pointer to array due to different element type", loc);
                        return std::nullopt;
                    }
                    return SymbolTable::Symbol(builder.CreateBitCast(sym.Value, toType), false);
                }
                // array to array
                if (sym.Value->getType()->isArrayTy())
                {
                    if (sym.Value->getType()->getArrayElementType() != toType->getArrayElementType())
                    {
                        ErrorHandler::PrintError("Cannot cast between arrays", loc);
                        return std::nullopt;
                    }
                    return sym;
                }
                ErrorHandler::PrintError("Cannot cast to array", loc);
                return std::nullopt;
            }
            if (toType->isIntegerTy())
            {
                // int to int
                if (sym.Value->getType()->isIntegerTy())
                {
                    if (sym.Value->getType()->getPrimitiveSizeInBits() != toType->getPrimitiveSizeInBits())
                    {
                        ErrorHandler::PrintWarning("Implicit integer cast", loc);
                        sym.Value = builder.CreateIntCast(sym.Value, toType, true);
                    }
                    return sym;
                }
                // float to int
                if (sym.Value->getType()->isFloatingPointTy())
                {
                    ErrorHandler::PrintWarning("Implicit cast from float point to integer", loc);
                    sym.Value = builder.CreateFPToSI(sym.Value, toType);
                    return sym;
                }
                ErrorHandler::PrintError("Cannot cast to integer", loc);
                return std::nullopt;
            }
            if (toType->isFloatingPointTy())
            {
                // int to float
                if (sym.Value->getType()->isIntegerTy())
                {
                    ErrorHandler::PrintWarning("Implicit cast from integer to float point", loc);
                    sym.Value = builder.CreateSIToFP(sym.Value, toType);
                    return sym;
                }
                // float to float
                if (sym.Value->getType()->isFloatingPointTy())
                {
                    if (sym.Value->getType()->getPrimitiveSizeInBits() != toType->getPrimitiveSizeInBits())
                    {
                        ErrorHandler::PrintWarning("Implicit float point cast", loc);
                        sym.Value = builder.CreateFPCast(sym.Value, toType);
                    }
                    return sym;
                }
                ErrorHandler::PrintError("Cannot cast to float point", loc);
                return std::nullopt;
            }
            // object to object
            if (toType == sym.Value->getType())
                return sym;
            ErrorHandler::PrintError("Cannot cast between objects", loc);
            return std::nullopt;
        }

        inline static void CastToCommonType(SymbolTable::Symbol &left, SymbolTable::Symbol &right, const ErrorHandler::Location &loc,
                                            llvm::IRBuilder<> &builder, llvm::LLVMContext &context)
        {
            if (left.IsRef || right.IsRef)
                return;
            if (!((left.Value->getType()->isFloatingPointTy() || left.Value->getType()->isIntegerTy()) &&
                  (right.Value->getType()->isFloatingPointTy() || right.Value->getType()->isIntegerTy())))
                return;
            if ((left.Value->getType()->isFloatingPointTy() && right.Value->getType()->isIntegerTy()) ||
                (left.Value->getType()->isIntegerTy() && right.Value->getType()->isFloatingPointTy()))
            {
                left = *CastLLVMType(left, llvm::Type::getDoubleTy(context), false, loc, builder);
                right = *CastLLVMType(right, llvm::Type::getDoubleTy(context), false, loc, builder);
                return;
            }
            auto size = std::max(left.Value->getType()->getPrimitiveSizeInBits(),
                                 right.Value->getType()->getPrimitiveSizeInBits());
            if (left.Value->getType()->isIntegerTy() && right.Value->getType()->isIntegerTy())
            {
                left = *CastLLVMType(left, llvm::Type::getIntNTy(context, size), false, loc, builder);
                right = *CastLLVMType(right, llvm::Type::getIntNTy(context, size), false, loc, builder);
                return;
            }
            if (left.Value->getType()->isFloatingPointTy() && right.Value->getType()->isFloatingPointTy())
            {
                llvm::Type *toType;
                switch (size)
                {
                case 32:
                    toType = llvm::Type::getFloatTy(context);
                    break;
                case 64:
                    toType = llvm::Type::getDoubleTy(context);
                    break;
                default:
                    assert(false);
                }
                left = *CastLLVMType(left, toType, false, loc, builder);
                right = *CastLLVMType(right, toType, false, loc, builder);
                return;
            }
            assert(false);
        }

        inline static opt<SymbolTable::Symbol> Assign(SymbolTable::Symbol lhs, SymbolTable::Symbol rhs,
                                                      const ErrorHandler::Location &loc, llvm::IRBuilder<> &builder)
        {
            if (!lhs.IsRef)
            {
                ErrorHandler::PrintError("Assigned expression must be an lvalue", loc);
                return std::nullopt;
            }
            auto leftType = lhs.Value->getType()->getPointerElementType();
            auto newRhs = CastLLVMType(rhs, leftType, false, loc, builder);
            if (!newRhs)
                return std::nullopt;
            builder.CreateStore(newRhs->Value, lhs.Value);
            return lhs;
        }
    };

    class Constant : public Expression
    {
    private:
        std::variant<bool, long long, double> _Value;
        ErrorHandler::Location _Location;

    public:
        inline explicit Constant(bool value, const ErrorHandler::Location &loc)
            : _Value(value), _Location(loc) {}
        inline explicit Constant(long long value, const ErrorHandler::Location &loc)
            : _Value(value), _Location(loc) {}
        inline explicit Constant(double value, const ErrorHandler::Location &loc)
            : _Value(value), _Location(loc) {}

        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Location; }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            if (auto pv = std::get_if<bool>(&_Value))
                os << hint << "Constant: " << (*pv ? "true" : "false") << '\n';
            if (auto pv = std::get_if<long long>(&_Value))
                os << hint << "Constant: " << *pv << '\n';
            else if (auto pv = std::get_if<double>(&_Value))
                os << hint << "Constant: " << *pv << '\n';
            else
                assert(false);
        }

        // virtual bool Analyze(SymbolTable *syms);

        inline virtual opt<SymbolTable::Symbol> CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                                        llvm::IRBuilder<> &builder) override
        {
            llvm::Value *value;
            if (auto pv = std::get_if<bool>(&_Value))
            {
                auto type = llvm::Type::getInt1Ty(context);
                value = llvm::ConstantInt::get(type, llvm::APInt(1, *pv, false));
            }
            else if (auto pv = std::get_if<long long>(&_Value))
            {
                auto type = llvm::Type::getInt64Ty(context);
                value = llvm::ConstantInt::get(type, llvm::APInt(64, *pv, true));
            }
            else if (auto pv = std::get_if<double>(&_Value))
            {
                auto type = llvm::Type::getDoubleTy(context);
                value = llvm::ConstantFP::get(type, llvm::APFloat(*pv));
            }
            else
                assert(false);
            return SymbolTable::Symbol(value, false);
        };
    };

    class Variable : public Expression
    {
    private:
        ptr<ID> _Name;

    public:
        inline explicit Variable(ptr<Base> &name) : _Name(to<ID>(name)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "Variable: \n";
            _Name->Show(os, hint + '\t');
        }

        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Name->GetLocation(); }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     auto res = syms->Search(_Name->GetName());
        //     if (!res)
        //     {
        //         std::ostringstream ss;
        //         ss << '\'' << _Name->GetName() << "' was not declared in this scope";
        //         ErrorHandler::PrintError(ss.str(), _Name->GetLocation());
        //         return false;
        //     }
        //     _ExprType = *res;
        //     return true;
        // }

        inline virtual opt<SymbolTable::Symbol> CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                                        llvm::IRBuilder<> &builder) override
        {
            const auto &name = _Name->GetName();
            auto value = syms.Search(name);
            if (!value)
            {
                std::ostringstream ss;
                ss << '\'' << name << "' was not declared in this scope";
                ErrorHandler::PrintError(ss.str(), GetLocation());
                return std::nullopt;
            }
            return SymbolTable::Symbol(value, true);
        };
    };

    class BiOpExpr : public Expression
    {
    public:
        enum BiOp
        {
            ASSIGN,

            LESS,
            GREATER,
            LESS_EQUAL,
            GREATER_EQUAL,
            EQUAL,
            NOT_EQUAL,

            ADD,
            SUB,
            MUL,
            DIV,
        };

    private:
        ptr<Expression> _Left, _Right;
        BiOp _Op;

        inline static bool IsAssign(BiOp op) { return ASSIGN <= op && op <= ASSIGN; }
        inline static bool IsRelOp(BiOp op) { return LESS <= op && op <= NOT_EQUAL; }
        inline static bool IsNumOp(BiOp op) { return ADD <= op && op <= DIV; }

    public:
        inline explicit BiOpExpr(ptr<Base> &left, ptr<Base> &right, BiOp op)
            : _Left(to<Expression>(left)), _Right(to<Expression>(right)), _Op(op) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            constexpr const char *ENUM_NAMES[] = {"ASSIGN", "LESS", "GREATER", "LESS_EQUAL", "GREATER_EQUAL",
                                                  "EQUAL", "NOT_EQUAL", "ADD", "SUB", "MUL", "DIV"};
            os << hint << "BiOpExpr: " << ENUM_NAMES[_Op] << '\n';
            os << hint << "\tLeft: \n";
            _Left->Show(os, hint + "\t\t");
            os << hint << "\tRight: \n";
            _Right->Show(os, hint + "\t\t");
        }

        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Left->GetLocation(); }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool l = _Left->Analyze(syms);
        //     bool r = _Right->Analyze(syms);
        //     return l && r;
        // }

        inline virtual opt<SymbolTable::Symbol> CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                                        llvm::IRBuilder<> &builder) override
        {
            opt<SymbolTable::Symbol> left, right;
            if (IsAssign(_Op))
            {
                // Right hand side should be evaluated first in assign operation
                right = _Right->CodeGen(syms, context, builder);
                left = _Left->CodeGen(syms, context, builder);
            }
            else
            {
                left = _Left->CodeGen(syms, context, builder);
                right = _Right->CodeGen(syms, context, builder);
            }
            if (!left || !right)
                return std::nullopt;
            if (IsAssign(_Op))
            {
                // TODO: Compound assignment, could do recursively
                // if (_Op != ASSIGN)
                //     right = left + right; // for example
                return Assign(*left, *right, _Left->GetLocation(), builder);
            }
            left = Dereference(left, builder);
            right = Dereference(right, builder);
            if (IsRelOp(_Op))
            {
                if ((left->Value->getType()->isArrayTy() || left->Value->getType()->isPointerTy()) &&
                    (right->Value->getType()->isArrayTy() || right->Value->getType()->isPointerTy()))
                {
                    left->Value = builder.CreateBitCast(left->Value, llvm::Type::getInt64Ty(context));
                    right->Value = builder.CreateBitCast(right->Value, llvm::Type::getInt64Ty(context));
                }
                else if ((left->Value->getType()->isFloatingPointTy() || left->Value->getType()->isIntegerTy()) &&
                         (right->Value->getType()->isFloatingPointTy() || right->Value->getType()->isIntegerTy()))
                    CastToCommonType(*left, *right, _Left->GetLocation(), builder, context);
                else
                {
                    ErrorHandler::PrintError("Operand type is invalid", _Left->GetLocation());
                    return std::nullopt;
                }
                switch (_Op)
                {
                case LESS:
                    return SymbolTable::Symbol(builder.CreateICmpULT(left->Value, right->Value), false);
                case GREATER:
                    return SymbolTable::Symbol(builder.CreateICmpUGT(left->Value, right->Value), false);
                case LESS_EQUAL:
                    return SymbolTable::Symbol(builder.CreateICmpULE(left->Value, right->Value), false);
                case GREATER_EQUAL:
                    return SymbolTable::Symbol(builder.CreateICmpUGE(left->Value, right->Value), false);
                case EQUAL:
                    return SymbolTable::Symbol(builder.CreateICmpEQ(left->Value, right->Value), false);
                case NOT_EQUAL:
                    return SymbolTable::Symbol(builder.CreateICmpNE(left->Value, right->Value), false);
                default:
                    assert(false);
                }
            }
            if (IsNumOp(_Op))
            {
                if (_Op == SUB &&
                    (left->Value->getType()->isArrayTy() || left->Value->getType()->isPointerTy()) &&
                    (right->Value->getType()->isArrayTy() || right->Value->getType()->isPointerTy()))
                {
                    auto newL = builder.CreateBitCast(left->Value, llvm::Type::getInt64Ty(context));
                    auto newR = builder.CreateBitCast(right->Value, llvm::Type::getInt64Ty(context));
                    return SymbolTable::Symbol(builder.CreateSub(newL, newR), false);
                }
                if ((_Op == ADD || _Op == SUB) &&
                    (((left->Value->getType()->isArrayTy() || left->Value->getType()->isPointerTy()) && right->Value->getType()->isIntegerTy()) ||
                     ((right->Value->getType()->isArrayTy() || right->Value->getType()->isPointerTy()) && left->Value->getType()->isIntegerTy())))
                {
                    auto newL = builder.CreateBitCast(left->Value, llvm::Type::getInt64Ty(context));
                    auto newR = builder.CreateBitCast(right->Value, llvm::Type::getInt64Ty(context));
                    llvm::Type *type;
                    if (left->Value->getType()->isArrayTy() || left->Value->getType()->isPointerTy())
                        type = left->Value->getType();
                    else if (right->Value->getType()->isArrayTy() || right->Value->getType()->isPointerTy())
                        type = right->Value->getType();
                    else
                        assert(false);
                    // TODO: size of pointer element
                    llvm::Value *value;
                    if (_Op == ADD)
                        value = builder.CreateAdd(newL, newR);
                    else if (_Op == SUB)
                        value = builder.CreateSub(newL, newR);
                    else
                        assert(false);
                    return SymbolTable::Symbol(builder.CreateBitCast(value, type), false);
                }
                else if ((left->Value->getType()->isFloatingPointTy() || left->Value->getType()->isIntegerTy()) &&
                         (right->Value->getType()->isFloatingPointTy() || right->Value->getType()->isIntegerTy()))
                    CastToCommonType(*left, *right, _Left->GetLocation(), builder, context);
                else
                {
                    ErrorHandler::PrintError("Operand type is invalid", _Left->GetLocation());
                    return std::nullopt;
                }
                switch (_Op)
                {
                case ADD:
                    return SymbolTable::Symbol(builder.CreateAdd(left->Value, right->Value), false);
                case SUB:
                    return SymbolTable::Symbol(builder.CreateSub(left->Value, right->Value), false);
                case MUL:
                    return SymbolTable::Symbol(builder.CreateMul(left->Value, right->Value), false);
                case DIV:
                    return SymbolTable::Symbol(builder.CreateSDiv(left->Value, right->Value), false);
                default:
                    assert(false);
                }
            }
            assert(false);
        };
    };

    class UnOpExpr : public Expression
    {
    public:
        enum UnOp
        {
            POS,
            NEG,
            DEREF,
            ADDR
        };

    private:
        ptr<Expression> _Operand;
        UnOp _Op;

    public:
        inline explicit UnOpExpr(ptr<Base> &operand, UnOp op)
            : _Operand(to<Expression>(operand)), _Op(op) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            constexpr const char *ENUM_NAMES[] = {"NEG", "POS", "DEREF", "ADDR"};
            os << hint << "UnOpExpr: " << ENUM_NAMES[_Op] << '\n';
            _Operand->Show(os, hint + '\t');
        }

        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Operand->GetLocation(); }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     return _Operand->Analyze(syms);
        // }

        inline virtual opt<SymbolTable::Symbol> CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                                        llvm::IRBuilder<> &builder) override
        {
            auto value = _Operand->CodeGen(syms, context, builder);
            if (_Op == POS)
            {
                value = Dereference(value, builder);
                if (!value->Value->getType()->isIntegerTy() && !value->Value->getType()->isFloatingPointTy())
                {
                    ErrorHandler::PrintError("Invalid positive operator", _Operand->GetLocation());
                    return std::nullopt;
                }
                return value;
            }
            if (_Op == NEG)
            {
                value = Dereference(value, builder);
                if (!value->Value->getType()->isIntegerTy() && !value->Value->getType()->isFloatingPointTy())
                {
                    ErrorHandler::PrintError("Invalid negative operator", _Operand->GetLocation());
                    return std::nullopt;
                }
                if (value->Value->getType()->isIntegerTy())
                    return SymbolTable::Symbol(builder.CreateNeg(value->Value), false);
                if (value->Value->getType()->isFloatingPointTy())
                    return SymbolTable::Symbol(builder.CreateFNeg(value->Value), false);
                assert(false);
            }
            if (_Op == DEREF)
            {
                value = Dereference(value, builder);
                if (!value->Value->getType()->isArrayTy() && !value->Value->getType()->isPointerTy())
                {
                    ErrorHandler::PrintError("Invalid dereference operator", _Operand->GetLocation());
                    return std::nullopt;
                }
                if (value->Value->getType()->isArrayTy())
                {
                    auto eleTy = value->Value->getType()->getArrayElementType();
                    auto v = builder.CreateBitCast(value->Value, llvm::PointerType::get(eleTy, 0));
                    return SymbolTable::Symbol(v, true);
                }
                if (value->Value->getType()->isPointerTy())
                    return SymbolTable::Symbol(value->Value, true);
                assert(false);
            }
            if (_Op == ADDR)
            {
                if (!value->IsRef)
                {
                    ErrorHandler::PrintError("Expression must be an lvalue", _Operand->GetLocation());
                    return std::nullopt;
                }
                return SymbolTable::Symbol(value->Value, false);
            }
            return std::nullopt;
        };
    };

    class CallExpr : public Expression
    {
    private:
        ptr<Expression> _Func;
        arr<ptr<Expression>> _ArgList;

    public:
        inline explicit CallExpr() = default;
        inline explicit CallExpr(ptr<Base> &arg) { _ArgList.push_back(to<Expression>(arg)); }

        inline void AddArg(ptr<Base> &arg) { _ArgList.push_back(to<Expression>(arg)); }
        inline void SetFunc(ptr<Base> &func) { _Func = to<Expression>(func); }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "CallExpr: \n";
            os << hint << "\tFunction: \n";
            _Func->Show(os, hint + "\t\t");
            if (_ArgList.empty())
                os << hint << "\tNo Arguments\n";
            else
                for (const auto &arg : _ArgList)
                {
                    os << hint << "\tArgument: \n";
                    arg->Show(os, hint + "\t\t");
                }
        }

        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Func->GetLocation(); }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool success = true;
        //     if (!_Func->Analyze(syms))
        //         success = false;
        //     for (auto& i : _ArgList)
        //         if (!i->Analyze(syms))
        //             success = false;
        //     return success;
        // }

        inline virtual opt<SymbolTable::Symbol> CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                                        llvm::IRBuilder<> &builder) override
        {
            auto funcExpr = _Func->CodeGen(syms, context, builder);
            auto func = llvm::dyn_cast<llvm::Function>(funcExpr->Value);
            if (!func)
            {
                ErrorHandler::PrintError("Only function is allowed to be called", _Func->GetLocation());
                return std::nullopt;
            }
            if (func->arg_size() != _ArgList.size())
            {
                ErrorHandler::PrintError("The number of arguments does not match", _Func->GetLocation());
                return std::nullopt;
            }
            std::vector<llvm::Value *> args;
            for (unsigned int i = 0; i < _ArgList.size(); ++i)
            {
                auto arg = _ArgList[i]->CodeGen(syms, context, builder);
                if (!arg)
                    return std::nullopt;
                arg = CastLLVMType(*arg, func->getArg(i)->getType(), false, _ArgList[i]->GetLocation(), builder);
                if (!arg)
                    return std::nullopt;
                args.push_back(arg->Value);
            }
            return SymbolTable::Symbol(builder.CreateCall(func, args), false);
        };
    };

    class IndexExpr : public Expression
    {
    private:
        ptr<Expression> _Expr, _Index;

    public:
        inline explicit IndexExpr(ptr<Base> &expr, ptr<Base> &index)
            : _Expr(to<Expression>(expr)), _Index(to<Expression>(index)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "IndexExpr: " << '\n';
            os << hint << "\tExpr: \n";
            _Expr->Show(os, hint + "\t\t");
            os << hint << "\tIndex: \n";
            _Index->Show(os, hint + "\t\t");
        }

        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Expr->GetLocation(); }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool l = _Left->Analyze(syms);
        //     bool r = _Right->Analyze(syms);
        //     return l && r;
        // }

        inline virtual opt<SymbolTable::Symbol> CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                                        llvm::IRBuilder<> &builder) override
        {
            opt<SymbolTable::Symbol> expr, index;
            expr = _Expr->CodeGen(syms, context, builder);
            index = _Index->CodeGen(syms, context, builder);
            if (!expr || !index)
                return std::nullopt;
            expr = Dereference(expr, builder);
            index = Dereference(index, builder);
            if (!expr->Value->getType()->isArrayTy() && !expr->Value->getType()->isPointerTy())
            {
                ErrorHandler::PrintError("Operand must be array or pointer", _Expr->GetLocation());
                return std::nullopt;
            }
            if (!index->Value->getType()->isIntegerTy())
            {
                ErrorHandler::PrintError("Operand must be integer", _Index->GetLocation());
                return std::nullopt;
            }
            // expr = Arr2Ptr(expr, builder);
            auto value = builder.CreateGEP(expr->Value->getType(), expr->Value, std::vector<llvm::Value *>{index->Value});
            if (value->getType()->isArrayTy())
            {
                auto eleTy = value->getType()->getArrayElementType();
                auto v = builder.CreateBitCast(value, llvm::PointerType::get(eleTy, 0));
                return SymbolTable::Symbol(v, true);
            }
            if (value->getType()->isPointerTy())
                return SymbolTable::Symbol(value, true);
            assert(false);
        };
    };

} // namespace ast
