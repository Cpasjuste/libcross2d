////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef C2D_RECTANGLESHAPE_HPP
#define C2D_RECTANGLESHAPE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Shape.hpp"

namespace sfml {
////////////////////////////////////////////////////////////
/// \brief Specialized shape representing a rectangle
///
////////////////////////////////////////////////////////////
    class RectangleShape : public c2d::Shape {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// \param size Size of the rectangle
        ///
        ////////////////////////////////////////////////////////////
        explicit RectangleShape(const c2d::Vector2f &size = c2d::Vector2f(0, 0));

        ////////////////////////////////////////////////////////////
        /// \brief Set the size of the rectangle
        ///
        /// \param size New size of the rectangle
        ///
        /// \see getSize
        ///
        ////////////////////////////////////////////////////////////
        void setSize(const c2d::Vector2f &size);

        void setSize(float width, float height);

        ////////////////////////////////////////////////////////////
        /// \brief Get the size of the rectangle
        ///
        /// \return Size of the rectangle
        ///
        /// \see setSize
        ///
        ////////////////////////////////////////////////////////////
        const c2d::Vector2f &getSize() const;

        ////////////////////////////////////////////////////////////
        /// \brief set the local origin of the object
        ///
        /// The origin of an object defines the center point for
        /// all transformations (position, scale, rotation).
        /// The coordinates of this point must be relative to the
        /// top-left corner of the object, and ignore all
        /// transformations (position, scale, rotation).
        /// The default origin of a transformable object is (0, 0).
        ///
        /// \see getOrigin
        ///
        ////////////////////////////////////////////////////////////
        void setOriginTopLeft();

        void setOriginTopRight();

        void setOriginCenter();

        void setOriginBottomLeft();

        void setOriginBottomRight();

        ////////////////////////////////////////////////////////////
        /// \brief Get the number of points defining the shape
        ///
        /// \return Number of points of the shape. For rectangle
        ///         shapes, this number is always 4.
        ///
        ////////////////////////////////////////////////////////////
        virtual std::size_t getPointCount() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get a point of the rectangle
        ///
        /// The returned point is in local coordinates, that is,
        /// the shape's transforms (position, rotation, scale) are
        /// not taken into account.
        /// The result is undefined if \a index is out of the valid range.
        ///
        /// \param index Index of the point to get, in range [0 .. 3]
        ///
        /// \return index-th point of the shape
        ///
        ////////////////////////////////////////////////////////////
        virtual c2d::Vector2f getPoint(std::size_t index) const;

    private:

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        c2d::Vector2f m_size; ///< Size of the rectangle
    };

} // namespace c2d


#endif // C2D_RECTANGLESHAPE_HPP


////////////////////////////////////////////////////////////
/// \class sf::RectangleShape
/// \ingroup graphics
///
/// This class inherits all the functions of sf::Transformable
/// (position, rotation, scale, bounds, ...) as well as the
/// functions of sf::Shape (outline, color, texture, ...).
///
/// Usage example:
/// \code
/// sf::RectangleShape rectangle;
/// rectangle.setSize(sf::Vector2f(100, 50));
/// rectangle.setOutlineColor(sf::Color::Red);
/// rectangle.setOutlineThickness(5);
/// rectangle.setPosition(10, 20);
/// ...
/// window.draw(rectangle);
/// \endcode
///
/// \see sf::Shape, sf::CircleShape, sf::ConvexShape
///
////////////////////////////////////////////////////////////
