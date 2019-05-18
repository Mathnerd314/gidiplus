/*
# <<BEGIN-copyright>>
# Copyright 2019, Lawrence Livermore National Security, LLC.
# See the top-level COPYRIGHT file for details.
# 
# SPDX-License-Identifier: MIT
# <<END-copyright>>
*/

#include "GIDI.hpp"

namespace GIDI {

/*! \class ThermalNeutronScatteringLaw1d
 * Class for the GNDS <**thermalNeutronScatteringLaw**> node.
 */

/* *********************************************************************************************************//**
 *
 * @param a_construction    [in]    Used to pass user options to the constructor.
 * @param a_node            [in]    The **pugi::xml_node** to be parsed and used to construct the XYs2d.
 * @param a_parent          [in]    The parent GIDI::Suite.
 ***********************************************************************************************************/

ThermalNeutronScatteringLaw1d::ThermalNeutronScatteringLaw1d( Construction::Settings const &a_construction, pugi::xml_node const &a_node, Suite *a_parent ) :
        Function1dForm( a_construction, a_node, f_thermalNeutronScatteringLaw1d, a_parent ),
        m_href( a_node.attribute( hrefAttribute ).value( ) ) {

}

/* *********************************************************************************************************//**
 ***********************************************************************************************************/

ThermalNeutronScatteringLaw1d::~ThermalNeutronScatteringLaw1d( ) {

}

/* *********************************************************************************************************//**
 * Returns the domain minimum for the instance.
 *
 * @return          The domain minimum for the instance.
 ***********************************************************************************************************/

double ThermalNeutronScatteringLaw1d::domainMin( ) const {

    return( 0.0 );              // FIXME
}

/* *********************************************************************************************************//**
 * Returns the domain maximum for the instance.
 *
 * @return              The domain maximum for the instance.
 ***********************************************************************************************************/

double ThermalNeutronScatteringLaw1d::domainMax( ) const {

    return( 1.0 );              // FIXME
}

/* *********************************************************************************************************//**
 * Returns the value of the function *f(x1)* at the specified point and *a_x1*.
 * **This is currently not implemented**.
 *
 * @param a_x1              [in]    The value of the **x1** axis.
 * @return                          The value of the function evaluated at *a_x1*.
 ***********************************************************************************************************/

double ThermalNeutronScatteringLaw1d::evaluate( double a_x1 ) const {

    throw std::runtime_error( "ThermalNeutronScatteringLaw1d::evaluate: not implemented" );
}

}