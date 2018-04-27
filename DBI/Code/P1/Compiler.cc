#include "Compiler.h"
char *Compiler::outFile=NULL;
extern bool modifyData; 

void Compiler::Compile()
{
    
    if(chkParse()){
        if(modifyData){
            exec->executeDataQuery();
            return;
        }
        Optimize();
        exec->init();
       // if(!runOrPrint)
        //exec->printTree(root);
        //exec->levelOrderPrint(root);
         exec->executeQuery(root);
         exec->printNDel();
    }
    else{
        cout<<"Query is Wrong!!!";
    }
}

void Compiler::Optimize(){    
    if(qOps[Join]) 
        opt->findOrder();
    opt->makePlan();
    exec->setrootNPipe(root,opt->PipeNum);
}


bool Compiler::chkParse()
{
    TypesOfOps typeOp;
    //check if the code parses correctly
    if(par->parseAndCheck()){
        if(modifyData) return true;
        for(int x=0;x<=QOPSSIZE;x++)  qOps[x]=false;
                            
        if(tables->next!=NULL){
            typeOp = Join;
            qOps[typeOp]=true;
        }
        if(finalFunction!=NULL){
            if(groupingAtts!=NULL){
                typeOp = GroupBy;
                qOps[typeOp]=true;
            }
            else{
                typeOp = Sum;
                qOps[typeOp]=true;
            }
        }            
        if( qOps[GroupBy]==false && qOps[Sum]==false){
            typeOp = Project;
            qOps[typeOp]=true;
        }
        if(distinctAtts!=0 ||distinctFunc!=0){
            typeOp = Distinct;
            qOps[typeOp]=true;
        }            
        opt->SetQueryOps(qOps,QOPSSIZE,&root);
        return true;
    }
    return false;
}
