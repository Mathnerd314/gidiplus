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

/*
=========================================================
*/
PQ_suite::PQ_suite( HAPI::Node const &a_node ) :
        m_label( a_node.name( ) ) {

    for( HAPI::Node child = a_node.first_child( ); !child.empty( ); child = child.next_sibling( ) ) {
        std::string name( child.name( ) );
        PhysicalQuantity *quantity;

        if( name == PoPI_doubleChars ) {
            quantity = new PQ_double( child ); }
        else if( name == PoPI_integerChars ) {
            quantity = new PQ_integer( child ); }
        else if( name == PoPI_fractionChars ) {
            quantity = new PQ_fraction( child ); }
        else if( name == PoPI_stringChars ) {
            quantity = new PQ_string( child ); }
        else if( name == PoPI_shellChars ) {
            quantity = new PQ_shell( child ); }
        else {
            continue;
        }
        push_back( quantity );
    }
}
/*
=========================================================
*/
PQ_suite::~PQ_suite( ) {

    std::string::size_type i1, __size = size( );

    for( i1 = 0; i1 < __size; ++i1 ) delete (*this)[i1];
}
/*
=========================================================
*/
void PQ_suite::toXMLList( std::vector<std::string> &a_XMLList, std::string const &a_indent1 ) const {

    std::string indent2 = a_indent1 + "  ";

    if( size( ) == 0 ) return;
    std::string header = a_indent1 + "<" + m_label + ">";
    a_XMLList.push_back( header );
    for( std::vector<PhysicalQuantity *>::const_iterator iter = begin( ); iter != end( ); ++iter )
        (*iter)->toXMLList( a_XMLList, indent2 );
    appendXMLEnd( a_XMLList, m_label );
}

}
