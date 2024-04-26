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
// Created by Anthony on 30/03/2024.
//

#ifndef ECONSIMPLUSPLUS_MATERIAL_H
#define ECONSIMPLUSPLUS_MATERIAL_H

namespace EconSimPlusPlus {
    /// The lighting properties of a material.
    struct Material {
        int diffuseTextureUnit;
        int specularTextureUnit;
        float shininess;
    };
} // namespace EconSimPlusPlus

#endif // ECONSIMPLUSPLUS_MATERIAL_H
