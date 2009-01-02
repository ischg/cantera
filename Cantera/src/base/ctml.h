/**
 * @file ctml.h
 *
 * CTML ("Cantera Markup Language") is the variant of XML that Cantera uses
 * to store data. These functions read and write it. 
 * 
 * see also: importCTML, ck2ctml.
 */


/* $Author$
 * $Revision$
 * $Date$
 */

// Copyright 2002  California Institute of Technology

#ifndef CT_CTML_H
#define CT_CTML_H

#include "ct_defs.h"
#include "xml.h"
#include "Array.h"

namespace ctml {

  const std::string CTML_Version = "1.4.1";

  bool isBuiltin(std::string nm);

  void addBool(Cantera::XML_Node& node, 
	       std::string title, 
	       bool val);

  void addInteger(Cantera::XML_Node& node, 
		  std::string title, 
		  int val, 
		  std::string units="", 
		  std::string type="");

  void addFloat(Cantera::XML_Node& node, 
		std::string title, 
		doublereal val, 
		std::string units="", 
		std::string type="", 
		doublereal minval = Cantera::Undef,
		doublereal maxval = Cantera::Undef);

  void addIntegerArray(Cantera::XML_Node& node, 
		       std::string title, 
		       int n, 
		       const int* vals, 
		       std::string units="", 
		       std::string type="",
		       doublereal minval=Cantera::Undef, 
		       doublereal maxval=Cantera::Undef);

  void addFloatArray(Cantera::XML_Node& node, 
		     std::string title, 
		     int n, 
		     const double* vals, 
		     std::string units="", 
		     std::string type="",
		     doublereal minval = Cantera::Undef,
		     doublereal maxval = Cantera::Undef);

  //!  This function adds a child node with the name string with a string value
  //!  to the current node
  /*! 
   *   This function will add a child node to the current XML node, with the
   *   name "string". It will have a title attribute, and the body
   *   of the XML node will be filled out with the valueString argument verbatim.
   *
   *  Example:  
   *
   * Code snipet:
   *       @verbatum
         const XML_Node &node;
	 addString(XML_Node& node, std::string titleString, std::string valueString, 
	           std::string typeString);
   @endverbatum
   *
   *  Creates the following the snippet in the XML file:
   *  @verbatum
     <string title="titleString" type="typeString">
        valueString
     <\string>
   @endverbatum
   *
   *   @param node          reference to the XML_Node object of the parent XML element
   *   @param valueString   Value string to be used in the new XML node.
   *   @param titleString   String name of the title attribute
   *   @param typeString    String type. This is an optional parameter.
   */
  void addString(Cantera::XML_Node& node,  std::string titleString, 
		 std::string valueString, std::string typeString="");

  void getFloatArray(const Cantera::XML_Node& node, Cantera::vector_fp& v, 
		     bool convert=true, std::string type="",
		     std::string nodeName = "floatArray");

  void getStringArray(const Cantera::XML_Node& node, std::vector<std::string>& v);
  void getStringArray(const std::string& val, std::vector<std::string>& v);
  void getMap(const Cantera::XML_Node& node, std::map<std::string, std::string>& m);

  //! This function interprets the value portion of an XML element
  //! as a series of "Pairs" separated by white space.
  /*!
   * Each pair consists of nonwhite-space characters.
   * The first ":" found in the pair string is used to separate
   * the string into two parts. The first part is called the "key"
   * The second part is called the "val".
   * String vectors of key[i] and val[i] are returned in the
   * argument list.
   * Warning: No spaces are allowed in each pair. Quotes are part
   *          of the string.
   *   Example: @verbatum
   *    <xmlNode> 
   red:112    blue:34
   green:banana
   </xmlNode>      @endverbatum
   * 
   * Returns:
   *          key       val
   *     0:   "red"     "112"
   *     1:   "blue"    "34"
   *     2:   "green"   "banana"
   */
  void getPairs(const Cantera::XML_Node& node, std::vector<std::string>& key, 
		std::vector<std::string>& val);

  void getMatrixValues(const Cantera::XML_Node& node, 
		       const std::vector<std::string>& keyString1,
		       const std::vector<std::string>& keyString2,
		       Cantera::Array2D &returnValues, bool convert = true,
		       bool matrixSymmetric = false);

  void getIntegers(const Cantera::XML_Node& node, std::map<std::string,int>& v);
  void getFloats(const Cantera::XML_Node& node, std::map<std::string,double>& v,
		 bool convert=true);

  
  //!  Get a floating-point value from a child element. 
  /*! 
   *  Returns a double value for the child named 'name' of element 'parent'. If
   *  'type' is supplied and matches a known unit type, unit
   *  conversion to SI will be done if the child element has an attribute
   *  'units'.
   *
   *  Note, it's an error for the child element not to exist.
   *
   *  Example:  
   *
   * Code snipet:
   *       @verbatum
         const XML_Node &State_XMLNode;
         doublereal pres = OneAtm;
         if (state_XMLNode.hasChild("pressure")) {
           pres = getFloat(State_XMLNode, "pressure", "toSI");
         }
   @endverbatum
   *
   *  reads the corresponding XML file:
   *  @verbatum
     <state>
        <pressure units="Pa"> 101325.0 </pressure>
     <\state>
   @endverbatum
   *
   *   @param parent reference to the XML_Node object of the parent XML element
   *   @param name   Name of the XML child element
   *   @param type   String type. Currently known types are "toSI" and "actEnergy",
   *                 and "" , for no conversion. The default value is "",
   *                 which implies that no conversion is allowed.
   */
  doublereal getFloat(const Cantera::XML_Node& parent, std::string name,
		      std::string type="");

  //!  Get a floating-point value from a child element with a defined units field
  /*! 
   *  Returns a double value for the child named 'name' of element 'parent'.
   *  'type' must be supplied and match a known unit type.
   *
   *  Note, it's an error for the child element not to exist.
   *
   *  Example:  
   *
   * Code snipet:
   *       @verbatum
         const XML_Node &State_XMLNode;
         doublereal pres = OneAtm;
         if (state_XMLNode.hasChild("pressure")) {
           pres = getFloatDefaultUnits(State_XMLNode, "pressure", "Pa", "toSI");
         }
   @endverbatum
   *
   *  reads the corresponding XML file:
   *  @verbatum
     <state>
        <pressure units="Pa"> 101325.0 </pressure>
     <\state>
   @endverbatum
   *
   *   @param parent reference to the XML_Node object of the parent XML element
   *   @param name   Name of the XML child element
   *   @param defaultUnits Default units string to be found in the units attribute.
   *                 If the units string in the XML field is equal to defaultUnits,
   *                 no units conversion will be carried out.
   *   @param type   String type. Currently known types are "toSI" and "actEnergy",
   *                 and "" , for no conversion. The default value is "",
   *                 which implies that no conversion is allowed.
   */
  doublereal getFloatDefaultUnits(const Cantera::XML_Node& parent, std::string name,
				  std::string defaultUnits, std::string type="toSI");

  //!  Get an integer value from a child element. 
  /*! 
   *  Returns an integer value for the child named 'name' of element 'parent'.
   *
   *  Note, it's an error for the child element not to exist.
   *
   *  Example:  
   *
   * Code snipet:
   *       @verbatum
         const XML_Node &State_XMLNode;
         int number = 1;
         if (state_XMLNode.hasChild("NumProcs")) {
           number = getInteger(State_XMLNode, "numProcs");
         }
   @endverbatum
   *
   *  reads the corresponding XML file:
   *  @verbatum
     <state>
        <numProcs> 10 <numProcs/>
     <\state>
   @endverbatum
   *
   *   @param parent reference to the XML_Node object of the parent XML element
   *   @param name   Name of the XML child element
   */
  int getInteger(const Cantera::XML_Node& parent, std::string name);
    

  void getFunction(const Cantera::XML_Node& node, std::string& type, 
		   doublereal& xmin, doublereal& xmax, Cantera::vector_fp& coeffs);
  Cantera::XML_Node* getByTitle(Cantera::XML_Node& node, std::string title);
  void getString(Cantera::XML_Node& node, std::string title, 
		 std::string& val, std::string& type);

  std::string getString(const Cantera::XML_Node& parent, std::string name);

  // these are defined in ct2ctml.cpp
  void get_CTML_Tree(Cantera::XML_Node* node, std::string file, int debug = 0);

  //! Convert a cti file into a ctml file
  /*!
   *  
   *  @ingroup inputfiles
   */
  void ct2ctml(const char* file, int debug = 0);
}

#endif
