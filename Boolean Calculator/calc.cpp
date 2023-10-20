//#include <iostream>
//// #include "formulaenode.h"
//#include "formulae.h"
//#include "errors.h"
//
//int main()
//{
//  /*  Formula F("sin(x)^2 + cos(x)^2 - y^3");
//
//    double result = F.calc();
//
//    std::cout << F.str() << " = " << result << std::endl;
//    */
//
///*
//    NumNode A(2), B(-3);
//    PlusNode C(&A, &B);
//
//    std::cout << C.str() << " = " << C.calc() << std::endl;
//*/
///*    FormulaNode* A, * B, * C;
//    
//    A = new ParamNode('a');
//    B = new ParamNode('b');
//    C = new PlusNode(A, B);
//    A = new NumNode(1);
//    C = new MinusNode(A, C);
//    B = new PowNode(new NumNode(3), new ParamNode('c'));
//    C = new MultNode(C, B);
//    std::cout << C->str() << std::endl;
//    double result = C->calc();
//    std::cout << C->str() << " = " << result << std::endl;
//
//    C = new UMinusNode(C);
//
//    std::cout << C->str() << std::endl;
//    result = C->calc();
//    std::cout << C->str() << " = " << result << std::endl;
//    delete C;
//    */
//    char buf[1024];
//    while (1) {
//        std::cout << ">> ";
//        std::cin.getline(buf, 1024);
//        if (strlen(buf) == 0) break;
//        if (strcmp(buf, "clear") == 0) {
//            global::Workspace.clear();
//            continue;
//        }
//        try {
//            Formula F(buf);
//            //std::cout << F.str() << std::endl;
//            double result = F.calc();
//            std::cout <<"  "<<  F.str() << " = " << result << std::endl;
//        }
//        catch (const char* error) {
//            std::cout << "Error: " << error << std::endl;
//        }
//        catch (const Error& e) {
//            std::cout << "Error: " << e.what() << std::endl;
//        }
//        catch (...) {
//            std::cout << "Unknown error" << std::endl;
//        }
//    }
//}
