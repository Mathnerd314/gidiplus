/*
# <<BEGIN-copyright>>
# Copyright 2019, Lawrence Livermore National Security, LLC.
# See the top-level COPYRIGHT file for details.
# 
# SPDX-License-Identifier: MIT
# <<END-copyright>>
*/

#include "PoPI.hpp"

namespace PoPI {

#define PoPI_chemicalElementChars "chemicalElement"
#define PoPI_isotopesChars "isotopes"
#define PoPI_Z_Chars "Z"

/*
=========================================================
*/
ChemicalElement::ChemicalElement( HAPI::Node const &a_node, Database *a_DB, Database *a_parent ) :
        SymbolBase( a_node, Particle_class::chemicalElement ),
        m_Z( a_node.attribute( PoPI_Z_Chars ).as_int( ) ),
        m_name( a_node.attribute( PoPI_nameChars ).value( ) ),
        m_isotopes( PoPI_isotopesChars ) {

    addToSymbols( a_DB );
    m_isotopes.appendFromParentNode( a_node.child( PoPI_isotopesChars ), a_DB, this );
}
/*
=========================================================
*/
ChemicalElement::~ChemicalElement( ) {

}
/*
=========================================================
*/
void ChemicalElement::calculateNuclideGammaBranchStateInfos( PoPI::Database const &a_pops, NuclideGammaBranchStateInfos &a_nuclideGammaBranchStateInfos ) const {

    for( std::size_t i1 = 0; i1 <  m_isotopes.size( ); ++i1 ) {
        Isotope const &isotope = m_isotopes[i1];

        isotope.calculateNuclideGammaBranchStateInfos( a_pops, a_nuclideGammaBranchStateInfos );
    }
}
/*
=========================================================
*/
void ChemicalElement::toXMLList( std::vector<std::string> &a_XMLList, std::string const &a_indent1 ) const {

    std::string::size_type size = m_isotopes.size( );
    char ZStr[8];
    sprintf( ZStr, "%d", m_Z );

    if( size == 0 ) return;

    std::string header = a_indent1 + "<chemicalElement symbol=\"" + symbol( ) + "\" Z=\"" + ZStr + "\" name=\"" + m_name + "\">";
    a_XMLList.push_back( header );

    std::string indent2 = a_indent1 + "  ";
    std::string isotopeSuite = indent2 + "<" + PoPI_isotopesChars + ">";
    a_XMLList.push_back( isotopeSuite );

    std::string indent3 = indent2 + "  ";
    for( std::string::size_type i1 = 0; i1 < size; ++i1 ) m_isotopes[i1].toXMLList( a_XMLList, indent3 );

    appendXMLEnd( a_XMLList, PoPI_isotopesChars );
    appendXMLEnd( a_XMLList, PoPI_chemicalElementChars );
}   

}
