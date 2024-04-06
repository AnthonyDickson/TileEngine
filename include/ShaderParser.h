//  EconSimPlusPlus
//  A program that simulates the historical development of capitalist economies.
//  Copyright (C) 2024.   Anthony Dickson anthony.dickson9656@gmail.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by Anthony on 6/04/2024.
//

#ifndef ECONSIMPLUSPLUS_SHADERPARSER_H
#define ECONSIMPLUSPLUS_SHADERPARSER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/** A GLSL array type identifier. */
struct IndexedIdentifier {
    /** The identifier for a variable in a GLSL shader. */
    std::string identifier;
    /** How many array slots the variable has declared. */
    int arraySize;
};

/** Parses GLSL shader source code. */
class ShaderParser {
private:
    /** The names of the uniform variables in a GLSL shader. */
    std::unordered_set<std::string> uniformNames{};
    /** Mapping between the name of user-defined structs and their member identifiers. */
    std::unordered_map<std::string, std::vector<std::string>> structTypes{};
    /** Mapping between preprocessor definitions and their integer values. */
    std::unordered_map<std::string, int> preprocessorVariables{};
public:
    /**
     * Parses the shader source code and extracts the names of uniform variables.
     *
     * @note Assumes that your source code has been compiled (i.e. it is valid GLSL), has no whitespace (except for
     * indentation), the code is formatted according to standard conventions, and all uniform and struct declarations
     * are placed before the main function.
     * @param shaderSource The shader source code as a string.
     * @return A set of uniform variable names.
     */
    std::unordered_set<std::string> getUniformNames(const std::string &shaderSource);

private:
    /**
     * Trim leading and trailing whitespace from a string.
     * @param string The string to trim.
     * @param whitespaceChars The characters to consider as whitespace.
     * @return The trimmed copy of the string.
     */
    static std::string trim(const std::string &string, const std::string &whitespaceChars = " \t");

    /**
     * Helper function to extract the array size and identifier from an array type declaration.
     * @param identifier The identifier with the array size declaration.
     * @return A struct containing the identifier without the array size declaration and the array size.
     */
    IndexedIdentifier getIndexedIdentifier(const std::string &identifier);

    /**
     * Helper function that inserts the given identifier into the uniform names member.
     * @param identifier The identifier of a uniform variable.
     * @param type The type of the uniform variable.
     */
    void insertUniformName(const std::string &identifier, const std::string &type);

    /**
     * Helper function that inserts the names of struct members into the uniform names member.
     * @param identifier The identifier of a struct type uniform variable.
     * @param memberNames The name of the struct members.
     */
    void insertStructIdentifiers(const std::string &identifier, const std::vector<std::string> &memberNames);
};


#endif //ECONSIMPLUSPLUS_SHADERPARSER_H
