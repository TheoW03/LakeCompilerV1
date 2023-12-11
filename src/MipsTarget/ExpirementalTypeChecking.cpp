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
        CastNode *a = dynamic_cast<CastNode *>(op.get());

        type_of c = traverse_tree_integer(move(a->expression));
        if (a->type.id == type::INT)
        {
            if (c.type_value == type::FLOAT)
            {
                type_of d;
                d.value = c.value / OFFSET;
                d.type_value = type::INT;
                return d;
            }
            return c;
        }
        else if (a->type.id == type::FLOAT)
        {
            if (c.type_value == type::INT)
            {
                type_of d;
                d.value = c.value * OFFSET;
                d.type_value = type::FLOAT;
                return d;
            }
            return c;
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

        type_of right = traverse_tree_integer(move(op->right));
        type_of left = traverse_tree_integer(move(op->left));
        int a = 0;
        int b = 0;

  	if (left.type_value == type::FLOAT || right.type_value == type::FLOAT)
  	{

      		if (left.type_value == type::INT)
      		{
          		a = left.value * OFFSET;
      		}
      		else
      		{
          		a = left.value;
      		}

      		if (right.type_value == type::INT)
      		{
          		b = right.value * OFFSET;
      		}
      		else
      		{
         		 b = right.value;
      		}
      		type_of k;

      		k.value = (a + b);
      		k.type_value = type::FLOAT;
      		return k;
	}
        a = right.value;
        b = left.value;
        type_of k;
        k.value = (a + b);
        k.type_value = type::INT;
        return k;
    }
}


