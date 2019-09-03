//
//  Teste.hpp
//  Arvores
//
//  Created by Marcos Amorim on 16/10/2018.
//  Copyright © 2018 Marcos Amorim. All rights reserved.
//

#ifndef Teste_hpp
#define Teste_hpp

#include <stdio.h>
#include <string>

class Teste {
    std::string greeting;
public:
    Teste();
    template<int x> int TesteTemplate();
    std::string greet();
};


#endif /* Teste_hpp */



