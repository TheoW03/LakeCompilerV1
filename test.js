let regAlloc = 0;

function genCode(node) {
    if (node.type == 'number') {
        let reg = allocReg();
        emit(`MOV ${reg}, ${node.value}`);
        return reg;
    }
    //when its a varaible free register
    // if (node.type == 'ident') {
    //     return  emit(`MOV ${reg}, ${node.value}`); //use A's value
    // }
    if (node.type == 'binop') {
        let leftReg = genCode(node.left);
        let rightReg = genCode(node.right);
        let resultReg = allocReg();
        switch (node.value) {
            case '+':
                emit(`ADD ${resultReg}, ${leftReg}, ${rightReg}`);
                break;
            case '-':
                emit(`SUB ${resultReg}, ${leftReg}, ${rightReg}`);
                break;
            case '*':
                emit(`MUL ${resultReg}, ${leftReg}, ${rightReg}`);
                break;
            case '/':
                emit(`DIV ${resultReg}, ${leftReg}, ${rightReg}`);
                break;
        }
        freeReg(leftReg);
        freeReg(rightReg);
        return resultReg;
    }
}

function allocReg() {
    return 'r' + regAlloc++;
}

function freeReg(reg) {
    regAlloc--;
}

function emit(code) {
    console.log(code);
}

let ast = {
    type: 'binop',
    value: '+',
    left: {
        type: 'ident',
        value: 'a'
    },
    right: {
        type: 'number',
        value: '1'
    }
};

genCode(ast);