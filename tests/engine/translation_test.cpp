/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "catch_amalgamated.hpp"
#include "engine_logger.hpp"
#include "translation.hpp"

using namespace Brenta::Types;
using namespace Brenta::Utils;

TEST_CASE("Translation")
{
    Logger::SetLogLevel(Brenta::Types::LogLevel::DISABLED);
    
    SECTION("Create a Translation")
    {
        Translation translation = Translation();
        REQUIRE(translation.view == glm::mat4(1.0f));
        REQUIRE(translation.projection == glm::mat4(1.0f));
        REQUIRE(translation.model == glm::mat4(1.0f));

        glm::mat4 view = glm::mat4(3.0f);
        glm::mat4 projection = glm::mat4(3.0f);
        glm::mat4 model = glm::mat4(3.0f);
        translation = Translation(view, projection, model);
        REQUIRE(translation.view == view);
        REQUIRE(translation.projection == projection);
        REQUIRE(translation.model == model);
    }

    SECTION("set view")
    {
        Translation translation = Translation();
        translation.setView(glm::mat4(3.0f));
        REQUIRE(translation.view == glm::mat4(3.0f));
    }

    SECTION("set model")
    {
        Translation translation = Translation();
        translation.setModel(glm::mat4(3.0f));
        REQUIRE(translation.model == glm::mat4(3.0f));
    }

    SECTION("translate")
    {
        Translation translation = Translation();
        translation.translate(glm::vec3(1.0f, 2.0f, 3.0f));
        REQUIRE(translation.model == glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f)));
    }

    SECTION("rotate")
    {
        Translation translation = Translation();
        translation.rotate(glm::vec3(1.0f, 2.0f, 3.0f));
        glm::mat4 model = glm::mat4(1.0);
        model = glm::rotate(model, glm::radians(1.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(2.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(3.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f));
        REQUIRE(translation.model == model);
    }

    SECTION("scale")
    {
        Translation translation = Translation();
        translation.scale(2.0f);
        REQUIRE(translation.model == glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)));
    }
}
