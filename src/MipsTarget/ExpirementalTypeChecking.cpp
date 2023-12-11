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

type_of tree_traverse_float(unique_ptr<Node> op);
type_of traverse_tree_integer(unique_ptr<Node> op)
{
    if (op == nullptr)
    {
        type_of a;
        a.value = 0;
        a.type_value = type::INT;
        return a;
    }
    if (instanceof <CastNode *>(op.get()))
    {
	  
        CastNode *c = dynamic_cast<CastNode *>(op.get());
	if(c->type.id == type::INT)
	{	
		type_of a = tree_traverse_float(move(c->expression));
		a.value /= OFFSET;
		a.type_value = type::INT;
		return a;
	}

    }
    if (instanceof <IntegerNode *>(op.get()))
    {
        IntegerNode *a = dynamic_cast<IntegerNode *>(op.get());
        type_of b;
        b.value = stoi(a->num);
        b.type_value = type::INT;
        return b;
    }
    if(instanceof<FloatNode*>(op.get()))
    {

    	cout << "type mismatch" << endl;
	exit(1);
    }
    
    if (instanceof <OperatorNode *>(op.get()))
    {

        type_of right = traverse_tree_integer(move(op->right));
        type_of left = traverse_tree_integer(move(op->left));
        int a = 0;
        int b = 0; 
        a = right.value;
        b = left.value;
        type_of k;
        k.value = (a + b);
        k.type_value = type::INT;
        return k;
    }
}



type_of tree_traverse_float(unique_ptr<Node> op)
{
	if (op == nullptr)
    	{
        	type_of a;
        	a.value = 0;
       	 	a.type_value = type::FLOAT;
        	return a;
    	}
	 if (instanceof <IntegerNode *>(op.get()))
    	{
        	IntegerNode *a = dynamic_cast<IntegerNode *>(op.get());
        	type_of b;
        	b.value = stoi(a->num) * OFFSET;
        	b.type_value = type::FLOAT;
        	return b;
    	}
    	if (instanceof <FloatNode *>(op.get()))
    	{
        	FloatNode *a = dynamic_cast<FloatNode *>(op.get());
        	type_of b;
        	b.value = stoi(a->num);
        	b.type_value = type::FLOAT;
        	return b;
    	}
 	if (instanceof <OperatorNode *>(op.get()))
   	{

        	type_of right =  tree_traverse_float(move(op->right));
        	type_of left = tree_traverse_float(move(op->left));
        	int a = 0;
        	int b = 0; 
        	a = right.value;
        	b = left.value;
        	type_of k;
        	k.value = (a + b);
        	k.type_value = type::FLOAT;
        	return k;
    }
	

}	
