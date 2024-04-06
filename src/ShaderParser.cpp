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

#include "ShaderParser.h"

#include <format>
#include <sstream>

std::unordered_set<std::string> ShaderParser::getUniformNames(const std::string &shaderSource) {
    uniformNames.clear();
    structTypes.clear();
    preprocessorVariables.clear();

    bool isInStruct{false};
    std::string currentStructName{};

    std::istringstream shaderSourceStream{shaderSource};

    for (std::string line; std::getline(shaderSourceStream, line, '\n');) {
        const auto trimmedLine{trim(line)};

        if (trimmedLine.empty() or trimmedLine.starts_with("//")) {
            continue;
        }

        if (trimmedLine.starts_with("void main()")) {
            break;
        }

        if (trimmedLine.starts_with("#define")) {
            std::string directive{};
            std::string identifier{};
            std::string value{};

            std::istringstream lineStream{trimmedLine};
            lineStream >> directive;
            lineStream >> identifier;
            lineStream >> value;

            preprocessorVariables[identifier] = std::stoi(value);
        } else if (trimmedLine.starts_with("struct")) {
            isInStruct = true;

            std::istringstream lineStream{trimmedLine};
            std::string qualifier{};
            lineStream >> qualifier;
            // The first token after 'struct' must be the name of the struct.
            lineStream >> currentStructName;
            structTypes[currentStructName] = {};
        } else if (isInStruct and trimmedLine.ends_with("};")) {
            isInStruct = false;
            currentStructName.clear();
        } else if (isInStruct and !trimmedLine.starts_with('{')) {
            std::istringstream lineStream{trimmedLine};
            std::string type{};
            std::string identifier{};
            lineStream >> type;
            lineStream >> identifier;

            if (identifier.ends_with(';')) {
                identifier.pop_back();
            }

            structTypes[currentStructName].push_back(identifier);
        } else if (trimmedLine.starts_with("uniform")) {
            std::istringstream lineStream{trimmedLine};
            std::string qualifier{};
            std::string type{};
            std::string identifier{};

            lineStream >> qualifier;
            lineStream >> type;
            lineStream >> identifier;

            if (identifier.ends_with(';')) {
                identifier.pop_back();
            }

            int arraySize{0};

            if (identifier.ends_with(']')) {
                const auto indexedIdentifier = getIndexedIdentifier(identifier);
                identifier = indexedIdentifier.identifier;
                arraySize = indexedIdentifier.arraySize;
            }

            if (arraySize > 0) {
                for (int i = 0; i < arraySize; ++i) {
                    std::string indexedIdentifier{std::format("{:s}[{:d}]", identifier, i)};

                    insertUniformName(indexedIdentifier, type);
                }
            } else {
                insertUniformName(identifier, type);
            }
        }
    }

    return uniformNames;
}

std::string ShaderParser::trim(const std::string &string, const std::string &whitespaceChars) {
    const auto firstNonWhitespace{string.find_first_not_of(whitespaceChars)};
    const auto lastNonWhitespace{string.find_last_not_of(whitespaceChars)};

    if (firstNonWhitespace == std::string::npos or lastNonWhitespace == std::string::npos) {
        return std::string{};
    }

    return {string.substr(firstNonWhitespace, lastNonWhitespace - firstNonWhitespace + 1)};
}

void ShaderParser::insertStructIdentifiers(const std::string &identifier, const std::vector<std::string> &memberNames) {
    for (const auto &memberName: memberNames) {
        uniformNames.insert(std::format("{}.{}", identifier, memberName));
    }
}

void ShaderParser::insertUniformName(const std::string &identifier, const std::string &type) {
    if (structTypes.contains(type)) {
        insertStructIdentifiers(identifier, structTypes.at(type));
    } else {
        uniformNames.insert(identifier);
    }
}

IndexedIdentifier ShaderParser::getIndexedIdentifier(const std::string &identifier) {
    const auto firstBracketPosition = identifier.find_first_of('[');
    const auto arraySizeStringStart = firstBracketPosition + 1;
    const auto arraySizeStringEnd = identifier.find_last_not_of(']');
    const auto range = arraySizeStringEnd - arraySizeStringStart + 1;
    const auto arraySizeString = identifier.substr(arraySizeStringStart, range);

    int arraySize;

    if (preprocessorVariables.contains(arraySizeString)) {
        arraySize = preprocessorVariables.at(arraySizeString);
    } else {
        arraySize = std::stoi(arraySizeString);
    }

    return {identifier.substr(0, firstBracketPosition), arraySize};
}