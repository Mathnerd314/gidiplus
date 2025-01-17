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
#include <fstream>
#include <iomanip>
#include <string>

#include <HAPI.hpp>

using std::cout;
using std::endl;

int main( int argc, char **argv ) {

// FIXME (Caleb Mattoon)
// #ifndef HAPI_USE_HDF5
#if 1
    std::cerr << "    " << "parseHDF5 - no opt" << std::endl;
#else
  try {
    std::cerr << "    " << "parseHDF5" << std::endl;

    std::string protareFilename( "../sampleFile.h5" );

    HAPI::File *file = new HAPI::HDFFile( protareFilename.c_str() );
    HAPI::Node protare = file->first_child();

    cout << "Start reading:" << endl;
    cout << "projectile: " << protare.attribute("projectile").value() << ", ";
    cout << "target: " << protare.attribute("target").value() << endl;

    HAPI::Node style = protare.child("styles").first_child();
    cout << "contains styles: " << style.name();
    for (style = style.next_sibling(); !style.empty(); style.to_next_sibling() )
    {
        cout << ", " << style.name();
    }
    cout << endl;

    HAPI::Node documentation = protare.child("documentations").child("documentation");
    HAPI::Text doc_text = documentation.text();
    cout << "documentation: '" << doc_text.get() << "'" << endl;

    HAPI::Node reaction = protare.child("reactions").first_child();
    cout << "reaction " << reaction.attribute("label").value() << ":" << endl;
    HAPI::Node xys1d = reaction.child("crossSection").first_child();
    HAPI::Node values = xys1d.child("values");
    HAPI::Data data = values.data();
    nf_Buffer<double> xscData;
    data.getDoubles(xscData);

    cout << "  cross section points: ";
    for (int idx = 0; idx < data.length(); idx++)
        cout << " " << xscData[idx];
    cout << endl;

    HAPI::Node Qform = reaction.child("outputChannel").child("Q").first_child();
    cout << "  Q-value: " << Qform.name() << " value = " << Qform.attribute("constant").as_int();
    cout << " over range " << Qform.attribute("domainMin").as_double() << " - " << Qform.attribute("domainMax").as_double();
    cout << " " << Qform.child("axes").first_child().attribute("unit").value() << endl;

    HAPI::Node products = reaction.child("outputChannel").child("products");
    cout << "  products:" << endl;
    for (HAPI::Node product = products.first_child(); !product.empty(); product.to_next_sibling())
    {
        cout << "    " << product.attribute("pid").value();
        cout << " multiplicity " << product.child("multiplicity").child("constant1d").attribute("constant").value();
        HAPI::Node distForm = product.child("distribution").first_child();
        cout << " distribution " << distForm.name() << "." << distForm.first_child().name() << endl;
    }

    cout << endl << "Create empty node:";
    HAPI::Node emptyNode;
    cout << "         name = '" << emptyNode.name() << "', empty = " << emptyNode.empty() << endl;

    cout << "Access non-existent node:";
    HAPI::Node noSuchNode = protare.child("noSuchNode");
    cout << "  name = '" << noSuchNode.name() << "', empty = " << noSuchNode.empty() << endl;

    cout << "Access non-existent attr:";
    HAPI::Attribute noSuchAttr = protare.attribute("noSuchAttribute");
    cout << "  name = 'noSuchAttribute', value = '" << noSuchAttr.value() << "'" << endl;

    delete file;
  }
  catch (char const *str) {
    cout << str << endl;
    exit( EXIT_FAILURE );
  }
#endif
}
