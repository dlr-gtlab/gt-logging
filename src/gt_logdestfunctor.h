// Copyright (c) 2014, Razvan Petru
// Copyright (c) 2014, Omar Carey
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef GT_LOGDESTFUNCTOR_H
#define GT_LOGDESTFUNCTOR_H

#include "gt_logdest.h"

namespace gt
{

namespace log
{

//! Destination sink for various functors. Functor may not be null. Do not
//! access gt::log::Logger from inside LogFunction nor perform any
//! time-consuming operations.
class FunctorDestination : public Destination
{
public:

    /// Logging funcion for detailed messages
    using Functor =
            std::function<void(std::string const&, Level, Details const&)>;

    //! ctor
    explicit FunctorDestination(Functor detail)
        : m_functor(std::move(detail))
    {
        assert(m_functor);
    }

    //! Calls lambda to log formatted text
    void write(std::string const& message, Level level, Details const& details) override
    {
        m_functor(message, level, details);
    }

private:

    /// Functor
    Functor m_functor;
};

inline std::unique_ptr<FunctorDestination>
makeFunctorDestination(FunctorDestination::Functor functor)
{
    return std::make_unique<FunctorDestination>(std::move(functor));
}

} // namespace log

} // namespace gt

#endif // GT_LOGDESTFUNCTOR_H
