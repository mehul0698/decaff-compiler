#ifndef PRINT_VISIT
#define PRINT_VISIT

#include <iostream>
#include <vector>
#include "ast.h"
#include "Visitor.h"

class PrintVisitor : public Visitor{
public:
    void *visit(ASTProgram *node){
        std::cout << "\nProgram\n";
        if(node->getFdl() == NULL){
            // No field declarations found
        }
        else{
            std::vector<FieldDecl *> *z = node->getFdl();
            for(std::vector<FieldDecl *>::iterator it = z->begin() ; it != z->end(); it++){
                (*it)->accept(this);
            }               
        }
        std::cout << "\nField declarations\n";
        if(node->getMdl() == NULL){
            // No method declarations found
        }
        else{
            std::vector<MethodDecl *> *z = node->getMdl();
            for(std::vector<MethodDecl *>::iterator it = z->begin() ; it != z->end(); it++){
                (*it)->accept(this);
            }               
        }
        return NULL;
    }
    void *visit(FieldDecl *node){
        DataType type = node->getType();
        std::vector<VarIdentifier *> *var_id_list = node->getVar_id_list();
        std::vector<ArrIdentifier *> *arr_id_list = node->getArray_id_list();
        std::cout << node->parseDataType(type) << " ";

        if(var_id_list != NULL){
            for(std::vector<VarIdentifier *>::iterator it = var_id_list->begin() ; it != var_id_list->end(); it++){
                (*it)->accept(this);
            }   
        }

        if(arr_id_list != NULL){
            for(std::vector<ArrIdentifier *>::iterator it = arr_id_list->begin() ; it != arr_id_list->end(); it++){
                (*it)->accept(this);
            }
        }
        return NULL;
    }

    void *visit(VarIdentifier *node){
        std::string id = node->getID();
        std::cout << " " << id;
        return NULL;
    }

    void *visit(ArrIdentifier *node){
        int sz = node->getSize();
        std::string id = node->getID();
        std::cout << " " << id << "[" << sz << "]";
        return NULL;
    }

    void *visit(MethodDecl *node){
        DataType ret_type = node->getReturnType();
        std::string id    = node->getID();
        std::vector<TypeIdentifier *> *ag = node->getArguments();
        BlockStatement *block = node->getBlock();

        std::cout << node->parseDataType(ret_type) << " " << id <<" " << "(";
        if (ag != NULL) {
            for(std::vector<TypeIdentifier *>::iterator it = ag->begin() ; it != ag->end(); it++) {
                (*it)->accept(this);
            }
        }
        std::cout << ")\n";

        block->accept(this);
        return NULL;
    }
    void *visit(TypeIdentifier *node){
        DataType type = node->getType();
        std::string id = node->getID();
        std::cout << node->parseDataType(type) << " " << id << " ";
        return NULL;
    }

    void *visit(BlockStatement *node){
        std::vector<VarDecl *> *id_list = node->getID_list();
        std::vector<Statement *> *stmtlist = node->getStmtlist();

        std::cout << "{\n";
        if(stmtlist && id_list){
            //std::cout << stmtlist->size() + id_list->size() << "\n";
            for(std::vector<VarDecl *>::iterator it = id_list->begin() ; it != id_list->end(); it++) {
                (*it)->accept(this);
            }
            for(std::vector<Statement *>::iterator it = stmtlist->begin() ; it != stmtlist->end(); it++) {
                (*it)->accept(this);
            }
        }
        else if(stmtlist){
            std::cout << stmtlist->size() << "\n";
            for(std::vector<Statement *>::iterator it = stmtlist->begin() ; it != stmtlist->end(); it++) {
                (*it)->accept(this);
            }
        }
        else if(node->getID_list()){
            std::cout << id_list->size() << "\n";
            for(std::vector<VarDecl *>::iterator it = id_list->begin() ; it != id_list->end(); it++) {
                (*it)->accept(this);
            }
        }
        std::cout << "\n}\n";
        return NULL;
    }

    void *visit(AssignStatement *node){
        Location *loc = node->getLocation();
        AssignOp op = node->getOp();
        Expression *expr = node->getExpr();
        loc->accept(this);
        std::cout << " " << node->parseAssignOp(op) << " ";
        expr->accept(this);
        std::cout << "\n";
        return NULL;
    }

    void *visit(MethodCall *node){
        // Not required
        std::cout << "\n";
        return NULL;
    }

    void *visit(SmplMethod *node){
        std::string id = node->getID();
        std::vector<Expression *> *ag = node->getArguments();
        std::cout << id << "(";
        if(ag){
            for(std::vector<Expression *>::iterator it = ag->begin() ; it != ag->end(); it++){
              (*it)->accept(this);
            }
        }
        std::cout << ")";
        return NULL;
    }

    void *visit(CalloutMethod *node){
        std::string mname = node->getMethod_name();
        std::vector<CalloutArg *> *ag = node->getArguments();
        std::cout << mname << "(";
        if(ag){
            for(std::vector<CalloutArg *>::iterator it = ag->begin() ; it != ag->end(); it++){
              (*it)->accept(this);
            }
        }
        std::cout << ")\n";
        return NULL;
    }

    void *visit(CalloutArg *node){
        // Not required
        return NULL;
    }

    void *visit(StringCalloutArg *node){
        std::string ag = node->getArgument();
        std::cout << ag;
        return NULL;
    }

    void *visit(ExpressionCalloutArg *node){
        Expression *ag = node->getArgument();
        ag->accept(this);
        return NULL;
    }

    void *visit(IfStatement *node){
        Expression *cond = node->getCondition();
        BlockStatement *if_block = node->getIf_block();
        BlockStatement *else_block = node->getElse_block();

        std::cout << "if" << "(";
        cond->accept(this);
        std::cout<<")\n";
        if_block->accept(this);
        if(else_block != NULL)
            else_block->accept(this);
        return NULL;
    }

    void *visit(ForStatement * node){
        Expression *init = node->getInit_condition();
        Expression *end  = node->getEnd_condition();
        BlockStatement *block = node->getBlock();
        init->accept(this);
        end->accept(this);
        block->accept(this);
        return NULL;
    }

    void *visit(RetStatement *node){
        Expression *rt = node->getExpr();
        std::cout << "return ";
        rt->accept(this);
        return NULL;
    }

    void *visit(ConStatement *node){
        std::cout<<"continue\n";
        return NULL;
    }

    void *visit(BreakStatement *node){
        std::cout<<"break\n";
        return NULL;
    }

    void *visit(Location *node){
        // Some other classes derived from this
        std::cout<<"\n";
        return NULL;
    }

    void *visit(VarLocation *node){
        std::string id = node->getID();
        std::cout << id  << " ";
        return NULL;
    }

    void *visit(ArrayLocation *node){
        std::string id = node->getID();
        Expression *idx = node->getIndex();
        std::cout << id << " [" << idx << "]";
        return NULL;
    }

    void *visit(LitExpression *node){
        std::cout<<""<<std::endl;
        return NULL;
    }

    void *visit(IntLitExpression *node){
        int val = node->getValue();
        std::cout << " " << val;
        return NULL;
    }

    void *visit(CharacterLitExpression *node){
        char val = node->getValue();
        std::cout << " " << val;
        return NULL;
    }

    void *visit(TrueLitExpression *node){
        bool val = node->getValue();
        std::cout << " " << val;
        return NULL;
    }

    void *visit(FalseLitExpression *node){
        bool val = node->getValue();
        std::cout << " " << val;
        return NULL;
    }

    void *visit(BinOpExpression *node){
        Expression *l = node->getLeft();
        BinOp type = node->getOp();
        Expression *r = node->getRight();

        l->accept(this);
        std::cout << " " << node->parseBinOp(type) << " ";
        r->accept(this);
        return NULL;
    }

    void *visit(UnaryOpExpression *node){
        UnOp op = node->getOp();
        Expression *expr = node->getExpr();
        std::cout << node->parseUnOp(op) << " \n";
        expr->accept(this);
        return NULL;
    }

    void *visit(VarDecl *node){
        DataType type = node->getType();
        std::vector<VarIdentifier *> *id_list = node->getID_list();

        std::cout << node->parseDataType(type) << " ";
        for(std::vector<VarIdentifier *>::iterator it = id_list->begin(); it != id_list->end(); it++) {
              (*it)->accept(this);
            }
        std::cout << "\n";
        return NULL;
    }

    void *visit(Identifier *node){
        std::cout << "\n";
        return NULL;
    }

    void *visit(Statement *node){
        std::cout << "\n";
        return NULL;
    }
    void *visit(Expression *node){
        std::cout << "\n";
        return NULL;
    }
};
#endif
