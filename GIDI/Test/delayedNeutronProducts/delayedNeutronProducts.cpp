/*
# <<BEGIN-copyright>>
# Copyright 2019, Lawrence Livermore National Security, LLC.
# See the top-level COPYRIGHT file for details.
# 
# SPDX-License-Identifier: MIT
# <<END-copyright>>
*/

#include <stdlib.h>
#include <iostream>

#include "GIDI_testUtilities.hpp"

static char const *description = "Prints out delayed neutron product data.";

void main2( int argc, char **argv );
/*
=========================================================
*/
int main( int argc, char **argv ) {

    try {
        main2( argc, argv ); }
     catch (std::exception &exception) {
        std::cerr << exception.what( ) << std::endl;
        exit( EXIT_FAILURE ); }
    catch (char const *str) {
        std::cerr << str << std::endl;
        exit( EXIT_FAILURE ); }
    catch (std::string &str) {
        std::cerr << str << std::endl;
        exit( EXIT_FAILURE );
    }

    exit( EXIT_SUCCESS );
}
/*
=========================================================
*/
void main2( int argc, char **argv ) {

    argvOptions argv_options( __FILE__, description );
    ParseTestOptions parseTestOptions( argv_options, argc, argv );

    parseTestOptions.m_askGNDS_File = true;

    parseTestOptions.parse( );

    GIDI::Construction::PhotoMode photo_mode = parseTestOptions.photonMode( GIDI::Construction::PhotoMode::nuclearAndAtomic );
    GIDI::Construction::Settings construction( GIDI::Construction::ParseMode::all, photo_mode );
    PoPI::Database pops;
    GIDI::Protare *protare = parseTestOptions.protare( pops, "../pops.xml", "../all.map", construction, PoPI::IDs::neutron, "O16" );

    std::cout << stripDirectoryBase( protare->fileName( ), "/GIDI/Test/" ) << std::endl;

    GIDI::Styles::TemperatureInfos temperatures = protare->temperatures( );
    GIDI::Transporting::MG settings( protare->projectile( ).ID( ), GIDI::Transporting::Mode::multiGroup, GIDI::Transporting::DelayedNeutrons::on );
    GIDI::Transporting::Particles particles;

    GIDI::DelayedNeutronProducts delayedNeutronProducts = protare->delayedNeutronProducts( );
    std::cout << "Number of delayed neutrons = " << delayedNeutronProducts.size( ) << std::endl;

    for( std::size_t i1 = 0; i1 < delayedNeutronProducts.size( ); ++i1 ) {
        GIDI::DelayedNeutronProduct &delayedNeutronProduct = delayedNeutronProducts[i1];
        GIDI::Product const *product = delayedNeutronProduct.product( );

        std::cout << "    " << delayedNeutronProduct.delayedNeutronIndex( ) << ": rate = " << delayedNeutronProduct.rate( ).value( ) 
                << " " << delayedNeutronProduct.rate( ).unit( ) << std::endl;

        GIDI::Vector multiplicity = product->multiGroupMultiplicity( settings, temperatures[0], PoPI::IDs::neutron );
        printVector( "        Multiplicity:: ", multiplicity );

        int maxOrder = product->maximumLegendreOrder( settings, temperatures[0], PoPI::IDs::neutron );
        GIDI::Matrix matrix = product->multiGroupProductMatrix( settings, temperatures[0], particles, PoPI::IDs::neutron, 0 );
        printMatrix( "    Product Matrix", maxOrder, matrix, "        " );
    }

    delete protare;
}
