#include <iostream>
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"

#include "../../src/MipsTarget/VaraibleScope.h"
#include "../../src/MipsTarget/UtilFunctions.h"


using namespace std;


struct type_of
{
    int value;
    type type_value;
};

int tree_traverse_float(unique_ptr<Node> op);
int traverse_tree_integer(unique_ptr<Node> op)
{
    if (op == nullptr)
    {
        type_of a;
        a.value = 0;
        a.type_value = type::INT;
        return 0;
    }
    if (instanceof <CastNode *>(op.get()))
    {
	  
        CastNode *c = dynamic_cast<CastNode *>(op.get());
	if(c->type.id == type::INT){	
		int a = tree_traverse_float(move(c->expression));
		return a / OFFSET;
	}

    }
    if (instanceof <IntegerNode *>(op.get()))
    {
        IntegerNode *a = dynamic_cast<IntegerNode *>(op.get());
        type_of b;
        b.value = stoi(a->num);
        return b.value;
    }
    if(instanceof<FloatNode*>(op.get())){

    	cout << "type mismatch" << endl;
	exit(1);
    }
    
    if (instanceof <OperatorNode *>(op.get()))
    {

        int right = traverse_tree_integer(move(op->right));
        int left = traverse_tree_integer(move(op->left));

        return right + left;
    }
}



int tree_traverse_float(unique_ptr<Node> op)
{
	if (op == nullptr)
    	{
        	type_of a;
        	a.value = 0;
       	 	a.type_value = type::FLOAT;
        	return a.value;
    	}
 	if (instanceof <CastNode *>(op.get()))
    	{
	  
        	CastNode *c = dynamic_cast<CastNode *>(op.get());
		if(c->type.id == type::INT){	
			int a = tree_traverse_float(move(c->expression));
			int b = a/ OFFSET;
			return b * OFFSET;
		}
    	}

	 if (instanceof <IntegerNode *>(op.get()))
    	{
        	IntegerNode *a = dynamic_cast<IntegerNode *>(op.get());
        	type_of b;
        	b.value = stoi(a->num) * OFFSET;
        	b.type_value = type::FLOAT;
        	return b.value;
    	}
    	if (instanceof <FloatNode *>(op.get()))
    	{
        	FloatNode *a = dynamic_cast<FloatNode *>(op.get());
        	type_of b;
        	b.value = stoi(a->num);
        	b.type_value = type::FLOAT;
        	return b.value;
    	}
 	if (instanceof <OperatorNode *>(op.get()))
   	{

        	int right =  tree_traverse_float(move(op->right));
        	int left = tree_traverse_float(move(op->left));	
        	return right + left;
    }
	

}	
