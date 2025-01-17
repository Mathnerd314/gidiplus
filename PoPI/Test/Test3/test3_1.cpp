/*
# <<BEGIN-copyright>>
# Copyright 2019, Lawrence Livermore National Security, LLC.
# See the top-level COPYRIGHT file for details.
# 
# SPDX-License-Identifier: MIT
# <<END-copyright>>
*/

#include <iostream>

#include "PoPI.hpp"

/*
=========================================================
*/
int main( int argc, char **argv ) {

    std::cerr << "    " << __FILE__ << std::endl;

    std::string fileName( "../pops.xml" );

    try {
        PoPI::Database database( fileName );
        database.saveAs( "Outputs/pops3.xml.out" );
        database.print( ); }
    catch (char const *str) {
        std::cout << str << std::endl;
    }
}
